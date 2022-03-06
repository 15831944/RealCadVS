
#include "ncwin.h"
#include "RCOpenGL.h"
#include "rcglprinterwidget.h"
#include "RCView.h"
#include "rcprintpreview_dialog.h"

static PRINTDLG printdlg;
static int printdlgvalid = 0;


Printer::Printer(int lp)
{
  Surface::sizeevent(1000,1000);
  printscale = 1.0;
  npens = v.getinteger("pl::npens");
  blackandwhite = v.getinteger("pl::blackandwhite");
  status = 0;
  lineprinter = lp;
  columns = 0;  ncolumns = 0;
}

void Printer::initialize(void)
{
  DEVMODE *devmode;

  memset(&pd,0,sizeof(PRINTDLG));
  pd.lStructSize = sizeof(PRINTDLG);
  pd.hwndOwner = cadwindow->gethwnd();
  pd.hDevMode = NULL;
  pd.hDevNames = NULL;
  pd.hInstance = program->gethinst();
  pd.nFromPage = 1;
  pd.nToPage = 1;
  pd.nMinPage = 1;
  pd.nMaxPage = 9999;
  if (printdlgvalid)
    pd = printdlg;
  else
    {  pd.Flags = PD_RETURNDEFAULT;
       PrintDlg(&pd);
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       if (v.getinteger("pr::papersize") >= 0)
         devmode->dmPaperSize = v.getinteger("pr::papersize");
       if (v.getinteger("pr::orientation") >= 0)
         devmode->dmOrientation = v.getinteger("pr::orientation");
       GlobalUnlock(pd.hDevMode);
    }

  pd.Flags = PD_PRINTSETUP;
  if (PrintDlg(&pd) != 0)
    {  printdlg = pd;
       printdlgvalid = 1;
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       v.setinteger("pr::papersize",devmode->dmPaperSize);
       v.setinteger("pr::orientation",devmode->dmOrientation);
       GlobalUnlock(pd.hDevMode);
    } 
}

void Printer::getdefault(int portrait)
{int i;
#if ! defined(_WIN32_WCE)

 DEVMODE *devmode;
 DEVNAMES *devnames;

  if (status == 1)
    {  DeleteDC(pd.hDC);
       status = 0;
    } 
            
  memset(&pd,0,sizeof(PRINTDLG));
  pd.lStructSize = sizeof(PRINTDLG);
  pd.hwndOwner = cadwindow->gethwnd();
  pd.hDevMode = NULL;
  pd.hDevNames = NULL;
  pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
  pd.hInstance = program->gethinst();
  pd.nFromPage = 1;
  pd.nToPage = 1;
  pd.nMinPage = 1;
  pd.nMaxPage = 9999;

  if (printdlgvalid)
    {  pd = printdlg;
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       devnames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
       pd.hDC = CreateDC(0,(TCHAR*)devnames+devnames->wDeviceOffset,0,devmode);
       GlobalUnlock(pd.hDevMode);
       GlobalUnlock(pd.hDevNames);
    }
  else
    {  pd.Flags = PD_RETURNDEFAULT;
       PrintDlg(&pd);
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       if (v.getinteger("pr::papersize") >= 0)
         devmode->dmPaperSize = v.getinteger("pr::papersize");
       if (v.getinteger("pr::orientation") >= 0)
         devmode->dmOrientation = v.getinteger("pr::orientation");
       GlobalUnlock(pd.hDevMode);

       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       devnames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
       pd.hDC = CreateDC(0,(TCHAR*)devnames+devnames->wDeviceOffset,0,devmode);
       GlobalUnlock(pd.hDevMode);
       GlobalUnlock(pd.hDevNames);
       
    }

  if (pd.hDC == 0)
    {  pd.hDevMode = NULL;
       pd.hDevNames = NULL;
       pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
       if (PrintDlg(&pd) == 0)
         pd.hDC = 0;
    }

  if (pd.hDC != 0)
    {  for (i = 0 ; i < 2 ; i++)
         {  physwidth = GetDeviceCaps(pd.hDC, PHYSICALWIDTH) / double(GetDeviceCaps(pd.hDC,LOGPIXELSX)) * 25.4;
            physheight = GetDeviceCaps(pd.hDC, PHYSICALHEIGHT) / double(GetDeviceCaps(pd.hDC,LOGPIXELSY)) * 25.4;
            xoffset = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            twidth = GetDeviceCaps(pd.hDC, HORZSIZE);
            theight = GetDeviceCaps(pd.hDC, VERTSIZE);
            xdotspermm = double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE));
            ydotspermm = double(GetDeviceCaps(pd.hDC,VERTRES)) / double(GetDeviceCaps(pd.hDC,VERTSIZE));
            if (i == 0 && portrait && physwidth > physheight)
              {  devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
                 devnames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
                 devmode->dmOrientation = DMORIENT_PORTRAIT;
                 DeleteDC(pd.hDC);
                 pd.hDC = CreateDC(0,(TCHAR*)devnames+devnames->wDeviceOffset,0,devmode);
              }
            else if (i == 0 && ! portrait && physwidth < physheight)
              {  devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
                 devnames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
                 devmode->dmOrientation = DMORIENT_LANDSCAPE;
                 DeleteDC(pd.hDC);
                 pd.hDC = CreateDC(0,(TCHAR*)devnames+devnames->wDeviceOffset,0,devmode);
              }
            else
              {  status = 1;
                 break;
              }
         }

       printdlg = pd;
       printdlgvalid = 1;    
       
    }
  else
//WCEFIX
#endif
    status = 0;
}

void Printer::setupprinter(void)
{PRINTDLG newpd;
 DEVMODE *devmode;

#if ! defined(_WIN32_WCE)
  memcpy(&newpd,&pd,sizeof(PRINTDLG));

  if (printdlgvalid)
    newpd = printdlg;
  else
    {  newpd.Flags = PD_RETURNDEFAULT;
       PrintDlg(&newpd);
       devmode = (LPDEVMODE)GlobalLock(newpd.hDevMode);
       if (v.getinteger("pr::papersize") >= 0)
         devmode->dmPaperSize = v.getinteger("pr::papersize");
       if (v.getinteger("pr::orientation") >= 0)
         devmode->dmOrientation = v.getinteger("pr::orientation");
       GlobalUnlock(pd.hDevMode);
    }

  newpd.Flags = PD_PRINTSETUP | PD_RETURNDC | PD_NOSELECTION | PD_NOPAGENUMS;
  if (PrintDlg(&newpd) == TRUE)
    {  printdlg = newpd;
       printdlgvalid = 1;
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       v.setinteger("pr::papersize",devmode->dmPaperSize);
       v.setinteger("pr::orientation",devmode->dmOrientation);
       GlobalUnlock(pd.hDevMode);
       abort();  //  Delete the old device context
       memcpy(&pd,&newpd,sizeof(PRINTDLG));
       physwidth = GetDeviceCaps(pd.hDC, PHYSICALWIDTH) / double(GetDeviceCaps(pd.hDC,LOGPIXELSX)) * 25.4;
       physheight = GetDeviceCaps(pd.hDC, PHYSICALHEIGHT) / double(GetDeviceCaps(pd.hDC,LOGPIXELSY)) * 25.4;
       xoffset = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
       yoffset = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
       twidth = GetDeviceCaps(pd.hDC, HORZSIZE);
       theight = GetDeviceCaps(pd.hDC, VERTSIZE);
       xdotspermm = double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE));
       ydotspermm = double(GetDeviceCaps(pd.hDC,VERTRES)) / double(GetDeviceCaps(pd.hDC,VERTSIZE));
       status = 1;
    }
#endif
}

static double plot_scale;
static int number_pens,black_and_white,deviceid;

extern "C" UINT FAR PASCAL printcb(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM);
typedef UINT (FAR PASCAL *PRINTCB)(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM);

