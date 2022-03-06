
#include "ncwin.h"
#include "RCDialog.h"
#include "RCCentralWidget.h"
#include "RCStyleSheets.h"

#define INSTALLLICENSE

extern int check_license(char *label,char *label1,char *label2,int application,int showmessages);
extern int check_license(RCCHAR *label,RCCHAR *label1,RCCHAR *label2,RCCHAR application,RCCHAR showmessages);

extern int filesaveselectionas(EntityList *list,const RCCHAR *filename, const RCCHAR *filepath=0);
extern int filesaveselectionas(EntityList *list,const char *filename, const char *filepath=0);

static const int max_info = 50;

int DisableSave(void)
{RCCHAR n[100],d[300];
 int i,status;
  status = v.getfirststring(_RCT("in::"),n,d);
  for (i = 0 ; i < max_info ; i++)
    {  if (status && strncmp(_RCT("SaveMode"),n+4,8) == 0 && strcmp(d,_RCT("Disable Save")) == 0)
         break;
       status = v.getnextstring(_RCT("in::"),n,d);
    }
  return status && strcmp(d,"Disable Save") == 0;
}

int filesave(void)
{
  CadFile outfile;
 int status,nanimation,narchitect,nmachining,i,versions;
 RCCHAR filename1[600],filename2[600],filename[600],title[600],backupdirectory[600];
 ResourceString rs0(NCMAIN);
 ResourceString rs1(NCMAIN+1);
 ResourceString rs39(NCMAINB+39);

  if (DisableSave() && (GetKeyState(VK_SHIFT) & 0x8000) == 0)  //  If disable save has been set - don't display the save request dialog.
    {  cadwindow->MessageBox("This file cannot be saved.","Save",MB_OK);
       return 0;
    }

  if ((cad_version&15) == TRICAD_DEMO || (cad_version&15) == REALCAD_DEMO)
    {int i;
       i = 0;
       for (db.geometry.start() ; i < 100 && db.geometry.next() != 0 ; i++);
       if (i == 100)
         {  cadwindow->MessageBox("The demonstration version can only save small models.","Demonstration Version",MB_OK);
            return 0;
         }
    }

#if defined(INSTALLLICENSE) && (VERSION != FLEXICAD)
    {int i;
     Entity *e;
       for (db.geometry.start(),i = 0 ; i < 100 && db.geometry.next() != 0 ; i++);
         if (i == 100)
           {  if (! check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,0))
                {  cadwindow->MessageBox("This unlicensed software can only save small models.","Unlicensed software.",MB_ICONINFORMATION);
                   return 0;
                }
           }

       nmachining = narchitect = nanimation = 0;
       for (db.geometry.start() ;  (e = db.geometry.next()) != 0 ; )
         if (e->isa(group_entity))
           {  nmachining += strncmp("NCMill::",((Group *)e)->getclassname(),8) == 0;
              nanimation += strncmp("AN::",((Group *)e)->getinstance(),4) == 0;
           }
         else
           narchitect += e->isa(wall_entity);

         if (nanimation > 0 && ! check_license((char*)"Animation Application", (char*)"RealCADKey5", (char*)"RealCADKey6",2,1))  // Animation
           {  cadwindow->MessageBox("This software is not licensed to save animation files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }

         if (nmachining > 0 && ! check_license((char*)"Machining Application", (char*)"RealCADKey7", (char*)"RealCADKey8",3,1))  // Machining
           {  cadwindow->MessageBox("This software is not licensed to save machining files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }

         if (narchitect > 0 && ! check_license((char*)"Architect Application", (char*)"RealCADKey3", (char*)"RealCADKey4",1,1))  //  Architect
           {  cadwindow->MessageBox("This software is not licensed to save architect files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }
    }
#endif

  //  Copy the versions 
  versions = v.getinteger("db::versions");
  //_getcwd(backupdirectory,300);
  strcpy(filename,db.getname());
  if (strrchr(filename,'.') != 0)
    *strrchr(filename,'.') = 0; 

  GetPrivateProfileString("Backup","BackupDirectory","",backupdirectory,600,home.getinifilename());
  qDebug() << "backup folder : " << QString(backupdirectory);
  for (i = versions ; i > 0 ; i--)
    {  if (i == 1)
         strcpy(filename1,db.getname());
       else
         {
            sprintf(filename1,_RCT("%ls-BAK%d.cad"),filename,i-1);
            if (strlen(backupdirectory) > 0)
              {  GetFileTitle(filename1,title,600);
                 sprintf(filename1,_RCT("%ls\\%ls"),backupdirectory,title);
              }
         }
       sprintf(filename2,_RCT("%ls-BAK%d.cad"),filename,i);
       if (strlen(backupdirectory) > 0)
         {  GetFileTitle(filename2,title,600);
            sprintf(filename2,_RCT("%ls\\%ls"),backupdirectory,title);
         }
       qDebug() << "filename1 (old) : " << QString(filename1);
       qDebug() << "filename2 (new) : " << QString(filename2);
       CopyFile(filename1,filename2,FALSE);
    }
 
  if (! outfile.open(db.getname(),"wb"))
    {  cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
       status = 0;
    }
  else
    {  status  = db.save(&outfile);
       status &= outfile.close();
       if (! status)
         {  cadwindow->MessageBox(rs39.gets(),rs1.gets(),MB_ICONINFORMATION);
            _unlink(db.getname());
         }
       else
         program->filesaveapps();
       if (v.getinteger("db::autosaveenabled") != 0)
         program->setautosavetimer();
    }
  return status;
}
/**
 * @brief filesaveselectionas
 * @param list
 * @param filename
 * @return
 *
 * Entry point for save selected entities
 *
 * KMJ : Working in the Qt version
 *
 */
int filesaveselectionas(EntityList *list,const char *filename, const char *filepath)
{

    //return filesaveselectionas(list,(const RCCHAR*)CHAR2RCCHR(filename));
    return filesaveselectionas(list,(const RCCHAR*)QString(filename).constData());
}

int filesaveselectionas(EntityList *list,const RCCHAR *filename, const RCCHAR *filepath)
{CadFile outfile;
 int   status,nanimation,narchitect,nmachining;
 ResourceString rs0(NCMAIN);
 ResourceString rs1(NCMAIN+1);
 ResourceString rs39(NCMAINB+39);
 EntityList selection;
 RCOPENFILENAME ofn;
 RCCHAR szFile[600], szFileTitle[600],szDirName[600],szFilter[300],title[600];
 RCUINT  i;
 ResourceString rsnc1(NC+1);
 ResourceString rsnc2(NC+2);

  if (list != 0)
    selection = *list;
  else
    {  selection = state.getselection();
       if (selection.length() == 0)
         {  cadwindow->MessageBox("Please select the entities that make up the figure first.","Create Figure",MB_ICONINFORMATION);
            return 0;
         }
    }

  if ((cad_version&15) == TRICAD_DEMO || (cad_version&15) == REALCAD_DEMO)
    {int i;
       i = 0;
       for (selection.start() ; i < 100 && selection.next() != 0 ; i++);
       if (i == 100)
         {  cadwindow->MessageBox("The demonstration version can only save small models.","Demonstration Version",MB_ICONINFORMATION);
            return 0;
         }
    }

#if defined(INSTALLLICENSE) && (VERSION != FLEXICAD)
    {int i;
     Entity *e;
       for (selection.start(),i = 0 ; i < 100 && selection.next() != 0 ; i++);
         if (i == 100)
           {  if (! check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,1))
                {  cadwindow->MessageBox("This unlicensed software can only save small models.","Unlicensed software.",MB_ICONINFORMATION);
                   return 0;
                }
           }

       nmachining = narchitect = nanimation = 0;
       for (selection.start() ;  (e = selection.next()) != 0 ; )
         if (e->isa(group_entity))
           {  nmachining += strncmp("NCMill::",((Group *)e)->getclassname(),8) == 0;
              nanimation += strncmp("AN::",((Group *)e)->getinstance(),4) == 0;
           }
         else
           narchitect += e->isa(wall_entity);

         if (nanimation > 0 && ! check_license((char*)"Animation Application", (char*)"RealCADKey5", (char*)"RealCADKey6",2,1))  // Animation
           {  cadwindow->MessageBox("This software is not licensed to save animation files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }

         if (nmachining > 0 && ! check_license((char*)"Machining Application", (char*)"RealCADKey7", (char*)"RealCADKey8",3,1))  // Machining
           {  cadwindow->MessageBox("This software is not licensed to save machining files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }

         if (narchitect > 0 && ! check_license((char*)"Architect Application", (char*)"RealCADKey3", (char*)"RealCADKey4",1,1))  //  Architect
           {  cadwindow->MessageBox("This software is not licensed to save architect files.","RealCAD",MB_ICONINFORMATION);
              return 0;
           }
    }
#endif

  // v8 use the filepath argument if not NULL
  if(strlen(filepath) > 0)
      strcpy(szDirName,filepath);
  // v8
  // v7 code
  //_getcwd(szDirName, sizeof(szDirName));
  szFile[0] = '\0';

  _tcscpy(szFilter,rsnc1.getws());
  //for (i = 0; szFilter[i] != '\0'; i++)
  //  if (szFilter[i] == '|') szFilter[i] = '\0';

  memset(&ofn, 0, sizeof(RCOPENFILENAME));

  ofn.lStructSize = sizeof(RCOPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = rsnc2.getws();
  ofn.lpstrFileTitle = title;
  status = 0;


  if (filename != 0 || GetSaveFileName(&ofn))
    { if (filename != 0)
        strcpy(szFile,filename); 
      if (! outfile.open(szFile,TEXT("wb")))
         cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
      else
        {  status = db.save(&outfile,&selection);
           status &= outfile.close();
           if (! status)
             {  cadwindow->MessageBox(rs39.gets(),rs1.gets(),MB_ICONINFORMATION);
                _unlink(szFile);
             }
           else
             program->filesaveapps();
           if (v.getinteger("db::autosaveenabled") != 0)
             program->setautosavetimer();
        }
    }
  return status;
}

int filesaveas(void)
{ResourceString rsnc3(NC+3);
 RCOPENFILENAME ofn;
 RCCHAR szFile[600], szFileTitle[600],szDirName[600],szFilter[600],szDefExt[256];
 RCUINT  i;
 RCCHAR  oldname[600],title[600];
 ResourceString rsnc1(NC+1);
 ResourceString rsnc2(NC+2);

  // get the last file path
  strcpy(szDirName,v.getstring("db::last-cad-file-path"));
  if(strlen(szDirName) == 0)
     strcpy(szDirName,v.getstring("db::my-drawings-path"));
  if(strlen(szDirName) == 0)
     strcpy(szDirName,QDir::homePath());
  if(strlen(szDirName) == 0)
     _getcwd(szDirName, sizeof(szDirName));
  szFile[0] = '\0';

  if (strcmp(db.getname(),rsnc3.gets()) != 0)
    {//  Use the current filename as the default directory and title.
       strcpy(szFile,db.getname());
       _strlwr(szFile);
       GetFileTitle(szFile,szFileTitle,600);
       if (strstr(szFile,szFileTitle) != szFile && strstr(szFile,szFileTitle) != 0)
         {  strcpy(szDirName,szFile);
            *strstr(szDirName,szFileTitle) = 0;
            GetFileTitle(db.getname(),szFileTitle,600);
         }
       strcpy(szFile,db.getname());
    }


 _tcscpy(szFilter,rsnc1.getws());
  //for (i = 0; szFilter[i] != '\0'; i++)
  // if (szFilter[i] == '|') szFilter[i] = '\0';
 strcpy(szDefExt,"CAD");

 // KMJ : currently using the Windows ofn structure to pass data to the Qt file open
 memset(&ofn, 0, sizeof(RCOPENFILENAME));
 ofn.lStructSize = sizeof(RCOPENFILENAME);
 ofn.hwndOwner = cadwindow->gethwnd();
 ofn.lpstrFilter = szFilter;
 ofn.nFilterIndex = 1;
 ofn.lpstrFile= szFile;
 ofn.nMaxFile = sizeof(szFile);
 ofn.lpstrFileTitle = szFileTitle;
 ofn.nMaxFileTitle = sizeof(szFileTitle);
 ofn.lpstrInitialDir = szDirName;
 ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
 ofn.lpstrDefExt =  szDefExt;//rsnc2.getws();
 ofn.lpstrFileTitle = title;

 if (GetSaveFileName(&ofn))
   {  
      _tcscpy(oldname,db.getname());
      //QDir::setCurrent(QFileInfo(QString(ofn.lpstrFile)).absolutePath());
      //v.setstring("db::last-cad-file-path",QDir::currentPath().data());
      v.setstring("db::last-cad-file-path",QFileInfo(QString(ofn.lpstrFile)).path().data());
      WritePrivateProfileString(_RCT("Strings"),_RCT("db::last-cad-file-path"),
                                QFileInfo(QString(ofn.lpstrFile)).path().data(),home.getinifilename());

      db.setname(ofn.lpstrFile);
      if (filesave())
        {  db.settitle(title);
           cadwindow->updatetitle();
           cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
           return 1;
        }
      else
        {  db.setname(oldname);
           return 0;
        }
   }
 return 0;
}
int filechanged(const char *title,const char *prompt)
{
    return filechanged(CHAR2RCCHR(title),CHAR2RCCHR(prompt));
}

int filechanged( const RCCHAR *title ,const RCCHAR *prompt)
{
  RCCHAR mess[600];
  int status;
  ResourceString ncrs3(NC+3);
  if (DisableSave())  //  If disable save has been set - don't display the save request dialog.
    return 1;
  if (! db.getchanged()) return 1;
  sprintf(mess,prompt,db.getname());
  status = cadwindow->MessageBox(mess,title,MB_YESNOCANCEL);
  if (status == IDCANCEL) return 0;
  if (status == IDYES)
  {
      if (strcmp(db.getname(),ncrs3.gets()) == 0)
         return filesaveas();
       else
         return filesave();
  }
  return 1;
}

void file_command0(int *cstate,HighLevelEvent *,void **)
{
    //cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("File"),buttonmenus.match("Entities"));
    cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("File"),buttonmenus.match("Assistant"));
    *cstate = ExitState;
}

void file_command17(int *cstate,HighLevelEvent *,void **)
{
    if(cadwindow)
        ((QtMainWindow*)cadwindow->gethwnd())->on_action_File_Print_triggered();
    *cstate = ExitState;
}

void file_toolscommand(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}

void file_emailcommand(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}

void file_3DWarehouse(int *cstate,HighLevelEvent *,void **)
{
    QString inipath(home.getpublicfilename(_RCT("RealCADSketchUp.ini")));
    QSettings settings(QString(home.getpublicfilename(_RCT("RealCADSketchUp.ini"))),QSettings::IniFormat);
    QString defaultPath = QDir::homePath() + "/Downloads";
    QString download_directory = settings.value("RealCADSketchupInterface/DownloadDirectory","").toString();

    if(download_directory.length() == 0)
    {
        download_directory = QFileDialog::getExistingDirectory(0,"Select a download directory",defaultPath,
                                    QFileDialog::ShowDirsOnly
                                    /*| QFileDialog::DontUseNativeDialog*/);

        if(download_directory.length() > 0)
        {
            QString msg = "Save all downloads to " + download_directory;
            int ret = QMessageBox::question(0, "Save all downloads", msg,
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if(ret == QMessageBox::Yes)
                settings.setValue("RealCADSketchupInterface/DownloadDirectory",download_directory);
            else
                settings.setValue("RealCADSketchupInterface/DownloadDirectory","");
        }
    }

    QProcess *lastProcess = ((RCApplication*)app)->mSketchUpBrowser;
    if(lastProcess != 0)
    {
        Program::disconnect((QProcess*)(((RCApplication*)app)->mSketchUpBrowser),SIGNAL(readyReadStandardOutput()),(RCApplication*)app,SLOT(onReady3DWarehouseOutput()));
        Program::disconnect((QProcess*)(((RCApplication*)app)->mSketchUpBrowser),SIGNAL(finished(int, QProcess::ExitStatus)),(RCApplication*)app,SLOT(onFinished3DWarehouse(int, QProcess::ExitStatus)));
        lastProcess->kill();
        lastProcess->deleteLater();
    }

    ((RCApplication*)app)->mSketchUpBrowser = new QProcess(app);
#ifdef _MAC
    //QString program = QString(home.getexecutablefilename(_RCT("3D Warehouse.app")));
    QString program = QString(home.getexecutablefilename(_RCT("3D Warehouse")));
#else
    QString program = QString(home.getexecutablefilename(_RCT("3D Warehouse.exe")));
#endif
    Program::connect((QProcess*)(((RCApplication*)app)->mSketchUpBrowser),SIGNAL(readyReadStandardOutput()),(RCApplication*)app,SLOT(onReady3DWarehouseOutput()));
    Program::connect((QProcess*)(((RCApplication*)app)->mSketchUpBrowser),SIGNAL(finished(int, QProcess::ExitStatus)),(RCApplication*)app,SLOT(onFinished3DWarehouse(int, QProcess::ExitStatus)));


    QStringList arguments;
    arguments << "-style" << "fusion";
    if(download_directory.length() > 0)
        arguments << "-downloadpath" << download_directory;

    ((RCApplication*)app)->mSketchUpBrowser->start(program,arguments);

    *cstate = ExitState;
}

void file_command1(int *cstate,HighLevelEvent *,void **)
{
  ResourceString rs2(NCMAIN+2);
  ResourceString rs3(NCMAIN+3);
  *cstate = ExitState;
  if (! filechanged(rs2.gets(),rs3.gets())) return;
  program->setbusy(1);

  db.initialize();

  // initialise the splitter
  /*
  if(cadwindow)
  {
      ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget())->initialise();
      ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget())->addView((RCView*)cadwindow->getcurrentwindow()->gethwnd());
  }
  */

  program->loaddefaultapps();
  program->filenewapps();

  program->setbusy(0);
}

void file_newfromtemplate(int *cstate,HighLevelEvent *,void **)
{ResourceString rs2(NCMAIN+2);
 ResourceString rs3(NCMAIN+3);
 ResourceString rsnc3(NC+3);
 RCCHAR oldfilename[600],title[600];

  *cstate = ExitState;
  if (! filechanged(rs2.gets(),rs3.gets())) return;
  db.initialize();

  strcpy(oldfilename,v.getstring("fi::lastfilename"));

  // v8 use default path
  QString fullPath(v.getstring("db::template-drawings-path"));
  QDir path(fullPath);
  // if empty use the default
  qDebug() << "default templates path : " << QString(home.getpublicfilename("Template-Drawings"));
  if(fullPath.isEmpty())
      fullPath = QString(home.getpublicfilename("Template-Drawings"));
  // see if it exists
  if(!fullPath.isEmpty())
  {
      if(path.exists())
          _chdir(fullPath.data());
      else // last resort use the hard coded path
          _chdir(home.getpublicfilename("Template-Drawings"));
  }
  // v8

  //qDebug() << "template Folder : " << QString(home.getpublicfilename("Template-Drawings"));

  QString lastPath(v.getstring("db::last-cad-file-path"));
  v.setstring("db::last-cad-file-path",fullPath.data());
  file_command2(cstate,0,0);
  v.setstring("db::last-cad-file-path",lastPath.data());
  WritePrivateProfileString(_RCT("Strings"),_RCT("db::last-cad-file-path"),
                            lastPath.data(),home.getinifilename());

  db.setname(rsnc3.gets());
  db.settitle(rsnc3.gets());
  cadwindow->updatetitle();
  db.setchanged();  //  Make sure user is asked to save

  //  Restore the old filename - don't want users to open templates directly
  v.setstring("fi::lastfilename",oldfilename);
  WritePrivateProfileString(_RCT("Strings"),_RCT("fi::lastfilename"),oldfilename,home.getinifilename());
  GetFileTitle(oldfilename,title,600);
  if (strlen(oldfilename) > strlen(title))
    {  oldfilename[strlen(oldfilename)-strlen(title)-1] = 0;
       _chdir(oldfilename);
    }
  else
    _chdir(_RCT("c:\\"));

  program->loaddefaultapps();
  program->filenewapps();

  *cstate = ExitState;

}

#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD)
UINT APIENTRY PreviewHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{static RCCHAR lastfilename[600],filename[600];
 CadFile infile;
 Entity *e;
 Point pmin,pmax,pmin1,pmax1;
 long c;
 double du,dv,aspect;
#if ! defined(_WIN32_WCE)

  if (uiMsg == 1125)
    return 0;

  // KMJ fix this
  //CommDlg_OpenSave_GetFilePath(GetParent(hdlg),filename,300);

  if (uiMsg == WM_INITDIALOG)
    {  CheckDlgButton(hdlg,2001,GetPrivateProfileInt("FilePreview","Enabled",1,home.getinifilename()) ? BST_CHECKED : BST_UNCHECKED);
       strcpy(lastfilename,filename);
    }
  else if (uiMsg == WM_DESTROY)
    {  if (IsWindow(GetDlgItem(hdlg,2001)))
         {  if (IsDlgButtonChecked(hdlg,2001) == BST_CHECKED)
              WritePrivateProfileString("FilePreview","Enabled","1",home.getinifilename());
            else
              WritePrivateProfileString("FilePreview","Enabled","0",home.getinifilename());
         }
    }
  else if (strcmp(filename,lastfilename) != 0)
    {  InvalidateRect(GetDlgItem(hdlg,2000),0,TRUE);
       strcpy(lastfilename,filename);
    }

  if (uiMsg == WM_DRAWITEM)
    {DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT *) lParam;
     ResourceString rs0(NCVIEWA);
     View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
     RECT rect;
     RCHPEN hpen,oldhpen;
     EntityHeader header(0,0,0,0);
     Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
     BitMask options(32);
     HDC hdc;
     Values values(1);

       if (uiMsg == WM_DRAWITEM)
         hdc = dis->hDC;
       else
         hdc = 0;

       View3dOutput output(GetDlgItem(hdlg,2000),hdc,&top,1.0);
       output.setplotscale(values.getreal("vw::plotscale"));

       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
            IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
            rect.left = 0;  rect.top = 0;
            rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
            FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
            if (IsDlgButtonChecked(hdlg,2001) == BST_CHECKED && strlen(filename) > 0 && infile.open(filename,"rb"))
              {  SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
                 //  Start off with a plot scale of 1.0
                 output.setplotscale(1.0);
                 if (infile.readheader())
                   {  c = 0;
                      while ((e = infile.nextentity(0,&values)) != NULL)
                        {  if (e->getvisible()->test(0))
                             {  if (c == 0)
                                  e->extents(&output,&pmin,&pmax);
                                else
                                  {  e->extents(&output,&pmin1,&pmax1);
                                     if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                                     if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                                     if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                                     if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                                     if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                                     if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                                  }
                                 c++;
                             }
                           delete e;
                        }
                      du = fabs(pmax.x - pmin.x) / 2.0;  dv = fabs(pmax.y - pmin.y) / 2.0;
                      if (du > db.getptoler() || dv > db.getptoler())
                        {  aspect = output.getwidthmm() / output.getheightmm();
                           if (du / dv > aspect) dv = du / aspect;
                           if (du / dv < aspect) du = dv * aspect;
                           pmin.x = (pmin.x + pmax.x) / 2.0 - du;
                           pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
                           pmax.x = (pmin.x + pmax.x) / 2.0 + du;
                           pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
                           output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);
                        }
                   }
                 infile.close();
                 output.setplotscale(values.getreal("vw::plotscale"));
                 output.updatematrix();
                 if (infile.open(lastfilename,"rb"))
                   {  if (infile.readheader())
                        {  while ((e = infile.nextentity(NULL)) != NULL)
                             {  if (e->getvisible()->test(0))
                                  e->draw(DM_NORMAL,NULL,&output);
                                delete e;
                             }
                        }
                      infile.close();
                   }
              }
            else
              {  MoveToEx(output.gethdc(),0,rect.bottom-1,0);
                 LineTo(output.gethdc(),rect.right-1,0);
                 MoveToEx(output.gethdc(),0,0,0);
                 LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
                 SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
              }
         }
        return 1;
    }
#endif
  return 0;
}

LPWORD lpwAlign ( LPWORD lpIn)
{ULONG ul;
  ul = (ULONG) lpIn;
  ul +=3;
  ul >>=2;
  ul <<=2;
  return (LPWORD) ul;
}

int nCopyAnsiToWideChar (LPWORD lpWCStr, RCCHAR* lpAnsiIn)
{int cchAnsi = strlen(lpAnsiIn);
    return 0;
  //return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, lpAnsiIn, cchAnsi, (wchar_t *)lpWCStr, cchAnsi) + 1;
}
#endif

void open_stl(RCCHAR *filename)
{
 FILE *infile;
 double nx,ny,nz,x1,y1,z1,x2,y2,z2,x3,y3,z3;
 int nvertices,nindex,pass;
 Point *vertex;
 long *index;
 char line[300];

    if ((infile = fopen(QString(filename).toLatin1(),"r")) != 0)
    {   
        for (pass = 0 ; pass < 2 ; pass++)
        {
            fseek(infile,0l,SEEK_SET);
            nvertices = 0;  
            nindex = 0;


            int cnt=0;
            if (fgets(line,300,infile) != 0 && strncmp(line,"solid",5) == 0)
            {
                //fgets(line,300,infile);
                while ((cnt=fscanf(infile," facet normal %lf %lf %lf\n",&nx,&ny,&nz)) == 3)
                {   if (fscanf(infile," outer loop\n") == 0 &&
                        fscanf(infile," vertex %lf %lf %lf\n",&x1,&y1,&z1) == 3 &&
                        fscanf(infile," vertex %lf %lf %lf\n",&x2,&y2,&z2) == 3 &&
                        fscanf(infile," vertex %lf %lf %lf\n",&x3,&y3,&z3) == 3)
                    {   if (pass == 0)
                        {   nvertices += 3;
                            nindex += 3;
                        }
                        else if (pass == 1)
                        {   vertex[nvertices].x = x1;
                            vertex[nvertices].y = y1;
                            vertex[nvertices].z = z1;
                            vertex[nvertices+1].x = x2;
                            vertex[nvertices+1].y = y2;
                            vertex[nvertices+1].z = z2;
                            vertex[nvertices+2].x = x3;
                            vertex[nvertices+2].y = y3;
                            vertex[nvertices+2].z = z3;
                            index[nindex] = nvertices;
                            index[nindex+1] = nvertices+1;
                            index[nindex+2] = nvertices+2;
                            nvertices += 3;
                            nindex += 3;
                        }
                    }
                    if (fscanf(infile," endloop\n") != 0)
                        break;
                    if (fscanf(infile," endfacet\n") != 0)
                        break;
                }
                if (fscanf(infile," endsolid\n") == 0)
                {
                }
            } 

            if (pass == 0)
            {
                vertex = new Point[nvertices];
                index = new long[nindex];
            }
            else
            {
                db.geometry.add(new Patch(nvertices,vertex,nindex,index,BitMask(32)));
            }
        }
        fclose(infile);
    }

}

extern void open_dwg(RCCHAR *filename);
extern void open_dxf(RCCHAR *filename);

#if 0
// this now uses the new dxf import code
// defined in ncdxfin.cpp
void open_dxf(RCCHAR *filename)
{
    DXFInConvert dxffile;
    if (dxffile.open(filename))
    {   dxffile.loadcolourtable();
        dxffile.read();
        dxffile.close();
    }
    cadwindow->clearAllViews();
}
#endif

void open_iges(RCCHAR *filename)
{IGESIn iges_in;
  if (iges_in.open(filename))
    iges_in.read(1);
}

/**
 * @brief file_command2
 * @param cstate
 *
 * Entry point for the Open file command
 *
 */
void file_command2(int *cstate,HighLevelEvent *,void **)
{RCOPENFILENAME ofn;
    RCCHAR szDirName[256],szFile[256], szFileTitle[256],title[256],szFilter[256],szDefExt[256];
 RCUINT  i;
 RCWORD  *p, *dlgtemplate;
 int   nchar;
 RCDWORD lStyle;

#if defined(WIN32)
 StorageFile sinfile;
#endif
 CadFile oinfile;
 CadFile *infile;
 ResourceString rs4(NCMAIN+4);
 ResourceString rs5(NCMAIN+5);
 ResourceString rsnc1(NC+4);
 ResourceString rsnc2(NC+2);

    *cstate = ExitState;
    if (! filechanged(rs4.gets(),rs5.gets())) 
        return;

    // get the last file path
    strcpy(szDirName,v.getstring("db::last-cad-file-path"));
    if(strlen(szDirName) == 0)
        strcpy(szDirName,v.getstring("db::my-drawings-path"));
    if(strlen(szDirName) == 0)
        strcpy(szDirName,QDir::homePath());
    else if(!QFileInfo::exists(QString(szDirName)))
        strcpy(szDirName,QDir::homePath());
    if(strlen(szDirName) == 0)
        _getcwd(szDirName, sizeof(szDirName));

    szFile[0] = '\0';
    _tcscpy(szFilter,rsnc1.getws());
    //_tcscpy(szFilter,_RCT("CAD files (*.cad *.CAD);;STL files (*.stl);;DWG files (*.dwg);;DXF files (*.dxf);;IGES files (*.igs)"));
    //for (i = 0; szFilter[i] != '\0'; i++){
    //    if (szFilter[i] == '|') szFilter[i] = '\0';
    //}
    strcpy(szDefExt,"CAD");

// KMJ : currently using the Windows ofn structure to pass data to the Qt file open
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = cadwindow->gethwnd();
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile= szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt =  szDefExt;//rsnc2.getws();
    ofn.lpstrFileTitle = title;

#ifndef _USING_QT
#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
    if ( LOBYTE(LOWORD(GetVersion())) > 3)
    {   ofn.lpfnHook = (RCLPOFNHOOKPROC)PreviewHookProc;
        ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
    }
    else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    }

    /* allocate some memory to play with  */
    dlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

    /* start to fill in the dlgtemplate information.  addressing by WORDs */
    lStyle = WS_CHILD | WS_VISIBLE | DS_3DLOOK | DS_CONTROL | WS_CLIPSIBLINGS | DS_SETFONT;
    *p++ = LOWORD (lStyle);
    *p++ = HIWORD (lStyle);
    *p++ = 0;          // LOWORD (lExtendedStyle)
    *p++ = 0;          // HIWORD (lExtendedStyle)
    *p++ = 2;          // NumberOfItems
    *p++ = 0;          // x
    *p++ = 0;          // y
    *p++ = 180;        // cx
    *p++ = 80;         // cy
    *p++ = 0;          // Menu
    *p++ = 0;          // Class

    /* copy the title of the dialog */
    nchar = nCopyAnsiToWideChar (p, TEXT("Preview Template"));
    p += nchar;

    /* Font information because of DS_SETFONT */
    *p++ = 8;     // point size
    nchar = nCopyAnsiToWideChar (p, TEXT("MS Sans Serif"));  // Face name
    p += nchar;

    /* make sure the first item starts on a DWORD boundary */
    p = lpwAlign (p);

    /* now start with the first item */
    lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW;
    *p++ = LOWORD (lStyle);
    *p++ = HIWORD (lStyle);
    *p++ = 0;          // LOWORD (lExtendedStyle)
    *p++ = 0;          // HIWORD (lExtendedStyle)
    *p++ = 5;          // x
    *p++ = 5;          // y
    *p++ = 100;        // cx
    *p++ = 70;         // cy
    *p++ = 2000;       // ID

    /* fill in class i.d., this time by name */
    nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
    p += nchar;

    /* copy the text of the first item */
    nchar = nCopyAnsiToWideChar (p, TEXT("OK"));
    p += nchar;

    *p++ = 0;  // advance pointer over nExtraStuff WORD

    /* now the next item */
    lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | WS_TABSTOP;
    *p++ = LOWORD (lStyle);
    *p++ = HIWORD (lStyle);
    *p++ = 0;          // LOWORD (lExtendedStyle)
    *p++ = 0;          // HIWORD (lExtendedStyle)
    *p++ = 110;        // x
    *p++ = 5;          // y
    *p++ = 100;        // cx
    *p++ = 70;         // cy
    *p++ = 2001;       // ID

    /* fill in class i.d., this time by name */
    nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
    p += nchar;

    /* copy the text of the first item */
    nchar = nCopyAnsiToWideChar (p, TEXT("Show preview"));
    p += nchar;

    *p = 0;  // advance pointer over nExtraStuff WORD

    ofn.hInstance = (RCHINSTANCE)dlgtemplate;
    //  LocalFree (LocalHandle (pdlgtemplate));

#else
    {   ofn.lpfnHook = 0;
        ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    }
#endif
#endif // end of custom open file dialog

    if (GetOpenFileName(&ofn))
    {
        RCCHAR fn[600];
        strcpy(fn,ofn.lpstrFile);
        _strlwr(fn);
        if (strstr(fn,".stl") != 0)
            open_stl(ofn.lpstrFile);
        else if (strstr(fn,".dwg") != 0)
            open_dwg(ofn.lpstrFile);
        else if (strstr(fn,".dxf") != 0)
            open_dxf(ofn.lpstrFile);
        else if (strstr(fn,".igs") != 0)
            open_iges(ofn.lpstrFile);
        else
        {
#if defined(WIN32) && ! defined(_WIN32_WCE)
         RCCHAR fname[256],*ws;
         const RCCHAR *s;
            s = ofn.lpstrFile;  ws = fname;
            while ((*ws++ = *s++) != 0) {}
            if (StgIsStorageFile(fname) == S_OK)
                infile = & sinfile;
            else
#endif
                infile = & oinfile;
            if (infile->open(ofn.lpstrFile,"rb"))
            {
                program->setbusy(1);
                db.clear();
                db.settitle(title);
                cadwindow->updatetitle();
                db.setname(ofn.lpstrFile);
                if (db.load(infile))
                    program->fileopenapps();
                else
                    db.initialize();
                infile->close();
                cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
                // clear all pending events
                app->processEvents();
                // repaint all views
                cadwindow->paintall();
                program->setbusy(0);
            }
        }
        QString path = QFileInfo(QString(ofn.lpstrFile)).path();
        v.setstring("db::last-cad-file-path",path.data());
        WritePrivateProfileString(_RCT("Strings"),_RCT("db::last-cad-file-path"),path.data(),home.getinifilename());
    }
}

void file_command3(int *cstate,HighLevelEvent *,void **)
{ResourceString rsnc3(NC+3);
  if (strcmp(db.getname(),rsnc3.gets()) == 0)
    filesaveas();
  else
    filesave();
  *cstate = ExitState;
}

/**
 * @brief file_command20
 * @param cstate
 *
 *  Entry point for the save options command
 *
 */
void file_command20(int *cstate,HighLevelEvent *,void **)
{
  ResourceString rs27(NCMAINB+27),rs28(NCMAINB+28);
  program->setbusy(1);
  v.save();
  program->savedefaultapps();
  program->setbusy(0);
  cadwindow->MessageBox(rs27.gets(),rs28.gets(),MB_OK);
  *cstate = ExitState;
}

void file_command4(int *cstate,HighLevelEvent *,void **)
{ filesaveas();
  *cstate = ExitState;
}



void file_command7(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs6(NCMAIN+6);
    ResourceString rs10(NCMAIN+10);
    *cstate = ExitState;
    if (! filechanged(rs6.gets(),rs10.gets()))
        return;
    PostQuitMessage(0);
}

// these are the commands to open the files listed in the recent file list
void file_command8(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
  *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(0);
}

void file_command9(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(1);
}

void file_command10(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(2);
}

void file_command11(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(3);
}

void file_command12(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(4);
}

void file_command13(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(5);
}

void file_command14(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(6);
}

void file_command15(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(7);
}

void file_command16(int *cstate,HighLevelEvent *,void **)
{ResourceString rs11(NCMAIN+11);
 ResourceString rs9(NCMAIN+9);
 *cstate = ExitState;
  if (! filechanged(rs11.gets(),rs9.gets())) return;
  cadwindow->openlist.opendatabase(8);
}

void password_command(int *cstate,HighLevelEvent *,void **)
{//char message[256];
  *cstate = ExitState;
//  GetProfileString("CCIC-ASI","Port","",message,256);
//  if (strcmp(message,"AUS291194") != 0)
//    cadwindow->MessageBox("This is an unregistered copy of the software\nPlease install a registered copy.","Unregistered",MB_ICONINFORMATION);

#if 0
 Dialog dialog("Password_dialog");
 char password[20];
  strcpy(password,"");
  dialog.add(new StringDialogControl(100,password,19));
  if (dialog.process(cadwindow->gethwnd()) == TRUE)
    {  if (strcmp(password,"FromAus") == 0)
         return;
    }
  exit(1);
#endif

}

void RTColourScrollBarDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int npos)
{int pos,start,end;
 double colour;
#if ! defined(_WIN32_WCE)  
  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
  colour = 1.0 - double(GetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL) - start) / double(end - start);
//WCEFIX
#endif
  switch (scrollcode)
    {  case SB_TOP : colour = 1.0;  break;
       case SB_BOTTOM : colour = 0.0;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : colour -= 0.01;  break;
       case SB_LINEUP : colour += 0.01;  break;
       case SB_PAGEDOWN : colour -= 0.01;  break;
       case SB_PAGEUP : colour += 0.01;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         colour = 1.0 - double(npos - start) / double(end - start);
         break;
       case 10 :
         colour = npos;
         break;
    }
  if (colour > 1.0) colour = 1.0;
  if (colour < 0) colour = 0;
  pos = start + int((end - start) * (1.0 - colour) + 0.5);
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);
  ((RealDialogControl *)dialog->getcontrol(dc->getid()))->change(dialog,colour);
  dialog->updatecustomcontrols();
}

void RTColourScrollBarDialogControl::setscrollpos(Dialog *dialog,double colour)
{int pos,start,end;
#if ! defined(_WIN32_WCE)  
  GetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,&start,&end);
  pos = int(start + (end - start) * (1.0 - colour) + 0.5);
  SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);
//WCEFIX
#endif
}

void RTColourDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 double red,green,blue;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
  if (dialog->currentvalue(101,&red) &&
      dialog->currentvalue(102,&green) &&
      dialog->currentvalue(103,&blue))
    {  ((RTColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,red);
       ((RTColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,green);
       ((RTColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,blue);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       brush = CreateSolidBrush(RGB(red*255.5,green*255.5,blue*255.5));
       FillRect(output.gethdc(),&rect,brush);
       DeleteObject(brush);
       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
#if ! defined(_WIN32_WCE)  
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
//WCEFIX
#endif
            SelectObject(output.gethdc(),oldhpen);
            DeleteObject(hpen);
         }
    }
}

class SystemOptionsBackColourButtonDialogControl : public ButtonDialogControl
{
 protected:
   int baseid;
 public:
   SystemOptionsBackColourButtonDialogControl(int i,int bid) : ButtonDialogControl(i) {  baseid = bid;  }
   int select(Dialog *);
};

int SystemOptionsBackColourButtonDialogControl::select(Dialog *parentdialog)
{
    int state;
    double red,green,blue;
    if (parentdialog->currentvalue(baseid,&red) &&
        parentdialog->currentvalue(baseid+1,&green) &&
        parentdialog->currentvalue(baseid+2,&blue))
    {
        background_colour_command(&state,0,0);

        red   = ::v.getreal("wn::backgroundcolour.red");
        green = ::v.getreal("wn::backgroundcolour.green");
        blue  = ::v.getreal("wn::backgroundcolour.blue");

        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,red);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
    }
    return 0;
}

