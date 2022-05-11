
#include "ncwin.h"
#include <QToolBar>
#ifndef _USING_QT
#include "htmlhelp.h"
#endif
//
//  The implementation of the command stack
//

void CommandStack::push(Command c,int cid,int s,void *d,const BitMask &tm,const AttributeMasks &am,const EntityHeader &eh,EntityList &el,const RCCHAR *p)
{ stack.add(new CommandState(c,cid,s,d,tm,am,eh,el,p));
}

int CommandStack::pop(Command *c,int *cid,int *s,void **d,BitMask *tm,AttributeMasks *am,EntityHeader *eh,EntityList *el,RCCHAR *p)
{CommandState *cs;
 ResourceString rs1(NCWIN+1);
  stack.start();
  if ((cs = (CommandState *) stack.next()) != NULL)
    {  *c = cs->command;  *cid = cs->commandid;  *s = cs->state;
       *d = cs->data;  *tm = cs->tempmask;  *am = cs->selmask;
       *eh = cs->defheader;  *el = cs->excludelist;
       strcpy(p,cs->prompt);
       stack.del(cs);
       delete cs;
       return 1;
    }
  else
    {  *c = NULL;  *cid = 0;  *s = ExitState;  strcpy(p,rs1.gets());
       return 0;
    }
}

//
//  The implementation of the state machine
//

StateMachine::StateMachine() : tempentitymask(32),defheader(0,0,0,0),lastvisible(MaxViews)
{
#ifdef USING_WIDGETS
    qgiDragger = 0;
    mlastcslength = 0;
    glDraggerLine = 0;
#endif

   command = NULL;
   commandid = 0;
   lastcommandid = -1;
   lastsubcommandid = -1;
   lastviewno = 0;
   lastend = 0;
   lastpickpoint.setxyz(0,0,0);
   event = NULL;
   selmask.entity.setandclear(uv_mask,xyz_mask,undo_mask,linear_mask,surface_mask,uvsurface_mask,
      lineardimension_entity,angulardimension_entity,
      distancebearingdimension_entity,circulardimension_entity,end_list);
   selmask.layer.setall();
   selmask.colour.setall();
   selmask.style.setall();
   selmask.weight.setall();
   tempentitymask = selmask.entity;
   tempentitymask.set(xyz_mask);
   tempentitymask.set(uv_mask);
   pos.setxyz(0.0,0.0,0.0);
   dragon = 0;
   inputdevice = 0;
   handledentity = 0;
   strcpy(groupnamemask,"");

}

StateMachine::~StateMachine()
{
}

#ifdef NEW_RUBBERB
void StateMachine::sethandledentity(Entity *e)
{
    if (handledentity != 0 && cadwindow != 0)
    {
        //handledentity->draw(DM_NORMAL);
        handledentity->sethandles(0);
        //handledentity->draw(DM_ERASE);
        cadwindow->clearAllRubberBands();
    }
    handledentity = e;
    if (handledentity != 0)
    {
        //handledentity->draw(DM_NORMAL);
        handledentity->sethandles(1);
        //handledentity->draw(DM_ERASE);
        cadwindow->clearAllRubberBands();
    }
}
#else
void StateMachine::sethandledentity(Entity *e)
{
    if (handledentity != 0 && cadwindow != 0)
    {  handledentity->draw(DM_ERASE);
       handledentity->sethandles(0);
       handledentity->draw(DM_NORMAL);
    }
  handledentity = e;
  if (handledentity != 0)
    {  handledentity->draw(DM_ERASE);
       handledentity->sethandles(1);
       handledentity->draw(DM_NORMAL);
    }
}
#endif
Entity *StateMachine::gethandledentity(void)
{ return handledentity;
}

extern void RunVideo(int commandid);

void StateMachine::process(void)
{RCCHAR prompt[300];
 int maskcommand,filecommand;
 Command c;

    if (event != NULL)
    //if (event != NULL && commandid < 31000)
    {HighLevelEvent *e;
       e = event;  event = NULL;

        if (e->isa(NewCommand) && db.StartCommandCallback != 0)
            db.StartCommandCallback(((NewCommandEvent *)e)->getcommandid(),InitialState,0);

       if (e->isa(NewCommand) && ((NewCommandEvent *)e)->getcommandid() == 605)
         {  if (lastcommandid >= 0)
              {  program->help(HELP_CONTEXT,(DWORD)lastcommandid);
                 RunVideo(lastcommandid);
              }
         }
       else if (e->isa(NewCommand) &&
               ((command == 0 || commands.subcommand(lastcommandid) != 4) &&
               (((NewCommandEvent *)e)->getcommandid() != 200 || ! selmask.entity.test(undo_mask))))
         {    cadwindow->updatemenus(((NewCommandEvent *) e)->getcommandid());
              maskcommand = 0;
              filecommand = 0;
              if (commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 2)
                maskcommand = 1;
              else if (commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 1 ||
                       commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 5 ||
                       commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 6)
                cs.push(command,commandid,state,data,tempentitymask,selmask,defheader,excludelist,cadwindow->prompt->normalprompt());
              else
                {ExitEvent exit(((NewCommandEvent *) e)->getcommandid());
                   if (commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 3 ||
                       commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 5)
                     filecommand = 1;
                   strcpy(prompt,cadwindow->prompt->normalprompt());
                   while (command != NULL)
                     { command(&state,&exit,&data);
                       cs.pop(&command,&commandid,&state,&data,&tempentitymask,&selmask,&defheader,&excludelist,prompt);
                     }
                   cadwindow->prompt->normalprompt(prompt);
                   if (! filecommand)
                     cs.push(command,commandid,state,data,tempentitymask,selmask,defheader,excludelist,prompt);
                }
              if (! maskcommand && ((NewCommandEvent *)e)->getcommandid() != 2010)  //  Don't replace the mask when doing a polywindow command.
                {  selmask.entity.setandclear(uv_mask,xyz_mask,undo_mask,linear_mask,surface_mask,uvsurface_mask,
                                 lineardimension_entity,angulardimension_entity,
                                 distancebearingdimension_entity,circulardimension_entity,end_list);
                   tempentitymask = selmask.entity;
                   tempentitymask.set(xyz_mask);
                   tempentitymask.set(uv_mask);
                }
              c = commands.command(((NewCommandEvent *) e)->getcommandid());
              lastcommandid = ((NewCommandEvent *) e)->getcommandid();
              if (cs.length() == 2 && commands.subcommand(((NewCommandEvent *) e)->getcommandid()) == 6)
                lastsubcommandid = ((NewCommandEvent *) e)->getcommandid();
              else
                lastsubcommandid = -1;
              //  The following are restricted commands available only in the demo, or full versions.
              if (program->getlicensed() == 2 &&
                    (lastcommandid == 400 ||  //  Workplane commands
                     lastcommandid == 401 ||
                     lastcommandid == 402 ||
                     lastcommandid == 403 ||
                     lastcommandid == 405 ||
                     lastcommandid == 425 ||
					 lastcommandid == 427 ||
                     lastcommandid == 406 ||
                     lastcommandid == 407 ||
                     lastcommandid == 410 ||
                     lastcommandid == 411 ||
                     lastcommandid == 419 ||
                     lastcommandid == 418 ||
                     lastcommandid == 21000 ||
                     lastcommandid == 423 ||
                     lastcommandid == 22000 ||
                     lastcommandid == 30000 ||
                     lastcommandid == 901 ||
                     lastcommandid == 902 ||
                     lastcommandid == 903 ||
                     //lastcommandid == 911 ||  light can now run apps
                     lastcommandid == 915 ||
                     lastcommandid == 916 ||
                     lastcommandid == 528 ||
                     lastcommandid == 529 ||
                     lastcommandid == 537 ||
                     lastcommandid == 538 ||
                     lastcommandid == 539 ||
                     lastcommandid == 540 ||
                     lastcommandid == 1502 ||
                     lastcommandid == 1503 ||
                     lastcommandid == 1504 ||
                     lastcommandid == 1505 ||
                     lastcommandid == 1506 ||
                     lastcommandid == 1507 ||
                     lastcommandid == 1508 ||
                     lastcommandid == 1911 ||
                     // command ids added or changed for v7
                     lastcommandid == 5102 || // hidden line removal
                     lastcommandid == 5103 || // dashed hidden line
                     lastcommandid == 5104 || // shaded surfaces
                     lastcommandid == 5105 || // shaded surfaces with lines
                     lastcommandid == 5106 || // shaded dashed lines
                     lastcommandid == 5107 || // texture shaded
                     lastcommandid == 5108 || // texture shaded with lines
                     lastcommandid == 5109 //|| // VR 3d (stereo)
                     // ?lastcommandid == 5110 || // view as perspective
                     // ? lastcommandid == 5111 || // view as parallel
                     ))
                {  cadwindow->MessageBox("This command is not available in the light version.\nYou can purchase the professional version to access this command.","RealCAD Light",MB_ICONINFORMATION);
                   c = 0;
                }
              if (program->getlicensed() == 3 &&
                    (lastcommandid == 400 ||  //  Workplane commands
                     lastcommandid == 401 ||
                     lastcommandid == 402 ||
                     lastcommandid == 403 ||
                     lastcommandid == 404 ||
                     lastcommandid == 405 ||
                     lastcommandid == 425 ||
					 lastcommandid == 427 ||
                     lastcommandid == 406 ||
                     lastcommandid == 407 ||
                     lastcommandid == 410 ||
                     lastcommandid == 411 ||
                     lastcommandid == 419 ||
                     lastcommandid == 418 ||
                     lastcommandid == 21000 ||
                     lastcommandid == 423 ||
                     lastcommandid == 22000 ||
                     lastcommandid == 30000 ||
                     lastcommandid == 901 ||
                     lastcommandid == 902 ||
                     lastcommandid == 903 ||
                     lastcommandid == 911 || // load application
                     lastcommandid == 915 || // command window
                     lastcommandid == 916 || // unload application
                     lastcommandid == 528 ||
                     lastcommandid == 529 ||
                     lastcommandid == 537 ||
                     lastcommandid == 538 ||
                     lastcommandid == 539 ||
                     lastcommandid == 540 ||
                     lastcommandid == 700 || // export dxf
                     lastcommandid == 701 || // export iges
                     lastcommandid == 702 || // export hpgl
                     lastcommandid == 703 || // export stroked iges
                     lastcommandid == 750 || // import dxf
                     lastcommandid == 751 || // import iges
                     lastcommandid == 752 || // import hpgl
                     lastcommandid == 797 ||
                     lastcommandid == 1502 ||
                     lastcommandid == 1503 ||
                     lastcommandid == 1504 ||
                     lastcommandid == 1505 ||
                     lastcommandid == 1506 ||
                     lastcommandid == 1507 ||
                     lastcommandid == 1508 ||
                     lastcommandid == 1911 ||
                     // command ids added or changed for v7
                     lastcommandid == 704  || // export dwg
                     lastcommandid == 754  || // import dwg
                     lastcommandid == 755  || // import skp
                     lastcommandid == 10020 || // 3d warehouse
                     lastcommandid == 5102 || // hidden line removal
                     lastcommandid == 5103 || // dashed hidden line
                     lastcommandid == 5104 || // shaded surfaces
                     lastcommandid == 5105 || // shaded surfaces with lines
                     lastcommandid == 5106 || // shaded dashed lines
                     lastcommandid == 5107 || // texture shaded
                     lastcommandid == 5108 || // texture shaded with lines
                     lastcommandid == 5109 //|| // VR 3d (stereo)
                     // ?lastcommandid == 5110 || // view as perspective
                     // ? lastcommandid == 5111 || // view as parallel
                     ))
                {  cadwindow->MessageBox("This command is not available in the draft version.\nYou can purchase the professional version to access this command.","RealCAD Draft",MB_ICONINFORMATION);
                   c = 0;
                }

              if (c != NULL)
                {  if (maskcommand || filecommand)
                     {int s = InitialState;
                        c(&s,e,&data);
                     }
                   else
                     {  command = c;
                        commandid = ((NewCommandEvent *) e)->getcommandid();
                        state = InitialState;
                        //
                        //cadwindow->freezeBackImage();
                        //
                        command(&state,e,&data);
                        if (state == ExitState)
                          {
                             //
                             //cadwindow->unFreezeBackImage();
                             //
                             cs.pop(&command,&commandid,&state,&data,&tempentitymask,&selmask,&defheader,&excludelist,prompt);
                             cadwindow->prompt->normalprompt(prompt);
                             if (commands.subcommand(lastcommandid) == 4)
                               lastcommandid = 0;  //  Cancel the last command for these types of command
                             commandid = 0;
                             if (event == 0)
                               lastsubcommandid = -1;
                          }
                     }
                }
              cadwindow->updatemenus(commandid);
         }
       else
         {    if (command != NULL)
                {  command(&state,e,&data);
                   if (state == ExitState)
                     {
                        if (db.EndCommandCallback != 0)
                            db.EndCommandCallback(commandid,ExitState,0);

                        cs.pop(&command,&commandid,&state,&data,&tempentitymask,&selmask,&defheader,&excludelist,prompt);
                        cadwindow->prompt->normalprompt(prompt);
                        if (e->type() == NewCommand)
                          lastcommandid = commandid;
                        if (event == 0)
                          lastsubcommandid = -1;
                     }
                   else if (cs.length() == 1 && ! eventavailable() && lastsubcommandid >= 0)
                     sendevent(new NewCommandEvent(lastsubcommandid,0));
                   cadwindow->updatemenus(commandid);
                }
                else
                {
                    qDebug() << "command is null";
                }
         }

       if (e->isa(NewCommand))
            cadwindow->update_properties(-1);

       delete e;
    }
  if(cadwindow && cadwindow->prompt)
      if (command == NULL && ! dragging())
        cadwindow->prompt->setcolour(RGB(0,150,0));
      else if (cs.length() == 1 || dragging())
        cadwindow->prompt->setcolour(RGB(0,0,0));
      else if (cs.length() == 2)
        cadwindow->prompt->setcolour(RGB(0,0,200));

  //  Always clear the handles if a command is active.
  if (command != 0)
    sethandledentity(0);

  // debug
  /*
  if(command == 0)
      qDebug() << "command is zero!!";
  else
      qDebug() << "command is - " << command;
  */
}

void StateMachine::setinputdevice(int deviceno)
{ switch (inputdevice)
    {  case 0 :
         break;
       case 1 :
         //metrecom.terminate();
         break;
    }
  inputdevice = deviceno;
  switch (inputdevice)
    {  case 0 :
         cadwindow->cursorstyle(1);
         break;
       case 1 :
         cadwindow->cursorstyle(2);
         //if (! metrecom.initialize()) setinputdevice(0);
         break;
    }
}

void StateMachine::timerevent(void)
{ switch (inputdevice)
    {  case 0 :
         return;
       case 1 :
         //if (! metrecom.processinput())
            //setinputdevice(0);
         break;
    }
}

#if VERSION != TRICAD_STUDENT
CommandWindow::CommandWindow(Window *window,int w,int h) :
  Window((char*)"CadWindow",window->gethwnd(),(DWORD)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VSCROLL),0,0,w,h,SW_NORMAL,NULL)
{SIZE size;
 long i;
  SetScrollRange(gethwnd(),SB_VERT,0,1000,TRUE);
  hfont = CreateFont(18,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Courier New"));
  holdfont = (RCHFONT) SelectObject(gethdc(),hfont);
  GetTextExtentPoint(gethdc(),_RCT("A"),1,&size);
  cheight = size.cy;
  cwidth = size.cx;
  selectiondrawn = 0;
  captured = 0;
  x1 = y1 = x2 = y2 = 0;

  maxlines = 500;
  maxlength = 100;
  nlines = 0;
  lineno = 0;
  line = new RCCHAR *[maxlines];
  for (i = 0 ; i < maxlines ; i++)
    {  line[i] = new RCCHAR[maxlength];
       strcpy(line[i],"");
    }
  ybase = 0;
  title("Command Window");
}

CommandWindow::~CommandWindow()
{int i;
  if (hfont != 0 && gethdc() != 0)
    {  SelectObject(gethdc(),holdfont);
       DeleteObject(hfont);
    }
  for (i = 0 ; i < maxlines ; i++)
    delete [] line[i];
  delete [] line;
  cadwindow->selectwindow(0);
  cadwindow->setthecommandwindow(0);
}

void CommandWindow::paintevent(void)
{RECT rect;
 int i;

  GetClientRect(gethwnd(),&rect);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));

  SetTextColor(gethdc(),RGB(0,0,0));
  SetBkColor(gethdc(),RGB(255,255,255));

if (ybase < 0)
 ybase = 0;
if (ybase > maxlines)
  ybase = maxlines-1;

  for (i = 0 ; i < height / cheight ; i++)
    if (i + ybase < maxlines)
      TextOut(gethdc(),2,int(1 + i*cheight),(RCLPCTSTR)line[i+ybase],strlen(line[i+ybase]));

  paintselection(0);

  Window::paintevent();
}

int CommandWindow::deleteselection(void)
{unsigned long y,t;
 char l[300];
  if (x1 != x2 || y1 != y2)
    {  if (y2 < y1)
         {  t = y1;  y1 = y2;  y2 = t;
         }
       else if (y1 == y2 && x2 < x1)
         {  t = x1;  x1 = x2;  x2 = t;
         }
       if (y1 == y2 && line[y1] != 0)
         {  strcpy(l,line[y1]);
            if ((long)strlen(line[y1]) > x2)
              {  line[y1][(int)x1] = 0;
                 strcat(line[y1],l+(int)x2);
              }
            else
              line[y1][(int)x1] = 0;
         }
       else if (line[y1] != 0)
         {  line[y1][(int)x1] = 0;
            if (line[y2] != 0)
              strcat(line[y1],line[y2]+(int)x2);
            for (y = y2 ; y < (unsigned)(maxlines - 1) ; y++)
              strcpy(line[y-(y2-y1)],line[y]);
            for (y = maxlines-y2-y1 ; y <= (unsigned)(maxlines - 1) ; y++)
              strcpy(line[y],"");
         }
       y2 = y1;  x2 = x1;
       paintevent();
       return 1;
    }
  return 0;
}

void CommandWindow::paste(void)
{HGLOBAL clipdata;
 char *cd;
 long y,yt;
 char l[300],endline[300];
  if (OpenClipboard(cadwindow->gethwnd()))
    {  clipdata = GetClipboardData(CF_TEXT);
       if (clipdata != 0)
         {  cd = (char *) GlobalLock(clipdata);
            if (cd != 0)
              {  y = y1;
                 while (x1 > (long)strlen(line[y]))
                   strcat(line[y]," ");
                 strcpy(l,line[y]);
                 strcpy(endline,line[y]+x1);
                 l[x1] = 0;
                 while (*cd != 0)
                   {  if (*cd != '\n' && *cd != '\r')
                        {  l[strlen(l)+1] = 0;
                           l[strlen(l)] = *cd;
                        }
                      else if (*cd == '\n')
                        {  strcpy(line[y],l);
                           strcpy(l,"");
                           y++;
                           for (yt = maxlines-1 ; yt > y ; yt--)
                             strcpy(line[yt],line[yt-1]);
                           strcpy(line[y],"");
                        }
                      cd++;
                   }
                 strcat(l,endline);
                 strcpy(line[y],l);
                 GlobalUnlock(clipdata);
                 if (y == y1)
                   paintline(y1);
                 else
                   paintevent();
              }
         }
       CloseClipboard();
    }
}

void CommandWindow::copyselection(void)
{HGLOBAL clipdata;
 RCCHAR *cd;
 long t,y;
 DWORD nbytes;
  if (x1 != x2 || y1 != y2)
    {  if (y2 < y1)
         {  t = y1;  y1 = y2;  y2 = t;
         }
       else if (y1 == y2 && x2 < x1)
         {  t = x1;  x1 = x2;  x2 = t;
         }
       if (y1 == y2 && line[y1] != 0)
         {  if (x1 < (long)strlen(line[y1]))
              {  if (x2 < (long)strlen(line[y1]))
                   nbytes = x2 - x1;
                 else
                   nbytes = strlen(line[y1]) - x1;
              }
            else
              nbytes = 0;
         }
       else
         {  if (line[y1] != 0 && x1 < (long)strlen(line[y1]))
              nbytes = strlen(line[y1]) - x1 + 2;
            else
              nbytes = 2;
            if (line[y2] != 0)
              {  if (x2 < (long)strlen(line[y2]))
                   nbytes += x2;
                 else
                   nbytes += strlen(line[y2]);
              }
            for (y = y1 + 1 ; y < y2 ; y++)
              if (line[y] != 0)
                nbytes += strlen(line[y]) + 2;
              else
                nbytes += 2;
         }
       if (OpenClipboard(cadwindow->gethwnd()))
         {  clipdata = GlobalAlloc(0,nbytes+1);
            if (clipdata != 0)
              {  cd = (RCCHAR *) GlobalLock(clipdata);
                 if (cd != 0)
                   {  if (y1 == y2 && line[y1] != 0)
                        {  if (x1 < (long)strlen(line[y1]))
                             {  if (x2 < (long)strlen(line[y1]))
                                  {  strncpy(cd,line[y1]+x1,x2 - x1);
                                     cd[x2-x1] = 0;
                                  }
                                else
                                  strcpy(cd,line[y1]+x1);
                             }
                           else
                             strcpy(cd,"");
                        }
                      else
                        {  if (line[y1] != 0 && x1 < (long)strlen(line[y1]))
                             strcpy(cd,line[y1]+x1);
                           else
                             strcpy(cd,"");
                           strcat(cd,"\n\r");
                           for (y = y1 + 1 ; y < y2 ; y++)
                             {  if (line[y] != 0)
                                  strcat(cd,line[y]);
                                strcat(cd,"\n\r");
                             }
                           if (line[y2] != 0)
                             {  if (x2 >= (long)strlen(line[y2]))
                                  strcat(cd,line[y2]);
                                else
                                  {  strncat(cd,line[y2],x2);
                                     cd[nbytes] = 0;
                                  }
                             }
                        }
                   }
                 GlobalUnlock(clipdata);
                 SetClipboardData(CF_TEXT,clipdata);
              }
            CloseClipboard();
         }
    }
}

void CommandWindow::charevent(UINT c,DWORD vk)
{RCCHAR l[300],*s;
 long y;
  if (vk == VK_HOME)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1 = x2 = 0;  y1 = y2;
         }
       else
         x2 = 0;
       paintselection(0);
    }
  else if (vk == VK_END)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1 = x2 = strlen(line[y2]);
            y1 = y2;
         }
       else
         x2 = strlen(line[y2]);
       paintselection(0);
    }
  else if (vk == VK_RIGHT)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1++;  x2 = x1;
         }
       else
         x2++;
       paintselection(0);
    }
  else if (vk == VK_LEFT)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1--;  if (x1 < 0) x1 = 0;  x2 = x1;
         }
       else
         {  x2--;  if (x2 < 0) x2 = 0;
         }
       paintselection(0);
    }
  else if (vk == VK_DOWN)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  y1++;   y2 = y1;  x1 = x2;
            if (y1 >= ybase + height / cheight)
              {  ybase = (long)(y1 - height / cheight + 1);
                 if (ybase < 0) ybase = 0;
                 if (ybase >= maxlines)
                   {  ybase = maxlines - 1;
                      if (ybase < 0)
                        ybase = 0;
                      y1--;  y2--;
                   }
                 paintevent();
                 paintselection(1);
              }
         }
       else
         {  y2++;
            if (y2 >= ybase + height / cheight)
              {  ybase = (long)(y2 - height / cheight + 1);
                 if (ybase < 0) ybase = 0;
                 if (ybase >= maxlines)
                   {  ybase = maxlines - 1;
                      y2--;
                   }
                 paintevent();
                 paintselection(1);
              }
         }
       paintselection(0);
    }
  else if (vk == VK_PRIOR)
    {  paintselection(1);
       if (ybase < height / cheight - 1)
         {  y1 -= ybase;  y2 = y1;  x2 = x1;
            ybase = 0;
         }
       else
         {  y1 -= (long)(height / cheight - 1);  y2 = y1;  x2 = x1;
            ybase -= (long)(height / cheight - 1);
         }
       paintevent();
       paintselection(0);
    }
  else if (vk == VK_NEXT)
    {  paintselection(1);
       if (ybase < height / cheight - 1)
         {  y1 += long(height / cheight - 1);
            ybase += long(height / cheight - 1);
            y2 = y1;  x2 = x1;
         }
       paintevent();
       paintselection(0);
    }
  else if (vk == VK_UP)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  y1--;  if (y1 < 0) y1 = 0;  y2 = y1;
            if (y1 < ybase)
              {  ybase = y1;
                 paintevent();
                 paintselection(1);
              }
         }
       else
         {  y2--;  if (y2 < 0) y2 = 0;
            if (y2 < ybase)
              {  ybase = y2;
                 paintevent();
                 paintselection(1);
              }
         }
       paintselection(0);
    }
  else if (vk == VK_BACK)
    {  if (deleteselection())
         return;
       if (x1 > (long)strlen(line[y1]))
         {  x1--;  x2--;
         }
       else if (x1 == 0)
         {  if (y1 > 0)
              {  x1 = strlen(line[y1-1]);
                 strcat(line[y1-1],line[y1]);
                 for (y = y1 ; y < maxlines - 1 ; y++)
                   strcpy(line[y],line[y+1]);
                 strcpy(line[maxlines-1],"");
                 x2 = x1;  y1--;  y2--;
              }
         }
       else
         {  strcpy(l,line[y1]);
            strcpy(line[y1]+(int)x1-1,l+(int)x1);
            x1--;  x2 = x1;
            paintline(y1);
         }
       paintevent();
    }
  else if (vk == VK_INSERT)
    {  if ((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000)
         {  copyselection();
         }
       else if ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000)
         {  deleteselection();
            paste();
         }
    }
  else if (vk == VK_DELETE)
    {  copyselection();
       if (deleteselection())
         return;
       if (x1 >= (long)strlen(line[y1]) && y1+1 < maxlines)
         {  x1 = (long)strlen(line[y1]);
            strcat(line[y1],line[y1+1]);
            for (y = y1+1 ; y < maxlines - 1 ; y++)
              strcpy(line[y],line[y+1]);
            strcpy(line[maxlines-1],"");
            x2 = x1;
            paintevent();
         }
    }
  else if (vk == VK_RETURN)
    {  deleteselection();
       if (x1 > (long)strlen(line[y1]))
         {  x1 = x2 = 0;
            if (y1 < maxlines - 1)
              y1++;
            else
              {  for (y = maxlines-1 ; y > 1 ; y--)
                   strcpy(line[y],line[y-1]);
                 strcpy(line[maxlines-1],"");
              }
            y2 = y1;
            for (y = maxlines-1 ; y > y1 ; y--)
              strcpy(line[y],line[y-1]);
            strcpy(line[y1],"");
         }
       else
         {  strcpy(l,line[y1]);
            line[y1][(int)x1] = 0;
            s = l + (int)x1;
            x1 = x2 = 0;
            if (y1 < maxlines - 1)
              y1++;
            else
              {  for (y = maxlines-1 ; y > 1 ; y--)
                   strcpy(line[y],line[y-1]);
                 strcpy(line[maxlines-1],"");
              }
            y2 = y1;
            for (y = maxlines-1 ; y > y1 ; y--)
              strcpy(line[y],line[y-1]);
            strcpy(line[y1],s);
         }
       commandfile.send(line[y1-1]);
       if (y1 >= ybase + height / cheight - 2)
         {  ybase = long(y1 - height / cheight + 2);
            if (ybase < 0) ybase = 0;
            if (ybase >= maxlines)
              {  ybase = maxlines - 1;
                 if (ybase < 0)
                   ybase = 0;
              }
         }
       paintevent();
    }
  else if (c >= 32 && c <= 126)
    {  deleteselection();
       strcpy(l,line[y1]);
       while (x1 > (long)strlen(l))
         strcat(l," ");
       l[(int)x1] = (RCCHAR) c;
       l[(int)x1+1] = 0;
       if ((long)strlen(line[y1]) > x1)
         strcat(l,line[y1]+x1);
       x1++;  x2++;
       strcpy(line[y1],l);
       paintline(y1);
    }
  vscrollevent(1000,1000);
}