#if ! defined(_WIN32_WCE)
UINT FAR PASCAL printcb(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM)
{RCCHAR   *eptr;
 double  temp;
 long l;
 RCCHAR string[300],device[300],devinfo[300],*driver,*port;
 static RCCHAR lastdevice[300];
 RCHDC hdc;
  switch (iMessage)
    {  case WM_INITDIALOG:
         sprintf(string,"%.1f",plot_scale);
         SetDlgItemText(hDlg,2000,string);
         sprintf(string,"%d",number_pens);
         SetDlgItemText(hDlg,2001,string);
         CheckDlgButton(hDlg,2002,black_and_white);
         SetFocus(GetDlgItem(hDlg,2000));
         SendDlgItemMessage(hDlg,2000,EM_SETSEL,0,MAKELONG(0, -1));
         GetDlgItemText(hDlg,deviceid,device,300);
         strcpy(lastdevice,device);
         if (deviceid == 1088)  //  16 bit dialog, try to extract printer info.
           {  if (strchr(device,'(') != 0)
                strcpy(string,strchr(device,'(')+1);
              else
                strcpy(string,device);
              if (strrchr(string,' ') != 0)
                *strrchr(string,' ') = 0;
              if (strrchr(string,' ') != 0)
                *strrchr(string,' ') = 0;
              strcpy(device,string);
              GetProfileString(_RCT("PrinterPorts"),device,_RCT(""),devinfo,300);
              driver = devinfo;
              if (strchr(driver,',') != 0)
                {  port = strchr(driver,',') + 1;
                   *strchr(driver,',') = 0;
                   if (strchr(port,',') != 0)
                     *strchr(port,',') = 0;
                }
              else
                port = driver;
           }
         else
           driver = port = _RCT("");
         if ((hdc = CreateIC(driver,device,port,0)) != 0)
           {  EnableWindow(GetDlgItem(hDlg,2001),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
              EnableWindow(GetDlgItem(hDlg,2003),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
              DeleteDC(hdc);
           }
         break;
       case WM_COMMAND:
         if (wParam == IDOK)
           {ResourceString rsnc0(NC);
            ResourceString rs0(NCPRINT),rs1(NCPRINT+1),rs2(NCPRINT+2);
            ResourceString rs3(NCPRINT+3),rs8(NCPRINT+8),rs9(NCPRINT+9);
              GetDlgItemText(hDlg,2000,string,128);
              temp = strtod(string,&eptr);
              if (*eptr != 0)
                { RCMessageBox(hDlg,rs0.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              else if (temp < db.getptoler())
                { RCMessageBox(hDlg,rs1.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              else if (temp > 1000.0)
                { RCMessageBox(hDlg,rs2.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              plot_scale = temp;
              GetDlgItemText(hDlg,2001,string,128);
              l = strtol(string,&eptr,10);
              if (*eptr != 0)
                { RCMessageBox(hDlg,rs0.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              else if (l < 1)
                { RCMessageBox(hDlg,rs8.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              else if (l > 10000)
                { RCMessageBox(hDlg,rs9.gets(),rsnc0.gets(),MB_ICONINFORMATION);
                   return (TRUE);
                }
              number_pens = int(l);
              black_and_white = IsDlgButtonChecked(hDlg,2002);
           }
         else if (HIWORD(wParam) == CBN_SELENDOK && LOWORD(wParam) == deviceid)
           {  GetDlgItemText(hDlg,deviceid,device,300);
              if ((hdc = CreateIC(_RCT(""),device,_RCT(""),0)) != 0)
                {  EnableWindow(GetDlgItem(hDlg,2001),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
                   EnableWindow(GetDlgItem(hDlg,2003),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
                   DeleteDC(hdc);
                }
           }
         break;
       default :
         if (deviceid == 1088)
           {  GetDlgItemText(hDlg,deviceid,device,300);
              if (strcmp(device,lastdevice) == 0)
                break;
              strcpy(lastdevice,device);
              if (strchr(device,'(') != 0)
                strcpy(string,strchr(device,'(')+1);
              else
                strcpy(string,device);
              if (strrchr(string,' ') != 0)
                *strrchr(string,' ') = 0;
              if (strrchr(string,' ') != 0)
                *strrchr(string,' ') = 0;
              strcpy(device,string);
              GetProfileString(_RCT("PrinterPorts"),device,_RCT(""),devinfo,300);
              driver = devinfo;
              if (strchr(driver,',') != 0)
                {  port = strchr(driver,',') + 1;
                   *strchr(driver,',') = 0;
                   if (strchr(port,',') != 0)
                     *strchr(port,',') = 0;
                }
              else
                port = driver;
              if ((hdc = CreateIC(driver,device,port,0)) != 0)
                {  EnableWindow(GetDlgItem(hDlg,2001),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
                   EnableWindow(GetDlgItem(hDlg,2003),GetDeviceCaps(hdc,TECHNOLOGY) == DT_PLOTTER);
                   DeleteDC(hdc);
                }
           }
         break;
    }
  return FALSE;
}
#endif

int cad_version = VERSION | (MENUS<<4);

void Printer::print(int allowselection)
{
  //FARPROC printproc;
 PRINTCB printproc;
 ResourceString rs3(NCPRINT+3);
 ResourceString rs10(NCPRINT+10);
 ResourceString rs11(NCPRINT+11);
 POINT point;
 DEVMODE *devmode;

#if ! defined(_WIN32_WCE)
 short escapenum = GETPRINTINGOFFSET;

  plot_scale = printscale;
  number_pens = npens;
  black_and_white = blackandwhite;
  printproc = MakeProcInstance( printcb,program->gethinst());
  memset(&pd,0,sizeof(PRINTDLG));
  pd.lStructSize = sizeof(PRINTDLG);
  pd.hwndOwner = cadwindow == 0 ? 0 : cadwindow->gethwnd();
  if (! allowselection)
    pd.Flags |= PD_NOSELECTION;
  pd.hInstance = program->gethinst();
  pd.lpfnPrintHook = (LPSETUPHOOKPROC)(UINT (FAR PASCAL *)(HWND,UINT,WPARAM,LPARAM))printproc;
#if defined(WIN32)
  OSVERSIONINFO vi;
  memset(&vi,0,sizeof(vi));
  vi.dwOSVersionInfoSize = sizeof(vi);
  if (GetVersionEx(&vi))
    {  if (vi.dwPlatformId == VER_PLATFORM_WIN32s)
         {  pd.lpPrintTemplateName = _RCT("Print_dialog");  deviceid = 1088;
         }
       else
         {  pd.lpPrintTemplateName = _RCT("Print32_dialog");  deviceid = 1139;
         }
    }
  else
    {  pd.lpPrintTemplateName = _RCT("Print32_dialog");  deviceid = 1139;
    }
#else
  pd.lpPrintTemplateName = _RCT("Print_dialog");  deviceid = 1088;
#endif
  pd.nFromPage = 1;
  pd.nToPage = 1;
  pd.nMinPage = 1;
  pd.nMaxPage = 9999;

  if (printdlgvalid)
    pd = printdlg;
  else
    {  pd.Flags = PD_RETURNDEFAULT;
       PrintDlg(&pd);
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       if (v.getinteger("pr::papersize") >= 0)
         devmode->dmPaperSize = v.getinteger("pr::papersize");
       if (v.getinteger("pr::orientation") >= 0)
         devmode->dmOrientation = v.getinteger("pr::orientation");
       GlobalUnlock(pd.hDevMode);
    }
  
  pd.lpPrintTemplateName = _RCT("Print32_dialog");  deviceid = 1139;
  pd.lpfnPrintHook = (LPSETUPHOOKPROC)(UINT (FAR PASCAL *)(HWND,UINT,WPARAM,LPARAM))printproc;

  pd.Flags = PD_RETURNDC | PD_NOPAGENUMS |
             PD_ENABLEPRINTTEMPLATE | PD_ENABLEPRINTHOOK;

  if ((status = PrintDlg(&pd)) == TRUE)
    {  pd.lpPrintTemplateName = 0;
       pd.lpfnPrintHook = 0;
       printdlg = pd;
       printdlgvalid = 1;
       devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
       v.setinteger("pr::papersize",devmode->dmPaperSize);
       v.setinteger("pr::orientation",devmode->dmOrientation);
       GlobalUnlock(pd.hDevMode);


#if VERSION == 1
         {int w,h;
            w = GetDeviceCaps(pd.hDC,HORZSIZE);
            h = GetDeviceCaps(pd.hDC,VERTSIZE);
            if (w > h && (w > 307 || h > 220) ||
                w < h && (h > 307 || w > 220))
              { RCMessageBox(cadwindow->gethwnd(),rs11.gets(),rs10.gets(),MB_ICONSTOP);
                 di.cbSize = sizeof(DOCINFO);
                 di.lpszDocName = rs3.gets();
                 di.lpszOutput  = NULL;
                 abort();
              }
         }
#endif
       if (status != 0)
         {  selection = (pd.Flags & PD_SELECTION) == PD_SELECTION;
            printscale = plot_scale;
            npens = number_pens;
            blackandwhite = black_and_white;
            v.setinteger("pl::npens",npens);
            v.setinteger("pl::blackandwhite",blackandwhite);
            Surface::sizeevent(GetDeviceCaps(pd.hDC,HORZRES),GetDeviceCaps(pd.hDC,VERTRES));
            sethdc(NULL,pd.hDC);
            if (GetDeviceCaps(pd.hDC,TECHNOLOGY) == DT_PLOTTER)
              allocatepens(npens);
            else
              npens = 0;

            if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const RCCHAR *) &escapenum,0))
              Escape(pd.hDC,GETPRINTINGOFFSET,0,0,&point);
            else
              point.x = point.y = 0;
#if defined(WIN32)
            point.x = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            point.y = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
#endif

            di.cbSize = sizeof(DOCINFO);
            di.lpszDocName = L"CAD Drawing";
            di.lpszOutput  = 0;
#if defined(WIN32)
            di.lpszDatatype = 0;
            di.fwType = 0;
#endif
            status = 1;
         }
    }
  FreeProcInstance(printproc);
#endif
}

void Printer::print(PRINTDLG *user_pd,DOCINFO *user_di)
{ResourceString rs3(NCPRINT+3);
 ResourceString rs10(NCPRINT+10);
 ResourceString rs11(NCPRINT+11);
 POINT point;
 DEVMODE *devmode;
 short escapenum = GETPRINTINGOFFSET;

  pd = *user_pd;
  di = *user_di;

  plot_scale = printscale;
  number_pens = npens;
  black_and_white = blackandwhite;

  printdlg = pd;
  printdlgvalid = 1;
  devmode = (LPDEVMODE)GlobalLock(pd.hDevMode);
  v.setinteger("pr::papersize",devmode->dmPaperSize);
  v.setinteger("pr::orientation",devmode->dmOrientation);
  GlobalUnlock(pd.hDevMode);

  selection = (pd.Flags & PD_SELECTION) == PD_SELECTION;
  printscale = plot_scale;
  npens = number_pens;
  blackandwhite = black_and_white;
  v.setinteger("pl::npens",npens);
  v.setinteger("pl::blackandwhite",blackandwhite);
  Surface::sizeevent(GetDeviceCaps(pd.hDC,HORZRES),GetDeviceCaps(pd.hDC,VERTRES));
  sethdc(NULL,pd.hDC);
  if (GetDeviceCaps(pd.hDC,TECHNOLOGY) == DT_PLOTTER)
    allocatepens(npens);
  else
    npens = 0;

  point.x = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
  point.y = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);

  status = 1;

}


void Printer::firstpage(void)
{
#if ! defined(_WIN32_WCE)
 short escapenum = GETPRINTINGOFFSET;
 POINT point;
  if (status == 1)
    {  if (GetDeviceCaps(pd.hDC,TECHNOLOGY) == DT_PLOTTER)
         allocatepens(npens);
       else
         npens = 0;

       if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const RCCHAR *) &escapenum,0))
         Escape(pd.hDC,GETPRINTINGOFFSET,0,0,&point);
       else
         point.x = point.y = 0;
#if defined(WIN32)
       point.x = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
       point.y = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
#endif

       di.cbSize = sizeof(DOCINFO);
       di.lpszDocName = L"CAD Drawing";
       di.lpszOutput  = 0;
#if defined(WIN32)
       di.lpszDatatype = 0;
       di.fwType = 0;
#endif
       newpage();
    }
#endif
}

void Printer::abort(void)
{
#if ! defined(_WIN32_WCE)
 if (status)
    {  if (lineprinter)
         {  SelectObject(pd.hDC,holdfont);
            DeleteObject(hboldfont);
            DeleteObject(hlargefont);
            DeleteObject(hnormalfont);
            DeleteObject(hsmallfont);
            DeleteObject(hsmallboldfont);
         }
       AbortDoc(pd.hDC);
       DeleteDC(pd.hDC);
       status = 0;
    }
#endif
}

//extern char *llabel;//fg
//extern RCCHAR *llabel;

void Printer::lastpage(void)
{
    RCCHAR rcllabel[100];
    strcpy(rcllabel,llabel);
#if ! defined(_WIN32_WCE)
  if (status != 0)
    {  if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {

#if defined(TRICAD)
          int h = int((GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 11.0 / 72.0 * 25.4);
          RCHFONT hfont,holdfont;
          double xoffset,yoffset;
          SIZE size;

            xoffset = v.getreal("pr::xoffset");
            yoffset = v.getreal("pr::yoffset");
            xoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSX) / 25.4;
            yoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSY) / 25.4;
            SetBkMode(pd.hDC,OPAQUE);
            SetBkColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_TOP | TA_LEFT);
            hfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            GetTextExtentPoint(pd.hDC,rcllabel,strlen(rcllabel),&size);
            if (xoffset + size.cx > GetDeviceCaps(pd.hDC,HORZRES))
              xoffset = GetDeviceCaps(pd.hDC,HORZRES) - size.cx;
            if (yoffset + size.cy > GetDeviceCaps(pd.hDC,VERTRES))
              yoffset = GetDeviceCaps(pd.hDC,VERTRES) - size.cy;
            xoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            if (xoffset < 0.0) xoffset = 0.0;
            if (yoffset < 0.0) yoffset = 0.0;
            TextOut(pd.hDC,int(xoffset),int(yoffset),rcllabel,strlen(rcllabel));
            SelectObject(pd.hDC,holdfont);
#endif
         }

       EndPage(pd.hDC);
       EndDoc(pd.hDC);
       status = 1;
    }
#endif
}

Printer::~Printer()
{
#if ! defined(_WIN32_WCE)
 if (status)
    {  if (lineprinter)
         {  SelectObject(pd.hDC,holdfont);
            DeleteObject(hboldfont);
            DeleteObject(hlargefont);
            DeleteObject(hnormalfont);
            DeleteObject(hsmallfont);
            DeleteObject(hsmallboldfont);
         }
#if VERSION == TRICAD
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {int h = int((GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 11.0 / 72.0 * 25.4);
          RCHFONT hfont,holdfont;
          double xoffset,yoffset;
          SIZE size;
            xoffset = v.getreal("pr::xoffset");
            yoffset = v.getreal("pr::yoffset");
            xoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSX) / 25.4;
            yoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSY) / 25.4;
            SetBkMode(pd.hDC,OPAQUE);
            SetBkColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_TOP | TA_LEFT);
            hfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            GetTextExtentPoint(pd.hDC,llabel,strlen(llabel),&size);
            if (xoffset + size.cx > GetDeviceCaps(pd.hDC,HORZRES))
              xoffset = GetDeviceCaps(pd.hDC,HORZRES) - size.cx;
            if (yoffset + size.cy > GetDeviceCaps(pd.hDC,VERTRES))
              yoffset = GetDeviceCaps(pd.hDC,VERTRES) - size.cy;
            xoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            if (xoffset < 0.0) xoffset = 0.0;
            if (yoffset < 0.0) yoffset = 0.0;
            TextOut(pd.hDC,int(xoffset),int(yoffset),llabel,strlen(llabel));
            SelectObject(pd.hDC,holdfont);
         }
#endif
       EndPage(pd.hDC);
       EndDoc(pd.hDC);
       DeleteDC(pd.hDC);
   }
#endif
}

void Printer::newpage(void)
{
    RCCHAR rcllabel[100];
    strcpy(rcllabel,llabel);

    if (! status) return;
#if ! defined(_WIN32_WCE)

  if (status == 1)
    {  status = 2;
       StartDoc(pd.hDC,&di);
       StartPage(pd.hDC);
       if (lineprinter)
         {int h;
            y = 0;
            columns = 0;  ncolumns = 0;
            h = int(double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 5.644);
            hlargefont = CreateFont(h,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"Arial");
            h = int(double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 4.000);
            hboldfont = CreateFont(h,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"Arial");
            hnormalfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"Arial");
            h = int(double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 3.3);
            hsmallfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"Arial");
            hsmallboldfont = CreateFont(h,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,"Arial");
            holdfont = (RCHFONT) SelectObject(pd.hDC,hnormalfont);
            setmargins(0.0,0.0,0.0,0.0);
         }
#if VERSION != TRICAD
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {int h;
          RCHFONT hfont,holdfont;
          double a;

            h = GetDeviceCaps(pd.hDC,HORZRES) / 15;
            a = atan2((double)GetDeviceCaps(pd.hDC,VERTRES),(double)GetDeviceCaps(pd.hDC,HORZRES))*1800.0/acos(-1.0);
            hfont = CreateFont(h,0,int(a),int(a),FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            SetBkMode(pd.hDC,TRANSPARENT);
            SetBkColor(pd.hDC,RGB(255,255,255));
            SetTextColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_CENTER | TA_BOTTOM);
            TextOut(pd.hDC,GetDeviceCaps(pd.hDC,HORZRES)/2,GetDeviceCaps(pd.hDC,VERTRES)/2,rcllabel,strlen(rcllabel));
            SetTextAlign(pd.hDC,TA_CENTER | TA_TOP);
            TextOut(pd.hDC,GetDeviceCaps(pd.hDC,HORZRES)/2,GetDeviceCaps(pd.hDC,VERTRES)/2,_RCT("RealCAD Student License"),23);
            SelectObject(pd.hDC,holdfont);
         }
#endif


    }
  else
    {  

#if VERSION == TRICAD
        if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {int h = int((GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 11.0 / 72.0 * 25.4);
          RCHFONT hfont,holdfont;
          double xoffset,yoffset;
          SIZE size;
            xoffset = v.getreal("pr::xoffset");
            yoffset = v.getreal("pr::yoffset");
            xoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSX) / 25.4;
            yoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSY) / 25.4;
            SetBkMode(pd.hDC,OPAQUE);
            SetBkColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_TOP | TA_LEFT);
            hfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            GetTextExtentPoint(pd.hDC,llabel,strlen(llabel),&size);
            if (xoffset + size.cx > GetDeviceCaps(pd.hDC,HORZRES))
              xoffset = GetDeviceCaps(pd.hDC,HORZRES) - size.cx;
            if (yoffset + size.cy > GetDeviceCaps(pd.hDC,VERTRES))
              yoffset = GetDeviceCaps(pd.hDC,VERTRES) - size.cy;
            xoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            if (xoffset < 0.0) xoffset = 0.0;
            if (yoffset < 0.0) yoffset = 0.0;
            TextOut(pd.hDC,int(xoffset),int(yoffset),llabel,strlen(llabel));
            SelectObject(pd.hDC,holdfont);
         }
#endif

       EndPage(pd.hDC);
       StartPage(pd.hDC);
    }
  y = 0;
#endif
}

int Printer::getattop(void)
{ return y == 0;
}

void Printer::setmargins(double l,double r,double t,double b)
{double scale;
  if (! status) return;
#if ! defined(_WIN32_WCE)
  scale = double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE));
  left = int(l * scale);
  right = int(GetDeviceCaps(pd.hDC,HORZRES) - r * scale);
  top = int(t * scale);
  bottom = int(GetDeviceCaps(pd.hDC,VERTRES) - b * scale);
#endif
}

void Printer::setcolumns(int n,double *c)
{int i;
 double scale;
  if (! status) return;
#if ! defined(_WIN32_WCE)
  delete columns;
  if ((columns = new int[n]) == 0) return;
  scale = double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE));
  ncolumns = n;
  for (i = 0 ; i < ncolumns ; i++)
    columns[i] = (int)(scale * c[i]);
#endif
}

void Printer::starttable(int,int *)
{ if (! status) return;

}

void Printer::endtable(void)
{ if (! status) return;
}

void Printer::setbold(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hboldfont);
#endif
}

void Printer::setlarge(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hlargefont);
#endif
}

void Printer::setnormal(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hnormalfont);
#endif
}

void Printer::setsmall(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hsmallfont);
#endif
}

void Printer::setsmallbold(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hsmallboldfont);
#endif
}

void Printer::setheadingone(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hnormalfont);
#endif
}

void Printer::setheadingtwo(void)
{ if (! status) return;
#if ! defined(_WIN32_WCE)
  SelectObject(pd.hDC,hnormalfont);
#endif
}

void Printer::overline(int tab1,int tab2)
{RCHPEN hpen,holdpen;
#if ! defined(_WIN32_WCE)
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN) SelectObject(pd.hDC,hpen);
  MoveToEx(pd.hDC,columns[tab1],y+top,0);
  LineTo(pd.hDC,columns[tab2],y+top);
  SelectObject(pd.hDC,holdpen);
  DeleteObject(hpen);
#endif
}

void Printer::printtext(char *line,int tabstop,int just)
{
    printtext(CHAR2RCCHR(line),tabstop,just);
}

void Printer::printtext(RCCHAR *line,int tabstop,int just)
{int x,x1,x2;
 RCCHAR *nexttab,string[512],*s;
  if (! status) return;
#if ! defined(_WIN32_WCE)
  strncpy(string,line,512);  string[511] = 0;  s = string;
  do {  nexttab = strchr(s,'\t');
        if (nexttab != 0)
          *nexttab = 0;
        if (tabstop <= 0 || columns == 0)
          {  x1 = 0;  x2 = right - left;
          }
        else
          {  if (tabstop >= ncolumns)
               {  x1 = columns[ncolumns-1];  x2 = right;
               }
             else
               {  x1 = columns[tabstop-1];  x2 = columns[tabstop];
               }
          }
        if (just > 0)
          {SIZE size;
             GetTextExtentPoint(pd.hDC,s,strlen(s),&size);
             if (just == 1)
               x = (x1 + x2) / 2 - size.cx / 2;
             else
               x = x2 - size.cx;
          }
        else
          x = x1;
        TextOut(pd.hDC,x+left,y+top,s,strlen(s));
        if (nexttab != 0)
          s = nexttab + 1;
        else
          s = 0;
        tabstop++;
     } while (s != 0);
#endif
}