class SystemOptionsSelectColourButtonDialogControl : public ButtonDialogControl
{
 protected:
   int baseid;
 public:
   SystemOptionsSelectColourButtonDialogControl(int i,int bid) : ButtonDialogControl(i) {  baseid = bid;  }
   int select(Dialog *);
};

int SystemOptionsSelectColourButtonDialogControl::select(Dialog *parentdialog)
{
    int state;
    double red,green,blue;
    if (parentdialog->currentvalue(baseid,&red) &&
        parentdialog->currentvalue(baseid+1,&green) &&
        parentdialog->currentvalue(baseid+2,&blue))
    {
        select_colour_command(&state,0,0);

        db.setSelectColour(red,green,blue);

        red   = ::v.getreal("db::selectcolour.red");
        green = ::v.getreal("db::selectcolour.green");
        blue  = ::v.getreal("db::selectcolour.blue");

        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,red);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
    }
    return 0;
}

int SelectColourButtonDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    //index = db.header.getcolour();
    index = -1;
    ((RealDialogControl*)parentdialog->getcontrol(baseid))->currentvalue(parentdialog,&red);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->currentvalue(parentdialog,&green);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->currentvalue(parentdialog,&blue);

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    if(strlen(title) < 1)
        dialog.title(app->tr("Define Colour").data());
    else
        dialog.title(title);

    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = red;
    lastGreen = green;
    lastBlue  = blue;

    int status=0;
    if((status=dialog.process()) == TRUE)
    {
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,red);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
        status=1;
    }
    else
    {
        // reset the state
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,lastRed) ;
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,lastGreen);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,lastBlue);
        status=0;
    }

    db.setnumericdisplaytolerance(n);

    return 0;

