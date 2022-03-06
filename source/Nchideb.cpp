
#include "ncwin.h"

// old hidden line display setting
void hide_command1(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCHIDEB),rs1(NCHIDEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {   cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("hd::dashhiddenlines",0);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    cadwindow->getcurrentsurface()->setrepaintstyle(Hidden);
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

// old hidden dashed line display setting
void hide_command2(int *cstate,HighLevelEvent *,void **)
{ResourceString rs0(NCHIDEB),rs1(NCHIDEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {   cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("hd::dashhiddenlines",1);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    cadwindow->getcurrentsurface()->setrepaintstyle(HiddenDashed);
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

void viewstyle_command0(int *cstate,HighLevelEvent *,void **)
{
    cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Viewstyle"));
    *cstate = ExitState;
}

void viewstyle_command1(int *cstate,HighLevelEvent *,void **)
{
    QtEndScene();
    cadwindow->paintall(1);
    *cstate = ExitState;
}

// hidden lines display setting
void viewstyle_command2(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCHIDEB),rs1(NCHIDEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {   cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("hd::dashhiddenlines",0);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    cadwindow->getcurrentsurface()->setrepaintstyle(Hidden);
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

// hidden dashed lines display setting
void viewstyle_command3(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCHIDEB),rs1(NCHIDEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {   cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("hd::dashhiddenlines",1);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    cadwindow->getcurrentsurface()->setrepaintstyle(HiddenDashed);
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

#if 1
// shaded display setting
// no shade setting dialog
void viewstyle_command4(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("sh::hiddenlines",0);
    v.setinteger("hd::dashhiddenlines",0);
    v.setinteger("sh::openglshading",1);
    cadwindow->getcurrentsurface()->setrepaintstyle(Shaded);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;

}
#else
// with shade setting dialog
void viewstyle_command4(int *cstate,HighLevelEvent *,void **)
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
#endif

// shaded with hidden lines display setting
void viewstyle_command5(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("sh::hiddenlines",1);
    v.setinteger("hd::dashhiddenlines",0);
    v.setinteger("sh::openglshading",1);
    cadwindow->getcurrentsurface()->setrepaintstyle(ShadedHidden);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

// shaded with dashed hidden lines display setting
void viewstyle_command6(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("sh::hiddenlines",1);
    v.setinteger("hd::dashhiddenlines",1);
    v.setinteger("sh::openglshading",1);
    cadwindow->getcurrentsurface()->setrepaintstyle(ShadedHiddenDashed);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

// shaded with textures display setting
void viewstyle_command7(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("sh::hiddenlines",0);
    v.setinteger("hd::dashhiddenlines",0);
    v.setinteger("sh::openglshading",1);
    cadwindow->getcurrentsurface()->setrepaintstyle(ShadedTexture);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

// shaded with textures and hidden lines display setting
void viewstyle_command8(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }

    v.setinteger("sh::hiddenlines",1);
    v.setinteger("hd::dashhiddenlines",0);
    v.setinteger("sh::openglshading",1);
    cadwindow->getcurrentsurface()->setrepaintstyle(ShadedHiddenTexture);
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface());
    InvalidateRect(cadwindow->getcurrentsurface()->gethwnd(),0,TRUE);
    program->processpaintmessages();

    *cstate = ExitState;
}

void viewstyle_command9(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;

}

/**
 * @brief viewstyle_command10
 * @param cstate
 *
 * Entry point for the View style view as perspective command
 * this is the same as view_command8()
 *
 */
void viewstyle_command10(int *cstate,HighLevelEvent *,void **)
{
    Dialog *dialog;
    double focaldist;
    Point eye;
    View *view;
    ResourceString rs53(NCVIEWB+53);
    ResourceString rs54(NCVIEWB+54);
    ResourceString rs55(NCVIEWB+55);
    ResourceString rs56(NCVIEWB+56);
    ResourceString rs57(NCVIEWB+57);
    ResourceString rs58(NCVIEWB+58);
    ResourceString rs59(NCVIEWB+59);
    ResourceString rs60(NCVIEWB+60);
    ResourceString rs61(NCVIEWB+61);
     if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
       cadwindow->MessageBox(rs53.gets(),rs54.gets(),MB_ICONINFORMATION);
     else
       {  view = cadwindow->getcurrentsurface()->getview();
          db.savezoomundo(1,cadwindow->getcurrentwindow());
          if (! view->changeview(1))
            cadwindow->MessageBox(rs55.gets(),rs56.gets(),MB_ICONINFORMATION);
          else
            {  focaldist = (view->geteye()-view->getref()).length();
               if ((dialog = new Dialog(rs57.gets(),rs58.gets(),&focaldist,ScaleByLength)) == NULL)
                 state.fail(Memory,rs59.gets());
               else if (dialog->process() == TRUE)
                 {  eye = view->getref() + (view->geteye() - view->getref()).normalize() * focaldist;
                    if (! view->changeview(0,eye))
                      cadwindow->MessageBox(rs60.gets(),rs61.gets(),MB_ICONINFORMATION);
                    else
                      cadwindow->updatewindows(view);
                    delete dialog;
                 }
            }
       }
     *cstate = ExitState;
}

/**
 * @brief viewstyle_command11
 * @param cstate
 *
 * Entry point for View styles view as parallel command
 * this is the same as view_command9
 */
void viewstyle_command11(int *cstate,HighLevelEvent *,void **)
{
    ResourceString rs62(NCVIEWB+62),rs63(NCVIEWB+63),rs64(NCVIEWB+64);
      if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
        cadwindow->MessageBox(rs62.gets(),rs63.gets(),MB_ICONINFORMATION);
      else
        {  db.savezoomundo(1,cadwindow->getcurrentwindow());
           if (! cadwindow->getcurrentsurface()->getview()->changeview(0))
             cadwindow->MessageBox(rs64.gets(),rs63.gets(),MB_ICONINFORMATION);
           else
             cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
        }
      *cstate = ExitState;
}

/**
 * @brief viewstyle_command11
 * @param cstate
 *
 * Entry point for shade options settings
 * this is for OpenGL rendering settings
 */
void viewstyle_command12(int *cstate,HighLevelEvent *,void **)
{
    Dialog dialog("Shade_Dialog");
    double ambient,diffuse;
    ResourceString rs0(NCSHADEB);
    ResourceString rs1(NCSHADEB+1);

    *cstate = ExitState;
    if (cadwindow->getcurrentsurface() == NULL)
    {
        cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
        return;
    }
    ambient = v.getreal("sh::ambient");
    diffuse = v.getreal("sh::diffuse");
    dialog.add(new RealDialogControl(100,&ambient,0.1));
    dialog.add(new RealDialogControl(101,&diffuse,0.1));
    if (dialog.process() == TRUE)
    {
         v.setreal("sh::ambient",ambient);
         v.setreal("sh::diffuse",diffuse);
         *cstate = ExitState;
    }
}
