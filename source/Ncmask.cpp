
#include "ncwin.h"
#include "Ncdialog.h"
#include "weight_dialog.h"
#include "maskchange_dialog.h"
#include "Styleset_Dialog.h"
#include "definestyle_dialog.h"
#include "definecolour_dialog.h"
#include "definepatternstyle_dialog.h"
#include "RCView.h"
#include "RCGraphicsView.h"

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

void mask_command0(int *cstate,HighLevelEvent *,void **)
{
  //cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("SelectionMask")));
  cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("SelectionMask")),buttonmenus.match("Interrupt"));
  *cstate = ExitState;
}

void mask_command100(int *cstate,HighLevelEvent *,void **)
{
  cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("SelectionMask")),buttonmenus.match("Interrupt"));
  *cstate = ExitState;
}

void mask_command1(int *cstate,HighLevelEvent *,void **)
{ state.setupentitymask();
  *cstate = ExitState;
}

// KMJ : new version of mask_command1
void mask_command101(int *cstate,HighLevelEvent *,void **)
{
  state.setupentitymaskEx();
  *cstate = ExitState;
}

class ColourMaskDialogControl : public CustomDialogControl
  {public:
     int scrollid;
     int width,height,base,nacross,ndown,n,lastselected,startselection,direction,scroll;
     BitMask *colours;
     ColourMaskDialogControl(int id1,int id2,int n1,int n2,int n3,BitMask *c) :
       CustomDialogControl(id1)
         {  scrollid = id2;
            width = height = base = scroll = 0;  lastselected = -1;  startselection = -1;  colours = c;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void set(Dialog *,int);
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void lbuttonupevent(Dialog *,int,int,int);
     void mousemoveevent(Dialog *,int,int,int);
     void timerevent(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void ColourMaskDialogControl::set(Dialog *dialog,int value)
{ if (value)
    colours->setall();
  else
    colours->clearall();
  draw(dialog,NULL);
}

void ColourMaskDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RCHBRUSH hbrush;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  hbrush = CreateSolidBrush(RGB(db.colourtable[box].red,db.colourtable[box].green,db.colourtable[box].blue));
  FillRect(hDC,&rect,hbrush);
  DeleteObject(hbrush);
  sprintf(text,"%d",box);
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = w / 2 - size.cx / 2;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = w / 2 - LOWORD(te) / 2;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  if (db.colourtable[box].red + db.colourtable[box].green + db.colourtable[box].blue < 50)
    SetTextColor(hDC,RGB(255,255,255));
  else
    SetTextColor(hDC,RGB(0,0,0));
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  rect.left = int(x + 1);  rect.top = int(y + 1);
  rect.right = int(w * (int((box - base) % nacross) + 1));
  rect.bottom = int(h * (int((box - base) / nacross) + 1));
  if (drawmode == DM_NORMAL)
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(GRAY_BRUSH));
  else
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  rect.left++;  rect.right--;  rect.top++;  rect.bottom--;
  if (drawmode == DM_NORMAL)
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(GRAY_BRUSH));
  else
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void ColourMaskDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,k,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN)SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      {  k = base + i * nacross + j;
         draw(dialog,hDC,k,(colours->test(k) ? DM_SELECT : DM_NORMAL));
      }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void ColourMaskDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{int i,i1,i2;

  SetCapture(hWnd);
  lastselected = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));

  if ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && startselection >= 0)
    {  if (startselection < lastselected)
         {  i1 = startselection;  i2 = lastselected;
         }
       else
         {  i1 = lastselected;  i2 = startselection;  
         }
       for (i = i1 ; i <= i2 ; i++)
         {  colours->set(i,colours->test(startselection));
            draw(dialog,NULL,i,(colours->test(i) ?  DM_SELECT : DM_NORMAL));
         }
    }
  else
    {  startselection = lastselected;
       colours->toggle(lastselected);
       draw(dialog,NULL,lastselected,(colours->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }

  direction = -1;
}

void ColourMaskDialogControl::mousemoveevent(Dialog *dialog,int x,int y,int buttons)
{int i,k,d,cx,cy;
  if (lastselected < 0) return;
  if (! (buttons & MK_LBUTTON))
    {  lastselected = -1;  return;
    }
  cx = int(double(x)/double(width)*double(nacross));
  cy = int(double(y)/double(height)*double(ndown));
  if (y < 0) cy--;
  if (cx < 0) cx = 0; else if (cx >= nacross) cx = nacross - 1;
  if (cy < 0)
    {  cy = 0;
       if (base > 0)
         {  scroll = -nacross;
            vscrollevent(dialog,0,SB_LINEUP,0);
            cy = 0;
         }
       else
         scroll = 0;
    }
  else if (cy >= ndown)
    {  cy = ndown - 1;
       if (base < n - nacross*ndown)
         {  scroll = nacross;
            vscrollevent(dialog,0,SB_LINEDOWN,0);
            cy = ndown - 1;
         }
       else
         scroll = 0;
    }
  else
    scroll = 0; 
  k = base + cy * nacross + cx;
  if (k == lastselected) return;
  d = k > lastselected;
  for (i = k ; i != lastselected ; (d ? i-- : i++))
    {  colours->toggle(i);
       draw(dialog,NULL,i,(colours->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  if (direction != -1 && d != direction)
    {  colours->toggle(lastselected);
       draw(dialog,NULL,lastselected,(colours->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
  direction = d;
}

void ColourMaskDialogControl::timerevent(Dialog *dialog)
{int i,k;
  if (scroll == 0)
    return;
  else if (scroll < 0)
    {  if (base > 0)
         {  vscrollevent(dialog,0,SB_LINEUP,0);
            k = lastselected + scroll;
            if (k < 0) k = 0;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  else
    {  if (base < n - nacross * ndown)
         {  vscrollevent(dialog,0,SB_LINEDOWN,0);
            k = lastselected + scroll;
            if (k >= n) k = n-1;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  for (i = k ; i != lastselected ; (i > lastselected ? i-- : i++))
    {  colours->toggle(i);
       draw(dialog,NULL,i,(colours->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
}

void ColourMaskDialogControl::lbuttonupevent(Dialog *,int,int,int)
{ ReleaseCapture();
  lastselected = -1;
}

void ColourMaskDialogControl::changefocus(Dialog *,int)
{ lastselected = -1;
}

void ColourMaskDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class ColourMaskButtonDialogControl : public ButtonDialogControl
  {public:
     ColourMaskButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int ColourMaskButtonDialogControl::select(Dialog *dialog)
{ColourMaskDialogControl *cdc;
  if ((cdc = (ColourMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
  if (id == 101)
    cdc->set(dialog,255);
  else
    cdc->set(dialog,0);
  return 0;
}

void mask_command2(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Colourmask_dialog");
 DialogControl *dc;
 BitMask colours(256);
 ResourceString rs26(NCMAIN+26);
  colours = state.selmask.colour;
  dialog.title(rs26.gets());
  dialog.add((ColourMaskDialogControl *)(dc = new ColourMaskDialogControl(100,1100,256,10,8,&colours)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new ColourMaskButtonDialogControl(101));
  dialog.add(new ColourMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.colour = colours;
  *cstate = ExitState;
}

void mask_command102(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Colourmask_dialog");
 DialogControl *dc;
 BitMask colours(256);
 ResourceString rs26(NCMAIN+26);
  colours = state.selmask.colour;
  dialog.title(rs26.gets());
  dialog.add((ColourMaskDialogControl *)(dc = new ColourMaskDialogControl(100,1100,256,10,8,&colours)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new ColourMaskButtonDialogControl(101));
  dialog.add(new ColourMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.colour = colours;
  *cstate = ExitState;
}

class LayerMaskDialogControl : public CustomDialogControl
  {public:
     int scrollid;
     int width,height,base,nacross,ndown,n,lastselected,direction,scroll,startselection;
     BitMask *layers;
     LayerMaskDialogControl(int id1,int id2,int n1,int n2,int n3,BitMask *l) :
       CustomDialogControl(id1)
         {  scrollid = id2;
            width = height = base = scroll = 0;  lastselected = -1;  layers = l;  startselection = -1;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void set(Dialog *,int);
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void lbuttonupevent(Dialog *,int,int,int);
     void mousemoveevent(Dialog *,int,int,int);
     void timerevent(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
     void setbase(Dialog *,int);

  };


void LayerMaskDialogControl::set(Dialog *dialog,int value)
{ if (value)
    layers->setall();
  else
    layers->clearall();
  draw(dialog,NULL);
}


void FillRect(RCHDC hDC,int left,int top,int right,int bottom,RCHBRUSH hbrush)
{RECT rect;

  rect.left = left;
  rect.right = right;
  rect.top = top;
  rect.bottom = bottom;

  FillRect(hDC,&rect,hbrush);
  
}


void LayerMaskDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RECT rect;
 RCHFONT hfont,holdfont;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  
  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       SetTextColor(hDC,RGB(0,0,0));
    }
  else
    {RCHBRUSH hbrush;                              
       hbrush = CreateSolidBrush(RGB(0,0,128));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
       SetTextColor(hDC,RGB(255,255,255));
    }

  rect.left -= 2;
  rect.top -= 2;
  rect.right += 2;
  rect.bottom += 2;

  FillRect(hDC,rect.left,rect.top,rect.right,rect.top+1,GetSysColorBrush(COLOR_3DHILIGHT));
  FillRect(hDC,rect.left,rect.top,rect.left+1,rect.bottom,GetSysColorBrush(COLOR_3DHILIGHT));

  FillRect(hDC,rect.left+1,rect.top+1,rect.right,rect.top+2,GetSysColorBrush(COLOR_3DLIGHT));
  FillRect(hDC,rect.left+1,rect.top+2,rect.left+2,rect.bottom-1,GetSysColorBrush(COLOR_3DLIGHT));

  FillRect(hDC,rect.left,rect.bottom-1,rect.right,rect.bottom,GetSysColorBrush(COLOR_3DDKSHADOW));
  FillRect(hDC,rect.right-1,rect.top,rect.right,rect.bottom,GetSysColorBrush(COLOR_3DDKSHADOW));

  FillRect(hDC,rect.left+1,rect.bottom-2,rect.right-1,rect.bottom-1,GetSysColorBrush(COLOR_3DSHADOW));
  FillRect(hDC,rect.right-2,rect.top+1,rect.right-1,rect.bottom-1,GetSysColorBrush(COLOR_3DSHADOW));


  FillRect(hDC,rect.left,rect.top,rect.left+1,rect.top+1,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  FillRect(hDC,rect.left,rect.bottom-1,rect.left+1,rect.bottom,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  FillRect(hDC,rect.right-1,rect.top,rect.right,rect.top+1,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  FillRect(hDC,rect.right-1,rect.bottom-1,rect.right,rect.bottom,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
       


  strcpy(text,db.layernames.getlayername(box));
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = w / 2 - size.cx / 2;
  GetTextExtentPoint(hDC,_RCT("0"),1,&size);
  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = w / 2 - LOWORD(te) / 2;
  te = GetTextExtent(hDC,_RCT("0"),1);
  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  sprintf(text,"%d",box);
  TextOut(hDC,int(5+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void LayerMaskDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,k,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = (RCHPEN) CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      {  k = base + i * nacross + j;
         draw(dialog,hDC,k,(layers->test(k) ?  DM_SELECT : DM_NORMAL));
      }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void LayerMaskDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{int i,i1,i2;

  SetCapture(hWnd);
  lastselected = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));

  i1 = lastselected;
  if ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && startselection >= 0)
    {  if (startselection < lastselected)
         {  i1 = startselection;  i2 = lastselected;
         }
       else
         {  i1 = lastselected;  i2 = startselection;  
         }
       for (i = i1 ; i <= i2 ; i++)
         {  layers->set(i,layers->test(startselection));
            draw(dialog,NULL,i,(layers->test(i) ? DM_SELECT : DM_NORMAL));
         }
    }
  else
    {  startselection = lastselected;
       layers->toggle(lastselected);
       draw(dialog,NULL,lastselected,(layers->test(lastselected) ? DM_SELECT : DM_NORMAL));
    }

  direction = -1;
}

void LayerMaskDialogControl::mousemoveevent(Dialog *dialog,int x,int y,int buttons)
{int i,k,d,cx,cy;
  if (lastselected < 0) return;
  if (! (buttons & MK_LBUTTON))
    {  lastselected = -1;  return;
    }
  cx = int(double(x)/double(width)*double(nacross));
  cy = int(double(y)/double(height)*double(ndown));
  if (y < 0) cy--;
  if (cx < 0) cx = 0; else if (cx >= nacross) cx = nacross - 1;
  if (cy < 0)
    {  cy = 0;
       if (base > 0)
         {  scroll = -nacross;
            vscrollevent(dialog,0,SB_LINEUP,0);
            cy = 0;
         }
       else
         scroll = 0;
    }
  else if (cy >= ndown)
    {  cy = ndown - 1;
       if (base < n - nacross*ndown)
         {  scroll = nacross;
            vscrollevent(dialog,0,SB_LINEDOWN,0);
            cy = ndown - 1;
         }
       else
         scroll = 0;
    }
  else
    scroll = 0; 
  k = base + cy * nacross + cx;
  if (k == lastselected) return;
  d = k > lastselected;
  for (i = k ; i != lastselected ; (d ? i-- : i++))
    {  layers->toggle(i);
       draw(dialog,NULL,i,(layers->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  if (direction != -1 && d != direction)
    {  layers->toggle(lastselected);
       draw(dialog,NULL,lastselected,(layers->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
  direction = d;
}

void LayerMaskDialogControl::timerevent(Dialog *dialog)
{int i,k;
  if (scroll == 0)
    return;
  else if (scroll < 0)
    {  if (base > 0)
         {  vscrollevent(dialog,0,SB_LINEUP,0);
            k = lastselected + scroll;
            if (k < 0) k = 0;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  else
    {  if (base < n - nacross * ndown)
         {  vscrollevent(dialog,0,SB_LINEDOWN,0);
            k = lastselected + scroll;
            if (k >= n) k = n-1;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  for (i = k ; i != lastselected ; (i > lastselected ? i-- : i++))
    {  layers->toggle(i);
       draw(dialog,NULL,i,(layers->test(i) ? DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
}

void LayerMaskDialogControl::lbuttonupevent(Dialog *,int,int,int)
{ ReleaseCapture();
  lastselected = -1;
}

void LayerMaskDialogControl::changefocus(Dialog *,int)
{ lastselected = -1;
}

void LayerMaskDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

void LayerMaskDialogControl::setbase(Dialog *dialog,int b)
{int start,end,pos;
  base = b;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class LMaskListDialogControl : public ListDialogControl
  {private:
     char name[300];
   public:
     LMaskListDialogControl(int id) : ListDialogControl(id,0,0,CHAR2RCCHR(name),300)  {  name[0] = 0;  };
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };


void LMaskListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int i;
 RCCHAR key[300],layergroupname[300],value[600],*s;
 LayerMaskDialogControl *ldc;

  if ((ldc = (LayerMaskDialogControl *)dialog->getcontrol(118)) == NULL) return;

  GetDlgItemText(dialog->gethdlg(),115,layergroupname,300);

  for (i = 1 ; ; i++)
    {  sprintf(key,"LayerGroupName%d",i);
       GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
       if (strlen(value) == 0)
         break;
       s = strtok(value,_RCT("\t"));
       if (s != 0 && _stricmp(s,layergroupname) == 0) 
         {  s = strtok(0,_RCT("\t"));
            if (s != 0 && _stricmp(s,"Deleted") != 0)
              {  ldc->layers->loadascii((char*)s->toLatin1());
                 InvalidateRect(GetDlgItem(dialog->gethdlg(),118),0,TRUE);
              } 
            break;
         }
    }
  
  DialogControl::changefocus(dialog,updatescreen);

}

void LMaskListDialogControl::load(Dialog *dialog)
{RCCHAR key[300],value[300],*s,name[300];
 int i;

  removeallitems(dialog);
  for (i = 1 ; ; i++)
    {  sprintf(key,"LayerGroupName%d",i);
#if ! defined(_WIN32_WCE)
       GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
#endif
       if (strlen(value) == 0)
         break;
       s = strtok(value,_RCT("\t"));
       if (s != 0) 
         {  strcpy(name,s);
            s = strtok(0,_RCT("\t"));
            if (s != 0 && _stricmp(s,"deleted") != 0)
              additem(dialog,name);
         }
    }

}

class LayerMaskButtonDialogControl : public ButtonDialogControl
  {public:
     LayerMaskButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int LayerMaskButtonDialogControl::select(Dialog *dialog)
{LayerMaskDialogControl *ldc,*ldc1,*ldc2;
 LMaskListDialogControl *lmldc;
 int i,changed;
 RCCHAR layergroupname[300],value[600],text[600],key[300],*s;


  if (id >= 105 && id <= 114)
    {  ((LayerMaskDialogControl *)dialog->getcontrol(100))->setbase(dialog,(id - 105)*100);
    }
  else if (id >= 121 && id <= 130)
    {  ((LayerMaskDialogControl *)dialog->getcontrol(118))->setbase(dialog,(id - 121)*100);
    }
  else if (id == 101 || id == 102 || id == 119 || id == 120)
    {  if (id == 101 || id == 102)
         {  if ((ldc = (LayerMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
         }
       else
         {  if ((ldc = (LayerMaskDialogControl *)dialog->getcontrol(118)) == NULL) return 0;
         }
       if (id == 101 || id == 119)
         ldc->set(dialog,255);
       else
         ldc->set(dialog,0);
    }
  else if (id == 131 || id == 132)
    {  

       if ((ldc1 = (LayerMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
       if ((ldc2 = (LayerMaskDialogControl *)dialog->getcontrol(118)) == NULL) return 0;

       changed = 0;
       for (i = 0 ; i < MAX_LAYERS ; i++)
         if (ldc2->layers->test(i))
           {  ldc1->layers->set(i,id == 131);
              changed = 1;
           }

       if (changed)
         InvalidateRect(GetDlgItem(dialog->gethdlg(),100),0,TRUE);

    }
  else if (id == 116)
    {  

       if ((ldc = (LayerMaskDialogControl *)dialog->getcontrol(118)) == NULL) return 0;
       if ((lmldc = (LMaskListDialogControl *)dialog->getcontrol(115)) == NULL) return 0;
       lmldc->currentvalue(dialog,layergroupname,300);

       if (strlen(layergroupname) > 0) 
         {
            for (i = 1 ; ; i++)
              {  sprintf(key,"LayerGroupName%d",i);
                 GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
                 if (strlen(value) == 0)
                   break;
                 s = strtok(value,_RCT("\t"));
                 if (s != 0 && _stricmp(s,layergroupname) == 0) 
                   {  s = strtok(0,_RCT("\t"));
                      if (_stricmp(s,"Deleted") == 0)
                        lmldc->additem(dialog,layergroupname);
                      sprintf(value,"%s\t%s",layergroupname,ldc->layers->saveascii((char*)RCPRINTABLE(text)));
                      WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
                      break;
                   }
              }

            if (strlen(value) == 0)
              {  sprintf(value,"%s\t%s",layergroupname,ldc->layers->saveascii((char*)RCPRINTABLE(text)));
                 WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
                 lmldc->additem(dialog,layergroupname);
              }

         }


    }
  else if (id == 117)
    {  

       if ((ldc = (LayerMaskDialogControl *)dialog->getcontrol(118)) == NULL) return 0;
       if ((lmldc = (LMaskListDialogControl *)dialog->getcontrol(115)) == NULL) return 0;
       lmldc->currentvalue(dialog,layergroupname,300);

       if (strlen(layergroupname) > 0) 
         {
            for (i = 1 ; ; i++)
              {  sprintf(key,"LayerGroupName%d",i);
                 GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
                 if (strlen(value) == 0)
                   break;
                 s = strtok(value,_RCT("\t"));
                 if (s != 0 && _stricmp(s,layergroupname) == 0) 
                   {  sprintf(value,"%s\tDeleted",layergroupname);
                      WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
                      lmldc->removeitem(dialog,layergroupname);
                      lmldc->change(dialog,_RCT(""));
                      break;
                   }
              }

         }


    }


  return 0;
}

void mask_command3(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Layermask_dialog");
 DialogControl *dc;
 BitMask layers(MAX_LAYERS);
 BitMask grouplayers(MAX_LAYERS);
 ResourceString rs27(NCMAIN+27);
  layers = state.selmask.layer;
  dialog.title(rs27.gets());
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(100,1100,MAX_LAYERS,1,8,&layers)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new LayerMaskButtonDialogControl(101));
  dialog.add(new LayerMaskButtonDialogControl(102));
  dialog.add(new LayerMaskButtonDialogControl(105));
  dialog.add(new LayerMaskButtonDialogControl(106));
  dialog.add(new LayerMaskButtonDialogControl(107));
  dialog.add(new LayerMaskButtonDialogControl(108));
  dialog.add(new LayerMaskButtonDialogControl(109));
  dialog.add(new LayerMaskButtonDialogControl(110));
  dialog.add(new LayerMaskButtonDialogControl(111));
  dialog.add(new LayerMaskButtonDialogControl(112));
  dialog.add(new LayerMaskButtonDialogControl(113));
  dialog.add(new LayerMaskButtonDialogControl(114));

  dialog.add(new LMaskListDialogControl(115));
  dialog.add(new LayerMaskButtonDialogControl(116));
  dialog.add(new LayerMaskButtonDialogControl(117));
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(118,1180,MAX_LAYERS,1,8,&grouplayers)));
  dialog.add(new ScrollBarDialogControl(1180,dc));
  dialog.add(new LayerMaskButtonDialogControl(119));
  dialog.add(new LayerMaskButtonDialogControl(120));
  dialog.add(new LayerMaskButtonDialogControl(121));
  dialog.add(new LayerMaskButtonDialogControl(122));
  dialog.add(new LayerMaskButtonDialogControl(123));
  dialog.add(new LayerMaskButtonDialogControl(124));
  dialog.add(new LayerMaskButtonDialogControl(125));
  dialog.add(new LayerMaskButtonDialogControl(126));
  dialog.add(new LayerMaskButtonDialogControl(127));
  dialog.add(new LayerMaskButtonDialogControl(128));
  dialog.add(new LayerMaskButtonDialogControl(129));
  dialog.add(new LayerMaskButtonDialogControl(130));


  dialog.add(new LayerMaskButtonDialogControl(131));
  dialog.add(new LayerMaskButtonDialogControl(132));


  if (dialog.process() == TRUE)
    {  state.selmask.layer = layers;
       if (cadwindow->getlayerwindow() != 0)
         cadwindow->getlayerwindow()->paintevent();
    }
  *cstate = ExitState;
}

void mask_command103(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Layermask_dialog");
 DialogControl *dc;
 BitMask layers(MAX_LAYERS);
 BitMask grouplayers(MAX_LAYERS);
 ResourceString rs27(NCMAIN+27);
  layers = state.selmask.layer;
  dialog.title(rs27.gets());
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(100,1100,MAX_LAYERS,1,8,&layers)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new LayerMaskButtonDialogControl(101));
  dialog.add(new LayerMaskButtonDialogControl(102));
  dialog.add(new LayerMaskButtonDialogControl(105));
  dialog.add(new LayerMaskButtonDialogControl(106));
  dialog.add(new LayerMaskButtonDialogControl(107));
  dialog.add(new LayerMaskButtonDialogControl(108));
  dialog.add(new LayerMaskButtonDialogControl(109));
  dialog.add(new LayerMaskButtonDialogControl(110));
  dialog.add(new LayerMaskButtonDialogControl(111));
  dialog.add(new LayerMaskButtonDialogControl(112));
  dialog.add(new LayerMaskButtonDialogControl(113));
  dialog.add(new LayerMaskButtonDialogControl(114));

  dialog.add(new LMaskListDialogControl(115));
  dialog.add(new LayerMaskButtonDialogControl(116));
  dialog.add(new LayerMaskButtonDialogControl(117));
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(118,1180,MAX_LAYERS,1,8,&grouplayers)));
  dialog.add(new ScrollBarDialogControl(1180,dc));
  dialog.add(new LayerMaskButtonDialogControl(119));
  dialog.add(new LayerMaskButtonDialogControl(120));
  dialog.add(new LayerMaskButtonDialogControl(121));
  dialog.add(new LayerMaskButtonDialogControl(122));
  dialog.add(new LayerMaskButtonDialogControl(123));
  dialog.add(new LayerMaskButtonDialogControl(124));
  dialog.add(new LayerMaskButtonDialogControl(125));
  dialog.add(new LayerMaskButtonDialogControl(126));
  dialog.add(new LayerMaskButtonDialogControl(127));
  dialog.add(new LayerMaskButtonDialogControl(128));
  dialog.add(new LayerMaskButtonDialogControl(129));
  dialog.add(new LayerMaskButtonDialogControl(130));


  dialog.add(new LayerMaskButtonDialogControl(131));
  dialog.add(new LayerMaskButtonDialogControl(132));


  if (dialog.process() == TRUE)
    {  state.selmask.layer = layers;
       if (cadwindow->getlayerwindow() != 0)
         cadwindow->getlayerwindow()->paintevent();
    }
  *cstate = ExitState;
}

class StyleMaskDialogControl : public CustomDialogControl
  {public:
     int scrollid;
     int width,height,base,nacross,ndown,n,lastselected,startselection,direction,scroll;
     BitMask *styles;
     StyleMaskDialogControl(int id1,int id2,int n1,int n2,int n3,BitMask *s) :
       CustomDialogControl(id1)
         {  scrollid = id2;
            width = height = base = scroll = 0;  lastselected = -1;  styles = s;  startselection = -1;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void set(Dialog *,int);
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void lbuttonupevent(Dialog *,int,int,int);
     void mousemoveevent(Dialog *,int,int,int);
     void timerevent(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void StyleMaskDialogControl::set(Dialog *dialog,int value)
{ if (value)
    styles->setall();
  else
    styles->clearall();
  draw(dialog,NULL);
} 

void StyleMaskDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RCHPEN hpen,holdpen;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
 LineStyle *linestyle;
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       hpen = CreatePen(PS_SOLID,db.lineweights.weighttohdc(hDC,db.header.getweight()),RGB(0,0,0));
       SetTextColor(hDC,RGB(0,0,0));
    }
  else
    {RCHBRUSH hbrush;
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
       hpen = CreatePen(PS_SOLID,db.lineweights.weighttohdc(hDC,db.header.getweight()),RGB(255,255,255));
       SetTextColor(hDC,RGB(255,255,255));
    }
  Surface surface;
  surface.sethdc(NULL,hDC);
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  if ((linestyle = db.linestyles.indextostyle(box)) == NULL)
    {  surface.moveto(50,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
       surface.lineto(rect.right - 10,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
    }
  else
    {  linestyle->start(1.0,rect.right - 60);
       linestyle->draw(&surface,50,(rect.top + rect.bottom) / 2,
                                 rect.right - 10,(rect.top + rect.bottom) / 2);
    }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  sprintf(text,"%d",box);
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = 5;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = 5;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void StyleMaskDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,k,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      {  k = base + i * nacross + j;
         draw(dialog,hDC,k,(styles->test(k) ?  DM_SELECT : DM_NORMAL));
      }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void StyleMaskDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{int i,i1,i2;

  SetCapture(hWnd);
  lastselected = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));


  if ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && startselection >= 0)
    {  if (startselection < lastselected)
         {  i1 = startselection;  i2 = lastselected;
         }
       else
         {  i1 = lastselected;  i2 = startselection;  
         }
       for (i = i1 ; i <= i2 ; i++)
         {  styles->set(i,styles->test(startselection));
            draw(dialog,NULL,i,(styles->test(i) ?  DM_SELECT : DM_NORMAL));
         }
    }
  else
    {  startselection = lastselected;
       styles->toggle(lastselected);
       draw(dialog,NULL,lastselected,(styles->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }

  direction = -1;
}

void StyleMaskDialogControl::mousemoveevent(Dialog *dialog,int x,int y,int buttons)
{int i,k,d,cx,cy;
  if (lastselected < 0) return;
  if (! (buttons & MK_LBUTTON))
    {  lastselected = -1;  return;
    }
  cx = int(double(x)/double(width)*double(nacross));
  cy = int(double(y)/double(height)*double(ndown));
  if (y < 0) cy--;
  if (cx < 0) cx = 0; else if (cx >= nacross) cx = nacross - 1;
  if (cy < 0)
    {  cy = 0;
       if (base > 0)
         {  scroll = -nacross;
            vscrollevent(dialog,0,SB_LINEUP,0);
            cy = 0;
         }
       else
         scroll = 0;
    }
  else if (cy >= ndown)
    {  cy = ndown - 1;
       if (base < n - nacross*ndown)
         {  scroll = nacross;
            vscrollevent(dialog,0,SB_LINEDOWN,0);
            cy = ndown - 1;
         }
       else
         scroll = 0;
    }
  else
    scroll = 0; 
  k = base + cy * nacross + cx;
  if (k == lastselected) return;
  d = k > lastselected;
  for (i = k ; i != lastselected ; (d ? i-- : i++))
    {  styles->toggle(i);
       draw(dialog,NULL,i,(styles->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  if (direction != -1 && d != direction)
    {  styles->toggle(lastselected);
       draw(dialog,NULL,lastselected,(styles->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
  direction = d;
}

void StyleMaskDialogControl::timerevent(Dialog *dialog)
{int i,k;
  if (scroll == 0)
    return;
  else if (scroll < 0)
    {  if (base > 0)
         {  vscrollevent(dialog,0,SB_LINEUP,0);
            k = lastselected + scroll;
            if (k < 0) k = 0;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  else
    {  if (base < n - nacross * ndown)
         {  vscrollevent(dialog,0,SB_LINEDOWN,0);
            k = lastselected + scroll;
            if (k >= n) k = n-1;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  for (i = k ; i != lastselected ; (i > lastselected ? i-- : i++))
    {  styles->toggle(i);
       draw(dialog,NULL,i,(styles->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
}

void StyleMaskDialogControl::lbuttonupevent(Dialog *,int,int,int)
{ ReleaseCapture();
  lastselected = -1;
}

void StyleMaskDialogControl::changefocus(Dialog *,int)
{ lastselected = -1;
}

void StyleMaskDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class StyleMaskButtonDialogControl : public ButtonDialogControl
  {public:
     StyleMaskButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int StyleMaskButtonDialogControl::select(Dialog *dialog)
{StyleMaskDialogControl *sdc;
  if ((sdc = (StyleMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
  if (id == 101)
    sdc->set(dialog,255);
  else
    sdc->set(dialog,0);
  return 0;
}

void mask_command4(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Stylemask_dialog");
 DialogControl *dc;
 BitMask styles(256);
 ResourceString rs28(NCMAIN+28);
  styles = state.selmask.style;
  dialog.title(rs28.gets());
  dialog.add((StyleMaskDialogControl *)(dc = new StyleMaskDialogControl(100,1100,256,1,8,&styles)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new StyleMaskButtonDialogControl(101));
  dialog.add(new StyleMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.style = styles;
  *cstate = ExitState;
}

void mask_command104(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Stylemask_dialog");
 DialogControl *dc;
 BitMask styles(256);
 ResourceString rs28(NCMAIN+28);
  styles = state.selmask.style;
  dialog.title(rs28.gets());
  dialog.add((StyleMaskDialogControl *)(dc = new StyleMaskDialogControl(100,1100,256,1,8,&styles)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new StyleMaskButtonDialogControl(101));
  dialog.add(new StyleMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.style = styles;
  *cstate = ExitState;
}

class WeightMaskDialogControl : public CustomDialogControl
  {public:
     int scrollid;
     int width,height,base,nacross,ndown,n,lastselected,startselection,direction,scroll;
     BitMask *weights;
     WeightMaskDialogControl(int id1,int id2,int n1,int n2,int n3,BitMask *w) :
       CustomDialogControl(id1)
         {  scrollid = id2;
            width = height = base = scroll = 0;  lastselected = -1;  weights = w;  startselection = -1;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void set(Dialog *,int);
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void lbuttonupevent(Dialog *,int,int,int);
     void mousemoveevent(Dialog *,int,int,int);
     void timerevent(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void WeightMaskDialogControl::set(Dialog *dialog,int value)
{ if (value)
    weights->setall();
  else
    weights->clearall();
  draw(dialog,NULL);
} 

void WeightMaskDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RCHPEN hpen,holdpen;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       hpen = CreatePen(PS_SOLID,db.lineweights.weighttohdc(hDC,box < 3 ? box : box == 3 ? 11 : box - 1),RGB(0,0,0));
       SetTextColor(hDC,RGB(0,0,0));
    }
  else
    {RCHBRUSH hbrush;                              
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
       hpen = CreatePen(PS_SOLID,db.lineweights.weighttohdc(hDC,box < 3 ? box : box == 3 ? 11 : box - 1),RGB(255,255,255));
       SetTextColor(hDC,RGB(255,255,255));
    }
  holdpen = (RCHPEN)SelectObject(hDC,hpen);
  MoveToEx(hDC,50,(rect.top + rect.bottom) / 2,0);
  LineTo(hDC,rect.right - 10,(rect.top + rect.bottom) / 2);
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  sprintf(text,"%.2f",db.lineweights.weighttomm(box < 3 ? box : box == 3 ? 11 : box - 1));
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT)SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = 5;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = 5;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void WeightMaskDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,k,newdc,weightindex;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN)SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      {  k = base + i * nacross + j;
         weightindex = k < 3 ? k : k == 3 ? 11 : k - 1;
         draw(dialog,hDC,k,(weights->test(weightindex) ?  DM_SELECT : DM_NORMAL));
      }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void WeightMaskDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{int weightindex,value,i1,i2,i;
  SetCapture(hWnd);
  lastselected = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));

  if ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && startselection >= 0)
    {  if (startselection < lastselected)
         {  i1 = startselection;  i2 = lastselected;
         }
       else
         {  i1 = lastselected;  i2 = startselection;  
         }

       weightindex = startselection < 3 ? startselection : startselection == 3 ? 11 : startselection - 1;
       value = weights->test(weightindex);
       for (i = i1 ; i <= i2 ; i++)
         {  weightindex = i < 3 ? i : i == 3 ? 11 : i - 1;
            weights->set(weightindex,value);
            draw(dialog,NULL,i,(weights->test(weightindex) ? DM_SELECT : DM_NORMAL));
         }
    }
  else
    {  startselection = lastselected;
       weightindex = lastselected < 3 ? lastselected : lastselected == 3 ? 11 : lastselected - 1;
       weights->toggle(weightindex);
       draw(dialog,NULL,lastselected,(weights->test(weightindex) ? DM_SELECT : DM_NORMAL));
    }

  direction = -1;
}

void WeightMaskDialogControl::mousemoveevent(Dialog *dialog,int x,int y,int buttons)
{int i,k,d,cx,cy,weightindex;
  if (lastselected < 0) return;
  if (! (buttons & MK_LBUTTON))
    {  lastselected = -1;  return;
    }
  cx = int(double(x)/double(width)*double(nacross));
  cy = int(double(y)/double(height)*double(ndown));
  if (y < 0) cy--;
  if (cx < 0) cx = 0; else if (cx >= nacross) cx = nacross - 1;
  if (cy < 0)
    {  cy = 0;
       if (base > 0)
         {  scroll = -nacross;
            vscrollevent(dialog,0,SB_LINEUP,0);
            cy = 0;
         }
       else
         scroll = 0;
    }
  else if (cy >= ndown)
    {  cy = ndown - 1;
       if (base < n - nacross*ndown)
         {  scroll = nacross;
            vscrollevent(dialog,0,SB_LINEDOWN,0);
            cy = ndown - 1;
         }
       else
         scroll = 0;
    }
  else
    scroll = 0; 
  k = base + cy * nacross + cx;
  if (k == lastselected) return;
  d = k > lastselected;
  for (i = k ; i != lastselected ; (d ? i-- : i++))
    {  weightindex = i < 3 ? i : i == 3 ? 11 : i - 1;
       weights->toggle(weightindex);
       draw(dialog,NULL,i,(weights->test(weightindex) ?  DM_SELECT : DM_NORMAL));
    }
  if (direction != -1 && d != direction)
    {  weightindex = lastselected < 3 ? lastselected : lastselected == 3 ? 11 : lastselected - 1;
       weights->toggle(weightindex);
       draw(dialog,NULL,lastselected,(weights->test(weightindex) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
  direction = d;
}

void WeightMaskDialogControl::timerevent(Dialog *dialog)
{int i,k;
  if (scroll == 0)
    return;
  else if (scroll < 0)
    {  if (base > 0)
         {  vscrollevent(dialog,0,SB_LINEUP,0);
            k = lastselected + scroll;
            if (k < 0) k = 0;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  else
    {  if (base < n - nacross * ndown)
         {  vscrollevent(dialog,0,SB_LINEDOWN,0);
            k = lastselected + scroll;
            if (k >= n) k = n-1;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  for (i = k ; i != lastselected ; (i > lastselected ? i-- : i++))
    {  weights->toggle(i);
       draw(dialog,NULL,i,(weights->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
}

void WeightMaskDialogControl::lbuttonupevent(Dialog *,int,int,int)
{ ReleaseCapture();
  lastselected = -1;
}

void WeightMaskDialogControl::changefocus(Dialog *,int)
{ lastselected = -1;
}

void WeightMaskDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class WeightMaskButtonDialogControl : public ButtonDialogControl
  {public:
     WeightMaskButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int WeightMaskButtonDialogControl::select(Dialog *dialog)
{WeightMaskDialogControl *wdc;
  if ((wdc = (WeightMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
  if (id == 101)
    wdc->set(dialog,255);
  else
    wdc->set(dialog,0);
  return 0;
}

void mask_command5(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Weightmask_dialog");
 DialogControl *dc;
 BitMask weights(256);
 ResourceString rs29(NCMAIN+29);
  weights = state.selmask.weight;
  dialog.title(rs29.gets());
  dialog.add((WeightMaskDialogControl *)(dc = new WeightMaskDialogControl(100,1100,db.lineweights.numweights(),1,8,&weights)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new WeightMaskButtonDialogControl(101));
  dialog.add(new WeightMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.weight = weights;
  *cstate = ExitState;
}

void mask_command105(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Weightmask_dialog");
 DialogControl *dc;
 BitMask weights(256);
 ResourceString rs29(NCMAIN+29);
  weights = state.selmask.weight;
  dialog.title(rs29.gets());
  dialog.add((WeightMaskDialogControl *)(dc = new WeightMaskDialogControl(100,1100,db.lineweights.numweights(),1,8,&weights)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new WeightMaskButtonDialogControl(101));
  dialog.add(new WeightMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    state.selmask.weight = weights;
  *cstate = ExitState;
}

void mask_command6(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("DisplayMask")));
  *cstate = ExitState;
}

void mask_command106(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("DisplayMask")));
  *cstate = ExitState;
}

class EDMaskButtonDialogControl : public ButtonDialogControl
  {private:
     int action;
   public:
     EDMaskButtonDialogControl(int id,int a) : ButtonDialogControl(id) {  action = a;  }
     int select(Dialog *);
  };

int EDMaskButtonDialogControl::select(Dialog *dialog)
{int i;
  for (i = 100 ; i <= 114 ; i++)
    ((CheckBoxDialogControl *)dialog->getcontrol(i))->change(dialog,action);
  return 0;
}

void mask_command7(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("EntityDisplaymask_dialog");
 int i,values[15];
  values[0] = db.dismask.entity.test(point_entity);
  values[1] = db.dismask.entity.test(line_entity);
  values[2] = db.dismask.entity.test(circle_entity);
  values[3] = db.dismask.entity.test(ellipse_entity);
  values[4] = db.dismask.entity.test(curve_entity);
  values[5] = db.dismask.entity.test(plane_entity);
  values[6] = db.dismask.entity.test(ruledsurface_entity);
  values[7] = db.dismask.entity.test(surfaceofrev_entity);
  values[8] = db.dismask.entity.test(text_entity);
  values[9] = db.dismask.entity.test(figure_entity);
  values[10] = db.dismask.entity.test(group_entity);
  values[11] = db.dismask.entity.test(dimension_mask);
  values[12] = db.dismask.entity.test(light_entity);
  values[13] = db.dismask.entity.test(patch_entity);
  values[14] = db.dismask.entity.test(bitmap_entity);
  for (i = 0 ; i < 15 ; i++)
    dialog.add(new CheckBoxDialogControl(100+i,&values[i]));
  dialog.add(new EDMaskButtonDialogControl(115,0));
  dialog.add(new EDMaskButtonDialogControl(116,1));
  if (dialog.process() == TRUE)
    {  db.dismask.entity.set(point_entity,values[0]);
       db.dismask.entity.set(line_entity,values[1]);
       db.dismask.entity.set(circle_entity,values[2]);
       db.dismask.entity.set(ellipse_entity,values[3]);
       db.dismask.entity.set(curve_entity,values[4]);
       db.dismask.entity.set(plane_entity,values[5]);
       db.dismask.entity.set(ruledsurface_entity,values[6]);
       db.dismask.entity.set(surfaceofrev_entity,values[7]);
       db.dismask.entity.set(text_entity,values[8]);
       db.dismask.entity.set(figure_entity,values[9]);
       db.dismask.entity.set(group_entity,values[10]);
       db.dismask.entity.set(dimension_mask,values[11]);
       db.dismask.entity.set(light_entity,values[12]);
       db.dismask.entity.set(patch_entity,values[13]);
       db.dismask.entity.set(bitmap_entity,values[14]);
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void mask_command107(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("EntityDisplaymask_dialog");
 int i,values[15];
  values[0] = db.dismask.entity.test(point_entity);
  values[1] = db.dismask.entity.test(line_entity);
  values[2] = db.dismask.entity.test(circle_entity);
  values[3] = db.dismask.entity.test(ellipse_entity);
  values[4] = db.dismask.entity.test(curve_entity);
  values[5] = db.dismask.entity.test(plane_entity);
  values[6] = db.dismask.entity.test(ruledsurface_entity);
  values[7] = db.dismask.entity.test(surfaceofrev_entity);
  values[8] = db.dismask.entity.test(text_entity);
  values[9] = db.dismask.entity.test(figure_entity);
  values[10] = db.dismask.entity.test(group_entity);
  values[11] = db.dismask.entity.test(dimension_mask);
  values[12] = db.dismask.entity.test(light_entity);
  values[13] = db.dismask.entity.test(patch_entity);
  values[14] = db.dismask.entity.test(bitmap_entity);
  for (i = 0 ; i < 15 ; i++)
    dialog.add(new CheckBoxDialogControl(100+i,&values[i]));
  dialog.add(new EDMaskButtonDialogControl(115,0));
  dialog.add(new EDMaskButtonDialogControl(116,1));
  if (dialog.process() == TRUE)
    {  db.dismask.entity.set(point_entity,values[0]);
       db.dismask.entity.set(line_entity,values[1]);
       db.dismask.entity.set(circle_entity,values[2]);
       db.dismask.entity.set(ellipse_entity,values[3]);
       db.dismask.entity.set(curve_entity,values[4]);
       db.dismask.entity.set(plane_entity,values[5]);
       db.dismask.entity.set(ruledsurface_entity,values[6]);
       db.dismask.entity.set(surfaceofrev_entity,values[7]);
       db.dismask.entity.set(text_entity,values[8]);
       db.dismask.entity.set(figure_entity,values[9]);
       db.dismask.entity.set(group_entity,values[10]);
       db.dismask.entity.set(dimension_mask,values[11]);
       db.dismask.entity.set(light_entity,values[12]);
       db.dismask.entity.set(patch_entity,values[13]);
       db.dismask.entity.set(bitmap_entity,values[14]);
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void mask_command8(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Colourmask_dialog");
 DialogControl *dc;
 BitMask colours(256);
 ResourceString rs30(NCMAIN+30);
  colours = db.dismask.colour;
  dialog.title(rs30.gets());
  dialog.add((ColourMaskDialogControl *)(dc = new ColourMaskDialogControl(100,1100,256,10,8,&colours)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new ColourMaskButtonDialogControl(101));
  dialog.add(new ColourMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    {  db.dismask.colour = colours;
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void mask_command108(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Colourmask_dialog");
 DialogControl *dc;
 BitMask colours(256);
 ResourceString rs30(NCMAIN+30);
  colours = db.dismask.colour;
  dialog.title(rs30.gets());
  dialog.add((ColourMaskDialogControl *)(dc = new ColourMaskDialogControl(100,1100,256,10,8,&colours)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new ColourMaskButtonDialogControl(101));
  dialog.add(new ColourMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    {  db.dismask.colour = colours;
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

extern void window_command5(int *cstate,HighLevelEvent *,void **);

void mask_command9(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Layermask_dialog");
 DialogControl *dc;
 BitMask layers(MAX_LAYERS);
 BitMask grouplayers(MAX_LAYERS);
 ResourceString rs31(NCMAIN+31);
  layers = db.dismask.layer;
  dialog.title(rs31.gets());
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(100,1100,MAX_LAYERS,1,8,&layers)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new LayerMaskButtonDialogControl(101));
  dialog.add(new LayerMaskButtonDialogControl(102));
  dialog.add(new LayerMaskButtonDialogControl(105));
  dialog.add(new LayerMaskButtonDialogControl(106));
  dialog.add(new LayerMaskButtonDialogControl(107));
  dialog.add(new LayerMaskButtonDialogControl(108));
  dialog.add(new LayerMaskButtonDialogControl(109));
  dialog.add(new LayerMaskButtonDialogControl(110));
  dialog.add(new LayerMaskButtonDialogControl(111));
  dialog.add(new LayerMaskButtonDialogControl(112));
  dialog.add(new LayerMaskButtonDialogControl(113));
  dialog.add(new LayerMaskButtonDialogControl(114));

  dialog.add(new LMaskListDialogControl(115));
  dialog.add(new LayerMaskButtonDialogControl(116));
  dialog.add(new LayerMaskButtonDialogControl(117));
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(118,1180,MAX_LAYERS,1,8,&grouplayers)));
  dialog.add(new ScrollBarDialogControl(1180,dc));
  dialog.add(new LayerMaskButtonDialogControl(119));
  dialog.add(new LayerMaskButtonDialogControl(120));
  dialog.add(new LayerMaskButtonDialogControl(121));
  dialog.add(new LayerMaskButtonDialogControl(122));
  dialog.add(new LayerMaskButtonDialogControl(123));
  dialog.add(new LayerMaskButtonDialogControl(124));
  dialog.add(new LayerMaskButtonDialogControl(125));
  dialog.add(new LayerMaskButtonDialogControl(126));
  dialog.add(new LayerMaskButtonDialogControl(127));
  dialog.add(new LayerMaskButtonDialogControl(128));
  dialog.add(new LayerMaskButtonDialogControl(129));
  dialog.add(new LayerMaskButtonDialogControl(130));

  dialog.add(new LayerMaskButtonDialogControl(131));
  dialog.add(new LayerMaskButtonDialogControl(132));


  if (dialog.process() == TRUE)
    {  db.dismask.layer = layers;
       if (cadwindow->getlayerwindow() != 0)
         cadwindow->getlayerwindow()->paintevent();
       window_command5(cstate,0,0);
    }
  *cstate = ExitState;
}

void mask_command109(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Layermask_dialog");
 DialogControl *dc;
 BitMask layers(MAX_LAYERS);
 BitMask grouplayers(MAX_LAYERS);
 ResourceString rs31(NCMAIN+31);
  layers = db.dismask.layer;
  dialog.title(rs31.gets());
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(100,1100,MAX_LAYERS,1,8,&layers)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new LayerMaskButtonDialogControl(101));
  dialog.add(new LayerMaskButtonDialogControl(102));
  dialog.add(new LayerMaskButtonDialogControl(105));
  dialog.add(new LayerMaskButtonDialogControl(106));
  dialog.add(new LayerMaskButtonDialogControl(107));
  dialog.add(new LayerMaskButtonDialogControl(108));
  dialog.add(new LayerMaskButtonDialogControl(109));
  dialog.add(new LayerMaskButtonDialogControl(110));
  dialog.add(new LayerMaskButtonDialogControl(111));
  dialog.add(new LayerMaskButtonDialogControl(112));
  dialog.add(new LayerMaskButtonDialogControl(113));
  dialog.add(new LayerMaskButtonDialogControl(114));

  dialog.add(new LMaskListDialogControl(115));
  dialog.add(new LayerMaskButtonDialogControl(116));
  dialog.add(new LayerMaskButtonDialogControl(117));
  dialog.add((LayerMaskDialogControl *)(dc = new LayerMaskDialogControl(118,1180,MAX_LAYERS,1,8,&grouplayers)));
  dialog.add(new ScrollBarDialogControl(1180,dc));
  dialog.add(new LayerMaskButtonDialogControl(119));
  dialog.add(new LayerMaskButtonDialogControl(120));
  dialog.add(new LayerMaskButtonDialogControl(121));
  dialog.add(new LayerMaskButtonDialogControl(122));
  dialog.add(new LayerMaskButtonDialogControl(123));
  dialog.add(new LayerMaskButtonDialogControl(124));
  dialog.add(new LayerMaskButtonDialogControl(125));
  dialog.add(new LayerMaskButtonDialogControl(126));
  dialog.add(new LayerMaskButtonDialogControl(127));
  dialog.add(new LayerMaskButtonDialogControl(128));
  dialog.add(new LayerMaskButtonDialogControl(129));
  dialog.add(new LayerMaskButtonDialogControl(130));

  dialog.add(new LayerMaskButtonDialogControl(131));
  dialog.add(new LayerMaskButtonDialogControl(132));


  if (dialog.process() == TRUE)
    {  db.dismask.layer = layers;
       if (cadwindow->getlayerwindow() != 0)
         cadwindow->getlayerwindow()->paintevent();
       window_command5(cstate,0,0);
    }
  *cstate = ExitState;
}

void mask_command10(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Stylemask_dialog");
 DialogControl *dc;
 BitMask styles(256);
 ResourceString rs32(NCMAIN+32);
  styles = db.dismask.style;
  dialog.title(rs32.gets());
  dialog.add((StyleMaskDialogControl *)(dc = new StyleMaskDialogControl(100,1100,256,1,8,&styles)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new StyleMaskButtonDialogControl(101));
  dialog.add(new StyleMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    {  db.dismask.style = styles;
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void mask_command110(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Stylemask_dialog");
 DialogControl *dc;
 BitMask styles(256);
 ResourceString rs32(NCMAIN+32);
  styles = db.dismask.style;
  dialog.title(rs32.gets());
  dialog.add((StyleMaskDialogControl *)(dc = new StyleMaskDialogControl(100,1100,256,1,8,&styles)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new StyleMaskButtonDialogControl(101));
  dialog.add(new StyleMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    {  db.dismask.style = styles;
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void mask_command11(int *cstate,HighLevelEvent *,void **)
{
  Dialog dialog("Weightmask_dialog");
  DialogControl *dc;
  BitMask weights(256);
  ResourceString rs33(NCMAIN+33);
  weights = db.dismask.weight;
  dialog.title(rs33.gets());
  dialog.add((WeightMaskDialogControl *)(dc = new WeightMaskDialogControl(100,1100,db.lineweights.numweights(),1,8,&weights)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new WeightMaskButtonDialogControl(101));
  dialog.add(new WeightMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
  {
       db.dismask.weight = weights;
       cadwindow->paintall();
  }
  *cstate = ExitState;
}

void mask_command111(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("Weightmask_Dialog");
 DialogControl *dc;
 BitMask weights(256);
 ResourceString rs33(NCMAIN+33);
  weights = db.dismask.weight;
  dialog.title(rs33.gets());
  dialog.add((WeightMaskDialogControl *)(dc = new WeightMaskDialogControl(100,1100,db.lineweights.numweights(),1,8,&weights)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new WeightMaskButtonDialogControl(101));
  dialog.add(new WeightMaskButtonDialogControl(102));
  if (dialog.process() == TRUE)
    {  db.dismask.weight = weights;
       cadwindow->paintall();
    }
  *cstate = ExitState;
}

class ColourDialogControl : public CustomDialogControl
  {public:
     int scrollid,valueid;
     int value,width,height,base,nacross,ndown,n;
     ColourDialogControl(int id1,int id2,int id3,int n1,int n2,int n3,int colour) :
       CustomDialogControl(id1)
         {  scrollid = id2;  valueid = id3;
            width = height = base = 0;  value = colour;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void change(Dialog *,RCHDC,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void vscrollevent(Dialog *,int,int,int);
     void lbuttondblclkevent(Dialog *,int,int,int);
  };

void ColourDialogControl::change(Dialog *dialog,RCHDC hDC,int v)
{  draw(dialog,hDC,value,DM_NORMAL);
   value = v;
   draw(dialog,hDC,value,DM_SELECT);
}

void ColourDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RCHBRUSH hbrush;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  if (GetDlgItem(dialog->gethdlg(),id) == 0)
         return;
       hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  hbrush = CreateSolidBrush(RGB(db.colourtable[box].red,db.colourtable[box].green,db.colourtable[box].blue));
  FillRect(hDC,&rect,hbrush);
  DeleteObject(hbrush);
  sprintf(text,"%d",box);
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT)SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = w / 2 - size.cx / 2;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = w / 2 - LOWORD(te) / 2;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  if (db.colourtable[box].red + db.colourtable[box].green + db.colourtable[box].blue < 50)
    SetTextColor(hDC,RGB(255,255,255));
  else
    SetTextColor(hDC,RGB(0,0,0));
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  rect.left = int(x + 1);  rect.top = int(y + 1);
  rect.right = int(w * (int((box - base) % nacross) + 1));
  rect.bottom = int(h * (int((box - base) / nacross) + 1));
  if (drawmode == DM_NORMAL)
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(GRAY_BRUSH));
  else
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  rect.left++;  rect.right--;  rect.top++;  rect.bottom--;
  if (drawmode == DM_NORMAL)
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(GRAY_BRUSH));
  else
    FrameRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void ColourDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      draw(dialog,hDC,base + i * nacross + j,DM_NORMAL);
  draw(dialog,hDC,value,DM_SELECT);
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void ColourDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{
    char text[300];
  draw(dialog,NULL,value,DM_NORMAL);
  value = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));
  draw(dialog,NULL,value,DM_SELECT);
  sprintf(text,"%d",value);
  dialog->SetDlgItemText(valueid,text);
}

void ColourDialogControl::lbuttondblclkevent(Dialog *dialog,int,int,int)
{ SendMessage(dialog->gethdlg(),WM_COMMAND,IDOK,0);
}

void ColourDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{
    int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class ColourAssocIntegerDialogControl : public IntegerDialogControl
  {public:
     ColourAssocIntegerDialogControl(int id,int *v) : IntegerDialogControl(id,v) {}
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void ColourAssocIntegerDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{long v;
 char string[300];
 ColourDialogControl *sdc;
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v--;
       else if (scrollcode == SB_LINEUP)
         v++;
       if (v < 0) v = 0; else if (v > 255) v = 255;
       sprintf(string,"%ld",v);
       dialog->SetDlgItemText(id,string);
       if ((sdc = (ColourDialogControl *)dialog->getcontrol(101)) != NULL)
         sdc->change(dialog,NULL,int(v));
    }
}

void ColourAssocIntegerDialogControl::changefocus(Dialog *dialog,int)
{ColourDialogControl *sdc;
 long v;
  if ((sdc = (ColourDialogControl *)dialog->getcontrol(101)) == NULL) return;
  currentvalue(dialog,&v);
  sdc->change(dialog,NULL,int(v));
}

/*
// now defined in ncdialog.h
class ColourScrollBarDialogControl : public ScrollBarDialogControl
  {public:
     ColourScrollBarDialogControl(int i,DialogControl *dc1) :
        ScrollBarDialogControl(i,dc1) {  }
     void vscrollevent(Dialog *,int,int,int);
     void setscrollpos(Dialog *,int);
  };
*/

void ColourScrollBarDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int npos)
{
    int pos,colour,start,end;

  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
#if ! defined(_WIN32_WCE)
  colour = int(double(GetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL) - start) / double(end - start) * 255.0 + 0.5);
#endif
  switch (scrollcode)
    {  case SB_TOP : colour = 255;  break;
       case SB_BOTTOM : colour = 0;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : colour++;  break;
       case SB_LINEUP : colour--;  break;
       case SB_PAGEDOWN : colour++;  break;
       case SB_PAGEUP : colour--;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         colour = int(double(npos - start) / double(end - start) * 255.0 + 0.5);
         break;
       case 10 :
         colour = npos;
         break;
    }
  if (colour > 255) colour = 255;
  if (colour < 0) colour = 0;
  pos = start + int((end - start) * (double(colour)) / 255.0+0.5);
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);
  if (id <= 106)
    {  ((IntegerDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,colour);
    }
  else 
    {  if (id == 110)
         ((RealDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,colour/255.0 * 360.0);
       else if (id == 111)
         ((RealDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,colour/255.0);
       else if (id == 112)
         ((RealDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,colour/255.0);

    }
  dialog->updatecustomcontrols();

}

void ColourScrollBarDialogControl::setscrollpos(Dialog *dialog,int colour)
{
#ifdef USING_WIDGETS
    int pos,start,end;
    start = ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->minimum();
    end = ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->maximum();
    pos = int(start + (end - start) * (double(colour)) / 255.0);
    ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->setSliderPosition(pos);
    //((QSlider*)GetDlgItem(dialog->gethdlg(),id))->setValue(pos);
#else
  int pos,start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
  qDebug() << "setscrollpos : start : " << start << " end : " << end;
  pos = int(start + (end - start) * (double(colour)) / 255.0);
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);
#endif
}

void TransparencyScrollBarDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int npos)
{
  int pos,value,start,end;

  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
  value = int(double(GetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL) - start) / double(end - start) * 255.0 + 0.5);
  switch (scrollcode)
    {  case SB_TOP : value = 100.0;  break;
       case SB_BOTTOM : value = 0;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : value++;  break;
       case SB_LINEUP : value--;  break;
       case SB_PAGEDOWN : value++;  break;
       case SB_PAGEUP : value--;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         value = int(double(npos - start) / double(end - start));
         break;
       case 10 :
         value = npos;
         break;
    }
  if (value > 100.0) value = 100.0;
  if (value < 0) value = 0;
  pos = start + int((end - start) * (double(value)));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);

  if(dc != 0)
    ((RealDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,value);

  dialog->updatecustomcontrols();

}

void TransparencyScrollBarDialogControl::setscrollpos(Dialog *dialog,int value)
{
#ifdef USING_WIDGETS
    int pos,start,end;
    start = ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->minimum();
    end = ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->maximum();
    pos = int(start + (end - start) * (double(value)));
    ((QSlider*)GetDlgItem(dialog->gethdlg(),id))->setSliderPosition(pos);
    //((QSlider*)GetDlgItem(dialog->gethdlg(),id))->setValue(pos);
#else
  int pos,start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
  qDebug() << "setscrollpos : start : " << start << " end : " << end;
  pos = int(start + (end - start) * (double(colour)) / 255.0);
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);
#endif
}

class ColourDisplayDialogControl : public DisplayDialogControl
  {private:
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
   public:
     ColourDisplayDialogControl(int id,Dialog *d,Colour c) : DisplayDialogControl(id)
        {  lastindex = -1;  parentdialog = d;  lastcolour = c;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };


void ColourDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if (dialog->getcontrol(100) != 0)
    {  if (! dialog->currentvalue(100,&index)) return;
       if (lastindex != -1 && lastindex != index)
         {  ((IntegerDialogControl *) dialog->getcontrol(101))->
                       change(dialog,db.colourtable[index].red);
            ((IntegerDialogControl *) dialog->getcontrol(102))->
                       change(dialog,db.colourtable[index].green);
            ((IntegerDialogControl *) dialog->getcontrol(103))->
                       change(dialog,db.colourtable[index].blue);
         }
       lastindex = index;
    }

  if (dialog->currentvalue(101,&red) &&
      dialog->currentvalue(102,&green) &&
      dialog->currentvalue(103,&blue) &&
      dialog->currentvalue(107,&hue) &&
      dialog->currentvalue(108,&lightness) &&
      dialog->currentvalue(109,&saturation))
    {  if (lastcolour.red == red && lastcolour.green == green && lastcolour.blue == blue)
         {//  HLS has changed
            c.sethls(hue,lightness,saturation);
            red = c.red;
            green = c.green;
            blue = c.blue;

            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,red);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,green);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,blue);
            ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,red);
            ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,green);
            ((IntegerDialogControl *)dialog->getcontrol(103))->change(dialog,blue);

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));

         }
       else
         {//  RGB has changed
            c.set(red,green,blue);
            c.gethls(&hue,&lightness,&saturation);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,red);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,green);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,blue);

            ((RealDialogControl *)dialog->getcontrol(107))->change(dialog,hue);
            ((RealDialogControl *)dialog->getcontrol(108))->change(dialog,lightness);
            ((RealDialogControl *)dialog->getcontrol(109))->change(dialog,saturation);

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));

         } 
       lastcolour.set(red,green,blue);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       brush = CreateSolidBrush(RGB(red,green,blue));
       FillRect(output.gethdc(),&rect,brush);
       DeleteObject(brush);
       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
            SelectObject(output.gethdc(),oldhpen);
            DeleteObject(hpen);
         }
    }
}

class DefineColourButtonDialogControl : public ButtonDialogControl
{public:
   DefineColourButtonDialogControl(int i) :
       ButtonDialogControl(i) {  }
   int select(Dialog *);
};

class SetButtonDialogControl : public ButtonDialogControl
{
public:
   SetButtonDialogControl(int id) :
       ButtonDialogControl(id) {; }
   int select(Dialog *);
};

int SetButtonDialogControl::select(Dialog *dialog)
{int index,red,green,blue;
 char key[256];

    if (dialog->currentvalue(100,&index) &&
        dialog->currentvalue(101,&red) &&
        dialog->currentvalue(102,&green) &&
        dialog->currentvalue(103,&blue))
    {
       db.colourtable[index].red = Byte(red);
       db.colourtable[index].green = Byte(green);
       db.colourtable[index].blue = Byte(blue);
       sprintf(key,"cl::%d",index);
       v.setlong(key,(long(red) << 16) | (long(green) << 8) | blue);
    }

  return 0;
}

/*
 * defined in ncdialog.h
class ColourApplyButtonDialogControl : public ButtonDialogControl
{
public:
    int mode;
public:
   ColourApplyButtonDialogControl(int id, int m=0) :
       ButtonDialogControl(id) { mode = m;  }
   int select(Dialog *);
};
*/

/**
 * @brief ColourApplyButtonDialogControl::select
 * @param dialog
 * @return
 * The select function for the colour apply dialog control.
 * it changes the colour depending on the current mode of the dialog
 */
int ColourApplyButtonDialogControl::select(Dialog *dialog)
{
 int index,ired,igreen,iblue;
 double red,green,blue;
 char key[256];

    if(mode == 0)
    { // current colour palette colour define mode
        if (dialog->currentvalue(100,&index) &&
            dialog->currentvalue(101,&red) &&
            dialog->currentvalue(102,&green) &&
            dialog->currentvalue(103,&blue))
        {
            ired   = (int)(red*255.0+0.5);
            igreen = (int)(green*255.0+0.5);
            iblue  = (int)(blue*255.0+0.5);
           db.colourtable[index].red = Byte(ired);
           db.colourtable[index].green = Byte(igreen);
           db.colourtable[index].blue = Byte(iblue);
           sprintf(key,"cl::%d",index);
           v.setlong(key,(long(ired) << 16) | (long(igreen) << 8) | iblue);
        }
    }
    else if(mode == 1)
    { // background colour define mode
        if (/*dialog->currentvalue(100,&index) &&*/
            dialog->currentvalue(101,&red) &&
            dialog->currentvalue(102,&green) &&
            dialog->currentvalue(103,&blue))
          {
            ::v.setreal("wn::backgroundcolour.red",red);
            ::v.setreal("wn::backgroundcolour.green",green);
            ::v.setreal("wn::backgroundcolour.blue",blue);

            // set the scene background for instant update
            ired   = (int)(red*255.0+0.5);
            igreen = (int)(green*255.0+0.5);
            iblue  = (int)(blue*255.0+0.5);
            cadwindow->setbackground(RGB(ired,igreen,iblue));
            cadwindow->getcurrentwindow()->setbackground(RGB(ired,igreen,iblue));
            cadwindow->getScene()->setBackgroundBrush(QBrush(QColor(ired,igreen,iblue)));
            cadwindow->paintall();
          }
    }
    else if(mode == 2)
    { // any colour define mode
        if (/*dialog->currentvalue(100,&index) &&*/
            dialog->currentvalue(101,&red) &&
            dialog->currentvalue(102,&green) &&
            dialog->currentvalue(103,&blue))
          {
            ::v.setreal("op::colourselector.red",red);
            ::v.setreal("op::colourselector.green",green);
            ::v.setreal("op::colourselector.blue",blue);
          }
    }

  return 0;
}

int DefineColourButtonDialogControl::select(Dialog *parentdialog)
{
 Dialog dialog("Definecolour_Dialog");
 DialogControl *dc;
 int i,index,red,green,blue;
 double hue,lightness,saturation;
 char key[300];

  if (id == 102)
    {  if (! parentdialog->currentvalue(100,&index)) 
         return 0;
       red = db.colourtable[index].red;
       green = db.colourtable[index].green;
       blue = db.colourtable[index].blue;

       Colour c;
       c.set(red,green,blue);
       c.gethls(&hue,&lightness,&saturation); 

       dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
       dialog.add(new ScrollBarDialogControl(1100,dc));

       dialog.add(dc = new IntegerDialogControl(101,&red,0,255));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new ColourScrollBarDialogControl(104,dc));
       dialog.add(dc = new IntegerDialogControl(102,&green,0,255));
       dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(new ColourScrollBarDialogControl(105,dc));
       dialog.add(dc = new IntegerDialogControl(103,&blue,0,255));
       dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(new ColourScrollBarDialogControl(106,dc));


       dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
       dialog.add(new ScrollBarDialogControl(1107,dc));
       dialog.add(new ColourScrollBarDialogControl(110,dc));
       dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0,1.0));
       dialog.add(new ScrollBarDialogControl(1108,dc));
       dialog.add(new ColourScrollBarDialogControl(111,dc));
       dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1109,dc));
       dialog.add(new ColourScrollBarDialogControl(112,dc));

       dialog.add((DisplayDialogControl *) new ColourDisplayDialogControl(113,parentdialog,c));
       dialog.add(new SetButtonDialogControl(114));
       int n = db.getnumericdisplaytolerance();
       db.setnumericdisplaytolerance(3);   
       dialog.process();
       db.setnumericdisplaytolerance(n);   
       ((IntegerDialogControl *) parentdialog->getcontrol(100))->change(parentdialog,index);
    }
  else
    {  v.setlong("cl::0",(255L << 16));
       v.setlong("cl::1",(255L << 8));
       v.setlong("cl::2",255L);
       v.setlong("cl::3",(255L << 16) | (255L << 8));
       v.setlong("cl::4",(255L << 8) | 255L);
       v.setlong("cl::5",(255L << 16) | 255L);
       v.setlong("cl::6",(127L << 16));
       v.setlong("cl::7",(127L << 8));
       v.setlong("cl::8",127L);
       if (::v.getreal("wn::backgroundcolour.red") == 0.0 && ::v.getreal("wn::backgroundcolour.green") == 0.0 && ::v.getreal("wn::backgroundcolour.blue") == 0.0)
         v.setlong("cl::9",(255L << 16) | (255L << 8) | 255);
       else
         v.setlong("cl::9",0L);
       for (i = 10 ; i <= 254 ; i++)
         {Colour colour;
            sprintf(key,"cl::%d",i);
            colour.sethls(i < 122 + 10 ? (i-10)/122.0*360.0 : (i-10-122)/122.0*360.0,i < 122 + 9 ? 0.5 : 0.25,1.00);
            v.setlong(key,(colour.red<<16) | (colour.green<<8) | colour.blue);
         }

       for (i = 245 ; i <= 255 ; i++)
         {Colour colour;
            sprintf(key,"cl::%d",i);
            colour.red = colour.green = colour.blue = int(255.0 * double(i-245)/10.0);
            v.setlong(key,(colour.red<<16) | (colour.green<<8) | colour.blue);
         }

       for (i = 0 ; i < 256 ; i++)
         {long c;
            sprintf(key,"cl::%d",i);
            if ((c = v.getlong(key)) != -MAXLONG)
              db.colourtable[i].set(int((c >>16)&255),int((c>>8)&255),int(c&255));
         }

    }

  ((ColourDialogControl *)parentdialog->getcontrol(101))->draw(parentdialog,NULL);

  return 0;
}

void colour_command(int *cstate,HighLevelEvent *event,void **c)
{
 static int laststate;
 static EntityList selectionlist;
 Dialog dialog("Colour_Dialog");
 DialogControl *dc;
 BitMask change(32);
 EntityList list,elist;
 Entity *e;
 int colour,status;
 ResourceString rs34(NCMAIN+34);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {  change.set(0);
       if (*cstate == 1001)
            colour = *((int *)c);
       else
            colour = v.getinteger("df::colour");
       if (! state.getselection().empty()) 
         dialog.title(rs34.gets());
       dialog.add(dc = new ColourAssocIntegerDialogControl(100,&colour));
       dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((ColourDialogControl *)(dc = new ColourDialogControl(101,1101,100,256,10,8,colour)));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new DefineColourButtonDialogControl(102));
       dialog.add(new DefineColourButtonDialogControl(103));
       if (event != 0)
         dialog.add(new ButtonDialogControl(104));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) != FALSE)
         {  if (status == 104)
              {  laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the colour from");
              }
            else
              {  if (*cstate == 1000 || *cstate == 1001)
                   *((int *)c) = colour;
                 else
                   {  list = state.getselection().copy();
                      if (list.empty())
                        {  db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                           v.setinteger("df::colour",colour);
                           cadwindow->updatetitle();
                        }
                      else 
                        {  db.saveundo(UD_STARTBLOCK,NULL);
                           RemoveLockedEntities(&list,0);
                           for (list.start() ; (e = list.next()) != NULL ; )
                             {  e->change(change,colour,0,0,0);
                                if (e->isa(figure_entity))
                                  {  cadwindow->invalidatedisplaylist(e);
                                     e->draw(DM_NORMAL);
                                  }
                                
                             }
                           list.destroy();
                           db.saveundo(UD_ENDBLOCK,NULL);
                        }
                   }
                 *cstate = ExitState;
              }
         }
       else
         *cstate = ExitState;
    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(0);
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            if (laststate == 1000)
              *((int *)c) = colour;
            else if (selectionlist.empty())
              {  db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                 v.setinteger("df::colour",colour);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,colour,0,0,0);
                   }
                 list.copy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }  
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;

}

void colour_commandEx(int *cstate,HighLevelEvent *event,void **c)
{
 static int laststate;
 static EntityList selectionlist;
 //Dialog dialog("Colour_dialog");
 Dialog dialog("MaskChange_Dialog");
 DialogControl *dc;
 BitMask change(32);
 EntityList list,elist;
 Entity *e;
 int colour,status;
 ResourceString rs34(NCMAIN+34);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {   change.set(0);
       if (*cstate == 1001)
       {
            colour = *((int *)c);
            // store the colour for the dialog to get
            v.setinteger("ss::colour",colour);
       }
       else
       {
            colour = v.getinteger("df::colour");
            // store the colour for the dialog to get
            v.setinteger("ss::colour",colour);
       }
       if (! state.getselection().empty())
         dialog.title(rs34.gets());
       //dialog.add(dc = new ColourAssocIntegerDialogControl(100,&colour));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((ColourDialogControl *)(dc = new ColourDialogControl(1131,0,0,256,10,8,colour)));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       //dialog.add(new DefineColourButtonDialogControl(102));
       //dialog.add(new DefineColourButtonDialogControl(103));
       if (event != 0)
         dialog.add(new ButtonDialogControl(107));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) != FALSE)
         {
           //qDebug() << "Satus is : " << status;
           if (status == 107)
              {
                 laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the colour from");
              }
            else
              {  if (*cstate == 1000 || *cstate == 1001)
                 {
                   colour = v.getinteger("ss::colour");
                   *((int *)c) = colour;
                 }
                 else
                   {  list = state.getselection().copy();
                      if (list.empty())
                        {
                           colour = v.getinteger("ss::colour");
                           db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                           v.setinteger("df::colour",colour);
                           cadwindow->updatetitle();
                        }
                      else
                        {
                           colour = v.getinteger("ss::colour");
                           db.saveundo(UD_STARTBLOCK,NULL);
                           RemoveLockedEntities(&list,0);
                           for (list.start() ; (e = list.next()) != NULL ; )
                             {  e->change(change,colour,0,0,0);
                                if (e->isa(figure_entity))
                                  {  cadwindow->invalidatedisplaylist(e);
                                     e->draw(DM_NORMAL);
                                  }

                             }
                           list.destroy();
                           db.saveundo(UD_ENDBLOCK,NULL);
                        }
                   }
                 *cstate = ExitState;
              }
         }
       else
         *cstate = ExitState;
       //qDebug() << "Satus is : " << status;
    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(0);
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            if (laststate == 1000)
              *((int *)c) = colour;
            else if (selectionlist.empty())
              {  db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                 v.setinteger("df::colour",colour);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,colour,0,0,0);
                   }
                 list.copy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;

#ifdef USING_WIDGETS
  if (*cstate == ExitState)
  {
    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Colour"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Colour"));
  }
#endif
}

class LayerDialogControl : public CustomDialogControl
  {public:
     int scrollid,valueid;
     int value,width,height,base,nacross,ndown,n;
     LayerDialogControl(int id1,int id2,int id3,int n1,int n2,int n3,int layer) :
       CustomDialogControl(id1)
         {  scrollid = id2;  valueid = id3;
            width = height = base = 0;  value = layer;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void change(Dialog *,RCHDC,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void vscrollevent(Dialog *,int,int,int);
     void setbase(Dialog *,int);
  };

void LayerDialogControl::change(Dialog *dialog,RCHDC hDC,int v)
{
#ifdef USING_WIDGETS
   value = v;
#else
   draw(dialog,hDC,value,DM_NORMAL);
   value = v;
   ((StringDialogControl *)dialog->getcontrol(101))->change(dialog,db.layernames.getlayername(v,1));
   draw(dialog,hDC,value,DM_SELECT);
#endif
}

void LayerDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RECT rect;
 RCHFONT hfont,holdfont;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  if (GetDlgItem(dialog->gethdlg(),id) == 0)
         return;
       hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       SetTextColor(hDC,RGB(0,0,0));
    }
  else
    {RCHBRUSH hbrush;                              
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
       SetTextColor(hDC,RGB(255,255,255));
    }
  strcpy(text,db.layernames.getlayername(box,1));
#ifdef _USING_QTCHAR
  hfont = CreateFontW(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,_RCT("MS Sans Serif"));
#else
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
#endif
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
#ifdef _USING_QTCHAR
  GetTextExtentPoint32W(hDC,text,strlen(text),&size);
#else
  GetTextExtentPoint(hDC,text,strlen(text),&size);
#endif
  xo = w / 2 - size.cx / 2;
  GetTextExtentPoint(hDC,_RCT("0"),1,&size);
  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = w / 2 - LOWORD(te) / 2;
  te = GetTextExtent(hDC,_RCT("0"),1);
  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOutW(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)), text,strlen(text));
  sprintf(text,"%d",box);
  TextOutW(hDC,int(2+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)), text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void LayerDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      draw(dialog,hDC,base + i * nacross + j,DM_NORMAL);
  draw(dialog,hDC,value,DM_SELECT);
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void LayerDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{char text[300];
  draw(dialog,NULL,value,DM_NORMAL);
  value = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));
  draw(dialog,NULL,value,DM_SELECT);
  sprintf(text,"%d",value);
  dialog->SetDlgItemText(valueid,text);
  ((StringDialogControl *)dialog->getcontrol(101))->change(dialog,db.layernames.getlayername(value,1));
}

void LayerDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

void LayerDialogControl::setbase(Dialog *dialog,int b)
{int start,end,pos;

  base = b;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);

  draw(dialog,NULL);

}

class LayerAssocIntegerDialogControl : public IntegerDialogControl
  {public:
     LayerAssocIntegerDialogControl(int id,int *v) : IntegerDialogControl(id,v) {}
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void LayerAssocIntegerDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{long v;
 char string[300];
 LayerDialogControl *sdc;
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v--;
       else if (scrollcode == SB_LINEUP)
         v++;
       if (v < 0) v = 0; else if (v > 999) v = 999;
       sprintf(string,"%ld",v);
       dialog->SetDlgItemText(id,string);
       if ((sdc = (LayerDialogControl *)dialog->getcontrol(103)) != NULL)
         sdc->change(dialog,NULL,int(v));
    }
}

void LayerAssocIntegerDialogControl::changefocus(Dialog *dialog,int)
{LayerDialogControl *sdc;
 long v;
  if ((sdc = (LayerDialogControl *)dialog->getcontrol(103)) == NULL) return;
  currentvalue(dialog,&v);
  sdc->change(dialog,NULL,int(v));
  ((StringDialogControl *)dialog->getcontrol(101))->change(dialog,db.layernames.getlayername(v,1));
}


class LayerButtonDialogControl : public ButtonDialogControl
  {public:
     LayerButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int LayerButtonDialogControl::select(Dialog *dialog)
{RCCHAR layername[300];
 int layer;
  if (id >= 105 && id <= 114)
    {  ((LayerDialogControl *)dialog->getcontrol(103))->setbase(dialog,(id - 105)*100);
    }
  else if (dialog->currentvalue(100,&layer) &&
      dialog->currentvalue(101,layername,300))
    {  db.layernames.setlayername(layer,layername);
       ((LayerDialogControl *)dialog->getcontrol(103))->change(dialog,NULL,int(layer));
    }
  return 0;
}

extern BitMask *GetLockedLayerTable(void);

void layer_command(int *cstate,HighLevelEvent *event,void **l)
{static int laststate;
 static EntityList selectionlist;
 Dialog dialog("Layer_Dialog");
 DialogControl *dc;
 BitMask change(32);
 EntityList list,elist;
 Entity *e;
 int layer,status;
 RCCHAR layername[300];
 ResourceString rs35(NCMAIN+35);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {  change.set(1);
       if (*cstate == 1001)
        layer = *((int *)l);
       else
        layer = v.getinteger("df::layer");
       strcpy(layername,db.layernames.getlayername(layer,1));
       if (! state.getselection().empty()) 
         dialog.title(rs35.gets());
       dialog.add(dc = new LayerAssocIntegerDialogControl(100,&layer));
       dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add(new StringDialogControl(101,layername,300));
       dialog.add(new LayerButtonDialogControl(102));
       dialog.add(new LayerButtonDialogControl(105));
       dialog.add(new LayerButtonDialogControl(106));
       dialog.add(new LayerButtonDialogControl(107));
       dialog.add(new LayerButtonDialogControl(108));
       dialog.add(new LayerButtonDialogControl(109));
       dialog.add(new LayerButtonDialogControl(110));
       dialog.add(new LayerButtonDialogControl(111));
       dialog.add(new LayerButtonDialogControl(112));
       dialog.add(new LayerButtonDialogControl(113));
       dialog.add(new LayerButtonDialogControl(114));
       dialog.add((LayerDialogControl *)(dc = new LayerDialogControl(103,1103,100,MAX_LAYERS,1,8,layer)));
       dialog.add(new ScrollBarDialogControl(1103,dc));
       if (event != 0)
         dialog.add(new ButtonDialogControl(104));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) != FALSE)
         {  if (status == 104)
              {  laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the layer from");
              }
            else
              {  if (*cstate == 1000 || *cstate == 1001)
                   *((int *)l) = layer;
                 else if (GetLockedLayerTable()->test(layer))
                   cadwindow->MessageBox("This layer is currently locked.","Select Layer",MB_OK);
                 else if (state.getselection().empty())
                   {  db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                      v.setinteger("df::layer",layer);
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }
                 else
                   {  list = state.getselection().copy();
                      db.saveundo(UD_STARTBLOCK,NULL);
                      RemoveLockedEntities(&list,0);
                      for (list.start() ; (e = list.next()) != NULL ; )
                        {  if (e->isa(figure_entity))
                             cadwindow->invalidatedisplaylist(e);
                           e->change(change,0,layer,0,0);
                        }
                      list.destroy();
                      db.saveundo(UD_ENDBLOCK,NULL);
                   }
                 *cstate = ExitState;
              }
         }
       else
         *cstate = ExitState;
       
    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(1);
            layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            if (laststate == 1000)
              *((int *)l) = layer;
            else if (GetLockedLayerTable()->test(layer))
              cadwindow->MessageBox("This layer is currently locked.","Select Layer",MB_OK);
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                 v.setinteger("df::layer",layer);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,layer,0,0);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }  
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;
}

void layer_commandEx(int *cstate,HighLevelEvent *event,void **l)
{
 static int laststate;
 static EntityList selectionlist;
 //Dialog dialog("Layer_dialog");
 Dialog dialog("MaskChange_Dialog");
 DialogControl *dc;
 BitMask change(32);
 EntityList list,elist;
 Entity *e;
 int layer,status;
 RCCHAR layername[300];
 ResourceString rs35(NCMAIN+35);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {  change.set(1);
       if (*cstate == 1001)
       {
        layer = *((int *)l);
        // store the layer for the dialog to get
        v.setinteger("ss::layer",layer);
       }
       else
       {
        layer = v.getinteger("df::layer");
        // store the layer for the dialog to get
        v.setinteger("ss::layer",layer);
       }
       strcpy(layername,db.layernames.getlayername(layer,1));
       if (! state.getselection().empty())
         dialog.title(rs35.gets());
       //dialog.add(dc = new LayerAssocIntegerDialogControl(100,&layer));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       //dialog.add(new StringDialogControl(101,layername,300));
       //dialog.add(new LayerButtonDialogControl(102));
       //dialog.add(new LayerButtonDialogControl(105));
       //dialog.add(new LayerButtonDialogControl(106));
       //dialog.add(new LayerButtonDialogControl(107));
       //dialog.add(new LayerButtonDialogControl(108));
       //dialog.add(new LayerButtonDialogControl(109));
       //dialog.add(new LayerButtonDialogControl(110));
       //dialog.add(new LayerButtonDialogControl(111));
       //dialog.add(new LayerButtonDialogControl(112));
       //dialog.add(new LayerButtonDialogControl(113));
       //dialog.add(new LayerButtonDialogControl(114));
       dialog.add((LayerDialogControl *)(dc = new LayerDialogControl(1111,0,0,MAX_LAYERS,1,8,layer)));
       //dialog.add(new ScrollBarDialogControl(1103,dc));
       if (event != 0)
          dialog.add(new ButtonDialogControl(107));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) != FALSE)
       {
           if (status == 107)
           {
                 laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the layer from");
           }
           else
           {
                 if (*cstate == 1000 || *cstate == 1001)
                 {
                     layer = v.getinteger("ss::layer");
                     *((int *)l) = layer;
                 }
                 else if (GetLockedLayerTable()->test(layer))
                   cadwindow->MessageBox("This layer is currently locked.","Select Layer",MB_OK);
                 else if (state.getselection().empty())
                   {
                      layer = v.getinteger("ss::layer");
                      db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                      v.setinteger("df::layer",layer);
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }
                 else
                   {
                      layer = v.getinteger("ss::layer");
                      list = state.getselection().copy();
                      db.saveundo(UD_STARTBLOCK,NULL);
                      RemoveLockedEntities(&list,0);
                      for (list.start() ; (e = list.next()) != NULL ; )
                        {  if (e->isa(figure_entity))
                             cadwindow->invalidatedisplaylist(e);
                           e->change(change,0,layer,0,0);
                        }
                      list.destroy();
                      db.saveundo(UD_ENDBLOCK,NULL);
                   }
                 *cstate = ExitState;
           }
       }
       else
           *cstate = ExitState;

    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(1);
            layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            if (laststate == 1000)
              *((int *)l) = layer;
            else if (GetLockedLayerTable()->test(layer))
              cadwindow->MessageBox("This layer is currently locked.","Select Layer",MB_OK);
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                 v.setinteger("df::layer",layer);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,layer,0,0);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;

#ifdef USING_WIDGETS
  if(*cstate == ExitState)
  {
    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Layer"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Layer"));
  }
#endif
}

class StyleDialogControl : public CustomDialogControl
  {public:
     int scrollid,valueid;
     int value,width,height,base,nacross,ndown,n;
     StyleDialogControl(int id1,int id2,int id3,int n1,int n2,int n3,int layer) :
       CustomDialogControl(id1)
         {  scrollid = id2;  valueid = id3;
            width = height = base = 0;  value = layer;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void change(Dialog *,RCHDC,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void vscrollevent(Dialog *,int,int,int);
  };

void StyleDialogControl::change(Dialog *dialog,RCHDC hDC,int v)
{
#ifdef USING_WIDGETS
    value = v;
#else
   draw(dialog,hDC,value,DM_NORMAL);
   value = v;
   draw(dialog,hDC,value,DM_SELECT);
#endif
}

void StyleDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
 LineStyle *linestyle;
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  if (GetDlgItem(dialog->gethdlg(),id) == 0)
         return;
       hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {RCHBRUSH hbrush;
       hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
    }
  else
    {RCHBRUSH hbrush;
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
    }
  Surface surface;
  surface.sethdc(NULL,hDC);
  surface.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),box,db.header.getweight(),0);
  if ((linestyle = db.linestyles.indextostyle(box)) == NULL)
    {  surface.moveto(50,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
       surface.lineto(rect.right - 10,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
    }
  else
    {  linestyle->start(1.0,rect.right - 60);
       linestyle->draw(&surface,50,(rect.top + rect.bottom) / 2,
                                 rect.right - 10,(rect.top + rect.bottom) / 2);
    }
  sprintf(text,"%d",box);
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = 5;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = 5;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void StyleDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
 RCHBRUSH hbrush;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
  FillRect(hDC,&rect,hbrush);
  DeleteObject(hbrush);
  hpen = CreatePen(PS_SOLID,1,RGB(255-::v.getreal("wn::backgroundcolour.red")*255.0,255-::v.getreal("wn::backgroundcolour.green")*255.0,255-::v.getreal("wn::backgroundcolour.blue")*255.0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      draw(dialog,hDC,base + i * nacross + j,DM_NORMAL);
  draw(dialog,hDC,value,DM_SELECT);
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void StyleDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{char text[300];
  draw(dialog,NULL,value,DM_NORMAL);
  value = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));
  draw(dialog,NULL,value,DM_SELECT);
  sprintf(text,"%d",value);
  dialog->SetDlgItemText(valueid,text);
}

void StyleDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

class StyleAssocIntegerDialogControl : public IntegerDialogControl
  {public:
     StyleAssocIntegerDialogControl(int id,int *v) : IntegerDialogControl(id,v) {}
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };

void StyleAssocIntegerDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{long v;
 char string[300];
 StyleDialogControl *sdc;
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v--;
       else if (scrollcode == SB_LINEUP)
         v++;
       if (v < 0) v = 0; else if (v > 255) v = 255;
       sprintf(string,"%ld",v);
       dialog->SetDlgItemText(id,string);
       if ((sdc = (StyleDialogControl *)dialog->getcontrol(101)) != NULL)
         sdc->change(dialog,NULL,int(v));
    }
}

void StyleAssocIntegerDialogControl::changefocus(Dialog *dialog,int)
{StyleDialogControl *sdc;
 long v;
  if ((sdc = (StyleDialogControl *)dialog->getcontrol(101)) == NULL) return;
  currentvalue(dialog,&v);
  sdc->change(dialog,NULL,int(v));
}

/*
// this is now define in the ncdialog.h file
class StyleDisplayDialogControl : public DisplayDialogControl
  {private:
     int lastindex;
     int lastsegment;
     LineStyle **linestyle;
     Dialog *parentdialog;
   public:
     StyleDisplayDialogControl(int id,Dialog *d,LineStyle **ls) : DisplayDialogControl(id)
        {  lastindex = lastsegment = -1;  parentdialog = d;  linestyle = ls;
        }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void StyleDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int index,nsegments,csegment;
 double dash,space;
 RECT rect;
 RCHBRUSH hbrush;

  if(hWnd == 0) return;
  if (! dialog->currentvalue(100,&index)) return;
  if (! dialog->currentvalue(101,&nsegments)) return;
  if (! dialog->currentvalue(102,&csegment)) return;
  if (nsegments > 0 && csegment > nsegments)
    {  csegment = nsegments;
       ((IntegerDialogControl *) dialog->getcontrol(102))->change(dialog,csegment);
    }
  if (lastindex != -1 && lastindex != index)
    {  delete *linestyle;
       *linestyle = db.linestyles.indextostyle(index);
       if (*linestyle == NULL)
         {  nsegments = 0;
            csegment = 1;
            dash = space = 0.0;
            *linestyle = new LineStyle;
            lastsegment = -1;
         }
       else
         {  *linestyle = (*linestyle)->copy();
            nsegments = (*linestyle)->getnsegments();
            csegment = 1;
            (*linestyle)->getsegment(csegment,&dash,&space);
            lastsegment = -1;
         }
       ((IntegerDialogControl *) dialog->getcontrol(101))->
                  change(dialog,nsegments);
       ((IntegerDialogControl *) dialog->getcontrol(102))->
                  change(dialog,csegment);
       ((RealDialogControl *) dialog->getcontrol(103))->
                  change(dialog,dash);
       ((RealDialogControl *) dialog->getcontrol(104))->
                  change(dialog,space);
    }
  lastindex = index;
  if (lastsegment != -1 && lastsegment != csegment)
    {  (*linestyle)->getsegment(csegment,&dash,&space);
       ((RealDialogControl *) dialog->getcontrol(103))->
                  change(dialog,dash);
       ((RealDialogControl *) dialog->getcontrol(104))->
                  change(dialog,space);

    }
  lastsegment = csegment;

#ifdef USING_WIDGETS
  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DefineStyle_Dialog*)dialog->gethdlg())->getControl(105);
  QGraphicsScene *scene = 0;
  if(ctl)
      scene = ctl->scene();

  if(ctl && scene)
  {
      output.setScene(scene);
      scene->setSceneRect(QRectF());
      scene->clear();
      QRect qrect = ctl->rect();
      rect.left = qrect.left();
      rect.top = qrect.top();
      rect.right = qrect.right();
      rect.bottom = qrect.bottom();

      output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                               ::v.getreal("wn::backgroundcolour.blue")*255.0));

      scene->setBackgroundBrush(QBrush(QColor(output.getbackground())));

      if (dialog->currentvalue(101,&nsegments) &&
          dialog->currentvalue(102,&csegment) &&
          dialog->currentvalue(103,&dash) &&
          dialog->currentvalue(104,&space))
        {
              (*linestyle)->setnsegments(nsegments);
               if (nsegments > 0)
                 (*linestyle)->setsegment(csegment,dash,space);

               output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
               output.curPen.setCapStyle(Qt::FlatCap);
               if (nsegments > 0)
               {
                    (*linestyle)->start(0.5,rect.right - 20);
                    (*linestyle)->draw(&output,10,(rect.top + rect.bottom) / 2,
                               rect.right - 10,(rect.top + rect.bottom) / 2);
               }
               else
               {
                    output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),0,db.header.getweight(),0);
                    output.moveto(10,output.getheight()-(rect.top + rect.bottom) / 2);
                    output.lineto(rect.right - 10,output.getheight()-(rect.top + rect.bottom) / 2);
               }
          }
    }
#else
  if (dialog->currentvalue(101,&nsegments) &&
      dialog->currentvalue(102,&csegment) &&
      dialog->currentvalue(103,&dash) &&
      dialog->currentvalue(104,&space))
    {  (*linestyle)->setnsegments(nsegments);
       if (nsegments > 0)
         (*linestyle)->setsegment(csegment,dash,space);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       FillRect(output.gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
       output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       output.setup(1.0,0.0,DM_GRID,0,0,0,0);
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);
       output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
       if (nsegments > 0)
         {  (*linestyle)->start(1.0,rect.right - 20);
            (*linestyle)->draw(&output,10,(rect.top + rect.bottom) / 2,
                       rect.right - 10,(rect.top + rect.bottom) / 2);
         }
       else
         {  MoveToEx(output.gethdc(),10,(rect.top + rect.bottom) / 2,0);
            LineTo(output.gethdc(),rect.right - 10,(rect.top + rect.bottom) / 2);
         }
    }
#endif
}

class DefineStyleButtonDialogControl : public ButtonDialogControl
{public:
   DefineStyleButtonDialogControl(int i) :
       ButtonDialogControl(i) {  }
   int select(Dialog *);
};

int DefineStyleButtonDialogControl::select(Dialog *parentdialog)
{
 Dialog dialog("DefineStyle_Dialog");
 DialogControl *dc;
 int index,nsegments,csegment;
 double dash,space;
 LineStyle *linestyle;
  //if (! parentdialog->currentvalue(100,&index)) return 0;
  index = v.getinteger("ss::style");
  linestyle = db.linestyles.indextostyle(index);
  if (linestyle == NULL)
  {
       nsegments = 0;
       csegment = 1;
       dash = space = 0.0;
       linestyle = new LineStyle;
  }
  else
  {
       linestyle = linestyle->copy();
       nsegments = linestyle->getnsegments();
       csegment = 1;
       linestyle->getsegment(csegment,&dash,&space);
  }
  dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
  //dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new IntegerDialogControl(101,&nsegments,0,1000));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new IntegerDialogControl(102,&csegment,1,1000));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&dash,0.1,0.0,1E20,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&space,0.1,0.0,1E20,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add((DisplayDialogControl *) new StyleDisplayDialogControl(105,parentdialog,&linestyle));
  if (dialog.process() == TRUE)
  {
       //((IntegerDialogControl *) parentdialog->getcontrol(100))->change(parentdialog,index);
       //((StyleDialogControl *)parentdialog->getcontrol(101))->draw(parentdialog,NULL);
       ((StyleDialogControl *)parentdialog->getcontrol(101))->change(parentdialog,NULL,index);
       db.linestyles.del(index);
       if (linestyle->getnsegments() > 0)
         db.linestyles.set(index,linestyle->copy());
       //InvalidateRect(parentdialog->gethdlg(),0,TRUE);
  }
  return 0;
}

class PSFNButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     PSFNButtonDialogControl(int id) : ButtonDialogControl(id)  {  }
     int select(Dialog *);
  };

int PSFNButtonDialogControl::select(Dialog *dialog)
{OPENFILENAME ofn;
 RCCHAR dirname[300],filename[300],filetitle[300],filter[300];
 int i;

  // v8
  // use the default path
  strcpy(dirname,v.getstring("db::line-styles-path"));  
  if(strlen(dirname) == 0)
      strcpy(dirname,home.getpublicfilename("Line-Styles"));
  if(strlen(dirname) > 0)
  {
      // see if it exists
      QDir path;
      path = QString(dirname);
      if(!path.exists())
      {
          // last result use the hard coded path
          strcpy(dirname,home.getpublicfilename("Line-Styles"));
      }
  }
  // v8
  // v7 code
  //_tgetcwd(dirname, sizeof(dirname));

  filename[0] = '\0';
  _tcscpy(filter,TEXT("CAD Pattern files *.cad|*.cad|"));
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = _RCT("CAD");
  ofn.lpstrTitle = _RCT("Select the pattern file");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn))
    {
       // path will have in neutral seperators
       // we need native separators mostly for the patter file paths
       // only the name in the patternsytyle object is neutral
       strcpy(filename,QDir::toNativeSeparators(QString(ofn.lpstrFile)));
       ((StringDialogControl *)dialog->getcontrol(id-1))->change(dialog,filename);
       //((DisplayDialogControl *)dialog->getcontrol(id+1))->draw(dialog,0);
    }
  return 0;
}

/*
// declared in ncdialog.h
class PatternStyleDisplayDialogControl : public DisplayDialogControl
  {private:
     Dialog *parentdialog;
     RCCHAR oldpatternlinestyle[300];
   public:
     PatternStyleDisplayDialogControl(int id,Dialog *d) : DisplayDialogControl(id)
        {  parentdialog = d;  strcpy(oldpatternlinestyle,"");
        }
     void draw(Dialog *dialog,RCHDC hDC);
     void UpdateLineStyle(RCCHAR *pls)  {  strcpy(oldpatternlinestyle,pls);  }
  };
*/

void PatternStyleDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int orientation,fitting,mirror,size;
 double height,lengthscale,shear,gap;
 RCCHAR filename[300],nutralFilename[300];
 RECT rect;
 RCCHAR patternlinestyle[300],section[300],text[300];
 RCHBRUSH hbrush;

 if(hWnd == 0)
     return;

  dialog->currentvalue(110,patternlinestyle,300);

  if (strcmp(patternlinestyle,oldpatternlinestyle) != 0)
    {  sprintf(section,_RCT("PatternLineStyle-%ls"),patternlinestyle);
       GetPrivateProfileString(section,_RCT("pls::height"),_RCT("1.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(100,text);
       GetPrivateProfileString(section,_RCT("pls::lengthscale"),_RCT("1.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(101,text);
       GetPrivateProfileString(section,_RCT("pls::size"),_RCT("Paper Size"),text,300,home.getinifilename());  dialog->SetDlgItemText(112,text);
       GetPrivateProfileString(section,_RCT("pls::filename"),_RCT(""),text,300,home.getinifilename()); dialog->SetDlgItemText(106,text);
       strcpy(oldpatternlinestyle,patternlinestyle);
    }

  shear = v.getreal("ps::shear");
  gap = v.getreal("ps::gap");
  orientation = v.getinteger("ps::orientation");
  fitting = v.getinteger("ps::fitting");
  mirror = v.getinteger("ps::mirror");

  if (dialog->currentvalue(100,&height) &&
      dialog->currentvalue(101,&lengthscale) &&
      dialog->currentvalue(112,&size) &&
      //dialog->currentvalue(102,&shear) &&
      //dialog->currentvalue(103,&gap) &&
      //dialog->currentvalue(104,&orientation) &&
      //dialog->currentvalue(105,&fitting) &&
      dialog->currentvalue(106,filename,300))
      //dialog->currentvalue(108,&mirror))
    {

      QGraphicsView *ctl = 0;
      if(dialog->gethdlg())
        ctl = (QGraphicsView*)((DefinePatternStyle_Dialog*)dialog->gethdlg())->getControl(109);
      QGraphicsScene *scene = 0;
      if(ctl)
          scene = ctl->scene();

      if(ctl && scene)
      {
          output.setScene(scene);
          scene->setSceneRect(QRectF());
          scene->clear();
          QRect qrect = ctl->rect();

          scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                                                  ::v.getreal("wn::backgroundcolour.green")*255.0+.05,
                                                  ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5)));

          output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                                   ::v.getreal("wn::backgroundcolour.green")*255.0+0.5,
                                   ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5));

          output.sizeevent(qrect.width(),qrect.height());
          scene->update();

          rect.left = 0;  rect.top = 0;
          rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());

          if(strlen(filename) == 0)
              return;

          double a,x,y,r;
          output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
          //IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
          output.setplotscale(v.getreal("vw::plotscale"));
          // make sure the patterstyle object has neutral path separators
          strcpy(nutralFilename,QDir::fromNativeSeparators(QString(filename)));

          PatternLineStyle pls(height,lengthscale,shear,gap,0.0,0.0,orientation,fitting,size,filename);

          x = (rect.left + rect.right) / 2.0;
          y = rect.bottom + (rect.bottom - rect.top) * 0.35;
          r = rect.bottom - rect.top;
          pls.start(output.getxdotspermm(),v.getreal("vw::plotscale"));
          for (a = acos(-1.0) * 3.0 / 4.0 - acos(-1.0) / 150.0 ; a >= acos(-1.0) / 4.0 ; a -= acos(-1.0) / 150.0)
          {
              pls.draw(&output,x + cos(a + acos(-1.0) / 150.0) * r,y - sin(a + acos(-1.0) / 150.0) * r,x + cos(a) * r,y - sin(a) * r);
          }
          pls.end();

      }
      /*
       double a,x,y,r;
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       FillRect(output.gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
       output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       output.setup(1.0,0.0,DM_GRID,0,0,0,0);
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);

       output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
       IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       output.setplotscale(v.getreal("vw::plotscale"));
       PatternLineStyle pls(height,lengthscale,shear,gap,0.0,0.0,orientation,fitting,size,filename);
       
       x = (rect.left + rect.right) / 2.0;
       y = rect.bottom + (rect.bottom - rect.top) * 0.35;
       r = rect.bottom - rect.top;
       pls.start(output.getxdotspermm(),v.getreal("vw::plotscale"));
       for (a = acos(-1.0) * 3.0 / 4.0 - acos(-1.0) / 150.0 ; a >= acos(-1.0) / 4.0 ; a -= acos(-1.0) / 150.0)
         pls.draw(&output,x + cos(a + acos(-1.0) / 150.0) * r,y - sin(a + acos(-1.0) / 150.0) * r,x + cos(a) * r,y - sin(a) * r);
       pls.end();
       */
    }
}

class PLSListDialogControl : public ListDialogControl
  {public:
     PLSListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void PLSListDialogControl::load(Dialog *dialog)
{
 RCCHAR patternlinestyle[300],key[300];
 int i;

  for (i = 0 ; ; i++)
    {  sprintf(key,_RCT("PatternLineStyle%d"),i);
       GetPrivateProfileString(_RCT("PatternLineStyles"),key,_RCT(""),patternlinestyle,300,home.getinifilename());
       if (strlen(patternlinestyle) == 0)
         break;
       if (strcmp(patternlinestyle,_RCT("$$__Deleted__$$")) != 0)
         ((ListDialogControl *)dialog->getcontrol(110))->additem(dialog,patternlinestyle);
    }

  dialog->SetDlgItemText(110,svalue);

}


class PLSButtonDialogControl : public ButtonDialogControl
  {public:
     PLSButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int PLSButtonDialogControl::select(Dialog *dialog)
{RCCHAR patternlinestyle[300],pls[300],key[300],section[300],text[300];
 int i;

 // KMJ: NOTE: the INI file and dialogs all show native path separators

  if (id == 121)
    {//  Delete this linestyle
       dialog->currentvalue(110,patternlinestyle,300);
       ((ListDialogControl *)dialog->getcontrol(110))->removeitem(dialog,patternlinestyle);
       //((PatternStyleDisplayDialogControl *)dialog->getcontrol(110))->UpdateLineStyle(_RCT(""));
       ((PatternStyleDisplayDialogControl *)dialog->getcontrol(109))->UpdateLineStyle(_RCT(""));
       for (i = 0 ; ; i++)
         {  sprintf(key,_RCT("PatternLineStyle%d"),i);
            GetPrivateProfileString(_RCT("PatternLineStyles"),key,_RCT(""),pls,300,home.getinifilename());
            if (strcmp(patternlinestyle,pls) == 0)
              {  WritePrivateProfileString(_RCT("PatternLineStyles"),key,_RCT("$$__Deleted__$$"),home.getinifilename());
                 break;
              }
         }
       return 0;  
    }
  else if (id == 122)
    return 122;

  dialog->currentvalue(110,patternlinestyle,300);
  //((PatternStyleDisplayDialogControl *)dialog->getcontrol(110))->UpdateLineStyle(patternlinestyle);
  ((PatternStyleDisplayDialogControl *)dialog->getcontrol(109))->UpdateLineStyle(patternlinestyle);
  for (i = 0 ; ; i++)
    {  sprintf(key,_RCT("PatternLineStyle%d"),i);
       GetPrivateProfileString(_RCT("PatternLineStyles"),key,_RCT(""),pls,300,home.getinifilename());
       if (strcmp(patternlinestyle,pls) == 0 || strlen(pls) == 0)
         break;
    }

  if (strlen(pls) == 0)
    {  ((ListDialogControl *)dialog->getcontrol(110))->additem(dialog,patternlinestyle);
       WritePrivateProfileString(_RCT("PatternLineStyles"),key,patternlinestyle,home.getinifilename());
    }

  sprintf(section,_RCT("PatternLineStyle-%ls"),patternlinestyle);
  dialog->GetDlgItemText(100,text,300);  WritePrivateProfileString(section,_RCT("pls::height"),text,home.getinifilename());
  dialog->GetDlgItemText(101,text,300);  WritePrivateProfileString(section,_RCT("pls::lengthscale"),text,home.getinifilename());
  dialog->GetDlgItemText(112,text,300);  WritePrivateProfileString(section,_RCT("pls::size"),text,home.getinifilename());
  dialog->GetDlgItemText(106,text,300);  WritePrivateProfileString(section,_RCT("pls::filename"),text,home.getinifilename());
 
  return 0;

}


 // declared in ncdialog.h
class DefinePatternStyleButtonDialogControl : public ButtonDialogControl
{public:
   DefinePatternStyleButtonDialogControl(int i) : ButtonDialogControl(i) {}
   int select(Dialog *);
};


int DefinePatternStyleButtonDialogControl::select(Dialog *parentdialog)
{
 Dialog dialog("DefinePatternStyle_Dialog");
 DialogControl *dc;
 int orientation,fitting,size,status;
 double height,lengthscale,shear,gap;
 RCCHAR filename[300],name[300];
 //static RCCHAR *orientationl[] = {  _RCT("Deformed"), _RCT("Rotated"), _RCT("Vertical")  };
 //static RCCHAR *fittingl[] = {  _RCT("None"), _RCT("Fit at ends"), _RCT("Fit at corners")  };
 ResourceString rs1(4131), rs2(4132);
 RCCHAR *sizel[] = { rs1.getws(), rs2.getws() };
 
  height = v.getreal("ps::height");
  lengthscale = v.getreal("ps::lengthscale");
  shear = v.getreal("ps::shear");
  gap = v.getreal("ps::gap");
  orientation = v.getinteger("ps::orientation");
  fitting = v.getinteger("ps::fitting");
  size = v.getinteger("ps::size");
  strcpy(filename,v.getstring("ps::filename"));

  strcpy(name,"");
  dialog.add(new PLSListDialogControl(110,name,300));
  dialog.add(dc = new RealDialogControl(100,&height,1.0,1.0E-10,1E10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&lengthscale,1.0,1.0E-10,1E10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  //dialog.add(dc = new RealDialogControl(102,&shear,1.0,-2.0,2.0));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  //dialog.add(dc = new RealDialogControl(103,&gap,1.0,0.0,1E10));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  //dialog.add(new ListDialogControl(104,3,orientationl,&orientation));
  //dialog.add(new ListDialogControl(105,3,fittingl,&fitting));
  dialog.add(new StringDialogControl(106,filename,300));
  dialog.add(new PSFNButtonDialogControl(107));
  dialog.add(new PLSButtonDialogControl(120));
  dialog.add(new PLSButtonDialogControl(121));
  dialog.add(new PLSButtonDialogControl(122));
  //dialog.add(new CheckBoxDialogControl(108,&mirror));
  dialog.add((DisplayDialogControl *) new PatternStyleDisplayDialogControl(109,parentdialog));
  dialog.add(new ListDialogControl(112,2,sizel,&size));
  if ((status = dialog.process()) == TRUE)
    {
       v.setinteger("ss::style",255);
       //((QTableView*)((MaskChange_Dialog*)parentdialog->gethdlg())->getControl(1141))->selectRow(v.getinteger("ss::style"));
       ((MaskChange_Dialog*)parentdialog->gethdlg())->selectRow(v.getinteger("ss::style"));
       v.setreal("ps::height",height);
       v.setreal("ps::lengthscale",lengthscale);
       v.setreal("ps::shear",shear);
       v.setreal("ps::gap",gap);
       v.setinteger("ps::orientation",orientation);
       v.setinteger("ps::fitting",fitting);
       v.setinteger("ps::size",size);
       v.setstring("ps::filename",filename);
       //InvalidateRect(parentdialog->gethdlg(),0,TRUE);
    }
  else if (status == 122)
  {
      state.sendevent(new NewCommandEvent(221,0));
      return 122;
  }
  return 0;
}

extern int filesaveselectionas(EntityList *list,const RCCHAR *filename, const RCCHAR *filepath=0);
extern int filesaveselectionas(EntityList *list,const char *filename, const char *filepath=0);

void style_commandEx(int *cstate,HighLevelEvent *event,void **s)
{
 static int laststate;
 static EntityList selectionlist;
 static Point p1;
 static EntityList pelist;
 //Dialog dialog("Style_dialog");
 Dialog dialog("MaskChange_Dialog");
 DialogControl *dc;
 EntityList list,elist;
 Entity *e;
 BitMask change(32);
 int i,n,style,status;
 ResourceString rs36(NCMAIN+36);
 Point p2,p3,p4;
 Line *line;


  switch(*cstate)
    {case InitialState:
     case 1000:
     case 1001:
       change.set(2);
       if (*cstate == 1001)
       {
         style = *((int *)s);
         // store the style for the dialog to get
         v.setinteger("ss::style",style);
       }
       else
       {
         style = v.getinteger("df::style");
         // store the style for the dialog to get
         v.setinteger("ss::style",style);
       }
       if (! state.getselection().empty()) dialog.title(rs36.gets());
       //dialog.add(dc = new StyleAssocIntegerDialogControl(100,&style));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((StyleDialogControl *)(dc = new StyleDialogControl(1141,0,0,256,1,8,style)));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new DefineStyleButtonDialogControl(106));
       //dialog.add(new DefinePatternStyleButtonDialogControl(103));
       if (event != 0)
         dialog.add(new ButtonDialogControl(107));
       dialog.setposition(DPTR);

       if ((status = dialog.process()) == TRUE)
         {
            if (*cstate == 1000 || *cstate == 1001)
            {
                style = v.getinteger("ss::style");
                *((int *)s) = style;
            }
            else if (state.getselection().empty())
              {
                 style = v.getinteger("ss::style");
                 db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                 v.setinteger("df::style",style);
                 cadwindow->updatetitle();
              }
            else
              {
                 style = v.getinteger("ss::style");
                 list = state.getselection().copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,style,0);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            *cstate = ExitState;
         }
       else if (status == 107)
         {  laststate = *cstate;
            selectionlist = state.getselection().copy();
            state.destroyselection(0);
            *cstate = 1;
            cadwindow->prompt->normalprompt("Select the entity to clone the style from");
         } 
       else if (status == 103)
         {  pelist = state.getselection().copy();
            state.destroyselection(0);
            *cstate = 100;
         }
       else 
         *cstate = ExitState;
       break;
     case 1 : 
       if (event->isa(EntitySelected))
         {  change.set(2);
            style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            if (laststate == 1000)
              *((int *)s) = style;
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                 v.setinteger("df::style",style);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,style,0);
                   }
                 list.destroy(); 
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }  
       *cstate = ExitState;
       break;
     case 100 :
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 101;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 101 :
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            if (p2.x - p1.x < 0.001)
              cadwindow->MessageBox("The start and end of the line are too close.","Create Line Pattern",MB_OK);
            else
              {//  Copy the current entities to a list of new lines - to be deleted in a moment
                 for (pelist.start() ; (e = pelist.next()) != 0 ; )
                   {  if (e->isa(line_entity))
                        n = 1;
                      else if (e->isa(circle_entity))
                        n = 5;
                      else if (e->isa(curve_entity))
                        n = 5;
                      else
                        n = 0;

                      if (n > 0)
                        {  p3 = ((LinearEntity *)e)->position(0.0);
                           for (i = 1 ; i <= n ; i++)
                             {  p4 = ((LinearEntity *)e)->position(double(i)/double(n));
                                line = new Line((p3-p1)/(p2.x - p1.x),(p4-p1)/(p2.x - p1.x));
                                db.geometry.add(line,0,0);
                                list.add(line);
                                p3 = p4;
                             }
                        }

                      e->unhighlight();

                   }

                 state.setselection(list.copy(),1); 
                 filesaveselectionas(0,(char*)0);
                 state.destroyselection(0);

                 for (list.start() ; (e = list.next()) != 0 ; )
                   db.geometry.del(e,0,0);

              } 
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
//
//  About to enter state actions
//
  if (*cstate == 100 || *cstate == 101)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 100)
    cadwindow->prompt->normalprompt("Locate the start of the line pattern");
  else if (*cstate == 101)
    cadwindow->prompt->normalprompt("Locate the end of the line pattern");
#ifdef USING_WIDGETS
  else if (*cstate == ExitState)
  {
    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Style"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Style"));
  }
#endif

}

void style_command(int *cstate,HighLevelEvent *event,void **s)
{static int laststate;
 static EntityList selectionlist;
 static Point p1;
 static EntityList pelist;
 Dialog dialog("Style_Dialog");
 DialogControl *dc;
 EntityList list,elist;
 Entity *e;
 BitMask change(32);
 int i,n,style,status;
 ResourceString rs36(NCMAIN+36);
 Point p2,p3,p4;
 Line *line;


  switch(*cstate)
    {case InitialState:
     case 1000:
     case 1001:
       change.set(2);
       if (*cstate == 1001)
         style = *((int *)s);
       else
         style = v.getinteger("df::style");
       if (! state.getselection().empty()) dialog.title(rs36.gets());
       dialog.add(dc = new StyleAssocIntegerDialogControl(100,&style));
       dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((StyleDialogControl *)(dc = new StyleDialogControl(101,1101,100,256,1,8,style)));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new DefineStyleButtonDialogControl(102));
       dialog.add(new DefinePatternStyleButtonDialogControl(103));
       if (event != 0)
         dialog.add(new ButtonDialogControl(104));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) == TRUE)
         {  if (*cstate == 1000 || *cstate == 1001)
              *((int *)s) = style;
            else if (state.getselection().empty())
              {  db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                 v.setinteger("df::style",style);
                 cadwindow->updatetitle();
              }
            else
              {  list = state.getselection().copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,style,0);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            *cstate = ExitState;
         }
       else if (status == 104)
         {  laststate = *cstate;
            selectionlist = state.getselection().copy();
            state.destroyselection(0);
            *cstate = 1;
            cadwindow->prompt->normalprompt("Select the entity to clone the style from");
         }
       else if (status == 103)
         {  pelist = state.getselection().copy();
            state.destroyselection(0);
            *cstate = 100;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  change.set(2);
            style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            if (laststate == 1000)
              *((int *)s) = style;
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                 v.setinteger("df::style",style);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,style,0);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }
       *cstate = ExitState;
       break;
     case 100 :
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 101;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 101 :
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            if (p2.x - p1.x < 0.001)
              cadwindow->MessageBox("The start and end of the line are too close.","Create Line Pattern",MB_OK);
            else
              {//  Copy the current entities to a list of new lines - to be deleted in a moment
                 for (pelist.start() ; (e = pelist.next()) != 0 ; )
                   {  if (e->isa(line_entity))
                        n = 1;
                      else if (e->isa(circle_entity))
                        n = 5;
                      else if (e->isa(curve_entity))
                        n = 5;
                      else
                        n = 0;

                      if (n > 0)
                        {  p3 = ((LinearEntity *)e)->position(0.0);
                           for (i = 1 ; i <= n ; i++)
                             {  p4 = ((LinearEntity *)e)->position(double(i)/double(n));
                                line = new Line((p3-p1)/(p2.x - p1.x),(p4-p1)/(p2.x - p1.x));
                                db.geometry.add(line,0,0);
                                list.add(line);
                                p3 = p4;
                             }
                        }

                      e->unhighlight();

                   }

                 state.setselection(list.copy(),1);
                 filesaveselectionas(0,(char*)0);
                 state.destroyselection(0);

                 for (list.start() ; (e = list.next()) != 0 ; )
                   db.geometry.del(e,0,0);

              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
//
//  About to enter state actions
//
  if (*cstate == 100 || *cstate == 101)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 100)
    cadwindow->prompt->normalprompt("Locate the start of the line pattern");
  else if (*cstate == 101)
    cadwindow->prompt->normalprompt("Locate the end of the line pattern");

}

void clone_command(int *cstate,HighLevelEvent *event,void **s)
{
 static int laststate;
 static EntityList selectionlist;
 static Point p1;
 static EntityList pelist;
 EntityList list,elist;
 int colour,layer,style,weight;
 ResourceString rs36(NCMAIN+36);
 Point p2,p3,p4;
 int cloneMode;
 BitMask change(32);
 CloneInfo cloneInfo;

  switch(*cstate)
    {case InitialState:
       *cstate = 1;
       break;
     case 1000:
      if(s != 0)
          cloneInfo = CloneInfo(*(CloneInfo*)s);
        *cstate = 2;
        break;
     case 1 :
       state.destroyselection(1); 
       if (event->isa(EntitySelected))
       {
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            layer  = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            style  = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            v.setinteger("df::layer",layer);
            v.setinteger("df::style",style);
            v.setinteger("df::weight",weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
       }
       *cstate = ExitState;
#ifdef USING_WIDGETS
  cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Clone"));
  cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Clone"));
#endif
       break;
    case 2:
      state.destroyselection(1);
      if (event->isa(EntitySelected))
      {
          if(cloneInfo.mode == 0)
          {
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            layer  = cloneInfo.layer;
            style  = cloneInfo.style;
            weight = cloneInfo.weight;
          }
          else if(cloneInfo.mode == 0)
          {
            colour = cloneInfo.colour;
            layer  = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            style  = cloneInfo.style;
            weight = cloneInfo.weight;
          }
          else if(cloneInfo.mode == 0)
          {
            colour = cloneInfo.colour;
            layer  = cloneInfo.layer;
            style  = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            weight = cloneInfo.weight;
          }
          else if(cloneInfo.mode == 0)
          {
            colour = cloneInfo.colour;
            layer  = cloneInfo.layer;
            style  = cloneInfo.style;
            weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
          }
          db.header.change(colour,layer, style,weight);
          v.setinteger("df::colour",colour);
          v.setinteger("df::layer",layer);
          v.setinteger("df::style",style);
          v.setinteger("df::weight",weight);
          if (cadwindow->getlayerwindow() != 0)
            cadwindow->getlayerwindow()->paintevent();
          cadwindow->updatetitle();
      }
      *cstate = ExitState;
      break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 || *cstate == 2)
    cadwindow->prompt->normalprompt("Locate the entity to clone the attributes of");

}

// todo proposed new combied clone command
// for cloning all properties
// from a selected entity to set the defaults
// from a selected entity to other selected entities
// for cloning individual properties
// from a selected entity to set the defaults
// from a selected entity to other selected entities
//
void new_clone_command(int *cstate,HighLevelEvent *event,void **s)
{
 static EntityList selectionlist;
 Entity *e;
 int colour,layer,style,weight;

  switch(*cstate)
    {case InitialState:
       if(! state.getselection().empty())
       {
          selectionlist = state.getselection().copy();
          state.destroyselection(1);
          selectionlist.start();
          e = selectionlist.next();
          if(e != NULL )
          {
              colour = e->getcolour();
              layer  = v.getinteger("df::layer");
              style  = v.getinteger("df::style");
              weight = v.getinteger("df::weight");
              db.header.change(colour,layer,style,weight);
              v.setinteger("df::colour",colour);
              v.setinteger("df::layer",layer);
              v.setinteger("df::style",style);
              v.setinteger("df::weight",weight);
              if (cadwindow->getlayerwindow() != 0)
                cadwindow->getlayerwindow()->paintevent();
              cadwindow->updatetitle();
          }
       }
       *cstate = ExitState;
       break;
    }
}

#ifdef USING_WIDGETS
void clone_colour_command(int *cstate,HighLevelEvent *event,void **s)
{
 static EntityList selectionlist;
 Entity *e;
 int colour,layer,style,weight;
 //ResourceString rs36(NCMAIN+36);
 BitMask change(32);

  switch(*cstate)
    {case InitialState:
       if(! state.getselection().empty())
       {
          selectionlist = state.getselection().copy();
          state.destroyselection(0);
       }
       *cstate = 1;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected))
       {
           if(! selectionlist.empty())
           {
               change.set(0);
               colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
               db.saveundo(UD_STARTBLOCK,NULL);
               RemoveLockedEntities(&selectionlist,0);
               for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
               {
                   if (e->isa(figure_entity))
                      cadwindow->invalidatedisplaylist(e);
                    e->change(change,colour,0,0,0);
               }
               //selectionlist.unhighlight();
               //selectionlist.destroy();
               state.setselection(selectionlist,1);
               db.saveundo(UD_ENDBLOCK,NULL);
               cadwindow->paintall(1);
           }
           else
           {
               colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
               layer  = v.getinteger("df::layer");
               style  = v.getinteger("df::style");
               weight = v.getinteger("df::weight");
               db.header.change(colour,layer,style,weight);
               v.setinteger("df::colour",colour);
               v.setinteger("df::layer",layer);
               v.setinteger("df::style",style);
               v.setinteger("df::weight",weight);
               if (cadwindow->getlayerwindow() != 0)
                 cadwindow->getlayerwindow()->paintevent();
               cadwindow->updatetitle();
           }
       }
       *cstate = ExitState;
       break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 )
    cadwindow->prompt->normalprompt("Locate the entity to clone the colour of");

}

void clone_to_default_colour_command(int *cstate,HighLevelEvent *event,void **s)
{
 static EntityList selectionlist;
 Entity *e;
 int colour,layer,style,weight;

  switch(*cstate)
    {case InitialState:
       if(! state.getselection().empty())
       {
          selectionlist = state.getselection().copy();
          state.destroyselection(1);
          selectionlist.start();
          e = selectionlist.next();
          if(e != NULL )
          {
              colour = e->getcolour();
              layer  = v.getinteger("df::layer");
              style  = v.getinteger("df::style");
              weight = v.getinteger("df::weight");
              db.header.change(colour,layer,style,weight);
              v.setinteger("df::colour",colour);
              v.setinteger("df::layer",layer);
              v.setinteger("df::style",style);
              v.setinteger("df::weight",weight);
              if (cadwindow->getlayerwindow() != 0)
                cadwindow->getlayerwindow()->paintevent();
              cadwindow->updatetitle();
          }
       }
       *cstate = ExitState;
       break;
    }
}

void clone_to_default_layer_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;

     switch(*cstate)
       {case InitialState:
          if(! state.getselection().empty())
          {
             selectionlist = state.getselection().copy();
             state.destroyselection(1);
             selectionlist.start();
             e = selectionlist.next();
             if(e != NULL )
             {
                 colour = v.getinteger("df::colour");
                 layer  = e->getlayer();
                 style  = v.getinteger("df::style");
                 weight = v.getinteger("df::weight");
                 db.header.change(colour,layer,style,weight);
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",weight);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
             }
          }
          *cstate = ExitState;
          break;
       }
}

void clone_to_default_style_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;

     switch(*cstate)
       {case InitialState:
          if(! state.getselection().empty())
          {
             selectionlist = state.getselection().copy();
             state.destroyselection(1);
             selectionlist.start();
             e = selectionlist.next();
             if(e != NULL )
             {
                 colour = v.getinteger("df::colour");
                 layer  = v.getinteger("df::layer");
                 style  = e->getstyle();
                 weight = v.getinteger("df::weight");
                 db.header.change(colour,layer,style,weight);
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",weight);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
             }
          }
          *cstate = ExitState;
          break;
       }

}

void clone_to_default_weight_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;

     switch(*cstate)
       {case InitialState:
          if(! state.getselection().empty())
          {
             selectionlist = state.getselection().copy();
             state.destroyselection(1);
             selectionlist.start();
             e = selectionlist.next();
             if(e != NULL )
             {
                 colour = v.getinteger("df::colour");
                 layer  = v.getinteger("df::layer");
                 style  = v.getinteger("df::style");
                 weight = e->getweight();
                 db.header.change(colour,layer,style,weight);
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",weight);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
             }
          }
          *cstate = ExitState;
          break;
       }
}

void clone_all_to_default_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;

     switch(*cstate)
       {case InitialState:
          if(! state.getselection().empty())
          {
             selectionlist = state.getselection().copy();
             state.destroyselection(1);
             selectionlist.start();
             e = selectionlist.next();
             if(e != NULL )
             {
                 colour = e->getcolour();
                 layer  = e->getlayer();
                 style  = e->getstyle();
                 weight = e->getweight();
                 db.header.change(colour,layer,style,weight);
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",weight);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
             }
          }
          *cstate = ExitState;
          break;
       }
}

void clone_layer_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;
    //ResourceString rs36(NCMAIN+36);
    BitMask change(32);

  switch(*cstate)
    {case InitialState:
      if(! state.getselection().empty())
      {
         selectionlist = state.getselection().copy();
         state.destroyselection(0);
      }
      *cstate = 1;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected))
       {
           if(! selectionlist.empty())
           {
               change.set(1);
               layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
               db.saveundo(UD_STARTBLOCK,NULL);
               RemoveLockedEntities(&selectionlist,0);
               for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
               {
                   if (e->isa(figure_entity))
                      cadwindow->invalidatedisplaylist(e);
                    e->change(change,0,layer,0,0);
               }
               //selectionlist.unhighlight();
               //selectionlist.destroy();
               state.setselection(selectionlist,1);
               db.saveundo(UD_ENDBLOCK,NULL);
               cadwindow->paintall(1);
           }
           else
           {
             colour = v.getinteger("df::colour");
            layer  = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            style  = v.getinteger("df::style");
            weight = v.getinteger("df::weight");
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            v.setinteger("df::layer",layer);
            v.setinteger("df::style",style);
            v.setinteger("df::weight",weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
           }
       }
       *cstate = ExitState;
       break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 )
    cadwindow->prompt->normalprompt("Locate the entity to clone the layer of");

}

void clone_style_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;
    //ResourceString rs36(NCMAIN+36);
    BitMask change(32);

  switch(*cstate)
    {case InitialState:
      if(! state.getselection().empty())
      {
         selectionlist = state.getselection().copy();
         state.destroyselection(0);
      }
      *cstate = 1;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected))
       {
           if(! selectionlist.empty())
           {
               change.set(2);
               style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
               db.saveundo(UD_STARTBLOCK,NULL);
               RemoveLockedEntities(&selectionlist,0);
               for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
               {
                   if (e->isa(figure_entity))
                      cadwindow->invalidatedisplaylist(e);
                    e->change(change,0,0,style,0);
               }
               //selectionlist.unhighlight();
               //selectionlist.destroy();
               state.setselection(selectionlist,1);
               db.saveundo(UD_ENDBLOCK,NULL);
               cadwindow->paintall(1);
           }
           else
           {
            colour = v.getinteger("df::colour");
            layer  = v.getinteger("df::layer");
            style  = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            weight = v.getinteger("df::weight");
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            v.setinteger("df::layer",layer);
            v.setinteger("df::style",style);
            v.setinteger("df::weight",weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
           }
       }
       *cstate = ExitState;
       break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 )
    cadwindow->prompt->normalprompt("Locate the entity to clone the style of");

}

void clone_weight_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;
    //ResourceString rs36(NCMAIN+36);
    BitMask change(32);

  switch(*cstate)
    {case InitialState:
      if(! state.getselection().empty())
      {
         selectionlist = state.getselection().copy();
         state.destroyselection(0);
      }
      *cstate = 1;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected))
       {
           if(! selectionlist.empty())
           {
               change.set(3);
               weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
               db.saveundo(UD_STARTBLOCK,NULL);
               RemoveLockedEntities(&selectionlist,0);
               for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
               {
                   if (e->isa(figure_entity))
                      cadwindow->invalidatedisplaylist(e);
                    e->change(change,0,0,0,weight);
               }
               //selectionlist.unhighlight();
               //selectionlist.destroy();
               state.setselection(selectionlist,1);
              db.saveundo(UD_ENDBLOCK,NULL);
               cadwindow->paintall(1);
           }
           else
           {
            colour = v.getinteger("df::colour");
            layer  = v.getinteger("df::layer");
            style  = v.getinteger("df::style");
            weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            v.setinteger("df::layer",layer);
            v.setinteger("df::style",style);
            v.setinteger("df::weight",weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
           }
       }
       *cstate = ExitState;
       break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 )
    cadwindow->prompt->normalprompt("Locate the entity to clone the weight of");

}

/**
 * @brief clone_all_command
 * @param cstate
 * @param event
 * @param s
 *
 * Clones the colour, layer, weight and style from one selected entity to another
 *
 */
void clone_all_command(int *cstate,HighLevelEvent *event,void **s)
{
    static EntityList selectionlist;
    Entity *e;
    int colour,layer,style,weight;
    //ResourceString rs36(NCMAIN+36);
    BitMask change(32);

  switch(*cstate)
    {case InitialState:
      if(! state.getselection().empty())
      {
         selectionlist = state.getselection().copy();
         state.destroyselection(0);
      }
      *cstate = 1;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected))
       {
           if(! selectionlist.empty())
           {
               change.set(0);
               change.set(1);
               change.set(2);
               change.set(3);
               colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();//v.getinteger("df::colour");
               layer  = ((EntitySelectedEvent *) event)->getentity()->getlayer(); //v.getinteger("df::layer");
               style  = ((EntitySelectedEvent *) event)->getentity()->getstyle(); // v.getinteger("df::style");
               weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
               db.saveundo(UD_STARTBLOCK,NULL);
               RemoveLockedEntities(&selectionlist,0);
               for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
               {
                   if (e->isa(figure_entity))
                      cadwindow->invalidatedisplaylist(e);
                    e->change(change,colour,layer,style,weight);
               }
               state.setselection(selectionlist,1);
               db.saveundo(UD_ENDBLOCK,NULL);
               cadwindow->paintall(1);
           }
           else
           {
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            layer  = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            style  = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            weight = ((EntitySelectedEvent *) event)->getentity()->getweight();
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            v.setinteger("df::layer",layer);
            v.setinteger("df::style",style);
            v.setinteger("df::weight",weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
           }
       }
       *cstate = ExitState;
       break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1 )
    cadwindow->prompt->normalprompt("Locate the entity to clone the weight of");

}

void define_pattern_line_style_command(int *cstate,HighLevelEvent *event,void **)
{
    static int laststate;
    static EntityList pelist;
    static Point p1;
    Point p2,p3,p4;
    Entity *e;
    Line *line;
    EntityList list;
    int i,n;

    switch(*cstate)
    {
    case InitialState:
        laststate = *cstate;
        pelist = state.getselection().copy();
        state.destroyselection(0);
        *cstate = 100;
        break;
    case 100 : // start point of line pattern
      if (event->isa(Coordinate3d))
        {  p1 = ((Coordinate3dEvent *) event)->getp();
           *cstate = 101;
        }
      else if (event->isa(Abort) || event->isa(Exit))
        *cstate = ExitState;
      break;
    case 101 : // end point of line pattern
      if (event->isa(Coordinate3d))
        {  p2 = ((Coordinate3dEvent *) event)->getp();
           if (p2.x - p1.x < 0.001)
             cadwindow->MessageBox("The start and end of the line are too close.","Create Line Pattern",MB_OK);
           else
             {//  Copy the current entities to a list of new lines - to be deleted in a moment
                for (pelist.start() ; (e = pelist.next()) != 0 ; )
                  {  if (e->isa(line_entity))
                       n = 1;
                     else if (e->isa(circle_entity))
                       n = 5;
                     else if (e->isa(curve_entity))
                       n = 5;
                     else
                       n = 0;

                     if (n > 0)
                       {  p3 = ((LinearEntity *)e)->position(0.0);
                          for (i = 1 ; i <= n ; i++)
                            {  p4 = ((LinearEntity *)e)->position(double(i)/double(n));
                               line = new Line((p3-p1)/(p2.x - p1.x),(p4-p1)/(p2.x - p1.x));
                               db.geometry.add(line,0,0);
                               list.add(line);
                               p3 = p4;
                            }
                       }

                     e->unhighlight();

                  }

                state.setselection(list.copy(),1);
                // v8
                RCCHAR filepath[600];
                strcpy(filepath,v.getstring("db::line-styles-path"));
                if(strlen(filepath) > 1)
                {
                    QDir path;
                    path = QString(filepath);
                    if(!path.exists())
                        strcpy(filepath,home.getpublicfilename("Line-Styles"));
                }
                else
                {
                    // last resort use the hard coded path
                    strcpy(filepath,home.getpublicfilename("Line-Styles"));
                }
                filesaveselectionas(0,(RCCHAR*)0,filepath);
                // v8
                state.destroyselection(0);

                for (list.start() ; (e = list.next()) != 0 ; )
                  db.geometry.del(e,0,0);

             }
           *cstate = ExitState;
        }
      else if (event->isa(Abort) || event->isa(Exit))
        *cstate = ExitState;
      break;
    }
    //
    //  About to enter state actions
    //
    if (*cstate == 100 || *cstate == 101)
      state.selmask.entity.clearandset(xyz_mask,end_list);

    if (*cstate == 100)
      cadwindow->prompt->normalprompt("Locate the start of the line pattern");
    else if (*cstate == 101)
      cadwindow->prompt->normalprompt("Locate the end of the line pattern");
}
#endif

class WeightDialogControl : public CustomDialogControl
{
 public:
     int scrollid,valueid;
     int value,width,height,base,nacross,ndown,n;
     WeightDialogControl(int id1,int id2,int id3,int n1,int n2,int n3,int weight) :
       CustomDialogControl(id1)
         {  scrollid = id2;  valueid = id3;
            width = height = base = 0;  value = weight;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void change(Dialog *,RCHDC,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void vscrollevent(Dialog *,int,int,int);
};

void WeightDialogControl::change(Dialog *dialog,RCHDC hDC,int v)
{
#ifdef USING_WIDGETS
    value=v;
#else
   draw(dialog,0,value < 3 ? value : value == 11 ? 3 : value + 1,DM_NORMAL);
   value = v;
   draw(dialog,0,value < 3 ? value : value == 11 ? 3 : value + 1,DM_SELECT);
#endif
}

void WeightDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{
 int newdc,weightindex;
 RCHFONT hfont,holdfont;
 RECT rect;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
 LineStyle *linestyle;
 RCHBRUSH hbrush;

  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  if (GetDlgItem(dialog->gethdlg(),id) == 0)
         return;
       hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
    }
  else
    {RCHBRUSH hbrush;                              
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
    }
  Surface surface;
  surface.sethdc(NULL,hDC);
  weightindex = box < 3 ? box : box == 3 ? 11 : box - 1;
  surface.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),weightindex,0);
  if ((linestyle = db.linestyles.indextostyle(db.header.getstyle())) == NULL)
    {  surface.moveto(50,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
       surface.lineto(rect.right - 10,surface.getheight()-1.0-(rect.top + rect.bottom) / 2);
    }
  else
    {  linestyle->start(1.0,rect.right - 60);
       linestyle->draw(&surface,50,(rect.top + rect.bottom) / 2,
                                 rect.right - 10,(rect.top + rect.bottom) / 2);
    }
  sprintf(text,"%.2f",db.lineweights.weighttomm(weightindex));
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"MS Sans Serif");
  if (hfont != 0)
    holdfont = (RCHFONT) SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
#if ! defined(_WIN32_WCE)
  GetTextExtentPoint(hDC,text,strlen(text),&size);
//WCEFIX
#endif
  xo = 5;  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = 5;  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void WeightDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 int i,j,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
 RCHBRUSH hbrush;

  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
  FillRect(hDC,&rect,hbrush);
  DeleteObject(hbrush);
  hpen = CreatePen(PS_SOLID,1,RGB(255-::v.getreal("wn::backgroundcolour.red")*255.0,255-::v.getreal("wn::backgroundcolour.green")*255.0,255-::v.getreal("wn::backgroundcolour.blue")*255.0));
  holdpen = (RCHPEN) SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      draw(dialog,hDC,base + i * nacross + j,DM_NORMAL);
  draw(dialog,hDC,value < 3 ? value : value == 11 ? 3 : value + 1,DM_SELECT);
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void WeightDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{
  char text[300];
  draw(dialog,NULL,value < 3 ? value : value == 11 ? 3 : value + 1,DM_NORMAL);
  value = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));
  value = value < 3 ? value : value == 3 ? 11 : value - 1;
  draw(dialog,NULL,value < 3 ? value : value == 11 ? 3 : value + 1,DM_SELECT);
  sprintf(text,"%.2f",db.lineweights.weighttomm(value));
  dialog->SetDlgItemText(valueid,text);
}

void WeightDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{
  int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

/*
 * this is now in the ncdialog header
class WeightAssocRealDialogControl : public RealDialogControl
{
public:
     WeightAssocRealDialogControl(int id,double *v, QWidget *qw=0) : RealDialogControl(id,v,DontScale,qw){ }
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
};
*/

void WeightAssocRealDialogControl::change(Dialog *d,double v)
{
    double curweight;
    double newweight;
    int weight;

    if (currentvalue(d,&curweight))
    {
        weight = db.lineweights.mmtoweight(v);
        if (weight < 0) weight = 0;
        if (weight > db.lineweights.numweights()) weight = db.lineweights.numweights() - 1;
        newweight = db.lineweights.weighttomm(weight);
        RealDialogControl::change(d,newweight);
    }
}

void WeightAssocRealDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{
 double v;
 int weight;
 char string[300];
 WeightDialogControl *sdc;
  if (currentvalue(dialog,&v))
    {  weight = db.lineweights.mmtoweight(v);
       if (scrollcode == SB_LINEDOWN)
         {  weight--;
            if (weight == 2)
              weight = 11;
            else if (weight == 10)
              weight = 2;
         }
       else if (scrollcode == SB_LINEUP)
         {  weight++;
            if (weight == 3)
              weight = 11;
            else if (weight == 12)
              weight = 3;
            else if (weight == 11)
              weight = 10;
         }
         
       if (weight < 0) weight = 0;
       if (weight > db.lineweights.numweights()) weight = db.lineweights.numweights() - 1;
       sprintf(string,"%.2f",db.lineweights.weighttomm(weight));
       dialog->SetDlgItemText(id,string);
       if (id == 100 && (sdc = (WeightDialogControl *)dialog->getcontrol(101)) != NULL)
         sdc->change(dialog,NULL,weight);
    }
}

void WeightAssocRealDialogControl::changefocus(Dialog *dialog,int)
{
 WeightDialogControl *sdc;
 double v;
  if (id != 100)
    return;
  if ((sdc = (WeightDialogControl *)dialog->getcontrol(id+1)) == NULL) return;
  currentvalue(dialog,&v);
  sdc->change(dialog,NULL,db.lineweights.mmtoweight(v));
}

void weight_command(int *cstate,HighLevelEvent *event,void **wt)
{
 static int laststate;
 static EntityList selectionlist;
 Dialog dialog("Weight_Dialog");
 DialogControl *dc;
 EntityList list,elist;
 Entity *e;
 BitMask change(32);
 double weight;
 int w,status,iweight;
 ResourceString rs37(NCMAIN+37);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {  change.set(3);
       if (*cstate == 1001)
       {
         weight = *((double *)wt);
         // store the weight for the QDoubleSpinBox to get
         v.setreal("ss::weight",weight);
       }
       else
       {
          weight = db.lineweights.weighttomm(v.getinteger("df::weight"));
          // store the weight for the QDoubleSpinBox to get
          v.setreal("ss::weight",weight);
       }
       iweight = db.lineweights.mmtoweight(weight);
       if (! state.getselection().empty()) 
       dialog.title(rs37.gets());
       dialog.add(dc = new WeightAssocRealDialogControl(100,&weight));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((WeightDialogControl *)(dc = new WeightDialogControl(101,1101,100,db.lineweights.numweights(),1,8,iweight)));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       if (event != 0)
         dialog.add(new ButtonDialogControl(102));
       dialog.setposition(DPTR);
       if ((status = dialog.process()) != FALSE)
         {  if (status == 102)
              {  laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the weight from");
              }
            else
              {  if (*cstate == 1000 || *cstate == 1001)             
                   *((double *)wt) = weight;
                 else if (state.getselection().empty())
                   {
                      db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),db.lineweights.mmtoweight(weight));
                      v.setinteger("df::weight",db.lineweights.mmtoweight(weight));
                      cadwindow->updatetitle();
                   }
                 else
                   {  list = state.getselection().copy();
                      w = db.lineweights.mmtoweight(weight);
                      db.saveundo(UD_STARTBLOCK,NULL);
                      RemoveLockedEntities(&list,0);
                      for (list.start() ; (e = list.next()) != NULL ; )
                        {  if (e->isa(figure_entity))
                             cadwindow->invalidatedisplaylist(e);
                           e->change(change,0,0,0,w);
                        }
                      db.saveundo(UD_ENDBLOCK,NULL);
                      list.destroy(); 
                   }
                 *cstate = ExitState;
              }
         }
       else
         *cstate = ExitState;
    } 
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(3);
            w = ((EntitySelectedEvent *) event)->getentity()->getweight();
            if (laststate == 1000)
              *((int *)wt) = w;
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),w);
                 v.setinteger("df::weight",w);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,0,w);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }  
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;

#ifdef USING_WIDGETS
  cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Weight"));
  cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Weight"));
#endif

}

void weight_commandEx(int *cstate,HighLevelEvent *event,void **wt)
{
  static int laststate;
  static EntityList selectionlist;
  //Dialog dialog("Weight_dialog");
  Dialog dialog("MaskChange_dialog");
  DialogControl *dc;
  EntityList list,elist;
  Entity *e;
  BitMask change(32);
  double weight;
  int w,status,iweight;
  ResourceString rs37(NCMAIN+37);

  if (*cstate == InitialState || *cstate == 1000 || *cstate == 1001)
    {  change.set(3);
       if (*cstate == 1001)
       {
           weight = *((double *)wt);
           // store the weight for the dialog to get
           v.setreal("ss::weight",weight);
       }
       else
       {
           //qDebug() << "weight command : df::weight : " << v.getinteger("df::weight");
           //qDebug() << "weight command : def weight : " << db.lineweights.weighttomm(v.getinteger("df::weight"));
           weight = db.lineweights.weighttomm(v.getinteger("df::weight"));
           // store the weight for the dialog to get
           v.setreal("ss::weight",weight);
       }
       iweight = db.lineweights.mmtoweight(weight);
       if (! state.getselection().empty())
       //dialog.title(rs37.gets());
       //dialog.add(dc = new WeightAssocRealDialogControl(100,&weight));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       dialog.add((WeightDialogControl *)(dc = new WeightDialogControl(1121,0,0,db.lineweights.numweights(),1,8,iweight)));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       if (event != 0)
         dialog.add(new ButtonDialogControl(107));
       dialog.setposition(DPTR);

       if ((status = dialog.process()) != FALSE)
         {  if (status == 107)
              {  laststate = *cstate;
                 selectionlist = state.getselection().copy();
                 state.destroyselection(0);
                 *cstate = 1;
                 cadwindow->prompt->normalprompt("Select the entity to clone the weight from");
              }
            else
              {
                 if (*cstate == 1000 || *cstate == 1001)
                 {
                     weight = v.getreal("ss::weight");
                     *((double *)wt) = weight;
                 }
                 else if (state.getselection().empty())
                   {
                     //qDebug() << "weight command : ss::weight : " << v.getreal("ss::weight");
                     //qDebug() << "weight command : mmtoweight : " << db.lineweights.mmtoweight(weight);
                      weight = v.getreal("ss::weight");
                      db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),db.lineweights.mmtoweight(weight));
                      v.setinteger("df::weight",db.lineweights.mmtoweight(weight));
                      cadwindow->updatetitle();
                   }
                 else
                   {
                     weight = v.getreal("ss::weight");
                     list = state.getselection().copy();
                      w = db.lineweights.mmtoweight(weight);
                      db.saveundo(UD_STARTBLOCK,NULL);
                      RemoveLockedEntities(&list,0);
                      for (list.start() ; (e = list.next()) != NULL ; )
                        {  if (e->isa(figure_entity))
                             cadwindow->invalidatedisplaylist(e);
                           e->change(change,0,0,0,w);
                        }
                      db.saveundo(UD_ENDBLOCK,NULL);
                      list.destroy();
                   }
                 *cstate = ExitState;
              }
         }
       else
         *cstate = ExitState;
    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(3);
            w = ((EntitySelectedEvent *) event)->getentity()->getweight();
            if (laststate == 1000)
              *((int *)wt) = w;
            else if (selectionlist.empty())
              {  db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),w);
                 v.setinteger("df::weight",w);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,0,0,0,w);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;

#ifdef USING_WIDGETS
  if(*cstate == ExitState)
  {
    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Weight"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Weight"));
  }
#endif

}

class TBButtonDialogControl : public ButtonDialogControl
  {public:
     TBButtonDialogControl(int i) : ButtonDialogControl(i) {};
     int select(Dialog *);
  };

int TBButtonDialogControl::select(Dialog *dialog)
{
    MenuWindow *mw;
    ButtonMenu *bm;
    RCCHAR text[300];
    int i;

    if (id == 102)
    {//  Add this to the visible menu list.
        ((ListDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,text,300);
        if (strlen(text) > 0)
        {
            ((ListDialogControl *)dialog->getcontrol(100))->removeitem(dialog,text);
            ((ListDialogControl *)dialog->getcontrol(101))->removeitem(dialog,text);  //  Delete the item in case it is already there
            ((ListDialogControl *)dialog->getcontrol(101))->additem(dialog,text);
        }
    }
    else if (id == 103)
    {//  Add this to the invisible menu list.
        ((ListDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,text,300);
        if (strlen(text) > 0)
        {
            ((ListDialogControl *)dialog->getcontrol(101))->removeitem(dialog,text);
            ((ListDialogControl *)dialog->getcontrol(100))->removeitem(dialog,text); //  Delete the item in case it is already there
            ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,text);
        }
    }
    else if (id == 104)
    {
        QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

        for (i = 0 ; ((ListDialogControl *)dialog->getcontrol(100))->getitem(dialog,i,text,300) != 0 ; i++)
        {
            if (_stricmp(text,"Assistant") == 0 && cadwindow->IsVisible(bm = buttonmenus.match("Assistant")))
            {
                cadwindow->buttonmenu(BM_MAININTERRUPT,0);
                cadwindow->buttonmenu(BM_SUBINTERRUPT,0);
                cadwindow->delcontrol(bm);
                bm->getQW()->hide();
                settings.sync();
                settings.remove("Layout/MainInterrupt");
            }
            else if (_stricmp(text,"Entities") == 0 && cadwindow->IsVisible((bm = buttonmenus.match("Entities"))))
            {
                cadwindow->buttonmenu(BM_MAINCOMMAND,0);
                cadwindow->buttonmenu(BM_SUBCOMMAND,0);
                cadwindow->delcontrol(bm);
                bm->getQW()->hide();
                settings.sync();
                settings.remove("Layout/MainMenu");
            }
            else if (_stricmp(text,"Quick") == 0 && cadwindow->IsVisible((bm = buttonmenus.match("Quick"))))
            {
                cadwindow->buttonmenu(BM_QUICK,0);
                cadwindow->delcontrol(bm);
                bm->getQW()->hide();
                settings.sync();
                settings.remove("Layout/Quick");
            }
            else if (_stricmp(text,"Coordinate Entry") == 0 && cadwindow->IsVisible(cadwindow->coordinateentry))
            {
                cadwindow->coordinateentry->getQW()->hide();
                cadwindow->delcontrol(cadwindow->coordinateentry);
                //cadwindow->coordinateentry->parent = 0;
                //cadwindow->coordinateentry->hDC = 0;
                //delete cadwindow->coordinateentry;
                //cadwindow->coordinateentry = 0;
                settings.sync();
                settings.remove("Layout/CoordinateEntry");

            }
            else if (_stricmp(text,"Style Bar") == 0 && cadwindow->IsVisible(cadwindow->styleentry))
            {
                // the style entry is never deleted just hidden
                ((QToolBar*)cadwindow->styleentry->getQW())->hide();
                cadwindow->delcontrol(cadwindow->styleentry);
                settings.sync();
                settings.remove("Layout/StyleEntry");

            }
            else if (_stricmp(text,"Coordinate Display") == 0 && cadwindow->IsVisible(cadwindow->coordinatedisplay))
            {
                cadwindow->coordinatedisplay->getQW()->hide();
                cadwindow->delcontrol(cadwindow->coordinatedisplay);
                //cadwindow->coordinatedisplay->parent = 0;
                //cadwindow->coordinatedisplay->hDC = 0;
                //delete cadwindow->coordinatedisplay;
                //cadwindow->coordinatedisplay = 0;
                settings.sync();
                settings.remove("Layout/CoordinateDisplay");
            }
            else if (_stricmp(text,"Prompt") == 0 && cadwindow->IsVisible(cadwindow->prompt))
            {
                // the prompt is never deleted just hidden
                ((QToolBar*)cadwindow->prompt->getQW()->parent())->hide();
                cadwindow->delcontrol(cadwindow->prompt);
                settings.sync();
                settings.remove("Layout/Prompt");
            }
            else if (_stricmp(text,"Click Status") == 0 && cadwindow->IsVisible(cadwindow->statusprompt))
            {
                // the status is never deleted just hidden
                ((QToolBar*)cadwindow->statusprompt->getQW()->parent())->hide();
                cadwindow->delcontrol(cadwindow->statusprompt);
                settings.sync();
                settings.remove("Layout/StatusPrompt");
            }
            else if ((bm = buttonmenus.match(text)) != 0 && cadwindow->IsVisible(bm))
            {//  Create a window and add the menu to it.
                /*
              bm->parent = 0;
              mw = new MenuWindow(cadwindow->getsubwindow(),410,40,bm->w,bm->h);
              bm->parent = 0;
              bm->move(mw,0,0,-1);
              InvalidateRect(mw->gethwnd(),0,TRUE);
              */

                // KMJ: ToDo test wit application menus
                cadwindow->buttonmenu(BM_MAINAPPLICATION,0);
                cadwindow->delcontrol(bm);
                bm->getQW()->hide();
                settings.sync();
                settings.remove("Layout/MainApplcation");

            }
        }
        //
        for (i = 0 ; ((ListDialogControl *)dialog->getcontrol(101))->getitem(dialog,i,text,300) != 0 ; i++)
        {
            if (_stricmp(text,"Assistant") == 0 && ! cadwindow->IsVisible(bm = buttonmenus.match("Assistant")))
            {
                cadwindow->buttonmenu(BM_MAININTERRUPT,0);
                cadwindow->buttonmenu(BM_SUBINTERRUPT,0);
                cadwindow->SetInterruptCADMenu(bm);
                bm->move(cadwindow,0,0,-1);
                //bm->move(cadwindow,0,0,-1);
                bm->getQW()->show();
                //((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->deselect();
                //((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->select();
                cadwindow->updatemenu();
                settings.sync();
                ((RCToolBox*)bm->getQW())->saveState();
            }
            else if (_stricmp(text,"Entities") == 0 && ! cadwindow->IsVisible((bm = buttonmenus.match("Entities"))))
            {
                cadwindow->buttonmenu(BM_MAINCOMMAND,0);
                cadwindow->buttonmenu(BM_SUBCOMMAND,0);
                cadwindow->SetMainCADMenu(bm);
                bm->move(cadwindow,0,0,-1);
                bm->getQW()->show();
                //((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->deselect();
                //((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->select();
                cadwindow->updatemenu();
                settings.sync();
                ((RCToolBox*)bm->getQW())->saveState();
            }
            else if (_stricmp(text,"Quick") == 0 && ! cadwindow->IsVisible((bm = buttonmenus.match("Quick"))))
            {
                cadwindow->buttonmenu(BM_QUICK,0);
                cadwindow->SetQuickCADMenu(bm);
                bm->move(cadwindow,0,0,-1);
                bm->getQW()->show();
                settings.sync();
                ((RCToolBox*)bm->getQW())->saveState();
            }
            else if (_stricmp(text,"Coordinate Entry") == 0 && ! cadwindow->IsVisible(cadwindow->coordinateentry))
            {
                SIZE size;
                TEXTMETRIC tm;
                GetTextMetrics(cadwindow->gethdc(),&tm);
                GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
                int coordheight = tm.tmHeight * 3 / 2 + 6;

                if(cadwindow->coordinateentry == 0)
                {
                    cadwindow->coordinateentry = new CoordinateEntry(cadwindow->getsubwindow(),400,30,size.cx * 3);
                    cadwindow->coordinateentry->setcsystem(0);
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setWindowTitle(app->tr("Coordinate Entry"));
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setFloatable(true);
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setMinimumHeight(coordheight+6);
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setMaximumHeight(coordheight+6);
                    ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinateentry->getQW());
                    ((QToolBar*)cadwindow->coordinateentry->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
                }

                cadwindow->addcontrol(cadwindow->coordinateentry);
                ((QToolBar*)cadwindow->coordinateentry->getQW())->show();

                settings.setValue("Layout/CoordinateEntry",((QToolBar*)cadwindow->coordinateentry->getQW())->saveGeometry());
            }
            else if (_stricmp(text,"Style Bar") == 0 && ! cadwindow->IsVisible(cadwindow->styleentry))
            {
                SIZE size;
                TEXTMETRIC tm;
                GetTextMetrics(cadwindow->gethdc(),&tm);
                GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
                int coordheight = tm.tmHeight * 3 / 2 + 6;

                if(cadwindow->styleentry == 0)
                {
                    cadwindow->styleentry = new StyleEntry(cadwindow->getsubwindow(),460,60,605);
                    ((QToolBar*)cadwindow->styleentry->getQW())->setWindowTitle(app->tr("Style Bar"));
                    ((QToolBar*)cadwindow->styleentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ((QToolBar*)cadwindow->styleentry->getQW())->setFloatable(true);
                    ((QToolBar*)cadwindow->styleentry->getQW())->setMovable(true);
                    //((QToolBar*)styleentry->getQW())->setMinimumHeight(coordheight+6);
                    //((QToolBar*)styleentry->getQW())->setMaximumHeight(coordheight+6);
                    ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::BottomToolBarArea,(QToolBar*)cadwindow->styleentry->getQW());
                    ((QToolBar*)cadwindow->styleentry->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
                }

                cadwindow->addcontrol(cadwindow->styleentry);
                ((QToolBar*)cadwindow->styleentry->getQW())->show();

                settings.setValue("Layout/StyleEntry",((QToolBar*)cadwindow->styleentry->getQW())->saveGeometry());

            }
            else if (_stricmp(text,"Coordinate Display") == 0 && ! cadwindow->IsVisible(cadwindow->coordinatedisplay))
            {
                //  Create a window and add the menu to it.
                SIZE size;
                TEXTMETRIC tm;
                GetTextMetrics(cadwindow->gethdc(),&tm);
                GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
                int coordheight = tm.tmHeight * 3 / 2 + 6;

                if(cadwindow->coordinatedisplay == 0)
                {
                    cadwindow->coordinatedisplay = new CoordinateDisplay(cadwindow->getsubwindow(),410,60,size.cx * 3);
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setWindowTitle(app->tr("Coordinate Display"));
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setFloatable(true);
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setMinimumHeight(coordheight+6);
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setMaximumHeight(coordheight+6);
                    ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinatedisplay->getQW());
                    ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
                }

                cadwindow->addcontrol(cadwindow->coordinatedisplay);
                ((QToolBar*)cadwindow->coordinatedisplay->getQW())->show();

                settings.setValue("Layout/CoordinateDisplay",((QToolBar*)cadwindow->coordinatedisplay->getQW())->saveGeometry());
            }
            else if (_stricmp(text,"Prompt") == 0 && ! cadwindow->IsVisible(cadwindow->prompt))
            {
                SIZE size;
                TEXTMETRIC tm;
                GetTextMetrics(cadwindow->gethdc(),&tm);
                GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
                int iconheight = 64;
                int coordheight = tm.tmHeight * 3 / 2 + 6;

                QToolBar * promptToolbar = 0;
                if(cadwindow->prompt == 0)
                {
                    QToolBar * promptToolbar = new QToolBar("PromptToolBar",(QtMainWindow*)cadwindow->gethwnd());
                    promptToolbar->setObjectName("PromptToolBar");
                    promptToolbar->setMinimumHeight(coordheight+6);
                    promptToolbar->setMaximumHeight(coordheight+6);
                    promptToolbar->setMinimumWidth(500);
                    promptToolbar->setMaximumWidth(500);
                    promptToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    promptToolbar->setFloatable(true);
                    promptToolbar->setMovable(true);

                    cadwindow->prompt = new Prompt(12,iconheight + coordheight + 8,440,promptToolbar);

                    ((QLabel*)cadwindow->prompt->getQW())->setObjectName("PromptLabel");
                    ((QLabel*)cadwindow->prompt->getQW())->setMinimumHeight(coordheight-6);
                    ((QLabel*)cadwindow->prompt->getQW())->setMaximumHeight(coordheight-6);
                    ((QLabel*)cadwindow->prompt->getQW())->setMinimumWidth(440);
                    ((QLabel*)cadwindow->prompt->getQW())->setMaximumWidth(440);
                    ((QLabel*)cadwindow->prompt->getQW())->setText("Prompt");
                    ((QLabel*)cadwindow->prompt->getQW())->setTextFormat(Qt::AutoText);
                    ((QLabel*)cadwindow->prompt->getQW())->setAutoFillBackground(true);
                    ((QLabel*)cadwindow->prompt->getQW())->setBackgroundRole(QPalette::Background);
                    QPalette PromptPalette = ((QLabel*)cadwindow->prompt->getQW())->palette();
                    PromptPalette.setColor(QPalette::Window,QColor(255,255,255));
                    PromptPalette.setColor(QPalette::WindowText,QColor(0,0,0));
                    ((QLabel*)cadwindow->prompt->getQW())->setPalette(PromptPalette);

                    promptToolbar->setMinimumWidth(((QLabel*)cadwindow->prompt->getQW())->width()+20);
                    promptToolbar->setMaximumWidth(((QLabel*)cadwindow->prompt->getQW())->width()+20);

                    promptToolbar->addWidget((QLabel*)cadwindow->prompt->getQW());

                    ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,promptToolbar);
                    promptToolbar->setParent((QtMainWindow*)cadwindow->gethwnd());
                }
                else
                    promptToolbar = (QToolBar*)cadwindow->prompt->getQW()->parent();

                cadwindow->prompt->fixup(cadwindow);
                cadwindow->prompt->normalprompt(NCWIN+1);
                cadwindow->prompt->setcolour(RGB(0,255,0));

                cadwindow->addcontrol(cadwindow->prompt);

                if(promptToolbar != 0)
                {
                    promptToolbar->show();
                    settings.setValue("Layout/Prompt",promptToolbar->saveGeometry());
                }
            }
            else if (_stricmp(text,"Click Status") == 0 && ! cadwindow->IsVisible(cadwindow->statusprompt))
            {
                SIZE size;
                TEXTMETRIC tm;
                GetTextMetrics(cadwindow->gethdc(),&tm);
                GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
                int iconheight = 64;
                int coordheight = tm.tmHeight * 3 / 2 + 6;

                QToolBar * statusToolbar = 0;
                if(cadwindow->statusprompt == 0)
                {
                    QToolBar * statusToolbar = new QToolBar("StatusToolBar",(QtMainWindow*)cadwindow->gethwnd());
                    statusToolbar->setObjectName("StatusToolBar");
                    statusToolbar->setWindowTitle(app->tr("Status"));
                    statusToolbar->setMinimumHeight(coordheight+6);
                    statusToolbar->setMaximumHeight(coordheight+6);
                    statusToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    statusToolbar->setFloatable(true);
                    statusToolbar->setMovable(true);

                    cadwindow->statusprompt = new Prompt(12 + size.cx * 4 + 20,iconheight + coordheight + 8,size.cx,statusToolbar);

                    ((QLabel*)cadwindow->statusprompt->getQW())->setObjectName("StatusLabel");
                    ((QLabel*)cadwindow->statusprompt->getQW())->setMinimumHeight(coordheight-6);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setMaximumHeight(coordheight-6);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setMinimumWidth(size.cx);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setMaximumWidth(220);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setText("Status");
                    ((QLabel*)cadwindow->statusprompt->getQW())->setTextFormat(Qt::AutoText);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setAutoFillBackground(true);
                    ((QLabel*)cadwindow->statusprompt->getQW())->setBackgroundRole(QPalette::Background);
                    QPalette StatusPalette = ((QLabel*)cadwindow->statusprompt->getQW())->palette();
                    StatusPalette.setColor(QPalette::Window,QColor(255,255,255));
                    StatusPalette.setColor(QPalette::WindowText,QColor(0,0,0));
                    ((QLabel*)cadwindow->statusprompt->getQW())->setPalette(StatusPalette);

                    statusToolbar->setMinimumWidth(((QLabel*)cadwindow->statusprompt->getQW())->width()+20);
                    statusToolbar->setMaximumWidth(((QLabel*)cadwindow->statusprompt->getQW())->width()+20);

                    statusToolbar->addWidget((QLabel*)cadwindow->statusprompt->getQW());

                    ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,statusToolbar);
                    statusToolbar->setParent((QtMainWindow*)cadwindow->gethwnd());
                }
                else
                    statusToolbar = (QToolBar*)cadwindow->statusprompt->getQW()->parent();

                cadwindow->statusprompt->fixup(cadwindow);
                cadwindow->statusprompt->normalprompt(_RCT(""));
                cadwindow->statusprompt->setcolour(RGB(255,0,0));

                cadwindow->addcontrol(cadwindow->statusprompt);
                statusToolbar->show();

                settings.setValue("Layout/StatusPrompt",statusToolbar->saveGeometry());
            }
            else if ((bm = buttonmenus.match(text)) != 0 && ! cadwindow->IsVisible(bm))
            {
                //  Create a window and add the menu to it.
                /*
                 bm->parent = 0;
                 mw = new MenuWindow(cadwindow->getsubwindow(),410,40,bm->w,bm->h);
                 bm->parent = 0;
                 bm->move(mw,0,0,-1);
                 InvalidateRect(mw->gethwnd(),0,TRUE);
                 */

                // KMJ: ToDo test with application menus
                cadwindow->buttonmenu(BM_MAINAPPLICATION,bm);
                bm->move(cadwindow,0,0,-1);
                bm->getQW()->show();
                settings.sync();
                ((RCToolBox*)bm->getQW())->saveState();

            }
        }
        return IDOK;
    }
    else if (id == 105)
    {
        // reset
        QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

        // hide everything first
        //if ( cadwindow->IsVisible((bm = buttonmenus.match("Entities"))))
        {
            bm = buttonmenus.match("Entities");
            cadwindow->buttonmenu(BM_MAINCOMMAND,0);
            cadwindow->buttonmenu(BM_SUBCOMMAND,0);
            cadwindow->delcontrol(bm);
            // remove the dock widget instead of hide
            if( ! ((QDockWidget*)bm->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeDockWidget((QDockWidget*)bm->getQW());
            else
                bm->getQW()->hide();
            settings.sync();
            settings.remove("Layout/MainMenu");
        }
        //if (cadwindow->IsVisible(bm = buttonmenus.match("Assistant")))
        {
            bm = buttonmenus.match("Assistant");
            cadwindow->buttonmenu(BM_MAININTERRUPT,0);
            cadwindow->buttonmenu(BM_SUBINTERRUPT,0);
            cadwindow->delcontrol(bm);
            if( ! ((QDockWidget*)bm->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeDockWidget((QDockWidget*)bm->getQW());
            else
                bm->getQW()->hide();
            settings.sync();
            settings.remove("Layout/MainInterrupt");
        }
        //if (cadwindow->IsVisible((bm = buttonmenus.match("Quick"))))
        {
            bm = buttonmenus.match("Quick");
            cadwindow->buttonmenu(BM_QUICK,0);
            cadwindow->delcontrol(bm);
            if( ! ((QDockWidget*)bm->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeDockWidget((QDockWidget*)bm->getQW());
            else
                bm->getQW()->hide();
            settings.sync();
            settings.remove("Layout/Quick");
        }
        //if ( cadwindow->IsVisible(cadwindow->coordinateentry))
        {
            if( ! ((QToolBar*)cadwindow->coordinateentry->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeToolBar((QToolBar*)cadwindow->coordinateentry->getQW());
            else
                cadwindow->coordinateentry->getQW()->hide();
            cadwindow->delcontrol(cadwindow->coordinateentry);
            //cadwindow->coordinateentry->parent = 0;
            //cadwindow->coordinateentry->hDC = 0;
            //delete cadwindow->coordinateentry;
            //cadwindow->coordinateentry = 0;
            settings.sync();
            settings.remove("Layout/CoordinateEntry");

        }
        //if (cadwindow->IsVisible(cadwindow->styleentry))
        {
            // the style entry is never deleted just hidden
            if( ! ((QToolBar*)cadwindow->styleentry->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeToolBar((QToolBar*)cadwindow->styleentry->getQW());
            else
                ((QToolBar*)cadwindow->styleentry->getQW())->hide();
            cadwindow->delcontrol(cadwindow->styleentry);
            settings.sync();
            settings.remove("Layout/StyleEntry");

        }
        //if (cadwindow->IsVisible(cadwindow->coordinatedisplay))
        {
            if( ! ((QToolBar*)cadwindow->coordinatedisplay->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeToolBar((QToolBar*)cadwindow->coordinatedisplay->getQW());
            else
                cadwindow->coordinatedisplay->getQW()->hide();
            cadwindow->delcontrol(cadwindow->coordinatedisplay);
            //cadwindow->coordinatedisplay->parent = 0;
            //cadwindow->coordinatedisplay->hDC = 0;
            //delete cadwindow->coordinatedisplay;
            //cadwindow->coordinatedisplay = 0;
            settings.sync();
            settings.remove("Layout/CoordinateDisplay");
        }
        //if ( cadwindow->IsVisible(cadwindow->prompt))
        {
            // the prompt is never deleted just hidden
            if( ! ((QToolBar*)cadwindow->prompt->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeToolBar((QToolBar*)cadwindow->prompt->getQW()->parent());
            else
                ((QToolBar*)cadwindow->prompt->getQW()->parent())->hide();
            cadwindow->delcontrol(cadwindow->prompt);
            settings.sync();
            settings.remove("Layout/Prompt");
        }
        //if ( cadwindow->IsVisible(cadwindow->statusprompt))
        {
            // the status is never deleted just hidden
            if( ! ((QToolBar*)cadwindow->statusprompt->getQW())->isFloating())
                ((QtMainWindow*)cadwindow->gethwnd())->removeToolBar((QToolBar*)cadwindow->statusprompt->getQW()->parent());
            else
                ((QToolBar*)cadwindow->statusprompt->getQW()->parent())->hide();
            cadwindow->delcontrol(cadwindow->statusprompt);
            settings.sync();
            settings.remove("Layout/StatusPrompt");
        }
        // show the default layout
        //if (! cadwindow->IsVisible((bm = buttonmenus.match("Entities"))))
        {
            bm = buttonmenus.match("Entities");
            cadwindow->buttonmenu(BM_MAINCOMMAND,0);
            cadwindow->buttonmenu(BM_SUBCOMMAND,0);
            cadwindow->SetMainCADMenu(bm);
            bm->move(cadwindow,0,0,-1);
            //((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->deselect();
            //((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->select();
            // put the dock widget back
            ((RCToolBox*)bm->getQW())->setFloating(false);
            ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)bm->getQW());
            bm->getQW()->show();
            cadwindow->updatemenu();
            settings.sync();
            ((RCToolBox*)bm->getQW())->saveState();
        }
        //if (! cadwindow->IsVisible(bm = buttonmenus.match("Assistant")))
        {
            bm = buttonmenus.match("Assistant");
            cadwindow->buttonmenu(BM_MAININTERRUPT,0);
            cadwindow->buttonmenu(BM_SUBINTERRUPT,0);
            cadwindow->SetInterruptCADMenu(bm);
            bm->move(cadwindow,0,0,-1);
            // put the dock widget back
            ((RCToolBox*)bm->getQW())->setFloating(false);
            ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(RCToolBox*)bm->getQW());
            bm->getQW()->show();
            //((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->deselect();
            //((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->select();
            cadwindow->updatemenu();
            settings.sync();
            ((RCToolBox*)bm->getQW())->saveState();
        }
        //if (! cadwindow->IsVisible((bm = buttonmenus.match("Quick"))))
        {
            bm = buttonmenus.match("Quick");
            cadwindow->buttonmenu(BM_QUICK,0);
            cadwindow->SetQuickCADMenu(bm);
            bm->move(cadwindow,0,0,-1);
            // put the dock widget back
            ((RCToolBox*)bm->getQW())->setFloating(false);
            ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(RCToolBox*)bm->getQW());
            bm->getQW()->show();
            settings.sync();
            ((RCToolBox*)bm->getQW())->saveState();
        }
        //if ( ! cadwindow->IsVisible(cadwindow->prompt))
        {
            SIZE size;
            TEXTMETRIC tm;
            GetTextMetrics(cadwindow->gethdc(),&tm);
            GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
            int iconheight = 64;
            int coordheight = tm.tmHeight * 3 / 2 + 6;

            QToolBar * promptToolbar = 0;
            if(cadwindow->prompt == 0)
            {
                QToolBar * promptToolbar = new QToolBar("PromptToolBar",(QtMainWindow*)cadwindow->gethwnd());
                promptToolbar->setObjectName("PromptToolBar");
                promptToolbar->setMinimumHeight(coordheight+6);
                promptToolbar->setMaximumHeight(coordheight+6);
                promptToolbar->setMinimumWidth(500);
                promptToolbar->setMaximumWidth(500);
                promptToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                promptToolbar->setFloatable(true);
                promptToolbar->setMovable(true);

                cadwindow->prompt = new Prompt(12,iconheight + coordheight + 8,440,promptToolbar);

                ((QLabel*)cadwindow->prompt->getQW())->setObjectName("PromptLabel");
                ((QLabel*)cadwindow->prompt->getQW())->setMinimumHeight(coordheight-6);
                ((QLabel*)cadwindow->prompt->getQW())->setMaximumHeight(coordheight-6);
                ((QLabel*)cadwindow->prompt->getQW())->setMinimumWidth(440);
                ((QLabel*)cadwindow->prompt->getQW())->setMaximumWidth(440);
                ((QLabel*)cadwindow->prompt->getQW())->setText("Prompt");
                ((QLabel*)cadwindow->prompt->getQW())->setTextFormat(Qt::AutoText);
                ((QLabel*)cadwindow->prompt->getQW())->setAutoFillBackground(true);
                ((QLabel*)cadwindow->prompt->getQW())->setBackgroundRole(QPalette::Background);
                QPalette PromptPalette = ((QLabel*)cadwindow->prompt->getQW())->palette();
                PromptPalette.setColor(QPalette::Window,QColor(255,255,255));
                PromptPalette.setColor(QPalette::WindowText,QColor(0,0,0));
                ((QLabel*)cadwindow->prompt->getQW())->setPalette(PromptPalette);

                promptToolbar->setMinimumWidth(((QLabel*)cadwindow->prompt->getQW())->width()+20);
                promptToolbar->setMaximumWidth(((QLabel*)cadwindow->prompt->getQW())->width()+20);

                promptToolbar->addWidget((QLabel*)cadwindow->prompt->getQW());

                ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,promptToolbar);
                promptToolbar->setParent((QtMainWindow*)cadwindow->gethwnd());
            }
            else
                promptToolbar = (QToolBar*)cadwindow->prompt->getQW()->parent();

            cadwindow->prompt->fixup(cadwindow);
            cadwindow->prompt->normalprompt(NCWIN+1);
            cadwindow->prompt->setcolour(RGB(0,255,0));

            ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,promptToolbar);
            cadwindow->addcontrol(cadwindow->prompt);

            if(promptToolbar != 0)
            {
                promptToolbar->show();
                settings.setValue("Layout/Prompt",promptToolbar->saveGeometry());
            }
        }
        // this is not shown by default
        /*
          if ( ! cadwindow->IsVisible(cadwindow->statusprompt))
          {
               SIZE size;
               TEXTMETRIC tm;
               GetTextMetrics(cadwindow->gethdc(),&tm);
               GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
               int iconheight = 64;
               int coordheight = tm.tmHeight * 3 / 2 + 6;

               QToolBar * statusToolbar = 0;
               if(cadwindow->statusprompt == 0)
               {
                   QToolBar * statusToolbar = new QToolBar("StatusToolBar",(QtMainWindow*)cadwindow->gethwnd());
                   statusToolbar->setObjectName("StatusToolBar");
                   statusToolbar->setWindowTitle(app->tr("Status"));
                   statusToolbar->setMinimumHeight(coordheight+6);
                   statusToolbar->setMaximumHeight(coordheight+6);
                   statusToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                   statusToolbar->setFloatable(true);
                   statusToolbar->setMovable(true);

                   cadwindow->statusprompt = new Prompt(12 + size.cx * 4 + 20,iconheight + coordheight + 8,size.cx,statusToolbar);

                   ((QLabel*)cadwindow->statusprompt->getQW())->setObjectName("StatusLabel");
                   ((QLabel*)cadwindow->statusprompt->getQW())->setMinimumHeight(coordheight-6);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setMaximumHeight(coordheight-6);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setMinimumWidth(size.cx);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setMaximumWidth(220);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setText("Status");
                   ((QLabel*)cadwindow->statusprompt->getQW())->setTextFormat(Qt::AutoText);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setAutoFillBackground(true);
                   ((QLabel*)cadwindow->statusprompt->getQW())->setBackgroundRole(QPalette::Background);
                   QPalette StatusPalette = ((QLabel*)cadwindow->statusprompt->getQW())->palette();
                   StatusPalette.setColor(QPalette::Window,QColor(255,255,255));
                   StatusPalette.setColor(QPalette::WindowText,QColor(0,0,0));
                   ((QLabel*)cadwindow->statusprompt->getQW())->setPalette(StatusPalette);

                   statusToolbar->setMinimumWidth(((QLabel*)cadwindow->statusprompt->getQW())->width()+20);
                   statusToolbar->setMaximumWidth(((QLabel*)cadwindow->statusprompt->getQW())->width()+20);

                   statusToolbar->addWidget((QLabel*)cadwindow->statusprompt->getQW());

                   ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,statusToolbar);
                   statusToolbar->setParent((QtMainWindow*)cadwindow->gethwnd());
               }
               else
                   statusToolbar = (QToolBar*)cadwindow->statusprompt->getQW()->parent();

               cadwindow->statusprompt->fixup(cadwindow);
               cadwindow->statusprompt->normalprompt(_RCT(""));
               cadwindow->statusprompt->setcolour(RGB(255,0,0));

               ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::BottomToolBarArea,(QToolBar*)cadwindow->statusprompt->getQW());
               cadwindow->addcontrol(cadwindow->statusprompt);
               statusToolbar->show();

               settings.setValue("Layout/StatusPrompt",statusToolbar->saveGeometry());
          }
          */
        //if ( ! cadwindow->IsVisible(cadwindow->coordinateentry))
        {
            SIZE size;
            TEXTMETRIC tm;
            GetTextMetrics(cadwindow->gethdc(),&tm);
            GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
            int coordheight = tm.tmHeight * 3 / 2 + 6;

            if(cadwindow->coordinateentry == 0)
            {
                cadwindow->coordinateentry = new CoordinateEntry(cadwindow->getsubwindow(),400,30,size.cx * 3);
                cadwindow->coordinateentry->setcsystem(0);
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setWindowTitle(app->tr("Coordinate Entry"));
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setFloatable(true);
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setMinimumHeight(coordheight+6);
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setMaximumHeight(coordheight+6);
                ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinateentry->getQW());
                ((QToolBar*)cadwindow->coordinateentry->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
            }

            ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinateentry->getQW());
            cadwindow->addcontrol(cadwindow->coordinateentry);
            ((QToolBar*)cadwindow->coordinateentry->getQW())->show();

            settings.setValue("Layout/CoordinateEntry",((QToolBar*)cadwindow->coordinateentry->getQW())->saveGeometry());
        }
        // this is not show by default
        /*
          if ( ! cadwindow->IsVisible(cadwindow->coordinatedisplay))
          {
               //  Create a window and add the menu to it.
               SIZE size;
               TEXTMETRIC tm;
               GetTextMetrics(cadwindow->gethdc(),&tm);
               GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
               int coordheight = tm.tmHeight * 3 / 2 + 6;

               if(cadwindow->coordinatedisplay == 0)
               {
                   cadwindow->coordinatedisplay = new CoordinateDisplay(cadwindow->getsubwindow(),410,60,size.cx * 3);
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setWindowTitle(app->tr("Coordinate Display"));
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setFloatable(true);
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setMinimumHeight(coordheight+6);
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setMaximumHeight(coordheight+6);
                   ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinatedisplay->getQW());
                   ((QToolBar*)cadwindow->coordinatedisplay->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
               }

               ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::TopToolBarArea,(QToolBar*)cadwindow->coordinatedisplay->getQW());
               cadwindow->addcontrol(cadwindow->coordinatedisplay);
               ((QToolBar*)cadwindow->coordinatedisplay->getQW())->show();

               settings.setValue("Layout/CoordinateDisplay",((QToolBar*)cadwindow->coordinatedisplay->getQW())->saveGeometry());
          }
          */
        //if ( ! cadwindow->IsVisible(cadwindow->styleentry))
        {
            SIZE size;
            TEXTMETRIC tm;
            GetTextMetrics(cadwindow->gethdc(),&tm);
            GetTextExtentPoint32(cadwindow->gethdc(),_RCT("X 1.2345678901234567"),20,&size);
            int coordheight = tm.tmHeight * 3 / 2 + 6;

            if(cadwindow->styleentry == 0)
            {
                cadwindow->styleentry = new StyleEntry(cadwindow->getsubwindow(),460,60,605);
                ((QToolBar*)cadwindow->styleentry->getQW())->setWindowTitle(app->tr("Style Bar"));
                ((QToolBar*)cadwindow->styleentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                ((QToolBar*)cadwindow->styleentry->getQW())->setFloatable(true);
                //((QToolBar*)styleentry->getQW())->setMinimumHeight(coordheight+6);
                //((QToolBar*)styleentry->getQW())->setMaximumHeight(coordheight+6);
                ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::BottomToolBarArea,(QToolBar*)cadwindow->styleentry->getQW());
                ((QToolBar*)cadwindow->styleentry->getQW())->setParent((QtMainWindow*)cadwindow->gethwnd());
            }

            ((QtMainWindow*)cadwindow->gethwnd())->addToolBar(Qt::BottomToolBarArea,(QToolBar*)cadwindow->styleentry->getQW());
            cadwindow->addcontrol(cadwindow->styleentry);
            ((QToolBar*)cadwindow->styleentry->getQW())->show();

            settings.setValue("Layout/StyleEntry",((QToolBar*)cadwindow->styleentry->getQW())->saveGeometry());

        }
        return IDOK;
    }
    return 0;
}

void toolmenu_command(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("ToolBars_Dialog");
 const char *cnames[] = {   "Entities", "Assistant","Quick",
                      "Coordinate Entry", "Coordinate Display",
                      "Prompt", "Click Status", "Style Bar",
                      "Animation", "Raytrace", "Composed Drawing",
                      "Transform"
                  };
 RCCHAR *names[12];
 RCCHAR *visiblenames[12];
 int nvisible,ninvisible;

 ninvisible = 0;
 //  Main CAD menus
 if (!cadwindow->IsVisible(buttonmenus.match("Entities")))
 {
     names[ninvisible++] = new RCCHAR[strlen("Entities")+1];
     strcpy(names[ninvisible-1],"Entities");
 }
 if (!cadwindow->IsVisible(buttonmenus.match("Assistant")))
 {
     names[ninvisible++] = new RCCHAR[strlen("Assistant")+1];
     strcpy(names[ninvisible-1],"Assistant");
 }
 if (!cadwindow->IsVisible(buttonmenus.match("Quick")))
 {
     names[ninvisible++] = new RCCHAR[strlen("Quick")+1];
     strcpy(names[ninvisible-1],"Quick");
 }
 //  Miscellaneous Controls

 if (!cadwindow->IsVisible(cadwindow->coordinateentry))
 {
     names[ninvisible++] = new RCCHAR[strlen("Coordinate Entry")+1];
     strcpy(names[ninvisible-1],"Coordinate Entry");
 }
 if (!cadwindow->IsVisible(cadwindow->coordinatedisplay))
 {
     names[ninvisible++] = new RCCHAR[strlen("Coordinate Display")+1];
     strcpy(names[ninvisible-1],"Coordinate Display");
 }
 if (!cadwindow->IsVisible(cadwindow->prompt))
 {
   names[ninvisible++] = new RCCHAR[strlen("Prompt")+1];
   strcpy(names[ninvisible-1],"Prompt");
 }
 if (!cadwindow->IsVisible(cadwindow->statusprompt))
 {
   names[ninvisible++] = new RCCHAR[strlen("Click Status")+1];
   strcpy(names[ninvisible-1],"Click Status");
 }
 if (!cadwindow->IsVisible(cadwindow->styleentry))
 {
   names[ninvisible++] = new RCCHAR[strlen("Style Bar")+1];
   strcpy(names[ninvisible-1],"Style Bar");
 }
#if 0
 //  Applications
 if (cadwindow->IsVisible(buttonmenus.match(_RCT("Animation"))))
   visiblenames[nvisible++] = _RCT("Animation");
 if (cadwindow->IsVisible(buttonmenus.match(_RCT("Raytrace"))))
   visiblenames[nvisible++] = _RCT("Raytrace");
 if (cadwindow->IsVisible(buttonmenus.match(_RCT("Composed Drawing"))))
   visiblenames[nvisible++] = _RCT("Composed Drawing");
 if (cadwindow->IsVisible(buttonmenus.match(_RCT("Transform"))))
   visiblenames[nvisible++] = _RCT("Transform");
#endif

 for(int i=0; i<ninvisible; i++)
     qDebug() << QString(names[i]);

  nvisible = 0;  
  //  Main CAD menus
  if (cadwindow->IsVisible(buttonmenus.match("Entities")))
  {
      visiblenames[nvisible++] = new RCCHAR[strlen("Entities")+1];
      strcpy(visiblenames[nvisible-1],"Entities");
  }
  if (cadwindow->IsVisible(buttonmenus.match("Assistant")))
  {
      visiblenames[nvisible++] = new RCCHAR[strlen("Assistant")+1];
      strcpy(visiblenames[nvisible-1],"Assistant");
  }
  if (cadwindow->IsVisible(buttonmenus.match("Quick")))
  {
      visiblenames[nvisible++] = new RCCHAR[strlen("Quick")+1];
      strcpy(visiblenames[nvisible-1],"Quick");
  }
  //  Miscellaneous Controls

  if (cadwindow->IsVisible(cadwindow->coordinateentry))
  {
      visiblenames[nvisible++] = new RCCHAR[strlen("Coordinate Entry")+1];
      strcpy(visiblenames[nvisible-1],"Coordinate Entry");
  }
  if (cadwindow->IsVisible(cadwindow->coordinatedisplay))
  {
      visiblenames[nvisible++] = new RCCHAR[strlen("Coordinate Display")+1];
      strcpy(visiblenames[nvisible-1],"Coordinate Display");
  }
  if (cadwindow->IsVisible(cadwindow->prompt))
  {
    visiblenames[nvisible++] = new RCCHAR[strlen("Prompt")+1];
    strcpy(visiblenames[nvisible-1],"Prompt");
  }
  if (cadwindow->IsVisible(cadwindow->statusprompt))
  {
    visiblenames[nvisible++] = new RCCHAR[strlen("Click Status")+1];
    strcpy(visiblenames[nvisible-1],"Click Status");
  }
  if (cadwindow->IsVisible(cadwindow->styleentry))
  {
    visiblenames[nvisible++] = new RCCHAR[strlen("Style Bar")+1];
    strcpy(visiblenames[nvisible-1],"Style Bar");
  }
#if 0
  //  Applications
  if (cadwindow->IsVisible(buttonmenus.match(_RCT("Animation"))))
    visiblenames[nvisible++] = _RCT("Animation");
  if (cadwindow->IsVisible(buttonmenus.match(_RCT("Raytrace"))))
    visiblenames[nvisible++] = _RCT("Raytrace");
  if (cadwindow->IsVisible(buttonmenus.match(_RCT("Composed Drawing"))))
    visiblenames[nvisible++] = _RCT("Composed Drawing");
  if (cadwindow->IsVisible(buttonmenus.match(_RCT("Transform"))))
    visiblenames[nvisible++] = _RCT("Transform");
#endif

  for(int i=0; i<nvisible; i++)
      qDebug() << QString(visiblenames[i]);

  dialog.add(new ListDialogControl(100,ninvisible,names));
  dialog.add(new ListDialogControl(101,nvisible,visiblenames));
  dialog.add(new TBButtonDialogControl(102));
  dialog.add(new TBButtonDialogControl(103));
  dialog.add(new TBButtonDialogControl(104));
  dialog.add(new TBButtonDialogControl(105));

  dialog.process();
  *cstate = ExitState;
}


void showproperties_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->viewproperties->restore_width();
  *cstate = ExitState;
}

void interruptmenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->movemenu(BM_MAININTERRUPT);
  *cstate = ExitState;
}

void quickmenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->movemenu(BM_QUICK);
  *cstate = ExitState;
}

void applicationmenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->movemenu(BM_MAINAPPLICATION);
  *cstate = ExitState;
}

void prompt_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->moveprompt();
  *cstate = ExitState;
}

/*
class SSListDialogControl : public ListDialogControl
  {private:
     RCCHAR name[300];
   public:
     SSListDialogControl(int id) : ListDialogControl(id,0,0,name,300)  {  name[0] = 0;  };
     void load(Dialog *);
  };
*/

void SSListDialogControl::load(Dialog *dialog)
{RCCHAR key[300],value[300],*s,name[300];
 int i;
 QStringList items;
  //this->sort();
  removeallitems(dialog);
  for (i = 1 ; ; i++)
    {  sprintf(key,"StyleSet%d",i);
#if ! defined(_WIN32_WCE)
       GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
#endif
       if (strlen(value) == 0)
         break;
       s = strtok(value,_RCT(","));
       if (s != 0) 
         {  strcpy(name,s);
            //additem(dialog,name);//fg.2022
			items.append(QString(name));
            s = strtok(0,_RCT(","));
            if (s != 0 && atoi(s) == db.header.getcolour())
              {  s = strtok(0,_RCT(","));
                 if (s != 0 && atoi(s) == db.header.getlayer())
                   {  s = strtok(0,_RCT(","));
                      if (s != 0 && atoi(s) == db.header.getstyle())
                        {  s = strtok(0,_RCT(","));
                           if (s != 0 && db.lineweights.mmtoweight(atof(s)) == db.header.getweight())
                             change(dialog,name); 
                        }
                   }
              }
         }
    }
  items.sort();
  for (QStringList::iterator it = items.begin(); it != items.end(); ++it)
	  additem(dialog, (RCCHAR*)(*it).unicode());
}

/*!
 * \fn void SSListDialogControl::sort()
 * \brief Unicode vale sorts ini file StyleSet names.
 *
 * Rearranges key-value pairs in the cad.ini file StyleSet section.
 * Uses insertion-sort process to alphabetise style names.
 *
 * \sa SSListDialogControl::compare(...)
 */
void SSListDialogControl::sort()
{
    RCCHAR key1[300],value1[300],*s1,name1[300], key2[300],value2[300],*s2,name2[300];
    int i, j;

    for (i = 2; ; i++) {
        j = i;
        sprintf(key1,"StyleSet%d",j);
        GetPrivateProfileString(_RCT("StyleSet"),key1,_RCT(""),value1,300,home.getinifilename());
        if (strlen(value1) == 0)
            break;
        while (j > 1) {
            sprintf(key1,"StyleSet%d",j);
            sprintf(key2,"StyleSet%d",j-1);
#if ! defined(_WIN32_WCE)
            GetPrivateProfileString(_RCT("StyleSet"),key1,_RCT(""),value1,300,home.getinifilename());
            GetPrivateProfileString(_RCT("StyleSet"),key2,_RCT(""),value2,300,home.getinifilename());			
#endif
            s1 = strtok(value1,_RCT(","));
            s2 = strtok(value2,_RCT(","));
            if (s1 != 0 && s2 != 0) {
                strcpy(name1,s1);
                strcpy(name2,s2);
                if (compare(name1,name2) > 0) {
                    qDebug() << "Swap StyleSet n and n-1, n=" << j;
#if !defined(_WIN32_WCE)
		
					QSettings settings(QString(home.getinifilename()), QSettings::IniFormat);
					QString qkey1 = QString("StyleSet") + QString("/") + QString(key1);
					QString qvalue1 = QString(value1);
					settings.setValue(qkey1, qvalue1);
					QString qkey2 = QString("StyleSet") + QString("/") + QString(key2);
					QString qvalue2 = QString(value2);
					settings.setValue(qkey2, qvalue2);
                    //WritePrivateProfileString(_RCT("StyleSet"),key1, value2,home.getinifilename());
                    //WritePrivateProfileString(_RCT("StyleSet"),key2, value1,home.getinifilename());
#endif					
                    j--;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
    }
}

/*!
 * \fn int SSListDialogControl::compare(RCCHAR left[300], RCCHAR right[300])
 * \brief SSListDialogControl::sort
 *
 * Returns an integer indicating relative sorted position
 * of parameters, 1 for right>left, -1 for left>right
 * and 0 otherwise. Comparison is based on lowest index
 * difference in Unicode values between RCCHAR arrays.
 *
 * \sa SSListDialogControl::sort()
*/
int SSListDialogControl::compare(RCCHAR left[300], RCCHAR right[300]) {
    bool trigger = false;
    int i = 0;
    while (!(left[i] == 0 || right[i] == 0)) {
        if(operator<(left[i],right[i])) {
            return 1;
        }
        else if(operator>(left[i],right[i])) {
            return -1;
        }
        else {
            i++;
            continue;
        }
    }

    if(left[i] == 0 && right[i] == 0) {
        return 0;
    }
    else if(left[i] == 0) {
        return 1;
    }
    else {
        return -1;
    }
}

void SSListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{
    //qDebug() << "SSListDialogControl::changefocus : hello";
}

void SSListDialogControl::change(Dialog *dialog,RCCHAR *str)
{
    //qDebug() << "SSListDialogControl::change : hello";
    //ListDialogControl::change(dialog,str);
}

/*
 *this is now in the header
class SSDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR lastname[300];
   public:
     SSDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(lastname,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
     void clear(void)  {  strcpy(lastname,"");  }
  };
*/

class SSButtonDialogControl : public ButtonDialogControl
  {public:
     SSButtonDialogControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int SSButtonDialogControl::select(Dialog *dialog)
{int i,movedown,state,colour,layer,style,iweight;
 double weight;
 RCCHAR name[300],key[300],keydown[300],value[300],message[300],*s;
 
  state = 1000;  //  
  if (id == 105)
  {//  Add or modify a new style
       dialog->currentvalue(100,name,300);
       dialog->currentvalue(101,&colour);
       dialog->currentvalue(102,&layer);
       dialog->currentvalue(103,&style);
       dialog->currentvalue(104,&weight);

       if (strchr(name,',') != 0) 
         *strchr(name,',') = 0;

       if (strlen(name) > 0)
         {  for (i = 1 ; ; i++)
              {  sprintf(key,"StyleSet%d",i);
#if ! defined(_WIN32_WCE)
                 GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
#endif
                 //qDebug() << "Value : " << QString(value);
                 if (strlen(value) == 0)
                   break;
                 s = strtok(value,_RCT(","));
                 if (_stricmp(s,name) == 0)
                   break;
              }

            if (strlen(value) == 0) //  Must be a new entry
              ((SSListDialogControl *)dialog->getcontrol(100))->additem(dialog,name);

            sprintf(value,"%ls,%d,%d,%d,%.2lf",name,colour,layer,style,weight);
#if ! defined(_WIN32_WCE)
            WritePrivateProfileString(_RCT("StyleSet"),key,value,home.getinifilename());
#endif


         }

  }
  else if (id == 107)
  {
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
       //colour = db.header.getcolour();
       ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&colour);
       general_property_command(&state,0,(void **)&colour);
       ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,colour);
       ((SSDisplayDialogControl *)dialog->getcontrol(106))->draw(dialog,0);
  }
  else if (id == 108)
  {
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
       //layer = db.header.getlayer();
       ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&layer);
       general_property_command(&state,0,(void **)&layer);
       ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,layer);
       ((SSDisplayDialogControl *)dialog->getcontrol(106))->draw(dialog,0);
  }
  else if (id == 109)
  {
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
       //style = db.header.getstyle();
       ((IntegerDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&style);
       general_property_command(&state,0,(void **)&style);
       ((IntegerDialogControl *)dialog->getcontrol(103))->change(dialog,style);
       ((SSDisplayDialogControl *)dialog->getcontrol(106))->draw(dialog,0);
  }
  else if (id == 110)
  {
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
       //iweight = db.header.getweight();
       //weight = db.lineweights.weighttomm(iweight);
       ((WeightAssocRealDialogControl *)dialog->getcontrol(104))->currentvalue(dialog,&weight);
       general_property_command(&state,0,(void **)&weight);
       ((WeightAssocRealDialogControl *)dialog->getcontrol(104))->change(dialog,weight);
       ((SSDisplayDialogControl *)dialog->getcontrol(106))->draw(dialog,0);
  }
  else if (id == 111)
    {//  Delete a style
       dialog->currentvalue(100,name,300);
       sprintf(message,"Really delete style set '%ls'?",name);
       if (strlen(name) > 0 &&RCMessageBox(cadwindow->gethwnd(),message,_RCT("Style Set"),MB_YESNO) == IDYES)
         {  for (i = 1,movedown = 0 ; ; i++)
              {  sprintf(key,"StyleSet%d",i);
                 sprintf(keydown,"StyleSet%d",i-1);
#if ! defined(_WIN32_WCE)
                 GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
#endif
                 if (strlen(value) == 0)
                   {  if (movedown)
                         WritePrivateProfileString(_RCT("StyleSet"),keydown,_RCT(""),home.getinifilename());
                      break;
                   }
                 s = strtok(value,_RCT(","));
                 if (_stricmp(s,name) == 0)
                   movedown = 1;
                 else if (movedown)
                   {
#if ! defined(_WIN32_WCE)
                       GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
                       WritePrivateProfileString(_RCT("StyleSet"),keydown,value,home.getinifilename());
#endif
                   }

              }

            ((SSListDialogControl *)dialog->getcontrol(100))->load(dialog);
            ((SSDisplayDialogControl *)dialog->getcontrol(106))->clear();
            ((SSDisplayDialogControl *)dialog->getcontrol(106))->draw(dialog,0);

         }
    }

  return 0;
}

void SSDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 RCHBRUSH hbrush;
 int i,colour,style;
 double weight;
 EntityHeader header(0,0,0,0);
 LineStyle *linestyle;
 RCCHAR name[300],key[300],value[300],*s;

 //qDebug() << "Entering : SSDisplayDialogControl::draw";

 if(hWnd == 0) return;

  if (dialog->currentvalue(100,name,300) != 0 && strcmp(name,lastname) != 0 && strlen(name) > 0)
    {
      for (i = 1 ; ; i++)
         {  sprintf(key,"StyleSet%d",i);
#if ! defined(_WIN32_WCE)
            GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
#endif
            if (strlen(value) == 0)
              break;
            s = strtok(value,_RCT(","));
            if (_stricmp(s,name) == 0)
              {  s = strtok(0,_RCT(","));
                 if (s != 0)
                   ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,atoi(s));
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,atoi(s));
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   ((IntegerDialogControl *)dialog->getcontrol(103))->change(dialog,atoi(s));
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   ((WeightAssocRealDialogControl *)dialog->getcontrol(104))->change(dialog,atof(s));
              }
         }

    }

  strcpy(lastname,name);

#ifdef USING_WIDGETS

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((Styleset_Dialog*)dialog->gethdlg())->getControl(106);
  QGraphicsScene *scene = 0;
  if(ctl)
      scene = ctl->scene();

  if(ctl && scene)
  {
      // using the ouput object to draw on the QGraphicsView widget on the dialog
      // the draw method also has this functionality as a backup
      output.setScene(scene);
      scene->setSceneRect(QRectF());
      scene->clear();
      QRect qrect = ctl->rect();
      rect.left = qrect.left();
      rect.top = qrect.top();
      rect.right = qrect.right();
      rect.bottom = qrect.bottom();

      output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                               ::v.getreal("wn::backgroundcolour.blue")*255.0));

      if (dialog->currentvalue(101,&colour) &&
          dialog->currentvalue(103,&style) &&
          dialog->currentvalue(104,&weight))
        {
           output.setup(1.0,0.0,DM_NORMAL,colour,style,db.lineweights.mmtoweight(weight),0);
           output.curPen.setCapStyle(Qt::FlatCap);
           if ((linestyle = db.linestyles.indextostyle(style)) == NULL)
           {  
                output.moveto(10,output.getheight()-1.0-(rect.top + rect.bottom) / 2);
                output.lineto((QGraphicsItem*)0,rect.right - 10,output.getheight()-1.0-(rect.top + rect.bottom) / 2);
           }
           else
           {
               linestyle->start(0.25,rect.right - 20);
               linestyle->draw(&output,10,(rect.top + rect.bottom) / 2,
                               rect.right - 10,(rect.top + rect.bottom) / 2);
           }
        }
  }
  else // get the control and draw the sample
      if(dialog->getQW())
          ((Styleset_Dialog*)dialog->gethdlg())->draw_sample();

#else
  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {
       oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
       IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());

       hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
       FillRect(output.gethdc(),&rect,hbrush);
       DeleteObject(hbrush);

       output.moveto(0,0);
       output.lineto(rect.right-1,0);
       output.lineto(rect.right-1,rect.bottom-1);
       output.lineto(0,rect.bottom-1);
       output.lineto(0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(101,&colour) &&
           dialog->currentvalue(103,&style) &&
           dialog->currentvalue(104,&weight))
         {  output.setup(1.0,0.0,DM_NORMAL,colour,style,db.lineweights.mmtoweight(weight),0);
            if ((linestyle = db.linestyles.indextostyle(style)) == NULL)
              {  output.moveto(10,output.getheight()-1.0-(rect.top + rect.bottom) / 2);
                 output.lineto(rect.right - 10,output.getheight()-1.0-(rect.top + rect.bottom) / 2);
              }
            else
              {  linestyle->start(1.0,rect.right - 20);
                 linestyle->draw(&output,10,(rect.top + rect.bottom) / 2,
                                  rect.right - 10,(rect.top + rect.bottom) / 2);
              }
         }
    }
#endif
}

void styleset_command(int *cstate,HighLevelEvent *event,void **)
{
 static int laststate;
 static EntityList selectionlist;
 //Dialog dialog("StyleSet_dialog");
 Dialog dialog("Styleset_Dialog");
 DialogControl *dc;
 int colour,layer,style,status,w;
 double weight;
 EntityList list,elist;
 Entity *e;
 BitMask change(32);

  if (*cstate == InitialState)
    {
       change.set(0);
       change.set(1);
       change.set(2);
       change.set(3);

       colour = v.getinteger("df::colour");
       layer = v.getinteger("df::layer");
       style = v.getinteger("df::style");
       weight = db.lineweights.weighttomm(v.getinteger("df::weight"));

       dialog.add(new SSListDialogControl(100));
       dialog.add(dc = new IntegerDialogControl(101,&colour,0,255));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(dc = new IntegerDialogControl(102,&layer,0,999));
       //dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(dc = new IntegerDialogControl(103,&style,0,255));
       //dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(dc = new WeightAssocRealDialogControl(104,&weight));
       //dialog.add(new ScrollBarDialogControl(1104,dc));
       dialog.add((DisplayDialogControl *) new SSDisplayDialogControl(106));
       dialog.add(new SSButtonDialogControl(105));
       dialog.add(new SSButtonDialogControl(107));
       dialog.add(new SSButtonDialogControl(108));
       dialog.add(new SSButtonDialogControl(109));
       dialog.add(new SSButtonDialogControl(110));
       dialog.add(new SSButtonDialogControl(111));
       dialog.add(new ButtonDialogControl(112));
       if (! state.getselection().empty()) 
         dialog.title("Change Style Set");
       dialog.setposition(DPTR);

       int dt = db.getnumericdisplaytolerance();
       db.setnumericdisplaytolerance(2);
       status = dialog.process();
       db.setnumericdisplaytolerance(dt);

       if (status == 112)
         {  laststate = *cstate;
            selectionlist = state.getselection().copy();
            state.destroyselection(0);
            *cstate = 1;
            cadwindow->prompt->normalprompt("Select the entity to clone the style from");
         }
       else if (status == TRUE)
         {  list = state.getselection().copy();
            if (list.empty())
              {  db.header.change(colour,layer,style,db.lineweights.mmtoweight(weight));
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",db.lineweights.mmtoweight(weight));
                 if (cadwindow->styleentry != 0)
                   cadwindow->styleentry->settype(_RCT(""));  //  Clear the style entry - to force an update
                 cadwindow->updatetitle();
              }
            else
              {  db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,colour,layer,style,db.lineweights.mmtoweight(weight));
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
                 list.destroy();

              }
            *cstate = ExitState;
         }
       else
         *cstate = ExitState;
    }
  else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  change.set(0);
            change.set(1);
            change.set(2);
            change.set(3);
            colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
            layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
            style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
            w = ((EntitySelectedEvent *) event)->getentity()->getweight();
            if (selectionlist.empty())
              {  db.header.change(colour,layer,style,w);
                 v.setinteger("df::colour",colour);
                 v.setinteger("df::layer",layer);
                 v.setinteger("df::style",style);
                 v.setinteger("df::weight",w);
                 if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 if (cadwindow->styleentry != 0)
                   cadwindow->styleentry->settype(_RCT(""));  //  Clear the style entry - to force an update
                 cadwindow->updatetitle();
              }
            else
              {  list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   {  if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,colour,layer,style,w);
                   }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            state.setselection(selectionlist,1);
            selectionlist = elist;
         }  
       *cstate = ExitState;
    }
  else
    *cstate = ExitState;


  if (*cstate == ExitState)
    cadwindow->updatetitle();

#ifdef USING_WIDGETS
  if (*cstate == ExitState)
  {
    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("StyleSet"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("StyleSet"));
  }
#endif

}

/**
 * @brief mask_snaps_command
 * @param cstate
 *
 * uses the snamask dialog to set snapping and dragging options
 * KMJ : works in the Qt version
 */
void mask_snaps_command(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("SnapMask_Dialog");
 BitMask snapmask(32);
 ResourceString rs33(NCMAIN+33);
 int dragstyle,constraindrag;

  snapmask = db.snapmask;
  dragstyle = v.getinteger("db::dragstyle");
  constraindrag = v.getinteger("db::constraindrag");

  dialog.add(new CheckBoxDialogControl(100,&snapmask,SnapMaskOrigin));
  dialog.add(new CheckBoxDialogControl(101,&snapmask,SnapMaskIntersection));
  dialog.add(new CheckBoxDialogControl(102,&snapmask,SnapMaskEnd));
  dialog.add(new CheckBoxDialogControl(103,&dragstyle));
  dialog.add(new CheckBoxDialogControl(104,&constraindrag));

  dialog.setposition(DPTR);
  if (dialog.process() == TRUE)
    {  db.snapmask = snapmask;
       v.setinteger("db::dragstyle",dragstyle);
       v.setinteger("db::constraindrag",constraindrag);
       cadwindow->paintall();
    }

  *cstate = ExitState;
}

// not used any more replaced by the new general properties command
void entity_commandEx(int *cstate,HighLevelEvent *,void **)
{
    Dialog dialog("MaskChange_Dialog");
    dialog.process();
    *cstate = ExitState;

    cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Entity"));
    cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Entity"));
}

/**
 * @brief OtherColourDisplayDialogControl::draw
 * @param dialog
 * @param hDC
 * Other Colour Dialog current colour control
 * for use with the DefineColour_Dialog when defining colours other than the palette
 */
void OtherColourDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output1(hWnd,hDC,&top,1.0);
 View3dOutput output2(hWnd,hDC,&top,1.0);
 View3dOutput output3(hWnd,hDC,&top,1.0);
 double red,green,blue;
 //fred,fgreen,fblue;
 int index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

    //qDebug() << "OtherColourDisplayDialogControl::draw : start";

  if (dialog->getcontrol(100) != 0)
  {
       if (! dialog->currentvalue(100,&index))
           return;
       if (lastindex != -1 && lastindex != index)
       {
           ((RealDialogControl *) dialog->getcontrol(101))->
                       change(dialog,db.colourtable[index].red/255.0);
           ((RealDialogControl *) dialog->getcontrol(102))->
                       change(dialog,db.colourtable[index].green/255.0);
            ((RealDialogControl *) dialog->getcontrol(103))->
                       change(dialog,db.colourtable[index].blue/255.0);
       }
       lastindex = index;
  }

  if (dialog->currentvalue(101,&red) &&
      dialog->currentvalue(102,&green) &&
      dialog->currentvalue(103,&blue) &&
      dialog->currentvalue(107,&hue) &&
      dialog->currentvalue(108,&lightness) &&
      dialog->currentvalue(109,&saturation))
    {
      if (lastcolour == Colour(red,green,blue))
      //if (lastcolour.red == (int)red*255.0+0.5 && lastcolour.green == (int)green*255.0+0.5 && lastcolour.blue == (int)blue*255.0+0.5)
      {//  HLS has changed
          //qDebug() << "HLS has changed";
            c.sethls(hue,lightness,saturation);
            c.get(&red,&green,&blue);

            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));

       }
       else
       {//  RGB has changed
          //qDebug() << "RGB has changed";
            c.set(red,green,blue);
            c.gethls(&hue,&lightness,&saturation);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));
       }
       lastcolour.set(red,green,blue);

       // qDebug() << "draw the other samples";
       // draw the other samples
       DisplayDialogControl *ddc;
       for (controllist.start() ; (ddc = (DisplayDialogControl *)controllist.next()) != NULL ; )
       {
           ((OtherColourSampleDialogControl*)ddc)->change(lastcolour);
           ((OtherColourSampleDialogControl*)ddc)->draw(dialog,dialog->gethdlg());
       }
   }
}

/**
 * @brief OtherColourSampleDialogControl::draw
 * @param dialog
 * @param hDC
 * Other Colour Dialog sample colour control
 * for use with the DefineColour_Dialog when defining colours other than the palette
 */
void OtherColourSampleDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if(hWnd == 0) return;

  //qDebug() << "OtherColourSampleDialogControl::draw start";
    fred   = fixedcolour.red;
    fgreen = fixedcolour.green;
    fblue  = fixedcolour.blue;

    // this is the new colour
    red   = lastcolour.red;
    green = lastcolour.green;
    blue  = lastcolour.blue;

       QGraphicsView *gv;
       QGraphicsScene *scene;
       // qDebug() << "OtherColourSampleDialogControl::draw id " << id;
       if(id == 115)
       {
           //qDebug() << "OtherColourSampleDialogControl::draw 115";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(115);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               //qDebug() << "setting colour sample scene";
               output.setScene(scene);
               output.setWidet(gv);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : red : " << red << " green : " << green << " blue : " << blue;
               scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
           }
       }
       else if(id == 113)
       {
           //qDebug() << "otherColourSampleDialogControl::draw 113";
           // this control is showing the current background colour
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(113);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               //qDebug() << "setting the fixed colour sample scene";
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : fred : " << fred << " fgreen : " << fgreen << " fblue : " << fblue;
               scene->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
           }
       }
       else if(id == 116)
       {
           //qDebug() << "OtherColourSampleDialogControl::draw 116";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(116);
           if(gv)
               scene = gv->scene();
            //qDebug() << "OtherColourSampleDialogControl::draw gv" << gv;
            //qDebug() << "OtherColourSampleDialogControl::draw scene" << scene;
           if(gv && scene)
           {
               //qDebug() << "OtherColourSampleDialogControl::draw setting scene output" ;
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               QRect qrect = gv->rect();
               rect.left = qrect.left();
               rect.top = qrect.top();
               rect.right = qrect.right();
               rect.bottom = qrect.bottom();
               //qDebug() << "OtherColourSampleDialogControl::draw setbackground" ;
               //output.setbackground(RGB(red,green,blue));
               // scene->setBackgroundBrush(QBrush(output.getbackground()));
                scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
                //qDebug() << "OtherColourSampleDialogControl::draw getstyle" <<db.header.getstyle();
                LineStyle *linestyle = db.linestyles.indextostyle(db.header.getstyle());
                //qDebug() << "OtherColourSampleDialogControl::draw setupt" ;
                output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
                //qDebug() << "OtherColourSampleDialogControl::drawsetcap" ;
                output.curPen.setCapStyle(Qt::FlatCap);
                //qDebug() << "OtherColourSampleDialogControl::draw pencolour" ;
                output.curPen.setColor(QColor(db.colourtable[db.header.getcolour()].red,
                                              db.colourtable[db.header.getcolour()].green,
                                              db.colourtable[db.header.getcolour()].blue));
                //qDebug() << "OtherColourSampleDialogControl::draw linestyle" << linestyle;
                if (linestyle && linestyle->getnsegments() > 0)
                {//qDebug() << "OtherColourSampleDialogControl::draw linestyle" ;
                     linestyle->start(0.5,rect.right - 20);
                     linestyle->draw(&output,10,(rect.top + rect.bottom) / 2,
                                rect.right - 10,(rect.top + rect.bottom) / 2);
                }
                else
                {//qDebug() << "OtherColourSampleDialogControl::draw moveto" ;
                     output.moveto(10,output.getheight()-(rect.top + rect.bottom) / 2);
                     output.lineto(rect.right - 10,output.getheight()-(rect.top + rect.bottom) / 2);
                }
                //qDebug() << "OtherColourSampleDialogControl::draw done" ;
           }
       }

}

void BackColourDialogSampleControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if(hWnd == 0) return;
    //qDebug() << "OtherColourSampleDialogControl::draw start";
    // these is the current background colour
    fred   = ::v.getreal("wn::backgroundcolour.red")  * 255.0;
    fgreen = ::v.getreal("wn::backgroundcolour.green")* 255.0;
    fblue  = ::v.getreal("wn::backgroundcolour.blue") * 255.0;

    // this is the new colour
    red   = lastcolour.red;
    green = lastcolour.green;
    blue  = lastcolour.blue;

#ifdef USING_WIDGETS
       QGraphicsView *gv;
       QGraphicsScene *scene;
       // qDebug() << "OtherColourSampleDialogControl::draw id " << id;
       if(id == 115)
       {
           //qDebug() << "OtherColourSampleDialogControl::draw 115";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(115);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               //qDebug() << "setting colour sample scene";
               output.setScene(scene);
               output.setWidet(gv);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : red : " << red << " green : " << green << " blue : " << blue;
               scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
           }
       }
       else if(id == 113)
       {
           //qDebug() << "otherColourSampleDialogControl::draw 113";
           // this control is showing the current background colour
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(113);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               //qDebug() << "setting the fixed colour sample scene";
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : fred : " << fred << " fgreen : " << fgreen << " fblue : " << fblue;
               scene->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
           }
       }
       else if(id == 116)
       {
           //qDebug() << "OtherColourSampleDialogControl::draw 116";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(116);
           if(gv)
               scene = gv->scene();
            //qDebug() << "OtherColourSampleDialogControl::draw gv" << gv;
            //qDebug() << "OtherColourSampleDialogControl::draw scene" << scene;
           if(gv && scene)
           {
               //qDebug() << "OtherColourSampleDialogControl::draw setting scene output" ;
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               QRect qrect = gv->rect();
               rect.left = qrect.left();
               rect.top = qrect.top();
               rect.right = qrect.right();
               rect.bottom = qrect.bottom();
               //qDebug() << "OtherColourSampleDialogControl::draw setbackground" ;
               //output.setbackground(RGB(red,green,blue));
               // scene->setBackgroundBrush(QBrush(output.getbackground()));
                scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
                //qDebug() << "OtherColourSampleDialogControl::draw getstyle" <<db.header.getstyle();
                LineStyle *linestyle = db.linestyles.indextostyle(db.header.getstyle());
                //qDebug() << "OtherColourSampleDialogControl::draw setupt" ;
                output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
                //qDebug() << "OtherColourSampleDialogControl::drawsetcap" ;
                output.curPen.setCapStyle(Qt::FlatCap);
                //qDebug() << "OtherColourSampleDialogControl::draw pencolour" ;
                output.curPen.setColor(QColor(db.colourtable[db.header.getcolour()].red,
                                              db.colourtable[db.header.getcolour()].green,
                                              db.colourtable[db.header.getcolour()].blue));
                //qDebug() << "OtherColourSampleDialogControl::draw linestyle" << linestyle;
                if (linestyle && linestyle->getnsegments() > 0)
                {//qDebug() << "OtherColourSampleDialogControl::draw linestyle" ;
                     linestyle->start(0.5,rect.right - 20);
                     linestyle->draw(&output,10,(rect.top + rect.bottom) / 2,
                                rect.right - 10,(rect.top + rect.bottom) / 2);
                }
                else
                {//qDebug() << "OtherColourSampleDialogControl::draw moveto" ;
                     output.moveto(10,output.getheight()-(rect.top + rect.bottom) / 2);
                     output.lineto(rect.right - 10,output.getheight()-(rect.top + rect.bottom) / 2);
                }
                //qDebug() << "OtherColourSampleDialogControl::draw done" ;
           }
       }


#else
       brush = CreateSolidBrush(RGB(red,green,blue));
       FillRect(output.gethdc(),&rect,brush);
       DeleteObject(brush);
       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
            SelectObject(output.gethdc(),oldhpen);
            DeleteObject(hpen);
         }
#endif
}

/**
 * @brief GPColourDisplayDialogControl::draw
 * @param dialog
 * @param hDC
 * General Properties Colour Dialog Current sample control
 * for use with the DefineColour_Dialog when defining palette colours
 */
void GPColourDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output1(hWnd,hDC,&top,1.0);
 View3dOutput output2(hWnd,hDC,&top,1.0);
 View3dOutput output3(hWnd,hDC,&top,1.0);
 double red,green,blue;
 int fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

    //qDebug() << "GPColourDisplayDialogControl::draw : start";

  if (dialog->getcontrol(100) != 0)
  {
       if (! dialog->currentvalue(100,&index))
           return;
       if (lastindex != -1 && lastindex != index)
         {  ((RealDialogControl *) dialog->getcontrol(101))->
                       change(dialog,db.colourtable[index].red/255.0);
            ((RealDialogControl *) dialog->getcontrol(102))->
                       change(dialog,db.colourtable[index].green/255.0);
            ((RealDialogControl *) dialog->getcontrol(103))->
                       change(dialog,db.colourtable[index].blue/255.0);
         }
       lastindex = index;
       //fred=db.colourtable[lastindex].red;
       //fgreen=db.colourtable[lastindex].green;
       //fblue=db.colourtable[lastindex].blue;
  }

  if (dialog->currentvalue(101,&red) &&
      dialog->currentvalue(102,&green) &&
      dialog->currentvalue(103,&blue) &&
      dialog->currentvalue(107,&hue) &&
      dialog->currentvalue(108,&lightness) &&
      dialog->currentvalue(109,&saturation))
    {
      if (lastcolour == Colour(red,green,blue))
      //if (lastcolour.red == red && lastcolour.green == green && lastcolour.blue == blue)
      {//  HLS has changed
          //qDebug() << "HLS has changed";
            c.sethls(hue,lightness,saturation);
            c.get(&red,&green,&blue);

            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));

       }
       else
       {//  RGB has changed
          //qDebug() << "RGB has changed";
            c.set(red,green,blue);
            c.gethls(&hue,&lightness,&saturation);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));
       }
       lastcolour.set(red,green,blue);

       QGraphicsView *gv1;
       QGraphicsScene *scene1;

       /*
       if(dialog->gethdlg())
           gv1 = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(115);
       if(gv1)
           scene1 = gv1->scene();

       // draw our sample

       if(gv1 && scene1)
       {
           //qDebug() << "setting colour sample scene";
           output1.setScene(scene1);
           scene1->setSceneRect(QRectF());
           scene1->clear();
           //qDebug() << "colour : fred : " << fred << " fgreen : " << green << " fblue : " << blue;
           scene1->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
       }
*/
        //qDebug() << "draw the other samples";
       // draw the other samples
       DisplayDialogControl *ddc;
       for (controllist.start() ; (ddc = (DisplayDialogControl *)controllist.next()) != NULL ; )
       {
           ((GPColourSampleDialogControl*)ddc)->change(lastcolour);
           ((GPColourSampleDialogControl*)ddc)->draw(dialog,dialog->gethdlg());
       }
   }
}