void CommandWindow::paintselection(int clear)
{RECT rect;
 int x,nchars;
 long y;
 char l[300];
  if (clear && ! selectiondrawn)
    return;
  if (clear)
    {  SetTextColor(gethdc(),RGB(0,0,0));
       SetBkColor(gethdc(),RGB(255,255,255));
    }
  else
    {  SetTextColor(gethdc(),GetSysColor(COLOR_HIGHLIGHTTEXT));
       SetBkColor(gethdc(),GetSysColor(COLOR_HIGHLIGHT));
    }
  if (clear)
    {  rect.left = (int) x2 * cwidth + 1;  rect.right = (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
#if ! defined(_WIN32_WCE)
       InvertRect(gethdc(),&rect);
#endif
    }
  SetBkMode(gethdc(),OPAQUE);
  for (y = y1 ; y <= y2 ; y++)
    {  if (y == y1)
         {  x = 2 + (int) x1 * cwidth;
            if ((long)strlen(line[y]) > x1)
              sprintf(l,"%-299s",line[y]+(int)x1);
            else
              sprintf(l,"%-299s"," ");
            nchars = int(width / cwidth) - (int)x1;
         }
       else
         {  if ((long)strlen(line[y]) > x1)
              sprintf(l,"%-299s",line[y]);
            else
              sprintf(l,"%-299s"," ");
            x = 2;
            nchars = long(width / cwidth);
         }
       if (y == y2)
         nchars = nchars - (int(width / cwidth) - (int) x2);
       TextOut(gethdc(),x,int(2+(y-ybase)*cheight),(RCLPCTSTR)l,nchars);
    }
  if (! clear)
    {  rect.left = (int) x2 * cwidth + 1;  rect.right = (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
#if ! defined(_WIN32_WCE)
       InvertRect(gethdc(),&rect);
#endif
    }
  selectiondrawn = ! clear;
}

void CommandWindow::paintline(long y)
{RECT rect;
  rect.left = 1;  rect.right = long(width);
  rect.top = (int)(y - ybase) * cheight + 1;  rect.bottom = (int)(y - ybase) * cheight + cheight + 1;
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  if (line[y] == 0)
    return;
  SetTextColor(gethdc(),RGB(0,0,0));
  SetBkColor(gethdc(),RGB(255,255,255));
  SetBkMode(gethdc(),OPAQUE);
  TextOut(gethdc(),2,int(1+(y-ybase)*cheight),(RCLPCTSTR)line[y],strlen(line[y]));

  if (y == y2)
    {  rect.left = (int) x2 * cwidth + 1;  rect.right = (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
#if ! defined(_WIN32)
       InvertRect(gethdc(),&rect);
#endif
       selectiondrawn = 1;
    }

  highlight(y);

}

void CommandWindow::highlight(long)
{
}

void CommandWindow::lbuttondownevent(int x,int y,int keys)
{ cadwindow->selectwindow(this);
  paintselection(1);
  if ((keys & MK_SHIFT) == MK_SHIFT)
    {  x2 = (x - 1) / cwidth;
       y2 = (y - 1) / cheight + ybase;
    }
  else
    {  x1 = (x - 1) / cwidth;  x2 = x1;
       y1 = (y - 1) / cheight + ybase;  y2 = y1;
    }
  paintselection(0);
  SetCapture(gethwnd());
  captured = 1;
}

void CommandWindow::setcurrentline(long l)
{ if (l < ybase || l >= ybase + height / cheight)
    {  x1 = x2 = 0;
       y1 = y2 = l;
       if (l < ybase)
         ybase = l;
       if (l >= ybase + height / cheight)
         ybase = long(l - height / cheight + 1);
       if (ybase < 0) ybase = 0;
       if (ybase >= nlines)
         ybase = nlines - 1;
       paintevent();
    }
  else
    {  paintselection(1);
       x1 = x2 = 0;
       y1 = y2 = l;
       paintselection(0);
    }
}

void CommandWindow::mousemoveevent(int x,int y,int b)
{ if (captured)
    {  paintselection(1);
       x2 = (x - 1) / cwidth;  if (x2 < 0) x2 = 0;
       y2 = (y - 1) / cheight + ybase;
       if (y2 < ybase)
         {  ybase = y2;
            if (ybase < 0)
              ybase = 0;
            paintevent();
         }
       else if (y2 - ybase > (height + cheight - 1 ) / cheight)
         {  ybase = long(y2 - (height + cheight - 1 ) / cheight + 1);
            if (ybase > nlines - height / cheight)
              ybase = long(nlines - height / cheight);
            if (ybase < 0)
              ybase = 0;
            paintevent();
         }
       else
         paintselection(0);
    }
  else
    Window::mousemoveevent(x,y,b);
}

void CommandWindow::lbuttonupevent(int x,int y,int b)
{ if (captured)
    {  paintselection(1);
       x2 = (x - 1) / cwidth;  if (x2 < 0) x2 = 0;
       y2 = (y - 1) / cheight + ybase;
       paintselection(0);
       ReleaseCapture();
       captured = 0;
    }
  else
    Window::mousemoveevent(x,y,b);
}

void CommandWindow::vscrollevent(int code,int pos)
{int start,end;
  GetScrollRange(gethwnd(),SB_VERT,&start,&end);
  switch (code)
    {case SB_BOTTOM : ybase = long(maxlines - height / cheight);  break;
     case SB_LINEDOWN : ybase++;  break;
     case SB_LINEUP : ybase--;  break;
     case SB_PAGEDOWN : ybase += long(height / cheight - 1);  break;
     case SB_PAGEUP : ybase -= long(height / cheight - 1);  break;
     case SB_THUMBTRACK	:
     case SB_THUMBPOSITION	:  ybase = long(double(pos - start) / double(end - start) * (maxlines - height / 18));  break;
     case SB_TOP : ybase = 0;  break;
    }
  if (ybase <= 0)
    ybase = 0;
  if (code != SB_THUMBTRACK)
    {  if (maxlines - height / cheight > 0)
         SetScrollPos(gethwnd(),SB_VERT,int(start + double(end - start) * ybase / (maxlines - height / 18)),TRUE);
       else
         SetScrollPos(gethwnd(),SB_VERT,0,TRUE);
    }
  if (code < 1000)
    paintevent();
}

void CommandWindow::lbuttondblclkevent(int,int,int)
{POINT p;
 RECT rect;
  GetWindowRect(gethwnd(),&rect);
  if (GetParent(gethwnd()) == cadwindow->gethwnd())
    {//  Change to floating
       SetParent(gethwnd(),cadwindow->subwindow->gethwnd());
       SetWindowLong(gethwnd(),GWL_STYLE,(DWORD)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VSCROLL | WS_VISIBLE | WS_CAPTION));
       SetWindowPos(gethwnd(),0,oldx,oldy,oldw,oldh,SWP_NOZORDER);
       cadwindow->sizeevent((int)cadwindow->getwidth(),(int)cadwindow->getheight());
    }
  else
    {//  Change to fixed
       p.x = rect.left;  p.y = rect.top;
       ScreenToClient(cadwindow->subwindow->gethwnd(),&p);
       rect.left = p.x;  rect.top = p.y;
       p.x = rect.right;  p.y = rect.bottom;
       ScreenToClient(cadwindow->subwindow->gethwnd(),&p);
       rect.right = p.x;  rect.bottom = p.y;
       oldx = rect.left;  oldy = rect.top;
       oldw = rect.right - rect.left;  oldh = rect.bottom - rect.top;
       SetParent(gethwnd(),cadwindow->gethwnd());
       p.x = 0;  p.y = 0;
       ClientToScreen(cadwindow->subwindow->gethwnd(),&p);
       ScreenToClient(cadwindow->gethwnd(),&p);
       SetWindowPos(cadwindow->subwindow->gethwnd(),0,(int)p.x,(int)p.y,(int)cadwindow->subwindow->getwidth(),(int)cadwindow->subwindow->getheight()-cadwindow->getpromptheight()*2,SWP_NOZORDER);
       SetWindowLong(gethwnd(),GWL_STYLE,(DWORD)(WS_CHILD | WS_DLGFRAME | WS_VSCROLL | WS_VISIBLE));
       SetWindowPos(gethwnd(),0,0,(int)cadwindow->getheight()-cadwindow->getpromptheight()*3,(int)cadwindow->getwidth()/2,cadwindow->getpromptheight()*3,SWP_NOZORDER);
       cadwindow->moveprompt(1);
    }
}

#endif

class ESMaskButtonDialogControl : public ButtonDialogControl
  {private:
     StateMachine *sm;
     int action;
   public:
     ESMaskButtonDialogControl(int id,int a,StateMachine *m) : ButtonDialogControl(id) {  action = a;  sm = m;  }
     int select(Dialog *);
  };

int ESMaskButtonDialogControl::select(Dialog *dialog)
{ if (sm->selmask.entity.test(xyz_mask)) ((CheckBoxDialogControl *)dialog->getcontrol(100))->change(dialog,action);
  if (sm->selmask.entity.test(point_entity))((CheckBoxDialogControl *)dialog->getcontrol(101))->change(dialog,action);
  if (sm->selmask.entity.test(line_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(102))->change(dialog,action);
  if (sm->selmask.entity.test(circle_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(103))->change(dialog,action);
  if (sm->selmask.entity.test(ellipse_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(104))->change(dialog,action);
  if (sm->selmask.entity.test(curve_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(105))->change(dialog,action);
  if (sm->selmask.entity.test(plane_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(106))->change(dialog,action);
  if (sm->selmask.entity.test(ruledsurface_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(107))->change(dialog,action);
  if (sm->selmask.entity.test(surfaceofrev_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(108))->change(dialog,action);
  if (sm->selmask.entity.test(text_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(109))->change(dialog,action);
  if (sm->selmask.entity.test(figure_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(110))->change(dialog,action);
  if (sm->selmask.entity.test(group_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(111))->change(dialog,action);
  if (sm->selmask.entity.test(dimension_mask)) ((CheckBoxDialogControl *)dialog->getcontrol(112))->change(dialog,action);
  if (sm->selmask.entity.test(light_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(113))->change(dialog,action);
  if (sm->selmask.entity.test(patch_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(114))->change(dialog,action);
  if (sm->selmask.entity.test(bitmap_entity)) ((CheckBoxDialogControl *)dialog->getcontrol(115))->change(dialog,action);
  return 0;
}

void StateMachine::setupentitymask(void)
{Dialog dialog(_RCT("Entitymask_Dialog"));
 int values[16];
  values[0] = tempentitymask.test(xyz_mask);
  values[1] = tempentitymask.test(point_entity);
  values[2] = tempentitymask.test(line_entity);
  values[3] = tempentitymask.test(circle_entity);
  values[4] = tempentitymask.test(ellipse_entity);
  values[5] = tempentitymask.test(curve_entity);
  values[6] = tempentitymask.test(plane_entity);
  values[7] = tempentitymask.test(ruledsurface_entity);
  values[8] = tempentitymask.test(surfaceofrev_entity);
  values[9] = tempentitymask.test(text_entity);
  values[10] = tempentitymask.test(figure_entity);
  values[11] = tempentitymask.test(group_entity);
  values[12] = tempentitymask.test(dimension_mask);
  values[13] = tempentitymask.test(light_entity);
  values[14] = tempentitymask.test(patch_entity);
  values[15] = tempentitymask.test(bitmap_entity);
  if (selmask.entity.test(xyz_mask)) dialog.add(new CheckBoxDialogControl(100,&values[0]));
  if (selmask.entity.test(point_entity)) dialog.add(new CheckBoxDialogControl(101,&values[1]));
  if (selmask.entity.test(line_entity)) dialog.add(new CheckBoxDialogControl(102,&values[2]));
  if (selmask.entity.test(circle_entity)) dialog.add(new CheckBoxDialogControl(103,&values[3]));
  if (selmask.entity.test(ellipse_entity)) dialog.add(new CheckBoxDialogControl(104,&values[4]));
  if (selmask.entity.test(curve_entity)) dialog.add(new CheckBoxDialogControl(105,&values[5]));
  if (selmask.entity.test(plane_entity)) dialog.add(new CheckBoxDialogControl(106,&values[6]));
  if (selmask.entity.test(ruledsurface_entity)) dialog.add(new CheckBoxDialogControl(107,&values[7]));
  if (selmask.entity.test(surfaceofrev_entity)) dialog.add(new CheckBoxDialogControl(108,&values[8]));
  if (selmask.entity.test(text_entity)) dialog.add(new CheckBoxDialogControl(109,&values[9]));
  if (selmask.entity.test(figure_entity)) dialog.add(new CheckBoxDialogControl(110,&values[10]));
  if (selmask.entity.test(group_entity)) dialog.add(new CheckBoxDialogControl(111,&values[11]));
  if (selmask.entity.test(dimension_mask)) dialog.add(new CheckBoxDialogControl(112,&values[12]));
  if (selmask.entity.test(light_entity)) dialog.add(new CheckBoxDialogControl(113,&values[13]));
  if (selmask.entity.test(patch_entity)) dialog.add(new CheckBoxDialogControl(114,&values[14]));
  if (selmask.entity.test(bitmap_entity)) dialog.add(new CheckBoxDialogControl(115,&values[15]));
  dialog.add(new ESMaskButtonDialogControl(116,0,this));
  dialog.add(new ESMaskButtonDialogControl(117,1,this));
  if (dialog.process() == TRUE)
    {  tempentitymask.set(xyz_mask,values[0]);
       tempentitymask.set(point_entity,values[1]);
       tempentitymask.set(line_entity,values[2]);
       tempentitymask.set(circle_entity,values[3]);
       tempentitymask.set(ellipse_entity,values[4]);
       tempentitymask.set(curve_entity,values[5]);
       tempentitymask.set(plane_entity,values[6]);
       tempentitymask.set(ruledsurface_entity,values[7]);
       tempentitymask.set(surfaceofrev_entity,values[8]);
       tempentitymask.set(text_entity,values[9]);
       tempentitymask.set(figure_entity,values[10]);
       tempentitymask.set(group_entity,values[11]);
       tempentitymask.set(dimension_mask,values[12]);
       tempentitymask.set(light_entity,values[13]);
       tempentitymask.set(patch_entity,values[14]);
       tempentitymask.set(bitmap_entity,values[15]);
    }
}

// KMJ : New Entity Mask command
void StateMachine::setupentitymaskEx(void)
{
  Dialog dialog(_RCT("MaskChange_Dialog"));
  int values[16];
  values[0] = tempentitymask.test(xyz_mask);
  values[1] = tempentitymask.test(point_entity);
  values[2] = tempentitymask.test(line_entity);
  values[3] = tempentitymask.test(circle_entity);
  values[4] = tempentitymask.test(ellipse_entity);
  values[5] = tempentitymask.test(curve_entity);
  values[6] = tempentitymask.test(plane_entity);
  values[7] = tempentitymask.test(ruledsurface_entity);
  values[8] = tempentitymask.test(surfaceofrev_entity);
  values[9] = tempentitymask.test(text_entity);
  values[10] = tempentitymask.test(figure_entity);
  values[11] = tempentitymask.test(group_entity);
  values[12] = tempentitymask.test(dimension_mask);
  values[13] = tempentitymask.test(light_entity);
  values[14] = tempentitymask.test(patch_entity);
  values[15] = tempentitymask.test(bitmap_entity);
  if (selmask.entity.test(xyz_mask)) dialog.add(new CheckBoxDialogControl(100,&values[0]));
  if (selmask.entity.test(point_entity)) dialog.add(new CheckBoxDialogControl(101,&values[1]));
  if (selmask.entity.test(line_entity)) dialog.add(new CheckBoxDialogControl(102,&values[2]));
  if (selmask.entity.test(circle_entity)) dialog.add(new CheckBoxDialogControl(103,&values[3]));
  if (selmask.entity.test(ellipse_entity)) dialog.add(new CheckBoxDialogControl(104,&values[4]));
  if (selmask.entity.test(curve_entity)) dialog.add(new CheckBoxDialogControl(105,&values[5]));
  if (selmask.entity.test(plane_entity)) dialog.add(new CheckBoxDialogControl(106,&values[6]));
  if (selmask.entity.test(ruledsurface_entity)) dialog.add(new CheckBoxDialogControl(107,&values[7]));
  if (selmask.entity.test(surfaceofrev_entity)) dialog.add(new CheckBoxDialogControl(108,&values[8]));
  if (selmask.entity.test(text_entity)) dialog.add(new CheckBoxDialogControl(109,&values[9]));
  if (selmask.entity.test(figure_entity)) dialog.add(new CheckBoxDialogControl(110,&values[10]));
  if (selmask.entity.test(group_entity)) dialog.add(new CheckBoxDialogControl(111,&values[11]));
  if (selmask.entity.test(dimension_mask)) dialog.add(new CheckBoxDialogControl(112,&values[12]));
  if (selmask.entity.test(light_entity)) dialog.add(new CheckBoxDialogControl(113,&values[13]));
  if (selmask.entity.test(patch_entity)) dialog.add(new CheckBoxDialogControl(114,&values[14]));
  if (selmask.entity.test(bitmap_entity)) dialog.add(new CheckBoxDialogControl(115,&values[15]));
  dialog.add(new ESMaskButtonDialogControl(116,0,this));
  dialog.add(new ESMaskButtonDialogControl(117,1,this));
  if (dialog.process() == TRUE)
    {  tempentitymask.set(xyz_mask,values[0]);
       tempentitymask.set(point_entity,values[1]);
       tempentitymask.set(line_entity,values[2]);
       tempentitymask.set(circle_entity,values[3]);
       tempentitymask.set(ellipse_entity,values[4]);
       tempentitymask.set(curve_entity,values[5]);
       tempentitymask.set(plane_entity,values[6]);
       tempentitymask.set(ruledsurface_entity,values[7]);
       tempentitymask.set(surfaceofrev_entity,values[8]);
       tempentitymask.set(text_entity,values[9]);
       tempentitymask.set(figure_entity,values[10]);
       tempentitymask.set(group_entity,values[11]);
       tempentitymask.set(dimension_mask,values[12]);
       tempentitymask.set(light_entity,values[13]);
       tempentitymask.set(patch_entity,values[14]);
       tempentitymask.set(bitmap_entity,values[15]);
    }
}

class RDRadioButtonDialogControl : public RadioButtonDialogControl
  {private:
     int *oldcs;
   public:
     RDRadioButtonDialogControl(int id,int id1,int id2,int *value,int *ocs) : RadioButtonDialogControl(id,id1,id2,value) {  oldcs = ocs;  *oldcs = *value;  }
     void load(Dialog *);
     int select(Dialog *);
     void update(Dialog *);
  };

Point StateMachine::from(int csystem,Point p,int ix,int iy,int iz)
{Point lastp;
 double r,a;
  lastp = getposition();
  if (db.workplanes.getcurrent() != NULL)
    lastp = db.workplanes.getcurrent()->modeltoworkplane(lastp);
  switch (csystem)
    {  case 0 :
         if (ix) p.x += lastp.x;
         if (iy) p.y += lastp.y;
         if (iz) p.z += lastp.z;
         break;
       case 1 :
         r = p.x;  a = p.y * M_PI / 180.0;
         p.x = r * cos(a);  p.y = r * sin(a);
         if (ix || iy)
          {  p.x += lastp.x;  p.y += lastp.y;
          }
         if (iz) p.z += lastp.z;
         break;
       case 2 :
         r = p.y;  a = p.x * M_PI / 180.0;
         p.x = r * sin(a);  p.y = r * cos(a);
         if (ix || iy)
          {  p.x += lastp.x;  p.y += lastp.y;
          }
         if (iz) p.z += lastp.z;
         break;
    }
  if (db.workplanes.getcurrent() != NULL)
    p = db.workplanes.getcurrent()->workplanetomodel(p);
  return p;
}

void RDRadioButtonDialogControl::update(Dialog *dialog)
{int cs;
 Point p;
 double r,cosa,sina,a;
  if (dialog->currentvalue(100,&cs) &&
      dialog->currentvalue(102,&p.x) &&
      dialog->currentvalue(104,&p.y) &&
      dialog->currentvalue(106,&p.z))
    {  p = state.from(*oldcs,p,0,0,0);
       if (cs == 0)
         {  dialog->SetDlgItemText(202,_RCT("X"));
            dialog->SetDlgItemText(204,_RCT("Y"));
            dialog->SetDlgItemText(206,_RCT("Z"));
            ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,p.x);
            ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,p.y);
            ((RealDialogControl *)dialog->getcontrol(106))->change(dialog,p.z);
         }
       else if (cs == 1)
         {  dialog->SetDlgItemText(202,_RCT("D"));
            dialog->SetDlgItemText(204,_RCT("A"));
            dialog->SetDlgItemText(206,_RCT("Z"));
            r = sqrt(p.x * p.x + p.y * p.y);
            if (fabs(r) > db.getptoler())
              {  cosa = p.x / r;  sina = p.y / r;
              }
            else
              {  cosa = 1.0;  sina = 0.0;
              }
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            a = acos(cosa);
            if (sina < 0.0) a = 2.0 * M_PI - a;
            a *= 180.0 / M_PI;
            ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,r);
            ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,a);
            ((RealDialogControl *)dialog->getcontrol(106))->change(dialog,p.z);
         }
       //((CheckBoxDialogControl *)dialog->getcontrol(103))->change(dialog,0);
       //((CheckBoxDialogControl *)dialog->getcontrol(105))->change(dialog,0);
       //((CheckBoxDialogControl *)dialog->getcontrol(107))->change(dialog,0);
       *oldcs = cs;
    }

    // this is for the Lt and Draft versions
  if (program->getlicensed() >= 2)
    EnableWindow(GetDlgItem(dialog->gethdlg(),106),0);

}

void RDRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  update(dialog);
}

int RDRadioButtonDialogControl::select(Dialog *dialog)
{int status;
  status = RadioButtonDialogControl::select(dialog);
  update(dialog);
  return status;
}

/**
 * @brief StateMachine::fromposition
 *
 * dialog for reference from position
 * used by position_command1
 *
 */
void StateMachine::fromposition(void)
{
 Dialog dialog(_RCT("Reference_Dialog"));
 int cs,ix,iy,iz,oldcs;
 View3dWindow *window;
 Point p,xaxis,yaxis;
  cs = v.getinteger("rf::cs");
  p = getposition();
  ix = iy = iz = 1;
  p.x = p.y = p.z = 0.0;
  dialog.add(new RDRadioButtonDialogControl(100,100,101,&cs,&oldcs));
  dialog.add(new RDRadioButtonDialogControl(101,100,101,&cs,&oldcs));
  dialog.add(new RealDialogControl(102,&p.x,ScaleByLength));
  dialog.add(new RealDialogControl(104,&p.y,ScaleByLength));
  dialog.add(new RealDialogControl(106,&p.z,ScaleByLength));
  //dialog.add(new CheckBoxDialogControl(103,&ix));
  //dialog.add(new CheckBoxDialogControl(105,&iy));
  //dialog.add(new CheckBoxDialogControl(107,&iz));
  if (dialog.process() == TRUE)
    {  v.setinteger("rf::cs",cs);
       p = from(cs,p,ix,iy,iz);
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
       sendevent(cevent);
    }
}

void StateMachine::fail(StatusCode code,const char *message)
{
    fail(  code,CHAR2RCCHR(message));
}

void StateMachine::fail(StatusCode code,const RCCHAR *message)
{RCCHAR *prefix,m[256];
 ResourceString rs12(NCWIN+12);
 ResourceString rs13(NCWIN+13);
 ResourceString rs14(NCWIN+14);
  if (code == Memory)
    prefix = rs12.gets();
  else if (code == ButtonMenuFull)
    prefix = rs13.gets();
  strcpy(m,prefix);  if (message != NULL) strcat(m,message);
  cadwindow->MessageBox(m,rs14.gets(),MB_ICONSTOP);
  PostQuitMessage(0);
}

void StateMachine::setposition(Point p)
{
  if (cadwindow && cadwindow->coordinateentry )
    cadwindow->coordinateentry->position(p);
  pos = p;
}

void StateMachine::sendevent(HighLevelEvent *nextevent)
{int inlist;

  if (cadwindow != 0)
    SetFocus(cadwindow->gethwnd());
  if (event != NULL) delete event;
  if ((event = nextevent) == NULL)
      return;
  if (event->isa(EntitySelected))
    {  if (! ((EntitySelectedEvent *)event)->getaddtoselection())
         {  selection.unhighlight();
            selection.destroy();
            mainmenu.selectionchanged(selection.length());
         }

       //  If add to selection is greater than or equal to 10, inlist has been precalculated by the calling function
       if (((EntitySelectedEvent *)event)->getaddtoselection() >= 20)
         inlist = 1;
       else if (((EntitySelectedEvent *)event)->getaddtoselection() >= 10)
         inlist = 0;
       else
         inlist = selection.inlist(((EntitySelectedEvent *)event)->getentity());

       if ((((EntitySelectedEvent *)event)->getaddtoselection()%10) == 1 && inlist)
         {  if (((EntitySelectedEvent *)event)->getaddtoselection() < 10)
              {//  If >= 10 then this entry has already been removed from the selection list
                 selection.del(((EntitySelectedEvent *)event)->getentity());
              }
            (((EntitySelectedEvent *)event)->getentity())->unhighlight();
            lastend = 0;
            lastpickpoint.setxyz(0.0,0.0,0.0);
         }
       else
         {
		   Entity* entity = ((EntitySelectedEvent *)event)->getentity();
            selection.add(entity);
						
            lastend = ((EntitySelectedEvent *)event)->getendselected();
            lastpickpoint = ((EntitySelectedEvent *)event)->getp();
         }

       if (((EntitySelectedEvent *)event)->getaddtoselection() < 10)
         mainmenu.selectionchanged(selection.length());

       if (((EntitySelectedEvent *)event)->getwindow() != NULL)
         {  lastvisible = ((EntitySelectedEvent *)event)->getwindow()->getvisibility();
            db.header.setvisible(lastvisible);
            strcpy(lastviewname,((EntitySelectedEvent *)event)->getwindow()->getview()->getname());
            lastviewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
         }
    }
  else if (event->isa(Coordinate3d))
    {
       //Coordinate3dEvent evtmp = *((Coordinate3dEvent *) event);
       setposition(((Coordinate3dEvent *) event)->getp());
       //event = (HighLevelEvent*)&evtmp;
       if (((Coordinate3dEvent *)event)->getwindow() != NULL)
       {
            db.header.setvisible(((Coordinate3dEvent *)event)->getwindow()->getvisibility());
            strcpy(lastviewname,((Coordinate3dEvent *)event)->getwindow()->getview()->getname());
            lastviewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
       }
    }
  else
    {
       if(cadwindow->statusprompt != 0)
           cadwindow->statusprompt->normalprompt(_RCT(""));
       if (event->isa(Abort))
       {
           BitMask mask(32);
            mask = tempentitymask;
            mask &= selmask.entity;
            if (! mask.test(uv_mask) && commands.subcommand(commandid) != 3)
              destroyselection(1);
            mainmenu.selectionchanged(selection.length());
       }
    }
}

void StateMachine::setselection(EntityList list,int highlighted)
{Entity *e;
  if (! (highlighted & 2))
      selection.unhighlight();
  selection.destroy();
  selection = list;
  if (! (highlighted & 1))
    {  cadwindow->startdrawingbatch();
       for (list.start() ; (e = list.next()) != NULL ; )
         e->highlight();
	   
       cadwindow->enddrawingbatch();
    }
  mainmenu.selectionchanged(selection.length());
}

void StateMachine::removefromselection(Entity *e)
{ selection.del(e);
  mainmenu.selectionchanged(selection.length());
}

void StateMachine::addtoselection(Entity *e)
{ if (! e->getselected())
    e->highlight();
  if (! selection.inlist(e))
    selection.add(e);
}

void StateMachine::destroyselection(int draw)
{
    if (handledentity != 0)
    {   handledentity->draw(DM_ERASE);
        handledentity->sethandles(0);
        if (! draw)
            handledentity->draw(DM_NORMAL);
        db.Selection(0);
    }
    if (draw)
        selection.unhighlight();
    selection.destroy();
    handledentity = 0;
    mainmenu.selectionchanged(selection.length());
}

static void SetCopyE0(Entity *e,void *)
{ e->setcopye(0);
}

void StateMachine::drawdraggerGL(int mode, View3dSurface *v)
{
    if(glDraggerLine != 0)
    {
        glDraggerLine->drawGL(mode,0,v);
    }
}

void StateMachine::drawdragGL(Point p)
{Transform t;
 Entity *e;
 BitMask *lockedlayertable;
 View *view3d;

 // KMJ do we need this? insert gl lists maybe??
  //cadwindow->startdrawingbatch();

 //qDebug() << "entering drawdrag";

  if (dragon == 2)
    {  if (handledentity != 0)
         {  handledentity->setcopye(0);
            handledentity->dbtraverseassociated(SetCopyE0,0);
            handledentity->clone(t);
            handledentity->getcopye()->movehandlepoint(lastdragwindow,0,dragdrawn-1,p);
#ifdef USING_WIDGETS
            //handledentity->getcopye()->qgi = qgiDragger;
            //handledentity->getcopye()->draw(DM_INVERT);
            //qgiDragger = handledentity->getcopye()->qgi;
            //if(qgiDragger)
            //    qgiDragger->setZValue(QGMAXZVALUE+QGZVALINC); // always on top
            cadwindow->clearAllRubberBands();
            handledentity->getcopye()->draw(DM_INVERT);
            delete handledentity->getcopye();
#else
            handledentity->getcopye()->draw(DM_INVERT);
            delete handledentity->getcopye();
#endif

         }
       else
         dragon = 0;
    }
  else
    {  if (dragdrawn == 1)
         {  t.translate(p-startdragpos);
          glDraggerTform = t;
          lockedlayertable = 0;
            if ((view3d = db.views3d.match(lastviewname)) != 0)
              lockedlayertable = view3d->getlockedlayertable();
            if (lockedlayertable == 0)
              lockedlayertable = &db.lockedmask.layer;

            cadwindow->clearAllRubberBands();
            for (selection.start() ; (e = selection.next()) != NULL ; )
              if (lastcontrol || ! lockedlayertable->test(e->getlayer()))
              {
                  //e->draw(DM_ERASE,&t);
                  //e->draw(DM_INVERT,&t);
              }
            cadwindow->paintall();
         }
       else
         {
            cadwindow->clearAllRubberBands();
#if 0
            Line line(p,startdragpos);
            line.qgi = qgiDragger;
            line.draw(DM_INVERT);
            qgiDragger = line.qgi;
            line.draw(DM_ERASE);
#else
            // for gl set the dragger line
            if(glDraggerLine)
            {
                glDraggerLine->setpts(p,startdragpos);
                cadwindow->paintall();
            }
#endif
         }
    }

  //cadwindow->enddrawingbatch();

}

void StateMachine::drawdrag(Point p)
{Transform t;
 Entity *e;
 BitMask *lockedlayertable;
 View *view3d;

 // KMJ do we need this? insert gl lists maybe??
  //cadwindow->startdrawingbatch();

 //qDebug() << "entering drawdrag";

  if (dragon == 2)
    {  if (handledentity != 0)
         {  handledentity->setcopye(0);
            handledentity->dbtraverseassociated(SetCopyE0,0);
            handledentity->clone(t);
            handledentity->getcopye()->movehandlepoint(lastdragwindow,0,dragdrawn-1,p);
#ifdef USING_WIDGETS
            //handledentity->getcopye()->qgi = qgiDragger;
            //handledentity->getcopye()->draw(DM_INVERT);
            //qgiDragger = handledentity->getcopye()->qgi;
            //if(qgiDragger)
            //    qgiDragger->setZValue(QGMAXZVALUE+QGZVALINC); // always on top
            cadwindow->clearAllRubberBands();
            handledentity->getcopye()->draw(DM_INVERT);
            delete handledentity->getcopye();
#else
            handledentity->getcopye()->draw(DM_INVERT);
            delete handledentity->getcopye();
#endif

         }
       else
         dragon = 0;
    }
  else
    {  if (dragdrawn == 1)
         {  t.translate(p-startdragpos);

            lockedlayertable = 0;
            if ((view3d = db.views3d.match(lastviewname)) != 0)
              lockedlayertable = view3d->getlockedlayertable();
            if (lockedlayertable == 0)
              lockedlayertable = &db.lockedmask.layer;

            cadwindow->clearAllRubberBands();
            for (selection.start() ; (e = selection.next()) != NULL ; )
              if (lastcontrol || ! lockedlayertable->test(e->getlayer()))
              {
                  //e->draw(DM_ERASE,&t);
                  e->draw(DM_INVERT,&t);
              }
         }
       else
         {
            cadwindow->clearAllRubberBands();
            Line line(p,startdragpos);
            line.qgi = qgiDragger;
            line.draw(DM_INVERT);
            qgiDragger = line.qgi;
            line.draw(DM_ERASE);
         }
    }

  //cadwindow->enddrawingbatch();

}

void StateMachine::startdragGL(Point p,View3dWindow *w,int handlenumber)
{int vn;

  if (handlenumber != 0)
    {  dragon = 2;
       dragdrawn = 1 + handlenumber;
       lastdragpos = startdragpos = p;
       lastdragwindow = w;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
       drawdragGL(lastdragpos);
    }
  else
    {

       if (v.getinteger("db::dragstyle") == 1)
         db.saveundo(UD_STARTBLOCK,NULL);

       if (dragon && dragdrawn)
         {  vn = lastdragwindow->getview()->getviewno();
            lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
            drawdragGL(lastdragpos);
            lastdragwindow->getview()->setviewno(vn);
         }
       setposition(p);
       dragon = 1;
       dragdrawn = 1 + db.getdragdetail();
       lastdragpos = startdragpos = p;
       lastdragwindow = w;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

       // for gl low detail dragger line
       delete glDraggerLine;
       glDraggerLine = new Line(p,lastdragpos);

       drawdragGL(lastdragpos);

    }
}

void StateMachine::startdrag(Point p,View3dWindow *w,int handlenumber)
{int vn;

  if (handlenumber != 0)
    {  dragon = 2;
       dragdrawn = 1 + handlenumber;
       lastdragpos = startdragpos = p;
       lastdragwindow = w;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
       drawdrag(lastdragpos);
    }
  else
    {

       if (v.getinteger("db::dragstyle") == 1)
         db.saveundo(UD_STARTBLOCK,NULL);

       if (dragon && dragdrawn)
         {  vn = lastdragwindow->getview()->getviewno();
            lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
            drawdrag(lastdragpos);
            lastdragwindow->getview()->setviewno(vn);
         }
       setposition(p);
       dragon = 1;
       dragdrawn = 1 + db.getdragdetail();
       lastdragpos = startdragpos = p;
       lastdragwindow = w;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
       drawdrag(lastdragpos);
    }
}

extern void stretchlist(EntityList *list,Transform t);

static void paintwindow(int,void *,View3dSurface *vs)
{ ((View3dWindow *)vs)->paintevent();
}

void StateMachine::dragtoGL(Point p,View3dWindow *w)
{int vn;
 Entity *e;
 Point xaxis,yaxis,zaxis,p1,p2;

  if (v.getinteger("db::constraindrag") == 1)
    {  selection.start();
       e = selection.next();
       if (e->isa(line_entity))
         {  xaxis = w->getxaxis();
            yaxis = w->getyaxis();
            zaxis = (xaxis.cross(yaxis)).normalize();
            xaxis = (((Line *)e)->direction(0.0).cross(zaxis)).normalize();
            yaxis = (zaxis.cross(xaxis)).normalize();
            p = startdragpos + xaxis * ((p - startdragpos).dot(xaxis));
         }
       else if (e->isa(plane_entity))
         {  p1 = w->modeltoscreen(startdragpos);
            p2 = w->modeltoscreen(startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())));
            p = w->modeltoscreen(p);
            p = Line(p1,p2).nearp(p);
            p = startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())) * ((p - p1).length()/(p2 - p1).length()) * ((p - p1).dot(p2 - p1) > 0.0 ? 1.0 : -1.0);
         }

    }

  if (dragon == 2)
    {  if (dragdrawn)
         drawdragGL(lastdragpos);
       lastdragpos = p;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
       drawdragGL(lastdragpos);
    }
  else if (dragon == 1)
    {

       if (v.getinteger("db::dragstyle") == 1)
         {Transform t;
            t.translate(p-startdragpos);
            startdragpos = p;
            cadwindow->disabledraw();
            stretchlist(&selection,t);
            cadwindow->enabledraw();
            dragdrawn = 0;
            //cadwindow->drawcallback(DM_NORMAL,0,paintwindow);
            cadwindow->drawcallback(RB_DRAW,0,paintwindow); // removes markers
            cadwindow->clearAllRubberBands(); // removes rubberbands
            return;
         }


        if (dragdrawn)
          {  vn = lastdragwindow->getview()->getviewno();
             lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
             drawdragGL(lastdragpos);
             lastdragwindow->getview()->setviewno(vn);
          }
        lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        lastdragpos = p;
        dragdrawn = 1 + db.getdragdetail();
        vn = w->getview()->getviewno();
        w->getview()->setviewno(1000);    //  Make all entities visible in this view
        drawdragGL(lastdragpos);
        w->getview()->setviewno(vn);
        lastdragwindow = w;
     }
}

void StateMachine::dragto(Point p,View3dWindow *w)
{int vn;
 Entity *e;
 Point xaxis,yaxis,zaxis,p1,p2;

  if (v.getinteger("db::constraindrag") == 1)
    {  selection.start();
       e = selection.next();
       if (e->isa(line_entity))
         {  xaxis = w->getxaxis();
            yaxis = w->getyaxis();
            zaxis = (xaxis.cross(yaxis)).normalize();
            xaxis = (((Line *)e)->direction(0.0).cross(zaxis)).normalize();
            yaxis = (zaxis.cross(xaxis)).normalize();
            p = startdragpos + xaxis * ((p - startdragpos).dot(xaxis));
         }
       else if (e->isa(plane_entity))
         {  p1 = w->modeltoscreen(startdragpos);
            p2 = w->modeltoscreen(startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())));
            p = w->modeltoscreen(p);
            p = Line(p1,p2).nearp(p);
            p = startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())) * ((p - p1).length()/(p2 - p1).length()) * ((p - p1).dot(p2 - p1) > 0.0 ? 1.0 : -1.0);
         }

    }

  if (dragon == 2)
    {  if (dragdrawn)
         drawdrag(lastdragpos);
       lastdragpos = p;
       lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
       drawdrag(lastdragpos);
    }
  else if (dragon == 1)
    {

       if (v.getinteger("db::dragstyle") == 1)
         {Transform t;
            t.translate(p-startdragpos);
            startdragpos = p;
            cadwindow->disabledraw();
            stretchlist(&selection,t);
            cadwindow->enabledraw();
            dragdrawn = 0;
            //cadwindow->drawcallback(DM_NORMAL,0,paintwindow);
            cadwindow->drawcallback(RB_DRAW,0,paintwindow); // removes markers
            cadwindow->clearAllRubberBands(); // removes rubberbands
            return;
         }


        if (dragdrawn)
          {  vn = lastdragwindow->getview()->getviewno();
             lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
             drawdrag(lastdragpos);
             lastdragwindow->getview()->setviewno(vn);
          }
        lastcontrol = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        lastdragpos = p;
        dragdrawn = 1 + db.getdragdetail();
        vn = w->getview()->getviewno();
        w->getview()->setviewno(1000);    //  Make all entities visible in this view
        drawdrag(lastdragpos);
        w->getview()->setviewno(vn);
        lastdragwindow = w;
     }
}

void StateMachine::moveselectionGL(Point p,View3dWindow *w)
{Transform t;
 Entity *e;
 EntityList movedlist,s;
 int i;
 BitMask *lockedlayertable;
 View *view3d;
 Point xaxis,yaxis,zaxis,p1,p2;

  if ((p - startdragpos).length() < db.getptoler()) return;

  if (v.getinteger("db::constraindrag") == 1)
    {  selection.start();
       e = selection.next();
       if (e->isa(line_entity))
         {  xaxis = w->getxaxis();
            yaxis = w->getyaxis();
            zaxis = (xaxis.cross(yaxis)).normalize();
            xaxis = (((Line *)e)->direction(0.0).cross(zaxis)).normalize();
            yaxis = (zaxis.cross(xaxis)).normalize();
            p = startdragpos + xaxis * ((p - startdragpos).dot(xaxis));
         }
       else if (e->isa(plane_entity))
         {  p1 = w->modeltoscreen(startdragpos);
            p2 = w->modeltoscreen(startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())));
            p = w->modeltoscreen(p);
            p = Line(p1,p2).nearp(p);
            p = startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())) * ((p - p1).length()/(p2 - p1).length()) * ((p - p1).dot(p2 - p1) > 0.0 ? 1.0 : -1.0);
         }

    }

  if (v.getinteger("db::dragstyle") == 1)
    {  t.translate(p-startdragpos);
       cadwindow->disabledraw();
       stretchlist(&selection,t);
       cadwindow->enabledraw();
       db.saveundo(UD_ENDBLOCK,NULL);
       cadwindow->paintall();
       return;
    }

  cadwindow->startdrawingbatch();

  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  t.translate(p-startdragpos);

  lockedlayertable = 0;
  if ((view3d = db.views3d.match(lastviewname)) != 0)
    lockedlayertable = view3d->getlockedlayertable();
  if (lockedlayertable == 0)
    lockedlayertable = &db.lockedmask.layer;

  for (selection.start() ; (e = selection.next()) != NULL ; )
    if (! lockedlayertable->test(e->getlayer()))
      s.add(e);
  s.reverse();

  for (s.start() ; (e = s.next()) != NULL ; )
    {  e->dbsetassociated();
       e->setstatus(1);
    }

  for (s.start() ; (e = s.next()) != NULL ; )
    if (e->getalist().empty()) e->drawGL(DM_ERASE);
  for (s.start() ; (e = s.next()) != NULL ; )
    if (e->getalist().empty()) e->move(&db,t,1);
  for (s.start() ; (e = s.next()) != NULL ; )
    if (! e->getalist().empty()) e->move(&db,t,1);
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )  //  Make visible in the view that the entities were dropped
    if (e->getcopye() != 0)
      {  //  If the entity is not visible in any view, leave it blank.
         for (i = 0 ; i < MaxViews ; i++)
           if (e->getcopye()->getvisible()->test(i))
             {  e->getcopye()->setvisible(w->getvisibility());
                break;
             }
         //e->getcopye()->drawGL(DM_NORMAL);
      }
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (! e->isa(group_entity) && e->getstatus())
      {
        movedlist.add(e);
        //e->drawGL(DM_NORMAL);
      }

  //  Note the following code is not proven correct for all cases of nested groups
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (e->isa(group_entity) && e->getstatus())
      {
        movedlist.add(e);
        //e->drawGL(DM_NORMAL);
      }

  db.saveundo(UD_MANYMOVED,&movedlist);
  movedlist.destroy();
  db.saveundo(UD_ENDBLOCK,NULL);
  s.destroy();

  cadwindow->enddrawingbatch();

}