void Printer::printtext(int x,int y,RCCHAR *s)
{
#if ! defined(_WIN32_WCE)
  TextOut(pd.hDC,x,y,s,strlen(s));
#endif
}

void Printer::newline(int nlines)
{int i;
 SIZE size;
  if (! status) return;
#if ! defined(_WIN32_WCE)
  GetTextExtentPoint(pd.hDC,_RCT("ABC"),3,&size);
  for (i = 0 ; i < nlines ; i++)
    {  y = y + size.cy;
       if (y + top > bottom)
         {  

#if VERSION == TRICAD
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {int h = int((GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 11.0 / 72.0 * 25.4);
          RCHFONT hfont,holdfont;
          double xoffset,yoffset;
          SIZE size;
            xoffset = v.getreal("pr::xoffset");
            yoffset = v.getreal("pr::yoffset");
            xoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSX) / 25.4;
            yoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSY) / 25.4;
            SetBkMode(pd.hDC,OPAQUE);
            SetBkColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_TOP | TA_LEFT);
            hfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            GetTextExtentPoint(pd.hDC,llabel,strlen(llabel),&size);
            if (xoffset + size.cx > GetDeviceCaps(pd.hDC,HORZRES))
              xoffset = GetDeviceCaps(pd.hDC,HORZRES) - size.cx;
            if (yoffset + size.cy > GetDeviceCaps(pd.hDC,VERTRES))
              yoffset = GetDeviceCaps(pd.hDC,VERTRES) - size.cy;
            xoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            if (xoffset < 0.0) xoffset = 0.0;
            if (yoffset < 0.0) yoffset = 0.0;
            TextOut(pd.hDC,int(xoffset),int(yoffset),llabel,strlen(llabel));
            SelectObject(pd.hDC,holdfont);
         }
#endif
       EndPage(pd.hDC);
            StartPage(pd.hDC);
            y = 0;
         }
    }
#endif
}

void Printer::nearbottom(int nlines)
{SIZE size;
#if ! defined(_WIN32_WCE)
  if (! status) return;
  GetTextExtentPoint(pd.hDC,_RCT("ABC"),3,&size);
  if (y + top + size.cy * nlines > bottom)
    {  

#if VERSION == TRICAD
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
         {int h = int((GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE)) * 11.0 / 72.0 * 25.4);
          RCHFONT hfont,holdfont;
          double xoffset,yoffset;
          SIZE size;
            xoffset = v.getreal("pr::xoffset");
            yoffset = v.getreal("pr::yoffset");
            xoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSX) / 25.4;
            yoffset *= GetDeviceCaps(pd.hDC,LOGPIXELSY) / 25.4;
            SetBkMode(pd.hDC,OPAQUE);
            SetBkColor(pd.hDC,RGB(220,220,220));
            SetTextAlign(pd.hDC,TA_TOP | TA_LEFT);
            hfont = CreateFont(h,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,TEXT("Arial"));
            holdfont = (RCHFONT) SelectObject(pd.hDC,hfont);
            GetTextExtentPoint(pd.hDC,llabel,strlen(llabel),&size);
            if (xoffset + size.cx > GetDeviceCaps(pd.hDC,HORZRES))
              xoffset = GetDeviceCaps(pd.hDC,HORZRES) - size.cx;
            if (yoffset + size.cy > GetDeviceCaps(pd.hDC,VERTRES))
              yoffset = GetDeviceCaps(pd.hDC,VERTRES) - size.cy;
            xoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
            yoffset -= GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
            if (xoffset < 0.0) xoffset = 0.0;
            if (yoffset < 0.0) yoffset = 0.0;
            TextOut(pd.hDC,int(xoffset),int(yoffset),llabel,strlen(llabel));
            SelectObject(pd.hDC,holdfont);
         }
#endif

       EndPage(pd.hDC);
       StartPage(pd.hDC);
       y = 0;
    }
#endif
}

View3dPrinter::View3dPrinter(View3dSurface *s) :
  Printer(),
  View3dSurface(s->getview(),s->getumin(),s->getvmin(),s->getumax(),s->getvmax(),0,0)
{ printscale = (s->getumax() - s->getumin()) / s->getwidthmm();
  plotscale = s->getplotscale();
  setrepaintstyle(s->getrepaintstyle());
  displaylayertable = view->getdisplaylayertable();
}

View3dPrinter::View3dPrinter(View3dSurface *s,double umin,double vmin,double umax,double vmax) :
  Printer(),
  View3dSurface(s->getview(),umin,vmin,umax,vmax,0,0)
{ scale = (s->getumax() - s->getumin()) / s->getwidthmm();
  plotscale = s->getplotscale();
  displaylayertable = view->getdisplaylayertable();
}

int View3dPrinter::startprint(void)
{  Printer::print(1);
   return getdefined();
}

double View3dPrinter::getphyswidthmm(void)
{
    return GetDeviceCaps(pd.hDC, PHYSICALWIDTH) / xdotspermm;
    /*
#if ! defined(_WIN32_WCE)
#if defined(WIN32)
  return GetDeviceCaps(pd.hDC, PHYSICALWIDTH) / xdotspermm;
#else
 short escapenum = GETPHYSPAGESIZE;
 POINT size;
  if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const char *) &escapenum,0) >= 0 &&
      Escape(pd.hDC,GETPHYSPAGESIZE,0,0,&size) >= 0)
    return size.x / xdotspermm;
  else
    return width / xdotspermm;
#endif
#else
  return 1.0;
#endif
*/
}

double View3dPrinter::getphysheightmm(void)
{
    return GetDeviceCaps(pd.hDC, PHYSICALHEIGHT) / ydotspermm;
    /*
#if ! defined(_WIN32_WCE)
#if defined(WIN32)
  return GetDeviceCaps(pd.hDC, PHYSICALHEIGHT) / ydotspermm;
#else
 short escapenum = GETPHYSPAGESIZE;
 POINT size;
  if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const char *) &escapenum,0) >= 0 &&
      Escape(pd.hDC,GETPHYSPAGESIZE,0,0,&size) >= 0)
    return size.y / ydotspermm;
  else
    return height / ydotspermm;
#endif
#else
  return 1.0;
#endif
*/
}