/**
 * @brief GPColourSampleDialogControl::draw
 * @param dialog
 * @param hDC
 * General Properties Colour Dialog other samples control
 * for use with the DefineColour_Dialog when defining palette colours
 */
void GPColourSampleDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if(hWnd == 0) return;

    //qDebug() << "GPColourSampleDialogControl::draw start";
    fred   = fixedcolour.red;
    fgreen = fixedcolour.green;
    fblue  = fixedcolour.blue;

    red   = lastcolour.red;
    green = lastcolour.green;
    blue  = lastcolour.blue;

       QGraphicsView *gv=0;
       QGraphicsScene *scene=0;
        //qDebug() << "GPColourSampleDialogControl::draw id " << id;
       if(id == 115)
       {
           // this is the new colour sample
           //qDebug() << "GPColourSampleDialogControl::draw 115";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(115);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               //qDebug() << "setting colour sample scene";
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : fred : " << fred << " fgreen : " << green << " fblue : " << blue;
               scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
           }
       }
       else if(id == 113)
       {
           //qDebug() << "GPColourSampleDialogControl::draw 113";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(113);
           if(gv)
               scene = gv->scene();

           if(gv && scene)
           {
               // this is the current palette index
               if (! dialog->currentvalue(100,&index)) return;
               //qDebug() << "setting colour sample scene";
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               //qDebug() << "colour : fred : " << fred << " fgreen : " << fgreen << " fblue : " << fblue;
               scene->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
           }
       }
       else if(id == 116)
       {
           // this is the background and line colour sample
           //qDebug() << "GPColourSampleDialogControl::draw 116";
           if(dialog->gethdlg())
             gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(116);
           if(gv)
               scene = gv->scene();
            //qDebug() << "GPColourSampleDialogControl::draw gv" << gv;
            //qDebug() << "GPColourSampleDialogControl::draw scene" << scene;
           if(gv && scene)
           {
               qDebug() << "GPColourSampleDialogControl::draw setting scene output" ;
               output.setScene(scene);
               scene->setSceneRect(QRectF());
               scene->clear();
               QRect qrect = gv->rect();
               rect.left = qrect.left();
               rect.top = qrect.top();
               rect.right = qrect.right();
               rect.bottom = qrect.bottom();
               // qDebug() << "GPColourSampleDialogControl::draw setbckgorund" ;
               output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                        ::v.getreal("wn::backgroundcolour.green")*255.0,
                                        ::v.getreal("wn::backgroundcolour.blue")*255.0));

                //qDebug() << "GPColourSampleDialogControl::draw scen back set" ;
                scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                        ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                        ::v.getreal("wn::backgroundcolour.blue")*255.0)));
                //qDebug() << "GPColourSampleDialogControl::draw getstyle" <<db.header.getstyle();
                LineStyle *linestyle = db.linestyles.indextostyle(db.header.getstyle());
                //qDebug() << "GPColourSampleDialogControl::draw setupt" ;
                output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
                //qDebug() << "GPColourSampleDialogControl::drawsetcap" ;
                output.curPen.setCapStyle(Qt::FlatCap);
                //qDebug() << "GPColourSampleDialogControl::draw pencolour" ;
                output.curPen.setColor(QColor(red,green,blue));
                //qDebug() << "GPColourSampleDialogControl::draw linestyle" << linestyle;
                if (linestyle && linestyle->getnsegments() > 0)
                {//qDebug() << "GPColourSampleDialogControl::draw linestyle" ;
                     linestyle->start(0.5,rect.right - 20);
                     linestyle->draw(&output,10,(rect.top + rect.bottom) / 2,
                                rect.right - 10,(rect.top + rect.bottom) / 2);
                }
                else
                {//qDebug() << "GPColourSampleDialogControl::draw moveto" ;
                     output.moveto(10,output.getheight()-(rect.top + rect.bottom) / 2);
                     output.lineto(rect.right - 10,output.getheight()-(rect.top + rect.bottom) / 2);
                }
                //qDebug() << "GPColourSampleDialogControl::draw done" ;
           }
       }
}