void StateMachine::moveselection(Point p,View3dWindow *w)
{Transform t;
 Entity *e;
 EntityList movedlist,s;
 int i;
 BitMask *lockedlayertable;
 View *view3d;
 Point xaxis,yaxis,zaxis,p1,p2;

  if ((p - startdragpos).length() < db.getptoler()) return;

  if (v.getinteger("db::constraindrag") == 1)
    {  selection.start();
       e = selection.next();
       if (e->isa(line_entity))
         {  xaxis = w->getxaxis();
            yaxis = w->getyaxis();
            zaxis = (xaxis.cross(yaxis)).normalize();
            xaxis = (((Line *)e)->direction(0.0).cross(zaxis)).normalize();
            yaxis = (zaxis.cross(xaxis)).normalize();
            p = startdragpos + xaxis * ((p - startdragpos).dot(xaxis));
         }
       else if (e->isa(plane_entity))
         {  p1 = w->modeltoscreen(startdragpos);
            p2 = w->modeltoscreen(startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())));
            p = w->modeltoscreen(p);
            p = Line(p1,p2).nearp(p);
            p = startdragpos + (((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis())) * ((p - p1).length()/(p2 - p1).length()) * ((p - p1).dot(p2 - p1) > 0.0 ? 1.0 : -1.0);
         }

    }

  if (v.getinteger("db::dragstyle") == 1)
    {  t.translate(p-startdragpos);
       cadwindow->disabledraw();
       stretchlist(&selection,t);
       cadwindow->enabledraw();
       db.saveundo(UD_ENDBLOCK,NULL);
       cadwindow->paintall();
       return;
    }

  cadwindow->startdrawingbatch();

  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  t.translate(p-startdragpos);

  lockedlayertable = 0;
  if ((view3d = db.views3d.match(lastviewname)) != 0)
    lockedlayertable = view3d->getlockedlayertable();
  if (lockedlayertable == 0)
    lockedlayertable = &db.lockedmask.layer;

  for (selection.start() ; (e = selection.next()) != NULL ; )
    if (! lockedlayertable->test(e->getlayer()))
      s.add(e);
  s.reverse();

  for (s.start() ; (e = s.next()) != NULL ; )
    {  e->dbsetassociated();
       e->setstatus(1);
    }

  for (s.start() ; (e = s.next()) != NULL ; )
    if (e->getalist().empty()) e->draw(DM_ERASE);
  for (s.start() ; (e = s.next()) != NULL ; )
    if (e->getalist().empty()) e->move(&db,t,1);
  for (s.start() ; (e = s.next()) != NULL ; )
    if (! e->getalist().empty()) e->move(&db,t,1);
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )  //  Make visible in the view that the entities were dropped
    if (e->getcopye() != 0)
      {  //  If the entity is not visible in any view, leave it blank.
         for (i = 0 ; i < MaxViews ; i++)
           if (e->getcopye()->getvisible()->test(i))
             {  e->getcopye()->setvisible(w->getvisibility());
                break;
             }
         e->getcopye()->draw(DM_NORMAL);
      }
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (! e->isa(group_entity) && e->getstatus())
      {  movedlist.add(e);  e->draw(DM_NORMAL);
      }

  //  Note the following code is not proven correct for all cases of nested groups
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (e->isa(group_entity) && e->getstatus())
      {  movedlist.add(e);  e->draw(DM_NORMAL);
      }

  db.saveundo(UD_MANYMOVED,&movedlist);
  movedlist.destroy();
  db.saveundo(UD_ENDBLOCK,NULL);
  s.destroy();

  cadwindow->enddrawingbatch();

}

void StateMachine::copyselectionGL(Point p,View3dWindow *w)
{Transform t;
 Entity *e,*starte;
 int i;

#ifdef NEW_DRAGGING
 cadwindow->startdrawingbatch();
#endif

  if ((p - startdragpos).length() < db.getptoler()) return;
  db.header.setvisible(w->getvisibility());
  t.translate(p-startdragpos);
  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  db.geometry.start();
  starte = db.geometry.next();
  for (selection.start(); (e = selection.next()) != 0; )
  {
	  if (e->getcopye() == 0) {
		  Entity* copye = e->clone(t);
		  db.geometry.add(copye);
		  
		  if (db.CopyAlogEntityCallback != 0 && db.copyAlogMap.contains(e->getid())) {
			  copyAlongDialogData dlgData = db.copyAlogMap[e->getid()];			 
			  db.copyAlogMap.insert(copye->getid(), dlgData.clone(t,e->isa(circle_entity)));
			  copye->setRefId(copye->getid());

		  }
	  }
    }
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 && e != starte ; )  //  Make visible in the view that the entities were dropped
    {  for (i = 0 ; i < MaxViews ; i++)
         if (e->getvisible()->test(i))
           {  e->setvisible(w->getvisibility());
              break;
           }
       e->drawGL(DM_NORMAL);
    }
  db.saveundo(UD_ENDBLOCK,NULL);

#ifdef NEW_DRAGGING
  cadwindow->enddrawingbatch();
#endif

}

void StateMachine::copyselection(Point p,View3dWindow *w)
{Transform t;
 Entity *e,*starte;
 int i;
  if ((p - startdragpos).length() < db.getptoler()) return;
  db.header.setvisible(w->getvisibility());
  t.translate(p-startdragpos);
  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  db.geometry.start();
  starte = db.geometry.next();
  for (selection.start() ; (e = selection.next()) != 0 ; )
    {  if (e->getcopye() == 0)
         db.geometry.add(e->clone(t),2);
    }
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 && e != starte ; )  //  Make visible in the view that the entities were dropped
    {  for (i = 0 ; i < MaxViews ; i++)
         if (e->getvisible()->test(i))
           {  e->setvisible(w->getvisibility());
              break;
           }
       e->draw(DM_NORMAL);
    }
  db.saveundo(UD_ENDBLOCK,NULL);
}

void StateMachine::enddragGL(void)
{int vn;
  if (dragon == 2)
    {
      if (dragdrawn)
         drawdragGL(lastdragpos);
#ifdef USING_WIDGETS
      cadwindow->clearAllRubberBands();
#endif
    }
  else if (dragon == 1)
    {  if (dragdrawn)
         {  vn = lastdragwindow->getview()->getviewno();
            lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
            drawdragGL(lastdragpos);
            cadwindow->clearAllRubberBands();
            lastdragwindow->getview()->setviewno(vn);
            // for gl
            glDraggerTform.identity();
            delete glDraggerLine;
            glDraggerLine = 0;
         }
    }
  dragon = 0;
  regenGLlists();
}

void StateMachine::enddrag(void)
{int vn;
  if (dragon == 2)
    {
      if (dragdrawn)
         drawdrag(lastdragpos);
      cadwindow->clearAllRubberBands();
    }
  else if (dragon == 1)
    {  if (dragdrawn)
         {  vn = lastdragwindow->getview()->getviewno();
            lastdragwindow->getview()->setviewno(1000);    //  Make all entities visible in this view
            drawdrag(lastdragpos);
            cadwindow->clearAllRubberBands();
            lastdragwindow->getview()->setviewno(vn);
         }
    }
  dragon = 0;
}

int StateMachine::eventavailable(void)
{ return event != NULL;
}

void MainMenu::selectionchanged(int enable)
{
 bool grayed;
 int i,allunlocked;
 QMenuBar* hmenu;
 EntityList list;
 BitMask *lockedlayertable;
 Entity *e;

  if (cadwindow == 0)
    return;

  if(cadwindow->gethwnd() == 0)
      return;

  //hmenu = (QMenuBar*)((QtMainWindow*)cadwindow->gethwnd())->menuBar;
  //heditmenu = ((QtMainWindow*)cadwindow->gethwnd())->menu_Edit;//GetSubMenu(hmenu,1);
  QMenu *qeditmenu = ((QtMainWindow*)cadwindow->gethwnd())->menu_Edit;//GetSubMenu(hmenu,1);
  //htransformmenu = ((QtMainWindow*)cadwindow->gethwnd())->menu_Transform;//GetSubMenu(hmenu,2);
  //harrangemenu = ((QtMainWindow*)cadwindow->gethwnd())->menu_Arrange;//GetSubMenu(hmenu,4);

  allunlocked = 1;

  if (cadwindow->getcurrentwindow() == 0 || cadwindow->getcurrentwindow()->getview()->getlockedlayertable() == 0)
    lockedlayertable = & db.lockedmask.layer;
  else
    lockedlayertable = cadwindow->getcurrentwindow()->getview()->getlockedlayertable();

  list = state.getselection();
  for (list.start() ; (e = list.next()) != 0 ; )
    if (lockedlayertable->test(e->getlayer()))
      {  allunlocked = 0;
         break;
      }

  if (! allunlocked)
    enable = 0;

#ifdef USING_WIDGETS
  /*
  // kmj: pending
  // not working causes a crash
  grayed = (enable != 0 ? true : false);
  QList<QAction *> editActions = qeditmenu->actions();
  for(i=0; i < editActions.size(); i++)
  {
      qDebug() << "editAction[" << i << "]" << editActions.at(i)->text();
      qDebug() << "editAction[" << i << "]" << editActions.at(i)->data();
      if( !editActions.at(i)->data().isValid())
          continue;
      if(editActions.at(i)->data().toInt() >= 202 && editActions.at(i)->data().toInt() <= 210)
      {
          if(editActions.at(i)->data().toInt() != 204)
              editActions.at(i)->setEnabled(grayed);
      }
  }
 */
#else
  grayed = (enable != 0 ? MF_ENABLED : MF_GRAYED);
  for (i = 202 ; i < 210 ; i++)
    if (i != 204)
      EnableMenuItem(heditmenu,i,MF_BYCOMMAND | grayed);

  for (i = 301 ; i <= 308 ; i++)
    EnableMenuItem(htransformmenu,i,MF_BYCOMMAND | grayed);
  for (i = 800 ; i < 804 ; i++)
    EnableMenuItem(harrangemenu,i,MF_BYCOMMAND | grayed);

  EnableMenuItem(htransformmenu,306,MF_BYCOMMAND | grayed);
#endif

}

void MainMenu::pasteredoundo(HWND hmenuselected)
{
  bool grayed;
  if (cadwindow == 0)
    return;

  heditmenu = GetSubMenu(GetMenu(cadwindow->gethwnd()),1);

  if (heditmenu == (HMENU) hmenuselected)
    {
       if (OpenClipboard(cadwindow->gethwnd()))
       {
            grayed = (GetClipboardData(cadformat) != 0 || GetClipboardData(CF_DIB) != 0  ? MF_ENABLED : MF_GRAYED);
            CloseClipboard();
       }
       else
         grayed = MF_GRAYED;
       EnableMenuItem(heditmenu,204,MF_BYCOMMAND | grayed);
       grayed = (db.getundoavailable() || state.selmask.entity.test(undo_mask) ? MF_ENABLED : MF_GRAYED);
       EnableMenuItem(heditmenu,200,MF_BYCOMMAND | grayed);
       grayed = (db.getredoavailable() ? MF_ENABLED : MF_GRAYED);
       EnableMenuItem(heditmenu,201,MF_BYCOMMAND | grayed);
    }
}
//
//  The Program Control classes
//
//
Program::Program(QObject * parent)
    : QObject(parent)
{
    haccel = 0;
    m_scene = 0;
    qpulsetimer=0;
    qautosavetimer=0;
    pluginmessboxup=false;
    herasecursor=0;
    hsizewecursor = 0;
    hwaitcursor = 0;
    harrowcursor = 0;
    hemptycursor = 0;
    hzoomcursor = 0;
    hleftclickcursor = 0;
    hrightclickcursor = 0;
    hcrosshaircursor = 0;
    hcrosshairsnapcursor = 0;
    hcrosshairentitycursor = 0;
    hcrosshairorigincursor = 0;
    hcrosshairintcursor = 0;
    hpositioncrosshaircursor = 0;
    hpositioncrosshairsnapcursor = 0;
    hpositioncrosshairentitycursor = 0;
    hpositioncrosshairorigincursor = 0;
    hpositioncrosshairintcursor = 0;

    m_interrupt=false;
    // temporary flag for when raytracing is running
    m_IsRaytracing = false;
    m_ProgressBarDlg = 0;
}

Program::Program(int & argc, char ** argv)
{
    haccel = 0;
    m_scene = 0;
    qpulsetimer=0;
    qautosavetimer=0;
    pluginmessboxup=false;
    herasecursor=0;
    hsizewecursor = 0;
    hwaitcursor = 0;
    harrowcursor = 0;
    hemptycursor = 0;
    hzoomcursor = 0;
    hleftclickcursor = 0;
    hrightclickcursor = 0;
    hcrosshaircursor = 0;
    hcrosshairsnapcursor = 0;
    hcrosshairentitycursor = 0;
    hcrosshairorigincursor = 0;
    hcrosshairintcursor = 0;
    hpositioncrosshaircursor = 0;
    hpositioncrosshairsnapcursor = 0;
    hpositioncrosshairentitycursor = 0;
    hpositioncrosshairorigincursor = 0;
    hpositioncrosshairintcursor = 0;

    m_interrupt=false;
    // temporary flag for when raytracing is running
    m_IsRaytracing = false;
    m_ProgressBarDlg = 0;
}

//Program::Program(RCHINSTANCE hinst1,int ncmdshow1,HACCEL haccel1,FARPROC fp1,FARPROC fp2,RCCHAR *cl)
Program::Program(RCHINSTANCE hinst1,int ncmdshow1,RCHACCEL haccel1,HOOKPROC fp1,HOOKPROC fp2, int argc, char ** argv)
{
    haccel = 0;
    m_scene = 0;
    qpulsetimer=0;
    qautosavetimer=0;
    pluginmessboxup=false;
    herasecursor=0;
    hsizewecursor = 0;
    hwaitcursor = 0;
    harrowcursor = 0;
    hemptycursor = 0;
    hzoomcursor = 0;
    hleftclickcursor = 0;
    hrightclickcursor = 0;
    hcrosshaircursor = 0;
    hcrosshairsnapcursor = 0;
    hcrosshairentitycursor = 0;
    hcrosshairorigincursor = 0;
    hcrosshairintcursor = 0;
    hpositioncrosshaircursor = 0;
    hpositioncrosshairsnapcursor = 0;
    hpositioncrosshairentitycursor = 0;
    hpositioncrosshairorigincursor = 0;
    hpositioncrosshairintcursor = 0;

    m_interrupt=false;
    // temporary flag for when raytracing is running
    m_IsRaytracing = false;
    m_ProgressBarDlg = 0;

    // put the command line into a single string and send to init
    RCCHAR cl;
    init(hinst1,ncmdshow1,haccel1,fp1,fp2,&cl);
}

Program::Program(RCHINSTANCE hinst1,int ncmdshow1,RCHACCEL haccel1,HOOKPROC fp1,HOOKPROC fp2,RCCHAR *cl)
{
    haccel = 0;
    m_scene = 0;
    qpulsetimer=0;
    qautosavetimer=0;
    pluginmessboxup=false;
    herasecursor=0;
    hsizewecursor = 0;
    hwaitcursor = 0;
    harrowcursor = 0;
    hemptycursor = 0;
    hzoomcursor = 0;

    m_interrupt=false;
    // temporary flag for when raytracing is running
    m_IsRaytracing = false;
    m_ProgressBarDlg = 0;

    init(hinst1,ncmdshow1,haccel1,fp1,fp2,cl);
}

bool Program::event(QEvent *e)
{
    //qDebug() << "Entering : Program::event";
    return true ;//QObject::event(e);
}

void Program::init(RCHINSTANCE hinst1,int ncmdshow1,RCHACCEL haccel1,HOOKPROC fp1,HOOKPROC fp2,RCCHAR *cl)
{
  RCCHAR text[300];

  haccel = 0;
  m_scene = 0;
  qpulsetimer=0;
  qautosavetimer=0;
  pluginmessboxup=false;
  hinst = hinst1;  ncmdshow = ncmdshow1;  haccel = haccel1;
  commandline = cl;

  m_interrupt=false;
  // temporary flag for when raytracing is running
  m_IsRaytracing = false;

  RCCursor cura,curb;
  //if( cura == curb )
  //    qDebug() << "OK";
  //else
  //    qDebug() << "NG";

  // KMJ TODO: look at this
#ifdef USING_WIDGETS
  // change the cursors depending on the background color
  QColor background(::v.getreal("wn::backgroundcolour.red")*255.0,
                    ::v.getreal("wn::backgroundcolour.green")*255.0,
                    ::v.getreal("wn::backgroundcolour.blue")*255.0);
  int reverseCursor = 0; // normal cursors
  reverseCursor = program->calcCursorFromBackground();
  if(hsizewecursor != 0) delete hsizewecursor;
  hsizewecursor = new RCCursor(IDC_SIZEWE);
  if(hwaitcursor != 0) delete hwaitcursor;
  hwaitcursor = new RCCursor(IDC_WAIT);
  if(harrowcursor != 0) delete harrowcursor;
  harrowcursor = new RCCursor(IDC_ARROW);
  if(reverseCursor)
  {
      if(hemptycursor != 0) delete hemptycursor;
      hemptycursor = LoadCursor(hinst,_RCT("Empty_cursor"));
      if(hzoomcursor != 0) delete hzoomcursor;
      hzoomcursor = LoadCursor(hinst,_RCT("Zoom_cursor"));
  }
  else
  {
    if(hemptycursor != 0) delete hemptycursor;
    hemptycursor = LoadCursor(hinst,_RCT("Empty_cursor"));
    if(hzoomcursor != 0) delete hzoomcursor;
    hzoomcursor = LoadCursor(hinst,_RCT("Zoom_cursor"));
  }
  hcursor = harrowcursor;
  hdefaultcursor = harrowcursor;
  updatecursortype(reverseCursor);
#else
  harrowcursor = LoadCursor(NULL,IDC_ARROW);
  hemptycursor = LoadCursor(hinst,_RCT("Empty_cursor"));
  hzoomcursor = LoadCursor(hinst,_RCT("Zoom_cursor"));
  hcursor = harrowcursor;
  hdefaultcursor = harrowcursor;
  updatecursortype();
#endif

  GetPrivateProfileString("Cursor","DemoMode","",text,300,home.getinifilename());
  if (strlen(text) > 0)
  {
      if(reverseCursor)
      {
           if(hleftclickcursor != 0) delete hleftclickcursor;
           hleftclickcursor =  LoadCursor(hinst,_RCT("LeftClick_cursor"));
           if(hrightclickcursor != 0) delete hrightclickcursor;
           hrightclickcursor = LoadCursor(hinst,_RCT("RightClick_cursor"));
      }
      else
      {
           if(hleftclickcursor != 0) delete hleftclickcursor;
           hleftclickcursor =  LoadCursor(hinst,_RCT("LeftClick_cursor"));
           if(hrightclickcursor != 0) delete hrightclickcursor;
           hrightclickcursor = LoadCursor(hinst,_RCT("RightClick_cursor"));
      }
  }
  else
  {
    if(hleftclickcursor != 0) delete hleftclickcursor;
    if(hrightclickcursor != 0) delete hrightclickcursor;
    hleftclickcursor = hrightclickcursor = (RCHCURSOR)0;
  }

  busy = 0;
  daysleft = -1;
  nextautosave = 0;
  nextautobackup = 0;
  helpfilter = &fp1;
  nextfilter = &fp2;

  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

}

void Program::updatecursortype(int reversed)
{
#ifdef USING_WIDGETS
    if( ! reversed)
    {
        if(herasecursor != 0) delete herasecursor;
        herasecursor = LoadCursor(hinst,_RCT("Erase_cursor"));
    }
    else
    {
        if(herasecursor != 0) delete herasecursor;
        herasecursor = LoadCursor(hinst,_RCT("REVErase_cursor"));
    }
    if (1 || v.getinteger("db::usewindowscursors") > 0)
    {
        if( ! reversed)
        {
             if(hcrosshaircursor != 0) delete hcrosshaircursor;
             hcrosshaircursor =  LoadCursor(hinst,_RCT("CrossHair_cursor"));
             if(hcrosshairsnapcursor != 0) delete hcrosshairsnapcursor;
             hcrosshairsnapcursor = LoadCursor(hinst,_RCT("CrossHairSnap_cursor"));
             if(hcrosshairentitycursor != 0) delete hcrosshairentitycursor;
             hcrosshairentitycursor = LoadCursor(hinst,_RCT("CrossHairEntity_cursor"));
             if(hcrosshairorigincursor != 0) delete hcrosshairorigincursor;
             hcrosshairorigincursor = LoadCursor(hinst,_RCT("CrossHairOrigin_cursor"));
             if(hcrosshairintcursor != 0) delete hcrosshairintcursor;
             hcrosshairintcursor = LoadCursor(hinst,_RCT("CrossHairInt_cursor"));
             if(hpositioncrosshaircursor != 0) delete hpositioncrosshaircursor;
             hpositioncrosshaircursor =  LoadCursor(hinst,_RCT("PositionCrossHair_cursor"));
             if(hpositioncrosshairsnapcursor != 0) delete hpositioncrosshairsnapcursor;
             hpositioncrosshairsnapcursor = LoadCursor(hinst,_RCT("PositionCrossHairSnap_cursor"));
             if(hpositioncrosshairentitycursor != 0) delete hpositioncrosshairentitycursor;
             hpositioncrosshairentitycursor = LoadCursor(hinst,_RCT("PositionCrossHairEntity_cursor"));
             if(hpositioncrosshairorigincursor != 0) delete hpositioncrosshairorigincursor;
             hpositioncrosshairorigincursor = LoadCursor(hinst,_RCT("PositionCrossHairOrigin_cursor"));
             if(hpositioncrosshairintcursor != 0) delete hpositioncrosshairintcursor;
             hpositioncrosshairintcursor = LoadCursor(hinst,_RCT("PositionCrossHairInt_cursor"));
        }
         else
         {
            if(hcrosshaircursor != 0) delete hcrosshaircursor;
            hcrosshaircursor =  LoadCursor(hinst,_RCT("REVCrossHair_cursor"));
            if(hcrosshairsnapcursor != 0) delete hcrosshairsnapcursor;
            hcrosshairsnapcursor = LoadCursor(hinst,_RCT("REVCrossHairSnap_cursor"));
            if(hcrosshairentitycursor != 0) delete hcrosshairentitycursor;
            hcrosshairentitycursor = LoadCursor(hinst,_RCT("REVCrossHairEntity_cursor"));
            if(hcrosshairorigincursor != 0) delete hcrosshairorigincursor;
            hcrosshairorigincursor = LoadCursor(hinst,_RCT("REVCrossHairOrigin_cursor"));
            if(hcrosshairintcursor != 0) delete hcrosshairintcursor;
            hcrosshairintcursor = LoadCursor(hinst,_RCT("REVCrossHairInt_cursor"));
            if(hpositioncrosshaircursor != 0) delete hpositioncrosshaircursor;
            hpositioncrosshaircursor =  LoadCursor(hinst,_RCT("REVPositionCrossHair_cursor"));
            if(hpositioncrosshairsnapcursor != 0) delete hpositioncrosshairsnapcursor;
            hpositioncrosshairsnapcursor = LoadCursor(hinst,_RCT("REVPositionCrossHairSnap_cursor"));
            if(hpositioncrosshairentitycursor != 0) delete hpositioncrosshairentitycursor;
            hpositioncrosshairentitycursor = LoadCursor(hinst,_RCT("REVPositionCrossHairEntity_cursor"));
            if(hpositioncrosshairorigincursor != 0) delete hpositioncrosshairorigincursor;
            hpositioncrosshairorigincursor = LoadCursor(hinst,_RCT("REVPositionCrossHairOrigin_cursor"));
            if(hpositioncrosshairintcursor != 0) delete hpositioncrosshairintcursor;
            hpositioncrosshairintcursor = LoadCursor(hinst,_RCT("REVPositionCrossHairInt_cursor"));
         }
    }
    else
    {
        if(hcrosshaircursor != 0) delete hcrosshaircursor;
        if(hcrosshairsnapcursor != 0) delete hcrosshairsnapcursor;
        if(hcrosshairentitycursor != 0) delete hcrosshairentitycursor;
        if(hcrosshairorigincursor != 0) delete hcrosshairorigincursor;
        if(hcrosshairintcursor != 0) delete hcrosshairintcursor;
        if(hpositioncrosshaircursor != 0) delete hpositioncrosshaircursor;
        if(hpositioncrosshairsnapcursor != 0) delete hpositioncrosshairsnapcursor;
        if(hpositioncrosshairentitycursor != 0) delete hpositioncrosshairentitycursor;
        if(hpositioncrosshairorigincursor != 0) delete hpositioncrosshairorigincursor;
        if(hpositioncrosshairintcursor != 0) delete hpositioncrosshairintcursor;
      hcrosshaircursor = hcrosshairsnapcursor = hcrosshairentitycursor = hpositioncrosshaircursor = hpositioncrosshairsnapcursor = hpositioncrosshairentitycursor = hpositioncrosshairorigincursor = hpositioncrosshairintcursor = hemptycursor;
    }
#else
  herasecursor =  LoadCursor(hinst,"Erase_cursor");
  if (v.getinteger("db::usewindowscursors") > 0)
  {
       hcrosshaircursor =  LoadCursor(hinst,"CrossHair_cursor");
       hcrosshairsnapcursor = LoadCursor(hinst,"CrossHairSnap_cursor");
       hcrosshairentitycursor = LoadCursor(hinst,"CrossHairEntity_cursor");
       hcrosshairorigincursor = LoadCursor(hinst,"CrossHairOrigin_cursor");
       hcrosshairintcursor = LoadCursor(hinst,"CrossHairInt_cursor");
       hpositioncrosshaircursor =  LoadCursor(hinst,"PositionCrossHair_cursor");
       hpositioncrosshairsnapcursor = LoadCursor(hinst,"PositionCrossHairSnap_cursor");
       hpositioncrosshairentitycursor = LoadCursor(hinst,"PositionCrossHairEntity_cursor");
       hpositioncrosshairorigincursor = LoadCursor(hinst,"PositionCrossHairOrigin_cursor");
       hpositioncrosshairintcursor = LoadCursor(hinst,"PositionCrossHairInt_cursor");
  }
  else
    hcrosshaircursor = hcrosshairsnapcursor = hcrosshairentitycursor = hpositioncrosshaircursor = hpositioncrosshairsnapcursor = hpositioncrosshairentitycursor = hpositioncrosshairorigincursor = hpositioncrosshairintcursor = hemptycursor;
#endif
}

void Program::ShowLeftClick(void)
{RCHCURSOR hcursor;
  if (hleftclickcursor != 0)
    {  hcursor = SetCursor(hleftclickcursor);
       Sleep(100);
       PlaySound(home.getexecutablefilename(_RCT("NIMClick.WAV")),0,SND_ASYNC | SND_FILENAME);
       SetCursor(hcursor);
    }
}

void Program::ShowRightClick(void)
{RCHCURSOR hcursor;
  if (hrightclickcursor != 0)
    {  hcursor = SetCursor(hrightclickcursor);
       Sleep(100);
       PlaySound(home.getexecutablefilename(_RCT("NIMClick.WAV")),0,SND_ASYNC | SND_FILENAME);
       SetCursor(hcursor);
    }
}

void Program::setautosavetimer(void)
{

  if(qautosavetimer == 0)
  {
      qautosavetimer = new QTimer(app);
      //disconnect(qautosavetimer, SIGNAL(timeout()), app, SLOT(checkautosavetimer()));
      //connect(qautosavetimer, SIGNAL(timeout()), app, SLOT(checkautosavetimer()));
      QObject::disconnect(qautosavetimer, SIGNAL(timeout()), app, SLOT(checkautosavetimer()));
      QObject::connect(qautosavetimer, SIGNAL(timeout()), app, SLOT(checkautosavetimer()));
  }
  if(qautosavetimer != 0)
  {
    qautosavetimer->stop();
    qautosavetimer->start((int)(v.getreal("db::autosavetimeout") * 60000.0));
  }
  nextautosave = timeGetTime() + (unsigned long)(v.getreal("db::autosavetimeout") * 60000.0);
  if (nextautobackup != 0xFFFFFFFF)
    nextautobackup = timeGetTime() + (unsigned long)(v.getreal("db::autosavetimeout") * 60000.0 / 4.0);
}

void Program::stopautosavetimer(void)
{
   if(qautosavetimer != 0)
       qautosavetimer->stop();

   nextautosave = 0;
  if (nextautobackup != 0xFFFFFFFF)
    nextautobackup = 0;
}

extern int check_license(char *label,char *label1,char *label2,int application,int showmessages);

void Program::checkautosavetimer(void)
{
    int cstate,changed;
    //  If the database has not changed then reset the autosave timer
    //if (! db.getchanged())
    //{
    //    setautosavetimer();
    //}
    qDebug() << "before autosave";
    qDebug() << "timeGetTime()" <<  timeGetTime();
    qDebug() << "nextautosave" <<  nextautosave;
    qDebug() << "nextautobackup" <<  nextautobackup;

    if (state.getcommandactive() == 0 && nextautosave != 0 && timeGetTime() > nextautosave)
    {
        // don't need to stop a Qt timer it repeats automatically until stopped
        // if really needed this should call set timer again
        //stopautosavetimer();
        qDebug() << "db::autosaveenabled" <<  v.getinteger("db::autosaveenabled");
        if (v.getinteger("db::autosaveenabled") == 2 || v.getinteger("db::autosaveenabled") == 1 && cadwindow->MessageBox("It is time to update your work.  Do you want to save now?","Auto Save Prompt",MB_YESNOCANCEL) == IDYES)
            file_command3(&cstate,0,0);
        setautosavetimer();
    }

    qDebug() << "after autosave";
    qDebug() << "timeGetTime()" <<  timeGetTime();
    qDebug() << "nextautosave" <<  nextautosave;
    qDebug() << "nextautobackup" <<  nextautobackup;

    if (nextautobackup != 0 && nextautobackup != 0xFFFFFFFF && timeGetTime() > nextautobackup)
    {
        nextautobackup = 0xFFFFFFFF;  //  Disable the autosave feature to avoid recursion.
        if (check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,0))
        {
            CadFile outfile;
            RCCHAR filename[300];
            GetTempPath(300,filename);
            strcat(filename,"RealCAD.CAD");
            strcpy(filename,QDir::toNativeSeparators(QString(filename)));
            qDebug() << "temp backup file" << QString(filename);
            if (outfile.open(filename,"wb"))
            {
                changed = db.getchanged();
                db.save(&outfile);
                outfile.close();
                nextautobackup = timeGetTime() + (unsigned long)(v.getreal("db::autosavetimeout") * 60000.0 / 4.0);
                if (changed)
                    db.setchanged();
            }
        }
    }
}

