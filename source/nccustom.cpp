
#include "ncwin.h"
#include <algorithm>

void custom_command1(int *cstate,HighLevelEvent *e,void **)
{int commandid;
  switch (*cstate)
    {case InitialState :
       *cstate = 1;
       cadwindow->prompt->normalprompt(NCCUSTOM);
       state.selmask.entity.clearandset(end_list);
       break;
     case 1 :
       if (e->isa(NewCommand) && (commandid = ((NewCommandEvent *)e)->getcommandid()) >= 0)
         cadwindow->subwindow->setclickcommandid(commandid);
       *cstate = ExitState;
    }
}


/**
 * @brief custom_command12
 * @param cstate
 *
 * Entry point for the load application command
 *
 */
#ifdef USING_WIDGETS
void custom_command12(int *cstate,HighLevelEvent *,void **)
{
 OFSTRUCT ll;
 RCHINSTANCE appinstance;
 RC_finddata_t fdfile;
 int handle;
 RCCHAR appname[300],*namel[100],*pathl[100];
 int i,napps,app,finished;
 Dialog dialog("SelectApplication_Dialog");
 //RCCHAR *(WINAPI *getcadappname)(void);
 char *(WINAPI *getcadappname)(void);
 //typedef RCCHAR *(WINAPI *getcadappname)(void);

  RCCHAR pluginType[10];
  RCCHAR nameFilter[10];

#ifdef _MAC
  strcpy(pluginType,"dylib");
#else
  strcpy(pluginType,"dll");
#endif

  QStringList excludeList;
#ifndef _MAC
  excludeList << "opengl32sw.dll";
#endif

  strcpy(nameFilter,"*.");
  strcat(nameFilter,pluginType);

  qDebug() << "home.getApplicationPath() : " << QString(home.getApplicationPath());
  qDebug() << "home.getApplicationDir() : " << QString(home.getApplicationDir());

  QDir dllFolder(QString(home.getApplicationDir()));
  qDebug() << "dllfolder : " << dllFolder;
  //handle = _findfirst(home.getexecutablefilename(_RCT("*.dll")),&fdfile);
  QStringList filterlist;
  filterlist << QString(nameFilter);
  QStringList libnamelist = dllFolder.entryList(filterlist);
  //QStringList libnamelist = dllFolder.entryList();
  napps = 0;

  // some qt dlls have been found to hang when unload is called
  // we have an exclude list for those
  // remove any dll on the exclude list here
  for(i=0; i<excludeList.size(); i++)
  {
      int index = libnamelist.indexOf(excludeList.at(i));
      if(index >= 0)
          libnamelist.removeAt(index);
  }

  //SetDllDirectory(home.getexecutablefilename(_RCT("dlls")));
  SetDllDirectory(home.getApplicationDir());

  //while (handle != -1)
  for(i=0; i<libnamelist.size(); i++)
  {
       QString libname = libnamelist.at(i);
       //if (OpenFile(home.getexecutablefilename(_RCT(fdfile.name)),&ll,OF_EXIST) != HFILE_ERROR)
       {
            qDebug() << "library : " << QString(home.getexecutablefilename(libname.data()));
            QLibrary *appinstance = new QLibrary(QString(home.getexecutablefilename(libname.data())),app);
            qDebug() << "appinstace : " << appinstance;
            //appinstance->setLoadHints(QLibrary::ResolveAllSymbolsHint);

            //SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
            //if ((appinstance = LoadLibrary(home.getexecutablefilename(_RCT(fdfile.name)))) != 0)
            if(appinstance && appinstance->load())
            {
                 qDebug() << "library loaded ok: " << QString(home.getexecutablefilename(libname.data()));
                 //getcadappname = (RCCHAR *(WINAPI *)(void))GetProcAddress(appinstance,"getcadappname");
                 //getcadappname = (RCCHAR *(WINAPI *)(void))appinstance->resolve();
                 getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
                 if (getcadappname != NULL)
                 {
                      qDebug() << "library app name is: " << getcadappname();
                      strcpy(appname,getcadappname());
                      namel[napps] = new RCCHAR[strlen(appname)+1];
                      strcpy(namel[napps],appname);
                      pathl[napps] = new RCCHAR[strlen(home.getexecutablefilename(libname.data()))+1];
                      strcpy(pathl[napps],home.getexecutablefilename(libname.data()));
                      qDebug() << "saved app path : " << QString(pathl[napps]);
                      napps++;
                 }
                 else
                     qDebug() << "no appname function found: ";

                 //FreeLibrary(appinstance);
                 appinstance->unload();
                 if(appinstance->unload())
                     appinstance->deleteLater();
                 appinstance = 0;
            }
            else
            {
                //qDebug() << "library did not load : " << QString(home.getexecutablefilename(libname.data()));
                //qDebug() << "QLibrary error string : " << appinstance->errorString();
                qInfo() << "library did not load : " << QString(home.getexecutablefilename(libname.data()));
                qInfo() << "QLibrary error string : " << appinstance->errorString();
            }
       }
  }
  app = 0;
  dialog.add(new ListDialogControl(100,napps,namel,&app));
  if (dialog.process() == TRUE)
  {
       qDebug() << "load path : " << QString(pathl[app]);
       program->loadapp(pathl[app]);
       for (i = 0 ; i < napps ; i++)
       {
            delete [] namel[i];
            delete [] pathl[i];
       }
  }

  *cstate = ExitState;
}
#else
void custom_command12(int *cstate,HighLevelEvent *,void **)
{
#if (VERSION != FLEXICAD)
#if defined(_MSC_VER) && ! defined(_WIN32_WCE) || defined(_MAC)
 OFSTRUCT ll;
 RCHINSTANCE appinstance;
 RC_finddata_t fdfile;
 int handle;
 RCCHAR appname[300],*namel[100],*pathl[100];
 int i,napps,app,finished;
 Dialog dialog("SelectApplication_Dialog");
 //RCCHAR *(WINAPI *getcadappname)(void);
 typedef RCCHAR *(WINAPI *getcadappname)(void);

  handle = _findfirst(home.getexecutablefilename(_RCT("*.dll")),&fdfile);
  napps = 0;


  SetDllDirectory(home.getexecutablefilename(_RCT("dlls")));

  while (handle != -1)
    {
       QLibrary myLib("mylib");
       //if (OpenFile(home.getexecutablefilename(_RCT(fdfile.name)),&ll,OF_EXIST) != HFILE_ERROR)
       {
            //SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
            if ((appinstance = LoadLibrary(home.getexecutablefilename(_RCT(fdfile.name)))) != 0)
              {  getcadappname = (RCCHAR *(WINAPI *)(void))GetProcAddress(appinstance,"getcadappname");
                 if (getcadappname != NULL)
                   {  strcpy(appname,getcadappname());
                      namel[napps] = new RCCHAR[strlen(appname)+1];
                      strcpy(namel[napps],appname);
                      pathl[napps] = new RCCHAR[strlen(home.getexecutablefilename(_RCT(fdfile.name)))+1];
                      strcpy(pathl[napps],home.getexecutablefilename(_RCT(fdfile.name)));
                      napps++;
                   }
                 FreeLibrary(appinstance);
                          }
       }
       if (_findnext(handle,&fdfile) != 0)
         {  _findclose(handle);
            handle = -1;
            finished = 1;
         }
    }
  app = 0;
  dialog.add(new ListDialogControl(100,napps,namel,&app));
  if (dialog.process() == TRUE)
    {  program->loadapp(pathl[app]);
       for (i = 0 ; i < napps ; i++)
         {  delete [] namel[i];
            delete [] pathl[i];
         }
    }
#else
#if ! defined(_WIN32_WCE)  //  WCEFIX
 OFSTRUCT ll;
 RCHINSTANCE appinstance;
 struct ffblk ffblk;
 RCCHAR appname[300],*namel[100],*pathl[100];
 int i,napps,app,finished;
 Dialog dialog("SelectApplication_Dialog");
 RCCHAR *(WINAPI *getcadappname)(void);
  finished = findfirst(home.getexecutablefilename("*.dll"),&ffblk,0);
  napps = 0;
  while (! finished)
    {  if (OpenFile(home.getexecutablefilename(ffblk.ff_name),&ll,OF_EXIST) != HFILE_ERROR)
         {  if ((appinstance = LoadLibrary(home.getexefilename(ffblk.ff_name))) != 0)
              {  (FARPROC) getcadappname = GetProcAddress(appinstance,"getcadappname");
                 if (getcadappname != NULL)
                   {  strcpy(appname,getcadappname());
                      namel[napps] = new RCCHAR[strlen(appname)+1];
                      strcpy(namel[napps],appname);
                      pathl[napps] = new RCCHAR[strlen(home.getexecutablefilename(ffblk.ff_name))+1];
                      strcpy(pathl[napps],home.getexecutablefilename(ffblk.ff_name));
                      napps++;
                   }
                 FreeLibrary(appinstance);
                  }
         }
       finished = findnext(&ffblk);
    }
  app = 0;
  dialog.add(new ListDialogControl(100,napps,namel,&app));
  if (dialog.process() == TRUE)
    {  program->loadapp(pathl[app]);
       for (i = 0 ; i < napps ; i++)
         {  delete [] namel[i];
            delete [] pathl[i];
         }
    }
#endif
#endif
#endif
  *cstate = ExitState;
}
#endif
//
//  Custom menu commands
//