class GPStyleApplyButtonDialogControl : public ButtonDialogControl
{public:
   int styleindex;
   LineStyle *linestyle;
   GPStyleApplyButtonDialogControl(int i) :
       ButtonDialogControl(i) { styleindex = -1; linestyle = 0; }
   int select(Dialog *);
};

int GPStyleApplyButtonDialogControl::select(Dialog *parentdialog)
{
    if (parentdialog->currentvalue(100,&styleindex) &&
        (linestyle=((StyleDisplayDialogControl*)parentdialog->getcontrol(105))->getLinestyle()))
    {
        db.linestyles.del(styleindex);
        if (linestyle->getnsegments() > 0)
            db.linestyles.set(styleindex,linestyle->copy());
    }
    return 0;
}

class GPChangeButtonDialogControl : public ButtonDialogControl
{public:
   GPChangeButtonDialogControl(int i) :
       ButtonDialogControl(i) {  }
   int select(Dialog *);
};

int GPChangeButtonDialogControl::select(Dialog *parentdialog)
{
    int currentTab=0;

    currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());

    //qDebug() << "GPChange... currentTab : " << currentTab;

    if(currentTab == 2 )
    { // colour change button click
        Dialog dialog("DefineColour_Dialog");
        DialogControl *dc;
        Colour lastColours[256];
        int i,index,ired,igreen,iblue;
        int lastIndex;
        double lastRed,lastGreen,lastBlue;
        double hue,lightness,saturation;
        double red,green,blue;
        char key[300];

        index = v.getinteger("ss::colour");
        //qDebug() << "GPChange... current index : " << index;

        red   = (double)db.colourtable[index].red/255.0;
        green = (double)db.colourtable[index].green/255.0;
        blue  = (double)db.colourtable[index].blue/255.0;

        // save the current state
        lastIndex = index;
        lastRed   = red;
        lastGreen = green;
        lastBlue  = blue;

        // copy the current colour table
        for(i=0; i<256; i++)
            lastColours[i] = db.colourtable[i];

        //qDebug() << "GPChange... current colour : red : " << red << " green : " << green << " blue : " << blue;

        Colour c;
        c.set(red,green,blue);
        c.gethls(&hue,&lightness,&saturation);

        dialog.add(dc = new IntegerDialogControl(100,&index,0,255));

        dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
        dialog.add(new ColourScrollBarDialogControl(104,dc,0,255));
        dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
        dialog.add(new ColourScrollBarDialogControl(105,dc,0,255));
        dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
        dialog.add(new ColourScrollBarDialogControl(106,dc,0,255));


        dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
        dialog.add(new ColourScrollBarDialogControl(110,dc,0,255));
        dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
        dialog.add(new ColourScrollBarDialogControl(111,dc,0,255));
        dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
        dialog.add(new ColourScrollBarDialogControl(112,dc,0,255));

        GPColourDisplayDialogControl *pddc;
        GPColourSampleDialogControl *sddc;
        dialog.add((DisplayDialogControl *)(pddc = new GPColourDisplayDialogControl(120,parentdialog,c)));
        dialog.add((DisplayDialogControl *)(sddc = new GPColourSampleDialogControl(113,pddc,parentdialog,c)));
        ((GPColourDisplayDialogControl *)pddc)->add(sddc);
        dialog.add((DisplayDialogControl *)(sddc = new GPColourSampleDialogControl(115,pddc,parentdialog,c)));
        ((GPColourDisplayDialogControl *)pddc)->add(sddc);
        dialog.add((DisplayDialogControl *)(sddc = new GPColourSampleDialogControl(116,pddc,parentdialog,c)));
        ((GPColourDisplayDialogControl *)pddc)->add(sddc);

        dialog.add(new ColourApplyButtonDialogControl(114,0));
        int n = db.getnumericdisplaytolerance();
        db.setnumericdisplaytolerance(3);

        if(dialog.process() == TRUE)
        {
            ired   = (int)(red*255.0+0.5);
            igreen = (int)(green*255.0+0.5);
            iblue  = (int)(blue*255.0+0.5);
            db.colourtable[index].red = Byte(ired);
            db.colourtable[index].green = Byte(igreen);
            db.colourtable[index].blue = Byte(iblue);
            sprintf(key,"cl::%d",index);
            v.setlong(key,(long(ired) << 16) | (long(igreen) << 8) | iblue);
            v.setinteger("ss:colour",index);
        }
        else
        {
            // restore the previous state
            index = lastIndex;
            red = lastRed;
            green = lastGreen;
            blue = lastBlue;

            // copy the current colour table
            for(i=0; i<256; i++)
                db.colourtable[i] = lastColours[i];

            ired   = (int)(red*255.0+0.5);
            igreen = (int)(green*255.0+0.5);
            iblue  = (int)(blue*255.0+0.5);
            sprintf(key,"cl::%d",index);
            v.setlong(key,(long(ired) << 16) | (long(igreen) << 8) | iblue);
            v.setinteger("ss:colour",index);
        }
        db.setnumericdisplaytolerance(n);

        return 0;
    }
    else if(currentTab == 3 )
    { // style change button click
        Dialog dialog("DefineStyle_Dialog");
        DialogControl *dc;
        int i,index,nsegments,csegment;
        int lastIndex;
        double dash,space;
        LineStyle *linestyle;
#if 0
        LineStyles lastStyles;
        // copy the current linestyles
        lastStyles.clear();
        for(i=0; i<256; i++)
        {
            if (db.linestyles.indextostyle(i) != NULL && db.linestyles.indextostyle(i)->getnsegments() > 0)
                lastStyles.set(i,db.linestyles.indextostyle(i)->copy());
            else
                lastStyles.set(i,NULL);
        }
#endif
        index = v.getinteger("ss::style");
        lastIndex = index;
        linestyle = db.linestyles.indextostyle(index);
        if (linestyle == NULL)
        {
           nsegments = 0;
           csegment = 1;
           dash = space = 0.0;
           linestyle = new LineStyle;
        }
        else
        {
           linestyle = linestyle->copy();
           nsegments = linestyle->getnsegments();
           csegment = 1;
           linestyle->getsegment(csegment,&dash,&space);
        }
        dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
        //dialog.add(new ScrollBarDialogControl(1100,dc));
        dialog.add(dc = new IntegerDialogControl(101,&nsegments,0,1000));
        //dialog.add(new ScrollBarDialogControl(1101,dc));
        dialog.add(dc = new IntegerDialogControl(102,&csegment,1,1000));
        //dialog.add(new ScrollBarDialogControl(1102,dc));
        dialog.add(dc = new RealDialogControl(103,&dash,0.1,0.0,1E20,ScaleByLength));
        //dialog.add(new ScrollBarDialogControl(1103,dc));
        dialog.add(dc = new RealDialogControl(104,&space,0.1,0.0,1E20,ScaleByLength));
        //dialog.add(new ScrollBarDialogControl(1104,dc));
        dialog.add((DisplayDialogControl *) new StyleDisplayDialogControl(105,parentdialog,&linestyle));
        dialog.add(new GPStyleApplyButtonDialogControl(106));

        if(dialog.process() == TRUE)
        {
            db.linestyles.del(index);
            if (linestyle->getnsegments() > 0)
                db.linestyles.set(index,linestyle->copy());

            v.setinteger("ss::style",index);
        }
        else
        {
#if 0
            // restore the status
            db.linestyles.clear();
            for(i=0; i<256; i++)
            {
                if (lastStyles.indextostyle(i) != NULL && lastStyles.indextostyle(i)->getnsegments() > 0)
                    db.linestyles.set(i,lastStyles.indextostyle(i)->copy());
                else
                    db.linestyles.set(i,NULL);
            }
#endif
            v.setinteger("ss::style",lastIndex);
        }

        return 0;
    }
}