struct HTMLHelpLinks
  {
   int pagenumber;
   const char *filename;
  } HTMLHelpLinks[] =
  {
    {  2108, "html\\cad0upd.htm" },
    {  2100, "html\\cad0upf.htm" },
    {  2101, "html\\cad0uph.htm" },
    {  2102, "html\\cad0upi.htm" },
    {  2103, "html\\cad0upj.htm" },
    {  2104, "html\\cad0upk.htm" },
    {  2105, "html\\cad0upl.htm" },
    {  2106, "html\\cad0upm.htm" },
    {  2200, "html\\cad0xv7.htm" },
    {  2300, "html\\cad110z.htm" },
    {  2301, "html\\cad1111.htm" },
    {  2302, "html\\cad1112.htm" },
    {  2303, "html\\cad1113.htm" },
    {  2304, "html\\cad1114.htm" },
    {  2305, "html\\cad1115.htm" },
    {  2400, "html\\cad146r.htm" },
    {  2401, "html\\cad146t.htm" },
    {  2402, "html\\cad146u.htm" },
    {  2403, "html\\cad146v.htm" },
    {  2404, "html\\cad146w.htm" },
    {  2405, "html\\cad146x.htm" },
    {  2508, "html\\cad17ch.htm" },
    {  2509, "html\\cad17ci.htm" },
    {  2500, "html\\cad17cj.htm" },
    {  2501, "html\\cad17cl.htm" },
    {  2502, "html\\cad17cm.htm" },
    {  2503, "html\\cad17cn.htm" },
    {  2504, "html\\cad17co.htm" },
    {  2505, "html\\cad17cp.htm" },
    {  2506, "html\\cad17cq.htm" },
    {  3002, "html\\cad184w.htm" },
    {  2608, "html\\cad1ai9.htm" },
    {  2609, "html\\cad1aia.htm" },
    {  2600, "html\\cad1aib.htm" },
    {  2601, "html\\cad1aid.htm" },
    {  2602, "html\\cad1aie.htm" },
    {  2603, "html\\cad1aif.htm" },
    {  2604, "html\\cad1aig.htm" },
    {  2605, "html\\cad1aih.htm" },
    {  2606, "html\\cad1aii.htm" },
    {  1950, "html\\cad1ehv.htm" },
    {  1001, "html\\cad1hv4.htm" },
    {  1000, "html\\cad1hwj.htm" },
    {  21011, "html\\cad1kto.htm" },
    {  21012, "html\\cad1ktp.htm" },
    {  21013, "html\\cad1ktq.htm" },
    {  21014, "html\\cad1ktr.htm" },
    {  21015, "html\\cad1kts.htm" },
    {  511, "html\\cad1ktt.htm" },
    {  21017, "html\\cad1ktu.htm" },
    {  21018, "html\\cad1ktv.htm" },
    {  21019, "html\\cad1ktw.htm" },
    {  21010, "html\\cad1ktx.htm" },
    {  21001, "html\\cad1l64.htm" },
    {  21002, "html\\cad1l65.htm" },
    {  21003, "html\\cad1l66.htm" },
    {  21004, "html\\cad1l67.htm" },
    {  21005, "html\\cad1l68.htm" },
    {  21006, "html\\cad1l69.htm" },
    {  21007, "html\\cad1l6a.htm" },
    {  21008, "html\\cad1l6b.htm" },
    {  21009, "html\\cad1l6c.htm" },
    {  21000, "html\\cad1l6d.htm" },
    {  2107, "html\\cad1z39.htm" },
    {  9997, "html\\cad22lx.htm" },
    {  20002, "html\\cad25m4.htm" },
    {  20002, "html\\cad25m5.htm" },
    {  20003, "html\\cad25m6.htm" },
    {  20000, "html\\cad25md.htm" },
    {  428, "html\\cad271v.htm" },
    {  113, "html\\cad273i.htm" },
    {  114, "html\\cad273j.htm" },
    {  115, "html\\cad273k.htm" },
    {  105, "html\\cad2980.htm" },
    {  213, "html\\cad2a9a.htm" },
    {  214, "html\\cad2a9b.htm" },
    {  215, "html\\cad2a9c.htm" },
    {  2507, "html\\cad2bqd.htm" },
    {  205, "html\\cad2cds.htm" },
    {  2607, "html\\cad2ew5.htm" },
    {  305, "html\\cad2fjk.htm" },
    {  413, "html\\cad2gku.htm" },
    {  414, "html\\cad2gkv.htm" },
    {  415, "html\\cad2gkw.htm" },
    {  405, "html\\cad2ipc.htm" },
    {  513, "html\\cad2jqm.htm" },
    {  514, "html\\cad2jqn.htm" },
    {  515, "html\\cad2jqo.htm" },
    {  525, "html\\cad2jsg.htm" },
    {  535, "html\\cad2ju8.htm" },
    {  545, "html\\cad2jw0.htm" },
    {  555, "html\\cad2jxs.htm" },
    {  505, "html\\cad2lv4.htm" },
    {  3003, "html\\cad2qem.htm" },
    {  3001, "html\\cad2qeo.htm" },
    {  3004, "html\\cad2qfz.htm" },
    {  3000, "html\\cad2qg3.htm" },
    {  2005, "html\\cad2rj7.htm" },
    {  2006, "html\\cad2rj8.htm" },
    {  2007, "html\\cad2rj9.htm" },
    {  913, "html\\cad2wdq.htm" },
    {  914, "html\\cad2wdr.htm" },
    {  915, "html\\cad2wds.htm" },
    {  428, "html\\cad2xbn.htm" },
    {  21020, "html\\cad4ldc.htm" },
    {  21021, "html\\cad4ldd.htm" },
    {  21022, "html\\cad4lde.htm" },
    {  21023, "html\\cad4ldf.htm" },
    {  21024, "html\\cad4ldg.htm" },
    {  21025, "html\\cad4ldh.htm" },
    {  21026, "html\\cad4ldi.htm" },
    {  21027, "html\\cad4ldj.htm" },
    {  21028, "html\\cad4ldk.htm" },
    {  21029, "html\\cad4ldl.htm" },
    {  111, "html\\cad55f2.htm" },
    {  112, "html\\cad55gf.htm" },
    {  1108, "html\\cad58fl.htm" },
    {  1100, "html\\cad58fn.htm" },
    {  1101, "html\\cad58fp.htm" },
    {  1102, "html\\cad58fq.htm" },
    {  1103, "html\\cad58fr.htm" },
    {  1104, "html\\cad58fs.htm" },
    {  1105, "html\\cad58ft.htm" },
    {  1106, "html\\cad58fu.htm" },
    {  211, "html\\cad58ku.htm" },
    {  212, "html\\cad58m7.htm" },
    {  1200, "html\\cad5blf.htm" },
    {  1201, "html\\cad5blh.htm" },
    {  1202, "html\\cad5bli.htm" },
    {  1203, "html\\cad5blj.htm" },
    {  1204, "html\\cad5blk.htm" },
    {  1205, "html\\cad5bll.htm" },
    {  1206, "html\\cad5blm.htm" },
    {  1300, "html\\cad5er7.htm" },
    {  1301, "html\\cad5er9.htm" },
    {  1302, "html\\cad5era.htm" },
    {  1303, "html\\cad5erb.htm" },
    {  1304, "html\\cad5erc.htm" },
    {  411, "html\\cad5ewe.htm" },
    {  412, "html\\cad5exr.htm" },
    {  1400, "html\\cad5hwz.htm" },
    {  1401, "html\\cad5hx1.htm" },
    {  1402, "html\\cad5hx2.htm" },
    {  1403, "html\\cad5hx3.htm" },
    {  1404, "html\\cad5hx4.htm" },
    {  1405, "html\\cad5hx5.htm" },
    {  1406, "html\\cad5hx6.htm" },
    {  511, "html\\cad5i26.htm" },
    {  512, "html\\cad5i3j.htm" },
    {  1510, "html\\cad5ik3.htm" },
    {  1508, "html\\cad5l2p.htm" },
    {  1509, "html\\cad5l2q.htm" },
    {  1500, "html\\cad5l2r.htm" },
    {  1501, "html\\cad5l2t.htm" },
    {  1502, "html\\cad5l2u.htm" },
    {  1503, "html\\cad5l2v.htm" },
    {  1504, "html\\cad5l2w.htm" },
    {  1505, "html\\cad5l2x.htm" },
    {  1506, "html\\cad5l2y.htm" },
    {  2002, "html\\cad5lv4.htm" },
    {  1600, "html\\cad5o8j.htm" },
    {  1601, "html\\cad5o8l.htm" },
    {  1602, "html\\cad5o8m.htm" },
    {  1603, "html\\cad5o8n.htm" },
    {  1604, "html\\cad5o8o.htm" },
    {  1710, "html\\cad5ovn.htm" },
    {  1901, "html\\cad5p83.htm" },
    {  1708, "html\\cad5re9.htm" },
    {  1709, "html\\cad5rea.htm" },
    {  1700, "html\\cad5reb.htm" },
    {  1701, "html\\cad5red.htm" },
    {  1702, "html\\cad5ree.htm" },
    {  1703, "html\\cad5ref.htm" },
    {  1704, "html\\cad5reg.htm" },
    {  1705, "html\\cad5reh.htm" },
    {  1706, "html\\cad5rei.htm" },
    {  1810, "html\\cad5s1f.htm" },
    {  1811, "html\\cad5sj9.htm" },
    {  1809, "html\\cad5uk2.htm" },
    {  1800, "html\\cad5uk3.htm" },
    {  1801, "html\\cad5uk5.htm" },
    {  1802, "html\\cad5uk6.htm" },
    {  1803, "html\\cad5uk7.htm" },
    {  1804, "html\\cad5uk8.htm" },
    {  1805, "html\\cad5uk9.htm" },
    {  1806, "html\\cad5uka.htm" },
    {  911, "html\\cad5upa.htm" },
    {  912, "html\\cad5uqn.htm" },
    {  1911, "html\\cad5vp1.htm" },
    {  1951, "html\\cad5xkl.htm" },
    {  1952, "html\\cad5xkm.htm" },
    {  1953, "html\\cad5xkn.htm" },
    {  1908, "html\\cad5xpt.htm" },
    {  1909, "html\\cad5xpu.htm" },
    {  1900, "html\\cad5xpv.htm" },
    {  1901, "html\\cad5xpx.htm" },
    {  1902, "html\\cad5xpy.htm" },
    {  1903, "html\\cad5xpz.htm" },
    {  1904, "html\\cad5xq0.htm" },
    {  1905, "html\\cad5xq1.htm" },
    {  1906, "html\\cad5xq2.htm" },
    {  9998, "html\\cad6cv5.htm" },
    {  9999, "html\\cad6cv6.htm" },
    {  106, "html\\cad6jzl.htm" },
    {  107, "html\\cad6jzm.htm" },
    {  108, "html\\cad6jzn.htm" },
    {  109, "html\\cad6jzo.htm" },
    {  100, "html\\cad6jzp.htm" },
    {  116, "html\\cad6k1d.htm" },
    {  117, "html\\cad6k1e.htm" },
    {  110, "html\\cad6k1h.htm" },
    {  120, "html\\cad6k39.htm" },
    {  101, "html\\cad6khj.htm" },
    {  102, "html\\cad6khk.htm" },
    {  103, "html\\cad6khl.htm" },
    {  104, "html\\cad6khm.htm" },
    {  206, "html\\cad6n5d.htm" },
    {  207, "html\\cad6n5e.htm" },
    {  208, "html\\cad6n5f.htm" },
    {  209, "html\\cad6n5g.htm" },
    {  200, "html\\cad6n5h.htm" },
    {  210, "html\\cad6n79.htm" },
    {  201, "html\\cad6nnb.htm" },
    {  202, "html\\cad6nnc.htm" },
    {  203, "html\\cad6nnd.htm" },
    {  204, "html\\cad6nne.htm" },
    {  306, "html\\cad6qb5.htm" },
    {  300, "html\\cad6qb9.htm" },
    {  301, "html\\cad6qt3.htm" },
    {  302, "html\\cad6qt4.htm" },
    {  303, "html\\cad6qt5.htm" },
    {  304, "html\\cad6qt6.htm" },
    {  406, "html\\cad6tgx.htm" },
    {  407, "html\\cad6tgy.htm" },
    {  408, "html\\cad6tgz.htm" },
    {  409, "html\\cad6th0.htm" },
    {  400, "html\\cad6th1.htm" },
    {  416, "html\\cad6tip.htm" },
    {  417, "html\\cad6tiq.htm" },
    {  418, "html\\cad6tir.htm" },
    {  419, "html\\cad6tis.htm" },
    {  410, "html\\cad6tit.htm" },
    {  428, "html\\cad6tkj.htm" },
    {  423, "html\\cad6tkp.htm" },
    {  401, "html\\cad6tyv.htm" },
    {  402, "html\\cad6tyw.htm" },
    {  403, "html\\cad6tyx.htm" },
    {  404, "html\\cad6tyy.htm" },
    {  506, "html\\cad6wmp.htm" },
    {  507, "html\\cad6wmq.htm" },
    {  508, "html\\cad6wmr.htm" },
    {  509, "html\\cad6wms.htm" },
    {  500, "html\\cad6wmt.htm" },
    {  516, "html\\cad6woh.htm" },
    {  517, "html\\cad6woi.htm" },
    {  518, "html\\cad6woj.htm" },
    {  519, "html\\cad6wok.htm" },
    {  510, "html\\cad6wol.htm" },
    {  526, "html\\cad6wq9.htm" },
    {  527, "html\\cad6wqa.htm" },
    {  528, "html\\cad6wqb.htm" },
    {  529, "html\\cad6wqc.htm" },
    {  520, "html\\cad6wqd.htm" },
    {  521, "html\\cad6wqf.htm" },
    {  522, "html\\cad6wqg.htm" },
    {  523, "html\\cad6wqh.htm" },
    {  524, "html\\cad6wqi.htm" },
    {  536, "html\\cad6ws1.htm" },
    {  537, "html\\cad6ws2.htm" },
    {  538, "html\\cad6ws3.htm" },
    {  539, "html\\cad6ws4.htm" },
    {  530, "html\\cad6ws5.htm" },
    {  531, "html\\cad6ws7.htm" },
    {  532, "html\\cad6ws8.htm" },
    {  533, "html\\cad6ws9.htm" },
    {  534, "html\\cad6wsa.htm" },
    {  546, "html\\cad6wtt.htm" },
    {  547, "html\\cad6wtu.htm" },
    {  548, "html\\cad6wtv.htm" },
    {  549, "html\\cad6wtw.htm" },
    {  540, "html\\cad6wtx.htm" },
    {  541, "html\\cad6wtz.htm" },
    {  542, "html\\cad6wu0.htm" },
    {  543, "html\\cad6wu1.htm" },
    {  544, "html\\cad6wu2.htm" },
    {  556, "html\\cad6wvl.htm" },
    {  550, "html\\cad6wvp.htm" },
    {  551, "html\\cad6wvr.htm" },
    {  552, "html\\cad6wvs.htm" },
    {  553, "html\\cad6wvt.htm" },
    {  554, "html\\cad6wvu.htm" },
    {  501, "html\\cad6x4n.htm" },
    {  502, "html\\cad6x4o.htm" },
    {  503, "html\\cad6x4p.htm" },
    {  504, "html\\cad6x4q.htm" },
    {  600, "html\\cad6zsl.htm" },
    {  601, "html\\cad70af.htm" },
    {  700, "html\\cad72yd.htm" },
    {  750, "html\\cad7379.htm" },
    {  751, "html\\cad737b.htm" },
    {  752, "html\\cad737c.htm" },
    {  797, "html\\cad73ea.htm" },
    {  701, "html\\cad73g7.htm" },
    {  702, "html\\cad73g8.htm" },
    {  703, "html\\cad73g9.htm" },
    {  2003, "html\\cad744u.htm" },
    {  2001, "html\\cad744w.htm" },
    {  2004, "html\\cad7467.htm" },
    {  2000, "html\\cad746b.htm" },
    {  800, "html\\cad7645.htm" },
    {  801, "html\\cad76lz.htm" },
    {  802, "html\\cad76m0.htm" },
    {  803, "html\\cad76m1.htm" },
    {  900, "html\\cad799x.htm" },
    {  916, "html\\cad79bl.htm" },
    {  901, "html\\cad79rr.htm" },
    {  902, "html\\cad79rs.htm" },
    {  903, "html\\cad79rt.htm" },
    {  1107, "html\\cad886t.htm" },
    {  1507, "html\\cad8ktx.htm" },
    {  1, "html\\cad8prl.htm" },
    {  2, "html\\cad8prm.htm" },
    {  3, "html\\cad8prn.htm" },
    {  4, "html\\cad8pro.htm" },
    {  5, "html\\cad8prp.htm" },
    {  6, "html\\cad8prq.htm" },
    {  7, "html\\cad8prr.htm" },
    {  8, "html\\cad8prs.htm" },
    {  9, "html\\cad8prt.htm" },
    {  1707, "html\\cad8r5h.htm" },
    {  1807, "html\\cad8ub9.htm" },
    {  1907, "html\\cad8xh1.htm" },
    {  1002, "html\\cad9zlc.htm" },
    {  0, 0 },
  };


void Program::help(int command,int pagenumber)
{FILE *infile;
 int i;

  if ((infile = fopen((char*)home.getexecutablefilename(_RCT("CAD.CHM")),"r")) != 0)
    {  fclose(infile);
       if (command == HELP_INDEX)
         pagenumber = 1;
       for (i = 0 ; HTMLHelpLinks[i].filename != 0 ; i++)
         if (HTMLHelpLinks[i].pagenumber == pagenumber)
           break;
       HtmlHelp(cadwindow->gethwnd(),(RCPCHAR)home.getexecutablefilename(_RCT("CAD.CHM")), HH_DISPLAY_TOPIC, (DWORD) HTMLHelpLinks[i].filename);
    }
  else
    WinHelp(cadwindow->gethwnd(),(RCLPCTSTR)home.getexecutablefilename(_RCT("CAD.HLP")),command,pagenumber);

}

void graphicsCursorDrawer()
{
    program->drawGraphicsCursor();
}

void Program::drawGraphicsCursor()
{
   if(cadwindow->getcurrentwindow() != 0)
   {
       cadwindow->getcurrentwindow()->drawCursor();
   }
}

#if VERSION == TRICAD_STUDENT
void Program::loadapp(char *path)
{
}
#endif
Home::Home(void)
{
}

Home::Home(bool dummy)
{
  RCCHAR title[300],location[300],programfiles[300],temp[300];
  QString tempName;
  //  Get the executable directory
  GetModuleFileName(0,temp,300);
  strcpy(executabledirectory,temp);

  tempName = QDir().fromNativeSeparators(QString(executabledirectory));
  strcpy(executabledirectory,tempName.data());

  if (strrchr(executabledirectory,'/') != 0)
      strrchr(executabledirectory,'/')[0] = 0;

  qDebug() << "Home() : executabledirectory : " << QString(executabledirectory);

#ifdef _MAC
      strcpy(programfiles,"/Applications");
#else
      if (getenv("PROGRAMFILES") != 0)
        strcpy(programfiles,getenv("PROGRAMFILES"));
      else
          strcpy(programfiles,"C:/Program Files");
#endif

  tempName = QDir().fromNativeSeparators(QString(programfiles));
  strcpy(programfiles,tempName.data());

  qDebug() << "Home() : programfiles : " << QString(programfiles);

#ifdef _MAC
  int endMarker = QString(executabledirectory).indexOf("/Contents/MacOS");
  QString bundlePath = QString(executabledirectory).left(endMarker);
  //QFileInfo fi(app->applicationFilePath());
  QFileInfo fi(bundlePath);
  qDebug() << "isBundle : " << fi.isBundle();
  QString bundle = fi.bundleName();
  strcpy(applicationTitle,bundle.constData());
  if(strlen(applicationTitle) < 1)
  {
      QString appName = app->applicationName();
      //appName += "-v" + QString("%1").arg(MAJOR_VERSION);
      strcpy(applicationTitle,appName.constData());
      //strcpy(applicationTitle,app->applicationName().constData());
  }
  QString appName = QString("-v%1").arg(MAJOR_VERSION);
  strcat(applicationTitle,appName.constData());
  qDebug() << "applicationTitle : " << QString(applicationTitle);
#else
  if (_strnicmp(executabledirectory,programfiles,strlen(programfiles)) == 0)
    strcpy(applicationTitle,executabledirectory + strlen(programfiles) + 1);
  else if (strrchr(executabledirectory,'/') != 0)
    strcpy(applicationTitle,strrchr(executabledirectory,'/') + 1);
  else
    strcpy(applicationTitle,"RealCAD");
#endif

  tempName = QDir().fromNativeSeparators(QString(applicationTitle));
  strcpy(applicationTitle,tempName.data());
  qDebug() << "Home() : title : " << QString(applicationTitle);

#ifdef _MAC
  strcpy(publicdirectory,"/Users/Shared");
  /*
  if (getenv("HOME") != 0)
      strcpy(publicdirectory,getenv("HOME"));
  else if (getenv("USER") != 0)
      strcat(publicdirectory,getenv("USER"));
  else
    strcpy(publicdirectory,executabledirectory);
  */
#else
  if (getenv("PUBLIC") != 0)
    strcpy(publicdirectory,getenv("PUBLIC"));
  else if (getenv("ALLUSERSPROFILE") != 0)
    strcpy(publicdirectory,getenv("ALLUSERSPROFILE"));
  else
    strcpy(publicdirectory,executabledirectory);
#endif

  tempName = QDir().fromNativeSeparators(QString(publicdirectory));
  strcpy(publicdirectory,tempName.data());

  qDebug() << "Home() : publicdirectory : " << QString(publicdirectory);

  if (strlen(publicdirectory) > 0 && publicdirectory[strlen(publicdirectory)-1] != '/')
    strcat(publicdirectory,_RCT("/"));
  strcat(publicdirectory,applicationTitle);

  //QMessageBox::information(0,"home",QString(publicdirectory));
#ifdef _RCDEBUG
  qDebug() << "Home() : publicdirectory : " << QString(publicdirectory);
#endif

#if 0 // not using setup.ini anymore
  strcpy(publicfilename,publicdirectory);
  strcat(publicfilename,_RCT("/Setup.ini"));

  qDebug() << "Home() : publicfilename : " << QString(publicfilename);

  QSettings settings(QString(publicfilename),QSettings::IniFormat);
  QString qslocation = settings.value("Setup/Location","").toString();
  strcpy(location,qslocation.data());
  //GetPrivateProfileString("Setup","Location","",(char*)location,300,publicfilename);

  qDebug() << "Home() : location : " << QString(location);

  if (strlen(location) == 0)
  {//  Old directory structure
     //    All files are located under the executable directory
     //    ini file is in the default directory
       strcpy(publicdirectory,executabledirectory);
       _getcwd(inifilename,300);
       if (strlen(inifilename) > 0 && inifilename[strlen(inifilename)-1] != '/')
         strcat(inifilename,_RCT("/"));
       strcat(inifilename,_RCT("cad.ini"));
       strcpy(licensefilename,_RCT(""));

  qDebug() << "Home() : inifilename : " << QString(inifilename);
  qDebug() << "Home() : licensefilename : " << QString(licensefilename);

  }
  else
#endif
  {
      //  New directory structure
     //    Executable files are located in the executable directory
     //    ini file is in the public directory

       strcpy(inifilename,publicdirectory);
       strcat(inifilename,_RCT("/cad.ini"));
#if 0
#ifdef _MAC
       strcpy(licensefilename,"/Users/Shared");
       /*
       if (getenv("HOME") != 0)
           strcpy(licensefilename,getenv("HOME"));
       else
         strcpy(licensefilename,executabledirectory);
       */
#else
       if (getenv("PUBLIC") != 0)
           strcpy(licensefilename,getenv("PUBLIC"));
       else if (getenv("ALLUSERSPROFILE") != 0)
         strcpy(licensefilename,getenv("ALLUSERSPROFILE"));
       else
         strcpy(licensefilename,executabledirectory);
#endif

#ifndef _MAC
       if (strlen(licensefilename) > 0 && licensefilename[strlen(licensefilename)-1] != '/')
         strcat(licensefilename,_RCT("/"));
       strcat(licensefilename,_RCT("RealCAD-V7"));
       _mkdir(licensefilename);
#endif
#endif
       strcpy(licensefilename,publicdirectory);
       QString licFile = QString("/RealCAD-V%1.INI").arg(MAJOR_VERSION);
       strcat(licensefilename,licFile.constData());
       //strcat(licensefilename,_RCT("/RealCAD-V8.INI"));

       tempName = QDir().fromNativeSeparators(QString(licensefilename));
       strcpy(licensefilename,tempName.data());

  qDebug() << "Home() : inifilename : " << QString(inifilename);
  qDebug() << "Home() : licensefilename : " << QString(licensefilename);

  strcpy(executableDir,app->applicationDirPath().data());
  tempName = QDir().fromNativeSeparators(QString(executableDir));
  strcpy(executableDir,tempName.data());

  strcpy(executablePath,app->applicationFilePath().data());
  tempName = QDir().fromNativeSeparators(QString(executablePath));
  strcpy(executablePath,tempName.data());

  }
}

Home::~Home()
{DebugTraceLine(__FILE__,__LINE__);
 DebugTraceLine(__FILE__,__LINE__);
}

RCCHAR *Home::getinifilename(void)
{
    strcpy(inifilename,QDir::toNativeSeparators(QString(inifilename)));
    return inifilename;
}

RCCHAR *Home::getlicensefilename(void)
{
    strcpy(licensefilename,QDir::toNativeSeparators(QString(licensefilename)));
    return licensefilename;
}


RCCHAR *Home::gettempfilename(RCCHAR *prefix)
{
    // KMJ NOTE: we use Qt separators internally and conver back to native as required
    // using Qt get env for portability
#if 0
  if (getenv("TMP") != 0)
    strcpy(tempfilename,getenv("TMP"));
  else if(getenv("TMPDIR") != 0)
    strcpy(tempfilename,getenv("TMPDIR"));
#else
  QString qtemppath = QDir::fromNativeSeparators(QDir::tempPath());
  strcpy(tempfilename,qtemppath);
#endif

  if (strlen(tempfilename) > 0 && tempfilename[strlen(tempfilename)-1] != '/')
    strcat(tempfilename,"/");
  strcat(tempfilename,"CADTEMP");
  _mkdir(tempfilename);
  strcat(tempfilename,"/");
  strcat(tempfilename,prefix);
  strcat(tempfilename,"XXXXXX");

  strcpy(tempfilename,QDir::toNativeSeparators(QString(tempfilename)));

#ifdef _MAC
  // Mac does not have a wide version of _mktemp??
  // use Qt??
  char ctempfilename[300];
  strcpy(ctempfilename,QString(tempfilename).toLatin1().constData());
  _mktemp(ctempfilename);
  strcpy(tempfilename,ctempfilename);
#else
  _mktemp(tempfilename);
#endif

  return tempfilename;
}


//
//  Return the full filename of a file with title name, located in the executable directory
//
RCCHAR *Home::getexecutablefilename(RCCHAR *name)
{
  strcpy(executablefilename,getApplicationDir());
  strcat(executablefilename,_RCT("/"));
  strcat(executablefilename,name);

  strcpy(executablefilename,QDir::toNativeSeparators(QString(executablefilename)));
  return executablefilename;
}

//
//  Return the full filename of a file with title name, located in the public directory
//
RCCHAR *Home::getpublicfilename(const char *name)
{
    return getpublicfilename(_RCT(name));
}

RCCHAR *Home::getpublicfilename(RCCHAR *name)
{
  int i;
  for(i=0; i<600; i++)
    publicfilename[i]=0;
  strcpy(publicfilename,publicdirectory);
  //strcat(publicfilename,"/");
  strcat(publicfilename,QString(QDir::separator()).toLatin1().data());
  strcat(publicfilename,name);

  strcpy(publicfilename,QDir::toNativeSeparators(QString(publicfilename)));
  return publicfilename;
}

RCCHAR *Home::getApplicationTitle()
{
  return applicationTitle;
}

RCCHAR *Home::getApplicationDir()
{
    strcpy(executableDir,QDir::toNativeSeparators(QString(executableDir)));
    return executableDir;
}

RCCHAR *Home::getApplicationPath()
{
    strcpy(executablePath,QDir::toNativeSeparators(QString(executablePath)));
    return executablePath;
}

RCCHAR *Home::getFigureFilepath(RCCHAR *extra)
{
    QDir path;
    static RCCHAR fullpath[600];
    // new for v8
    strcpy(fullpath,v.getstring("db::figures-components-path"));
    path.setPath(QString(fullpath));
    if(path.exists())
    {
        strcat(fullpath,extra);
        return fullpath;
    }
    // if the value is not defined look again
    strcpy(fullpath,getpublicfilename(""));
    path.setPath(QString(fullpath) + "Components");
    if(path.exists())
        strcat(fullpath,"Components");
    else
    {
        path.setPath(QString(fullpath) + "Figures-Components");
        if(path.exists())
        {
            strcat(fullpath,"Figures-Components");
            strcat(fullpath,"/");
            strcat(fullpath,extra);
            return fullpath;
        }
        path.setPath(QString(fullpath) + "Figures");
        if(path.exists())
        {
            strcat(fullpath,"Figures");
            strcat(fullpath,"/");
            strcat(fullpath,extra);
            return fullpath;
        }
    }
    //strcat(fullpath,"/");
    strcat(fullpath,extra);
    return fullpath;
}

Program::~Program()
{
 //RCHINSTANCE appinstance;
//DebugTraceLine(__FILE__,__LINE__);
  //if (hbrush != 0)
  //  DeleteObject(hbrush);
  //for (apps.start() ; (appinstance = (RCHINSTANCE) apps.next()) != 0 ; )
  //  FreeLibrary(appinstance);
//DebugTraceLine(__FILE__,__LINE__);

    delete herasecursor;
    delete hsizewecursor;
    delete hwaitcursor;
    delete harrowcursor;
    delete hzoomcursor;
    delete hemptycursor;
    delete hleftclickcursor;
    delete hrightclickcursor;
    delete hcrosshaircursor;
    delete hcrosshairsnapcursor;
    delete hcrosshairentitycursor;
    delete hcrosshairorigincursor;
    delete hcrosshairintcursor;
    delete hpositioncrosshaircursor;
    delete hpositioncrosshairsnapcursor;
    delete hpositioncrosshairentitycursor;
    delete hpositioncrosshairorigincursor;
    delete hpositioncrosshairintcursor;

    if(haccel != 0)
        delete haccel;
}

void Program::setbusy(int b)
{ if (b) busy++; else busy--;
  if (busy)
    {
       SetCursor(0);
       hcursor = hwaitcursor;//new RCCursor(IDC_WAIT);
       if (cadwindow != 0)
         cadwindow->cursoroff();
       rubberband.off();
       //app->setOverrideCursor((QCursor&)*hcursor);
    }
  else
    {
       SetCursor(0);
       hcursor = hdefaultcursor;
       if (cadwindow != 0)
         cadwindow->cursoron();
       state.sendevent(NULL);
       //app->restoreOverrideCursor();
    }
  SetCursor(hcursor);
}

int Program::calcCursorFromBackground()
{
    Colour c;
    double hue,lightness,saturation;

    double red   = ::v.getreal("wn::backgroundcolour.red");
    double green = ::v.getreal("wn::backgroundcolour.green");
    double blue  = ::v.getreal("wn::backgroundcolour.blue");

    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    // reversed or white cursor
    // if black is used
    // if primary colours other than green are used
    // if saturation is low
    // if less than a mid grey is used

    // normal or black cursor
    // if white is used
    // if saturated green is used
    // if any two primary colours other than green are used
    // if saturation is high
    // if more than a mid gray is used

    // return 1 for a reversed cursor
    if( (hue <= 0.001 && saturation >= 0.999 && lightness >= 0.5) || // gray
         ((green >  0.7 && red   <= 0.5 && blue  <= 0.5)  || // green
         //(green <= 0.5 && red   >  0.5 && blue  >  0.5)  || // majenta
         (red   <= 0.7 && green >  0.5 && blue  >= 0.5)  || // cyan
         (blue  <= 0.5 && red   >  0.7 && green >= 0.7)) //|| // yellow
         //saturation > 0.4  // any light colour
        )
        return 0; // normal or black cursor
    else
        return 1; // reversed or white cursor
}

void Program::setcursorshape(CursorShape cs)
{
    switch (cs)
    {
     case CursorArrow :
       cursorlist.add(hdefaultcursor);
       hdefaultcursor = harrowcursor;
       break;
     case CursorZoom :
       cursorlist.add(hdefaultcursor);
       hdefaultcursor = hzoomcursor;
       break;
     case CursorPosition :
       cursorlist.add(hdefaultcursor);
       hdefaultcursor = hpositioncrosshaircursor;
       break;
     case CursorRestore :
       cursorlist.start();
       hdefaultcursor = (RCHCURSOR) cursorlist.next();
       if (hdefaultcursor == 0)
         hdefaultcursor = harrowcursor;
       cursorlist.del(hdefaultcursor);
    }
  hcursor = hdefaultcursor;
  SetCursor(0);
  SetCursor(hcursor);
}

int Program::setprogressbardlg(RCHWND progressbar)
{
    m_ProgressBarDlg = progressbar;
    /*
    if(m_ProgressBarDlg != 0)
    {
        int value = ((QProgressDialog*)m_ProgressBarDlg)->minimum();
        ((QProgressDialog*)m_ProgressBarDlg)->setValue(value);
    }
    */
    return 0;
}

int Program::tickprogressup()
{
    if(m_ProgressBarDlg != 0 && v.getinteger("prg::doprogressuptick"))
    {
        int value = ((QProgressDialog*)m_ProgressBarDlg)->value();
        ((QProgressDialog*)m_ProgressBarDlg)->setValue(++value);
        //app->processEvents();
    }
    return 0;
}

void Program::cancelprogressbardlg()
{
    if(m_ProgressBarDlg != 0)
        ((QProgressDialog*)m_ProgressBarDlg)->cancel();
}

int Program::getinterrupt(void)
{
  //app->processEvents(/*QEventLoop::ExcludeUserInputEvents*/);
  //processmessages(0);
  //app->processEvents();
  //qDebug() << "getinterrupt() - state.eventavailable()" << state.eventavailable();
  //return state.eventavailable();
  if(m_ProgressBarDlg != 0)
  {
      //tickprogressup();
      if(((QProgressDialog*)m_ProgressBarDlg)->wasCanceled())
      {
          m_interrupt = false;
          return true;
      }
  }

  if(m_interrupt)
  {
      m_interrupt = false;
      return true;
  }
  else
  {
      return false;
  }
}

void Program::processpaintmessages(void)
{
#ifdef USING_WIDGETS
    // no messages to get in Qt so using this function
    if(app->hasPendingEvents ())
        app->sendPostedEvents( (QObject*)cadwindow->gethwnd(), QEvent::Paint );
    cadwindow->paintall();
#else
  RCMSG msg;
  while (PeekMessage(&msg,NULL,WM_PAINT,WM_PAINT,PM_REMOVE))
    {  TranslateAccelerator(cadwindow->gethwnd(),program->gethaccel(),&msg);
       TranslateMessage(&msg);
       DispatchMessage(&msg);
    }
#endif
}