void View3dPrinter::endprint(double umin1,double vmin1,double umax1,double vmax1)
{int w,h,bw,bh,status,outside,i,nbands,bandheight,drawwp,rotated;
 double x,y,x1,y1,x2,y2,umax2,vmin2,umin3,umax3,vmin3,vmax3;

#if ! defined(_WIN32_WCE)
 ResourceString rs4(NCPRINT+4);
 ResourceString rs5(NCPRINT+5);
 ResourceString rs6(NCPRINT+6);
 POINT origin,size;
//#if defined(WIN32)
  origin.x = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
  origin.y = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
  size.x = GetDeviceCaps(pd.hDC, PHYSICALWIDTH);
  size.y = GetDeviceCaps(pd.hDC, PHYSICALHEIGHT);
  if (size.x > 0 && size.y > 0)
      /*
#else
  short escapenum = GETPRINTINGOFFSET;
  if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const char *) &escapenum,0) >= 0 &&
      Escape(pd.hDC,GETPHYSPAGESIZE,0,0,&size) >= 0 &&
      Escape(pd.hDC,GETPRINTINGOFFSET,0,0,&origin) >= 0 &&
      size.x > 0 && size.y > 0)
#endif
*/
    {
      double vr,hr;
       vr = GetDeviceCaps(gethdc(),VERTRES);
       hr = GetDeviceCaps(gethdc(),HORZRES);
       umin = umin1 + (umax1 - umin1) / (size.x-1) * origin.x;
       vmin = vmin1 + (vmax1 - vmin1) / (size.y-1) * (size.y - 1 - vr - 1 - origin.y);
       umax = umin1 + (umax1 - umin1) / (size.x-1) * (origin.x + hr - 1);
       vmax = vmin1 + (vmax1 - vmin1) / (size.y-1) * (size.y - 1 - origin.y);
       umin = umin1;
       vmin = vmin1;
       umax = umax1;
       vmax = vmax1;
    }
  else
    {  umax = umax1;  vmax = vmax1;  umin = umin1;  vmin = vmin1;
    }
  w = int((umax - umin)  * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
  h = int((vmax - vmin)  * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
  rotated = 0;
  if ((w < h && getheight() < getwidth() || w > h && getheight() > getwidth()) && requiresopengl() == 0)
    {  status =RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs4.gets(),rs5.gets(),MB_YESNOCANCEL);
       if (status == IDYES)
         {  w = int((umax - umin) * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
            h = int((vmax - vmin) * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
            settransform2d(0.0,-1.0,h-1-origin.x,-1.0,0.0,w-1-origin.y);
            rotated = 1;
         }
       else if (status == IDCANCEL)
         {  abort();
            return;
         }
       else
         settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
    }
  else
    settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
  outside = 0;
  x = 1.0;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
  x1 = x2 = x;  y1 = y2 = y;
  outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
  x = w-2;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
  if (x < x1) x1 = x;  if (x > x2) x2 = x;
  if (y < y1) y1 = y;  if (y > y2) y2 = y;
  outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
  x = w-2;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
  if (x < x1) x1 = x;  if (x > x2) x2 = x;
  if (y < y1) y1 = y;  if (y > y2) y2 = y;
  outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
  x = 1.0;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
  if (x < x1) x1 = x;  if (x > x2) x2 = x;
  if (y < y1) y1 = y;  if (y > y2) y2 = y;
  outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
  if (outside)
    {  status =RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs6.gets(),rs5.gets(),MB_YESNO);
       if (status == IDNO)
         {  abort();
            return;
         }
    }
  newpage();
  sizeevent(w,h);
  updatematrix();
  setclip(-1,-1,x2,y2);

  if (requiresopengl())
    {  
       umax2 = umin + w / GetDeviceCaps(gethdc(),LOGPIXELSX) * 25.4 * printscale;
       vmin2 = vmax - h / GetDeviceCaps(gethdc(),LOGPIXELSY) * 25.4 * printscale;

       bw = int((umax2-umin)/printscale*getxdotspermm());
       bh = int((vmax-vmin2)/printscale*getydotspermm());

       umin3 = umin + (umax2 - umin) / bw * GetDeviceCaps(gethdc(),PHYSICALOFFSETX);
       umax3 = umin + (umax2 - umin) / bw * (GetDeviceCaps(gethdc(),PHYSICALOFFSETX) + w);
       vmin3 = vmin2 + (vmax - vmin2) / bh * GetDeviceCaps(gethdc(),PHYSICALOFFSETY);
       vmax3 = vmin2 + (vmax - vmin2) / bh * (GetDeviceCaps(gethdc(),PHYSICALOFFSETY) + h);

       nbands = (w * h * 3 + 9999999) / 10000000;

       bandheight = h / nbands;

       View3dOffScreenBitmap bitmap(this,w,bandheight + 60);
  
       bitmap.setbackground(RGB(255,255,255));
       bitmap.setplotscale(getplotscale());
       bitmap.setblackandwhite(getblackandwhite());
 
       drawwp = v.getinteger("wp::display");
       v.setinteger("wp::display",0);

       for (i = 0 ; i < nbands ; i++)
         {  bitmap.zoom(umin3,vmin3 - (vmax3 - vmin3) / h * 30 + (vmax3-vmin3)*(nbands-1-i)/nbands,
                        umax3,vmin3 + (vmax3 - vmin3) / h * 30 + (vmax3-vmin3)*(nbands-i)/nbands);
            bitmap.paintevent();
            BitBlt(gethdc(),0,i*bandheight,w,bandheight,bitmap.gethdc(),0,30,SRCCOPY);

         }

       ReleaseTextures(bitmap.gethdc());

       v.setinteger("wp::display",drawwp);

    }
  else
    {  if (getrepaintstyle() == Wireframe ||getrepaintstyle() == RayTraced)
         db.geometry.draw(DM_NORMAL,this);
       else if (getrepaintstyle() == Hidden)
         {HideImage image(this,1 + v.getinteger("hd::dashhiddenlines") * 2);
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
    } 
#endif
}

View2dPrinter::View2dPrinter(View2d *v,View *t) :
  Printer(),
  View3dSurface(t,0.0,0.0,v->getpaperwidth(),v->getpaperheight(),0,0)
{ printscale = 1.0;
  plotscale = 1;
  view2d = v;
  top = t;
}

View2dPrinter::~View2dPrinter()
{  if (top != NULL) delete top;
}

void View2dPrinter::print(PRINTDLG *userpd,DOCINFO *userdi)
{int w,h,status,outside;
 double x,y,x1,y1,x2,y2;
 ResourceString rs4(NCPRINT+4);
 ResourceString rs5(NCPRINT+5);
 ResourceString rs6(NCPRINT+6);
 POINT origin;

#if ! defined(_WIN32_WCE)
   if (userpd == 0 || userdi == 0)
     Printer::print(0);
   else 
     Printer::print(userpd,userdi);
   if (getdefined())
     {
//#if defined(WIN32)
       origin.x = GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
       origin.y = GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
       /*
#else
       short escapenum = GETPRINTINGOFFSET;
       origin.x = 0;
       origin.y = 0;
       if (Escape(pd.hDC,QUERYESCSUPPORT,sizeof(short),(const char *) &escapenum,0) >= 0)
         Escape(pd.hDC,GETPRINTINGOFFSET,0,0,&origin);
#endif
*/
       w = int((umax - umin) * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
       h = int((vmax - vmin) * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
       if (w < h && getheight() < getwidth() || w > h && getheight() > getwidth())
         {  status =RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs4.gets(),rs5.gets(),MB_YESNOCANCEL);
            if (status == IDYES)
              {  w = int((umax - umin) * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
                 h = int((vmax - vmin) * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
                 settransform2d(0.0,-1.0,h-1-origin.x,-1.0,0.0,w-1-origin.y);
              }
            else if (status == IDCANCEL)
              {  abort();
                 return;
              }
            else
              settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
         }
       else
         settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
       outside = 0;
       x = 1.0;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
       x1 = x2 = x;  y1 = y2 = y;
       outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
       x = w-2;  y = 1.0;  transform2d(&x,&y);    x += origin.x;  y += origin.y;
       if (x < x1) x1 = x;  if (x > x2) x2 = x;
       if (y < y1) y1 = y;  if (y > y2) y2 = y;
       outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
       x = w-2;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
       if (x < x1) x1 = x;  if (x > x2) x2 = x;
       if (y < y1) y1 = y;  if (y > y2) y2 = y;
       outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
       x = 1.0;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
       if (x < x1) x1 = x;  if (x > x2) x2 = x;
       if (y < y1) y1 = y;  if (y > y2) y2 = y;
       outside |= x < 0.0 || x-20 > GetDeviceCaps(pd.hDC,PHYSICALWIDTH) || y < 0.0 || y-20 > GetDeviceCaps(pd.hDC,PHYSICALHEIGHT);
       if (outside)
         {  status =RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs6.gets(),rs5.gets(),MB_YESNO);
            if (status == IDNO)
              {  abort();
                 return;
              }
         }

       newpage();
       sizeevent(w,h);
       updatematrix();
       setclip(-1,-1,x2,y2);
       printnext(view2d);
     }
#endif
}

void View2dPrinter::printnext(View2d *v2d)
{View3dSubSurface *ss;
 OneView *ov;

  view2d = v2d;

  //  Set the visibility for this view
  top->setviewno(view2d->getviewno());


  if (requiresopengl())
    {int i,w,h,pw,ph,nbands,bandheight;
     double umin1,umax1,vmin1,vmax1;

       pw = GetDeviceCaps(gethdc(),HORZRES);
       ph = GetDeviceCaps(gethdc(),VERTRES);

       w = int((umax-umin)/printscale*getxdotspermm());
       h = int((vmax-vmin)/printscale*getydotspermm());

       umin1 = umin + (umax - umin) / w * GetDeviceCaps(gethdc(),PHYSICALOFFSETX);
       umax1 = umin + (umax - umin) / w * (GetDeviceCaps(gethdc(),PHYSICALOFFSETX) + pw);
       vmin1 = vmin + (vmax - vmin) / h * GetDeviceCaps(gethdc(),PHYSICALOFFSETY);
       vmax1 = vmin + (vmax - vmin) / h * (GetDeviceCaps(gethdc(),PHYSICALOFFSETY) + ph);

       nbands = (pw * ph * 3 + 9999999) / 10000000;

       bandheight = ph / nbands;
       View2dOffScreenBitmap bitmap(this,view2d,pw,bandheight+60);
       bitmap.setxdotspermm(getxdotspermm());
       bitmap.setydotspermm(getydotspermm());
       bitmap.setbackground(RGB(255,255,255));
       bitmap.setplotscale(1.0);
       bitmap.setblackandwhite(getblackandwhite());

       for (i = 0 ; i < nbands ; i++)
         {  bitmap.zoom(umin1,vmin1 - (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-1-i)/nbands,
                        umax1,vmin1 + (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-i)/nbands);
            bitmap.setbandoffset(i * bandheight);
            RECT rect;
            rect.left = rect.top = 0; 
            rect.right = w;  rect.bottom = bandheight;
            FillRect(bitmap.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
            bitmap.paintevent();
            BitBlt(gethdc(),0,i*bandheight,pw,bandheight,bitmap.gethdc(),0,30,SRCCOPY);
         }

       ReleaseTextures(bitmap.gethdc());


    }
  else
    {

       //  Draw all the sub views
       for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
         if ((ss = new View3dSubSurface(this,ov)) != NULL)
           {  if (ov->gethiddenlines() || ov->getshaded())
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

       //  Draw the main view
       db.geometry.draw(DM_NORMAL,this);
    }
}

RCView3dPrinter::RCView3dPrinter(View3dWindow *s, QPrinter *qprt) :
 View3dSurface(s->getview(),s->getumin(),s->getvmin(),s->getumax(),s->getvmax(),0,0)
{
    // assign the printer
    qprinter = qprt;
    double printPhysicalWidth  = (qprinter->physicalDpiX() == 0 ? qprinter->logicalDpiX() : qprinter->physicalDpiX()) / 25.4 * qprinter->pageSizeMM().width();
    double printPhysicalHeight = (qprinter->physicalDpiY() == 0 ? qprinter->logicalDpiY() : qprinter->physicalDpiY()) / 25.4 * qprinter->pageSizeMM().height();

    // initialize with the input view settings
    //width = s->getwidth();
    //height = s->getheight();
    //xdotspermm = s->getxdotspermm();
    //ydotspermm = s->getydotspermm();
    //scale = s->getscale();
    //printscale = (s->getumax() - s->getumax()) / s->getwidthmm();
    printscale = s->getplotscale();
    plotscale = s->getplotscale();

    // create a graphics scene
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    //scene->setBspTreeDepth(32);
    scene->setObjectName("PrinterScene");
    scene->connect(scene,&QGraphicsScene::destroyed,RCView3dPrinter::sceneDestroyed);
    setScene(scene);

    // set the scene to be the same size as the printer
    if(qprinter->orientation() == QPrinter::Portrait)
        scene->setSceneRect(0,0,printPhysicalWidth,printPhysicalHeight);
    else
        scene->setSceneRect(0,0,printPhysicalHeight,printPhysicalWidth);

    if(s->getrepaintstyle() == RayTraced)
    {
        setrepaintstyle(s->getrepaintstyle());
        backImage = s->getScene()->backgroundBrush().textureImage();
        //backImage = backImage.scaled(scene->sceneRect().width(),scene->sceneRect().height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }

    // resize the view to be the same size as the printer
    sizeevent(scene->sceneRect().width(),scene->sceneRect().height());
    xdotspermm = qprinter->logicalDpiX() / 25.4;
    ydotspermm = qprinter->logicalDpiY() / 25.4;
    printxdotspermm = qprinter->logicalDpiX() / 25.4;
    printydotspermm = qprinter->logicalDpiY() / 25.4;
    widthmm = qprinter->widthMM();
    heightmm = qprinter->heightMM();

    qDebug() << "umin " << umin;
    qDebug() << "umax " << umax;
    qDebug() << "vmin " << vmin;
    qDebug() << "vmax " << vmax;
    qDebug() << "ulength " << umax-umin;
    qDebug() << "vlength " << vmax-vmin;

    // zoom the viewport to fit the printer
    // kmj changed for fixing dashed line drawing
    //zoom(printscale);
    double scale = (umax - umin) / widthmm;
    double u = (umin + umax) / 2.0;
    double v = (vmin + vmax) / 2.0;
    double du = widthmm * scale / 2.0;
    double dv = heightmm * scale / 2.0;
    zoom(u - du,v - dv,u + du,v + dv);
    //zoom(printscale);
    //
    qDebug() << "sceneRect " << scene->sceneRect();
    qDebug() << "getvmax " << s->getvmax();
    qDebug() << "getvmin " << s->getvmin();
    qDebug() << "v length " << (s->getvmax() - s->getvmin());
    qDebug() << "getumax " << s->getumax();
    qDebug() << "getumin " << s->getumin();
    qDebug() << "u length " << (s->getumax() - s->getumin());
    qDebug() << "getwidth " << s->getwidth();
    qDebug() << "getheight " << s->getheight();
    qDebug() << "getwidthmm " << s->getwidthmm();
    qDebug() << "getheightmm " << s->getheightmm();
    qDebug() << "printscale " << (s->getumax() - s->getumin()) / s->getwidthmm();
    //

    // init the print scale default setting
    // diff to 7.5
    //this->scale = (s->getumax() - s->getumin()) / s->getwidthmm(); // 7.5
    //this->scale = (getumax() - getumin()) / getwidthmm();// 7.6
    // diff to 7.5
    //printscale = (s->getumax() - s->getumin()) / s->getwidthmm();// 7.5
    //printscale = (getumax() - getumin()) / getwidthmm(); // 7.6
    printscale = s->getplotscale(); // 7.6
    // diff to 7.5
    //plotscale = ::v.getreal("vw::plotscale"); // 7.6
    //plotscale = s->getplotscale(); // 7.5
    //plotscale = (s->getumax() - s->getumin()) / s->getwidthmm();
    setrepaintstyle(s->getrepaintstyle());
    displaylayertable = view->getdisplaylayertable();
    //scene->setMinimumRenderSize(1.0);
}

void RCView3dPrinter::sceneDestroyed(QObject*)
{
    qDebug() << "sceneDestroyed";
}

RCView3dPrinter::~RCView3dPrinter()
{
    if(getScene())
    {
        getScene()->deleteLater();
        setScene(0);
    }
}

void RCView3dPrinter::debug(RCCHAR *filepath)
{
    QFile file;
    QString message;
    QDebug debugout(&message);
    if(strlen(filepath) > 0)
    {
        QString qsFilepath(filepath);
        file.setFileName(qsFilepath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            debugout = QDebug(&file);
    }

    debugout << "RCView3dPrinter Debug\n";
    debugout << "umin........ " << umin << "\n";
    debugout << "umax........ " << umax << "\n";
    debugout << "vmin........ " << vmin << "\n";
    debugout << "vmin........ " << vmax << "\n";
    debugout << "width....... " << width << "\n";
    debugout << "height...... " << height << "\n";
    debugout << "width MM.... " << getwidthmm() << "\n";
    debugout << "height MM... " << getheightmm() << "\n";
    debugout << "scale....... " << scale << "\n";
    debugout << "plotscale... " << plotscale << "\n";
    debugout << "printscale.. " << printscale << "\n";
    debugout << "xdotspermm.. " << getxdotspermm() << "\n";
    debugout << "ydotspermm.. " << getydotspermm() << "\n";

    if(strlen(filepath) > 0)
        file.close();
    else
        qDebug() << message;
}

void RCView3dPrinter::render(QPainter *painter,double umin1, double vmin1, double umax1, double vmax1)
{
     int w,h,bw,bh,status,outside,i,nbands,bandheight,drawwp,rotated;
     double x,y,x1,y1,x2,y2,umax2,vmin2,umin3,umax3,vmin3,vmax3;
     ResourceString rs4(NCPRINT+4);
     ResourceString rs5(NCPRINT+5);
     ResourceString rs6(NCPRINT+6);
     POINT origin,size;
/*
     origin.x = 0.0; //GetDeviceCaps(pd.hDC, PHYSICALOFFSETX);
     origin.y = 0.0; //GetDeviceCaps(pd.hDC, PHYSICALOFFSETY);
     size.x = qprinter->logicalDpiX() / 25.4 * qprinter->widthMM();  //GetDeviceCaps(pd.hDC, PHYSICALWIDTH);
     size.y = qprinter->logicalDpiY() / 25.4 * qprinter->heightMM(); //GetDeviceCaps(pd.hDC, PHYSICALHEIGHT);
     if (size.x > 0 && size.y > 0)
     {
         double vr,hr;
          vr = qprinter->width();  //GetDeviceCaps(gethdc(),VERTRES);
          hr = qprinter->height(); //GetDeviceCaps(gethdc(),HORZRES);
          umin = umin1 + (umax1 - umin1) / (size.x-1) * origin.x;
          vmin = vmin1 + (vmax1 - vmin1) / (size.y-1) * (size.y - 1 - vr - 1 - origin.y);
          umax = umin1 + (umax1 - umin1) / (size.x-1) * (origin.x + hr - 1);
          vmax = vmin1 + (vmax1 - vmin1) / (size.y-1) * (size.y - 1 - origin.y);
          //umin = umin1;
          //vmin = vmin1;
          //umax = umax1;
          //vmax = vmax1;
     }
     else
     {
         umax = umax1;  vmax = vmax1;  umin = umin1;  vmin = vmin1;
     }
*/
     umin = umin1;
     vmin = vmin1;
     umax = umax1;
     vmax = vmax1;

     qDebug() << "qprinter->logicalDpiX()" << qprinter->logicalDpiX();
     qDebug() << "printscale" << printscale;
     qDebug() << "qprinter->logicalDpiX() / 25.4 / printscale" << qprinter->logicalDpiX() / 25.4 / printscale;
     qDebug() << "qprinter->logicalDpiY() / 25.4 / printscale" << qprinter->logicalDpiY() / 25.4 / printscale;
     //w = int((umax - umin)  * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
     //h = int((vmax - vmin)  * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
     w = int((umax - umin)  * qprinter->logicalDpiX() / 25.4 / printscale ); // 7.5
     h = int((vmax - vmin)  * qprinter->logicalDpiY() / 25.4 / printscale ); // 7.5
     //w = int((umax - umin)  * qprinter->logicalDpiX() / 25.4); // 7.5
     //h = int((vmax - vmin)  * qprinter->logicalDpiY() / 25.4); // 7.5
     // this makes the line thickness constant for scaled and fit to paper sizing
     //w = int(painter->viewport().width()) ; // 7.6
     //h = int(painter->viewport().height()) ;// 7.6
     //w = getwidth();
     //h = getheight();
     w = getwidthmm() * getprintxdotspermm();
     h = getheightmm() * getprintydotspermm();

     rotated = 0;

     qDebug() << "w " << w;
     qDebug() << "h " << h;
     qDebug() << "getwidth " << getwidth();
     qDebug() << "getheight " << getheight();
     qDebug() << "getwidthmm * xpdotmm " << getwidthmm() * getprintxdotspermm();
     qDebug() << "getheightmm * ypdotsmm " << getheightmm() * getprintxdotspermm();
     qDebug() << "printer width" << qprinter->width();
     qDebug() << "printer height" << qprinter->height();
     qDebug() << "painter viewport width" << painter->viewport().width();
     qDebug() << "painter viewport height" << painter->viewport().height();
     qDebug() << "scene " << getScene()->objectName();
     qDebug() << "sceneRect " << getScene()->sceneRect();
     qDebug() << "requiresopengl " << requiresopengl();
/*
     if ((w < h && getheight() < getwidth() || w > h && getheight() > getwidth()) && requiresopengl() == 0)
       {
          status = RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs4.gets(),rs5.gets(),MB_YESNOCANCEL);
          if (status == IDYES)
          {
               //w = int((umax - umin) * GetDeviceCaps(gethdc(),LOGPIXELSX) / 25.4 / printscale);
               //h = int((vmax - vmin) * GetDeviceCaps(gethdc(),LOGPIXELSY) / 25.4 / printscale);
               w = int((umax - umin) * qprinter->logicalDpiX() / 25.4 / printscale);
               h = int((vmax - vmin) * qprinter->logicalDpiY() / 25.4 / printscale);
               // debug
               settransform2d(0.0,-1.0,h-1-origin.x,-1.0,0.0,w-1-origin.y);
               rotated = 1;
          }
          else if (status == IDCANCEL)
          {
               //abort();
               qprinter->abort();
               return;
          }
          else
            settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
       }
     //else
     //  settransform2d(1.0,0.0,-origin.x,0.0,-1.0,h-1-origin.y);
     */
#if 0
     outside = 0;
     x = 1.0;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     x1 = x2 = x;  y1 = y2 = y;
     //outside |= x < 0.0 || x-20 > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y-20 > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     outside |= x < 0.0 || x > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     //x = w-2;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     x = w;  y = 1.0;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     if (x < x1) x1 = x;  if (x > x2) x2 = x;
     if (y < y1) y1 = y;  if (y > y2) y2 = y;
     //outside |= x < 0.0 || x-20 > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y-20 > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     outside |= x < 0.0 || x > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     //x = w-2;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     x = w;  y = h;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     if (x < x1) x1 = x;  if (x > x2) x2 = x;
     if (y < y1) y1 = y;  if (y > y2) y2 = y;
     //outside |= x < 0.0 || x-20 > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y-20 > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     outside |= x < 0.0 || x > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     //x = 1.0;  y = h-2;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     x = 1.0;  y = h;  transform2d(&x,&y);  x += origin.x;  y += origin.y;
     if (x < x1) x1 = x;  if (x > x2) x2 = x;
     if (y < y1) y1 = y;  if (y > y2) y2 = y;
     //outside |= x < 0.0 || x-20 > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y-20 > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     outside |= x < 0.0 || x > qprinter->logicalDpiX() / 25.4 * qprinter->widthMM() || y < 0.0 || y > qprinter->logicalDpiY() / 25.4 * qprinter->heightMM();
     /*
     if (outside)
     {
          status =RCMessageBox(cadwindow == 0 ? 0 : cadwindow->gethwnd(),rs6.gets(),rs5.gets(),MB_YESNO);
          if (status == IDNO)
          {
               //abort();
              qprinter->abort();
               return;
          }
     }
     */
#endif
     //qprinter->newpage();
     double cl = qMax(w,h);
     sizeevent(w,h);
     //sizeevent(cl,cl);
     //sizeevent(double(painter->device()->width()),double(painter->device()->height()));
     updatematrix();
     //setclip(-1,-1,x2,y2);
     setclip(-1,-1,w+1,h+1);
     //setclip(-1,-1,cl,cl);
     getScene()->setSceneRect(0,0,w,h);
     //getScene()->setSceneRect(0,0,cl,cl);
     qDebug() << "sceneRect : " << getScene()->sceneRect();

     double xscale = double(painter->device()->width()) / double(w);
     double yscale = double(painter->device()->height()) / double(h);
     double pscale = qMin(xscale,yscale);

     //setprintxdotspermm(qprinter->logicalDpiX() / 25.4 / pscale);
     //setprintydotspermm(qprinter->logicalDpiY() / 25.4 / pscale);
     setprintxdotspermm(qprinter->logicalDpiX() / 25.4);
     setprintydotspermm(qprinter->logicalDpiY() / 25.4);

     qDebug() << "w " << w;
     qDebug() << "h " << h;
     qDebug() << "getwidth " << getwidth();
     qDebug() << "getheight " << getheight();
     qDebug() << "requiresopengl " << requiresopengl();
     qDebug() << "scene " << getScene()->objectName();
     qDebug() << "sceneRect " << getScene()->sceneRect();

#if 0
#if 1
     // force line style display on for printing
     int dbWeightDisplay = db.getlineweightdisplay();
     db.setlineweightdisplay(1);
     db.drawforQtPrint(DM_PRINT,(RCView3dPrinter*)this, getScene());
     db.setlineweightdisplay(dbWeightDisplay);
#endif
     qDebug() <<  "painter width :" << painter->device()->width();
     qDebug() <<  "painter height :" << painter->device()->height();
     qDebug() <<  "painter widthMM :" << painter->device()->widthMM();
     qDebug() <<  "painter heightMM :" << painter->device()->heightMM();
#if 1
     QRectF srectf = getScene()->sceneRect();
     QRect scenerect(srectf.x(),srectf.y(),srectf.width(),srectf.height());
     // get the offset for the print preview
     QRect  pagerect  = qprinter->pageRect();
     double dx = pagerect.width()/2 - double(scenerect.width()*pscale/2.);
     double dy = pagerect.height()/2 - double(scenerect.height()*pscale/2.);
     painter->translate(dx, dy );
#endif
#endif

     qDebug() << "painter->device() : " << painter->device()->paintEngine()->type();
     qDebug() << "painter->compositionMode() : " << painter->compositionMode();

#if 1
     // if we are rendering with gl use a gl offscreen buffer
     // copy the buffer to an image and draw it to the printers painter first
     // then draw any vector stuff on top
     bool glRenderingRequired = false;

     // if this is a view 2d check for any rendered viewports
     if(getsurfacetype() == View2dSurf)
     {
         OneView *ov;
         View2d *view2d = ((RCView2dPrinter*)this)->getview2dwindow()->getview2d();

         for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
         {
             if(ov->getshaded())
             {
                 glRenderingRequired = true;
                 break;
             }
         }
     }

     if(getrepaintstyle() == Shaded ||
        getrepaintstyle() == ShadedHidden ||
        getrepaintstyle() == ShadedHiddenDashed ||
        getrepaintstyle() == ShadedTexture ||
        getrepaintstyle() == ShadedHiddenTexture)
         glRenderingRequired = true;

     if(glRenderingRequired)
     {
         /*
         // KMJ: added for debugging dashed lines
         int w = int(painter->viewport().width()) ;
         int h = int(painter->viewport().height()) ;
         //qprinter->newpage();
         double cl = qMax(w,h);
         sizeevent(w,h);
         //sizeevent(cl,cl);
         //sizeevent(double(painter->device()->width()),double(painter->device()->height()));
         updatematrix();
         //setclip(-1,-1,x2,y2);
         setclip(-1,-1,w+1,h+1);
         //setclip(-1,-1,cl,cl);
         getScene()->setSceneRect(0,0,w,h);
         //getScene()->setSceneRect(0,0,cl,cl);
         qDebug() << "sceneRect : " << getScene()->sceneRect();

         xscale = double(painter->device()->width()) / double(w);
         yscale = double(painter->device()->height()) / double(h);
         pscale = qMin(xscale,yscale);

         //setprintxdotspermm(qprinter->logicalDpiX() / 25.4 / pscale);
         //setprintydotspermm(qprinter->logicalDpiY() / 25.4 / pscale);
         setprintxdotspermm(qprinter->logicalDpiX() / 25.4);
         setprintydotspermm(qprinter->logicalDpiY() / 25.4);
         // end added for debbuging dashed lines
         */

         // find the limits of the GL device
         GLint dims[2];
         glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &dims[0]);

         qDebug() << "GL max view port dims[0] : " << dims[0];
         qDebug() << "GL max view port dims[1] : " << dims[1];

         GLint texSize;
         glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
         qDebug() << "GL_MAX_TEXTURE_SIZE Max texture is " << texSize;

         GLint maxRenderBufferSize;
         glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT,&maxRenderBufferSize);
         qDebug() << "GL_MAX_RENDERBUFFER_SIZE_EXT is " << maxRenderBufferSize;

         // get the output size in pixels
         // we could use the painter rect as the render viewport
         //int deviceWidth = painter->viewport().width();
         //int deviceHeight = painter->viewport().height();
         // w,h are the device rect (=scene rect) which is bigger than the painter rect
         // the painter scales to the printer device rect

         int deviceWidth  = w;
         int deviceHeight = h;

         qDebug() << "deviceWidth : " << deviceWidth;
         qDebug() << "deviceHeight : " << deviceHeight;
         qDebug() << "sceneRect" << getScene()->sceneRect();

         if(deviceWidth > dims[0])
             qDebug() << "output width is larger than gl max viewport width : " << dims[0];

         if(deviceHeight > dims[1])
             qDebug() << "output width is larger than gl max viewport height : " << dims[1];

         int deviceMax = max(deviceWidth,deviceHeight);
         int rbMax = maxRenderBufferSize;
         int xvpnum = 1;
         int yvpnum = 1;
         fboWidth = deviceWidth;
         fboHeight = deviceHeight;

         if(deviceMax > rbMax)
         {
             // fit the biggest rectangle of the same proportion as the printer
             // which will fit into a square which is maxRenderBufferSize in W and H
             double fboScale = double(maxRenderBufferSize) / double(deviceMax);
             fboWidth  = maxRenderBufferSize;
             fboHeight = int(deviceHeight * fboScale);
             if(deviceHeight > deviceWidth)
             {
                 fboWidth  = int(deviceWidth * fboScale);
                 fboHeight = maxRenderBufferSize;
             }
         }

         qDebug() << "fboWidth : " << fboWidth;
         qDebug() << "fboHeight : " << fboHeight;

         // create the opengl paint device
         QOpenGLFramebufferObjectFormat format;
         format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
         format.setInternalTextureFormat(GL_RGBA);
         QOpenGLFramebufferObject device(fboWidth,fboHeight,format);

         if(device.isValid())
             qDebug() << "frame buffer object is valid";
         else
             qDebug() << "frame buffer object is not valid";

         device.bind();

         // KMJ should probably move this gl setting stuff to the db.drawforQtPrintGL function??
         // start of gl settings
         glViewport(0, 0, fboWidth, fboHeight);
         RCCheckGlError();

         View *view;
         Point eye,pmin,pmax;
         view = getview();
         db.geometry.extents(this,&pmin,&pmax);
         if (view->getperspective())
           eye = view->geteye();
         else
           eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

         SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
         SetViewport( getumin(), getvmin(), getumax(), getvmax());
         glViewport(0, 0, fboWidth, fboHeight);

         QtBeginScene();
         BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);
         //RCCheckGlError();
         //glClearColor(1.0f,0.0f,0.0f,1.0f);
         //RCCheckGlError();
         glEnable(GL_LIGHTING);
         //RCCheckGlError();
         glEnable(GL_DEPTH_TEST);
         //RCCheckGlError();
         glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
         //RCCheckGlError();

         EntityList dblist = *db.geometry.getlist();
         Entity *e=0;
         int nlights=0;
         for (dblist.end() ; (e = dblist.last()) != NULL ; )
         {
           if (e->isa(light_entity) && e->isvisible(this))
           {
                double red,green,blue;
                if (((Light *)e)->getoptions().test(1))
                  {  red = green = blue = 1.0;
                  }
                else
                  {  red = db.colourtable[e->getcolour()].red/255.0;
                     green = db.colourtable[e->getcolour()].green/255.0;
                     blue = db.colourtable[e->getcolour()].blue/255.0;
                  }

                SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                nlights++;
           }
         }

         if (nlights == 0)
             SetLight(eye.x,eye.y,eye.z);
         // end of gl settings

         db.drawforQtPrintGL(DM_PRINT,this, getScene());
         RCCheckGlError();

         glFlush();
         QtEndScene();

         device.release();
         //device.bindDefault();
#if 0
         // id using a separate gl context for the fbo
         surface->destroy();
         delete surface;
#endif

         // convert the fbo data to an image then scale it and draw it to the painter
         // it is the same size as the printer so it is the background of the printing
         //QImage fboImage(device.toImage());
         //QImage printImage(fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_RGB32);
         QImage printImage(device.toImage().convertToFormat(QImage::Format_RGB32));

         if(printImage.isNull())
             qDebug() << "print Image is not valid";
         else
             qDebug() << "print Image is valid";
#ifdef _RCDEBUG
         if(!printImage.isNull())
             printImage.save(QString(home.getpublicfilename("printImage.jpg")),"jpg");
#endif
         qDebug() << "print image rect     : " << printImage.rect();
         qDebug() << "painter rect         : " << painter->viewport();
         qDebug() << "viewTransformEnabled : " << painter->viewTransformEnabled();

         QRect source(printImage.rect()),target(painter->viewport());

         qDebug() << "print source rect : " << source;
         qDebug() << "print target rect : " << target;

         painter->drawImage(painter->viewport(),printImage,printImage.rect());
     }
#endif

     //debug(home.getpublicfilename("PrintSheetSelectInRenderDebug.txt"));

     if(getrepaintstyle() == RayTraced && !backImage.isNull())
     {
         QRect scenerect = backImage.rect();
         QRect  pagerect  = qprinter->pageRect();
         double dx = pagerect.width()/2 - double(scenerect.width()*pscale/2.);
         double dy = pagerect.height()/2 - double(scenerect.height()*pscale/2.);
         scenerect.translate(dx,dy);
         painter->drawImage(scenerect,backImage,backImage.rect());
     }
     else
     {
         // force line style display on for printing
         int dbWeightDisplay = db.getlineweightdisplay();
         db.setlineweightdisplay(1);
         db.drawforQtPrint(DM_PRINT,(RCView3dPrinter*)this, getScene());
         db.setlineweightdisplay(dbWeightDisplay);

         qDebug() <<  "painter width :" << painter->device()->width();
         qDebug() <<  "painter height :" << painter->device()->height();
         qDebug() <<  "painter widthMM :" << painter->device()->widthMM();
         qDebug() <<  "painter heightMM :" << painter->device()->heightMM();

         QRectF srectf = getScene()->sceneRect();
         QRect scenerect(srectf.x(),srectf.y(),srectf.width(),srectf.height());
         // get the offset for the print preview
         QRect  pagerect  = qprinter->pageRect();
         double dx = pagerect.width()/2 - double(scenerect.width()*pscale/2.);
         double dy = pagerect.height()/2 - double(scenerect.height()*pscale/2.);
         painter->translate(dx, dy );

         this->getScene()->render(painter/*,
                                  QRectF(0, 0,qprinter->width(), qprinter->height()),
                                  scenerect*/);
     }
}

double RCView3dPrinter::getphyswidthmm(void)
{
     return qprinter->pageSizeMM().width();
}

double RCView3dPrinter::getphysheightmm(void)
{
    return qprinter->pageSizeMM().height();
}

// zoom in view coordinates by a scale value
// hint taken from zoom_command5 in nczone.cpp
void RCView3dPrinter::zoom(double scale)
{
    double du = scale * getwidthmm() / 2.0;
    double dv = scale * getheightmm() / 2.0;
    double cu = (double)getwidth() / 2.0 / (double)getwidth() * (umax - umin) + umin;
    double cv = (double)getheight() / 2.0 / (double)getheight() * (vmax - vmin) + vmin;

    zoom(cu-du,cv-dv,cu+du,cv+dv);
}

// move to a point in viewing coordinates
// not tested yet
void RCView3dPrinter::zoom(double u, double v)
{
    double du = umax - umin;
    double dv = vmax - vmin;
    zoom(u - du,v - dv,u + du,v + dv);
}

// zoom to point in view coordinats and scale by a value
// not tested yet
void RCView3dPrinter::zoom(double u, double v, double scale )
{
    double du = umax - umin;
    double dv = vmax - vmin;
    zoom((u - du) * scale,(v - dv) * scale, (u + du) * scale, (v + dv) * scale);
}

// zoom to window
void RCView3dPrinter::zoom(double u1,double v1,double u2,double v2)
{
    umin = u1; vmin = v1; umax= u2; vmax = v2;
    fixzoom();
    Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
    if (db.getenableautoplotscale() != 0 && getsurfacetype() != View2dSurf)
      plotscale = (umax - umin) / getwidthmm() * db.getautoplotscale();
    updatematrix();
}

void RCView3dPrinter::setzoom(double umin1,double vmin1,double umax1,double vmax1)
{
    umin = umin1; vmin = vmin1; umax = umax1; vmax = vmax1;
}

void RCView3dPrinter::fixzoom()
{
  double umin1,umax1,vmin1,vmax1,aspect,du,dv;
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
  //aspect = getwidthmm() / getheightmm() * v.getreal("vw::aspectadjustment");
  aspect = getwidthmm() / getheightmm();
  if (du / dv > aspect) dv = du / aspect;
  if (du / dv < aspect) du = dv * aspect;
  umin = (umin1 + umax1) / 2.0 - du;
  vmin = (vmin1 + vmax1) / 2.0 - dv;
  umax = (umin1 + umax1) / 2.0 + du;
  vmax = (vmin1 + vmax1) / 2.0 + dv;
}

RCView2dPrinter::RCView2dPrinter(View3dWindow *s, QPrinter *qprt) :
    RCView3dPrinter( s, qprt)
{
    view2dwindow = (View2dWindow *)s;
    top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
    view2d = view2dwindow->getview2d();

    umin = 0;
    umax = view2dwindow->getview2d()->getpaperwidth();
    vmin = 0;
    vmax = view2dwindow->getview2d()->getpaperheight();

    // zoom the viewport to fit the printer
    double scale = (umax - umin) / getwidthmm();
    double u = (umin + umax) / 2.0;
    double v = (vmin + vmax) / 2.0;

    double du = getwidthmm() * scale / 2.0;
    //double dv = getheightmm() * scale / 2.0 / ::v.getreal("vw::aspectadjustment");
    double dv = getheightmm() * scale / 2.0;
    zoom(u - du,v - dv,u + du,v + dv);

    //printscale = 1.0;
    printscale = (getumax() - getumin()) / getwidthmm();
    // diff to 7.5
    //plotscale = (getumax() - getumin()) / getwidthmm();// 7.6
    plotscale = 1;// 7.5
}

View2d* RCView2dPrinter::getvew2d()
{
    //return view2d;
    return view2dwindow->getview2d();
}

View* RCView2dPrinter::getview()
{
    //return top;
    return view2dwindow->getview();
}

View2dWindow* RCView2dPrinter::getview2dwindow()
{
    return view2dwindow;
}

RCView2dPrinter::~RCView2dPrinter()
{
    qDebug() << "~RCView2dPrinter";
}


#if ! defined(_WIN32_WCE)
MetaFile::MetaFile(RCCHAR *filename)
{RCHDC hdc1;
  Surface::sizeevent(1000,1000);
  hmf = NULL;
  //hdc1 = CreateMetaFile(filename);
  hdc1 = CreateEnhMetaFile(cadwindow->gethdc(),filename,0,0);
  sethdc(NULL,hdc1);
  status = hdc1 != NULL;
}

MetaFile::~MetaFile()
{ if (status) DeleteEnhMetaFile(hmf);
}

void MetaFile::close(void)
{RCHDC hdc;
  if (status)
    {  hdc = gethdc();  freehdc();
       hmf = CloseEnhMetaFile(hdc);
    }
}

void MetaFile::copytoclipboard(void)
{ if (hmf == NULL) return;
  if (OpenClipboard(cadwindow->gethwnd()))
    {  EmptyClipboard();
       SetClipboardData(CF_ENHMETAFILE,hmf);
       status = 0;
       CloseClipboard();
    }
}

View3dMetaFile::View3dMetaFile(RCCHAR *filename,View3dSurface *v) :
  MetaFile(filename),View3dSurface(v->getview(),v->getumin(),v->getvmin(),v->getumax(),v->getvmax(),0,0)
{RECT rect;
 RCHDC hdc1;
  close();
  sizeevent(v->getwidth(),v->getheight());
  rect.left = 0;  rect.top = 0;
  rect.right = (int)((getumax()-getumin())/v->getscale() * 100.0);  
  rect.bottom = (int)((getvmax()-getvmin())/v->getscale() * 100.0);
  hdc1 = CreateEnhMetaFile(cadwindow->gethdc(),filename,&rect,0);
  sethdc(NULL,hdc1);
  status = hdc1 != NULL;

  setrepaintstyle(v->getrepaintstyle());
  sizeevent(v->getwidth(),v->getheight());
  umin = v->getumin();
  vmin = v->getvmin();
  umax = v->getumax();
  vmax = v->getvmax();
  plotscale = v->getplotscale();
  updatematrix();
  setscale(v->getscale());  //  Use the same scale as the current view.

  //ScaleWindowExtEx(hdc1,1,10,1,10,0);

}

View3dMetaFile::View3dMetaFile(RCCHAR *filename,View3dSurface *v,double umin1,double vmin1,double umax1,double vmax1) :
  MetaFile(filename),View3dSurface(v->getview(),v->getumin(),v->getvmin(),v->getumax(),v->getvmax(),0,0)
{RECT rect;
 RCHDC hdc1;
  close();
  sizeevent(int(v->getwidth()*(umax1-umin1)/(umax-umin)),int(v->getheight()*(vmax1-vmin1)/(vmax-vmin)));
  rect.left = 0;  rect.top = 0;
  rect.right = (int)((umax1-umin1)/v->getscale() * 100.0);  
  rect.bottom = (int)((vmax1-vmin1)/v->getscale() * 100.0);
  hdc1 = CreateEnhMetaFile(cadwindow->gethdc(),filename,&rect,0);
  sethdc(NULL,hdc1);
  status = hdc1 != NULL;

  setrepaintstyle(v->getrepaintstyle());
  sizeevent(int(v->getwidth()*(umax1-umin1)/(umax-umin)),int(v->getheight()*(vmax1-vmin1)/(vmax-vmin)));

  umin = umin1;
  vmin = vmin1;
  umax = umax1;
  vmax = vmax1;
  updatematrix();
  if (db.getenableautoplotscale() != 0 && getsurfacetype() != View2dSurf)
    plotscale = (umax - umin) / getwidthmm() * db.getautoplotscale();

//  ScaleWindowExtEx(hdc1,10,1,10,1,0);

}

void View3dMetaFile::draw(void)
{ //SetWindowExtEx(gethdc(),int(getwidth()),int(getheight()),0);
  if (getrepaintstyle() == Wireframe ||getrepaintstyle() == RayTraced)
    {
       db.draw(DM_NORMAL,this);
       db.grid.draw(DM_NORMAL,this);
       if (db.workplanes.getcurrent() != NULL)
         db.workplanes.getcurrent()->draw(DM_NORMAL,this);
    }
  else if (getrepaintstyle() == Hidden)
    {HideImage image(this,1 + v.getinteger("hd::dashhiddenlines") * 2);
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
}

View2dMetaFile::View2dMetaFile(RCCHAR *filename,View2d *v,View *t) :
  MetaFile(filename),
  View3dSurface(t,0.0,0.0,v->getpaperwidth(),v->getpaperheight(),0,0)
{RECT rect;
 RCHDC hdc1,hdctemp;
  close();
  hdctemp = GetDC(0);
  sizeevent((umax - umin) /
            double(GetDeviceCaps(hdctemp,HORZSIZE)) *
            double(GetDeviceCaps(hdctemp,HORZRES)),
            (vmax - vmin) /
            double(GetDeviceCaps(hdctemp,VERTSIZE)) *
            double(GetDeviceCaps(hdctemp,VERTRES)));
  ReleaseDC(NULL,hdctemp);
  rect.left = 0;  rect.top = 0;
  rect.right = (long)(v->getpaperwidth() * 100.0);  rect.bottom = (long)(v->getpaperheight() * 100.0);
  hdc1 = CreateEnhMetaFile(cadwindow->gethdc(),filename,&rect,0);
  sethdc(NULL,hdc1);
  status = hdc1 != NULL;

  view2d = v;
  top = t;
}

View2dMetaFile::~View2dMetaFile()
{ if (top != NULL) delete top;
}

void View2dMetaFile::zoom(double umin1,double vmin1,double umax1,double vmax1)
{ sizeevent(getwidth()*(umax1-umin1)/(umax-umin),getheight()*(vmax1-vmin1)/(vmax-vmin));
  umin = umin1;
  vmin = vmin1;
  umax = umax1;
  vmax = vmax1;
  updatematrix();
}

void View2dMetaFile::draw(void)
{OneView *ov;
 View3dSubSurface *ss;
 RCHDC hdctemp;
  hdctemp = GetDC(NULL);
  sizeevent((umax - umin) /
            double(GetDeviceCaps(hdctemp,HORZSIZE)) *
            double(GetDeviceCaps(hdctemp,HORZRES)),
            (vmax - vmin) /
            double(GetDeviceCaps(hdctemp,VERTSIZE)) *
            double(GetDeviceCaps(hdctemp,VERTRES)));
  ReleaseDC(NULL,hdctemp);
  updatematrix();
  SetWindowExtEx(gethdc(),int(getwidth()),int(getheight()),0);
  for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
    if ((ss = new View3dSubSurface(this,ov)) != NULL)
      {  if (ov->gethiddenlines())
           {HideImage image(ss,ov->getdashhidden() * 2);
              image.hide();
           }
         else
           ss->paintevent();
         delete ss;
      }
  db.geometry.draw(DM_NORMAL,this);
}

#else
#endif

struct RBPrint5_data
  {View3dPrinter *view3dprinter;
   Point xaxis,yaxis;
  };

void RBPrint5(int,void *data,Point *p2,View3dSurface *surface)
{RBPrint5_data *rbdata = (RBPrint5_data *) data;
 Point p1;
  if (surface != 0)
    {  rbdata->xaxis = surface->getuaxis() * rbdata->view3dprinter->getphyswidthmm() * rbdata->view3dprinter->getprintscale();
       rbdata->yaxis = surface->getvaxis() * rbdata->view3dprinter->getphysheightmm() * rbdata->view3dprinter->getprintscale();
    }
  Line line1(*p2,*p2+rbdata->xaxis);  line1.draw(DM_INVERTDASH);
  Line line2(*p2+rbdata->xaxis,*p2+rbdata->xaxis+rbdata->yaxis);  line2.draw(DM_INVERTDASH);
  Line line3(*p2+rbdata->xaxis+rbdata->yaxis,*p2+rbdata->yaxis);  line3.draw(DM_INVERTDASH);
  Line line4(*p2+rbdata->yaxis,*p2);  line4.draw(DM_INVERTDASH);
}

// print preview command
void file_command5(int *cstate,HighLevelEvent *event,void **data)
{RBPrint5_data *rbdata = (RBPrint5_data *) *data;
 static View3dWindow *view3dwindow;
 static View2dWindow *view2dwindow;
 ResourceString rs5(NCPRINT+5),rs7(NCPRINT+7);
 Point p;
  switch (*cstate)
    {case InitialState :
       rbdata = 0;
       if (cadwindow->printpreviewwindow != 0)
         {  cadwindow->printpreviewwindow->print();
            *cstate = ExitState;
         }
       else if ((view3dwindow = cadwindow->getcurrentwindow()) == NULL)
         {  cadwindow->MessageBox(rs7.gets(),rs5.gets(),MB_ICONSTOP);
            *cstate = ExitState;
         }
       else if (view3dwindow->getsurfacetype() == View3dSurf)
         {  rbdata = new RBPrint5_data;  *data = rbdata;
            if (rbdata != 0)
              {  rbdata->view3dprinter = new View3dPrinter(view3dwindow);
                 if (rbdata->view3dprinter != 0 && rbdata->view3dprinter->startprint())
                   {  if (rbdata->view3dprinter->getselection())
                        *cstate = 1;
                      else
                        {  rbdata->view3dprinter->endprint(view3dwindow->getumin(),view3dwindow->getvmin(),view3dwindow->getumax(),view3dwindow->getvmax());
                           delete rbdata->view3dprinter;
                           *cstate = ExitState;
                        }
                   }
                 else
                   {  delete rbdata->view3dprinter;
                      *cstate = ExitState;
                   }
              }
            else
              *cstate = ExitState;
         }
       else if (view3dwindow->getsurfacetype() == View2dSurf)
         {  view2dwindow = (View2dWindow *) view3dwindow;
            View *top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
            View2dPrinter view2dprinter(view2dwindow->getview2d(),top);
            view2dprinter.print(0,0);
            *cstate = ExitState;
         }
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = view3dwindow->modeltouv(((Coordinate3dEvent *)event)->getp());
            rbdata->view3dprinter->endprint(p.x,p.y,p.x+rbdata->view3dprinter->getphyswidthmm() * rbdata->view3dprinter->getprintscale(),
                                            p.y+rbdata->view3dprinter->getphysheightmm() * rbdata->view3dprinter->getprintscale());
            delete rbdata->view3dprinter;
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  rbdata->view3dprinter->abort();
            delete rbdata->view3dprinter;
            *cstate = ExitState;
         }
       break;
    }
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBPrint5,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCPRINT+12);
    }
  else
    delete rbdata;
}

// print command
void file_command6(int *cstate,HighLevelEvent *,void **)
{
  Printer printer;
  printer.initialize();
  *cstate = ExitState;
}

void copymeta_command(int *cstate,HighLevelEvent *,void **)
{View3dWindow *window;
 View2dWindow *view2dwindow;
  *cstate = ExitState;
  if ((window = cadwindow->getcurrentwindow()) == NULL) return;
  if (window->getsurfacetype() == View3dSurf)
    {  View3dMetaFile view3dmf(NULL,cadwindow->getcurrentsurface());
       view3dmf.draw();
       view3dmf.close();
       view3dmf.copytoclipboard();
    }
  else
    {  view2dwindow = (View2dWindow *) window;
       View *top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
       View2dMetaFile view2dmf(NULL,view2dwindow->getview2d(),top);
       view2dmf.draw();
       view2dmf.close();
       view2dmf.copytoclipboard();
    }
}

void copymetawindow_command(int *cstate,HighLevelEvent *event,void **)
{static Point p1;
 static View3dWindow *zoomwindow;
 View3dWindow *window;
 View2dWindow *view2dwindow;
 Point p2;
 double t;
  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != 0)
                {  zoomwindow = window;
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
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != 0 && window == zoomwindow)
                {  p2 = ((CoordinateuvEvent *) event)->coordinate();
                   if (p1.x > p2.x)
                     {  t = p1.x;  p1.x = p2.x;  p2.x = t;
                     }
                   if (p1.y > p2.y)
                     {  t = p1.y;  p1.y = p2.y;  p2.y = t;
                     }
                   if (p1.x != p2.x && p1.y != p2.y)
                   if (window->getsurfacetype() == View3dSurf)
                     {  View3dMetaFile view3dmf(NULL,window,p1.x,p1.y,p2.x,p2.y);
                        view3dmf.draw();
                        view3dmf.close();
                        view3dmf.copytoclipboard();
                     }
                   else
                     {  view2dwindow = (View2dWindow *) window;
                        View *top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
                        View2dMetaFile view2dmf(NULL,view2dwindow->getview2d(),top);
                        view2dmf.zoom(p1.x,p1.y,p2.x,p2.y);
                        view2dmf.draw();
                        view2dmf.close();
                        view2dmf.copytoclipboard();
                     }               
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
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+31);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+32);
    }
}

void copybitmap_toclipboard_command(int *cstate,HighLevelEvent *event,void **)
{
    if (cadwindow->getcurrentwindow() != 0)
    {
        cadwindow->getcurrentwindow()->paintevent();
        app->processEvents();
        QPixmap screenshot = QPixmap::fromImage(((QGLWidget*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view()->viewport())->grabFrameBuffer());

        QClipboard *clipboard = QApplication::clipboard();
        if(clipboard)
        {
            clipboard->clear();
            clipboard->setPixmap(screenshot);
            // check to see if it happend
            QPixmap test = clipboard->pixmap();
            if(test.isNull())
                QMessageBox::warning(0,"Clipboard Error","Failed to paste to the Clipboard\nTry again with a smaller image size");
        }
    }
    *cstate = ExitState;
}

void copybitmapwindow_toclipboard_command(int *cstate,HighLevelEvent *event,void **)
{
     static Point p1;
     static View3dWindow *zoomwindow;
     View3dWindow *window;
     View2dWindow *view2dwindow;
     Point p2;
     double t;
      switch (*cstate)
        {  case InitialState :
             *cstate = 1;
             break;
           case 1 :
             if (event->isa(Coordinateuv))
               {  if ((window = ((CoordinateuvEvent *) event)->window()) != 0)
                    {  zoomwindow = window;
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
             {
                  if ((window = ((CoordinateuvEvent *) event)->window()) != 0 && window == zoomwindow)
                  {
                      window->cursorerase();
                       p2 = ((CoordinateuvEvent *) event)->coordinate();
                       p1 = window->uvtoscreen(p1);
                       p2 = window->uvtoscreen(p2);
                       if (p1.x > p2.x)
                         {  t = p1.x;  p1.x = p2.x;  p2.x = t;
                         }
                       if (p1.y > p2.y)
                         {  t = p1.y;  p1.y = p2.y;  p2.y = t;
                         }
                       if (p1.x != p2.x && p1.y != p2.y)
                       {
                           app->processEvents();
                           window->paintevent();
                           QPixmap screenshot = QPixmap::fromImage(((QGLWidget*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view()->viewport())->grabFrameBuffer());
                           qreal pdpr = screenshot.devicePixelRatio();
                           QPixmap cutout = screenshot.copy(p1.x*pdpr+1,p1.y*pdpr+1,p2.x*pdpr-p1.x*pdpr-2,p2.y*pdpr-p1.y*pdpr-2);

                           QClipboard *clipboard = QApplication::clipboard();
                           if(clipboard)
                           {
                               clipboard->clear();
                               clipboard->setPixmap(cutout);
                               // check to see if it happend
                               QPixmap test = clipboard->pixmap();
                               if(test.isNull())
                                   QMessageBox::warning(0,"Clipboard Error","Failed to paste to the Clipboard\nTry again with a smaller image size");
                           }
                       }
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
    if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+31);
    }
    else if (*cstate == 2)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+32);
    }
}

/**
 * @brief copybitmap_command
 * @param cstate
 *
 * Entry point for the copy bitmap command
 */
void copybitmap_command(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("CopyBitmap_Dialog");
#ifdef USING_WIDGETS
 RCSTRING CSTRbitmapsizel[] = { "Original","2x","3x","4x","5x","6x","7x","8x"  };
 RCCHAR bm1[20],bm2[20],bm3[20],bm4[20],bm5[20],bm6[20],bm7[20],bm8[20];
 RCCHAR *bitmapsizel[] = { bm1, bm2, bm3, bm4, bm5, bm6, bm7, bm8 };

 RCSTRING CSTRsavetypel[] = {"Clipboard", "Windows Bitmap","JPEG" };
 RCCHAR sv1[20],sv2[20],sv3[20];
 RCCHAR *savetypel[] = { sv1, sv2, sv3 };

 RCSTRING CSTRqualityl[] = { "Low", "Medium", "High"  };
 RCCHAR qu1[20],qu2[20],qu3[20];
 RCCHAR *qualityl[] = { qu1 , qu2, qu3 };
#else
 RCCHAR *bitmapsizel[] = {  _RCT("Original"),_RCT("2x"),_RCT("3x"),_RCT("4x"),_RCT("5x"),_RCT("6x"),_RCT("7x"),_RCT("8x")  };
 RCCHAR *savetypel[] = {  _RCT("Clipboard"), _RCT("Windows Bitmap"), _RCT("JPEG")  };
 RCCHAR *qualityl[] = {  _RCT("Low"), _RCT("Medium"), _RCT("High")  };
#endif

 RCCHAR defExt[10],dlgTitle[100];
 RCCHAR filename[300],*fn;
 int bitmapsize,savetype,quality;

#ifdef USING_WIDGETS
 for(int i=0; i<8; i++)
     RCSTRCPY(bitmapsizel[i],&CSTRbitmapsizel[i]);

 for(int i=0; i<3; i++)
     RCSTRCPY(savetypel[i],&CSTRsavetypel[i]);

 for(int i=0; i<3; i++)
     RCSTRCPY(qualityl[i],&CSTRqualityl[i]);
#endif

  if (cadwindow->getcurrentwindow() == 0)
    {  cadwindow->MessageBox("Please select the window to copy first","Copy Bitmap",MB_OK);
       return;
    }

  bitmapsize = v.getinteger("cb::bitmapsize");
  savetype = v.getinteger("cb::savetype");
  quality = v.getinteger("cb::quality");

  dialog.add(new ListDialogControl(100,8,bitmapsizel,&bitmapsize));
  dialog.add(new ListDialogControl(101,3,savetypel,&savetype));
  dialog.add(new ListDialogControl(102,3,qualityl,&quality));

  if (dialog.process() == TRUE)
    {  v.setinteger("cb::bitmapsize",bitmapsize);
       v.setinteger("cb::savetype",savetype);
       v.setinteger("cb::quality",quality);

#ifdef USING_WIDGETS
       if (savetype > 0)
       {
            RCOPENFILENAME ofn;
            RCCHAR dirname[300],filter[300];
            RCUINT i;

            _getcwd(filename, 300);
            //filename[0] = '\0';

            if (savetype == 1)
              strcpy(filter,_RCT("Windows Bitmap Files (*.bmp)"));
            else
              strcpy(filter,_RCT("JPEG Files (*.jpg *.jpeg)"));

            //for (i = 0; filter[i] != '\0'; i++)
            //  if (filter[i] == '|') filter[i] = '\0';

            memset(&ofn, 0, sizeof(RCOPENFILENAME));
            ofn.lStructSize = sizeof(RCOPENFILENAME);
            ofn.hwndOwner = cadwindow->gethwnd();
            ofn.lpstrFilter = filter;
            ofn.nFilterIndex = 1;
            ofn.lpstrFile= filename;
            ofn.nMaxFile = 300;
            ofn.lpstrInitialDir = dirname;
            if (savetype == 1)
            {
                 strcpy(defExt,_RCT("bmp"));
                 ofn.lpstrDefExt = defExt;
                 strcpy(dlgTitle,_RCT("Select the bitmap save filename"));
                 ofn.lpstrTitle = dlgTitle;
                 savetype = 1;
            }
            else
            {
                 strcpy(defExt,_RCT("jpg"));
                 ofn.lpstrDefExt = defExt;
                 strcpy(dlgTitle,_RCT("Select the JPEG save filename"));
                 ofn.lpstrTitle = dlgTitle;
                 savetype = 4;
            }

            ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn) == 0)
              return;
            fn = filename;

            qDebug() << "Bitmap files name : " << QString(filename);
       }
       else
       {
           fn = 0;
            savetype = 3; // clipboard
       }
#else
       if (savetype > 0)
         {OPENFILENAME ofn;
          RCCHAR dirname[300],filter[300];
          RCUINT i;

            _getcwd(filename, sizeof(filename));
            filename[0] = '\0';
            if (savetype == 1)
              strcpy(filter,"Windows Bitmap Files *.BMP|*.BMP||");
            else
              strcpy(filter,"JPEG Files *.JPG|*.JPG||");
            for (i = 0; filter[i] != '\0'; i++)
              if (filter[i] == '|') filter[i] = '\0';
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = cadwindow->gethwnd();
            ofn.lpstrFilter = filter;
            ofn.nFilterIndex = 1;
            ofn.lpstrFile= filename;
            ofn.nMaxFile = 300;
            ofn.lpstrInitialDir = dirname;
            if (savetype == 1)
              {  ofn.lpstrDefExt = _RCT("BMP");
                 ofn.lpstrTitle = _RCT("Select the bitmap save filename");
                 savetype = 1;
              }
            else
              {  ofn.lpstrDefExt = _RCT("JPG");
                 ofn.lpstrTitle = _RCT("Select the JPEG save filename");
                 savetype = 4;
              }

            ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn) == 0)
              return;
            fn = filename;
         }  
       else
         {  fn = 0;
            savetype = 3;
         }
#endif

#ifdef USING_WIDGETS
       if (cadwindow->getcurrentwindow() != 0)
       {
           QPixmap screenshot = QPixmap::fromImage(((QGLWidget*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view()->viewport())->grabFrameBuffer());
           QPixmap scaled = screenshot.scaled(screenshot.width()*(bitmapsize+1),screenshot.height()*(bitmapsize+1));

           if( savetype == 1 ) // bitmap
               scaled.save(QString(fn));
           else if( savetype == 4 )
           { // jpg
               QImageWriter iwriter;
               iwriter.setFileName(QString(fn));
               iwriter.setFormat("jpg");
               // jpeg quality 0 ~ 100
               switch(quality)
               {
               case 0: // 0 quality is useless
                   iwriter.setQuality(20);
                   break;
               case 1:
                   iwriter.setQuality(60);
                   break;
               case 2:
                   iwriter.setQuality(100);
                   break;
               }
               iwriter.write(scaled.toImage());
           }
           else if( savetype == 3)
           { // clipboard
               QClipboard *clipboard = QApplication::clipboard();
               if(clipboard)
               {
                   clipboard->clear();
                   clipboard->setPixmap(scaled);
                   // check to see if it happend
                   QPixmap test = clipboard->pixmap();
                   qDebug() << "test" << test;
                   qDebug() << "test rect" << test.rect();
                   if(test.isNull())
                       QMessageBox::warning(0,"Clipboard Error","Failed to paste to the Clipboard\nTry again with a smaller image size");
               }
           }
       }
#else
       if (cadwindow->getcurrentwindow() != 0)
         {  if (cadwindow->getcurrentwindow()->getsurfacetype() == View3dSurf)
              {  View3dOffScreenBitmap vosb(cadwindow->getcurrentwindow(),bitmapsize+1);
                 if (vosb.getdefined())
                   {  vosb.paintevent();
                      vosb.saveimage(savetype,fn,quality);
                   }
                 else
                   cadwindow->MessageBox("The bitmap is too large to create.","Copy Bitmap",MB_OK);
              }
            else 
              {  View2dOffScreenBitmap vosb((View2dWindow *)cadwindow->getcurrentwindow(),((View2dWindow *)cadwindow->getcurrentwindow())->getview2d(),bitmapsize+1);
                 if (vosb.getdefined())
                   {  vosb.paintevent();
                      vosb.saveimage(savetype,fn,quality);
                   }
                 else
                   cadwindow->MessageBox("The bitmap is too large to create.","Copy Bitmap",MB_OK);
              }
         }
#endif

    } 
  *cstate = ExitState;
}

/**
 * @brief printpreview_command1
 * @param cstate
 *
 * Entry point for the Print Preview Window command
 *
 */
void printpreview_command1(int *cstate,HighLevelEvent *,void **)
{ResourceString rs5(NCPRINT+5),rs7(NCPRINT+7);
 RECT rect;
 View3dWindow *view3dwindow;
  *cstate = ExitState;
 QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

#ifdef USING_WIDGETS
 if ((view3dwindow = cadwindow->getcurrentwindow()) == 0)
   cadwindow->MessageBox(rs7.gets(),rs5.gets(),MB_ICONSTOP);
 else if (cadwindow->printpreviewwindow == 0)
 {
     QPrinterInfo printerinfo = QPrinterInfo::defaultPrinter();
     QString lastPrinterName = settings.value("Strings/pr::lastprinter",printerinfo.printerName()).toString();

     if(settings.value("Integers/pr::uselastprinter").toInt() && lastPrinterName != "InternalPDF")
     {
         QList<QPrinterInfo> availablePrinters = QPrinterInfo::availablePrinters();
         for(int i=0; i<availablePrinters.size(); i++)
             if(availablePrinters.at(i).printerName() == lastPrinterName)
                 printerinfo = availablePrinters.at(i);
     }

     QPrinter *printer = new QPrinter(printerinfo,QPrinter::HighResolution);
     //QPrinter printer(printerinfo,QPrinter::HighResolution);
     printer->pageLayout().setUnits(QPageLayout::Millimeter);

     if(lastPrinterName == "InternalPDF")
         printer->setOutputFormat(QPrinter::PdfFormat);

     qDebug() << "default paper name : " << printerinfo.defaultPageSize().name();
     qDebug() << "printer paper name : " << printer->paperName();
     //printer.setPageSize(QPageSize(QSizeF(v.getreal("pp::paperwidth"),v.getreal("pp::paperheight")),QPageSize::Millimeter));

     if (view3dwindow->getsurfacetype() == View2dSurf)
     {
        //cadwindow->printpreviewwindow->setview2dwindow((View2dWindow *)view3dwindow);
        v.setreal("pp::paperwidth",((View2dWindow*)view3dwindow)->getview2d()->getpaperwidth());
        v.setreal("pp::paperheight",((View2dWindow*)view3dwindow)->getview2d()->getpaperheight());
        v.setreal("pp::printscale",0.0);
        v.setreal("pr::printscale",0.0);
        v.setstring("pr::scalename","Fit to Paper");

        if(QString(v.getstring("pp::orientation")) == "portrait")
        {
            printer->setPageSize(QPageSize(QSizeF(v.getreal("pp::paperwidth"),v.getreal("pp::paperheight")),
                                           QPageSize::Millimeter));
            printer->setPageOrientation(QPageLayout::Portrait);
            v.setstring("pp::orientation","portrait");
        }
        else
        {
            printer->setPageSize(QPageSize(QSizeF(v.getreal("pp::paperheight"),v.getreal("pp::paperwidth")),
                                           QPageSize::Millimeter));
            printer->setPageOrientation(QPageLayout::Landscape);
            v.setstring("pp::orientation","landscape");
        }
     }
     else
     {
         v.setreal("pp::printscale",0.0);
         v.setreal("pr::printscale",0.0);
         v.setstring("pr::scalename","Fit to Paper");

         if(QString(v.getstring("pp::orientation")) == "portrait")
         {
             printer->setPageSize(QPageSize(QSizeF(v.getreal("pp::paperwidth"),v.getreal("pp::paperheight")),
                                            QPageSize::Millimeter));
             printer->setPageOrientation(QPageLayout::Portrait);
         }
         else
         {
             printer->setPageSize(QPageSize(QSizeF(v.getreal("pp::paperheight"),v.getreal("pp::paperwidth")),
                                            QPageSize::Millimeter));
             printer->setPageOrientation(QPageLayout::Landscape);
         }
     }

     //if( QString(v.getstring("pp::orientation")) == "portrait" )
     //    printer->setOrientation(QPrinter::Portrait);
     //else
     //    printer->setOrientation(QPrinter::Landscape);

     printer->setDocName(QString(db.gettitle()));

     QtMainWindow *mw = (QtMainWindow*)cadwindow->gethwnd();
     //RCPrintPreview_Dialog preview(&printer, this, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
     RCPrintPreview_Dialog preview(printer, mw, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

     qDebug() << "printer paper name : " << printer->paperName();
     qDebug() << "preview paper name : " << preview.mPaperTypes->currentText();

     preview.move(QPoint(mw->geometry().x(),mw->geometry().y()));
     int dw = mw->frameGeometry().width()-mw->geometry().width();
     int dh = mw->frameGeometry().height()-mw->geometry().height();
     preview.resize(QSize(mw->geometry().width()-dw,mw->geometry().height()-dh));

     if (view3dwindow->getsurfacetype() == View3dSurf &&
         view3dwindow->getrepaintstyle() == RayTraced )
         mw->connect(&preview, SIGNAL(paintRequested(QPrinter*)),mw, SLOT(printRTSheet(QPrinter*)));
     else
         mw->connect(&preview, SIGNAL(paintRequested(QPrinter*)),mw, SLOT(printSheet(QPrinter*)));

     preview.exec();

     delete printer;

 }
#else
  if ((view3dwindow = cadwindow->getcurrentwindow()) == 0)
    cadwindow->MessageBox(rs7.gets(),rs5.gets(),MB_ICONSTOP);
  else if (cadwindow->printpreviewwindow == 0)
    {  cadwindow->showfloatingmenu(0);
       GetClientRect(cadwindow->gethwnd(),&rect);
       ShowWindow(cadwindow->subwindow->gethwnd(),SW_HIDE);
       cadwindow->coordinateentry->setvisible(SW_HIDE);
       cadwindow->printpreviewwindow = new PrintPreviewWindow(_RCT("CadWindow"),cadwindow->gethwnd(),WS_CHILD | WS_VISIBLE,0,0,rect.right,rect.bottom);
       if (view3dwindow->getsurfacetype() == View3dSurf)
         cadwindow->printpreviewwindow->setview3dwindow(view3dwindow);
       else
         cadwindow->printpreviewwindow->setview2dwindow((View2dWindow *)view3dwindow);
    }
#endif
}

struct RBPrintPreview2_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgi1,*qgi2,*qgi3,*qgi4;
#endif
   double width,height,scale;
   Point xaxis,yaxis;
   View3dSurface *lastSurface;
};

void RBPrintPreview2(int,void *data,Point *p2,View3dSurface *surface)
{
  RBPrintPreview2_data *rbdata = (RBPrintPreview2_data *) data;

  if (surface != 0)
  {
       rbdata->lastSurface = surface;
       double scale = rbdata->scale;
       if(scale == 0.0)
           scale = 1.0;
       rbdata->xaxis = surface->getuaxis() * rbdata->width * scale;
       rbdata->yaxis = surface->getvaxis() * rbdata->height * scale;
  }
  Line line1(*p2,*p2+rbdata->xaxis);
  line1.qgi = rbdata->qgi1;
  line1.draw(DM_INVERTDASH);
  if(line1.qgi)
      line1.qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
  rbdata->qgi1 = line1.qgi;

  Line line2(*p2+rbdata->xaxis,*p2+rbdata->xaxis+rbdata->yaxis);
  line2.qgi = rbdata->qgi2;
  line2.draw(DM_INVERTDASH);
  if(line2.qgi)
      line2.qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
  rbdata->qgi2 = line2.qgi;

  Line line3(*p2+rbdata->xaxis+rbdata->yaxis,*p2+rbdata->yaxis);
  line3.qgi = rbdata->qgi3;
  line3.draw(DM_INVERTDASH);
  if(line3.qgi)
      line3.qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
  rbdata->qgi3 = line3.qgi;

  Line line4(*p2+rbdata->yaxis,*p2);
  line4.qgi = rbdata->qgi4;
  line4.draw(DM_INVERTDASH);
  if(line4.qgi)
      line4.qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
  rbdata->qgi4 = line4.qgi;

}

class PPPaperSizeDialogControl : public ListDialogControl
{
  private:
     int lastindex;
   public:
     PPPaperSizeDialogControl(int id,int n,RCCHAR **list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     PPPaperSizeDialogControl(int id,int n,QString *list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
};

void PPPaperSizeDialogControl::load(Dialog *dialog)
{
  ListDialogControl::load(dialog);
  changefocus(dialog);
}

void PPPaperSizeDialogControl::changefocus(Dialog *dialog,int)
{
  int index;
  if (dialog->currentvalue(100,&index) && index != lastindex)
    {  if (index < View2d::nsizes - 1)
         {  ((RealDialogControl *)dialog->getcontrol(101))->change(dialog,View2d::widths[index]);
            ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,View2d::heights[index]);
         }
       else
         {  ((RealDialogControl *)dialog->getcontrol(101))->change(dialog,v.getreal("cd::nonstdwidth"));
            ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,v.getreal("cd::nonstdheight"));
         }
       lastindex = index;
    }
}

/**
 * @brief printpreview_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry Point for the Print Preview Scaled Region command
 *
 */
void printpreview_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBPrintPreview2_data *rbdata = (RBPrintPreview2_data *) *data;
 View3dWindow *view3dwindow;
 ResourceString rs5(NCPRINT+5),rs7(NCPRINT+7);
 Point p;
 RECT rect;
 Dialog dialog("PrintPreview_Dialog");
 DialogControl *dc;
 int index=0;

  switch (*cstate)
    {
       case InitialState :
       rbdata = new RBPrintPreview2_data;  *data = rbdata;
       if (rbdata != 0 && cadwindow->printpreviewwindow == 0)
         {
            rbdata->qgi1=0;rbdata->qgi2=0;rbdata->qgi3=0;rbdata->qgi4=0;
            rbdata->lastSurface=0;

            rbdata->width = v.getreal("pp::paperwidth");
            rbdata->height = v.getreal("pp::paperheight");
            rbdata->scale = v.getreal("pp::printscale") == 0.0 ? 1 : v.getreal("pp::printscale");
            for (index = 0 ; index < View2d::nsizes-1 ; index++)
              if (fabs(rbdata->width-View2d::widths[index]) < 1.0 && fabs(rbdata->height-View2d::heights[index]) < 1.0)
                break;
            dialog.add(new PPPaperSizeDialogControl(100,View2d::nsizes,View2d::sizes,&index));
            dialog.add(new RealDialogControl(101,&rbdata->width,1.0,0.0,1E10,ScaleByLength));
            dialog.add(new RealDialogControl(102,&rbdata->height,1.0,0.0,1E10,ScaleByLength));
            //dialog.add(new RealDialogControl(103,&rbdata->scale,1.0,0.0,1E10));
            dialog.add(new ScaleDialogControl(104,&rbdata->scale));
            if (dialog.process() == TRUE)
              {
                 v.setreal("pp::paperwidth",rbdata->width);
                 v.setreal("pp::paperheight",rbdata->height);
                 v.setreal("pp::printscale",rbdata->scale);
                 v.setreal("pr::paperwidth",rbdata->width);
                 v.setreal("pr::paperheight",rbdata->height);
                 v.setreal("pr::printscale",rbdata->scale);
                 v.setreal("vw::plotscale",rbdata->scale);
                 v.setstring("pp::papername",View2d::sizes[index].data());
                 v.setstring("pr::scalename","");
                 if(rbdata->scale == 0.0)
                 {
                     v.setreal("vw::plotscale",rbdata->scale);
                    v.setreal("pp::printscale",1.0);
                    v.setreal("pr::printscale",1.0);
                    v.setstring("pr::scalename","Fit to Paper");
                 }
                 if(rbdata->height > rbdata->width)
                     v.setstring("pp::orientation","portrait");
                 else
                     v.setstring("pp::orientation","landscape");

                 if (index == View2d::nsizes - 1)
                 {
                     v.setreal("cd::nonstdwidth",rbdata->width);
                     v.setreal("cd::nonstdheight",rbdata->height);
                 }

                 qDebug() << "pp::paperwidth" << v.getreal("pp::paperwidth");
                 qDebug() << "pp::paperheight" << v.getreal("pp::paperheight");
                 qDebug() << "pp::printscale" << v.getreal("pp::printscale");
                 qDebug() << "pr::printscale" << v.getreal("pr::printscale");
                 qDebug() << "pp::papername" << QString(v.getstring("pp::papername"));
                 qDebug() << "pp::orientation" << QString(v.getstring("pp::orientation"));

                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0)
              view3dwindow = ((Coordinate3dEvent *)event)->getwindow();
            else
              view3dwindow = cadwindow->getcurrentwindow();
            if (view3dwindow != 0)
            {
                 // the QT print preview dialog is posted by QMainWindow code
                if(cadwindow && cadwindow->gethwnd())
                {
                    if(rbdata->qgi1)
                        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi1);
                    delete rbdata->qgi1;rbdata->qgi1=0;
                    if(rbdata->qgi2)
                        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi2);
                    delete rbdata->qgi2;rbdata->qgi2=0;
                    if(rbdata->qgi3)
                        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi3);
                    delete rbdata->qgi3;rbdata->qgi3=0;
                    if(rbdata->qgi4)
                        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi4);
                    delete rbdata->qgi4;rbdata->qgi4=0;

                    QtMainWindow *mw = (QtMainWindow*)cadwindow->gethwnd();

                    QPrinter printer(QPrinter::HighResolution);
                    printer.pageLayout().setUnits(QPageLayout::Millimeter);

                    // Qt printers are always set as width by height for any orientation
                    printer.setPageSize(QPageSize(QSizeF(v.getreal("pp::paperwidth"),v.getreal("pp::paperheight")),
                                                  QPageSize::Millimeter));

                    if( QString(v.getstring("pp::orientation")) == "portrait")
                        printer.setPageOrientation(QPageLayout::Portrait);
                    else
                        printer.setPageOrientation(QPageLayout::Landscape);

                    printer.setDocName(QString(db.gettitle()));
                    qDebug() << "pp::paperwidth" << v.getreal("pp::paperwidth");
                    qDebug() << "pp::paperheight" << v.getreal("pp::paperheight");

                    // using a sub classed version
                    //QPrintPreviewDialog preview(&printer, mw, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
                    RCPrintPreview_Dialog preview(&printer, mw, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
                    qDebug() << "pp::paperwidth" << v.getreal("pp::paperwidth");
                    qDebug() << "pp::paperheight" << v.getreal("pp::paperheight");
                    if(QString(v.getstring("pp::orientation")) == "portrait")
                        preview.setPortraitOrientation();
                    else
                        preview.setLandscapeOrientation();

                    preview.move(QPoint(mw->geometry().x(),mw->geometry().y()));
                    int dw = mw->frameGeometry().width()-mw->geometry().width();
                    int dh = mw->frameGeometry().height()-mw->geometry().height();
                    preview.resize(QSize(mw->geometry().width()-dw,mw->geometry().height()-dh));

                    // this is the left bottom point
                    Point pickp = ((Coordinate3dEvent *)event)->getp();
                    //pickp.x = pickp.x + rbdata->width * rbdata->scale / 2.0;
                    //pickp.y = pickp.y + rbdata->height * rbdata->scale / 2.0;

                    // put the pick point in the middle of the paper frame
                    // this makes is easier for the print preview command
                    // calculate the correct z value for the mid point
                    double scale = rbdata->scale;
                    if(scale == 0.0)
                        scale = 1.0;
                    rbdata->xaxis = rbdata->lastSurface->getuaxis() * rbdata->width * scale;
                    rbdata->yaxis = rbdata->lastSurface->getvaxis() * rbdata->height * scale;
                    pickp = pickp + (rbdata->xaxis+rbdata->yaxis) * 0.5;

                    // pass the pick point to the mainwindow
                    // KMJ: TODO: find a better method to do this??
                    v.setreal("pp:px",(pickp.x));
                    v.setreal("pp:py",(pickp.y));
                    v.setreal("pp:pz",(pickp.z));
                    v.setreal("pp:w",rbdata->width * rbdata->scale);
                    v.setreal("pp:h",rbdata->height * rbdata->scale);

                    if (view3dwindow->getsurfacetype() == View3dSurf &&
                        view3dwindow->getrepaintstyle() == RayTraced )
                        mw->connect(&preview, SIGNAL(paintRequested(QPrinter*)),mw, SLOT(printRTSheetSelect(QPrinter*)));
                    else
                        mw->connect(&preview, SIGNAL(paintRequested(QPrinter*)),mw, SLOT(printSheetSelect(QPrinter*)));

                    qDebug() << "pp::paperwidth" << v.getreal("pp::paperwidth");
                    qDebug() << "pp::paperheight" << v.getreal("pp::paperheight");
                    qDebug() << "pp::printscale" << v.getreal("pp::printscale");
                    qDebug() << "pr::printscale" << v.getreal("pr::printscale");
                    qDebug() << "pp::papername" << QString(v.getstring("pp::papername"));
                    qDebug() << "pp::orientation" << QString(v.getstring("pp::orientation"));

                    preview.exec();

                    //cadwindow->paintall();
                }
#if 0
                // old version
                 p = view3dwindow->modeltouv(((Coordinate3dEvent *)event)->getp());
                 GetClientRect(cadwindow->gethwnd(),&rect);
                 ShowWindow(cadwindow->subwindow->gethwnd(),SW_HIDE);
                 cadwindow->coordinateentry->setvisible(SW_HIDE);
                 cadwindow->printpreviewwindow = new PrintPreviewWindow(_RCT("PrintPreviewWindow"),cadwindow->gethwnd(),WS_CHILD | WS_VISIBLE,0,0,rect.right,rect.bottom);
                 if (view3dwindow->getsurfacetype() == View3dSurf)
                   cadwindow->printpreviewwindow->setview3dwindow(view3dwindow);
                 else
                   cadwindow->printpreviewwindow->setview2dwindow((View2dWindow *)view3dwindow);
                 cadwindow->printpreviewwindow->setprintscale(rbdata->scale);
                 cadwindow->printpreviewwindow->setzoom(p.x,p.y,p.x+rbdata->width * rbdata->scale,p.y + rbdata->height * rbdata->scale);
                 cadwindow->printpreviewwindow->printer.setblackandwhite(v.getinteger("pl::blackandwhite"));
#endif
            }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {
           *cstate = ExitState;
         }
       break;
    }
  if (*cstate == 1)
  {
       //((RCView*)cadwindow->getcurrentwindow()->gethwnd())->freezeBackImage();

       rubberband.begin(0);
       rubberband.add(RBPrintPreview2,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCPRINT+12);
  }
  else
  {
      //((RCView*)cadwindow->getcurrentwindow()->gethwnd())->unFreezeBackImage();

      if(rbdata->qgi1)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi1);
      delete rbdata->qgi1;rbdata->qgi1=0;
      if(rbdata->qgi2)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi2);
      delete rbdata->qgi2;rbdata->qgi2=0;
      if(rbdata->qgi3)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi3);
      delete rbdata->qgi3;rbdata->qgi3=0;
      if(rbdata->qgi4)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi4);
      delete rbdata->qgi4;rbdata->qgi4=0;

    delete rbdata;
  }
}