class GPButtonDialogControl : public ButtonDialogControl
  {public:
     GPButtonDialogControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };

int GPButtonDialogControl::select(Dialog *dialog)
{
    qDebug() << "select : " << id;

    RCCHAR tab[20];
    sprintf(tab,"%d",id-101);
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),tab,home.getinifilename());
    return 0;
}

class GPDefinePatternButtonDialogControl : public ButtonDialogControl
{public:
   GPDefinePatternButtonDialogControl(int i) :
       ButtonDialogControl(i) {  }
   int select(Dialog *);
};

int GPDefinePatternButtonDialogControl::select(Dialog *parentdialog)
{
    //qDebug() << "select : " << id;

   int status=0;
   Dialog  dialog("DefinePatternStyle_Dialog");




   if ((status = dialog.process()) == TRUE)
       return 0;
   else if(status == 122)
   {
       state.sendevent(new NewCommandEvent(221,0));
       return status;
   }
   return 0;
}

// KMJ : the new All in One general properties Change command
/**
 * @brief general_property_command
 * @param cstate
 * @param event
 * @param s
 */
void general_property_command(int *cstate,HighLevelEvent *event,void **s)
{
 static int laststate;
 static EntityList selectionlist;
 static Point p1;
 static EntityList pelist;
 //Dialog dialog("Style_dialog");
 Dialog dialog("MaskChange_Dialog");
 DialogControl *dc;
 EntityList list,elist;
 Entity *e;
 BitMask change(32);
 int i,n,style,colour,layer,iweight,entity,status;
 double weight;
 ResourceString rs36(NCMAIN+36);
 Point p2,p3,p4;
 Line *line;
 int currentTab=0;

 currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());

 if(event == 0)
     WritePrivateProfileString("DialogSettings","GenProps::Parent","Dialog",home.getinifilename());
 else
     WritePrivateProfileString("DialogSettings","GenProps::Parent","",home.getinifilename());

 switch(*cstate)
    {case InitialState:
     case 1000:
     case 1001:
       if (*cstate == 1000 || *cstate == 1001)
       {
           //qDebug() << "called from a dialog";

           switch(currentTab)
           {
           case 0:
               layer = *((int *)s);
               v.setinteger("ss::layer",layer);
               break;
           case 1:
               weight = *((double *)s);
               iweight = db.lineweights.mmtoweight(weight);
               v.setreal("ss::weight",weight);
               break;
           case 2:
               colour = *((int *)s);
               v.setinteger("ss::colour",colour);
               break;
           case 3:
               style = *((int *)s);
               v.setinteger("ss::style",style);
               break;
           case 4:
               entity = *((int *)s);
               v.setinteger("ss::entity",entity);
           }
       }
       else
       {
           //qDebug() << "called from a button";

           if (! state.getselection().empty())
           {
               if(state.getselection().length() > 1)
               {
                   layer = -1;//v.getinteger("df::layer");
                   v.setinteger("ss::layer",layer);

                   iweight = -1;//v.getinteger("df::weight");
                   weight = -1;;//db.lineweights.weighttomm(iweight);
                   v.setreal("ss::weight",weight);

                   colour = -1;//v.getinteger("df::colour");
                   v.setinteger("ss::colour",colour);

                   style = -1;//v.getinteger("df::style");
                   v.setinteger("ss::style",style);
               }
               else if(state.getselection().length() == 1)
               {
                   list = state.getselection().copy();
                   list.start();
                   e = list.next();

                   layer = e->getlayer();
                   v.setinteger("ss::layer",layer);

                   iweight = e->getweight();
                   weight = db.lineweights.weighttomm(iweight);
                   v.setreal("ss::weight",weight);

                   colour = e->getcolour();
                   v.setinteger("ss::colour",colour);

                   style = e->getstyle();
                   v.setinteger("ss::style",style);
               }
           }
           else
           {
                layer = v.getinteger("df::layer");
                v.setinteger("ss::layer",layer);

                iweight = v.getinteger("df::weight");
                weight = db.lineweights.weighttomm(iweight);
                v.setreal("ss::weight",weight);

                colour = v.getinteger("df::colour");
                v.setinteger("ss::colour",colour);

                style = v.getinteger("df::style");
                v.setinteger("ss::style",style);
            }
            //entity = v.getinteger("df::colour");
            //v.setinteger("ss::colour",colour);
       }

       if (! state.getselection().empty()) dialog.title(rs36.gets());
       // tab buttons
       dialog.add(new GPButtonDialogControl(101));
       dialog.add(new GPButtonDialogControl(102));
       dialog.add(new GPButtonDialogControl(103));
       dialog.add(new GPButtonDialogControl(104));
       dialog.add(new GPButtonDialogControl(105));
       // change button
       dialog.add(new GPChangeButtonDialogControl(106)); // change colour
       dialog.add(new GPChangeButtonDialogControl(114)); // change style
       //dialog.add(new DefineStyleButtonDialogControl(106));
       //dialog.add(new DefineColourButtonDialogControl(102));
       //dialog.add(new DefineColourButtonDialogControl(103));
       //dialog.add(new DefinePatternStyleButtonDialogControl(103));
       if (event != 0) // clone button not used with style sheets
         dialog.add(new ButtonDialogControl(107));

       //dialog.add(new GPDefinePatternButtonDialogControl(111));
       dialog.add(new DefinePatternStyleButtonDialogControl(111));

       dialog.setposition(DPTR);

       if ((status = dialog.process()) == TRUE)
       {
            WritePrivateProfileString("DialogSettings","GenProps::Returned","OK",home.getinifilename());

            if (*cstate == 1000 || *cstate == 1001)
            {
                //qDebug() << "called from another dialog";
                // OK when called from another dialog
                if(currentTab == 0)
                {
                    layer = v.getinteger("ss::layer");
                    *((int *)s) = layer;
                }
                else if(currentTab == 1)
                {
                    weight = v.getreal("ss::weight");
                    iweight = db.lineweights.mmtoweight(weight);
                    *((double *)s) = weight;
                }
                else if(currentTab == 2)
                {
                    colour = v.getinteger("ss::colour");
                    *((int *)s) = colour;
                }
                else if(currentTab = 3)
                {
                 style = v.getinteger("ss::style");
                *((int *)s) = style;
                }
                laststate = *cstate;
            }
            else if (state.getselection().empty())
            {
                 // normal OK behaviour
                 layer = v.getinteger("ss::layer");
                 v.setinteger("df::layer",layer);

                 weight = v.getreal("ss::weight");
                 iweight = db.lineweights.mmtoweight(weight);
                 v.setinteger("df::weight",iweight);

                 colour = v.getinteger("ss::colour");
                 v.setinteger("df::colour",colour);

                 style = v.getinteger("ss::style");
                 v.setinteger("df::style",style);

                 change.set(0);
                 change.set(1);
                 change.set(2);
                 change.set(3);
                 db.header.change(colour,layer,style,iweight);
                 cadwindow->updatetitle();
            }
            else
            {
                 // change preselected entities
                 // normal OK behaviour
                 layer = v.getinteger("ss::layer");
                 change.set(1,v.getinteger("ss::layerSet"));

                 weight = v.getreal("ss::weight");
                 iweight = db.lineweights.mmtoweight(weight);
                 change.set(3,v.getinteger("ss::weightSet"));

                 colour = v.getinteger("ss::colour");
                 change.set(0,v.getinteger("ss::colourSet"));

                 style = v.getinteger("ss::style");
                 change.set(2,v.getinteger("ss::styleSet"));

                 list = state.getselection().copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                 {
                     if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                      e->change(change,colour,layer,style,iweight);
                 }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);

                 cadwindow->paintall(1);
            }
            *cstate = ExitState;
       }
       else if (status == 107) // cloning button was pressed
       {
           laststate = *cstate;
            currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());
            int cmd = 0;
            switch(currentTab)
            {
            case 0: // layer
            default:
                cmd = 218;
                break;
            case 1: // weight
                cmd = 220;
                break;
            case 2: // colour
                cmd = 217;
                break;
            case 3: // style
                cmd = 219;
                break;
            }
            state.sendevent(new NewCommandEvent(cmd,0));
            *cstate = ExitState;
       }
       else if (status == 111) // define pattern style button was pressed
       {
           laststate = *cstate;
            //pelist = state.getselection().copy();
            //state.destroyselection(0);
           // run the define line pattern style command
           //state.sendevent(new NewCommandEvent(cmd,0));
           *cstate = ExitState;
       }
       else
       {
          WritePrivateProfileString("DialogSettings","GenProps::Returned","Cancel",home.getinifilename());
          *cstate = ExitState;
       }
      break;
     case 1 : // cloning change
       if (event->isa(EntitySelected))
       {
           //qDebug() << "cloning entity properties";
           //qDebug() << "GP dialog end OK : currentTab : " << currentTab;

           // normal OK behaviour
           layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
           v.setinteger("ss::layer",layer);
           //qDebug() << "dialog ok cloned layer : " << layer;

           iweight = ((EntitySelectedEvent *) event)->getentity()->getweight();
           weight = db.lineweights.weighttomm(iweight);
           v.setreal("ss::weight",weight);
           //qDebug() << "dialog ok cloned weight : " << weight;

           colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
           v.setinteger("ss::colour",colour);
           //qDebug() << "dialog ok colned colour : " << colour;

           style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
           v.setinteger("ss:sstyle",style);
           //qDebug() << "dialog ok cloned style : " << style;

           //entity = v.getinteger("df::colour");
           //v.setinteger("ss::colour",colour);


            if (laststate == 1000)
            {
                if(currentTab == 0)
                    *((int *)s) = layer;
                else if(currentTab == 1)
                    *((double *)s) = weight;
                else if(currentTab == 2)
                    *((int *)s) = colour;
                else if(currentTab == 3)
                    *((int *)s) = style;
            }
            else if (selectionlist.empty())
            {
                 // cloning only the current tab properties to the defaults
                 //db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                if(currentTab == 0)
                {
                    change.set(1);
                    db.header.change(0,layer,0,0);
                    db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                    v.setinteger("df::layer",layer);
                }
                else if(currentTab == 1)
                {
                    change.set(3);
                    db.header.change(0,0,0,iweight);
                    db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),iweight);
                    v.setinteger("df::weight",iweight);
                }
                else if(currentTab == 2)
                {
                    change.set(0);
                    db.header.change(colour,0,0,0);
                    db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                    v.setinteger("df::colour",colour);
                }
                else if(currentTab == 3)
                {
                     change.set(2);
                     db.header.change(0,0,style,0);
                     db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                     v.setinteger("df::style",style);
                }
                if (cadwindow->getlayerwindow() != 0)
                   cadwindow->getlayerwindow()->paintevent();
                 cadwindow->updatetitle();
            }
            else
            {
                 // cloning only the current tab properties to the pre-selected enitities
                 list = selectionlist.copy();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 RemoveLockedEntities(&list,0);
                 for (list.start() ; (e = list.next()) != NULL ; )
                 {
                     if (e->isa(figure_entity))
                        cadwindow->invalidatedisplaylist(e);
                     if(currentTab == 0)
                     {
                         change.set(1);
                         e->change(change,0,layer,0,0);
                     }
                     else if(currentTab == 1)
                     {
                         change.set(3);
                         e->change(change,0,0,0,iweight);
                     }
                     else if(currentTab == 2)
                     {
                         change.set(0);
                         e->change(change,colour,0,0,0);
                     }
                     else if(currentTab == 3)
                     {
                         change.set(2);
                         e->change(change,0,0,style,0);
                     }
              }
              list.destroy();
              db.saveundo(UD_ENDBLOCK,NULL);
            }
            state.setselection(selectionlist,1);
            selectionlist = elist;
       }
       *cstate = ExitState;
       break;
     case 100 : // start point of line pattern
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 101;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 101 : // end point of line pattern
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            if (p2.x - p1.x < 0.001)
              cadwindow->MessageBox("The start and end of the line are too close.","Create Line Pattern",MB_OK);
            else
              {//  Copy the current entities to a list of new lines - to be deleted in a moment
                 for (pelist.start() ; (e = pelist.next()) != 0 ; )
                   {  if (e->isa(line_entity))
                        n = 1;
                      else if (e->isa(circle_entity))
                        n = 5;
                      else if (e->isa(curve_entity))
                        n = 5;
                      else
                        n = 0;

                      if (n > 0)
                        {  p3 = ((LinearEntity *)e)->position(0.0);
                           for (i = 1 ; i <= n ; i++)
                             {  p4 = ((LinearEntity *)e)->position(double(i)/double(n));
                                line = new Line((p3-p1)/(p2.x - p1.x),(p4-p1)/(p2.x - p1.x));
                                db.geometry.add(line,0,0);
                                list.add(line);
                                p3 = p4;
                             }
                        }

                      e->unhighlight();

                   }

                 state.setselection(list.copy(),1);
                 filesaveselectionas(0,(char*)0);
                 state.destroyselection(0);

                 for (list.start() ; (e = list.next()) != 0 ; )
                   db.geometry.del(e,0,0);

              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
//
//  About to enter state actions
//
  if(*cstate == 1)
    state.selmask.entity.clearandset(linear_mask,surface_mask,end_list);
  else if (*cstate == 100 || *cstate == 101)
    state.selmask.entity.clearandset(xyz_mask,end_list);

  if(*cstate == 1)
    cadwindow->prompt->normalprompt("Select the entity to clone the style from");
  else if (*cstate == 100)
    cadwindow->prompt->normalprompt("Locate the start of the line pattern");
  else if (*cstate == 101)
    cadwindow->prompt->normalprompt("Locate the end of the line pattern");
  else if (*cstate == ExitState )
  {
      if(laststate != 1001)
      {
          //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Entity"));
          cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Entity"));

          //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Layer"));
          cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Layer"));

          //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Weight"));
          cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Weight"));

          //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Colour"));
          cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Colour"));

          //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Style"));
          cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Style"));
      }
  }

}