int Program::processmessages(int wait)
{
#ifdef USING_WIDGETS
    // KMJ : TODO :
    // look at how to use this function when using Qt
    // this is just a temporary method of handling it
    //if( state.getevent() != 0 && state.getcommandid() != 0)
        //state.process();
        //return 1;
    //if( state.getevent() == 0 && state.getcommandid() == 0)
    //if( state.getevent() == 0)
    //    return 0;
    RCMSG msg;
    if (wait)
    {
        if (wait == 1 || wait == 3)
        {
            // if GetMessage() returns 0 it is the WM_QUIT message
            //
             if (! GetMessage(&msg,NULL,NULL,NULL)) return 0;
        }
        else if (wait == 2)
        {
            //if (!app->instance()->eventDispatcher()->processEvents()) return 2;
            if (! PeekMessage(&msg,0,0,0,PM_REMOVE)) return 2; // false == no messages available
            if (msg.message == WM_QUIT) return 0;
        }
    }
    else
    {
        if (! PeekMessage(&msg,NULL,WM_RBUTTONDOWN,WM_RBUTTONDOWN,PM_REMOVE) &&
             ! PeekMessage(&msg,NULL,WM_CHAR,WM_CHAR,PM_REMOVE))
           return 1;
    }
    if (cadwindow != 0)
        TranslateAccelerator(cadwindow->gethwnd(),program->gethaccel(),&msg);
    TranslateMessage(&msg);
    if (cadwindow != 0 && msg.message == WM_KEYUP && msg.wParam == VK_F2)
    {
         msg.message = WM_CHAR;
         msg.wParam = '!';
         msg.hwnd = cadwindow->gethwnd();
    }
  #if VERSION != TRICAD_STUDENT
    else if (cadwindow != 0 && (msg.message == WM_KEYDOWN || msg.message == WM_CHAR || msg.message == WM_KEYUP) && cadwindow->getcadbasicwindow() != 0)
      msg.hwnd = cadwindow->getcadbasicwindow()->gethwnd();
    else if (cadwindow != 0 && (msg.message == WM_KEYDOWN || msg.message == WM_CHAR || msg.message == WM_KEYUP) && cadwindow->getcommandwindow() != 0)
      msg.hwnd = cadwindow->getcommandwindow()->gethwnd();
  #endif
    else if (cadwindow != 0 && cadwindow->viewproperties && msg.message == WM_CHAR && GetFocus() != cadwindow->styleentry->gettypehwnd() && GetFocus() != cadwindow->viewproperties->get_control())
    {
        RCHWND hwnd,parenthwnd;
        RCCHAR classname[300];

          //  If a modeless dialog has the focus - don't pass on the keystrokes to the cadwindow.
          hwnd = GetFocus();
          GetClassName(hwnd,classname,300);
          while (hwnd != 0 && strcmp(classname,"#32770") != 0)
            {  hwnd = GetParent(hwnd);
               GetClassName(hwnd,classname,300);
            }
          parenthwnd = GetParent(hwnd);
          GetClassName(hwnd,classname,300);
          if (strcmp(classname,"#32770") != 0 || parenthwnd != cadwindow->gethwnd())
           if (msg.wParam == 'P' || msg.wParam == 'p' ||
             msg.wParam == 'X' || msg.wParam == 'x' ||
             msg.wParam == 'Y' || msg.wParam == 'y' ||
             msg.wParam == 'Z' || msg.wParam == 'z' ||
             msg.wParam == 'R' || msg.wParam == 'r' ||
             msg.wParam == 'T' || msg.wParam == 't' ||
             msg.wParam == 'D' || msg.wParam == 'd' ||
             msg.wParam == 'A' || msg.wParam == 'a' ||
             msg.wParam == 'B' || msg.wParam == 'b' ||
             msg.wParam == 'L' || msg.wParam == 'l' ||
             msg.wParam == 'I' || msg.wParam == 'i' ||
             msg.wParam == 'M' || msg.wParam == 'm' ||
             msg.wParam == 'F' || msg.wParam == 'f' ||
             msg.wParam == '\t' || msg.wParam == '\r' || msg.wParam == ',' || msg.wParam == '\033')
           msg.hwnd = cadwindow->gethwnd();
    }
    else if (cadwindow != 0 && cadwindow->viewproperties && (GetFocus() == cadwindow->viewproperties->get_control() || GetParent(GetFocus()) == cadwindow->viewproperties->get_control()) && msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN)
    {// Update any property change
          cadwindow->viewproperties->mousemoveevent(-1,-1,0);
          cadwindow->viewproperties->mousemoveevent(-1,-1,msg.wParam == VK_TAB ? 0x8000 : MK_LBUTTON);
          cadwindow->viewproperties->mousemoveevent(-1,-1,0);
    }
    else if (cadwindow != 0 && cadwindow->viewproperties && (GetFocus() == cadwindow->viewproperties->get_control() || GetParent(GetFocus()) == cadwindow->viewproperties->get_control()) && msg.message == WM_KEYDOWN && msg.wParam == VK_TAB)
    {// Update any property change
          cadwindow->viewproperties->mousemoveevent(-1,-1,0x8000);
    }

    DispatchMessage(&msg);
    //app->processEvents(QEventLoop::WaitForMoreEvents);
    //app->processEvents();

    if (wait != 0 && wait != 3)
    {
        //if(state.getcommandactive() && state.getcommandid() >= 31000 &&
        //   program->pluginmessboxup)
        //    return 1;

        state.process();

        if (msg.message == WM_CHAR && GetFocus() != cadwindow->styleentry->gettypehwnd())
        {
             POINT point;
             RCHWND hWnd;
              GetCursorPos(&point);
              if ((hWnd = WindowFromPoint(point)) != NULL)
              {
                   ScreenToClient(hWnd,&point);
                   SendMessage(hWnd,WM_MOUSEMOVE,0,MAKELPARAM(point.x,point.y));
              }
        }
    }

    return 1;
#else
  RCMSG msg;
  if (wait)
  {
      if (wait == 1 || wait == 3)
      {
           if (! GetMessage(&msg,NULL,NULL,NULL)) return 0;
      }
      else if (wait == 2)
      {
          if (!app->instance()->eventDispatcher()->processEvents()) return 2;
          //if (! PeekMessage(&msg,0,0,0,PM_REMOVE)) return 2; // false == no messages available
          //if (msg.message == WM_QUIT) return 0;
      }
  }
  else
  {
      if (! PeekMessage(&msg,NULL,WM_RBUTTONDOWN,WM_RBUTTONDOWN,PM_REMOVE) &&
           ! PeekMessage(&msg,NULL,WM_CHAR,WM_CHAR,PM_REMOVE))
         return 1;
  }
  if (cadwindow != 0)
      TranslateAccelerator(cadwindow->gethwnd(),program->gethaccel(),&msg);
  TranslateMessage(&msg);
  if (cadwindow != 0 && msg.message == WM_KEYUP && msg.wParam == VK_F2)
  {
       msg.message = WM_CHAR;
       msg.wParam = '!';
       msg.hwnd = cadwindow->gethwnd();
  }
#if VERSION != TRICAD_STUDENT
  else if (cadwindow != 0 && (msg.message == WM_KEYDOWN || msg.message == WM_CHAR || msg.message == WM_KEYUP) && cadwindow->getcadbasicwindow() != 0)
    msg.hwnd = cadwindow->getcadbasicwindow()->gethwnd();
  else if (cadwindow != 0 && (msg.message == WM_KEYDOWN || msg.message == WM_CHAR || msg.message == WM_KEYUP) && cadwindow->getcommandwindow() != 0)
    msg.hwnd = cadwindow->getcommandwindow()->gethwnd();
#endif
  else if (cadwindow != 0 && msg.message == WM_CHAR && GetFocus() != cadwindow->styleentry->gettypehwnd() && GetFocus() != cadwindow->viewproperties->get_control())
  {
      RCHWND hwnd,parenthwnd;
      RCCHAR classname[300];

        //  If a modeless dialog has the focus - don't pass on the keystrokes to the cadwindow.
        hwnd = GetFocus();
        GetClassName(hwnd,classname,300);
        while (hwnd != 0 && strcmp(classname,"#32770") != 0)
          {  hwnd = GetParent(hwnd);
             GetClassName(hwnd,classname,300);
          }
        parenthwnd = GetParent(hwnd);
        GetClassName(hwnd,classname,300);
        if (strcmp(classname,"#32770") != 0 || parenthwnd != cadwindow->gethwnd())
         if (msg.wParam == 'P' || msg.wParam == 'p' ||
           msg.wParam == 'X' || msg.wParam == 'x' ||
           msg.wParam == 'Y' || msg.wParam == 'y' ||
           msg.wParam == 'Z' || msg.wParam == 'z' ||
           msg.wParam == 'R' || msg.wParam == 'r' ||
           msg.wParam == 'T' || msg.wParam == 't' ||
           msg.wParam == 'D' || msg.wParam == 'd' ||
           msg.wParam == 'A' || msg.wParam == 'a' ||
           msg.wParam == 'B' || msg.wParam == 'b' ||
           msg.wParam == 'L' || msg.wParam == 'l' ||
           msg.wParam == 'I' || msg.wParam == 'i' ||
           msg.wParam == 'M' || msg.wParam == 'm' ||
           msg.wParam == 'F' || msg.wParam == 'f' ||
           msg.wParam == '\t' || msg.wParam == '\r' || msg.wParam == ',' || msg.wParam == '\033')
         msg.hwnd = cadwindow->gethwnd();
  }
  else if (cadwindow != 0 && (GetFocus() == cadwindow->viewproperties->get_control() || GetParent(GetFocus()) == cadwindow->viewproperties->get_control()) && msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN)
  {// Update any property change
        cadwindow->viewproperties->mousemoveevent(-1,-1,0);
        cadwindow->viewproperties->mousemoveevent(-1,-1,msg.wParam == VK_TAB ? 0x8000 : MK_LBUTTON);
        cadwindow->viewproperties->mousemoveevent(-1,-1,0);
  }
  else if (cadwindow != 0 && (GetFocus() == cadwindow->viewproperties->get_control() || GetParent(GetFocus()) == cadwindow->viewproperties->get_control()) && msg.message == WM_KEYDOWN && msg.wParam == VK_TAB)
  {// Update any property change
        cadwindow->viewproperties->mousemoveevent(-1,-1,0x8000);
  }

  DispatchMessage(&msg);
  if (wait != 0 && wait != 3)
  {
      state.process();
       if (msg.message == WM_CHAR && GetFocus() != cadwindow->styleentry->gettypehwnd())
       {
           POINT point;
           RCHWND hWnd;
#if ! defined(_WIN32_WCE)
            GetCursorPos(&point);
            if ((hWnd = WindowFromPoint(point)) != NULL)
            {
                 ScreenToClient(hWnd,&point);
                 SendMessage(hWnd,WM_MOUSEMOVE,0,MAKELPARAM(point.x,point.y));
            }
//WCEFIX
#endif
       }
  }
  return 1;
#endif
}

ResourceString::ResourceString(int id)
{
  string = new TCHAR[DEFAULT_RESOURCE_STRING_LENGTH];
  RCstring = new RCCHAR[DEFAULT_RESOURCE_STRING_LENGTH];
  if (string == nullptr)
    {
        allocated = 0;
        string =(TCHAR*) L"Internal : Memory allocation error";
        strcpy(RCstring,"Internal : Memory allocation error");
    }
  else
    {
       allocated = 1;
       if (! LoadString(program->gethinst(),id,string,DEFAULT_RESOURCE_STRING_LENGTH))
       {
         strcpy(string,L"Internal : Unknown resource string");
         strcpy(RCstring,"Internal : Unknown resource string");
       }
       else
       {
           //LOCAL2RCHR(RCstring,string);
           strcpy(RCstring,string);
           //qDebug() << "ResourceString" << QString(RCstring);
       }
    }
}

//
//  The ButtonMenu class
//

/*!
 * xp,yp menu screen position
 * n number of buttons
 * ncol number of columns to display
 * sb show button flag
 */
ButtonMenu::ButtonMenu(const int xp,const int yp,const int n,const int ncol,const int sb, QWidget *qparent)
{
#ifdef USING_WIDGETS
    // KMJ : TODO : implement a custom widget
    // this is the menu button box
    //
    // this code is for tool bars
    m_qw = 0;
    /*
    m_qw = new QToolBar(qparent);
    if(m_qw)
    {
        hDC = m_qw;
        m_qw->objectName() = "RCButtonMenu";
        ((QToolBar*)m_qw)->setAllowedAreas(Qt::AllToolBarAreas);
        m_qw->setWindowTitle("Entities");
    }
    */
    // this code is for dock widgets
    // we don't create RCToolBox objects here we just put them in the list
    /*
    m_qw = new RCToolBox(qparent);
    if(m_qw)
    {
        hDC = m_qw;
        m_qw->objectName() = "RCButtonMenu";
        ((RCToolBox*)m_qw)->setAllowedAreas(Qt::AllDockWidgetAreas);
        m_qw->setWindowTitle("Entities");
    }
    */
    if ((buttons = new Button *[n]) == NULL)
    {
       ResourceString rs9(NCWIN+9);
       state.fail(Memory,rs9.gets());
       return;
    }

  hDC = 0;
  xpos = xp;
  ypos = yp;
  nbuttons = n;
  showbutton = sb;
  lastbutton = previousbutton = 0;
  nused = 0;
  over = 0;
  ncolumns = ncol;
  parent = 0;
  orientation = 0;// vertical=1;horizontal=2;
  w = -1;
  h = -1;
#else
    if ((buttons = new Button *[n]) == NULL)
    {
       ResourceString rs9(NCWIN+9);
       state.fail(Memory,rs9.gets());
       return;
    }

  hDC = 0;
  xpos = xp;
  ypos = yp;
  nbuttons = n;
  showbutton = sb;
  lastbutton = previousbutton = 0;
  nused = 0;
  over = 0;
  ncolumns = ncol;
  parent = 0;
  orientation = 0;
  w = -1;
  h = -1;
#endif
}

#ifdef USING_WIDGETS
bool ButtonMenu::setToolbox(RCToolBox *tb, int sId, int nmcols)
{
    int i;

    if(tb == 0)
        return false;

    m_qw = tb;
    tb->setButtonMenu(this);
    // the id currently starts from 1. there are nused buttons per section
    // use calculate the id here using the input sid argument and the loop counter
    // sid is zero based so we add nused to the result for the offset
    tb->initButtons(sId);
    RCToolButton *tbtn=0;
    for (i = 0 ; i < nbuttons ; i++)
    {
        if(i < nused )
            tbtn = tb->addButton(i+1+(sId*nmcols),sId,buttons[i]->name(),buttons[i]->comid(),buttons[i]);
        else
            tbtn = tb->addButton(i+1+(sId*nmcols),sId,"",-1,0);
    }

    if(sId == 0)
        tb->setWindowTitle(QString(buttonmenus.title(this)));
    //if(sId == 0)
    //    tb->setWindowTitle(QString(buttonmenus.match(this)));
    tb->setAllowedAreas(Qt::AllDockWidgetAreas);

    x = xpos = tb->pos().x();
    y = ypos = tb->pos().y();
    w = tb->width();
    h = tb->height();

    //debug();

    return true;
}
void ButtonMenu::debug()
{
    int i;
    Button *button=0;
    RCToolButton *qtb=0;
    for (i = 0 ; i < nused ; i++)
    {
        button = buttons[i];
        qtb = (RCToolButton*)button->getQW();
        //qDebug() << "button [" << i << "]";
        //qDebug() << "button bname : " << button->name();
        //qDebug() << "button comid : " << button->comid();
        //qDebug() << "button m_qw  : " << qtb;
        //qDebug() << "m_qw cname   : " << qtb->mcName;
        //qDebug() << "m_qw comid   : " << qtb->mcomId;
        //qDebug() << "m_qw isOn    : " << qtb->isPressed();
    }
}

#endif

void ButtonMenu::setorientation(int o)
{int maximumcustommenulength;

  maximumcustommenulength = GetPrivateProfileInt("Configuration","MaximumCustomMenuLength",99,home.getinifilename());

  if (o == 1) // vertical
    {  ncolumns = (nused + maximumcustommenulength - 1) / maximumcustommenulength;
       if (ncolumns < 1) ncolumns = 1;
    }
  else if (o == 2) // horizontal
    {  ncolumns = nused > maximumcustommenulength ? maximumcustommenulength : nused > 2 ? nused : 2;
    }
  orientation = o;
}

ButtonMenu::~ButtonMenu()
{int i;
  for (i = 0 ; i < nused ; i++)
    delete buttons[i];
  delete [] buttons;
}

void ButtonMenu::append(Button *button)
{
    if (nused >= nbuttons)
    {
        nbuttons++;
        Button **newbuttons = new Button*[nbuttons];
        for (int i = 0 ; i < nused ; i++)
        {
            newbuttons[i] = buttons[i];
            buttons[i]=0;
        }
        delete [] buttons;
        buttons = newbuttons;
    }
    add(button);
}

void ButtonMenu::add(Button *button)
{
  ResourceString rs10(NCWIN+10);
  int maximumcustommenulength;

  maximumcustommenulength = GetPrivateProfileInt("Configuration","MaximumCustomMenuLength",99,home.getinifilename());

#ifdef USING_WIDGETS
  // if the widget in this menu is not null add the button to it
  // depending on the type of widget used
  if(m_qw && qobject_cast<QDockWidget*>(m_qw) != 0)
      ((RCToolBox*)m_qw)->addButton(((RCToolBox*)m_qw)->nextId(),((RCToolBox*)m_qw)->nextSid(),button->name(),button->comid());
  else if(m_qw && qobject_cast<QToolBar*>(m_qw) != 0)
    ((QToolBar*)m_qw)->addWidget(button->getQW());

#endif

  if (nused == nbuttons)
    {  state.fail(ButtonMenuFull,rs10.gets());
       return;
    }
  if (nused == 0 && (showbutton &16))
    {  lastbutton = button;
       lastbutton->select(0);
    }
  buttons[nused++] = button;

  if (orientation == 1)
    {  ncolumns = (nused + maximumcustommenulength - 1) / maximumcustommenulength;
       if (ncolumns < 1) ncolumns = 1;
    }
  else if (orientation == 2)
    {  ncolumns = nused > maximumcustommenulength ? maximumcustommenulength : nused > 2 ? nused : 2;
    }

  if (nused <= ncolumns)
    {  w = button->w * ncolumns;
       h = button->h;
    }
  else
    {  w = button->w * ncolumns;
       h = button->h * int((nused + ncolumns - 1) / ncolumns);
    }
}

int ButtonMenu::del(int index)
{ if (index >= 0 && index < nused)
    {  del(buttons[index]);
       return 1;
    }
  else
    return 1;

}

void ButtonMenu::del(Button *button)
{int i;
 int maximumcustommenulength;
 int bw,bh;

  bw = button->w;
  bh = button->h;
  maximumcustommenulength = GetPrivateProfileInt("Configuration","MaximumCustomMenuLength",99,home.getinifilename());

  for (i = 0 ; i < nused ; i++)
    if (buttons[i] == button)
      {  delete buttons[i];
         for (; i < nused - 1 ; i++)
           buttons[i] = buttons[i+1];
         nused--;
         break;
      }
  for (i=nused; i < nbuttons ; i++)
    buttons[i] = 0;
  nbuttons--;

  if (orientation == 1)
    {  ncolumns = (nused + maximumcustommenulength - 1) / maximumcustommenulength;
       if (ncolumns < 1) ncolumns = 1;
    }
  else if (orientation == 2)
    {  ncolumns = nused > maximumcustommenulength ? maximumcustommenulength : nused > 1 ? nused : 1;
    }

  if (nused < 2)
    {  if (orientation == 0 || orientation == 1)
         {//  always allow at least 2 rows and 1 column
            w = bw;
            h = bh * 2;
         }
       else
         {  w = bw * 2;
            h = bh;  //  Allow for a single empty
         }
    }
  else if (nused <= ncolumns)
    {  w = buttons[0]->w * ncolumns;
       h = buttons[0]->h;
    }
  else
    {  w = buttons[0]->w * ncolumns;
       h = buttons[0]->h * int((nused + ncolumns - 1) / ncolumns);
    }

  //  Fixup the button positions
  for (i = 0 ; i < nused ; i++)
    {  if (buttons[i]->xpos == -1 && buttons[i]->ypos == -1)
         {  if (i == 0)
              {  buttons[i]->x = x;  buttons[i]->y = y;
              }
            else
              {  if (i % ncolumns == 0)
                   {  buttons[i]->x = x;
                      buttons[i]->y = buttons[i-1]->y + buttons[i]->h;
                   }
                 else
                   {  buttons[i]->x = buttons[i-1]->x + buttons[i]->w;
                      buttons[i]->y = buttons[i-1]->y;
                   }
              }
         }
       else
         {  buttons[i]->x = xpos;  buttons[i]->y = ypos;
         }
    }

}
void ButtonMenu::load(char *name)
{
    load(CHAR2RCCHR(name));
}

void ButtonMenu::load(RCCHAR *name)
{int i,id;
 RCCHAR key[300],bname[300];
 qDebug() << "buttonmenu load name: " << QString(name);
  for (i = 0 ; ; i++)
    {  sprintf(key,"mn::%lsMenu::Id%d",name,i);
       qDebug() << "buttonmenu load key: " << QString(key);
       id = v.getinteger(key);
       if (id <= 0) // this key might not be in the ini file so it could be 0
         break;
       sprintf(key,"mn::%lsMenu::Name%d",name,i);
       qDebug() << "buttonmenu load key: " << QString(key);
       strcpy(bname,v.getstring(key));
       //add(new Button(-1,-1,bname,id));
       append(new Button(-1,-1,bname,id));
    }

}

void ButtonMenu::store(char *name)
{
    store(QString(name).data());
}

void ButtonMenu::store(RCCHAR *name)
{
    qDebug() << "buttonmenu store name: " << QString(name);
    int i;
    RCCHAR key[300];
    for (i = 0 ; i < nused ; i++)
    buttons[i]->save(i,name);
    sprintf(key,"mn::%lsMenu::Id%d",name,i);
    qDebug() << "buttonmenu store key: " << QString(key);
    v.setinteger(key,-1);
    sprintf(key,"mn::%lsMenu::Name%d",name,i);
    qDebug() << "buttonmenu store key: " << QString(key);
    v.setstring(key,_RCT("<EON>"));
}

void ButtonMenu::move(int dx,int dy)
{   move(parent,xpos + dx,ypos + dy,ncolumns);
}


void ButtonMenu::move(Window *window,int xp,int yp,int ncol)
{int maximumcustommenulength;
 int bw,bh;

  maximumcustommenulength = GetPrivateProfileInt("Configuration","MaximumCustomMenuLength",99,home.getinifilename());

  if (nused > 0)
    {  bw = buttons[0]->w;
       bh = buttons[0]->h;
    }
  else
    {  bw = 24;
       bh = 24;
    }

  xpos = xp;
  ypos = yp;
  if (parent != 0)
    parent->delcontrol(this);
  window->addcontrol(this);
  dragging = 0;
  if (ncol >= 0)
    ncolumns = ncol;

  if (orientation == 1)
    {  ncolumns = (nused + maximumcustommenulength - 1) / maximumcustommenulength;
       if (ncolumns < 1) ncolumns = 1;
    }
  else if (orientation == 2)
    {  ncolumns = nused > maximumcustommenulength ? maximumcustommenulength : nused > 2 ? nused : 2;
    }

  if (nused < 2)
    {  if (orientation == 0 || orientation == 1)
         {//  always allow at least 2 rows and 1 column
            w = bw;
            h = bh*2;
         }
       else
         {  w = bw*2;
            h = bh;  //  Allow for a single empty
         }
    }
  else if (nused <= ncolumns)
    {  w = buttons[0]->w * ncolumns;
       h = buttons[0]->h;
       if (orientation == 1 && h < bw*2)
         h = bw*2;
    }
  else
    {  w = buttons[0]->w * ncolumns;
       h = buttons[0]->h * int ((nused + ncolumns - 1) / ncolumns);
    }
  fixup(window);
}

void ButtonMenu::fixup(Window *window)
{int i,maximumcustommenulength,nrows,bw,bh;
 RECT rect;
 WINDOWPLACEMENT wp;

  maximumcustommenulength = GetPrivateProfileInt("Configuration","MaximumCustomMenuLength",99,home.getinifilename());

  if (nused > 0)
    {  bw = buttons[0]->w;
       bh = buttons[0]->h;
    }
  else
    {  bw = 24;
       bh = 24;
    }

  Control::fixup(window);
  parent = window;
  for (i = 0 ; i < nused ; i++)
    {  buttons[i]->parent = window;
       buttons[i]->hDC = window->gethdc();
       if (buttons[i]->xpos == -1 && buttons[i]->ypos == -1)
         {  if (i == 0)
              {  buttons[i]->x = x;  buttons[i]->y = y;
              }
            else
              {  if (i % ncolumns == 0)
                   {  buttons[i]->x = x;
                      buttons[i]->y = buttons[i-1]->y + buttons[i]->h;
                   }
                 else
                   {  buttons[i]->x = buttons[i-1]->x + buttons[i]->w;
                      buttons[i]->y = buttons[i-1]->y;
                   }
              }
         }
       else
         {  buttons[i]->x = xpos;  buttons[i]->y = ypos;
         }
    }

  if (window != cadwindow && (orientation == 1 || orientation == 2))
    {//  Resize the floating window if necessary
       GetClientRect(window->gethwnd(),&rect);

       memset(&wp,0,sizeof(wp));
       wp.length = sizeof(wp);
       GetWindowPlacement(window->gethwnd(),&wp);

       wp.showCmd = SW_SHOWNORMAL;

       if (orientation == 1)
         {  ncolumns = (nused + maximumcustommenulength - 1) / maximumcustommenulength;
            if (ncolumns < 1) ncolumns = 1;
         }
       else if (orientation == 2)
         {  ncolumns = nused > maximumcustommenulength ? maximumcustommenulength : nused > 2 ? nused : 2;
         }

       if (orientation == 2)
         {  if (nused < 2)
              wp.rcNormalPosition.right += 2 * bw - rect.right;
            else
              wp.rcNormalPosition.right += ncolumns * bw - rect.right;

            nrows = (nused + ncolumns - 1) / ncolumns;
            if (nrows == 0) nrows = 1;
            wp.rcNormalPosition.bottom += nrows * bh - rect.bottom;


         }
       else
         {  if (ncolumns == 1 && nused < 2)
              wp.rcNormalPosition.bottom += 2 * bh - rect.bottom;
            else
              wp.rcNormalPosition.bottom += (nused + ncolumns - 1) / ncolumns * bh - rect.bottom;

            wp.rcNormalPosition.right += ncolumns * bw - rect.right;

         }

       SetWindowPlacement(window->gethwnd(),&wp);
    }

}

Button *ButtonMenu::match(RCCHAR *name)
{int i;
  for (i = 0 ; i < nused ; i++)
    if (buttons[i]->match(name))
      return buttons[i];
  return 0;
}

Button *ButtonMenu::match(int commandid)
{int i;

  if (commandid < 0 && -commandid-1 < nused)
      return buttons[-commandid-1];

  for (i = 0 ; i < nused ; i++)
    if (buttons[i]->match(commandid))
      return buttons[i];
  return 0;
}

int ButtonMenu::pick(int xp,int yp)
{
#ifdef USING_WIDGETS
#if 1
    if(m_qw)
    {
        //if(m_qw->rect().contains(m_qw->mapFromGlobal(QPoint(xp,yp))))
        if(m_qw->rect().contains(QPoint(xp,yp)))
        {
            if (over == 0)
            {
                over = 1;
                for (int i = 0 ; i < nused ; i++)
                    buttons[i]->pick(xp,yp);
                  // Press down button
            }
            return 1;
        }
        else
        {
            over = 0;
            return 0;
        }
    }
#endif
#else
  int i;

  if (dragging)
    return 1;
  else if (w >= h && (xp < x - MenuHandleOffset || xp > x + w - 1 || yp < y || yp > y + h - 1) ||
           w < h && (xp < x || xp > x + w - 1 || yp < y - MenuHandleOffset - MenuBorderOffset || yp > y + h - 1))
  {
      if (over == 1)
      {
          over = 0;
          for (i = 0 ; i < nused ; i++)
              buttons[i]->pick(xp,yp);
            // Press down button
      }

      return 0;
  }
  else
  {
      over = 1;
      return 1;
  }
#endif
  return 0;
}

void ButtonMenu::timerevent(void)
{int i;
  for (i = 0 ; i < nused ; i++)
    buttons[i]->timerevent();
}

void ButtonMenu::checkbutton(int c)
{
 int i;
 Button *button;
  if (showbutton&16)
    return;
  button = 0;
  for (i = 0 ; i < nused ; i++)
  {
    if (buttons[i]->match(c))
      button = buttons[i];
    else if (buttons[i] != lastbutton)
      buttons[i]->deselect();
  }
  if (button != lastbutton && lastbutton != 0)
  {
      lastbutton->deselect();
      previousbutton = lastbutton;
  }
  lastbutton = button;
  if (lastbutton != 0)
    lastbutton->select();
}

void ButtonMenu::restorepreviousbutton(void)
{ if (lastbutton != 0)
    lastbutton->deselect();
  lastbutton = previousbutton;
  if (lastbutton != 0)
    lastbutton->select();
}

void ButtonMenu::drawdivide(void)
{RCHBRUSH hbrush1,hbrush2;
 RECT rect;
 int i,bw,bh;
  if (this == 0) return;
  if (hDC == 0 || (cadwindow != 0 && cadwindow->printpreviewwindow != 0)) return;

  if (nused > 0)
    {  bw = buttons[0]->w;
       bh = buttons[0]->h;
    }
  else
    {  bw = 24;
       bh = 24;
    }


  hbrush1 = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  hbrush2 = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
  rect.left = x+1;
  rect.top = y + bh;
  rect.right = x + ncolumns * bw;
  rect.bottom = rect.top + 1;
  FillRect(hDC,&rect,hbrush1);

  for (i = 0 ; i < nused ; i++)
    if (buttons[i] == lastbutton)
      {  rect.left = x + bw*i;
         rect.top = y + bh-1;
         rect.right = x + bw*i + 1;
         rect.bottom = rect.top + 1;
         FillRect(hDC,&rect,hbrush1);

         rect.left = x + bw*i + 1;
         rect.top = y + bh-1;
         rect.right = x + bw*(i+1) - 1;
         rect.bottom = rect.top + 2;
         FillRect(hDC,&rect,hbrush2);

      }

  DeleteObject(hbrush1);
  DeleteObject(hbrush2);

}