CustomMenuActive custommenuactive;

void CustomMenuActive::setactive(int a)
{ active = a;
  v.setinteger("cm::active",a);
  if (active == 0)
    custommenuwindow = 0;
}


void CustomMenuActive::fixup(void)
{
 // MenuWindow *mw;

//  for (cadwindow->floatingmenulist.start() ; (mw = (MenuWindow *)cadwindow->floatingmenulist.next()) != 0 ; )
  //  if (mw->getcontrol(0) == buttonmenus.match("Custom"))
    //     mw->paintevent();
     // }


  buttonmenus.match("Custom")->fixup(buttonmenus.match("Custom")->parent);
  buttonmenus.match("Custom")->parent->paintevent();

}

CustomMenuWindow::CustomMenuWindow(Window *win,int x1,int y1,int w,int h,CustomMenuActive *aa) :
  MenuWindow(win,x1,y1,w,h)
{int x2,y2,xm,ym,wm,hm,mf;
 RCCHAR text[300];

  custommenuactive = aa;
  buttonmenu = custommenuactive->getbuttonmenu();
  custommenuactive->setactive(1);

  //  Try to dock Custom Menu

  x2 = x1 >= 0 ? x1 : int(cadwindow->getsubwindow()->getwidth() + x1);
  y2 = y1 >= 0 ? y1 : int(cadwindow->getsubwindow()->getheight() + y1);

  GetPrivateProfileString("Layout","Custom","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5)
    {  if (mf == 1)
         {  addcontrol(buttonmenu);
            buttonmenu->move(this,0,0,buttonmenu->getncolumns());
            SetWindowPos(gethwnd(),0,xm,ym,49,48+GetSystemMetrics(SM_CYCAPTION),SWP_NOSIZE | SWP_NOZORDER);
         }
       else
         {   cadwindow->delfloatingmenuwindow(this);
             ShowWindow(hWnd,SW_HIDE);
             buttonmenu->move(cadwindow,xm,ym,buttonmenu->getncolumns());
             cadwindow->sizeevent(-1,-1);
         }
    }
  else
    {  addcontrol(buttonmenu);
       buttonmenu->move(this,0,0,buttonmenu->getncolumns());
    }

}

CustomMenuWindow::~CustomMenuWindow()
{ custommenuactive->setactive(0);
}

//
//  Add button to custom menu
//

void custom_command13(int *cstate,HighLevelEvent *e,void **)
{int commandid;
  switch (*cstate)
    {case InitialState :
       if (buttonmenus.match("Custom") != 0)
         {  *cstate = 1;
            cadwindow->prompt->normalprompt(NCCUSTOM+1);
            state.selmask.entity.clearandset(end_list);
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (e->isa(NewCommand) && (commandid = ((NewCommandEvent *)e)->getcommandid()) >= 0)
         {Button *b;
            b = buttonmenus.match(commandid);
            if (b != 0)
              {
                 ButtonMenu *bm=0;
                 bm = buttonmenus.match("Custom");
                 bm->append(new Button(-1,-1,b,commandid));
                 buttonmenus.match("Custom")->store((char*)"Custom");
                 bm->move(cadwindow,0,0,-1);
                 if(bm->getQW() == 0)
                     bm->setToolbox(custommenuactive.getwindow(),0);
                 else
                 {
                     RCToolBox *rctb = custommenuactive.getwindow();
                     rctb->setup(rctb->getLayoutType(),bm->getncolumns(),bm->getnbuttons(),0);
                     bm->setToolbox(rctb,0);
                 }
                 custommenuactive.getbuttonmenu()->setorientation(custommenuactive.getbuttonmenu()->getorientation());
                 custommenuactive.getwindow()->adjustSize();
                 custommenuactive.getwindow()->show();
                 custommenuactive.getwindow()->saveState();
              }
         }
       else if (e->isa(Abort) || e->isa(Exit))
         *cstate = ExitState;
    }
}

//
//  Remove button from custom menu
//

void custom_command14(int *cstate,HighLevelEvent *e,void **)
{int commandid;
  switch (*cstate)
    {case InitialState :
       if (buttonmenus.match("Custom") != 0)
         {  *cstate = 1;
            cadwindow->prompt->normalprompt(NCCUSTOM+2);
            state.selmask.entity.clearandset(end_list);
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (e->isa(NewCommand) && (commandid = ((NewCommandEvent *)e)->getcommandid()) >= 0 && commandid != 5006 && commandid != 5008)
         {Button *b;
            b = buttonmenus.match("Custom")->match(commandid);
            if (b != 0)
              {
                 ButtonMenu *bm = custommenuactive.getbuttonmenu();
                 RCToolBox *rctb = custommenuactive.getwindow();
                 rctb->replaceButton((RCToolButton*)b->getQW(),0);
                 buttonmenus.match("Custom")->del(b);
                 buttonmenus.match("Custom")->store((char*)"Custom");
                 rctb->setup(rctb->getLayoutType(),bm->getncolumns(),bm->getnbuttons(),0);
                 bm->setToolbox(rctb,0);
                 custommenuactive.getbuttonmenu()->setorientation(custommenuactive.getbuttonmenu()->getorientation());
                 custommenuactive.getwindow()->adjustSize();
                 custommenuactive.getwindow()->show();
                 custommenuactive.getwindow()->saveState();
                 // if there are no buttons left set the custom menu to inactive
                 if(bm->getnusedbuttons() == 0)
                     v.setinteger("cm::active",0);
              }
         }
       else if (e->isa(Abort) || e->isa(Exit))
         *cstate = ExitState;
    }
}

//
//  Create custom menu (vertical)
//

void custom_command15(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
    ButtonMenu *menu=0;
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  if (v.getinteger("cm::active") != 0)
    { RCMessageBox(cadwindow->gethwnd(),"A custom menu is already active","Custom Menu",MB_OK);
       return;
    }
  if (custommenuactive.getbuttonmenu() == NULL)
    {
      menu = new ButtonMenu(0,0,1,5,15,(QtMainWindow*)cadwindow->gethwnd());
       if (menu != NULL)
         {
            menu->setorientation(1);
            menu->load((char*)"Custom");
            buttonmenus.add("Custom",menu);
            custommenuactive.setbuttonmenu(menu);
            custommenuactive.setactive(1);
         }
    }

  if (custommenuactive.getbuttonmenu() == NULL)
    {  cadwindow->MessageBox("Failed to create the custom menu button menu","Custom menu",MB_ICONSTOP);
       return;
    }
  if (custommenuactive.getbuttonmenu() != NULL)
    custommenuactive.getbuttonmenu()->setorientation(1);
  v.setinteger("cm::orientation",1);

//  new CustomMenuWindow(cadwindow->getsubwindow(),
//                       int(cadwindow->getsubwindow()->getwidth())-(24*2+GetSystemMetrics(SM_CXDLGFRAME)*3),0,
//                       24,24*10,&custommenuactive);

  RCToolBox::layoutType layouttype = RCToolBox::vLine;
  Qt::Orientation orentation = Qt::Vertical;
  RCToolBox *rctb=0;
  rctb = new RCToolBox(layouttype,1,1,0,(QtMainWindow*)cadwindow->gethwnd());
  if (v.getinteger("cm::active") != 0)
      rctb->setup(rctb->getLayoutType(),menu->getncolumns(),menu->getnbuttons(),0);
  rctb->setTitle(QString("Custom"));
  rctb->setRole(QString("Custom"));
  custommenuactive.setwindow(rctb);
  if(menu !=0)
    menu->setToolbox(rctb,0);
  QByteArray wstate = settings.value("WindowPlacement/Main","").toByteArray();
  bool restored = ((QtMainWindow*)cadwindow->gethwnd())->restoreDockWidget(rctb);
  if(!restored)
  {
      restored = rctb->restoreGeometry(wstate);
  }
  else
      ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::NoDockWidgetArea,(QDockWidget*)menu->getQW(),orentation);
  // reset the layout just to be sure
  rctb->resetLayout(layouttype);
  rctb->setFloating(true);
  rctb->move(((QtMainWindow*)cadwindow->gethwnd())->centralWidget()->rect().center());
  rctb->show();
  rctb->raise();
  rctb->saveState();

  *cstate = ExitState;
}

//
//  Create custom menu (horizontal)
//

void custom_command19(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
    ButtonMenu *menu=0;
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  if (v.getinteger("cm::active") != 0)
    { RCMessageBox(cadwindow->gethwnd(),"A custom menu is already active","Custom Menu",MB_OK);
       return;
    }
  if (custommenuactive.getbuttonmenu() == NULL)
    {  menu = new ButtonMenu(0,0,5,1,15,(QtMainWindow*)cadwindow->gethwnd());
       if (menu != NULL)
         {
            menu->setorientation(1);
            menu->load((char*)"Custom");
            buttonmenus.add("Custom",menu,"Custom");
            custommenuactive.setbuttonmenu(menu);
            custommenuactive.setactive(1);
         }
    }
  if (custommenuactive.getbuttonmenu() == NULL)
    {  cadwindow->MessageBox("Failed to create the custom menu button menu","Custom menu",MB_ICONSTOP);
       return;
    }
//  custommenuactive.setwindow(new CustomMenuWindow(cadwindow->getsubwindow(),
//                               int(cadwindow->getsubwindow()->getwidth())-(24*2+GetSystemMetrics(SM_CXDLGFRAME)*3),0,
//                               24,24*10,&custommenuactive));
  if (custommenuactive.getbuttonmenu() != NULL)
    custommenuactive.getbuttonmenu()->setorientation(2);
  v.setinteger("cm::orientation",2);

//  new CustomMenuWindow(cadwindow->getsubwindow(),
//                       int(cadwindow->getsubwindow()->getwidth())-(24*2+GetSystemMetrics(SM_CXDLGFRAME)*3),0,
//                       /*custommenuactive.getbuttonmenu()->nused*/10 * 24,24,&custommenuactive);

  RCToolBox::layoutType layouttype = RCToolBox::hLine;
  Qt::Orientation orentation = Qt::Horizontal;
  RCToolBox *rctb=0;
  rctb = new RCToolBox(layouttype,1,1,0,(QtMainWindow*)cadwindow->gethwnd());
  if (v.getinteger("cm::active") != 0)
      rctb->setup(rctb->getLayoutType(),menu->getncolumns(),menu->getnbuttons(),0);
  rctb->setTitle(QString("Custom"));
  rctb->setRole(QString("Custom"));
  custommenuactive.setwindow(rctb);
  if(menu !=0)
     menu->setToolbox(rctb,0);
  QByteArray wstate = settings.value("WindowPlacement/Main","").toByteArray();
  bool restored = ((QtMainWindow*)cadwindow->gethwnd())->restoreDockWidget(rctb);
  if(restored)
      restored = rctb->restoreGeometry(wstate);
  else
      ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::AllDockWidgetAreas,(QDockWidget*)menu->getQW(),orentation);
  // reset the layout just to be sure
  rctb->resetLayout(layouttype);
  rctb->setFloating(true);
  rctb->move(((QtMainWindow*)cadwindow->gethwnd())->centralWidget()->rect().center());
  rctb->raise();
  rctb->saveState();

  *cstate = ExitState;
}

void custom_command16(int *cstate,HighLevelEvent *,void **)
{
#if VERSION != TRICAD_STUDENT
  CommandWindow *cw;
  if (cadwindow->getthecommandwindow() == 0)
    {  cadwindow->setthecommandwindow(cw = new CommandWindow(cadwindow->getsubwindow(),int(cadwindow->getsubwindow()->getwidth() / 2),int(cadwindow->getsubwindow()->getheight() / 2)));
       cadwindow->selectwindow(cw);
    }
#endif
  *cstate = ExitState;
}

class APPListDialogControl : public ListDialogControl
  {public:
     APPListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void APPListDialogControl::load(Dialog *dialog)
{
  RCCHAR name[300];
  int i;
  ListDialogControl::load(dialog);
  for (i = 0 ; program->getloadedappname(i,name) != 0 ; i++)
    ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,name);
}

/**
 * @brief custom_command18
 * @param cstate
 *
 * Entry point for the unload application command
 *
 */
void custom_command18(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("SelectUnloadApplication_Dialog");
 RCCHAR name[300];

  strcpy(name,"");
  dialog.add(new APPListDialogControl(100,name,300));
  if (dialog.process() == TRUE)
    program->unloadapp(name);

  *cstate = ExitState;

}

void layerwindow_command(int *cstate,HighLevelEvent *,void **)
{LayerWindow *lw;
  if (cadwindow->getlayerwindow() == nullptr)
    cadwindow->setlayerwindow(lw = new LayerWindow(cadwindow,int(cadwindow->getsubwindow()->getwidth() / 5),int(cadwindow->getsubwindow()->getheight() / 2)));
  *cstate = ExitState;
}

void SaveAcceleratorTable(void)
{
 int i,naccel;
 //RCVACCEL *accel;
 RCCHAR key[300],value[300];

  // delete the values in the .ini file
  {
     QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
     settings.remove("Accelerators");
     settings.sync();
  }

  RCVACCELITER it;
  //std::sort(program->gethaccel()->begin(),program->gethaccel()->end());
  // get the length of the accel list
  naccel = (int)program->gethaccel()->size();
  //naccel = CopyAcceleratorTable(program->gethaccel(),nullptr,0);
  // copy db table with the copy constructor
  RCVACCEL accel(*program->gethaccel());
  //CopyAcceleratorTable(program->gethaccel(),&accel,naccel);


  for (i=0,it=accel.begin() ; i < naccel ; i++,++it)
    {  sprintf(key,"Accelerator%d",i+1);
       if (i < naccel)
         sprintf(value,"%d,%d,%d",((RCACCEL)*it).fVirt,((RCACCEL)*it).key,((RCACCEL)*it).cmd);
       else
         strcpy(value,"");
       WritePrivateProfileString(_RCT("Accelerators"),key,value,home.getinifilename());
    }
}

void LoadAcceleratorTable(void)
{int i,naccel,akey,fvert,cmd;
 //RCVACCEL *accel;
 RCHACCEL haccel;
 RCCHAR key[300],value[300];
 RCVACCELITER it;

  for (naccel = 0 ;  ; naccel++)
    {  sprintf(key,"Accelerator%d",naccel+1);
       GetPrivateProfileString(_RCT("Accelerators"),key,_RCT(""),value,300,home.getinifilename());
       if (strlen(value) == 0)
         break;
    }

  if (naccel > 0)
    {
       //accel = new RCVACCEL(naccel);
       RCVACCEL accel(naccel);
       //for (i=0,it=accel->begin() ; i < naccel ; i++,++it)
       for (i=0,it=accel.begin() ; i < naccel ; i++,++it)
         {  sprintf(key,"Accelerator%d",i+1);
            GetPrivateProfileString(_RCT("Accelerators"),key,_RCT(""),value,300,home.getinifilename());
            sscanf(value,"%d,%d,%d",&fvert,&akey,&cmd);
            RCACCEL ac = *it;
            ac.fVirt = (BYTE)fvert;
            if (akey >= Qt::Key_F1/*VK_F1*/ && akey <= Qt::Key_F12/*VK_F12*/)
              ac.fVirt |= FVIRTKEY;
            ac.key = akey;
            ac.cmd = cmd;
            // built the kstring
            if((ac.fVirt & FSHIFT) != 0)
                ac.kstring = "Shift+";
            if((ac.fVirt & FALT) != 0)
                ac.kstring = "Alt+";
            if((ac.fVirt & FCONTROL) != 0)
                ac.kstring = "Ctrl+";
            if((ac.fVirt & FVIRTKEY) != 0)
                ac.kstring += QKeySequence(ac.key).toString();//QString(ac.key);
            *it = ac;
         }

       haccel = CreateAcceleratorTable(&accel,naccel);
       if (haccel != 0)
         {  DestroyAcceleratorTable(program->gethaccel());
            program->sethaccel(haccel);
         }

    }

}


class KSCListDialogControl : public ListDialogControl
  {private:
   public:
     KSCListDialogControl(int id,RCCHAR *sc,int l) :
         ListDialogControl(id,0,(RCCHAR**)0)  {    }
     void load(Dialog *);
  };

#define KEYLIST_SIZE 55 // size of this array is 55
static const char *keylist[] = {   "A","B","C","D","E","F","G","H","I","J","K","L","M","N",        //#14
                             "O","P","Q","R","S","T","U","V","W","X","Y","Z",                //#12
                             "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12", //#12
                             "Del","End","Home","Ins","Pg Up","Pg Dn","Tab",                 //# 7 not used??
                             "0","1","2","3","4","5","6","7","8","9"                         //#10 not used??
                         };                                                                  //T55

RCCHAR *FormatAccelerator(RCACCEL *accel,RCCHAR *buffer)
{
  const char *key=nullptr;
  if (accel->key >= 'A' && accel->key <= 'Z')
    key = keylist[accel->key - 'A'];
  else if (accel->key >= Qt::Key_F1 /*VK_F1*/ && accel->key <= Qt::Key_F12/*VK_F12*/)
    key = keylist[26+accel->key-Qt::Key_F1/*VK_F1*/];
  else if (accel->key == Qt::Key_Delete /*VK_DELETE*/)
    key = keylist[38];
  else if (accel->key == Qt::Key_End /*VK_END*/)
    key = keylist[39];
  else if (accel->key == Qt::Key_Home /*VK_HOME*/)
    key = keylist[40];
  else if (accel->key == Qt::Key_Insert /*VK_INSERT*/)
    key = keylist[41];
  else if (accel->key == Qt::Key_PageDown /*VK_NEXT*/)
    key = keylist[42];
  else if (accel->key == Qt::Key_PageUp /*VK_PRIOR*/)
    key = keylist[43];
  else if (accel->key == Qt::Key_Tab /*VK_TAB*/)
    key = keylist[44];
  else if (accel->key >= '0' && accel->key <= '9')
    key = keylist[45 + accel->key - '0'];
  else
    key = (char*)"?";

  qDebug() << "key : " << QString(key);
  qDebug() << "key.tolatin1() : " << QString(key).toLatin1();
  QString descr = QString(commands.description(accel->cmd)).toLatin1().data();
  // remove any html tags from the command description
  descr = descr.remove("<b>");
  descr = descr.remove("</b>");
  descr = descr.replace("<hr>",",");

  sprintf(buffer,"%s%s%s:%s",accel->fVirt&FALT ? "Alt-" : "",accel->fVirt&FCONTROL ? "Ctrl-" : "",QString(key).toLatin1().data(),descr.toLatin1().data());

  return buffer;

}

void KSCListDialogControl::load(Dialog *dialog)
{
  int i,naccel;
  //RCVACCEL *accel;
  RCCHAR label[300]={0};
  RCVACCELITER it;

  naccel = CopyAcceleratorTable(program->gethaccel(),nullptr,0);
  //accel = new RCVACCEL(naccel);
  RCVACCEL accel(naccel);
  CopyAcceleratorTable(program->gethaccel(),&accel,naccel);
  //for (i=0,it=accel->begin() ; i < naccel ; i++,++it)
  for (i=0,it=accel.begin() ; i < naccel ; i++,++it)
  {
       RCACCEL ac = *it;
       FormatAccelerator(&ac,label);
       additem(dialog,label);
  }
}

class KSCButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     KSCButtonDialogControl(int id) : ButtonDialogControl(id)  {};
     int select(Dialog *dialog);
  };