void gp_dialog_command(int *cstate,HighLevelEvent *event,void **s)
{
    static int laststate;
    static EntityList selectionlist;
    static Point p1;
    static EntityList pelist;
    //Dialog dialog("Style_dialog");
    Dialog dialog("MaskChange_Dialog");
    DialogControl *dc;
    EntityList list,elist;
    Entity *e;
    BitMask change(32);
    int i,n,style,colour,layer,iweight,entity,status;
    double weight;
    ResourceString rs36(NCMAIN+36);
    Point p2,p3,p4;
    Line *line;
    int currentTab=0;

    currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());

    if(event == 0)
        WritePrivateProfileString("DialogSettings","GenProps::Parent","Dialog",home.getinifilename());
    else
        WritePrivateProfileString("DialogSettings","GenProps::Parent","",home.getinifilename());

          if (*cstate == 1000 || *cstate == 1001)
          {
              //qDebug() << "called from a dialog";

              switch(currentTab)
              {
              case 0:
                  layer = *((int *)s);
                  v.setinteger("ss::layer",layer);
                  break;
              case 1:
                  weight = *((double *)s);
                  iweight = db.lineweights.mmtoweight(weight);
                  v.setreal("ss::weight",weight);
                  break;
              case 2:
                  colour = *((int *)s);
                  v.setinteger("ss::colour",colour);
                  break;
              case 3:
                  style = *((int *)s);
                  v.setinteger("ss::style",style);
                  break;
              case 4:
                  entity = *((int *)s);
                  v.setinteger("ss::entity",entity);
              }
          }
          else
          {
              //qDebug() << "called from a button";

              if (! state.getselection().empty())
              {
                  if(state.getselection().length() > 1)
                  {
                      layer = -1;//v.getinteger("df::layer");
                      v.setinteger("ss::layer",layer);

                      iweight = -1;//v.getinteger("df::weight");
                      weight = -1;;//db.lineweights.weighttomm(iweight);
                      v.setreal("ss::weight",weight);

                      colour = -1;//v.getinteger("df::colour");
                      v.setinteger("ss::colour",colour);

                      style = -1;//v.getinteger("df::style");
                      v.setinteger("ss::style",style);
                  }
                  else if(state.getselection().length() == 1)
                  {
                      list = state.getselection().copy();
                      list.start();
                      e = list.next();

                      layer = e->getlayer();
                      v.setinteger("ss::layer",layer);

                      iweight = e->getweight();
                      weight = db.lineweights.weighttomm(iweight);
                      v.setreal("ss::weight",weight);

                      colour = e->getcolour();
                      v.setinteger("ss::colour",colour);

                      style = e->getstyle();
                      v.setinteger("ss::style",style);
                  }
              }
              else
              {
                   layer = v.getinteger("df::layer");
                   v.setinteger("ss::layer",layer);

                   iweight = v.getinteger("df::weight");
                   weight = db.lineweights.weighttomm(iweight);
                   v.setreal("ss::weight",weight);

                   colour = v.getinteger("df::colour");
                   v.setinteger("ss::colour",colour);

                   style = v.getinteger("df::style");
                   v.setinteger("ss::style",style);
               }
               //entity = v.getinteger("df::colour");
               //v.setinteger("ss::colour",colour);
          }

           // tab buttons
          dialog.add(new GPButtonDialogControl(101));
          dialog.add(new GPButtonDialogControl(102));
          dialog.add(new GPButtonDialogControl(103));
          dialog.add(new GPButtonDialogControl(104));
          dialog.add(new GPButtonDialogControl(105));
          // change button
          dialog.add(new GPChangeButtonDialogControl(106));
          //dialog.add(new DefineStyleButtonDialogControl(106));
          //dialog.add(new DefineColourButtonDialogControl(102));
          //dialog.add(new DefineColourButtonDialogControl(103));
          //dialog.add(new DefinePatternStyleButtonDialogControl(103));
          if (event != 0) // clone button not used with style sheets
            dialog.add(new ButtonDialogControl(107));

          dialog.add(new GPDefinePatternButtonDialogControl(111));

          dialog.setposition(DPTR);

          if ((status = dialog.process()) == TRUE)
          {
               WritePrivateProfileString("DialogSettings","GenProps::Returned","OK",home.getinifilename());

               if (*cstate == 1000 || *cstate == 1001)
               {
                   //qDebug() << "called from another dialog";
                   // OK when called from another dialog
                   if(currentTab == 0)
                   {
                       layer = v.getinteger("ss::layer");
                       *((int *)s) = layer;
                   }
                   else if(currentTab == 1)
                   {
                       weight = v.getreal("ss::weight");
                       iweight = db.lineweights.mmtoweight(weight);
                       *((double *)s) = weight;
                   }
                   else if(currentTab == 2)
                   {
                       colour = v.getinteger("ss::colour");
                       *((int *)s) = colour;
                   }
                   else if(currentTab = 3)
                   {
                    style = v.getinteger("ss::style");
                   *((int *)s) = style;
                   }
                   laststate = *cstate;
               }
               else if (state.getselection().empty())
               {
                    // normal OK behaviour
                    layer = v.getinteger("ss::layer");
                    v.setinteger("df::layer",layer);

                    weight = v.getreal("ss::weight");
                    iweight = db.lineweights.mmtoweight(weight);
                    v.setinteger("df::weight",iweight);

                    colour = v.getinteger("ss::colour");
                    v.setinteger("df::colour",colour);

                    style = v.getinteger("ss::style");
                    v.setinteger("df::style",style);

                    change.set(0);
                    change.set(1);
                    change.set(2);
                    change.set(3);
                    db.header.change(colour,layer,style,iweight);
                    cadwindow->updatetitle();
               }
               else
               {
                    // change preselected entities
                    // normal OK behaviour
                    layer = v.getinteger("ss::layer");
                    change.set(1,v.getinteger("ss::layerSet"));

                    weight = v.getreal("ss::weight");
                    iweight = db.lineweights.mmtoweight(weight);
                    change.set(3,v.getinteger("ss::weightSet"));

                    colour = v.getinteger("ss::colour");
                    change.set(0,v.getinteger("ss::colourSet"));

                    style = v.getinteger("ss::style");
                    change.set(2,v.getinteger("ss::styleSet"));

                    list = state.getselection().copy();
                    db.saveundo(UD_STARTBLOCK,NULL);
                    RemoveLockedEntities(&list,0);
                    for (list.start() ; (e = list.next()) != NULL ; )
                    {
                        if (e->isa(figure_entity))
                           cadwindow->invalidatedisplaylist(e);
                         e->change(change,colour,layer,style,iweight);
                    }
                    list.destroy();
                    db.saveundo(UD_ENDBLOCK,NULL);

                    cadwindow->paintall(1);
               }
               *cstate = ExitState;
          }
          else if (status == 107) // cloning button was pressed
          {
               currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());
               laststate = *cstate;
               //selectionlist = state.getselection().copy();
               //state.destroyselection(0);
               //*cstate = 100 + currentTab;
               *cstate = 1000;
               CloneInfo cloneInfo; cloneInfo.mode = currentTab;
               //clone_command(cstate,0,(void**)&cloneInfo);
               state.sendevent(new NewCommandEvent(216,0));

          }
          else if (status == 111) // define pattern style button was pressed
          {
               //pelist = state.getselection().copy();
               //state.destroyselection(0);
               *cstate = status;
          }
          else
          {
             WritePrivateProfileString("DialogSettings","GenProps::Returned","Cancel",home.getinifilename());
             *cstate = ExitState;
          }


         if(laststate != 1001)
         {
             //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Entity"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Entity"));

             //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Layer"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Layer"));

             //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Weight"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Weight"));

             //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Colour"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Colour"));

             //cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Style"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Style"));
         }

}