#if 0
 Dialog dialog("Selectcolour_dialog");
 DialogControl *dc;
 double red,green,blue;
  if (parentdialog->currentvalue(baseid,&red) &&
      parentdialog->currentvalue(baseid+1,&green) &&
      parentdialog->currentvalue(baseid+2,&blue))
    {  dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new RTColourScrollBarDialogControl(104,dc));
       dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(new RTColourScrollBarDialogControl(105,dc));
       dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(new RTColourScrollBarDialogControl(106,dc));
       dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,parentdialog));
       if (dialog.process(parentdialog->gethdlg()) == TRUE)
         {  ((RealDialogControl *) parentdialog->getcontrol(baseid))->change(parentdialog,red);
            ((RealDialogControl *) parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
            ((RealDialogControl *) parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
         }
    }
  return 0;
#endif
}

int SysPathButtonDialogControl::select(Dialog *dlg)
{
    // get the default paths from the ini file
    // put them in a list
    // set them back to the ini file on OK

    Dialog dialog("SystemPath_Dialog");




    if(dialog.process() == TRUE)
    {
        // set the path back to the ini file
    };


    return 0;
}

//extern char *llabel;
//extern RCCHAR *llabel;//fg

void system_command1(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("SystemOptions_Dialog");
 int incrementalcoordinates,device,zoomStep,scrollbars,sbars,dynamiccursor,lastautosaveenabled,autosaveenabled,enableautoplotscale,oldenableautoplotscale,numericdisplaytolerance,hidemeasuredialog,units,disablefastrepaint,usewindowscursors,showhandleswhenselected,versions,distancetracking,showstartupdialog,dialogboxautook,autoviewrefresh,bitmapsforcomposedviews;
 double ptoler,stoler,dtoler,ttoler,red,green,blue,r,g,b,autosavetimeout,lastautosavetimeout,xoffset,yoffset,autoplotscale,oldautoplotscale,minimumlineweight,aspectadjustment;
 double autosnapangle;
 double selectred,selectgreen,selectblue;
 int refDelay,savePreview;
 RCCHAR label1[300],label2[300],text[300];
 //ResourceString rs12(NCMAIN+12);// obsolete
 //ResourceString rs13(NCMAIN+13);// obsolete
 //ResourceString rs14(NCMAINB+3);// obsolete
 //ResourceString rs15(NCMAINB+4);// obsolete
 ResourceString rs14(5188);
 ResourceString rs15(5189);
 ResourceString rs16(5190);
 DialogControl *dc;
 int mousewheeldirection,oldmousewheeldirection;
 int antialiasing, oldantialiasing;
 int useScheme,lastMenuSize,useMenuSize;
 int reduceDetail;

#ifdef _USING_QTCHAR
 //RCCHAR *inputdevices[] = {  rs12.getws(),rs13.getws()  }; // obsolete
 //RCCHAR *dragdetails[] = {  rs14.getws(),rs15.getws()  }; // obsolete
 RCCHAR *zoomSteps[] = { rs14.getws(),rs15.getws(),rs16.getws() };
 RCSTRING CSTRunitsl[] = {  "millimetres", "metres", "inches", "feet and inches","' and \"", "cm"  };
 RCSTRING CSTRautosavel[] = {  "Off", "Save Prompt", "Auto Save"  };
 RCSTRING CSTDefSchemes[] = { "Dark","Light" };
 RCSTRING CSTRefDelay[] = { "None","Shortest","Short","Medium","Long","Longest" };
 RCCHAR *menusizes[] = { rs14.getws(),rs15.getws(),rs16.getws() };
 RCSTRING CSTSavePreview[] = { "No Preview","Screen Shot","Drawn Preview" };

 RCCHAR u1[20],u2[20],u3[20],u4[20],u5[20],u6[20];
 RCCHAR *unitsl[6] = { u1, u2, u3, u4, u5, u6 };
 RCCHAR a1[20],a2[20],a3[20];
 RCCHAR *autosavel[3] = { a1, a2, a3 };
 RCCHAR **colourSchemes=0; // may be more than 2 colour schemes in the future
 RCCHAR dt1[20],dt2[10],dt3[10],dt4[10],dt5[10],dt6[10];
 RCCHAR *refdelays[6] = { dt1, dt2, dt3, dt4, dt5, dt6  };
 RCCHAR sp1[12], sp2[22], sp3[27];
 RCCHAR *savepreviews[3] = { sp1, sp2, sp3  };

 for(int i=0;i<6;i++)
     RCSTRCPY(refdelays[i],&CSTRefDelay[i]);

 for(int i=0;i<6;i++)
	 RCSTRCPY(unitsl[i],&CSTRunitsl[i]);

 for(int i=0;i<3;i++)
	 RCSTRCPY(autosavel[i],&CSTRautosavel[i]);

 for(int i=0;i<3;i++)
     RCSTRCPY(savepreviews[i],&CSTSavePreview[i]);

 // get the list of custom styles
 char key[50];
 long lvalue;
 QStringList tempList;
 for(int i=0; ; i++)
 {
     sprintf(key,"CustomStyle%d",i);
     int cnt = GetPrivateProfileString("GuiStyle",key,"",text,300,home.getinifilename());
     if(cnt == 0)
         break;
     if (strlen(text) != 0)
         tempList.append(QString(text));
 }
 // make a new list with the default styles at the top
 int numSchemes = tempList.length()+2;
 colourSchemes = new RCCHAR*[numSchemes];
 for(int i=0; i<2; i++)
 {
     RCCHAR *csp = new RCCHAR[CSTDefSchemes[i].length()+1];
     RCSTRCPY(csp,&CSTDefSchemes[i]);
     colourSchemes[i] = csp;
 }
 for(int i=2; i<numSchemes; i++)
 {
     RCCHAR *csp = new RCCHAR[tempList.at(i-2).length()+1];
     RCSTRCPY(csp,&tempList.at(i-2));
     colourSchemes[i] = csp;
 }
 // get the last used style
 GetPrivateProfileString("GuiStyle","StyleName","Dark",text,300,home.getinifilename());
 // find it in the list
 for(useScheme=0; useScheme<numSchemes; useScheme++)
 {
    if(RCSTRING(colourSchemes[useScheme]) == RCSTRING(text))
        break;
 }
 // check to make sure we have found it
 // it might not be in the list and it might not be a default
 if(useScheme == numSchemes)
 {
     // did not find it so make a new list and add it on
     RCCHAR **temp = new RCCHAR*[numSchemes++];
     for(int i=0; i<numSchemes-1; i++)
         temp[i] = colourSchemes[i];
     RCCHAR *csp = new RCCHAR[RCSTRING(text).length()+1];
     RCSTRCPY(csp,text);
     temp[useScheme] = csp;
     delete [] colourSchemes;
     colourSchemes = temp;
 }

#else
 char *inputdevices[] = {  rs12.getws(),rs13.getws()  };
 char *dragdetails[] = {  rs14.getws(),rs15.getws()  };
 char *CSTRunitsl[] = {  "millimetres", "metres", "inches", "feet and inches","' and \"", "cm"  };
 char *CSTRautosavel[] = {  "Off", "Save Prompt", "Auto Save"  };
#endif

  device = state.getinputdevice();
  ptoler = db.getptoler();
  stoler = db.getstoler();
  dtoler = db.getdtoler();
  ttoler = db.getttoler();
  minimumlineweight = db.getminimumlineweight();
  numericdisplaytolerance = db.getnumericdisplaytolerance();
  r = red = v.getreal("wn::backgroundcolour.red");
  g = green = v.getreal("wn::backgroundcolour.green");
  b = blue = v.getreal("wn::backgroundcolour.blue");
  selectred = v.getreal("db::selectcolour.red");
  selectgreen = v.getreal("db::selectcolour.green");
  selectblue = v.getreal("db::selectcolour.blue");
  incrementalcoordinates = v.getinteger("db::incrementalcoordinates");
  xoffset = v.getreal("pr::xoffset");
  yoffset = v.getreal("pr::yoffset");
  sbars = scrollbars = v.getinteger("wn::scrollbars");
  dynamiccursor = v.getinteger("wn::dynamiccursor");
  distancetracking = v.getinteger("wn::distancetracking");
  hidemeasuredialog = v.getinteger("db::hidemeasuredialog");
  units = v.getinteger("db::units");
  dialogboxautook = v.getinteger("wn::dialogboxautook");
  autoviewrefresh = v.getinteger("vw::autoviewrefresh");
  enableautoplotscale = oldenableautoplotscale = v.getinteger("vw::enableautoplotscale");
  autoplotscale = oldautoplotscale = v.getreal("vw::autoplotscale");
  showhandleswhenselected = v.getinteger("db::showhandleswhenselected");
  //dragdetail = db.getdragdetail();// obsolete
  zoomStep = v.getinteger("db::zoomstep");
  lastautosavetimeout = autosavetimeout = v.getreal("db::autosavetimeout");
  lastautosaveenabled = autosaveenabled = v.getinteger("db::autosaveenabled");
  disablefastrepaint = v.getinteger("db::disablefastrepaint");
  usewindowscursors = v.getinteger("db::usewindowscursors");
  showstartupdialog = v.getinteger("wn::showstartupdialog");
  aspectadjustment = v.getreal("vw::aspectadjustment");
  bitmapsforcomposedviews = v.getinteger("vw::bitmapsforcomposedviews");
  GetPrivateProfileString("MouseWheel","Direction","1",text,300,home.getinifilename());
  oldmousewheeldirection = atoi(text);
  autosnapangle = v.getreal("ln::autosnapangle");
  useMenuSize = v.getinteger("wn::menusize");
  lastMenuSize = useMenuSize;
  savePreview = v.getinteger("db::previewtype");
  savePreview++;


  switch(v.getinteger("wn::screenrefreshdelay"))
  {
  case 0:
  default:
      refDelay = 0;
      break;
  case 250:
      refDelay = 1;
      break;
  case 500:
      refDelay = 2;
      break;
  case 1000:
      refDelay = 3;
      break;
  case 2000:
      refDelay = 4;
      break;
  case 3000:
      refDelay = 5;
      break;
  }

  reduceDetail = v.getinteger("wn::reducedrawdetail");

  // NOTE: mousewheeldirection 1 = checked = reversed; 0 = not checked = normal
  mousewheeldirection = oldmousewheeldirection ? 0 : 1;
  GetPrivateProfileString("Antialiasing","On","Y",text,300,home.getinifilename());
  oldantialiasing = (RCSTRING(text) == "Y") ? 1 : 0;
  // NOTE: for Qt antialiasing switch
  antialiasing = oldantialiasing;

  versions = v.getinteger("db::versions");
  //dialog.add(new ListDialogControl(100,2,inputdevices,&device)); // obsolete
  dialog.add(dc = new RealDialogControl(101,&ptoler,0.01,1.0E-6,0.1,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&dtoler,1.0,1.0E-10,10.0));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&stoler,0.1,0.1,10.0));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&ttoler,0.01,1.0E-6,0.1,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(new RealDialogControl(109,&autosnapangle));
  dialog.add(new ListDialogControl(105,3,zoomSteps,&zoomStep));
  //dialog.add(new ListDialogControl(105,2,dragdetails,&dragdetail));// obsolete
  //dialog.add(new CheckBoxDialogControl(110,&scrollbars)); // beta pending
  dialog.add(new CheckBoxDialogControl(111,&dynamiccursor));
  dialog.add(new CheckBoxDialogControl(129,&distancetracking));
  //dialog.add(new CheckBoxDialogControl(130,&showstartupdialog));
  dialog.add(dc = new RealDialogControl(106,&red,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&green,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(dc = new RealDialogControl(108,&blue,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(new SystemOptionsBackColourButtonDialogControl(1000,106));
  dialog.add(dc = new RealDialogControl(150,&selectred,0.1,0.0,1.0));
  dialog.add(dc = new RealDialogControl(151,&selectgreen,0.1,0.0,1.0));
  dialog.add(dc = new RealDialogControl(152,&selectblue,0.1,0.0,1.0));
  dialog.add(new SystemOptionsSelectColourButtonDialogControl(1001,150));
  dialog.add(dc = new RealDialogControl(112,&autosavetimeout,1.0,0.0,1000.0));
  //dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(new ListDialogControl(113,3,autosavel,&autosaveenabled));
  dialog.add(new CheckBoxDialogControl(118,&incrementalcoordinates));
  dialog.add(dc = new RealDialogControl(119,&autoplotscale));
  //dialog.add(new ScrollBarDialogControl(1119,dc));
  dialog.add(new CheckBoxDialogControl(120,&enableautoplotscale));
  dialog.add(dc = new IntegerDialogControl(121,&numericdisplaytolerance,0,16));
  //dialog.add(new ScrollBarDialogControl(1121,dc));
  //dialog.add(new CheckBoxDialogControl(122,&hidemeasuredialog)); // beta pending
  dialog.add(new ListDialogControl(123,6,unitsl,&units));
  //dialog.add(new CheckBoxDialogControl(124,&disablefastrepaint)); // beta pending
  //dialog.add(new CheckBoxDialogControl(125,&usewindowscursors)); // beta pending
  dialog.add(new CheckBoxDialogControl(126,&showhandleswhenselected));
  dialog.add(new CheckBoxDialogControl(132,&dialogboxautook));
  dialog.add(new CheckBoxDialogControl(133,&autoviewrefresh));
  //dialog.add(new CheckBoxDialogControl(134,&bitmapsforcomposedviews)); // beta pending
  dialog.add(new CheckBoxDialogControl(135,&mousewheeldirection));
  dialog.add(new CheckBoxDialogControl(136,&antialiasing));
  dialog.add(new CheckBoxDialogControl(138,&reduceDetail));
  dialog.add(new CheckBoxDialogControl(139,&showstartupdialog));
  dialog.add(dc = new IntegerDialogControl(127,&versions,0,20));
  //dialog.add(new ScrollBarDialogControl(1127,dc));
  dialog.add(dc = new RealDialogControl(128,&minimumlineweight,0.1,0.0,2.0));
  //dialog.add(new ScrollBarDialogControl(1128,dc));
  dialog.add(dc = new RealDialogControl(131,&aspectadjustment,0.05,0.1,10.0));
  //dialog.add(new ScrollBarDialogControl(1131,dc));
  dialog.add(new ListDialogControl(137,numSchemes,colourSchemes,&useScheme));
  dialog.add(new ListDialogControl(141,3,menusizes,&useMenuSize));
  dialog.add(new ListDialogControl(142,3,savepreviews,&savePreview));

  dialog.add(new ListDialogControl(110,6,refdelays,&refDelay));
  // v8 system path setup button
  dialog.add(new SysPathButtonDialogControl(143));

  /*
  ::v.setinteger("so::label",0);
  if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0)
  {
       ::v.setinteger("so::label",1);
       //strcpy(label1,"Registration Label    X offset");
       //dialog.add(new StringDialogControl(114,label1,300));
       dialog.add(dc = new RealDialogControl(115,&xoffset,1.0,0.0,40.0,ScaleByLength));
       //strcpy(label2,"Y offset");
       //dialog.add(new StringDialogControl(116,label2,300));
       dialog.add(dc = new RealDialogControl(117,&yoffset,1.0,0.0,40.0,ScaleByLength));
  }
  */

  int status = FALSE;
  if ((status=dialog.process())/* == TRUE*/)
    {  state.setinputdevice(device);
       db.setptoler(ptoler);
       db.setdtoler(dtoler);
       db.setstoler(stoler);
       db.setttoler(ttoler);
       //db.setdragdetail(dragdetail);// obsolete
       db.setzoomstep(zoomStep);// obsolete
       db.setdynamiccursor(dynamiccursor);
       db.setenableautoplotscale(enableautoplotscale);
       db.setautoplotscale(autoplotscale);
       db.setnumericdisplaytolerance(numericdisplaytolerance);
       db.setminimumlineweight(minimumlineweight);
       v.setreal("db::ptoler",ptoler);
       v.setreal("db::minimumlineweight",minimumlineweight);
       v.setreal("db::stoler",stoler);
       v.setreal("db::dtoler",dtoler);
       v.setreal("db::ttoler",ttoler);
       v.setinteger("db::units",units);
       v.setinteger("db::versions",versions);
       db.setunits(units);  
       //v.setinteger("db::dragdetail",dragdetail); // obsolete
       v.setinteger("db::zoomstep",zoomStep);
       v.setinteger("db::hidemeasuredialog",hidemeasuredialog);
       v.setinteger("wn::dynamiccursor",dynamiccursor);
       v.setinteger("wn::distancetracking",distancetracking);
       v.setinteger("wn::showstartupdialog",showstartupdialog);
       WritePrivateProfileString("Integers","wn::showstartupdialog",showstartupdialog ? "1" : "0",home.getinifilename());
       v.setreal("wn::backgroundcolour.red",red);
       v.setreal("wn::backgroundcolour.green",green);
       v.setreal("wn::backgroundcolour.blue",blue);
       v.setreal("db::selectcolour.red",selectred);
       v.setreal("db::selectcolour.green",selectgreen);
       v.setreal("db::selectcolour.blue",selectblue);
       db.setSelectColour(selectred,selectgreen,selectblue);
       v.setinteger("wn::scrollbars",scrollbars);
       v.setreal("db::autosavetimeout",autosavetimeout);
       v.setinteger("db::autosaveenabled",autosaveenabled);
       v.setinteger("db::disablefastrepaint",disablefastrepaint);
       v.setinteger("db::usewindowscursors",usewindowscursors);
       //v.setreal("pr::xoffset",xoffset);
       //v.setreal("pr::yoffset",yoffset);
       v.setinteger("db::incrementalcoordinates",incrementalcoordinates);
       v.setinteger("vw::enableautoplotscale",enableautoplotscale);
       v.setinteger("db::numericdisplaytolerance",numericdisplaytolerance);
       v.setreal("vw::autoplotscale",autoplotscale);
       v.setreal("vw::aspectadjustment",aspectadjustment);
       v.setinteger("db::showhandleswhenselected",showhandleswhenselected);
       v.setinteger("wn::dialogboxautook",dialogboxautook);
       //v.setinteger("vw::autoviewrefresh",autoviewrefresh); // beta pending
       v.setinteger("vw::bitmapsforcomposedviews",bitmapsforcomposedviews);
       // NOTE: mousewheeldirection value : 1 = reversed (checked); 0 = normal (unchecked)
       // ini file MouseWheel Direction value : 1 = normal; 0 = reversed
       WritePrivateProfileString("MouseWheel","Direction",mousewheeldirection ? "0" : "1",home.getinifilename());
       //NOTE: for Qt antialiasing switch : 1 = use AA; 0 = don't use AA
       WritePrivateProfileString("Antialiasing","On",antialiasing ? "Y" : "N",home.getinifilename());
       v.setreal("ln::autosnapangle",autosnapangle);
       v.setinteger("wn::menusize", useMenuSize);
       v.setinteger("db::previewtype",savePreview-1);

       int irefDelay = 0; // milliseconds
       switch(refDelay)
       {
       case 0:
       default:
           irefDelay = 0;
           break;
       case 1:
           irefDelay = 250;
           break;
       case 2:
           irefDelay = 500;
           break;
       case 3:
           irefDelay = 1000;
           break;
       case 4:
           irefDelay = 2000;
           break;
       case 5:
           irefDelay = 3000;
           break;
       }
       v.setinteger("wn::screenrefreshdelay",irefDelay);
       cadwindow->setRefreshDelay(irefDelay);
       v.setinteger("wn::reducedrawdetail",reduceDetail);
       cadwindow->setReduceDetail(reduceDetail);

       if (sbars != scrollbars || fabs(r - red) + fabs(g - green) + fabs(b - blue) > 0.001)
         cadwindow->paintall();
       if (lastautosavetimeout != autosavetimeout || lastautosaveenabled == 0 && autosaveenabled != 0)
         program->setautosavetimer();
       else if (lastautosaveenabled != 0 && autosaveenabled == 0)
         program->stopautosavetimer();

       if (oldenableautoplotscale != enableautoplotscale || oldautoplotscale != autoplotscale)
         window_command5(cstate,0,0);

       // NOTE: check button value : mousewheeldirection = 0 = normal (unchecked); 1 = reversed (checked)
       // window wheeldirection value : 1 = normal; -1 = reversed
       cadwindow->updatemousedirection(mousewheeldirection ? -1 : 1); // 1 = normal; -1 = reversed

       if(cadwindow->coordinateentry)
            cadwindow->coordinateentry->setcsystem(-1);

       cadwindow->setantialiasing(antialiasing);

       program->updatecursortype(program->calcCursorFromBackground());

       WritePrivateProfileString(_RCT("GuiStyle"),_RCT("StyleName"),colourSchemes[useScheme],home.getinifilename());
       RCSTRING styleSheetText;
       if(RCSTRING(colourSchemes[useScheme]) == "Dark")
           if(RCGetHDPIYscale(cadwindow->gethwnd()) == 2)
               styleSheetText = app->getStyleSheetText(RCDarkStyle2x);
           else
               styleSheetText = app->getStyleSheetText(RCDarkStyle);
       else if(RCSTRING(colourSchemes[useScheme]) == "Light")
           if(RCGetHDPIYscale(cadwindow->gethwnd()) == 2)
               styleSheetText = app->getStyleSheetText(RCLightStyle2x);
           else
               styleSheetText = app->getStyleSheetText(RCLightStyle);
       else
           styleSheetText = app->getStyleSheetText(RCSTRING(home.getpublicfilename(colourSchemes[useScheme])));

       // if the style sheet was empty use Dark
       if(styleSheetText.length() == 0)
           if(RCGetHDPIYscale() == 2)
               styleSheetText = app->getStyleSheetText(RCLightStyle2x);
           else
               styleSheetText = app->getStyleSheetText(RCLightStyle);

       app->setStyleSheet(styleSheetText);

       if(lastMenuSize != useMenuSize)
           cadwindow->resetbuttonmenus();
    }
  else
  {
      v.setreal("wn::backgroundcolour.red",red);
      v.setreal("wn::backgroundcolour.green",green);
      v.setreal("wn::backgroundcolour.blue",blue);
      cadwindow->setbackground(RGB(r*255,g*255,b*255));
      cadwindow->paintall();
  }

  // clean up the styles list
  for(int i=0; i<numSchemes; i++)
      delete [] colourSchemes[i];
  delete [] colourSchemes;

  if(status == 140)
  {
      // run the save options command
      state.sendevent(new NewCommandEvent(126,0));
  }
  *cstate = ExitState;
}

void system_command2(int *cstate,HighLevelEvent *,void **)
{ rubberband.off();
  db.setdragdetail(! db.getdragdetail());
  *cstate = ExitState;
}

class FRCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     FRCheckBoxDialogControl(int id,BitMask *o,int b) : CheckBoxDialogControl(id,o,b)  {;}
     void load(Dialog *);
     int select(Dialog *);
  };

void FRCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),201),oldvalue);
}