void printpreview_command3(int *cstate,HighLevelEvent *,void **)
{ cadwindow->printpreviewwindow->print();
  *cstate = ExitState;
}

void printpreview_command4(int *cstate,HighLevelEvent *,void **)
{ cadwindow->printpreviewwindow->printer.setupprinter();
  InvalidateRect(cadwindow->printpreviewwindow->gethwnd(),0,0);
  *cstate = ExitState;
}

void printpreview_command5(int *cstate,HighLevelEvent *,void **)
{double scale;
 int blackandwhite;
 Dialog dialog("PrintPreviewScale_Dialog");
 DialogControl *dc;
  scale = cadwindow->printpreviewwindow->getprintscale();
  blackandwhite = v.getinteger("pl::blackandwhite");
  dialog.add(dc = new RealDialogControl(100,&scale,1.0,0.0001,1E10));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new CheckBoxDialogControl(101,&blackandwhite));
  if (dialog.process() == TRUE)
    {  InvalidateRect(cadwindow->printpreviewwindow->gethwnd(),0,0);
       v.setinteger("pl::blackandwhite",blackandwhite);
       cadwindow->printpreviewwindow->setprintscale(scale);
       cadwindow->printpreviewwindow->printer.setblackandwhite(v.getinteger("pl::blackandwhite"));
    }
  *cstate = ExitState;
}

void printpreview_command6(int *cstate,HighLevelEvent *,void **)
{ 
  if (cadwindow->printpreviewwindow != 0)
    {  DestroyWindow(cadwindow->printpreviewwindow->gethwnd());
       cadwindow->printpreviewwindow = 0;
       ShowWindow(cadwindow->subwindow->gethwnd(),SW_SHOW);
       cadwindow->showfloatingmenu(1);
       cadwindow->coordinateentry->setvisible(SW_SHOW);
       *cstate = ExitState;
    }

}