int calcCursorFromBackground()
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

/**
 * @brief background_colour_command
 * @param cstate
 * @param event
 * @param s
 */
void background_colour_command(int *cstate,HighLevelEvent *event,void **s)
{
    //qDebug() << "Entering : background_colour_command";
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    //qDebug() << "cstate : " << *cstate;

    //index = v.getinteger("ss::colour");
    //qDebug() << "GPChange... current index : " << index;

    //index = db.header.getcolour();
    index = -1;
    red   = ::v.getreal("wn::backgroundcolour.red");
    green = ::v.getreal("wn::backgroundcolour.green");
    blue  = ::v.getreal("wn::backgroundcolour.blue");

    //qDebug() << "GPChange... current colour : red : " << red << " green : " << green << " blue : " << blue;
    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Background Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc,0,255));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc,0,255));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc,0,255));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc,0,255));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc,0,255));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc,0,255));

    OtherColourDisplayDialogControl *pddc;
    //OtherColourSampleDialogControl *sddc;
    BackColourDialogSampleControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    //dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    //dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    //dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,1));

    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = ::v.getreal("wn::backgroundcolour.red") ;
    lastGreen = ::v.getreal("wn::backgroundcolour.green") ;
    lastBlue  = ::v.getreal("wn::backgroundcolour.blue") ;

    if(dialog.process() == TRUE)
    {
        ::v.setreal("wn::backgroundcolour.red",(double)red);
        ::v.setreal("wn::backgroundcolour.green",(double)green);
        ::v.setreal("wn::backgroundcolour.blue",(double)blue);


        // set the scene background for instant update
        cadwindow->setbackground(RGB(red*255.0+0.5,green*255.0+0.5,blue*255.0+0.5));
        cadwindow->getcurrentwindow()->setbackground(RGB(red*255.0+0.5,green*255.0+0.5,blue*255.0+0.5));
        cadwindow->getScene()->setBackgroundBrush(QBrush(QColor(red*255.0+0.5,green*255.0+0.5,blue*255.0+0.5)));
        cadwindow->paintall();

        qDebug() << "set backcol GV Rect " << ((QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->rect();
        qDebug() << "set backcol GVVP Rect " << ((QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->viewport()->rect();
        qDebug() << "set backcol GV sceneRect " << ((QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->sceneRect();
        qDebug() << "set backcol  sceneRect " << cadwindow->getScene()->sceneRect();

        //QRectF frect = ((QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->sceneRect();
        //frect.adjust(1,1,frect.bottomRight().x()-frect.width()/2,frect.bottomRight().y()-frect.height()/2);
        //cadwindow->getScene()->addRect(frect);
        //cadwindow->getScene()->addRect(QRectF(0,0,20,20));
        //cadwindow->getScene()->addRect(QRectF(frect.center().x()-20,frect.center().y()-20,frect.center().x(),frect.center().y()));
        //cadwindow->getScene()->addLine(QLineF(frect.topLeft(),frect.bottomRight()));

        RCCOLORREF cl =  cadwindow->getbackground();

        // set the cursor type
        program->updatecursortype(calcCursorFromBackground()); // reversed cursor
    }
    else
    {
        // reset the state
        ::v.setreal("wn::backgroundcolour.red",(double)lastRed);
        ::v.setreal("wn::backgroundcolour.green",(double)lastGreen);
        ::v.setreal("wn::backgroundcolour.blue",(double)lastBlue);

        // set the scene background for instant update
        db.header.setcolour(lastIndex);
        cadwindow->setbackground(RGB(red*255.0+0.5,green*255.0+0.5,blue*255.0+0.5));
        cadwindow->getcurrentwindow()->setbackground(RGB(red*255.0+0.5,green*255.0+0.5,blue*255.0+0.5));
        cadwindow->getScene()->setBackgroundBrush(QBrush(QColor(lastRed*255.0+0.5,lastGreen*255.0+0.5,lastBlue*255.0+0.5)));
        cadwindow->paintall();
    }

    db.setnumericdisplaytolerance(n);
    *cstate = ExitState;

}


/**
 * @brief select_colour_command
 * @param cstate
 * @param event
 * @param s
 */
void select_colour_command(int *cstate,HighLevelEvent *event,void **s)
{
    //qDebug() << "Entering : background_colour_command";
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    //qDebug() << "cstate : " << *cstate;

    //index = v.getinteger("ss::colour");
    //qDebug() << "GPChange... current index : " << index;

    //index = db.header.getcolour();
    index = -1;
    red   = ::v.getreal("db::selectcolour.red");
    green = ::v.getreal("db::selectcolour.green");
    blue  = ::v.getreal("db::selectcolour.blue");

    //qDebug() << "GPChange... current colour : red : " << red << " green : " << green << " blue : " << blue;

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Select Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc,0,255));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc,0,255));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc,0,255));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc,0,255));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc,0,255));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc,0,255));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    //BackColourDialogSampleControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    //dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    //dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    //dialog.add((DisplayDialogControl *)(sddc = new BackColourDialogSampleControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,2));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastRed   = ::v.getreal("db::selectcolour.red") ;
    lastGreen = ::v.getreal("db::selectcolour.green") ;
    lastBlue  = ::v.getreal("db::selectcolour.blue") ;

    if(dialog.process() == TRUE)
    {
        ::v.setreal("db::selectcolour.red",(double)red);
        ::v.setreal("db::selectcolour.green",(double)green);
        ::v.setreal("db::selectcolour.blue",(double)blue);
    }
    else
    {
        // reset the state
        ::v.setreal("db::selectcolour.red",(double)lastRed);
        ::v.setreal("db::selectcolour.green",(double)lastGreen);
        ::v.setreal("db::selectcolour.blue",(double)lastBlue);
    }

    db.setnumericdisplaytolerance(n);
    *cstate = ExitState;

}