int FRCheckBoxDialogControl::select(Dialog *dialog)
{int status;
  status = CheckBoxDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),201),oldvalue);
  return status;
}


class FRButtonDialogControl : public ButtonDialogControl
  {public:
     FRButtonDialogControl(int id) : ButtonDialogControl(id)  {};
     int select(Dialog *);
  };


int FRButtonDialogControl::select(Dialog *dialog)
{RCOPENFILENAME ofn;
 RCCHAR dirname[600],filename[600],filetitle[600],filter[300];
 int i;
 WORD  *p, *dlgtemplate;
 int   nchar;
 DWORD lStyle;

  _getcwd(dirname, sizeof(dirname));
  filename[0] = '\0';
  _tcscpy(filter,TEXT("CAD Pattern files (*.cad)"));
  //for (i = 0; filter[i] != '\0'; i++)
  //  if (filter[i] == '|') filter[i] = '\0';
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
  ofn.lpstrTitle = _RCT("Select the compnent file");

#if 0
  #if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
  if ( LOBYTE(LOWORD(GetVersion())) > 3)
    {  ofn.lpfnHook = (RCLPOFNHOOKPROC)PreviewHookProc;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
    }
  else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    }

  /* allocate some memory to play with  */
  dlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

  /* start to fill in the dlgtemplate information.  addressing by WORDs */
  lStyle = WS_CHILD | WS_VISIBLE | DS_3DLOOK | DS_CONTROL | WS_CLIPSIBLINGS | DS_SETFONT;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 2;          // NumberOfItems
  *p++ = 0;          // x
  *p++ = 0;          // y
  *p++ = 180;        // cx
  *p++ = 80;         // cy
  *p++ = 0;          // Menu
  *p++ = 0;          // Class

  /* copy the title of the dialog */
  nchar = nCopyAnsiToWideChar (p, TEXT("Preview Template"));
  p += nchar;

 /* Font information because of DS_SETFONT */
  *p++ = 8;     // point size
  nchar = nCopyAnsiToWideChar (p, _RCT("MS Sans Serif"));  // Face name
  p += nchar;

  /* make sure the first item starts on a DWORD boundary */
  p = lpwAlign (p);

  /* now start with the first item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 5;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2000;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("OK"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  /* now the next item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | WS_TABSTOP;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 110;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2001;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("Show preview"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  ofn.hInstance = (RCHINSTANCE) dlgtemplate;
//  LocalFree (LocalHandle (pdlgtemplate));

#else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    }
#endif
#endif

  if (GetOpenFileName(&ofn))
    ((StringDialogControl *)dialog->getcontrol(100+(id%1000)))->change(dialog,ofn.lpstrFile);

  return 0;
}


/**
 * @brief findandreplace_command
 * @param cstate
 *
 * Entry point for the Find and Replace command
 *
 * KMJ : Working in the Qt version
 *
 */