int KSCButtonDialogControl::select(Dialog *dialog)
{int i,naccel;
 RCHACCEL haccel;
 //RCVACCEL *accel;
 RCCHAR label[300],text[300];
 RCVACCELITER it;

  if (((KSCListDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,text,300) != 0)
    {  naccel = CopyAcceleratorTable(program->gethaccel(),nullptr,0);
       //accel = new RCVACCEL(naccel);
       RCVACCEL accel(naccel);
       CopyAcceleratorTable(program->gethaccel(),&accel,naccel);
      // delete the old item from the ini file

       for (i=0,it=accel.begin() ; i < naccel ; i++,++it)
         {
            RCACCEL ac = *it;
            FormatAccelerator(&ac,label);
            if (strcmp(label,text) == 0)
              {  sprintf(text,"Delete accelerator :\n%ls?",label);
                 if (cadwindow->MessageBox(text,"Delete Accelerator",MB_YESNO) == IDYES)
                   {
                     // delete the values in the .ini file
                     /*
                     {
                         QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
                         settings.remove("Accelerators");
                         settings.sync();
                     }
                    */
                     for ( ; i < naccel - 1 ; i++,++it)
                      {
                        *it = *(it+1);
                        //accel[i] = accel[i+1];
                      }
                      naccel--;

                      haccel = CreateAcceleratorTable(&accel,naccel);
                      if (haccel != 0)
                        {  DestroyAcceleratorTable(program->gethaccel());
                           program->sethaccel(haccel);
                        }

                      SaveAcceleratorTable();
                      ((KSCListDialogControl *)dialog->getcontrol(100))->removeitem(dialog,label);

                      break;

                   }
              }
         }
    }

  return 0;

}

void keyboard_command(int *cstate,HighLevelEvent *e,void **)
{
 Dialog dialog("KeyboardShortcuts_Dialog");
 static int key,control,alt;
 int i,commandid,vk,fv,naccel;
 RCCHAR sc[300];
 RCHACCEL haccel;
 RCVACCELITER it;
 static RCCHAR **rckeylist=nullptr;

 if(*cstate == InitialState)
 {
     rckeylist = new RCCHAR*[KEYLIST_SIZE];
     for(int i=0; i<KEYLIST_SIZE; i++)
     {
         rckeylist[i] = new RCCHAR[strlen(QString(keylist[i]).data())+1];
         strcpy(rckeylist[i],keylist[i]);
     }
 }

  switch (*cstate)
    {case  InitialState:
       strcpy(sc,"");
       key = 0;
       control = alt = 0;
       dialog.add(new KSCListDialogControl(100,sc,300));
       dialog.add(new ListDialogControl(101,36,rckeylist,&key));
       dialog.add(new CheckBoxDialogControl(102,&control));
       dialog.add(new CheckBoxDialogControl(103,&alt));
       dialog.add(new KSCButtonDialogControl(104));
       if (dialog.process() == TRUE)
       {//  Add this to the accelerator table.
            *cstate = 1;
            cadwindow->prompt->normalprompt("Select the command to associate with this short cut key");
            state.selmask.entity.clearandset(end_list);
       }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (e->isa(NewCommand) && (commandid = ((NewCommandEvent *)e)->getcommandid()) >= 0)
         {//  Add this accelerator to the accelerator table.
            if (key < 26)
              vk = 'A' + key;
            else if (key < 38)
              vk = Qt::Key_F1 /*VK_F1*/ + key - 26;
            else if (key == 38)
              vk = Qt::Key_Delete /*VK_DELETE*/;
            else if (key == 39)
              vk = Qt::Key_End /*VK_END*/;
            else if (key == 40)
              vk = Qt::Key_Home /*VK_HOME*/;
            else if (key == 41)
              vk = Qt::Key_Insert /*VK_INSERT*/;
            else if (key == 42)
              vk = Qt::Key_PageDown /*VK_NEXT*/;
            else if (key == 43)
              vk = Qt::Key_PageUp /*VK_PRIOR*/;
            else if (key == 44)
              vk = Qt::Key_Tab /*VK_TAB*/;
            else if (key < 55)
              vk = '0' + key - 45;
            else
              vk = 'A';

            fv = FVIRTKEY;
            if (control)
              fv |= FCONTROL;
            if (alt)
              fv |= FALT;

            naccel = CopyAcceleratorTable(program->gethaccel(),0,0);
            RCVACCEL accel(naccel+1);
            CopyAcceleratorTable(program->gethaccel(),&accel,naccel);

            //for (i=0,it=accel->begin() ; i < naccel ; i++,++it)
            for (i=0,it=accel.begin() ; i < naccel ; i++,++it)
              {
                  RCACCEL ac = *it;
                  if (ac.key == vk && ac.fVirt == fv)
                   {
                      ac.cmd = commandid;
                      *it = ac;
                      break;
                   }
              }

            if (i == naccel)
              {
                 RCACCEL ac = *it;
                 ac.key = vk;
                 ac.fVirt = fv;
                 ac.cmd = commandid;
                 // built the kstring
                 if((ac.fVirt & FSHIFT) != 0)
                     ac.kstring = "Shift+";
                 if((ac.fVirt & FALT) != 0)
                     ac.kstring = "Alt+";
                 if((ac.fVirt & FCONTROL) != 0)
                     ac.kstring = "Ctrl+";
                 if((ac.fVirt & FVIRTKEY) != 0)
                     ac.kstring += QKeySequence(ac.key).toString();//QString(ac.key);
                 *it = ac;
                 naccel++;
              }

            haccel = CreateAcceleratorTable(&accel,naccel);
            if (haccel != nullptr)
              {  DestroyAcceleratorTable(program->gethaccel());
                 program->sethaccel(haccel);
              }

            SaveAcceleratorTable();

         }

       *cstate = ExitState;
       break;

    }

  if(*cstate == ExitState)
  {
      // clean up
      if(rckeylist)
      {
          for(int i=0; i<KEYLIST_SIZE; i++)
              delete rckeylist[i];
          //
          delete [] rckeylist;
      }
  }
}
