

#include "ncwin.h"

/**
 * @brief shade_command1
 * @param cstate
 *
 * Entry point for the Window Shade command
 *
 */
void shade_command1(int *cstate,HighLevelEvent *,void **)
{
 RCOPENFILENAME ofn;
 Dialog dialog("Shade_Dialog");
 DialogControl *dc;
 double ambient,diffuse;
 int save,hiddenlines,cancelled,openglshading;
 ResourceString rs0(NCSHADEB);
 ResourceString rs1(NCSHADEB+1);
 ResourceString rs2(NCSHADEB+2);
 ResourceString rs3(NCSHADEB+3);
 ResourceString rs4(NCSHADEB+4);

 RCCHAR szDirName[256],szFile[256], szFileTitle[256],szFilter[256],dtitle[256],title[256];
 UINT  i;

 *cstate = ExitState;
 if (cadwindow->getcurrentsurface() == NULL)
    {  cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
       return;
    }
 ambient = v.getreal("sh::ambient");
 diffuse = v.getreal("sh::diffuse");
 save = v.getinteger("sh::save");
 openglshading = v.getinteger("sh::openglshading");
 hiddenlines = v.getinteger("sh::hiddenlines");
 dialog.add(dc = new RealDialogControl(100,&ambient,0.1));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 dialog.add(dc = new RealDialogControl(101,&diffuse,0.1));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(new CheckBoxDialogControl(102,&save));
 dialog.add(new CheckBoxDialogControl(103,&hiddenlines));
 dialog.add(new CheckBoxDialogControl(104,&openglshading));
 if (dialog.process() == TRUE)
   {
      v.setreal("sh::ambient",ambient);
      v.setreal("sh::diffuse",diffuse);
      v.setinteger("sh::save",save);
      v.setinteger("sh::hiddenlines",hiddenlines);
      v.setinteger("sh::openglshading",openglshading);
      if (save)
        {

           _tcscpy(dtitle,rs4.getws());
           _getcwd(szDirName, sizeof(szDirName));
           szFile[0] = '\0';
           _tcscpy(szFilter,rs2.getws());

/*
#if defined(TRICAD)
           strrchr(szFilter,'|')[0] = 0;
           strrchr(szFilter,'|')[0] = 0;
           strrchr(szFilter,'|')[2] = 0;
           strrchr(szFilter,'|')[1] = 0;
           strrchr(szFilter,'|')[0] = 0;
#endif
           for (i = 0; szFilter[i] != '\0'; i++)
             if (szFilter[i] == '|') szFilter[i] = '\0';
*/

           memset(&ofn, 0, sizeof(RCOPENFILENAME));
           ofn.lStructSize = sizeof(RCOPENFILENAME);
           ofn.hwndOwner = cadwindow->gethwnd();
           ofn.lpstrTitle =  dtitle;
           ofn.lpstrFilter =  szFilter;
           ofn.nFilterIndex = 1;
           ofn.lpstrFile=  szFile;
           ofn.nMaxFile = sizeof(szFile);
           ofn.lpstrFileTitle =  szFileTitle;
           ofn.nMaxFileTitle = sizeof(szFileTitle);
           ofn.lpstrInitialDir =  szDirName;
           ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
           ofn.lpstrDefExt =  rs3.getws();
           ofn.lpstrFileTitle =  title;
           cancelled = ! GetSaveFileName(&ofn);
        }
      else
        cancelled = 0;
      if (! cancelled)
      {
          if(openglshading)
          {
           cadwindow->getcurrentsurface()->setrepaintstyle(Shaded);
           db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
           InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
           program->processpaintmessages();
           if (save)
              cadwindow->getcurrentsurface()->saveimage(1+(_tcsstr(ofn.lpstrFile,TEXT(".TGA")) != 0),ofn.lpstrFile,0);
          }
          else
          {
              cadwindow->getcurrentsurface()->setrepaintstyle(Wireframe);
              db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
              InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
              program->processpaintmessages();
          }
      }

      *cstate = ExitState;
   }
}