#ifdef _RCDEBUG
#define TEXT_COUNT 10000
#else
#define TEXT_COUNT 30000
#endif
void findandreplace_command(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("FindAndReplace_Dialog");
 RCCHAR find[600],replace[600],text[TEXT_COUNT],*at,*newat;
 Entity *e;
 BitMask options(32);
 EntityList list;
 AttributeList al;
 int addto,found,i,nfound;
 ResourceString rs6(NCMAINB+6),rs7(NCMAINB+7),rs8(NCMAINB+8);
 Point p(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),pmin,pmax,pmin1,pmax1;
 double du,dv;
 long c;

 strcpy(find,v.getstring("fr::find"));
  strcpy(replace,v.getstring("fr::replace"));
  options = *v.getbm("fr::options");
  dialog.add(new StringDialogControl(100,find,600));
  dialog.add(new FRCheckBoxDialogControl(102,&options,0));
  dialog.add(new StringDialogControl(101,replace,600));
  dialog.add(new CheckBoxDialogControl(103,&options,1));// text
  dialog.add(new CheckBoxDialogControl(104,&options,2));// match case
  dialog.add(new CheckBoxDialogControl(105,&options,3));// entity attributes
  dialog.add(new CheckBoxDialogControl(106,&options,4));// fit window selected entities
  dialog.add(new CheckBoxDialogControl(107,&options,5));// component names
  dialog.add(new CheckBoxDialogControl(108,&options,6));// group class
  dialog.add(new CheckBoxDialogControl(109,&options,7));// group instance
  dialog.add(new CheckBoxDialogControl(110,&options,10));// pattern fill names
  dialog.add(new CheckBoxDialogControl(111,&options,9));// bitmap fill names
  dialog.add(new CheckBoxDialogControl(112,&options,8));// bitmap names
  dialog.add(new CheckBoxDialogControl(113,&options,11));// material texture names
  dialog.add(new CheckBoxDialogControl(114,&options,12));// bumpmap texture names
  dialog.add(new FRButtonDialogControl(1000));
  dialog.add(new FRButtonDialogControl(1001));
  if (dialog.process() == TRUE)
    {  if (! cadwindow->getshiftstate())
         {  addto = 2;  state.destroyselection(1);
         }
       else
         addto = 1;
       program->setbusy(1);
       v.setstring("fr::find",find);
       v.setstring("fr::replace",replace);
       v.setbm("fr::options",options);
       if (! options.test(2))
         _strlwr(find);
       nfound = 0;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         {  found = 0;
            if (options.test(5) && e->isa(figure_entity))
              {BitMask change(32);
                 strncpy(text,((Figure *)e)->getlibrary(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Figure *)e)->getlibrary());
                           strcpy(text+i,replace);
                           strcat(text,((Figure *)e)->getlibrary() + i + strlen(find));
                           change.set(4);
                           ((Figure *)e)->change(change,*e,text,_RCT(""),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,options);
                           nfound++;
                        }
                   }
              }
            if (options.test(1) && e->isa(text_entity))
              {
                BitMask change(32);
                 RCSTRNCPY(text,((Text *)e)->gettext(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Text *)e)->gettext());
                           strcpy(text+i,replace);
                           strcat(text,((Text *)e)->gettext() + i + strlen(find));
                           change.set(31);
                           ((Text *)e)->change(change,*e,_RCT(""),0.0,0.0,0.0,0.0,0,text,0,options);
                           nfound++;
                        }
                   }
              }
            if (options.test(6) && e->isa(group_entity))
              {BitMask change(32);
                 strncpy(text,((Group *)e)->getclassname(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Group *)e)->getclassname());
                           strcpy(text+i,replace);
                           strcat(text,((Group *)e)->getclassname() + i + strlen(find));
                           change.set(4);
                           ((Group *)e)->change(change,*e,text,0,options);
                           nfound++;
                        }
                   }
              }
            if (options.test(7) && e->isa(group_entity))
              {BitMask change(32);
                 strncpy(text,((Group *)e)->getinstance(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Group *)e)->getinstance());
                           strcpy(text+i,replace);
                           strcat(text,((Group *)e)->getinstance() + i + strlen(find));
                           change.set(5);
                           ((Group *)e)->change(change,*e,0,text,options);
                           nfound++;
                        }
                   }
              }
            // bitmap entity file name
            if (options.test(8) && e->isa(bitmap_entity))
              {BitMask change(32);
                 strncpy(text,((BitMapE *)e)->getfilename(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((BitMapE *)e)->getfilename());
                           strcpy(text+i,replace);
                           strcat(text,((BitMapE *)e)->getfilename() + i + strlen(find));
                           change.set(15);
                           change.set(18);
                           ((BitMapE *)e)->change(change,*e,text,0,0,0,0,0,0,0,options);
                           nfound++;
                        }
                   }
              }
            // plane bitmap fill file name
            if (options.test(9) && e->isa(plane_entity) && ((Plane*)e)->getoptions().test(PlaneBitmapFilled))
              {BitMask change(32);
                 strncpy(text,((Plane *)e)->getpatternfilename(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Plane *)e)->getpatternfilename());
                           strcpy(text+i,replace);
                           strcat(text,((Plane *)e)->getpatternfilename() + i + strlen(find));
                           change.set(6);
                           ((Plane *)e)->change(change,*e,0,0,0,0,0,0,0,0,0,0,text,options);
                           nfound++;
                        }
                   }
              }
            // plane pattern fill file name
            if (options.test(10) && e->isa(plane_entity) && ((Plane*)e)->getoptions().test(PlanePatterned))
              {BitMask change(32);
                 strncpy(text,((Plane *)e)->getpatternfilename(),TEXT_COUNT);
                 text[TEXT_COUNT-1] = 0;
                 if (! options.test(2))
                   _strlwr(text);
                 if (strstr(text,find) != 0)
                   {  found = 1;
                      if (options.test(0))
                        {  if (nfound == 0)
                             db.saveundo(UD_STARTBLOCK,NULL);
                           i = strstr(text,find) - text;
                           strcpy(text,((Plane *)e)->getpatternfilename());
                           strcpy(text+i,replace);
                           strcat(text,((Plane *)e)->getpatternfilename() + i + strlen(find));
                           change.set(6);
                           ((Plane *)e)->change(change,*e,0,0,0,0,0,0,0,0,0,0,text,options);
                           nfound++;
                        }
                   }
              }
            BitMask mask;
            mask.clearandset(surface_mask,end_list);
            // texture map file name render attribute
            if (options.test(11) && e->match(mask))
            {
                al = e->getatlist();
                for (al.start() ; (at = al.next()) != 0 ; )
                {
                    strncpy(text,at,TEXT_COUNT);
                    text[TEXT_COUNT-1] = 0;
                    if (strstr(text,"SP::B:") != 0)
                    {
                        if (! options.test(2))
                          _strlwr(text);

                        if (strstr(text,find) != 0)
                          {  found = 1;
                             if (options.test(0) && (newat = new RCCHAR[strlen(text)-strlen(find)+strlen(replace)+1]) != 0)
                               {  if (nfound == 0)
                                    db.saveundo(UD_STARTBLOCK,NULL);
                                  i = strstr(text,find) - text;
                                  strcpy(text,at);
                                  strcpy(text+i,replace);
                                  strcat(text,at + i + strlen(find));
                                  e->delat(at);  e->setattribute(at);
                                  db.saveundo(UD_DELETEATT,e);
                                  delete at;
                                  strcpy(newat,text);
                                  e->addat(newat);  e->setattribute(newat);
                                  db.saveundo(UD_INSERTATT,e);
                                  nfound++;
                               }
                         }
                    }
                }
            }
            // bump map file name render attribute
            if (options.test(12) && e->match(mask))
            {
                al = e->getatlist();
                for (al.start() ; (at = al.next()) != 0 ; )
                {
                    strncpy(text,at,TEXT_COUNT);
                    text[TEXT_COUNT-1] = 0;
                    if (strstr(text,"SP::C:") != 0)
                    {
                        if (! options.test(2))
                          _strlwr(text);

                        if (strstr(text,find) != 0)
                          {  found = 1;
                             if (options.test(0) && (newat = new RCCHAR[strlen(text)-strlen(find)+strlen(replace)+1]) != 0)
                               {  if (nfound == 0)
                                    db.saveundo(UD_STARTBLOCK,NULL);
                                  i = strstr(text,find) - text;
                                  strcpy(text,at);
                                  strcpy(text+i,replace);
                                  strcat(text,at + i + strlen(find));
                                  e->delat(at);  e->setattribute(at);
                                  db.saveundo(UD_DELETEATT,e);
                                  delete at;
                                  strcpy(newat,text);
                                  e->addat(newat);  e->setattribute(newat);
                                  db.saveundo(UD_INSERTATT,e);
                                  nfound++;
                               }
                         }
                    }
                }
            }

            if (options.test(3))
              {  al = e->getatlist();
                 for (al.start() ; (at = al.next()) != 0 ; )
                   {  
                      strncpy(text,at,TEXT_COUNT);
                      text[TEXT_COUNT-1] = 0;
                      if (! options.test(2))
                        _strlwr(text);

                      if (strstr(text,find) != 0)
                        {  found = 1;
                           if (options.test(0) && (newat = new RCCHAR[strlen(text)-strlen(find)+strlen(replace)+1]) != 0)
                             {  if (nfound == 0)
                                  db.saveundo(UD_STARTBLOCK,NULL);
                                i = strstr(text,find) - text;
                                strcpy(text,at);
                                strcpy(text+i,replace);
                                strcat(text,at + i + strlen(find));
                                e->delat(at);  e->setattribute(at);
                                db.saveundo(UD_DELETEATT,e);
                                delete at;
                                strcpy(newat,text);
                                e->addat(newat);  e->setattribute(newat);
                                db.saveundo(UD_INSERTATT,e);
                                nfound++;
                             }
                        }
                   }
              }
            if (found)
              {  EntitySelectedEvent *eevent = new EntitySelectedEvent(e,p,xaxis,yaxis,0,addto,0);
                 state.sendevent(eevent);
                 list.add(e);
              }
         }
       if (options.test(0) && nfound > 0)
         db.saveundo(UD_ENDBLOCK,NULL);
       if (list.empty())
         cadwindow->MessageBox(rs7.gets(),rs6.gets(),MB_ICONINFORMATION);
       else if (options.test(4))
         {  if (cadwindow->getcurrentsurface() == 0)
              cadwindow->MessageBox(rs8.gets(),rs6.gets(),MB_ICONINFORMATION);
            else
              {  for (list.start(),c = 0 ; (e = list.next()) != NULL ; )
                   {  if (e->isvisible(cadwindow->getcurrentsurface()))
                        {  if (c == 0)
                             e->extents(cadwindow->getcurrentsurface(),&pmin,&pmax);
                           else
                             {  e->extents(cadwindow->getcurrentsurface(),&pmin1,&pmax1);
                               if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                               if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                               if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                               if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                               if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                               if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                            }
                          c++;
                       }
                   }
                 if (pmax.x - pmin.x > db.getptoler() && pmax.y - pmin.y > db.getptoler())
                   {  du = (pmax.x - pmin.x) * 0.02;
                      dv = (pmax.y - pmin.y) * 0.02;
                      db.savezoomundo(0,cadwindow->getcurrentwindow());
                      cadwindow->getcurrentwindow()->zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
                   }
              }
         }
       program->setbusy(0);
    }
  *cstate = ExitState;
}