void ButtonMenu::mousemoveevent(int xp,int yp,int buts)
{
#ifdef USING_WIDGETS
    ;
#else
 int i,offset,extra,lastover;
 MenuWindow *mw;
 POINT p1;
 RCCHAR text[300],menuname[300];

  for (i = 0 ; i < nused ; i++)
  {
       lastover = buttons[i]->getover();
       if (! dragging && buttons[i]->pick(xp,yp))
         {  if ((showbutton&16) && buts != 0)
              {  if (lastbutton != 0 && buttons[i] != lastbutton)
                   {  lastbutton->deselect();
                      previousbutton = lastbutton;
                   }
                 lastbutton = buttons[i];
                 lastbutton->select(3);
              }
            buttons[i]->mousemoveevent(xp,yp,buts);
         }
  }

  offset = 0;
  if ((showbutton & 4) == 0 && xp >= x - MenuHandleOffset && xp < x && yp > y + h)
    offset = -25;    //  This is a top menu - if we are dragging the bottom part, make sure we drag the top part as well
  if ((showbutton & 1) == 0 && xp >= x - MenuHandleOffset && xp < x && yp < y)
   offset = 25;      //  This is a bottom menu - if we are dragging the bottom part, make sure we drag the top part as well

  extra = 0;
  if ((showbutton & 1) == 0)
    extra = -25;    //  This is a top menu - if we are dragging the bottom part, make sure we drag the top part as well
  if ((showbutton & 4) == 0)
    extra = 25;      //  This is a bottom menu - if we are dragging the bottom part, make sure we drag the top part as well

  switch(drag(xp,yp,offset,extra,buts))
    {case 1 :

       if (buttonmenus.match(this) != 0 && strncmp(buttonmenus.match(this),"ApplicationMenu::",17) == 0)
         strcpy(menuname,buttonmenus.match(this));
       else
         strcpy(menuname,"");

       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }

       if (buttonmenus.match(this) == 0 && strlen(menuname) > 0)
         buttonmenus.add(menuname,this);

       move(cadwindow,xp + dragoffsetx,yp + dragoffsety,ncolumns);
       cadwindow->movemenu(this,xp + dragoffsetx,yp + dragoffsety);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,0,0",xp + dragoffsetx,yp + dragoffsety,ncolumns);
       if (this == cadwindow->GetMainCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainMenu"),text,home.getinifilename());
       else if (this == cadwindow->GetInterruptCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainInterrupt"),text,home.getinifilename());
       else if (this == cadwindow->GetApplicationCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainApplication"),text,home.getinifilename());
       else if (this == cadwindow->GetQuickCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("Quick"),text,home.getinifilename());
       else if (buttonmenus.match(this) != 0 && (strncmp(buttonmenus.match(this),"ApplicationMenu::",17) == 0 || _stricmp(buttonmenus.match(this),"Custom") == 0))
         WritePrivateProfileString(_RCT("Layout"),buttonmenus.match(this),text,home.getinifilename());

       if (buttonmenus.match(_RCT("Custom")) == this)
         v.setinteger("cm::active",1);

       break;
     case 2 :

       if (buttonmenus.match(this) != 0 && strncmp(buttonmenus.match(this),"ApplicationMenu::",17) == 0)
         strcpy(menuname,buttonmenus.match(this));
       else
         strcpy(menuname,"");

       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }

       if (buttonmenus.match(this) == 0 && strlen(menuname) > 0)
         buttonmenus.add(menuname,this);

       p1.x = xp + dragoffsetx;  p1.y = yp + dragoffsety;
       if (buts != -1)
         ClientToScreen(cadwindow->gethwnd(),&p1);
       mw = new MenuWindow(cadwindow->getsubwindow(),p1.x,p1.y,w,h+abs(extra));
       move(mw,0,extra >= 0 ? 0 : 25,ncolumns);
       cadwindow->movemenu(this,0,extra >= 0 ? 0 : 25);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       InvalidateRect(mw->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,%d,1",p1.x,p1.y,w,h+abs(extra));
       if (this == cadwindow->GetMainCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainMenu"),text,home.getinifilename());
       else if (this == cadwindow->GetInterruptCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainInterrupt"),text,home.getinifilename());
       else if (this == cadwindow->GetApplicationCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("MainApplication"),text,home.getinifilename());
       else if (this == cadwindow->GetQuickCADMenu())
         WritePrivateProfileString(_RCT("Layout"),_RCT("Quick"),text,home.getinifilename());
       else if (buttonmenus.match(this) != 0 && (strncmp(buttonmenus.match(this),"ApplicationMenu::",17) == 0 || _stricmp(buttonmenus.match(this),"Custom") == 0))
         WritePrivateProfileString(_RCT("Layout"),buttonmenus.match(this),text,home.getinifilename());

       if (buttonmenus.match(_RCT("Custom")) == this)
         v.setinteger("cm::active",1);

       break;
    }
#endif
}

void ButtonMenu::draw(int drawmode)
{
#ifdef USING_WIDGETS
    // KMJ : this is where special painting could be done
    // we just need to update the button box
    // so nothing much to do here at the moment

    if(m_qw)
        m_qw->update();
    //m_qw->repaint();

#else
 int i,w1,x1,x2,y1,y2;
 RECT rect;
 RCHBRUSH hbrush;
  if (hDC == 0)
    return;
  if (nused == 0 && ncolumns != 1)
    return;

  if (drawmode == DM_ERASE)
    {
       rect.left = x;  rect.top = y;  rect.right = x + w;  rect.bottom = y + h;
       hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
    }
  else
    {
       rect.left = x;  rect.top = y;  rect.right = x + w;  rect.bottom = y + h;
       hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
        for (i = 0 ; i < nused ; i++)
         buttons[i]->draw(drawmode);
    }

  if (nused > 0)
    w1 = buttons[0]->w * ncolumns;
  else
    w1 = 24;

  if ((showbutton & 15) == 15)
    {
       x1 = x2 = 0;
       y1 = 1;  y2 = -2;
       if (w < h)
         {
            x1 = MenuHandleOffset;  x2 = 0;
            y1 = -MenuHandleOffset;  y2 = 0;
         }
    }
  else
    {
       x1 = x2 = 0;
       y1 = -1; y2 = 1;
    }

  //  Draw the boundary around the menu
  rect.left = x - MenuBorderOffset - MenuHandleOffset + x1;  rect.right = x + w1 + MenuBorderOffset + x2;
  rect.top = y - MenuBorderOffset + y1;  rect.bottom = y - MenuBorderOffset + 1 + y1;
  if (showbutton & 1)
    FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
  rect.left = x - MenuBorderOffset - MenuHandleOffset + x1;  rect.right = x - MenuBorderOffset  - MenuHandleOffset + 1 + x1;
  rect.top = y - MenuBorderOffset + y1;  rect.bottom = y + h + MenuBorderOffset + y2;
  if (showbutton & 8)
    FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));

  rect.left = x - MenuBorderOffset - MenuHandleOffset + x1;  rect.right = x + w1 + MenuBorderOffset + x2;
  rect.top = y + h + MenuBorderOffset - 1 + y2;  rect.bottom = y + h + MenuBorderOffset + y2;
  if (showbutton & 4)
    FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
  rect.left = x + w1 + MenuBorderOffset - 1 + x2;  rect.right = x + w1 + MenuBorderOffset + x2;
  rect.top = y - MenuBorderOffset + y1;  rect.bottom = y + h + MenuBorderOffset + y2;
  if (showbutton & 2)
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
#endif
}


//
//  ViewTab class implementation
//    ViewTab implements the tabs used to control the views.
//


ViewTab::ViewTab(Window *p,int fs)
{char text[300];

    parent = p;
    hDC = p->gethdc();
    xpos = 0;
    ypos = 0;
    w = 0;
    h = 0;
    font_size = fs;
    hfont = CreateFont(font_size,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Arial"));
    tiled = 0;
    GetPrivateProfileString("Layout","TabDockTop","1",text,300,home.getinifilename());
    dock_top = atoi(text);
    last_buttons = 0;

}

ViewTab::~ViewTab()
{
}

int ViewTab::get_tab_height(double width)
{// Get the total width required, and so calculate the number of rows required.
 View3dWindow *window;
 View3dWindowList *list;
 double x;
 RCHFONT holdfont;
 int n_rows;
 SIZE size;

    holdfont = (RCHFONT)SelectObject(hDC,hfont);

    x = 10 + xpos;
    x += 5 + font_size + 6;

    list = cadwindow->GetView3dList();
    n_rows = 1;
    for (list->start() ; window = list->next(); )
    {   GetTextExtentPoint(hDC,(RCLPCTSTR)window->getview()->getname(),strlen(window->getview()->getname()),&size);
        if (x + size.cx + font_size + 5 > w)
        {   n_rows++;
            x = 10+xpos;
        }
        x += size.cx + font_size + 6;
    }
    SelectObject(hDC,holdfont);

    return (font_size + 7) * n_rows;

}

int ViewTab::pick(int xp,int yp)
{   return xp >= xpos && xp <= xpos + w - 1 && yp >= ypos && yp <= ypos + h - 1;
}


void ViewTab::select(HWND hwnd)
{
    current_tab_index++;
    if (current_tab_index == cadwindow->GetView3dList()->length())
        current_tab_index = 0;
    update_window_state();

}

void ViewTab::update_window_state(void)
{View3dWindow *window,*max_window;
 View3dWindowList *list;
 int i,j,list_length;
 RCHWND current_hwnd;
 WINDOWPLACEMENT wp;

    if (current_tab_index >= cadwindow->GetView3dList()->length())
        current_tab_index = cadwindow->GetView3dList()->length() - 1;
    if (current_tab_index < 0)
        current_tab_index = 0;

    v.setinteger("df::viewtab",current_tab_index);

    //  Save the current window
    list = cadwindow->GetView3dList();
    for (list->start(),i = 0 ; window = list->next(); i++)
        if (i == current_tab_index)
            current_hwnd = window->gethwnd();

    //  Sort the window list - Move the current maximum window name to the start of the list - by the end the list will be sorted
    list_length = list->length();
    for (i = 0 ; i < list_length ; i++)
    {   max_window = 0;
        for (list->start(),j = 0 ; window = list->next(); j++)
            if (j >= i)
            {   if (max_window == 0)
                    max_window = window;
                else if ((dynamic_cast<View2dWindow *>(max_window)) == 0 && (dynamic_cast<View2dWindow *>(window)) != 0)
                    max_window = window;  //  window is 2d and max_window is 3d
                else if ((dynamic_cast<View2dWindow *>(max_window)) == 0 && (dynamic_cast<View2dWindow *>(window)) == 0 && strcmp(max_window->getview()->getname(),window->getview()->getname()) < 0)
                    max_window = window;  //  both windows 3d and name is greater
                else if ((dynamic_cast<View2dWindow *>(max_window)) != 0 && (dynamic_cast<View2dWindow *>(window)) != 0 && strcmp((dynamic_cast<View2dWindow *>(max_window))->getview2d()->getname(),(dynamic_cast<View2dWindow *>(window))->getview2d()->getname()) < 0)
                    max_window = window;  //  both windows 2d and name is greater
            }
        if (max_window != 0)
        {   list->del(max_window);
            list->add(max_window);
        }
    }

    //  Update the current window
    list = cadwindow->GetView3dList();
    current_tab_index = 0;
    for (list->start(),i = 0 ; window = list->next(); i++)
        if (current_hwnd == window->gethwnd())
            current_tab_index = i;

    if (tiled)
    {
        list = cadwindow->GetView3dList();
        for (list->start() ; window = list->next(); )
            if (GetWindowLong(window->gethwnd(),GWL_USERDATA) != 0)
            {   wp.length = sizeof(WINDOWPLACEMENT);
                GetWindowPlacement(window->gethwnd(),&wp);
                if (wp.showCmd == SW_SHOWMAXIMIZED)
                    ShowWindow(window->gethwnd(),SW_MAXIMIZE);
                else
                    ShowWindow(window->gethwnd(),SW_SHOWNORMAL);
                SetWindowLong(window->gethwnd(),GWL_STYLE,GetWindowLong(window->gethwnd(),GWL_STYLE)|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
                SetWindowPos(window->gethwnd(),0,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                InvalidateRect(window->gethwnd(),0,TRUE);
            }
            else
                ShowWindow(window->gethwnd(),SW_HIDE);

    }
    else if (tiled == 0)
    {   list = cadwindow->GetView3dList();
        i = 0;
        for (list->start() ; window = list->next(); )
        {   ShowWindow(window->gethwnd(),SW_HIDE);
            i++;
        }

        list = cadwindow->GetView3dList();
        i = 0;
        for (list->start() ; window = list->next(); )
        {   if (i == current_tab_index)
            {   ShowWindow(window->gethwnd(),SW_MAXIMIZE);
                SetWindowLong(window->gethwnd(),GWL_STYLE,(GetWindowLong(window->gethwnd(),GWL_STYLE)|WS_VISIBLE)&~WS_MINIMIZEBOX&~WS_MAXIMIZEBOX);
                SetWindowPos(window->gethwnd(),0,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                InvalidateRect(window->gethwnd(),0,TRUE);
                cadwindow->setcurrentwindow(window);
            }
            i++;
        }
    }

    draw(DM_NORMAL);

}


class ATListDialogControl : public ListDialogControl
{private:
 public:
    ATListDialogControl(int id,int n,RCCHAR **list) : ListDialogControl(id,n,list,0,0) {}
    void lbuttondblclkevent(Dialog *,int,int,int);
};

void ATListDialogControl::lbuttondblclkevent(Dialog *dialog,int,int,int)
{   PostMessage(dialog->gethdlg(),WM_COMMAND,102,0);
}

class ATButtonDialogControl : public ButtonDialogControl
{private:
    ViewTab *view_tab;
 public:
    ATButtonDialogControl(int id,ViewTab *vt) : ButtonDialogControl(id)  {   view_tab = vt;   }
    int select(Dialog *);
};

int ATButtonDialogControl::select(Dialog *dialog)
{int i,index,nselecteditems,*selecteditems;
 View *view3d;
 View2d *view2d;
 RCCHAR name[300];
 View3dSurface *v3d;
 View3dWindowList *vl;

    index = 0;
    nselecteditems = SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELCOUNT,0,0);
    selecteditems = new int[nselecteditems];
    SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELITEMS,nselecteditems,(LPARAM)selecteditems);
    for (i = 0 ; i < nselecteditems ; i++)
    {   SendDlgItemMessage(dialog->gethdlg(),100,LB_GETTEXT,selecteditems[i],(LPARAM)name);
        if (GetKeyState(VK_SHIFT) & 0x8000)
        {//  If shift is held down, always add the view
            v3d = 0;
        }
        else
        {//  Check to see if this view already exists
            vl = cadwindow->GetView3dList();
            for (vl->start(),index = 0 ; (v3d = (View3dWindow *) vl->next()) != NULL ; index++)
                if (dynamic_cast<View2dWindow *>(v3d) == 0 && strcmp(name,v3d->getview()->getname()) == 0)
                {   view_tab->set_tab_index(index);
                    break;
                }
        }
        if (v3d == 0 && (view3d = db.views3d.match(name)) != NULL)
        {   view_tab->set_tab_index(0);
            cadwindow->create3dsubwindow(view3d);
        }
    }
    delete [] selecteditems;

    nselecteditems = SendDlgItemMessage(dialog->gethdlg(),101,LB_GETSELCOUNT,0,0);
    selecteditems = new int[nselecteditems];
    SendDlgItemMessage(dialog->gethdlg(),101,LB_GETSELITEMS,nselecteditems,(LPARAM)selecteditems);
    for (i = 0 ; i < nselecteditems ; i++)
    {   SendDlgItemMessage(dialog->gethdlg(),101,LB_GETTEXT,selecteditems[i],(LPARAM)name);
        if (GetKeyState(VK_SHIFT) & 0x8000)
        {//  If shift is held down, always add the view
            v3d = 0;
        }
        else
        {//  Check to see if this view already exists
            vl = cadwindow->GetView3dList();
            for (vl->start(),index = 0 ; (v3d = (View3dWindow *) vl->next()) != NULL ; index++)
                if (dynamic_cast<View2dWindow *>(v3d) != 0 && strcmp(name,(dynamic_cast<View2dWindow *>(v3d))->getview2d()->getname()) == 0)
                {   view_tab->set_tab_index(index);
                    break;
                }
        }
        if (v3d == 0 && (view2d = db.views2d.match(name)) != NULL)
        {   view_tab->set_tab_index(0);
            cadwindow->create2dsubwindow(view2d);
        }
    }
    delete [] selecteditems;

    return 102;
}


void ViewTab::mousemoveevent(int xp,int yp,int buttons)
{RCHFONT holdfont;
 SIZE size;
 int i,x,pick_row,row,last_tiled;
 View3dWindow *window;
 View3dWindowList *list;
 RCMSG msg;

    if ((MK_LBUTTON & buttons) && ! (MK_LBUTTON & last_buttons))
    {   holdfont = (RCHFONT)SelectObject(hDC,hfont);

        if (dock_top)
            pick_row = (ypos + h - yp) / (font_size + 7);
        else
            pick_row = (yp - ypos) / (font_size + 7);

        row = 0;
        x = 10+xpos;
        x += 5 + font_size + 6;
        if (xp < x && pick_row == row)
        {   dock_top = ! dock_top;
            cadwindow->sizeevent(-1,-1);
            WritePrivateProfileString("Layout","TabDockTop",dock_top ? "1" : "0",home.getinifilename());
            return;
        }

        list = cadwindow->GetView3dList();
        i = 0;
        for (list->start() ; window = list->next(); )
        {   GetTextExtentPoint(hDC,(RCLPCTSTR)window->getview()->getname(),strlen(window->getview()->getname()),&size);
            if (x + size.cx + font_size + 6 > w)
            {   x = 10+xpos;
                row++;
            }
            x += size.cx + font_size + 6;
            if (xp < x && pick_row == row)
                break;
            i++;
        }
        SelectObject(hDC,holdfont);

        if (i == list->length() && xp < x + 30 && pick_row == row)
        {int n3d,n2d,index;
         RCCHAR **view2dlist,**view3dlist;

            Dialog dialog("SelectViewForTab_Dialog");

            index = 0;
            db.views3d.names(&view3dlist,&n3d,&index);
            db.views2d.names(&view2dlist,&n2d,&index);
            if (view3dlist != 0 && view2dlist != 0)
            {   dialog.add(new ATListDialogControl(100,n3d,view3dlist));
                dialog.add(new ATListDialogControl(101,n2d,view2dlist));
                dialog.add(new ATButtonDialogControl(102,this));
                dialog.process();
                delete [] view2dlist;
                delete [] view3dlist;
            }

        }
        else if (i < list->length())
        {   current_tab_index = i;
            last_tiled = tiled;
            tiled = (MK_CONTROL & buttons) != 0;

            if (tiled)
            {   //  Clear the existing tiled windows if it is not currently tiled
                if (! last_tiled)
                {   list = cadwindow->GetView3dList();
                    for (list->start() ; window = list->next(); )
                        SetWindowLong(window->gethwnd(),GWL_USERDATA,0);
                }

                //  Add this window to the tiled list
                list = cadwindow->GetView3dList();
                i = 0;
                for (list->start() ; window = list->next(); )
                {   if (i == current_tab_index)
                        SetWindowLong(window->gethwnd(),GWL_USERDATA,tiled != last_tiled ? 1 : ! GetWindowLong(window->gethwnd(),GWL_USERDATA) );
                    i++;
                }
            }

        }

        update_window_state();

        while (PeekMessage(&msg,cadwindow->gethwnd(),WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE) != 0);

    }

    last_buttons = buttons;

}

int ViewTab::drawtab(RCCHAR *label,int x,int full_left,int full_right,int top_closed,int tab_index,int *row)
{RCHFONT holdfont;
 RCHPEN hpen,holdpen;
 SIZE size;
 int base_line,top_line,direction,row_height;
 RECT rect;
 RCHBRUSH hbrush;
 RCCOLORREF c1,c2,c3;

    SetTextColor(hDC,GetSysColor(COLOR_BTNTEXT));

    SetBkMode(hDC,TRANSPARENT);
    holdfont = (RCHFONT)SelectObject(hDC,hfont);

    //if (label[0] & 0x80)
    if (strlen(&label[0]) == 0)
    {   size.cx = 5;
    }
    else
        GetTextExtentPoint(hDC,(RCLPCTSTR)label,strlen(label),&size);

    row_height = font_size + 7;

    if (x + size.cx + font_size + 5 > w)
    {   (*row)++;
        x = 10+xpos;
    }

    c1 = GetSysColor(COLOR_3DSHADOW);
    c2 = GetSysColor(COLOR_3DFACE);
    c1 = RGB((GetRValue(c1)+GetRValue(c2))/2,(GetGValue(c1)+GetGValue(c2))/2,(GetBValue(c1)+GetBValue(c2))/2);
    c3 = RGB((GetRValue(c1)*2+GetRValue(c2))/3,(GetGValue(c1)*2+GetGValue(c2))/3,(GetBValue(c1)*2+GetBValue(c2))/3);

    if (dock_top)
    {   base_line = ypos + h - 1 - row_height * *row;
        top_line =  base_line - row_height + 4;
        direction = -1;
    }
    else
    {   base_line = ypos + row_height * *row;
        top_line =  ypos + row_height - 4 + row_height * *row;
        direction = 1;
    }

    if (top_closed == 0)
        top_line += direction * 2;


    rect.left = x;
    rect.right = x + size.cx + font_size + 5;

    if (dock_top)
    {   rect.top = top_line + 2;
        rect.bottom =  base_line - 1;
    }
    else
    {   rect.top = base_line + 2;
        rect.bottom = top_line - 1;
    }

    hbrush = CreateSolidBrush(top_closed != 0 ? GetSysColor(COLOR_3DFACE) : c1);
    FillRect(hDC,&rect,hbrush);
    DeleteObject(hbrush);

    if (! top_closed)
    {   rect.top = rect.top + (rect.bottom - rect.top) / 2;
        hbrush = CreateSolidBrush(c3);
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);
    }

    //if (label[0] & 0x80)
    if (strlen(&label[0]) == 0)
    {   hpen  = CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNTEXT));
        holdpen = (RCHPEN) SelectObject(hDC,hpen);

        MoveToEx(hDC,x+14,base_line + direction*6,0);
        LineTo(hDC,x+14,top_line-direction*4);

        MoveToEx(hDC,x+12,base_line + direction*10,0);
        LineTo(hDC,x+14,base_line + direction*6);
        MoveToEx(hDC,x+16,base_line + direction*10,0);
        LineTo(hDC,x+14,base_line + direction*5);

        SelectObject(hDC,holdpen);
        DeleteObject(hpen);

    }
    else
    {   if (dock_top)
            TextOut(hDC,x+10,ypos+1 + 2 + h - row_height * (*row+1),(RCLPCTSTR)label,strlen(label));
        else
            TextOut(hDC,x+10,ypos+1 + 2 + row_height * *row,(RCLPCTSTR)label,strlen(label));
    }

    SelectObject(hDC,hfont);

    hpen  = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
    holdpen = (RCHPEN) SelectObject(hDC,hpen);

    MoveToEx(hDC,x,base_line + direction*2,0);

    LineTo(hDC,x,top_line-direction*2);
    LineTo(hDC,x+2,top_line);
    LineTo(hDC,x+size.cx+font_size+4,top_line);
    LineTo(hDC,x+size.cx+font_size+6,top_line-direction*2);
    LineTo(hDC,x+size.cx+font_size+6,base_line + direction*1);
    SelectObject(hDC,holdpen);
    DeleteObject(hpen);

    hpen  = CreatePen(PS_SOLID,1,GetSysColor(COLOR_3DHIGHLIGHT));
    holdpen = (RCHPEN) SelectObject(hDC,hpen);

    MoveToEx(hDC,x+1,base_line + direction*2*top_closed,0);

    LineTo(hDC,x+1,top_line-direction*2);
    LineTo(hDC,x+2,top_line-direction*1);
    LineTo(hDC,x+size.cx+font_size+4,top_line-direction*1);
    LineTo(hDC,x+size.cx+font_size+5,top_line-direction*2);
    LineTo(hDC,x+size.cx+font_size+5,base_line + direction*(1-2*(!top_closed)));
    SelectObject(hDC,holdpen);
    DeleteObject(hpen);

    if (top_closed == 0)
    {   hpen  = CreatePen(PS_SOLID,1, c1);
        holdpen = (RCHPEN) SelectObject(hDC,hpen);
        MoveToEx(hDC,x+2,base_line,0);
        LineTo(hDC,x + size.cx + font_size + 5,base_line);
        MoveToEx(hDC,x+2,base_line + direction,0);
        LineTo(hDC,x + size.cx + font_size + 5,base_line + direction);
        SelectObject(hDC,holdpen);
        DeleteObject(hpen);
    }

    return x + size.cx + font_size + 6;
}


void ViewTab::draw(int drawmode)
{RECT rect;
 RCHBRUSH hbrush;
 int x,i,n_rows,row_height,row;
 View3dWindowList *list;
 View3dWindow *window,*nextwindow;

    if (hDC == 0)
        return;

    if (drawmode == DM_ERASE)
    {   rect.left = xpos;  rect.top = ypos;  rect.right = xpos + w;  rect.bottom = ypos + h;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);
    }
    else
    {

        if (get_tab_height(w) != h)
        {   cadwindow->sizeevent(-1,-1);
            return;
        }

        rect.left = xpos;  rect.top = ypos;  rect.right = xpos + w;  rect.bottom = ypos + h;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);

        row_height = font_size + 7;
        n_rows = h / row_height;

        for (i = 0 ; i < n_rows ; i++)
        {   rect.left = xpos;  rect.right = xpos + w;
            if (dock_top)
            {   rect.top = ypos+h-2-row_height*i;
                rect.bottom = ypos + h - row_height*i;
            }
            else
            {   rect.top = ypos+row_height*i;
                rect.bottom = ypos + 2+row_height*i;
            }

            hbrush = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
            FillRect(hDC,&rect,hbrush);
            DeleteObject(hbrush);
        }

        if (dock_top)
            rect.top++;
        else
            rect.bottom--;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);

        row = 0;
        list = cadwindow->GetView3dList();
        x = 10 + xpos;
        if (dock_top)
            x = drawtab(_RCT("\xFE"),x,0,0,1,1,&row);
        else
            x = drawtab(_RCT("\xFF"),x,0,0,1,1,&row);

        i = 0;
        list->start();
        window = list->next();
        while (window != 0)
        {   nextwindow = list->next();
            x = drawtab(window->getview()->getname(),x,i != current_tab_index + 1,i != current_tab_index - 1,! tiled && current_tab_index != i || tiled && !GetWindowLong(window->gethwnd(),GWL_USERDATA),1,&row);
            window = nextwindow;
            i++;
        }

        x = drawtab(_RCT("+"),x,0,0,1,1,&row);

    }

}

void ViewTab::size(int x,int y,int width,int height)
{   xpos = x;
    ypos = y;
    w = width;
    h = height;
}

void ViewTab::fixup(Window *)
{
}

void ViewTab::checkbutton(int c)
{
}

void ViewTab::timerevent(void)
{
}

//
//  ViewProperties
//    This class is used to display properties
//

ViewProperties::ViewProperties(Window *p,int fs)
#ifdef DERIVE_FROM_WIDGETS
    : Control(_qparent)
#endif
{
    char text[300];
    parent = p;
    hDC = p->gethdc();
    x = -1;
    xpos = 0;
    y = -1;
    ypos = 0;
    w = 0;
    h = 0;
    font_size = fs;
#ifdef _USING_QTCHAR
    hfont = CreateFontW(font_size,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Arial"));
#else
    hfont = CreateFont(font_size,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,"Arial");
#endif

    GetPrivateProfileString("Layout","PropertyDockLeft","1",text,300,home.getinifilename());
    dock_left = atoi(text);
    num_properties = 0;
    GetPrivateProfileString("Layout","CurrentPropertyWidth","250",text,300,home.getinifilename());
    property_width =  atoi(text);
    GetPrivateProfileString("Layout","PropertyLabelWidth","120",text,300,home.getinifilename());
    label_width =  atoi(text);

    //  Variables used when resizing the
    dragging = 0;
    last_drag_x = 0;
    last_drag_y = 0;
    drag_drawn = 0;

    control = 0;
    control_property_index = -1;

    last_entity = 0;
    entity_changed = 1;

}

ViewProperties::~ViewProperties()
{
}

void ViewProperties::set_last_entity(Entity *e)
{   entity_changed = e != last_entity;
    last_entity = e;
}

int ViewProperties::pick(int xp,int yp)
{   return dragging || xp >= xpos && xp <= xpos + w - 1 && yp >= ypos && yp <= ypos + h - 1;
}

int ViewProperties::get_current_group_states(void)
{int i,j,current_group_states;
    current_group_states = 0;
    for (i = 0,j = 0 ; i < num_properties ; i++)
        if (properties[i].type == VP_GROUP)
        {   current_group_states |= (1 << j) * (properties[i].state != 0);
            j++;
        }
    return current_group_states;
}

void ViewProperties::mousemoveevent(int xp,int yp,int buttons)
{int i,j,redraw,style,current_group_states;
 RCCHAR *s,text[50000];  //  This needs to be big to hold the font list
 RCMSG msg;

    if ((buttons & 0x8000) != 0 && control != 0)
    {   //  TAB has been pressed

        //  Delete the old control first
#ifdef _USING_QTCHAR
        //GetWindowTextW(control,(LPWSTR)text,300);

        RCCHAR wname[300];
        GetClassName(control,wname,300);

        if( strcmp(wname,"Edit") == 0 )
            GetWindowTextW(control,text,300);
        else
        {
            int sel = SendMessageW(control,CB_GETCURSEL,0,0);
            SendMessageW(control,CB_GETLBTEXT,sel,(LPARAM)text);
        }

#else
        GetWindowText(control,text,300);
#endif
        if (control_property_index >= 0)
        {   if (properties[control_property_index].process_property(PP_SETVALUE,properties[control_property_index].id,properties[control_property_index].property_data,text))
            {   current_group_states = 0;
                for (i = 0,j = 0 ; i < num_properties ; i++)
                    if (properties[i].type == VP_GROUP)
                    {   current_group_states |= (1 << j) * (properties[i].state != 0);
                        j++;
                    }
                cadwindow->update_properties(current_group_states);
            }
        }
        DestroyWindow(control);
        control = 0;
        SetFocus(cadwindow->gethwnd());

        //  Add the new control
        while (control == 0)
        {   control_property_index++;
            if (control_property_index == num_properties)
                break;

            if (properties[control_property_index].type == VP_PROPERTY && properties[control_property_index].state == 0)
            {   style = properties[control_property_index].process_property(PP_CLICK,properties[control_property_index].id,properties[control_property_index].property_data,text);
                if (style == 1)
                {   control = CreateWindowEx(0,_RCT("EDIT"),0,WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,properties[control_property_index].value_rect.left+2,properties[control_property_index].value_rect.top+2,properties[control_property_index].value_rect.right-properties[control_property_index].value_rect.left-3,properties[control_property_index].value_rect.bottom-properties[control_property_index].value_rect.top-3,parent->gethwnd(),0,0,0);
                    SendMessage(control,(WPARAM)WM_SETFONT,(LPARAM)hfont,0);
                    SetFocus(control);
                    properties[control_property_index].process_property(PP_GETVALUE,properties[control_property_index].id,properties[control_property_index].property_data,text);
#ifdef _USING_QTCHAR
                    SetWindowTextW(control,text);
#else
                    SetWindowText(control,text);
#endif
                    SendMessage(control,EM_SETSEL,0,-1);
                    redraw = 1;
                    break;
                }
                else if (style == 2 || style == 3)
                {   if (style == 2)
                        control = CreateWindowEx(0,_RCT("ComboBox"),0,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,properties[control_property_index].value_rect.left,properties[control_property_index].value_rect.top,properties[control_property_index].value_rect.right-properties[control_property_index].value_rect.left-1,properties[control_property_index].value_rect.bottom-properties[control_property_index].value_rect.top-4,parent->gethwnd(),0,0,0);
                    else
                        control = CreateWindowEx(0,_RCT("ComboBox"),0,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_SORT | WS_VSCROLL,properties[control_property_index].value_rect.left,properties[control_property_index].value_rect.top,properties[control_property_index].value_rect.right-properties[control_property_index].value_rect.left-1,properties[control_property_index].value_rect.bottom-properties[control_property_index].value_rect.top-4,parent->gethwnd(),0,0,0);
                    SendMessage(control,WM_SETFONT,(LPARAM)hfont,0);
                    SendMessage(control,CB_SETDROPPEDWIDTH,200,0);

                    SetFocus(control);
                    //  Add the list values
                    s = strtok(text,_RCT("\t"));
                    while (s)
                    {   SendMessageW(control,CB_ADDSTRING,0,(LPARAM)((LPCWSTR)s));
                        s = strtok((RCCHAR*)0,_RCT("\t"));
                    }

                    properties[control_property_index].process_property(PP_GETVALUE,properties[control_property_index].id,properties[control_property_index].property_data,text);
#ifdef _USING_QTCHAR
                    SetWindowTextW(control, text);
#else
                    SetWindowText(control,text);

#endif
                    SendMessage(control,EM_SETSEL,0,-1);
                    redraw = 1;
                    break;
                }
            }
        }

        draw(DM_NORMAL);

    }

    if ((MK_LBUTTON & buttons) && ! (MK_LBUTTON & last_buttons))
    {   redraw = 0;
        if (control != 0)
        {
#ifdef _USING_QTCHAR
            //GetWindowTextW(control,(LPWSTR)text,300);

            RCCHAR wname[300];
            GetClassName(control,wname,300);

            if( strcmp(wname,"Edit") == 0 )
                GetWindowTextW(control, text,300);
            else
            {
                int sel = SendMessageW(control,CB_GETCURSEL,0,0);
                SendMessageW(control,CB_GETLBTEXT,sel,(LPARAM)text);
            }
#else
            GetWindowText(control,text,300);
#endif
            if (control_property_index >= 0)
            {   if (properties[control_property_index].process_property(PP_SETVALUE,properties[control_property_index].id,properties[control_property_index].property_data,text))
                {   current_group_states = 0;
                    for (i = 0,j = 0 ; i < num_properties ; i++)
                        if (properties[i].type == VP_GROUP)
                        {   current_group_states |= (1 << j) * (properties[i].state != 0);
                            j++;
                        }
                    cadwindow->update_properties(current_group_states);
                }
            }
            DestroyWindow(control);
            control = 0;
            control_property_index = -1;
            redraw = 1;
            SetFocus(cadwindow->gethwnd());
        }
        if (yp < ypos + font_size + 7 && xp >= xpos + w - 32 && xp <= xpos + w - 15)
        {   dock_left = ! dock_left;
            if (dock_left)
                WritePrivateProfileString("Layout","PropertyDockLeft","1",home.getinifilename());
            else
                WritePrivateProfileString("Layout","PropertyDockLeft","0",home.getinifilename());
            cadwindow->sizeevent(-1,-1);
            for (;;)
            {   if (PeekMessage(&msg,cadwindow->gethwnd(),WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE) != 0 && msg.message == WM_LBUTTONUP)
                    break;
            }
            return;
        }
        else if (yp < ypos + font_size + 7 && xp > xpos + w - 15)
        {   property_width = 0;
            cadwindow->sizeevent(-1,-1);
            WritePrivateProfileString("Layout","CurrentPropertyWidth","0",home.getinifilename());
            for (;;)
            {   if (PeekMessage(&msg,cadwindow->gethwnd(),WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE) != 0 && msg.message == WM_LBUTTONUP)
                    break;
            }
            return;
        }
        else if (dock_left && xp > xpos + w - 4 || ! dock_left && xp < xpos + 4)
        {   drag_x = xp;
            drag_y = yp;
            //RCCursor *cursor = new RCCursor(IDC_SIZEWE);
            SetCursor(program->getsizewecursor());
            //delete cursor;
            SetCapture(parent->gethwnd());
            dragging = 1;
        }
        else
        {   for (i = 0 ; i < num_properties ; i++)
            {   if (properties[i].type == VP_PROPERTY)
                {   if (xp >= properties[i].label_rect.right - 2 && xp <= properties[i].label_rect.right + 2 &&
                        yp >= properties[i].label_rect.top && yp <= properties[i].label_rect.bottom)
                    {   drag_x = xp;
                        drag_y = yp;
                        //RCCursor *cursor = new RCCursor(IDC_SIZEWE);
                        SetCursor(program->getsizewecursor());
                        //delete cursor;
                        SetCapture(parent->gethwnd());
                        dragging = 2;
                    }
                }
            }
        }

        for (i = 0 ; i < num_properties ; i++)
        {   if (properties[i].type == VP_GROUP)
            {   if (xp >= properties[i].label_rect.left - 20 && xp <= properties[i].label_rect.right &&
                    yp >= properties[i].label_rect.top && yp <= properties[i].label_rect.bottom)
                {   properties[i].state = ! properties[i].state;
                    redraw = 1;
                }
            }
            else if (properties[i].type == VP_PROPERTY && properties[i].state == 0)
            {   if (xp >= properties[i].value_rect.left && xp <= properties[i].value_rect.right &&
                    yp >= properties[i].value_rect.top && yp <= properties[i].value_rect.bottom)
                {
                    style = properties[i].process_property(PP_CLICK,properties[i].id,properties[i].property_data,text);
                    if (style == 1)
                    {   control = CreateWindowEx(0,_RCT("EDIT"),0,WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,properties[i].value_rect.left+2,properties[i].value_rect.top+2,properties[i].value_rect.right-properties[i].value_rect.left-3,properties[i].value_rect.bottom-properties[i].value_rect.top-3,parent->gethwnd(),0,0,0);
                        SendMessage(control,WM_SETFONT,(LPARAM)hfont,0);
                        SetFocus(control);
                        properties[i].process_property(PP_GETVALUE,properties[i].id,properties[i].property_data,text);
                        SetWindowTextW(control,text);
                        SendMessage(control,EM_SETSEL,0,-1);
                        control_property_index = i;
                        redraw = 1;
                        break;
                    }
                    else if (style == 2 || style == 3)
                    {   if (style == 2)
                            control = CreateWindowEx(0,_RCT("ComboBox"),0,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,properties[i].value_rect.left,properties[i].value_rect.top,properties[i].value_rect.right-properties[i].value_rect.left-1,properties[i].value_rect.bottom-properties[i].value_rect.top-4,parent->gethwnd(),0,0,0);
                        else
                            control = CreateWindowEx(0,_RCT("ComboBox"),0,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_SORT | WS_VSCROLL,properties[i].value_rect.left,properties[i].value_rect.top,properties[i].value_rect.right-properties[i].value_rect.left-1,properties[i].value_rect.bottom-properties[i].value_rect.top-4,parent->gethwnd(),0,0,0);
                        SendMessage(control,WM_SETFONT,(LPARAM)hfont,0);
                        SendMessage(control,CB_SETDROPPEDWIDTH,200,0);

                        SetFocus(control);
                        s = strtok(text,_RCT("\t"));
                        while (s)
                        {   SendMessageW(control,CB_ADDSTRING,0,(LPARAM)((LPCWSTR)s));
                            s = strtok((RCCHAR*)0,_RCT("\t"));
                        }

                        properties[i].process_property(PP_GETVALUE,properties[i].id,properties[i].property_data,text);
                        SetWindowTextW(control, text);

                        SendMessage(control,EM_SETSEL,0,-1);
                        control_property_index = i;
                        redraw = 1;
                        break;
                    }
                    else
                    {   control_property_index = i;
                        redraw = 1;
                    }
                }
            }
        }

        if (redraw)
            draw(DM_NORMAL);

    }
    else if (! (MK_LBUTTON & buttons) && (MK_LBUTTON & last_buttons))
    {   if (dragging != 0)
        {   ReleaseCapture();
            if (dragging == 1)
            {   if (dock_left)
                    property_width = property_width + (xp - drag_x);
                else
                    property_width = property_width - (xp - drag_x);
                if (property_width < label_width + font_size * 6)
                    property_width = label_width + font_size * 6;
                sprintf(text,"%d",property_width);
                WritePrivateProfileString("Layout","PropertyWidth",RCPRINTABLE(text),home.getinifilename());
                WritePrivateProfileString("Layout","CurrentPropertyWidth",RCPRINTABLE(text),home.getinifilename());
            }
            else
            {   label_width = xp - xpos - font_size * 2;
                if (label_width < font_size*4)
                    label_width = font_size * 4;
                if (label_width > w - font_size * 8)
                    label_width = w - font_size * 8;
                sprintf(text,"%d",label_width);
                WritePrivateProfileString("Layout","PropertyLabelWidth",RCPRINTABLE(text),home.getinifilename());
            }

            w = property_width;
            drag_drawn = 0;
            dragging = 0;
            cadwindow->sizeevent(-1,-1);
        }
    }

    if (dock_left && xp > xpos + w - 4 || ! dock_left && xp < xpos + 4)
    {
        //RCCursor *cursor = new RCCursor(IDC_SIZEWE);
        SetCursor(program->getsizewecursor());
        //delete cursor;
    }
    else
    {   for (i = 0 ; i < num_properties ; i++)
        {   if (properties[i].type == VP_PROPERTY)
            {   if (xp >= properties[i].label_rect.right - 2 && xp <= properties[i].label_rect.right + 2 &&
                    yp >= properties[i].label_rect.top && yp <= properties[i].label_rect.bottom)
                {
                    //RCCursor *cursor = new RCCursor(IDC_SIZEWE);
                    SetCursor(program->getsizewecursor());
                    //delete cursor;
                    break;
                }
            }
        }
    }

    if (dragging == 1 || dragging == 2)
    {RCHDC hdc;
     RCHBRUSH holdbrush;
     POINT p1;
     int pw,lw;

        hdc = GetDC(0);
        holdbrush = (RCHBRUSH)SelectObject(hdc,GetStockObject(WHITE_BRUSH));

        if (drag_drawn)
        {   p1.x = last_drag_x - 1;  p1.y = ypos;
            ClientToScreen(cadwindow->gethwnd(),&p1);
            if (p1.x > 0 && p1.x < cadwindow->getwidth())
                PatBlt(hdc,p1.x,p1.y,2,h,PATINVERT);
            drag_drawn = 0;
        }

        //  Clamp the location of the drag position to something sensible
        if (dragging == 1)
        {   if (dock_left)
                pw = property_width + (xp - drag_x);
            else
                pw = property_width - (xp - drag_x);

            if (pw < label_width + font_size * 6)
            {   if (dock_left)
                    xp = xpos + label_width + font_size * 6;
                else
                    xp = xpos + w - label_width - font_size * 6;
            }
        }
        else
        {   lw = xp - xpos - font_size * 2;
            if (lw < font_size*4)
                xp = xpos + font_size * 6;
            if (lw > w - font_size * 8)
                xp = xpos + w - font_size * 6;
        }

        p1.x = xp - 1;  p1.y = ypos;
        ClientToScreen(cadwindow->gethwnd(),&p1);
        if (p1.x > 0 && p1.x < cadwindow->getwidth())
            PatBlt(hdc,p1.x,p1.y,2,h,PATINVERT);

        drag_drawn = 1;
        last_drag_x = xp;
        last_drag_y = yp;

        SelectObject(hdc,holdbrush);
        ReleaseDC(0,hdc);

    }

    last_buttons = buttons;

}

void ViewProperties::draw(int drawmode)
{RECT rect;
 RCHBRUSH hbrush;
 RCHPEN holdpen,hpen;
 int y,y1,dir,i,n_rows,row_height;
 RCHFONT holdfont;
 RCCOLORREF c1,c2;
 RCCHAR text[30000];
 char ctext[30000];

    if (hDC == 0 || property_width == 0)
        return;

    if (drawmode == DM_ERASE)
    {   rect.left = xpos;  rect.top = ypos;  rect.right = xpos + w;  rect.bottom = ypos + h;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);
    }
    else
    {
        rect.left = xpos;  rect.top = ypos;  rect.right = xpos + w;  rect.bottom = ypos + h;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);

        SetTextColor(hDC,GetSysColor(COLOR_BTNTEXT));

        row_height = font_size + 7;
        n_rows = h / row_height;

        rect.left = xpos;  rect.right = xpos + w;
        rect.top = ypos;
        rect.bottom = ypos + font_size + 7;
        hbrush = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);

        hbrush = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
        rect.left = xpos+w-3;  rect.right = xpos + w;
        rect.top = ypos;
        rect.bottom = ypos + h;
        FillRect(hDC,&rect,hbrush);
        rect.left = xpos;  rect.right = xpos + 3;
        rect.top = ypos;
        rect.bottom = ypos + h;
        FillRect(hDC,&rect,hbrush);
        DeleteObject(hbrush);

        holdfont = (RCHFONT)SelectObject(hDC,hfont);
        IntersectClipRect(hDC,xpos+1,ypos+1,xpos+w-1,ypos+h-1);
        TextOut(hDC,xpos+font_size/2,ypos+3,_RCT("Properties"),strlen("Properties"));
        SelectClipRgn(hDC,0);

        hpen  = CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNTEXT));
        holdpen = (RCHPEN) SelectObject(hDC,hpen);

        MoveToEx(hDC,xpos+w-32,ypos + 10,0);
        LineTo(hDC,xpos+w-19,ypos + 10);

        if (dock_left)
        {   MoveToEx(hDC,xpos+w-24,ypos + 8,0);
            LineTo(hDC,xpos+w-20,ypos + 10);
            MoveToEx(hDC,xpos+w-24,ypos + 12,0);
            LineTo(hDC,xpos+w-19,ypos + 10);
        }
        else
        {   MoveToEx(hDC,xpos+w-28,ypos + 8,0);
            LineTo(hDC,xpos+w-32,ypos + 10);
            MoveToEx(hDC,xpos+w-28,ypos + 12,0);
            LineTo(hDC,xpos+w-33,ypos + 10);
        }

        MoveToEx(hDC,xpos+w-7,ypos + 7,0);
        LineTo(hDC,xpos+w-14,ypos + 14);
        MoveToEx(hDC,xpos+w-13,ypos + 7,0);
        LineTo(hDC,xpos+w-6,ypos + 14);

        SelectObject(hDC,holdpen);
        DeleteObject(hpen);


        y = ypos + font_size * 2;

        for (i = 0 ; i < num_properties ; i++)
        {   if (properties[i].type == VP_GROUP)
            {   y += font_size / 2;
                properties[i].label_rect.left = xpos + font_size;
                properties[i].label_rect.right = xpos + w - font_size;
                properties[i].label_rect.top = y;
                properties[i].label_rect.bottom = y + font_size + 7;
                hbrush = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
                FillRect(hDC,&properties[i].label_rect,hbrush);
                DeleteObject(hbrush);
                rect = properties[i].label_rect;
                rect.left++;
                rect.right--;
                rect.top++;
                rect.bottom--;
                hbrush = CreateSolidBrush(GetSysColor(COLOR_3DSHADOW));
                FillRect(hDC,&rect,hbrush);
                DeleteObject(hbrush);

                c1 = GetSysColor(COLOR_3DDKSHADOW);
                c2 = GetSysColor(COLOR_3DSHADOW);
                c1 = RGB((GetRValue(c1)+GetRValue(c2)*3)/4,(GetGValue(c1)+GetGValue(c2)*3)/4,(GetBValue(c1)+GetBValue(c2)*3)/4);

                rect.left++;
                rect.right--;
                rect.top++;
                rect.bottom--;
                hbrush = CreateSolidBrush(c1);
                FillRect(hDC,&rect,hbrush);
                DeleteObject(hbrush);

                IntersectClipRect(hDC,rect.left,rect.top,rect.right,rect.bottom);
#ifdef _USING_QTCHAR
                TextOutW(hDC,rect.left+3,rect.top+2, properties[i].label,strlen(properties[i].label));
#else
                TextOut(hDC,rect.left+3,rect.top+2,properties[i].label,strlen(properties[i].label));
#endif
                SelectClipRgn(hDC,0);

                hpen  = CreatePen(PS_SOLID,1,GetSysColor(COLOR_BTNTEXT));
                holdpen = (RCHPEN) SelectObject(hDC,hpen);


                if (properties[i].state)
                {   y1 = y + 12;
                    dir = -1;
                }
                else
                {   y1 = y + 8;
                    dir = 1;
                }

                MoveToEx(hDC,xpos+w-15-font_size,y1,0);
                LineTo(hDC,xpos+w-14-font_size,y1);
                MoveToEx(hDC,xpos+w-16-font_size,y1+dir,0);
                LineTo(hDC,xpos+w-13-font_size,y1+dir);
                MoveToEx(hDC,xpos+w-17-font_size,y1+dir*2,0);
                LineTo(hDC,xpos+w-12-font_size,y1+dir*2);
                MoveToEx(hDC,xpos+w-18-font_size,y1+dir*3,0);
                LineTo(hDC,xpos+w-11-font_size,y1+dir*3);
                MoveToEx(hDC,xpos+w-19-font_size,y1+dir*4,0);
                LineTo(hDC,xpos+w-10-font_size,y1+dir*4);

                SelectObject(hDC,holdpen);
                DeleteObject(hpen);

                properties[i].value_rect.left = 0;
                properties[i].value_rect.right = 0;
                properties[i].value_rect.top = 0;
                properties[i].value_rect.bottom = 0;

                y += font_size + 6;

                if (properties[i].state)
                {   for (i++ ; i < num_properties ; i++)
                    {   if (properties[i].type == VP_GROUP)
                            break;
                        properties[i].label_rect.left = 0;
                        properties[i].label_rect.right = 0;
                        properties[i].label_rect.top = 0;
                        properties[i].label_rect.bottom = 0;
                        properties[i].value_rect.left = 0;
                        properties[i].value_rect.right = 0;
                        properties[i].value_rect.top = 0;
                        properties[i].value_rect.bottom = 0;
                    }
                    i--;
                }

            }
            else
            {   properties[i].label_rect.left = xpos + font_size*2;
                properties[i].label_rect.right = xpos + font_size*2 + label_width;
                properties[i].label_rect.top = y;
                properties[i].label_rect.bottom = y + font_size + 7;
                hbrush = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
                FillRect(hDC,&properties[i].label_rect,hbrush);
                DeleteObject(hbrush);
                rect = properties[i].label_rect;
                rect.left++;
                rect.right--;
                rect.top++;
                rect.bottom--;
                if (control_property_index == i)
                    hbrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
                else
                    hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
                FillRect(hDC,&rect,hbrush);
                DeleteObject(hbrush);

                IntersectClipRect(hDC,rect.left,rect.top,rect.right,rect.bottom);
#ifdef _USING_QTCHAR
                TextOutW(hDC,rect.left+3,rect.top+2, properties[i].label,strlen(properties[i].label));
#else
                TextOut(hDC,rect.left+3,rect.top+2,properties[i].label,strlen(properties[i].label));
#endif
                SelectClipRgn(hDC,0);

                properties[i].value_rect.left = xpos + font_size*2 + label_width;
                properties[i].value_rect.right = xpos + w - font_size;
                properties[i].value_rect.top = y;
                properties[i].value_rect.bottom = y + font_size + 7;
                hbrush = CreateSolidBrush(GetSysColor(COLOR_3DDKSHADOW));
                FillRect(hDC,&properties[i].value_rect,hbrush);
                DeleteObject(hbrush);
                rect = properties[i].value_rect;
                rect.left++;
                rect.right--;
                rect.top++;
                rect.bottom--;

#ifdef _USING_QTCHAR

                properties[i].state = properties[i].process_property(PP_GETVALUE,properties[i].id,properties[i].property_data,text);

#else
                properties[i].state = properties[i].process_property(PP_GETVALUE,properties[i].id,properties[i].property_data,text);
#endif
                if (properties[i].state == 0)
                    hbrush = CreateSolidBrush(RGB(255,255,255));
                else
                    hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
                FillRect(hDC,&rect,hbrush);
                DeleteObject(hbrush);

                IntersectClipRect(hDC,rect.left,rect.top,rect.right,rect.bottom);
#ifdef _USING_QTCHAR
                //if( ((Entity *)properties[i].property_data)->type() == text_entity )
                    TextOutW(hDC,rect.left+3,rect.top+2, text,strlen(text));
                //else
                //    TextOut(hDC,rect.left+3,rect.top+2,ctext,strlen(ctext));
#else
                TextOut(hDC,rect.left+3,rect.top+2,text,strlen(text));
#endif
                SelectClipRgn(hDC,0);

                y += font_size + 6;

            }

        }

        SelectObject(hDC,holdfont);

        drag_drawn = 0;


    }

}