#define GPUSER_EVENT 1000

void gp_dialog_launch()
{

}

void gp_execute_command(int *cstate,HighLevelEvent *event,void **s)
{
#if 0
    int status,exstate;
    UserEvent *exevent = new UserEvent(GPUSER_EVENT) ;

    if(*cstate == InitialState)
    {
        exstate = InitialState;
        general_property_command(&exstate,exevent,0);
        qDebug() << "cstate from gp : " << exstate;

        if(exstate == 100)// clone layer
            qDebug() << "clone layer";
        else if(exstate == 101)// clone weight
            qDebug() << "clone weight";
        else if(exstate == 102)// clone colour
            qDebug() << "clone colour";
        else if(exstate == 103)// clone style
            qDebug() << "clone style";
        else if(exstate == 111) // define pattern style
            qDebug() << "do define pattern style : ";
    }
    *cstate = ExitState;
#endif
    int exstate;
    UserEvent *exevent = new UserEvent(GPUSER_EVENT) ;

    static int laststate;
    static EntityList selectionlist;
    static Point p1;
    static EntityList pelist;
    //Dialog dialog("Style_dialog");
    Dialog dialog("MaskChange_Dialog");
    DialogControl *dc;
    EntityList list,elist;
    Entity *e;
    BitMask change(32);
    int i,n,style,colour,layer,iweight,entity,status;
    double weight;
    ResourceString rs36(NCMAIN+36);
    Point p2,p3,p4;
    Line *line;
    int currentTab=0;

    //currentTab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());

    //if(event == 0)
    //    WritePrivateProfileString("DialogSettings","GenProps::Parent","Dialog",home.getinifilename());
    //else
    //    WritePrivateProfileString("DialogSettings","GenProps::Parent","",home.getinifilename());

     switch(*cstate)
     {
          case InitialState:
          exstate = InitialState;
          general_property_command(&exstate,exevent,0);
          qDebug() << "cstate from gp : " << exstate;

          //RCCHAR dlgReturn[300];
          //GetPrivateProfileString("DialogSettings","GenProps::Returned","Cancel",dlgReturn,300,home.getinifilename());

          //selectionlist = state.getselection().copy();
          //state.destroyselection(0);
          if(exstate == 100)// clone layer
              qDebug() << "clone layer";
          else if(exstate == 101)// clone weight
              qDebug() << "clone weight";
          else if(exstate == 102)// clone colour
              qDebug() << "clone colour";
          else if(exstate == 103)// clone style
              qDebug() << "clone style";
          else if(exstate == 111) // define pattern style
          {
              qDebug() << "do define pattern style : ";
              pelist = state.getselection().copy();
              state.destroyselection(0);
              *cstate = 100;
          }
          else if( exstate == ExitState )
              *cstate = exstate;

          // WritePrivateProfileString("DialogSettings","GenProps::Returned","Cancel",home.getinifilename());
          // *cstate = ExitState;
         break;
        case 1 : // cloning change
          if (event->isa(EntitySelected))
          {
#if 0
              //qDebug() << "cloning entity properties";
              //qDebug() << "GP dialog end OK : currentTab : " << currentTab;

              // normal OK behaviour
              layer = ((EntitySelectedEvent *) event)->getentity()->getlayer();
              v.setinteger("ss::layer",layer);
              //qDebug() << "dialog ok cloned layer : " << layer;

              iweight = ((EntitySelectedEvent *) event)->getentity()->getweight();
              weight = db.lineweights.weighttomm(iweight);
              v.setreal("ss::weight",weight);
              //qDebug() << "dialog ok cloned weight : " << weight;

              colour = ((EntitySelectedEvent *) event)->getentity()->getcolour();
              v.setinteger("ss::colour",colour);
              //qDebug() << "dialog ok colned colour : " << colour;

              style = ((EntitySelectedEvent *) event)->getentity()->getstyle();
              v.setinteger("ss:sstyle",style);
              //qDebug() << "dialog ok cloned style : " << style;

              //entity = v.getinteger("df::colour");
              //v.setinteger("ss::colour",colour);


               if (laststate == 1000)
               {
                   if(currentTab == 0)
                       *((int *)s) = layer;
                   else if(currentTab == 1)
                       *((double *)s) = weight;
                   else if(currentTab == 2)
                       *((int *)s) = colour;
                   else if(currentTab == 3)
                       *((int *)s) = style;
               }
               else if (selectionlist.empty())
               {
                    // cloning only the current tab properties to the defaults
                    //db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                   if(currentTab == 0)
                   {
                       change.set(1);
                       db.header.change(0,layer,0,0);
                       db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
                       v.setinteger("df::layer",layer);
                   }
                   else if(currentTab == 1)
                   {
                       change.set(3);
                       db.header.change(0,0,0,iweight);
                       db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),iweight);
                       v.setinteger("df::weight",iweight);
                   }
                   else if(currentTab == 2)
                   {
                       change.set(0);
                       db.header.change(colour,0,0,0);
                       db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
                       v.setinteger("df::colour",colour);
                   }
                   else if(currentTab == 3)
                   {
                        change.set(2);
                        db.header.change(0,0,style,0);
                        db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
                        v.setinteger("df::style",style);
                   }
                   if (cadwindow->getlayerwindow() != 0)
                      cadwindow->getlayerwindow()->paintevent();
                    cadwindow->updatetitle();
               }
               else
               {
                    // cloning only the current tab properties to the pre-selected enitities
                    list = selectionlist.copy();
                    db.saveundo(UD_STARTBLOCK,NULL);
                    RemoveLockedEntities(&list,0);
                    for (list.start() ; (e = list.next()) != NULL ; )
                    {
                        if (e->isa(figure_entity))
                           cadwindow->invalidatedisplaylist(e);
                        if(currentTab == 0)
                        {
                            change.set(1);
                            e->change(change,0,layer,0,0);
                        }
                        else if(currentTab == 1)
                        {
                            change.set(3);
                            e->change(change,0,0,0,iweight);
                        }
                        else if(currentTab == 2)
                        {
                            change.set(0);
                            e->change(change,colour,0,0,0);
                        }
                        else if(currentTab == 3)
                        {
                            change.set(2);
                            e->change(change,0,0,style,0);
                        }
                 }
                 list.destroy();
                 db.saveundo(UD_ENDBLOCK,NULL);
               }
               state.setselection(selectionlist,1);
               selectionlist = elist;
#endif
          }
          *cstate = ExitState;
          break;
        case 100 : // start point of line pattern
          if (event->isa(Coordinate3d))
            {  p1 = ((Coordinate3dEvent *) event)->getp();
               *cstate = 101;
            }
          else if (event->isa(Abort) || event->isa(Exit))
            *cstate = ExitState;
          break;
        case 101 : // end point of line pattern
          if (event->isa(Coordinate3d))
            {  p2 = ((Coordinate3dEvent *) event)->getp();
               if (p2.x - p1.x < 0.001)
                 cadwindow->MessageBox("The start and end of the line are too close.","Create Line Pattern",MB_OK);
               else
                 {//  Copy the current entities to a list of new lines - to be deleted in a moment
                    for (pelist.start() ; (e = pelist.next()) != 0 ; )
                      {  if (e->isa(line_entity))
                           n = 1;
                         else if (e->isa(circle_entity))
                           n = 5;
                         else if (e->isa(curve_entity))
                           n = 5;
                         else
                           n = 0;

                         if (n > 0)
                           {  p3 = ((LinearEntity *)e)->position(0.0);
                              for (i = 1 ; i <= n ; i++)
                                {  p4 = ((LinearEntity *)e)->position(double(i)/double(n));
                                   line = new Line((p3-p1)/(p2.x - p1.x),(p4-p1)/(p2.x - p1.x));
                                   db.geometry.add(line,0,0);
                                   list.add(line);
                                   p3 = p4;
                                }
                           }

                         e->unhighlight();

                      }

                    state.setselection(list.copy(),1);
                    filesaveselectionas(0,(RCCHAR*)0);
                    state.destroyselection(0);

                    for (list.start() ; (e = list.next()) != 0 ; )
                      db.geometry.del(e,0,0);

                 }
               *cstate = ExitState;
            }
          else if (event->isa(Abort) || event->isa(Exit))
            *cstate = ExitState;
          break;
       }
       //
       //  About to enter state actions
       //
     if(*cstate == 1)
       state.selmask.entity.clearandset(linear_mask,surface_mask,end_list);
     else if (*cstate == 100 || *cstate == 101)
       state.selmask.entity.clearandset(xyz_mask,end_list);

     if(*cstate == 1)
       cadwindow->prompt->normalprompt("Select the entity to clone from");
     else if (*cstate == 100)
       cadwindow->prompt->normalprompt("Locate the start of the line pattern");
     else if (*cstate == 101)
       cadwindow->prompt->normalprompt("Locate the end of the line pattern");
     else if (*cstate == ExitState )
     {
         /*
         if(laststate != 1001)
         {
             cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Entity"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Entity"));

             cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Layer"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Layer"));

             cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Weight"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Weight"));

             cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Colour"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Colour"));

             cadwindow->styleentry->selectButton(cadwindow->styleentry->getButton("Style"));
             cadwindow->styleentry->deselectButton(cadwindow->styleentry->getButton("Style"));
         }
         */
     }
}