class FFButtonDialogControl : public ButtonDialogControl
  {public:
     FFButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int FFButtonDialogControl::select(Dialog *dialog)
{
 RCCHAR find[600],text[4096],directory[600],filename[600],filepath[600],*at;
 int searchtext,searchattributes,matchcase,found;
#if ! defined(_MSC_VER)
 //int status;
 //struct ffblk ff;
#endif
 BitMask options(32);
 CadFile infile;
 Entity *e;
 AttributeList al;
 ResourceString rs11(NCMAIN+11),rs9(NCMAIN+9);

  if (id == 101 && dialog->currentvalue(100,directory,600))
    {//  Browse button
     RCOPENFILENAME ofn;
     RCCHAR directory[600],filter[300],name[600],title[600];
     RCUINT  i;
       _tcscpy(name,TEXT("*.*"));
       _tcscpy(title,TEXT(""));
       _tcscpy(filter,TEXT("All files (*.*)"));
       //for (i = 0 ; filter[i] != 0 ; i++)
       //  if (filter[i] == '|') filter[i] = '\0';
       strcpy(directory,QDir::fromNativeSeparators(QString(directory)));
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile = name;
       ofn.nMaxFile = sizeof(name);
       ofn.lpstrFileTitle = title;
       ofn.nMaxFileTitle = sizeof(title);
       ofn.lpstrInitialDir = directory;
       ofn.lpstrTitle = _RCT("Select the directory to search");
       //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | ;
       ofn.Flags = QFileDialog::ShowDirsOnly;
       //if (GetOpenFileName(&ofn) && _tcsrchr(name,'\\') != 0)
       if (GetOpenFileName(&ofn) && _tcsrchr(name,'/') != 0)
       {
           // we only get folders so don't need this
            //*_tcsrchr(name,'\\') = 0;
            //*_tcsrchr(name,'/') = 0;
            strcpy(name,QDir::toNativeSeparators(QString(name)));
            ((StringDialogControl *)dialog->getcontrol(100))->change(dialog,name);
       }
       return 0;
    }
  else if (id == 107 && dialog->currentvalue(100,directory,600) && dialog->currentvalue(102,find,600) && dialog->currentvalue(103,&searchtext) && dialog->currentvalue(104,&matchcase) && dialog->currentvalue(105,&searchattributes))
    {//  Find button
       RC_finddata_t file;
       int handle;
       ((ListDialogControl *)dialog->getcontrol(106))->removeallitems(dialog);
       program->setbusy(1);
       v.setstring("ff::find",find);
       strcpy(directory,QDir::fromNativeSeparators(QString(directory)));
       v.setstring("ff::directory",directory);
       options.set(1,searchtext);
       options.set(2,matchcase);
       options.set(3,searchattributes);
       v.setbm("fr::options",options);
       if (! matchcase)
         _strlwr(find);
       strcpy(filename,directory);
       //if (strlen(filename) > 0 && filename[strlen(filename)-1] != '\\')
       //  strcat(filename,"\\*.cad");
       if (strlen(filename) > 0 && filename[strlen(filename)-1] != '/')
         strcat(filename,"/*.cad");
       else
         strcat(filename,"*.cad");

       //qDebug() << "find file name : " << QString(filename);

       file.filter =  QDir::Files;
       handle = _findfirst(filename,&file);
       while (handle != -1)
         {
            strcpy(filename,directory);
            //if (strlen(filename) > 0 && filename[strlen(filename)-1] != '\\')
            //  strcat(filename,"\\");
            if (strlen(filename) > 0 && filename[strlen(filename)-1] != '/')
              strcat(filename,"/");
            strcat(filename,file.name);
            if (infile.open(filename,"rb") != 0)
              {  if (infile.readheader())
                   {  found = 0;
                      while ((e = infile.nextentity(0)) != 0 && ! found)
                        {  if (options.test(1) && e->isa(text_entity))
                             {  RCSTRCPY(text,((Text *)e)->gettext());
                                if (! options.test(2))
                                  _strlwr(text);
                                if (strstr(text,find) != 0)
                                  {  found = 1;
                                  }
                             }
                           if (options.test(3))
                             {  al = e->getatlist();
                                for (al.start() ; (at = al.next()) != 0 ; )
                                  {  strcpy(text,at);
                                     if (! options.test(2))
                                       _strlwr(text);
                                     if (strstr(text,find) != 0)
                                       {  found = 1;
                                       }
                                  }
                             }
                           delete e;
                        }
                      qDebug() << "looking at: " << QString(filename);
                      if (found)
                      {
                          strcpy(filename,QDir::toNativeSeparators(QString(filename)));
                          ((ListDialogControl *)dialog->getcontrol(106))->additem(dialog,filename);
                      }
                   }
                 infile.close();
              }
            if (_findnext(handle,&file) != 0)
            {
                 _findclose(handle);
                 handle = -1;
            }
         }
       program->setbusy(0);
       return 0;
    }
  else if (id == 108)
    {//  Open button
       if (((ListDialogControl *)dialog->getcontrol(106))->currentvalue(dialog,filename,600) && filechanged(rs11.gets(),rs9.gets()))
         {
           strcpy(filepath,QDir::fromNativeSeparators(QString(filename)));
           if (infile.open(filepath,"rb"))
              {  db.clear();
                 //if (strrchr(filename,'\\') != 0)
                 //  db.settitle(strrchr(filename,'\\')+1);
                 if (strrchr(filepath,'/') != 0)
                 {
                     strcpy(filename,QDir::toNativeSeparators(QString(strrchr(filepath,'/')+1)));
                     db.settitle(filename);
                 }
                 else
                 {
                     strcpy(filename,QDir::toNativeSeparators(QString(filepath)));
                     db.settitle(filename);
                 }
                 cadwindow->updatetitle();
                 db.setname(filename);
                 if (db.load(&infile))
                   program->fileopenapps();
                 else
                   db.initialize();
                infile.close();
                cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
                cadwindow->paintall();
              }
            else
              {
                 char message[600];
                 strcpy(filename,QDir::toNativeSeparators(QString(filename)));
                 sprintf(message,"The file '%s' could not be found.",filename);
                 cadwindow->MessageBox(message,"Find in files",MB_ICONINFORMATION);
              }
            return 1;
         }
       else
         return 0;
    }
  return 0;
}

/**
 * @brief findinfiles_command
 * @param cstate
 *
 * Entry point for the Find in Files Command
 *
 *
 */
void findinfiles_command(int *cstate,HighLevelEvent *,void **)
{
 Dialog dialog("FindInFiles_Dialog");
 RCCHAR find[600],directory[600];
 int i;
 BitMask options(32);
 ResourceString rs6(NCMAINB+6),rs7(NCMAINB+7),rs8(NCMAINB+8);
  strcpy(find,v.getstring("ff::find"));
  strcpy(directory,v.getstring("ff::directory"));
  options = *v.getbm("ff::options");
  dialog.add(new StringDialogControl(100,directory,sizeof(directory)));
  dialog.add(new FFButtonDialogControl(101));
  dialog.add(new StringDialogControl(102,find,sizeof(find)));
  dialog.add(new CheckBoxDialogControl(103,&options,1));
  dialog.add(new CheckBoxDialogControl(104,&options,2));
  dialog.add(new CheckBoxDialogControl(105,&options,3));
  dialog.add(new ListDialogControl(106,0,(RCCHAR**)0,&i));
  dialog.add(new FFButtonDialogControl(107));
  dialog.add(new FFButtonDialogControl(108));
  dialog.process();
  *cstate = ExitState;
}

class EIStringDialogControl : public StringDialogControl
  {public:
     EIStringDialogControl(int id,RCCHAR *string,int length) : StringDialogControl(id,string,length) {}
     void load(Dialog *dialog);
  };

void EIStringDialogControl::load(Dialog *dialog)
{RCCHAR string[100];

  StringDialogControl::load(dialog);

  if (currentvalue(dialog,string,100) && (id & 1) == 0)
  {
        qDebug() <<  "string : " << QString(string);
        qDebug() <<  "compare : " << strncmp(string,"LoginTime:",10);

       EnableWindow(GetDlgItem(dialog->gethdlg(),id),strncmp(string,"LoginTime:",10) != 0);
       EnableWindow(GetDlgItem(dialog->gethdlg(),id+1),strncmp(string,"LoginTime:",10) != 0);
  }

}


class EIScrollBarDialogControl : public ScrollBarDialogControl
  {private:
     int *base,scrolling;
     RCCHAR *name,*description;
   public:
     EIScrollBarDialogControl(int id,int *b,RCCHAR *n,RCCHAR *d) :
       ScrollBarDialogControl(id,0) {  scrolling = 0;  base = b;  *base = 0;  name = n;  description = d;  }
     void vscrollevent(Dialog *,int,int,int);
  };

void EIScrollBarDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int i,n,start,end;
  if (scrolling)
    return;
  scrolling = 1;
  for (i = 0 ; i < 16 ; i += 2)
    {  ((StringDialogControl *)dialog->getcontrol(100+i))->currentvalue(dialog,name+(i/2+*base)*100,100);
       ((StringDialogControl *)dialog->getcontrol(100+i+1))->currentvalue(dialog,description+(i/2+*base)*300,300);
    }
  for (i = 0,n = 0 ; i < max_info ; i++)
    if (strlen(name+i*100) > 0)
      n = i + 1;

  GetScrollRange(GetDlgItem(dialog->gethdlg(),1100),SB_CTL,&start,&end);

  switch (scrollcode)
    {  case SB_BOTTOM : *base = n - 1;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : *base = *base + 1;  break;
       case SB_LINEUP : *base = *base - 1;  break;
       case SB_PAGEDOWN : *base = *base + 5;  break;
       case SB_PAGEUP : *base = *base + 5;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         *base = int(double(pos - start) / double(end - start) * (n-1));
         break;
    }
  if (*base > n-1) *base = n-1;
  if (*base > max_info - 8) *base = max_info - 8;
  if (*base < 0) *base = 0;
  if (scrollcode != SB_THUMBPOSITION && scrollcode != SB_THUMBTRACK)
    pos = int(start + (end - start) * double(*base) / double((n > 1 ? n - 1 : 1)));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),1100),SB_CTL,pos,TRUE);
  for (i = 0 ; i < 16 ; i += 2)
    {  ((StringDialogControl *)dialog->getcontrol(100+i))->change(dialog,name+(i/2+*base)*100);
       ((StringDialogControl *)dialog->getcontrol(100+i+1))->change(dialog,description+(i/2+*base)*300);

       EnableWindow(GetDlgItem(dialog->gethdlg(),100+i),strncmp(name+(i/2+*base)*100,"LoginTime:",10) != 0);
       EnableWindow(GetDlgItem(dialog->gethdlg(),100+i+1),strncmp(name+(i/2+*base)*100,"LoginTime:",10) != 0);
    }
  scrolling = 0;
}