void ViewProperties::size(int x,int y,int width,int height)
{   xpos = x;
    ypos = y;
    w = width;
    h = height;
}

void ViewProperties::fixup(Window *)
{
}

void ViewProperties::checkbutton(int c)
{
}

void ViewProperties::timerevent(void)
{
}

int ViewProperties::get_properties_width(void)
{   return property_width;
}

void ViewProperties::clear_properties(void)
{   num_properties = 0;
    if (control != 0)
    {   DestroyWindow(control);
        control = 0;
    }
}

void ViewProperties::add_property(RCCHAR *label,int open)
{   strcpy(properties[num_properties].label,label);
    properties[num_properties].process_property = 0;
    properties[num_properties].type = VP_GROUP;
    properties[num_properties].state = open;
    num_properties++;
}

 void ViewProperties::add_property(RCCHAR *label,int id,ProcessProperty pp,void *pd)
{   strcpy(properties[num_properties].label,label);
    properties[num_properties].process_property = pp;
    properties[num_properties].property_data = pd;
    properties[num_properties].type = VP_PROPERTY;
    properties[num_properties].id = id;
    properties[num_properties].state = 0;
    num_properties++;
}

void ViewProperties::restore_width(void)
{char text[300];
    GetPrivateProfileString("Layout","PropertyWidth","250",text,300,home.getinifilename());
    WritePrivateProfileString("Layout","CurrentPropertyWidth",text,home.getinifilename());
    property_width = atoi(text);
    cadwindow->sizeevent(-1,-1);
}


//
//  The 'ButtonMenus' class - THE place to store button all menus.
//

ButtonMenus::~ButtonMenus()
{OneButtonMenu *menu;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    delete menu;
  list.destroy();
}

ButtonMenu *ButtonMenus::match(const char *name)
{
    return match(CHAR2RCCHR(name));
}

ButtonMenu *ButtonMenus::match(const RCCHAR *name)
{
  OneButtonMenu *menu;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    if (strcmp(name,menu->name) == 0)
        return menu->menu;
  return NULL;
}

void ButtonMenus::deletemenu(ButtonMenu *bm)
{OneButtonMenu *menu;

  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    if (bm == menu->menu)
      {  list.del(menu);
         menu->menu = 0;  //  Don't delete the menu
         delete menu;
         break;
      }

}

RCCHAR *ButtonMenus::match(const ButtonMenu *bm)
{OneButtonMenu *menu;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    if (bm == menu->menu) return menu->name;
  return 0;
}

RCCHAR *ButtonMenus::title(const ButtonMenu *bm)
{OneButtonMenu *menu;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    if (bm == menu->menu) return menu->title;
  return 0;
}

ButtonMenu *ButtonMenus::getmenu(int i)
{OneButtonMenu *menu;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != 0 && i > 0 ; i--);
  return menu->menu;
}

Button *ButtonMenus::match(int commandid)
{OneButtonMenu *menu;
 Button *b;
  for (list.start() ; (menu = (OneButtonMenu *) list.next()) != NULL ; )
    if ((b = menu->menu->match(commandid)) != 0)
      return b;
  return NULL;
}

