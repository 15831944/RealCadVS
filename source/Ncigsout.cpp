
#include "ncwin.h"
#if ! defined(_WIN32_WCE)
#include <time.h>
#endif

#if MENUS != STUDENTMENUS

class IGESOut
  {private:
     FILE *outfile;
   public:
     IGESOut();
     ~IGESOut()  {  if (outfile != 0) fclose(outfile);  }
     int open(char *);
     int write(void);
     void close(void);
  };


IGESOut::IGESOut()
{ outfile = 0;
}

int IGESOut::open(char *s)
{ if (s == 0)
    {OPENFILENAME ofn;
     RCCHAR szDirName[256]={0}, szFile[256]={0}, szFileTitle[256]={0}, title[256]={0}, szFilter[256]={0};
     RCCHAR szDefExt[20]={0},szDlgTitle[50]={0};
     UINT  i;
     ResourceString rs2(NCDXF+2);
     ResourceString rs3(NCDXF+3);
     ResourceString rs4(NCDXF+4);
       _getcwd(szDirName, sizeof(szDirName));
       szFile[0] = '\0';
       _tcscpy(szFilter,TEXT("IGES Files (*.igs *.IGS)"));

       //for (i = 0; szFilter[i] != '\0'; i++)
       //  if (szFilter[i] == '|') szFilter[i] = '\0';
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
       strcpy(szDefExt,".igs");
       ofn.lpstrDefExt = szDefExt;
       ofn.lpstrFileTitle = title;       
       strcpy(szDlgTitle,"Export IGES File");
       ofn.lpstrTitle = szDlgTitle;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
       if (GetSaveFileName(&ofn))
         outfile = fopen(ofn.lpstrFile,TEXT("w"));
       else
         outfile = 0;
    }
  else
    outfile = fopen(s,"w");
  return outfile != 0;
}

int IGESOut::write(void)
{time_t ltime;
 struct tm *tm;
 char text[300];
 int nstart,nglobal,nparameter,ndirectory;
 Entity *e;

  if (outfile == 0)
    return 0;

#if ! defined(_WIN32_WCE)
  ltime = time(0L);
  tm = localtime(&ltime);
  strftime(_RCT(text),300,_RCT("%y%m%d.%H%M%M"),tm);
//WCEFIX
#endif

  nstart = 1;
  nglobal = 2;

  if (fprintf(outfile,"                                                                        S      1\n") < 0) return 0;
  if (fprintf(outfile,"1H,,1H;,1H ,4HIGES,7HRealCAD,3H1.0,32,38,7,128,16,1H ,1.0,              G      1\n") < 0) return 0;
  if (fprintf(outfile,"2,2HMM,8,2.0,13H%s,0.001,2.0E+04,1H ,2HAA,6,0;               G      2\n",text) < 0) return 0;

  ndirectory = 0;
  nparameter = 0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->exportiges(outfile,0,0,&ndirectory,&nparameter) == 0)
      return 0;

  ndirectory = 0;
  nparameter = 0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->exportiges(outfile,1,0,&ndirectory,&nparameter) == 0)
      return 0;

  if (fprintf(outfile,"S%7dG%7dD%7dP%7d                                        T      1\n",nstart,nglobal,ndirectory,nparameter) < 0) return 0;

  return 1;

}

void IGESOut::close(void)
{ if (outfile != 0)
    fclose(outfile);
  outfile = 0;
}

void igesout_command(int *cstate,HighLevelEvent *,void **)
{IGESOut igesfile;
  if (cadwindow->getcurrentwindow() == 0)
    cadwindow->MessageBox("Please select a window","Export IGES",MB_ICONINFORMATION);
  else
    if (igesfile.open(0))
      {  if (igesfile.write() == 0)
           cadwindow->MessageBox("An error occured while writing the file - check the disk and disk space.","Export IGES",MB_ICONINFORMATION);
         igesfile.close();
      }
  *cstate = ExitState;
}

void strokedigesout_command(int *cstate,HighLevelEvent *,void **)
{ if (cadwindow->getcurrentwindow() == 0)
    cadwindow->MessageBox("Please select a window","Export Stroked IGES",MB_ICONINFORMATION);
  else
    if (cadwindow->getcurrentwindow()->openiges(0))
      {  db.setdtoler(db.getdtoler()/200.0);
         if (cadwindow->getcurrentwindow()->getoffscreenbitmap() != 0)
           {  cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.clear();  
              cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.seterrorstatus(1);  //  Disable the display list while generating the IGES file
           }
         cadwindow->getcurrentwindow()->paintevent();
         cadwindow->getcurrentwindow()->setfileformat(2);  //  Now write the parameter section
         cadwindow->getcurrentwindow()->paintevent();
         if (cadwindow->getcurrentwindow()->getoffscreenbitmap() != 0)
           cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.seterrorstatus(0);
         cadwindow->getcurrentwindow()->closeiges();
         db.setdtoler(db.getdtoler()*200.0);
      }
  *cstate = ExitState;
}


#endif