/*
 *  declared in ncdialog.h
class EIListDialogControl : public ListDialogControl
{
public:
    RCCHAR *labels;
    RCCHAR *values;
    EIListDialogControl(int id,int num,RCCHAR *l,RCCHAR *v) : ListDialogControl(id,num,&l) { labels = l; values = v; }
    void load(Dialog *);
};
*/

void EIListDialogControl::load(Dialog *dlg)
{
    if(dlg != 0)
    {
        QTableWidget *tableWidget = (QTableWidget*)((RCDialog*)dlg->gethdlg())->getControl(100);
        if(tableWidget != 0)
        {
            tableWidget->setRowCount(n);
            tableWidget->setColumnCount(2);

            RCCHAR *plabels = labels;
            RCCHAR *pvalues = values;
            for(int i=0; i<n; i++)
            {
                QString itemString = QString(plabels);
                QTableWidgetItem *newItem = new QTableWidgetItem(itemString);
                tableWidget->setItem(i, 0, newItem);
                plabels += 100;
                itemString = QString(pvalues);
                newItem = new QTableWidgetItem(itemString);
                tableWidget->setItem(i, 1, newItem);
                pvalues += 300;
            }
        }
    }
}

/**
 * @brief editinfo_command
 * @param cstate
 *
 * Entry point for the edit information command
 *
 *
 */
void editinfo_command(int *cstate,HighLevelEvent *,void **)
{
    static Dialog *dialog=0;//("Information_Dialog");
    RCCHAR n[8][100],d[8][300],n1[100],d1[300];
    RCCHAR name[max_info*100],description[max_info*300];
    int i,j,num,base,status;
    status = v.getfirststring("in::",n1,d1);

    if(*cstate == InitialState)
    {
        dialog = new Dialog("Information_Dialog");
        for (i = 0,num = 0 ; i < max_info ; i++)
        {
            if (status)
            {
                num++;
                strcpy(name+i*100,n1+4);
                strcpy(description+i*300,d1);
                status = v.getnextstring("in::",n1,d1);
            }
            else
            {
                if (num > 0)
                {
                    for (j = 0 ; j < num / 2 ; j++)
                    {  strcpy(n1,name+j*100);
                        strcpy(name+j*100,name+(num-j-1)*100);
                        strcpy(name+(num-j-1)*100,n1);
                        strcpy(d1,description+j*300);
                        strcpy(description+j*300,description+(num-j-1)*300);
                        strcpy(description+(num-j-1)*300,d1);
                    }
                    num = 0;
                }
                strcpy(name+i*100,"");
                strcpy(description+i*300,"");
            }
        }

        dialog->add(new EIListDialogControl(100,max_info,name,description));
        *cstate = 1;
        if (dialog && dialog->process() == TRUE)
        {

            for (i = 0 ; i < max_info ; i++)
                if (strlen(name+i*100) > 0)
                {
                    strcpy(n1,"in::");  strcat(n1,name+i*100);
                    v.setstring(n1,description+i*300);
                }
            db.setchanged();
        }
        delete dialog;
        dialog = 0;
    }
    else if(*cstate == 1)
    {
        if(dialog && dialog->gethdlg())
            ((RCDialog*)dialog->gethdlg())->reject();
    }
    *cstate = ExitState;
}


void OpenPDF(RCCHAR *title)
{
#ifdef USING_WIDGETS
    QString url("file:///");

    url += QString(home.getpublicfilename(""));
    url += "Documents/" + QString(title);

    QDesktopServices::openUrl(QUrl(url,QUrl::TolerantMode));

#else
  RCCHAR key[300],path[300],filename[300];
  RCHKEY hkey;
  RCDWORD length;

  strcpy(key,"");
  strcpy(path,"");
  if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_RCT(".pdf"),0,KEY_READ,&hkey) == ERROR_SUCCESS)
    {  length = 300;
       RegQueryValueEx(hkey,(char*)0,0,NULL,(LPBYTE)key,&length);
    } 

  strcat(key,"\\shell\\open\\command");
  if (RegOpenKeyEx(HKEY_CLASSES_ROOT,key,0,KEY_READ,&hkey) == ERROR_SUCCESS)
    {  length = 300;
       RegQueryValueEx(hkey,(char*)0,0,NULL,(LPBYTE)path,&length);
    } 

  if (strstr(path,"%1") != 0)
    strstr(path,"%1")[1] = 's';
  else
    strcat(path," \"%s\"");

  sprintf(filename,path,home.getexecutablefilename(title));
  WinExec(filename,SW_NORMAL);
#endif
}

void help_command1(int *cstate,HighLevelEvent *,void **)
{ 
#if ! defined(_WIN32_WCE)
  if ((cad_version&15) == -1)
    cadwindow->MessageBox("Help not available in demonstration version.","AUSCAD",MB_ICONINFORMATION);
  else
    program->help(HELP_INDEX,0);
//WCEFIX
#endif
  *cstate = ExitState;
}


typedef int (*Convert)(char*);

void open_skp(RCCHAR *filename)
{
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libRealCADSketchUp.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("RealCADSketchUp.dll")));
#endif

    qInfo() << "filePath" << QString(filename).toLatin1().data();
    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(QString(filename).toLatin1().data()); // SKP import
    }
}

/**
 * @brief file_command18
 * @param cstate
 *
 * Entry point for the SKP import command
 * TODO: move this to a separate file for SKP input output
 *
 *  since version 7.
 */
void skpfilein_command(int *cstate,HighLevelEvent *,void **)
{
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libRealCADSketchUp.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("RealCADSketchUp.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(0); // SKP import
    }

  *cstate = ExitState;
}