//
//  The 'Button' class
//
void Button::init(int xp,int yp, RCCHAR *name,int comid,RCHINSTANCE hinstance, QWidget *qparent)
{
  RCCHAR rname[128];
  unsigned char *bits,bmihdata[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
  BITMAP bm;
  BITMAPINFO *bmi = (BITMAPINFO *) bmihdata;
  int i,j,k,l,nbytes,w1;
  RCHDC hdc;

  sharedbitmaps = 0;
  hbitmap=0;
  hmaskbitmap=0;
  if(strlen(name) > 0 && hinstance != 0)
  {
      strcpy(bname,name);
      strcpy(rname,_RCT("CA_"));  strcat(rname,name);  strcat(rname,_RCT("_bitmap"));
      hbitmap = (RCHBITMAP)LoadBitmap(hinstance,rname);
      hmaskbitmap = (RCHBITMAP)LoadBitmap(hinstance,rname);
      if (hbitmap == 0 || hmaskbitmap == 0)
      {
          strcpy(rname,name);  strcat(rname,_RCT("_bitmap"));
          hbitmap = LoadBitmap(hinstance,rname);
          hmaskbitmap = LoadBitmap(hinstance,rname);
      }
  }
  else if(strlen(name) > 0 && hinstance == 0)
  {
      strcpy(bname,name);
      hbitmap = (RCHBITMAP)LoadBitmap(hinstance,name);
      hmaskbitmap = (RCHBITMAP)LoadBitmap(hinstance,name);
  }
  hsavebitmap = 0;

  xpos = xp;  ypos = yp;
  w = bm.bmWidth;  h = bm.bmHeight;

  butstate = over = 0;
  commandid = comid;
  description = comid == 0 ? 0 : commands.description(comid);
  //qDebug() << "button descr : " << QString(description);
  shortdescription = comid == 0 ? 0 : commands.shortdescription(comid);
  //qDebug() << "button short descr : " << QString(shortdescription);

  hDC = 0;

  // a button has a QToolButton object
  // the qparent is usually a QToolBar but it could be any type of Widget
  // if we have an RCToolBox add this button to it
  // else of qparent is null just make an RCToolButton
  if( qobject_cast<QDockWidget*>(qparent) != 0  )
  {// this is for the RCToolBox object
      m_qw = ((RCToolBox*)qparent)->addButton(((RCToolBox*)qparent)->nextId(),
                                              ((RCToolBox*)qparent)->nextSid(),
                                               QString(name),comid);
      if(m_qw)
      {
        hDC = m_qw;
        ((RCToolButton*)m_qw)->rcparent = this;
        w = m_qw->width();  h = m_qw->height();
        ((RCToolButton*)m_qw)->objectName() = "RCToolBox";
        //QString tooltip = QString(shortdescription);
        QString tooltip = QString(description);
        m_qw->setToolTip(tooltip);
        //qDebug() << "RCToolButton : commandid" <<((RCToolButton*)m_qw)->mcomId;
        //qDebug() << "RCToolButton : rcparent" <<((RCToolButton*)m_qw)->rcparent;
      }
      else
      {
          // make new button for later use
          m_qw = new RCToolButton(qparent);
          if(m_qw)
          {
               hDC = m_qw;
              ((RCToolButton*)m_qw)->rcparent = this;
              ((RCToolButton*)m_qw)->mcomId = commandid;
              ((RCToolButton*)m_qw)->objectName() = "RCToolButton";
              if(hbitmap)
              {
                ((QToolButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
                  delete (QPixmap*)hbitmap; hbitmap=0;
                  delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
              }
              w = m_qw->width();  h = m_qw->height();
              //QString tooltip = QString(shortdescription);
              QString tooltip = QString(description);
              m_qw->setToolTip(tooltip);
          }
      }
  }
  else if( qobject_cast<RCStyleEntryToolBar*>(qparent) != 0  )
  {
      m_qw = new RCStyleButton(qparent);
      if(m_qw)
      {
           hDC = m_qw;
          ((RCStyleButton*)m_qw)->rcparent = this;
          ((RCStyleButton*)m_qw)->mcomId = commandid;
          ((RCStyleButton*)m_qw)->objectName() = "RCToolButton";
          if(hbitmap)
          {
            ((RCStyleButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
            delete (QPixmap*)hbitmap; hbitmap=0;
            delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
          }
          w = m_qw->width();  h = m_qw->height();
          //QString tooltip = QString(shortdescription);
          QString tooltip = QString(description);
          m_qw->setToolTip(tooltip);
      }
  }
  else
  { // this is for when the button is used later
      m_qw = new RCToolButton(qparent);
      if(m_qw)
      {
           hDC = m_qw;
          ((RCToolButton*)m_qw)->rcparent = this;
          ((RCToolButton*)m_qw)->mcomId = commandid;
          ((RCToolButton*)m_qw)->objectName() = "RCToolButton";
          if(hbitmap)
          {
            ((QToolButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
              delete (QPixmap*)hbitmap; hbitmap=0;
              delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
          }
          w = m_qw->width();  h = m_qw->height();
          //QString tooltip = QString(shortdescription);
          QString tooltip = QString(description);
          m_qw->setToolTip(tooltip);
      }
  }
  // disable if command not yet implemented
  if(m_qw)
  {
      if(commands.implemented(comid) == 0)
          m_qw->setEnabled(false);
  }
}

Button::Button(int xp, int yp, char *name, int comid, RCHINSTANCE hinstance, QWidget *qparent)
    //: Button(xp, yp,CHAR2RCCHR(name),comid,hinstance,qparent) // C++ 11 only
{
    init(xp, yp,CHAR2RCCHR(name),comid,hinstance,qparent);
}

Button::Button(int xp, int yp, char *name, int comid, QWidget *qparent, RCHINSTANCE hinstance)
    //: Button(xp, yp,CHAR2RCCHR(name),comid,hinstance,qparent) // C++ 11 only
{
    init(xp, yp,CHAR2RCCHR(name),comid,hinstance,qparent);
}

Button::Button(int xp, int yp, RCCHAR *name, int comid, RCHINSTANCE hinstance, QWidget *qparent)
{
    init(xp, yp,name,comid,hinstance,qparent);
}

Button::Button(int xp, int yp,RCHBITMAP icon,RCCHAR *name, int comid, RCHINSTANCE hinstance, QWidget *qparent)
{
    init(xp, yp,name,comid,0,qparent);
    hbitmap = icon;
    hmaskbitmap = icon;
}

Button::Button(int xp, int yp, RCCHAR *name, int comid, QWidget *qparent, RCHINSTANCE hinstance)
{
    init(xp, yp,name,comid,hinstance,qparent);
#if 0
  RCCHAR rname[128];
  unsigned char *bits,bmihdata[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
  BITMAP bm;
  BITMAPINFO *bmi = (BITMAPINFO *) bmihdata;
  int i,j,k,l,nbytes,w1;
  RCHDC hdc;

  sharedbitmaps = 0;
  strcpy(bname,name);
  strcpy(rname,_RCT("CA_"));  strcat(rname,name);  strcat(rname,_RCT("_bitmap"));
  hbitmap = LoadBitmap(hinstance,rname);
  hmaskbitmap = LoadBitmap(hinstance,rname);
  if (hbitmap == 0 || hmaskbitmap == 0)
  {
      strcpy(rname,name);  strcat(rname,_RCT("_bitmap"));
      hbitmap = LoadBitmap(hinstance,rname);
      hmaskbitmap = LoadBitmap(hinstance,rname);
  }

  hsavebitmap = 0;

//  Change colors to reflect current user settings
#if ! defined(_WIN32_WCE)
  for (l = 0 ; l < 2 ; l++)
    {
       if (l == 0 && hbitmap == 0 || l == 1 && hmaskbitmap == 0)
         continue;

       if (GetObject(l == 0 ? hbitmap : hmaskbitmap, sizeof(BITMAP), &bm) == 0)
         continue;

       bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
       bmi->bmiHeader.biWidth = bm.bmWidth;
       bmi->bmiHeader.biHeight = bm.bmHeight;
       bmi->bmiHeader.biPlanes = 1;
       bmi->bmiHeader.biBitCount = 8;
       bmi->bmiHeader.biCompression = BI_RGB;
       bmi->bmiHeader.biSizeImage = 0;
       bmi->bmiHeader.biXPelsPerMeter = 0;
       bmi->bmiHeader.biYPelsPerMeter = 0;
       bmi->bmiHeader.biClrUsed = 256;
       bmi->bmiHeader.biClrImportant = 256;
       hdc = GetDC(0);

       if (bm.bmBitsPixel	== 8)
         {  bits = new unsigned char[((bm.bmWidth+3)/4) * 4 * bm.bmHeight];
            if (GetDIBits(hdc,l == 0 ? hbitmap : hmaskbitmap,0,bm.bmHeight,bits,bmi,DIB_RGB_COLORS))
              {  if (l == 0)
                   {  for (i = 0 ; i < 256 ; i++)
                        if (bmi->bmiColors[i].rgbRed == 192 &&
                            bmi->bmiColors[i].rgbBlue == 192 &&
                            bmi->bmiColors[i].rgbGreen == 192)
                          {  bmi->bmiColors[i].rgbRed = 0;
                             bmi->bmiColors[i].rgbGreen = 0;
                             bmi->bmiColors[i].rgbBlue = 0;
                          }
                   }
                 else
                   {  for (i = 0 ; i < 256 ; i++)
                        if (bmi->bmiColors[i].rgbRed != 192 ||
                            bmi->bmiColors[i].rgbBlue != 192 ||
                            bmi->bmiColors[i].rgbGreen != 192)
                          {  bmi->bmiColors[i].rgbRed = 0;
                             bmi->bmiColors[i].rgbGreen = 0;
                             bmi->bmiColors[i].rgbBlue = 0;
                          }
                        else
                          {  bmi->bmiColors[i].rgbRed = 255;
                             bmi->bmiColors[i].rgbGreen = 255;
                             bmi->bmiColors[i].rgbBlue = 255;
                          }
                   }
                 SetDIBits(hdc,l == 0 ? hbitmap : hmaskbitmap,0,bm.bmHeight,bits,bmi,DIB_RGB_COLORS);
              }
            delete [] bits;
         }
       else
         {  bits = new unsigned char[((bm.bmWidth*3+3)/4) * 4 * bm.bmHeight];
            bmi->bmiHeader.biBitCount = 24;
            bmi->bmiHeader.biClrUsed = 0;
            bmi->bmiHeader.biClrImportant = 0;
            if (GetDIBits(hdc,l == 0 ? hbitmap : hmaskbitmap,0,bm.bmHeight,bits,bmi,DIB_RGB_COLORS))
              {  w1 = ((bm.bmWidth * 3 + 3) / 4) * 4;
                 nbytes = bm.bmWidth * 3;
                 for (i = 0 ; i < bm.bmHeight ; i++)
                   for (j = 0 ; j < nbytes ; j += 3)
                     {  k = i * w1 + j;
                        if (l == 0)
                          {  if (j == 0 || abs(bits[k]-192) < 3 && abs(bits[k+1]-192) < 3 && abs(bits[k+2]-192) < 3)
                               {  bits[k] = 0;
                                  bits[k+1] = 0;
                                  bits[k+2] = 0;
                               }
                          }
                        else
                          {  if (j != 0 && (abs(bits[k]-192) > 3 || abs(bits[k+1]-192) > 3 || abs(bits[k+2]-192) > 3))
                               {  bits[k] = 0;
                                  bits[k+1] = 0;
                                  bits[k+2] = 0;
                               }
                             else
                               {  bits[k] = 255;
                                  bits[k+1] = 255;
                                  bits[k+2] = 255;
                               }
                          }
                     }
                 SetDIBits(hdc,l == 0 ? hbitmap : hmaskbitmap,0,bm.bmHeight,bits,bmi,DIB_RGB_COLORS);
              }
            delete [] bits;
         }
       ReleaseDC(0,hdc);
    }

#endif

  xpos = xp;  ypos = yp;
  w = bm.bmWidth;  h = bm.bmHeight;

  butstate = over = 0;
  commandid = comid;
  description = commands.description(comid);
  shortdescription = commands.shortdescription(comid);

  hDC = 0;

  // a button has a QToolButton object
  // the qparent is usually a QTooBar but it could be any type of Widget
  m_qw = new QToolButton(qparent);
  ((QToolButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
#endif
}

void Button::save(int i,RCCHAR *name)
{
  qDebug() << "button stave name: " << QString(name);
  RCCHAR key[300];
  sprintf(key,"mn::%lsMenu::Id%d",name,i);
  qDebug() << "button save key: " << QString(key);
  v.setinteger(key,commandid);
  sprintf(key,"mn::%lsMenu::Name%d",name,i);
  qDebug() << "button save key: " << QString(key);
  v.setstring(key,bname);
}

Button::Button(int xp,int yp,Button *button,int comid,QWidget *qparent)
{
  sharedbitmaps = 1;
  strcpy(bname,button->bname);
  hbitmap = button->hbitmap;
  hmaskbitmap = button->hmaskbitmap;
  hsavebitmap = 0;

  RCCHAR rname[128];
  strcpy(rname,_RCT("CA_"));  strcat(rname,bname);  strcat(rname,_RCT("_bitmap"));
  hbitmap = (RCHBITMAP)LoadBitmap(program->gethinst(),rname);
  hmaskbitmap = (RCHBITMAP)LoadBitmap(program->gethinst(),rname);
  if (hbitmap == 0 || hmaskbitmap == 0)
  {
      strcpy(rname,bname);  strcat(rname,_RCT("_bitmap"));
      hbitmap = LoadBitmap(program->gethinst(),rname);
      hmaskbitmap = LoadBitmap(program->gethinst(),rname);
  }
  /*
  if(strlen(bname) > 0 && hinstance != 0)
  {
      //strcpy(bname,name);
      strcpy(rname,_RCT("CA_"));  strcat(rname,bname);  strcat(rname,_RCT("_bitmap"));
      hbitmap = (RCHBITMAP)LoadBitmap(hinstance,rname);
      hmaskbitmap = (RCHBITMAP)LoadBitmap(hinstance,rname);
      if (hbitmap == 0 || hmaskbitmap == 0)
      {
          strcpy(rname,bname);  strcat(rname,_RCT("_bitmap"));
          hbitmap = LoadBitmap(0,rname);
          hmaskbitmap = LoadBitmap(0,rname);
      }
  }
  else if(strlen(bname) > 0 && hinstance == 0)
  {
      //strcpy(bname,name);
      hbitmap = (RCHBITMAP)LoadBitmap(0,bname);
      hmaskbitmap = (RCHBITMAP)LoadBitmap(0,bname);
  }
  */
  hsavebitmap = 0;

  xpos = xp;  ypos = yp;
  w = button->w;
  h = button->h;


  butstate = over = 0;
  commandid = comid;
  description = commands.description(comid);
  shortdescription = commands.shortdescription(comid);

  // a button has a QToolButton object
  // the qparent is usually a QToolBar but it could be any type of Widget
  // if we have an RCToolBox add this button to it
  // else of qparent is null just make an RCToolButton
  if( qobject_cast<QDockWidget*>(qparent) != 0  )
  {// this is for the RCToolBox object
      m_qw = ((RCToolBox*)qparent)->addButton(((RCToolBox*)qparent)->nextId(),
                                              ((RCToolBox*)qparent)->nextSid(),
                                               QString(bname),comid);
      if(m_qw)
      {
        hDC = m_qw;
        ((RCToolButton*)m_qw)->rcparent = this;
        w = m_qw->width();  h = m_qw->height();
        ((RCToolButton*)m_qw)->objectName() = "RCToolBox";
        //QString tooltip = QString(shortdescription);
        QString tooltip = QString(description);
        m_qw->setToolTip(tooltip);
        //qDebug() << "RCToolButton : commandid" <<((RCToolButton*)m_qw)->mcomId;
        //qDebug() << "RCToolButton : rcparent" <<((RCToolButton*)m_qw)->rcparent;
      }
      else
      {
          // make new button for later use
          m_qw = new RCToolButton(qparent);
          if(m_qw)
          {
               hDC = m_qw;
              ((RCToolButton*)m_qw)->rcparent = this;
              ((RCToolButton*)m_qw)->mcomId = commandid;
              ((RCToolButton*)m_qw)->objectName() = "RCToolButton";
              if(hbitmap)
              {
                ((QToolButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
                delete (QPixmap*)hbitmap; hbitmap=0;
                delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
              }
              w = m_qw->width();  h = m_qw->height();
              //QString tooltip = QString(shortdescription);
              QString tooltip = QString(description);
              m_qw->setToolTip(tooltip);
          }
      }
  }
  else if( qobject_cast<RCStyleEntryToolBar*>(qparent) != 0  )
  {
      m_qw = new RCStyleButton(qparent);
      if(m_qw)
      {
           hDC = m_qw;
          ((RCStyleButton*)m_qw)->rcparent = this;
          ((RCStyleButton*)m_qw)->mcomId = commandid;
          ((RCStyleButton*)m_qw)->objectName() = "RCToolButton";
          if(hbitmap)
          {
            ((RCStyleButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
              delete (QPixmap*)hbitmap; hbitmap=0;
              delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
          }
          w = m_qw->width();  h = m_qw->height();
          //QString tooltip = QString(shortdescription);
          QString tooltip = QString(description);
          m_qw->setToolTip(tooltip);
      }
  }
  else
  { // this is for when the button is used later
      m_qw = new RCToolButton(qparent);
      if(m_qw)
      {
           hDC = m_qw;
          ((RCToolButton*)m_qw)->rcparent = this;
          ((RCToolButton*)m_qw)->mcomId = commandid;
          ((RCToolButton*)m_qw)->objectName() = "RCToolButton";
          if(hbitmap)
          {
            ((QToolButton*)m_qw)->setIcon(QIcon(*(QPixmap*)hbitmap));
            delete (QPixmap*)hbitmap; hbitmap=0;
            delete (QPixmap*)hmaskbitmap; hmaskbitmap=0;
          }
          w = m_qw->width();  h = m_qw->height();
          //QString tooltip = QString(shortdescription);
          QString tooltip = QString(description);
          m_qw->setToolTip(tooltip);
      }
  }
  // disable if command not yet implemented
  if(m_qw)
  {
      if(commands.implemented(comid) == 0)
          m_qw->setEnabled(false);
  }
}

Button::~Button()
{
    if (! sharedbitmaps)
    {
       //delete hbitmap; hbitmap=0;
       //delete hmaskbitmap;hmaskbitmap=0;
       //DeleteObject((RCHGDIOBJ)hbitmap);hbitmap=0;
       //DeleteObject((RCHGDIOBJ)hmaskbitmap);hmaskbitmap=0;
    }
    m_qw->setParent(0);
    m_qw->deleteLater();
    m_qw=0;
}

int Button::pick(int xp,int yp)
{
  RCHDC memhdc,screenhdc;
  RCHBITMAP hbitmapold;
  timerticks = 0;
  QRect grect;

  overx = xp;
  overy = yp;

  if(!m_qw)
      return 0;

   //if (xp < x || xp > x + w - 1 || yp < y || yp > y + h - 1)
  //if(!m_qw->rect().contains(m_qw->mapFromGlobal(QPoint(xp,yp))))
  if(app->widgetAt(xp,yp) != m_qw )
  {
       if (butstate == 1 || butstate == 2)
       {
           butstate = 0;  draw(DM_NORMAL);
       }
       if (over == 1)
       {
            over = 0;
            draw(DM_NORMAL);
            cadwindow->prompt->shownormalprompt();
            if (xp != -32767 || yp != -32767)
              program->setcommandid(0);
       }
       return 0;
  }
  else if (strlen(description) > 0)
  {
        if (over == 0)
        {
            cadwindow->prompt->tempprompt(description);
            program->setcommandid(commandid);
            over = 1;
            if (butstate != 3 && butstate != 4)
              butstate = 1;
            Sleep(30);
            draw(DM_NORMAL);
        }
        else
        {
            cadwindow->prompt->showtempprompt();
            program->setcommandid(commandid);
        }
  }
  return 1;
}

void Button::mousemoveevent(int,int,int buttons)
{
    if ((butstate < 2 || butstate == 4) && (MK_LBUTTON & buttons))
     {  butstate = 2;
        over = 1;
        draw(DM_NORMAL);
     }
   else if ((butstate == 2 || butstate == 3) && ! (MK_LBUTTON & buttons))
     {  butstate = butstate == 2 ? 0 : 4;
        over = 0;
        if (hsavebitmap != 0)
          {RCHDC memhdc,screenhdc;
           RCHBITMAP hbitmapold;
             memhdc = CreateCompatibleDC(cadwindow->gethdc());
             screenhdc = GetDC(0);
             hbitmapold = (RCHBITMAP) SelectObject(memhdc,hsavebitmap);
             BitBlt(screenhdc,saveoverx,saveovery,swidth,sheight,memhdc,0,0,SRCCOPY);
             SelectObject(memhdc,hbitmapold);
             DeleteObject(hsavebitmap);
             DeleteDC(memhdc);
             ReleaseDC(0,screenhdc);
             hsavebitmap = 0;
             timerticks = 0;
          }
        draw(DM_NORMAL);
        NewCommandEvent *nevent = new NewCommandEvent(commandid,parent);
        state.sendevent(nevent);
     }
}

void Button::draw(int)
{
#ifdef USING_WIDGETS
    if(m_qw)
    {
        m_qw->update();
    }
#else
 RCHDC      memhdc;
 RCHBITMAP  hbitmapold;
 RECT rect;
 RCHBRUSH hbrush1,hbrush2,hbrush3,hbrush4;
  if (hDC == 0) return;
  hbrush1 = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
  hbrush2 = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  memhdc = CreateCompatibleDC(hDC);
  rect.left = x;
  rect.right = x + w;
  rect.top = y;
  rect.bottom = y + h;
  FillRect(hDC,&rect,hbrush1);

  if (butstate != 0)
    {  if (butstate >= 2)
         {  hbrush3 = hbrush2;
            hbrush4 = (RCHBRUSH)GetStockObject(WHITE_BRUSH);
         }
       else
         {  hbrush3 = (RCHBRUSH)GetStockObject(WHITE_BRUSH);
            hbrush4 = hbrush2;
         }
       rect.left = x;
       rect.right = x + 1;
       rect.top = y;
       rect.bottom = y + h;
       FillRect(hDC,&rect,hbrush3);
       rect.left = x;
       rect.right = x + w;
       rect.top = y;
       rect.bottom = y + 1;
       FillRect(hDC,&rect,hbrush3);
       rect.left = x;
       rect.right = x + w;
       rect.top = y + h - 1;
       rect.bottom = y + h;
       FillRect(hDC,&rect,hbrush4);
       rect.left = x + w - 1;
       rect.right = x + w;
       rect.top = y;
       rect.bottom = y + h;
       FillRect(hDC,&rect,hbrush4);
       hbitmapold = (RCHBITMAP)SelectObject(memhdc,hmaskbitmap);
       BitBlt(hDC,x + (butstate >= 2),y+(butstate >= 2),w-(butstate >= 2)*2,h-(over == 2)*2,memhdc,0,0,SRCAND);
       SelectObject(memhdc,hbitmap);
       BitBlt(hDC,x + (butstate >= 2),y+(butstate >= 2),w-(butstate >= 2)*2,h-(over == 2)*2,memhdc,0,0,SRCPAINT);
       SelectObject(memhdc,hbitmapold);
    }
  else
    {  hbitmapold = (RCHBITMAP) SelectObject(memhdc,hmaskbitmap);
       BitBlt(hDC,x,y,w,h,memhdc,0,0,SRCAND);
       SelectObject(memhdc,hbitmap);
       BitBlt(hDC,x,y,w,h,memhdc,0,0,SRCPAINT);
       SelectObject(memhdc,hbitmapold);

    }
  DeleteDC(memhdc);

  buttonmenus.match(_RCT("Tool"))->drawdivide();
  buttonmenus.match(_RCT("Interrupt"))->drawdivide();

  if (butstate == 0)
    {  rect.left = x;
       rect.right = x + 1;
       rect.top = y + h - 1;
       rect.bottom = y + h + 1;
       FillRect(hDC,&rect,hbrush1);
    }

  DeleteObject(hbrush1);
  DeleteObject(hbrush2);
#endif
}

void Button::in(void)
{
    if (butstate == 0)
    {
       over = 1;
       butstate = 2;
       draw(DM_NORMAL);
    }
}

void Button::out(void)
{
    if (butstate >= 2)
    {
       over = 0;
       butstate = butstate == 2 ? 0 : 4;
       draw(DM_NORMAL);
    }
}

void Button::select(int s)
{
    //qDebug() << "entering button::select";
    //qDebug() << "button name :" << name();
    //qDebug() << "butstate : " << butstate;

    if (butstate != 3 && butstate != 4)
    {
       butstate = s;
       if(m_qw)
       {
           ((RCToolButton*)m_qw)->clicked();
           ((RCToolButton*)m_qw)->setChecked(true);
           ((RCToolButton*)m_qw)->setDown(true);
       }
       draw(DM_NORMAL);
       // KMJ :
       // TODO : find a way to update the button display
       // without using app->processEvents
       //state.process();
       //app->processEvents();

    }

    //qDebug() << "leaving button::select";
    //qDebug() << "butstate : " << butstate;
}

void Button::deselect(void)
{
    //qDebug() << "entering button::deselect";
    //qDebug() << "button name :" << name();
    //qDebug() << "butstate : " << butstate;

    if (butstate == 3 || butstate == 4)
    {
       butstate = 0;
       if(m_qw)
       {
           if(dynamic_cast<RCToolButton*>(m_qw) != 0)
           {
               ((RCToolButton*)m_qw)->setChecked(false);
               ((RCToolButton*)m_qw)->setDown(false);
           }
       }
       draw(DM_NORMAL);
       //state.process();
       //app->processEvents();
    }

    //qDebug() << "leaving button::deselect";
    //qDebug() << "butstate : " << butstate;
}

void Button::timerevent(void)
{
 #if 0
 RCHDC memhdc,screenhdc;
 RCHBITMAP  hbitmapold;
 RCHFONT hfont,holdfont;
 RECT rect;
 POINT p;
  if (! over || timerticks++ < 4 || hsavebitmap != 0)
    return;
  overx += 10;  overy += h;
  p.x = overx;
  p.y = overy;
  ClientToScreen(parent->gethwnd(),&p);
  overx = p.x;
  overy = p.y;
  screenhdc = GetDC(0);
  hfont = CreateFont(14,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Calibri"));
  holdfont = (RCHFONT)SelectObject(screenhdc,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(screenhdc,shortdescription,strlen(shortdescription),&size);
  swidth = size.cx + 6;
  sheight = size.cy + 6;
#else
  //swidth = LOWORD(GetTextExtent(screenhdc,shortdescription,strlen(shortdescription))) + 6;
  //sheight = HIWORD(GetTextExtent(screenhdc,shortdescription,strlen(shortdescription))) + 6;
#endif
  if (overx + swidth >= GetDeviceCaps(screenhdc,HORZRES))
    overx = GetDeviceCaps(screenhdc,HORZRES) - swidth;
  if (overy + sheight >= GetDeviceCaps(screenhdc,VERTRES))
    overy = GetDeviceCaps(screenhdc,VERTRES) - sheight;
  memhdc = CreateCompatibleDC(cadwindow->gethdc());
  hsavebitmap = CreateCompatibleBitmap(cadwindow->gethdc(),swidth,sheight);
  hbitmapold = (RCHBITMAP)SelectObject(memhdc,hsavebitmap);
  saveoverx = overx;  saveovery = overy;
  BitBlt(memhdc,0,0,swidth,sheight,screenhdc,saveoverx,saveovery,SRCCOPY);
  SelectObject(memhdc,hbitmapold);
  DeleteDC(memhdc);
  rect.left = overx;  rect.right = overx + swidth - 1;
  rect.top = overy;  rect.bottom = overy + sheight - 1;
  FillRect(screenhdc,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  FrameRect(screenhdc,&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  TextOut(screenhdc,overx+3,overy+3,shortdescription,strlen(shortdescription));
  SelectObject(screenhdc,holdfont);
  DeleteObject(hfont);
  ReleaseDC(0,screenhdc);
#endif
}

//
//  The 'Prompt' class
//
#ifdef USING_WIDGETS
void Prompt::windowHelper(QWidget *qparent)
{
    parent = new Window(_RCT("STATIC"),qparent,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                           xpos,ypos,w,16,SW_SHOW);
    // set the QWidget as the hDC
    hDC = (QWidget*)parent->gethwnd();
    //qDebug() << "prompt parent " << (QObject*)parent->gethwnd();
    // set the prompt widget to the child if there is a ToolBar
    m_qw=0;
    //if(dynamic_cast<QToolBar *>(qparent) != 0){
    if(((QObject*)parent->gethwnd())->objectName() == "QToolBar" ){
        m_qw = ((QObject*)parent->gethwnd())->findChild<QLabel *>("QLabel");
        if(m_qw == 0)
            m_qw = ((QObject*)parent->gethwnd())->findChild<QLabel *>("QLineEdit");
    }
    if(m_qw == 0)
        m_qw = (QWidget*)hDC;
}

Prompt::Prompt(int xp,int yp,int width,QWidget *qparent)
{
  m_qw = 0;
  parent = 0;
  xpos = xp;  ypos = yp;
  w = width;
  promptdisplayed = 0;
  curprompt[0] = nprompt[0] = tprompt[0] = 0;
  colour = RGB(0,0,0);
  backgroundcolour = GetSysColor(COLOR_BTNFACE);

  windowHelper(qparent);
}
#else
Prompt::Prompt(int xp,int yp,int width)
#ifdef DERIVE_FROM_WIDGETS
    : Control(_qparent)
#endif
{
  parent = 0;
  xpos = xp;  ypos = yp;
  w = width;
  promptdisplayed = 0;
  curprompt[0] = nprompt[0] = tprompt[0] = 0;
  colour = RGB(0,0,0);
  backgroundcolour = GetSysColor(COLOR_BTNFACE);
}
#endif

#ifdef USING_WIDGETS
Prompt::Prompt(int xp,int yp,char *normal, QWidget *qparent)
{
  m_qw = 0;
  parent = 0;
  xpos = xp;  ypos = yp;
  tprompt[0] = 0;
  strcpy(curprompt,normal);
  strcpy(nprompt,normal);
  promptdisplayed = -1;
  colour = RGB(0,0,0);
  backgroundcolour = GetSysColor(COLOR_BTNFACE);

  windowHelper(qparent);
}
#else
Prompt::Prompt(int xp,int yp,char *normal)
#ifdef DERIVE_FROM_WIDGETS
    : Control(_qparent)
#endif
{
    parent = 0;
    xpos = xp;  ypos = yp;
  tprompt[0] = 0;
  strcpy(curprompt,normal);
  strcpy(nprompt,normal);
  promptdisplayed = -1;
  colour = RGB(0,0,0);
  backgroundcolour = GetSysColor(COLOR_BTNFACE);
}
#endif

void Prompt::move(Window *win,int xp,int yp)
{
  SIZE size;
  RCHFONT hfont;
  xpos = xp;  ypos = yp;
#ifdef USING_WIDGETS
  fixup(win);
  h = 21;
  //qDebug() << "Prompt parent is a : " << ((QWidget*)parent->gethdc());
  if(((QObject*)parent->gethwnd())->objectName() == "QToolBar" )
  {
      ((QWidget*)parent->gethdc())->move(xp,yp);
      ((QWidget*)parent->gethdc())->update();
  }

#else
  fixup(win);
  parent = win;
  hDC = (QWidget*)parent->gethdc();
  hfont = (RCHFONT)SelectObject((QWidget*)hDC,MainWindow::gethsfont());
  GetTextExtentPoint32((QWidget*)hDC,_RCT("A"),1,&size);
  h = 21;
  SelectObject((QWidget*)hDC,hfont);
#endif
}

void Prompt::setcolour(RCCOLORREF cr)
{
#if defined(USING_WIDGETS)
    if (colour != cr)
    {
        colour = cr;
        QPalette palette = m_qw->palette();
        palette.setColor(QPalette::WindowText,QColor((int)RCGetRValue(colour),(int)RCGetGValue(colour),(int)RCGetBValue(colour)));
        m_qw->setPalette(palette);
       ((QWidget*)parent->gethdc())->update();
    }
#elif defined(DERIVE_FROM_WIDGETS)
    if (colour != cr)
    {  colour = cr;
        this->update();
    }
#else
    if (colour != cr)
    {  colour = cr;
       draw(DM_NORMAL);
    }
#endif
}

void Prompt::setbackgroundcolour(RCCOLORREF cr)
{
  backgroundcolour = cr;
#if defined(USING_WIDGETS)
  //draw(DM_NORMAL);
  QPalette palette = m_qw->palette();
  palette.setColor(QPalette::Window,QColor((int)RCGetRValue(backgroundcolour),(int)RCGetGValue(backgroundcolour),(int)RCGetBValue(backgroundcolour)));
  m_qw->setPalette(palette);
  ((QWidget*)parent->gethdc())->update();
#elif defined(DERIVE_FROM_WIDGETS)
  //paintEvent(&QPaintEvent(QWidget::rect()));
  this->update();
#else
  draw(DM_NORMAL);
#endif
}

void Prompt::paintEvent(QPaintEvent *)
{
    draw(DM_NORMAL);
}

void  Prompt::draw(int drawmode)
{
#ifdef USING_WIDGETS
    if (cadwindow != 0 && cadwindow->printpreviewwindow != 0)  //  No prompts during print preview
      return;
    if (drawmode == DM_NORMAL)
    {
        QPalette palette = m_qw->palette();
        palette.setColor(QPalette::WindowText,QColor((int)RCGetRValue(colour),(int)RCGetGValue(colour),(int)RCGetBValue(colour)));
        m_qw->setPalette(palette);

    }
    else
    {
        QPalette palette = m_qw->palette();
        palette.setColor(QPalette::Window,QColor((int)RCGetRValue(backgroundcolour),(int)RCGetGValue(backgroundcolour),(int)RCGetBValue(backgroundcolour)));
        m_qw->setPalette(palette);
    }
    ((QWidget*)parent->gethdc())->update();
#else
 RCHFONT hfont;
 RECT rect;
  if (cadwindow != 0 && cadwindow->printpreviewwindow != 0)  //  No prompts during print preview
    return;
  if (drawmode == DM_NORMAL)
    {
      SetTextColor(hDC,colour);
    }
  else
    {
      SetTextColor(hDC,backgroundcolour);  promptdisplayed = -1;
    }
  SetBkMode(hDC,TRANSPARENT);
  //hfont = (RCHFONT)SelectObject((QWidget*)hDC,MainWindow::gethsfont());
  rect.left = x;  rect.right = x + w;  rect.top = y;  rect.bottom = y + h;
  FillRect((QWidget*)hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  TextOut((QWidget*)hDC,x,y,curprompt,strlen(curprompt));
  //SelectObject((QWidget*)hDC,hfont);
  if (w > 0)
      drawborder();
#endif
#ifdef DERIVE_FROM_WIDGETS
      Control::update();
    //Control::paintEvent(&QPaintEvent(QWidget::rect()));
#endif
}

void Prompt::shownormalprompt(void)
{
  RCHFONT hfont;
  if (cadwindow != 0 && cadwindow->printpreviewwindow != 0)  //  No prompts during print preview
    return;
  if (promptdisplayed == 1) return;
#ifdef USING_WIDGETS
  strncpy(curprompt,nprompt,119);
  curprompt[119] = 0;
  if(m_qw->objectName().contains("QLabel") ||
     m_qw->objectName().contains("PromptLabel") ||
     m_qw->objectName().contains("StatusLabel"))
  {
      ((QLabel*)m_qw)->clear();
      ((QLabel*)m_qw)->setText(QString(curprompt));
      promptdisplayed = 1;
      ((QLabel*)m_qw)->update();
  }
  else if(m_qw->objectName().contains("QLineEdit"))
  {
      ((QLineEdit*)m_qw)->clear();
      ((QLineEdit*)m_qw)->setText(QString(curprompt));
      promptdisplayed = 1;
      ((QLineEdit*)m_qw)->update();
  }
  //((QWidget*)parent->gethdc())->update();

#else
  SetTextColor(hDC,backgroundcolour);  //
  hfont = (RCHFONT)SelectObject(hDC,MainWindow::gethsfont());
  TextOut(hDC,x,y,curprompt,strlen(curprompt));
  promptdisplayed = 1;
  SetTextColor(hDC,colour);
  strncpy(curprompt,nprompt,119);  curprompt[119] = 0;
  TextOut(hDC,x,y,curprompt,strlen(curprompt));
  SelectObject(hDC,hfont);
#endif
}

void  Prompt::showtempprompt(void)
{RCHFONT hfont;
  if (cadwindow != 0 && cadwindow->printpreviewwindow != 0)  //  No prompts during print preview
    return;
  if (promptdisplayed == 0) return;
#ifdef USING_WIDGETS
  strcpy(curprompt,tprompt);
  if(m_qw->objectName().contains("QLabel") ||
     m_qw->objectName().contains("PromptLabel"))
  {
      ((QLabel*)m_qw)->setTextFormat(Qt::AutoText);
      //((QLabel*)m_qw)->setText("");
      ((QLabel*)m_qw)->clear();
      ((QLabel*)m_qw)->setText(QString(this->curprompt));
      ((QLabel*)m_qw)->update();
      promptdisplayed = 0;
  }
  else if(m_qw->objectName().contains("QLineEdit"))
  {
      ((QLineEdit*)m_qw)->setText("");
      ((QLineEdit*)m_qw)->setText(QString(this->curprompt));
      ((QLineEdit*)m_qw)->update();
      promptdisplayed = 0;
  }
  //((QWidget*)parent->gethdc())->update();
#else
  SetTextColor(hDC,backgroundcolour);
  hfont = (RCHFONT)SelectObject(hDC,MainWindow::gethsfont());
  TextOut(hDC,x,y,curprompt,strlen(curprompt));
  promptdisplayed = 0;
  SetTextColor(hDC,colour);
  strcpy(curprompt,tprompt);
  TextOut(hDC,x,y,curprompt,strlen(curprompt));
  SelectObject(hDC,hfont);
#endif
}

void Prompt::normalprompt(const char *prompt)
{
    QString nps(prompt);
    normalprompt(nps.data());
}

void Prompt::normalprompt(const RCCHAR *prompt)
{
   strcpy(nprompt,prompt);
   promptdisplayed = -1;
   shownormalprompt();
   if (cadwindow != 0 && cadwindow->getcommandwindow() != 0 && this == cadwindow->prompt)
     cadwindow->getcommandwindow()->insert((char *)prompt);
}

void Prompt::normalprompt(int id)
{
   ResourceString rs(id);
   strcpy(nprompt,rs.gets());
   promptdisplayed = -1;
   shownormalprompt();
   if (cadwindow != 0 && cadwindow->getcommandwindow() != 0 && this == cadwindow->prompt)
     cadwindow->getcommandwindow()->insert((char *)rs.gets());
}

void Prompt::tempprompt(const RCCHAR *prompt)
{  strcpy(tprompt,prompt);
   promptdisplayed = -1;
   showtempprompt();
}

void Prompt::tempprompt(int id)
{
   ResourceString rs(id);
   strcpy(tprompt,rs.gets());
   promptdisplayed = -1;
   showtempprompt();
}

LayerWindow::LayerWindow(Window *window,int w,int h) :
  Window((char*)"CadWindow",window->gethwnd(),(DWORD)(WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VSCROLL),0,0,w,h,SW_NORMAL)
{SIZE size;
 ResourceString rs31(NCWIN+31);
  SetScrollRange(gethwnd(),SB_VERT,0,1000,TRUE);
  hfont = CreateFont(14,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Arial"));
  holdfont = (RCHFONT)SelectObject(gethdc(),hfont);
  GetTextExtentPoint(gethdc(),_RCT("A"),1,&size);
  cheight = size.cy;
  cwidth = size.cx;
  ybase = 0;
  selectall = 1;
  displayall = 1;
  title(rs31.gets());

  MONITORINFO mi;
  RECT wrect;

  mi.cbSize = sizeof(mi);
  if (GetMonitorInfo(MonitorFromWindow(cadwindow->gethwnd(),MONITOR_DEFAULTTOPRIMARY),&mi))
    {  wrect = mi.rcMonitor;
    }
  else
    GetWindowRect(GetDesktopWindow(),&wrect);

  SetWindowPos(hWnd,NULL,wrect.left,0,0,0,SWP_NOSIZE|SWP_NOZORDER);


}

LayerWindow::~LayerWindow()
{ if (hfont != 0 && gethdc() != 0)
    {  SelectObject(gethdc(),holdfont);
       DeleteObject(hfont);
    }
  cadwindow->setlayerwindow(0);
}

void LayerWindow::mousemoveevent(int x,int y,int b)
{ Window::mousemoveevent(x,y,b);
}

void LayerWindow::lbuttonupevent(int,int,int)
{
}

void LayerWindow::paintevent(void)
{RECT rect;
 long i,j,nlayers;
 int r;
 RCHBRUSH hbrush,holdbrush;
 RCHPEN holdpen;
 ResourceString rs31(NCWIN+31);

  GetClientRect(gethwnd(),&rect);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));

  nlayers = 2;
  for (i = 0 ; i < MAX_LAYERS ; i++)
    nlayers += strlen(db.layernames.getlayername(i,1)) > 0;

  SetTextColor(gethdc(),RGB(0,0,0));
  SetBkColor(gethdc(),RGB(255,255,255));
  TextOut(gethdc(),0,1-ybase*cheight,rs31.getws(),strlen(rs31.gets()));

  TextOut(gethdc(),cwidth,cheight-ybase*cheight,_RCT("S"),strlen("S"));
  TextOut(gethdc(),cwidth*5,cheight-ybase*cheight,_RCT("D"),strlen("D"));

  if (selectall)
    hbrush = CreateSolidBrush(RGB(0,0,255));
  else
    hbrush = CreateSolidBrush(RGB(255,255,255));
  rect.top = (1-ybase)*cheight + cheight / 4;
  rect.bottom = (2-ybase)*cheight - cheight / 4;
  rect.left = cwidth * 2 + cheight / 2;
  rect.right = cwidth * 2 + cheight;
  FillRect(gethdc(),&rect,hbrush);
  FrameRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  DeleteObject(hbrush);
  if (displayall)
    hbrush = CreateSolidBrush(RGB(255,0,0));
  else
    hbrush = CreateSolidBrush(RGB(255,255,255));
  rect.top = (1-ybase)*cheight + cheight / 4;
  rect.bottom = (2-ybase)*cheight - cheight / 4;
  rect.left = cwidth * 6 + cheight / 4;
  rect.right = cwidth * 6 + cheight * 3 / 4;
  FillRect(gethdc(),&rect,hbrush);
  FrameRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  DeleteObject(hbrush);

  holdpen = (RCHPEN)SelectObject(gethdc(),GetStockObject(BLACK_PEN));
  if (nlayers > 2)
    {  MoveToEx(gethdc(),cwidth*2,2 - ybase * cheight + cheight*2-3,0);
       LineTo(gethdc(),cwidth*2,1 + (nlayers - ybase) * cheight - cheight / 2);
    }
  MoveToEx(gethdc(),0,2 - ybase * cheight + cheight*2-3,0);
  LineTo(gethdc(),(int)width,2 - ybase * cheight + cheight*2-3);
  SelectObject(gethdc(),holdpen);

  for (i = 0,j = 0 ; i < MAX_LAYERS ; i++)
    if (strlen(db.layernames.getlayername(i,1)) > 0)
      {
#ifdef _USING_QTCHAR
            TextOutW(gethdc(),cwidth * 2 + cheight * 2,int((j+2-ybase)*cheight), db.layernames.getlayername(i,1),strlen(db.layernames.getlayername(i,1)));
#else
            TextOut(gethdc(),cwidth * 2 + cheight * 2,int((j+2-ybase)*cheight),db.layernames.getlayername(i),strlen(db.layernames.getlayername(i)));
#endif
         holdpen = (RCHPEN)SelectObject(gethdc(),GetStockObject(BLACK_PEN));
         if (db.header.getlayer() == i)
           hbrush = CreateSolidBrush(RGB(0,255,0));
         else
           hbrush = CreateSolidBrush(RGB(255,255,255));
         holdbrush = (RCHBRUSH) SelectObject(gethdc(),hbrush);

         r = cwidth / 2;
         Ellipse(gethdc(),cwidth-r,1 + (j + 2 - ybase) * cheight + cheight / 2-r,
                          cwidth+r,1 + (j + 2 - ybase) * cheight + cheight / 2+r);
         MoveToEx(gethdc(),cwidth*2,1 + (j + 2 - ybase) * cheight + cheight / 2,0);
         LineTo(gethdc(),cwidth*2+cheight/2,1 + (j + 2 - ybase) * cheight + cheight / 2);
         SelectObject(gethdc(),holdpen);
         SelectObject(gethdc(),holdbrush);
         DeleteObject(hbrush);

         if (state.selmask.layer.test(i))
           hbrush = CreateSolidBrush(RGB(0,0,255));
         else
           hbrush = CreateSolidBrush(RGB(255,255,255));
         rect.top = (j+2-ybase)*cheight + cheight / 4;
         rect.bottom = (j+3-ybase)*cheight - cheight / 4;
         rect.left = cwidth * 2 + cheight / 2;
         rect.right = cwidth * 2 + cheight;
         FillRect(gethdc(),&rect,hbrush);
         FrameRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
         DeleteObject(hbrush);
         if (db.dismask.layer.test(i))
           hbrush = CreateSolidBrush(RGB(255,0,0));
         else
           hbrush = CreateSolidBrush(RGB(255,255,255));
         rect.top = (j+2-ybase)*cheight + cheight / 4;
         rect.bottom = (j+3-ybase)*cheight - cheight / 4;
         rect.left = cwidth * 2 + cheight + cheight / 4;
         rect.right = cwidth * 2 + cheight + cheight * 3 / 4;
         FillRect(gethdc(),&rect,hbrush);
         FrameRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
         DeleteObject(hbrush);
         j++;
      }
}

void LayerWindow::vscrollevent(int code,int pos)
{int i,start,end,nlayers;
  nlayers = 2;
  for (i = 0 ; i < MAX_LAYERS ; i++)
    nlayers += strlen(db.layernames.getlayername(i,1)) > 0;
  GetScrollRange(gethwnd(),SB_VERT,&start,&end);
  switch (code)
    {case SB_BOTTOM : ybase = long(nlayers - height / cheight);  break;
     case SB_LINEDOWN : ybase++;  break;
     case SB_LINEUP : ybase--;  break;
     case SB_PAGEDOWN : ybase += long(height / cheight - 1);  break;
     case SB_PAGEUP : ybase -= long(height / cheight - 1);  break;
     case SB_THUMBTRACK	:
     case SB_THUMBPOSITION	:  ybase = long(double(pos - start) / double(end - start) * (nlayers - 1));  break;
     case SB_TOP : ybase = 0;  break;
    }
  if (ybase <= 0)
    ybase = 0;
  if (ybase > nlayers - 1)
    ybase = nlayers - 1;
  if (code != SB_THUMBTRACK)
    {  if (nlayers - 1 > 0)
         SetScrollPos(gethwnd(),SB_VERT,long(start + double(end - start) * ybase / (nlayers - 1)),TRUE);
       else
         SetScrollPos(gethwnd(),SB_VERT,0,TRUE);
    }
  if (code < 1000)
    paintevent();
}

void LayerWindow::lbuttondownevent(int x,int y,int)
{int i,j,layer;
 RCCHAR layername[300];
 ResourceString rs30(NCWIN+30),rs31(NCWIN+31),rs32(NCWIN+32),rs33(NCWIN+33),rs34(NCWIN+34),rs35(NCWIN+35);
  strcpy(layername,"");
  if (x < cwidth * 2 && y > cheight * 2 - ybase * cheight)
    {  layer = y / cheight  + ybase - 2;
       for (i = 0,j = 0 ; i < MAX_LAYERS ; i++)
         {  j += strlen(db.layernames.getlayername(i,1)) > 0;
            if (j == layer + 1)
              {  db.header.change(db.header.getcolour(),i,db.header.getstyle(),db.header.getweight());
                 db.dismask.layer.set(db.header.getlayer());
                 v.setinteger("df::layer",i);
                 cadwindow->updatetitle();
                 paintevent();
                 break;
              }
         }
    }
  else if (x < cwidth * 2 + cheight && y > cheight * 2 - ybase * cheight)
    {  layer = y / cheight  + ybase - 2;
       for (i = 0,j = 0 ; i < MAX_LAYERS ; i++)
         {  j += strlen(db.layernames.getlayername(i,1)) > 0;
            if (j == layer + 1)
              {  state.selmask.layer.toggle(i);
                 paintevent();
                 break;
              }
         }
    }
  else if (x < cwidth * 2 + cheight * 2 && y > cheight * 2 - ybase * cheight)
    {  layer = y / cheight  + ybase - 2;
       for (i = 0,j = 0 ; i < MAX_LAYERS ; i++)
         {  j += strlen(db.layernames.getlayername(i,1)) > 0;
            if (j == layer + 1)
              {  db.dismask.layer.toggle(i);
                 db.dismask.layer.set(db.header.getlayer());
                 paintevent();
                 break;
              }
         }
    }
  else if (y < cheight && ybase == 0)
    {Dialog dialog(rs31.gets(),rs33.gets(),layername,300);
       if (dialog.process() == TRUE)
         {  while (strlen(layername) > 0 && layername[strlen(layername)-1] == ' ')
              layername[strlen(layername)-1] = 0;
            for (i = 0 ; i < MAX_LAYERS ; i++)
              if (_stricmp(layername,db.layernames.getlayername(i,1)) == 0)
                break;
            if (strlen(layername) == 0)
              cadwindow->MessageBox(rs32.gets(),rs31.gets(),MB_ICONINFORMATION);
            else if (i < MAX_LAYERS)
              cadwindow->MessageBox(rs30.gets(),rs31.gets(),MB_ICONINFORMATION);
            else
              {  for (i = 0 ; i < MAX_LAYERS ; i++)
                   if (strlen(db.layernames.getlayername(i,1)) == 0)
                     break;
                 if (i < MAX_LAYERS)
                   {  db.layernames.setlayername(i,layername);
                      db.header.change(db.header.getcolour(),i,db.header.getstyle(),db.header.getweight());
                      v.setinteger("df::layer",i);
                      cadwindow->updatetitle();
                      paintevent();
                   }
              }
         }
    }
  else if (y > (ybase + 1) * cheight && y < (ybase+2) * cheight)
    {  if (x > cwidth * 2 && x < cwidth * 2 + cheight)
         {  selectall = ! selectall;
            for (i = 0 ; i < MAX_LAYERS ; i++)
              if (selectall)
                state.selmask.layer.set(i);
              else
                state.selmask.layer.clear(i);
            paintevent();
         }
       else if (x > cwidth * 6 && x < cwidth * 6 + cheight)
         {  displayall = ! displayall;
            for (i = 0 ; i < MAX_LAYERS ; i++)
              if (displayall)
                db.dismask.layer.set(i);
              else
                db.dismask.layer.clear(i);
            db.dismask.layer.set(db.header.getlayer());
            paintevent();
         }
    }
  else if (x > cwidth * 2 + cheight * 2 && y > cheight - ybase * cheight)
    {  layer = y / cheight  + ybase - 2;
       for (i = 0,j = 0 ; i < MAX_LAYERS ; i++)
         {  j += strlen(db.layernames.getlayername(i,1)) > 0;
            if (j == layer + 1)
              break;
         }
       if (i < MAX_LAYERS)
         {Dialog dialog(rs34.gets(),rs35.gets(),layername,300);
           strcpy(layername,db.layernames.getlayername(i,1));
           if (dialog.process() == TRUE)
             {  while (strlen(layername) > 0 && layername[strlen(layername)-1] == ' ')
                  layername[strlen(layername)-1] = 0;
                for (j = 0 ; j < MAX_LAYERS ; j++)
                  if (strlen(layername) > 0 && j != i && _stricmp(layername,db.layernames.getlayername(j,1)) == 0)
                    break;
                if (j < MAX_LAYERS)
                  cadwindow->MessageBox(rs30.gets(),rs34.gets(),MB_ICONINFORMATION);
                else
                  {  db.layernames.setlayername(i,layername);
                     db.header.change(db.header.getcolour(),i,db.header.getstyle(),db.header.getweight());
                     v.setinteger("df::layer",i);
                     cadwindow->updatetitle();
                     paintevent();
                  }
              }
         }
    }
}

void LayerWindow::paintline(int)
{
}

void LayerWindow::lbuttondblclkevent(int,int,int)
{POINT p;
 RECT rect;
  GetWindowRect(gethwnd(),&rect);
  if (GetParent(gethwnd()) == cadwindow->gethwnd())
    {//  Change to floating
       SetParent(gethwnd(),cadwindow->subwindow->gethwnd());
       p.x = 0;  p.y = 0;
       ClientToScreen(cadwindow->subwindow->gethwnd(),&p);
       ScreenToClient(cadwindow->gethwnd(),&p);
       SetWindowPos(cadwindow->subwindow->gethwnd(),0,0,(int)p.y,int(cadwindow->subwindow->getwidth() + p.x),(int)cadwindow->subwindow->getheight(),SWP_NOZORDER);
       SetWindowLong(gethwnd(),GWL_STYLE,(DWORD)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VSCROLL | WS_VISIBLE | WS_CAPTION));
       SetWindowPos(gethwnd(),0,oldx,oldy,oldw,oldh,SWP_NOZORDER);
    }
  else
    {//  Change to fixed
       p.x = rect.left;  p.y = rect.top;
       ScreenToClient(cadwindow->subwindow->gethwnd(),&p);
       rect.left = p.x;  rect.top = p.y;
       p.x = rect.right;  p.y = rect.bottom;
       ScreenToClient(cadwindow->subwindow->gethwnd(),&p);
       rect.right = p.x;  rect.bottom = p.y;
       oldx = rect.left;  oldy = rect.top;
       oldw = rect.right - rect.left;  oldh = rect.bottom - rect.top;
       SetParent(gethwnd(),cadwindow->gethwnd());
       p.x = 0;  p.y = 0;
       ClientToScreen(cadwindow->subwindow->gethwnd(),&p);
       ScreenToClient(cadwindow->gethwnd(),&p);
       SetWindowPos(cadwindow->subwindow->gethwnd(),0,rect.right-rect.left,(int)p.y,int(cadwindow->subwindow->getwidth() - getwidth()),int(cadwindow->subwindow->getheight()),SWP_NOZORDER);
       SetWindowLong(gethwnd(),GWL_STYLE,(DWORD)(WS_CHILD | WS_DLGFRAME | WS_VSCROLL | WS_VISIBLE));
       SetWindowPos(gethwnd(),0,0,(int)p.y,rect.right-rect.left,(int)(cadwindow->subwindow->getheight()),SWP_NOZORDER);
    }
}

void CommandWindow::insert(char *l)
{long y;
  y = y1;
  while (x1 > (long)strlen(line[y]))
    strcat(line[y]," ");
  strcat(line[y],l);
  strcat(line[y],":");
  x1 = x2 = 0;
  if (y1 < maxlines - 1)
    y1++;
  else
    {  for (y = maxlines-1 ; y > 1 ; y--)
         strcpy(line[y],line[y-1]);
       strcpy(line[maxlines-1],"");
    }
  y2 = y1;
  for (y = maxlines-1 ; y > y1 ; y--)
    strcpy(line[y],line[y-1]);
  strcpy(line[y1],"");
  if (y1 >= ybase + height / cheight - 2)
    {  ybase = long(y1 - height / cheight + 2);
       if (ybase < 0) ybase = 0;
       if (ybase >= maxlines)
         {  ybase = maxlines - 1;
            if (ybase < 0)
              ybase = 0;
         }
    }
  paintevent();
}

