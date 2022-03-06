
#include "ncwin.h"
#include "RCView.h"
#include "RCCentralWidget.h"
#ifndef _USING_QT
#include "sys/types.h"
#include "sys/stat.h"
#endif

void compose_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Layout"));
  *cstate = ExitState;
}


void view_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("View"));
  *cstate = ExitState;
}

void render_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Render"));
  *cstate = ExitState;
}

class ViewSelectListDialogControl : public ListDialogControl
  { public:
     ViewSelectListDialogControl(int id,int n,RCCHAR **list,int *index) :
         ListDialogControl(id,n,list,index)  {}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void ViewSelectListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void ViewSelectListDialogControl::changefocus(Dialog *dialog,int)
{RCCHAR viewname[256];
 View *v;
 Zone *zone;
  if (dialog->currentvalue(100,viewname,256))
  {
       ((ListDialogControl *) dialog->getcontrol(101))->removeallitems(dialog);
       if ((v = db.views3d.match(viewname)) != 0)
       {
           for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
              ((ListDialogControl *) dialog->getcontrol(101))->additem(dialog,zone->getname());
       }
  }
}

// list selection control for the layout SelectView_Dialog
// defined in dialog.h
/*
class SelectViewZoneListDialogControl : public ListDialogControl
{
public:
    int buddyId;
public:
     SelectViewZoneListDialogControl(int id,int bid, int n,RCCHAR **list,RCCHAR *index, int len) :
         ListDialogControl(id,n,list,index,len)  {buddyId = bid;}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     int select(Dialog *);
};
*/

void SelectViewZoneListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void SelectViewZoneListDialogControl::changefocus(Dialog *dialog,int)
{
  RCCHAR viewname[256];
  View *v;
  Zone *zone;

  qDebug() << "in SVZLDC changefocus()";
}

int SelectViewZoneListDialogControl::select(Dialog *)
{
    qDebug() << "in SVZLDC select()";
    return 1;
}

/**
 * @brief window_command1
 * @param cstate
 *
 * Entry point for the Display a 3D view in a new Window Command
 *
 *
 */
void window_command1(int *cstate,HighLevelEvent *,void **)
{View *v;
 int index1,n1;
 RCCHAR **namelist1,*namelist2[1],lastzonename[256];
 Zone *zone;
 ResourceString rs0(NCVIEWB);
 Dialog dialog(_RCT("SelectView_Dialog"));
  index1 = 0;
  namelist2[0] = _RCT("");
  db.views3d.names(&namelist1,&n1,&index1);
  lastzonename[0] = 0;
  if (namelist1 != 0 && n1 > 0)
  {
       dialog.add(new ViewSelectListDialogControl(100,n1,namelist1,&index1));
       dialog.add(new ListDialogControl(101,0,namelist2,lastzonename,256));
       ::v.setstring("op::lastzonename",lastzonename);
       if (dialog.process() == TRUE)
       {
           //qDebug() << QString(namelist1[index1]);
           if ((v = db.views3d.match(namelist1[index1])) != 0)
           {
               strcpy(lastzonename,::v.getstring("op::lastzonename"));
               //qDebug() << QString(v->getname());
                for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
                   if (strcmp(lastzonename,zone->getname()) == 0)
                   {
                        cadwindow->create3dsubwindow(v);
                        if (cadwindow->getcurrentwindow() != 0)
                          cadwindow->getcurrentwindow()->zoom(zone->getumin(),zone->getvmin(),zone->getumax(),zone->getvmax());
                        break;
                   }
                 if (zone == 0)
                   cadwindow->create3dsubwindow(v);
           }
       }
       delete [] namelist1;
  }
  *cstate = ExitState;
}

void window_command11(int *cstate,HighLevelEvent *,void **)
{View *v;
 int index1,n1;
 RCCHAR **namelist1,*namelist2[1],lastzonename[256];
 Zone *zone;
 ResourceString rs0(NCVIEWB);
 Dialog dialog(_RCT("SelectView_Dialog"));
  index1 = 0;
  namelist2[0] = _RCT("");
  db.views3d.names(&namelist1,&n1,&index1);
  lastzonename[0] = 0;
  if (namelist1 != 0 && n1 > 0)
  {
       dialog.add(new ViewSelectListDialogControl(100,n1,namelist1,&index1));
       dialog.add(new ListDialogControl(101,0,namelist2,lastzonename,256));
       ::v.setstring("op::lastzonename",lastzonename);
       if (dialog.process() == TRUE)
       {
           qDebug() << QString(namelist1[index1]);
           if ((v = db.views3d.match(namelist1[index1])) != 0)
           {
               View3dWindow *current=0;
               strcpy(lastzonename,::v.getstring("op::lastzonename"));
               qDebug() << "matched name : " << QString(v->getname());
                for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
                   if (strcmp(lastzonename,zone->getname()) == 0)
                   {
                        current = cadwindow->getcurrentwindow();
                        cadwindow->replace3dsubwindow(current,v);
                        if (cadwindow->getcurrentwindow() != 0)
                          cadwindow->getcurrentwindow()->zoom(zone->getumin(),zone->getvmin(),zone->getumax(),zone->getvmax());
                        break;
                   }
                 if (zone == 0)
                 {
                     ((QtMainWindow*)cadwindow->gethwnd())->centerWidget->debugView3dList();
                     current = cadwindow->getcurrentwindow();
                     qDebug() << "current name : " << QString(current->getview()->getname());
                     cadwindow->replace3dsubwindow(current,v);
                     //cadwindow->getcurrentsurface()->setView(v);
                     if (cadwindow->getcurrentwindow() != 0)
                       cadwindow->getcurrentwindow()->zoom(current->getumin(),current->getvmin(),current->getumax(),current->getvmax());
                     qDebug() << "current name : " << QString(current->getview()->getname());
                     ((QtMainWindow*)cadwindow->gethwnd())->resize(((QtMainWindow*)cadwindow->gethwnd())->size().width()-1,
                                                                   ((QtMainWindow*)cadwindow->gethwnd())->size().height()-1);
                     ((QtMainWindow*)cadwindow->gethwnd())->resize(((QtMainWindow*)cadwindow->gethwnd())->size());
                 }
                 ((QtMainWindow*)cadwindow->gethwnd())->centerWidget->debugView3dList();
           }
       }
       delete [] namelist1;
  }
  *cstate = ExitState;
}

void window_command2(int *cstate,HighLevelEvent *,void **)
{ cadwindow->cascade();
  *cstate = ExitState;
}

void window_command3(int *cstate,HighLevelEvent *,void **)
{ cadwindow->tile();
  *cstate = ExitState;
}

void window_command10(int *cstate,HighLevelEvent *,void **)
{ cadwindow->tab();
  *cstate = ExitState;
}

void paint_command0(int *cstate,HighLevelEvent *,void **)
{
  QtEndScene();
  cadwindow->paintall(1);
  *cstate = ExitState;
}

static void regenerateall(int,void *,View3dSurface *vs)
{
    if (((View3dWindow *)vs)->getoffscreenbitmap() != 0)
    {   ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.clear();
        ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.setorgtransform(vs->getumin(),vs->getvmin(),vs->getumax(),vs->getvmax());
        if (dynamic_cast<View2dWindow *>(vs) != 0)
            dynamic_cast<View2dWindow *>(vs)->regenerate();
    }
    else
    {   if (dynamic_cast<View2dWindow *>(vs) != 0)
            dynamic_cast<View2dWindow *>(vs)->regenerate();
    }
    db.clearScene(DM_ERASE,vs);
    if(vs->getScene())
        vs->getScene()->clear();
    InvalidateRect(vs->gethwnd(),0,TRUE);
}


void window_command4(int *cstate,HighLevelEvent *,void **)
{View3dSurface *vs;
 ResourceString rs127(NCVIEWB+127),rs128(NCVIEWB+128);
    if ((vs = cadwindow->getcurrentsurface()) == NULL)
	    cadwindow->MessageBox(rs127.gets(),rs128.gets(),MB_ICONINFORMATION);
    else
        regenerateall(0,0,vs);
    *cstate = ExitState;
}

void window_command5(int *cstate,HighLevelEvent *,void **)
{
    Entity *e;
  //cadwindow->drawcallback(0,NULL,regenerateall);
  cadwindow->paintall();
  for (db.geometry.start() ;  (e = db.geometry.next()) != 0 ; )
    if (e->issurface())
      ((SurfaceEntity *)e)->cleartriangles();
  *cstate = ExitState;
}


class DrawingSelectListDialogControl : public ListDialogControl
  {private:
   public:
     int selected[MaxViews]; 
     DrawingSelectListDialogControl(int id,int n,RCCHAR **list) :
         ListDialogControl(id,n,list,(int *)0)  {  memset(selected,-1,MaxViews * sizeof(int));  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
      
  };

void DrawingSelectListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void DrawingSelectListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{RCCHAR drawingname[256];
 View2d *v;
 Zone *zone;
 int i,j,k;

  if (updatescreen == 0)
    return;

  if (dialog->currentvalue(100,drawingname,256) && IsWindow(GetDlgItem(dialog->gethdlg(),100)))
    {  //  Get the list of selected items
       memset(selected,-1,MaxViews * sizeof(int));
       k = SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELITEMS,MaxViews,(RCLPARAM)selected);
       if (k == LB_ERR)
         memset(selected,-1,MaxViews * sizeof(int));
       else 
         for ( ; k < MaxViews ; k++)
           selected[k] = -1;                        

       //  Convert the index to an index into the original, unsorted list
       for (i = 0 ; i < MaxViews ; i++)
         if (selected[i] >= 0)
           {  SendDlgItemMessage(dialog->gethdlg(),100,LB_GETTEXT,selected[i],(RCLPARAM)drawingname);
              for (j = 0 ; j < n ; j++)
                if (RCSTRCMP(drawingname,list[j]) == 0)
                  selected[i] = j;
           }
        
       ((ListDialogControl *) dialog->getcontrol(101))->removeallitems(dialog);
       if ((v = db.views2d.match(drawingname)) != 0)
         {  for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
              ((ListDialogControl *) dialog->getcontrol(101))->additem(dialog,zone->getname());
         }
    }
}

/**
 * @brief window_command6
 * @param cstate
 *
 * Entry point for the Compose drawing select drawing command
 *
 * KMJ: Working in the Qt Version
 *
 */
void window_command6(int *cstate,HighLevelEvent *,void **)
{View2d *v;
 Dialog dialog(_RCT("SelectDrawing_Dialog"));
 int i,index1,n,m;
 RCCHAR **namelist1,*namelist2[1],lastzonename[256];
 Zone *zone;
 DrawingSelectListDialogControl *lsd;

  db.views2d.names(&namelist1,&n,&index1);
  // remove any layouts that already have windows up
  /*
  RCCHAR **checklist,**templist;
  db.views2d.names(&checklist,&n,&index1);
  if((templist = new RCCHAR *[n]) != NULL)
  {
      m=n;
      /*
      for(i=0; i<n; i++)
      {
          templist[i] = checklist[i];
          View3dWindow *window;
          for(cadwindow->GetView3dList()->start(); (window = cadwindow->GetView3dList()->next()) != 0 ;)
          {
              if(window->getsurfacetype() == View2dSurf)
              {
                  if(strcmp(((View2dWindow*)window)->getview2d()->getname(),checklist[i]) == 0)
                  {
                      templist[i] = 0;
                      m--;
                  }
              }
          }
      }
      if((namelist1 = new RCCHAR *[m]) != NULL)
      {
          m=0;
          for(i=0; i<n; i++)
              if(templist[i] != 0)
              {
                namelist1[m] = templist[i];
                m++;
              }
          n=m;
      }
      delete templist;
      delete checklist;
  }
    */
  namelist2[0] = _RCT("");
  lastzonename[0] = 0;
  if (namelist1 != NULL)
  	 {  dialog.add(lsd = new DrawingSelectListDialogControl(100,n,namelist1));
       dialog.add(new ListDialogControl(101,0,namelist2,lastzonename,256));
       ::v.setstring("op::lastzonename",lastzonename);
       if (dialog.process() == TRUE)
         {  for (i = 0 ; i < MaxViews ; i++)
              if (lsd->selected[i] >= 0 && (v = db.views2d.match(namelist1[lsd->selected[i]])) != 0)
                {
                  View3dWindow *window;
                  for(cadwindow->GetView3dList()->start(); (window = cadwindow->GetView3dList()->next()) != 0 ;)
                  {
                      if(window->getsurfacetype() == View2dSurf)
                      {
                          if(strcmp(((View2dWindow*)window)->getview2d()->getname(),namelist1[lsd->selected[i]])== 0)
                          {
                              //cadwindow->MessageBox("This layout is already displayed in a window and cannot be used again","Select Layout",MB_OK);
                              // show the view and make it the active view
                              ((QtMainWindow*)cadwindow->gethwnd())->centerWidget->maximizeView((RCView*)window->gethwnd());
                              ((QtMainWindow*)cadwindow->gethwnd())->centerWidget->setActiveWindow((RCView*)window->gethwnd());
                              delete namelist1;
                              *cstate = ExitState;
                              return;
                          }
                      }
                  }
                  strcpy(lastzonename,::v.getstring("op::lastzonename"));
                  for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
                     if (strcmp(lastzonename,zone->getname()) == 0)
                       {  cadwindow->create2dsubwindow(v);
                          if (cadwindow->getcurrentwindow() != 0)
                            cadwindow->getcurrentwindow()->zoom(zone->getumin(),zone->getvmin(),zone->getumax(),zone->getvmax());
                          break;
                       }
                   if (zone == 0)
                     cadwindow->create2dsubwindow(v);
                }
         }
       delete namelist1;
    }
  *cstate = ExitState;
}

/**
 * @brief renamedrawing_command
 * @param cstate
 *
 * Entry point for the Rename Composed View command
 *
 * KMJ: Working in Qt Version
 *
 */
void renamedrawing_command(int *cstate,HighLevelEvent *,void **)
{View2d *v;
 Dialog dialog(_RCT("RenameDrawing_Dialog"));
 int index1,n,i;
 RCCHAR **namelist1,newname[300];

  db.views2d.names(&namelist1,&n,&index1);
  strcpy(newname,"");
  if (namelist1 != NULL)
  	 {  dialog.add(new ListDialogControl(100,n,namelist1,&index1));
        dialog.add(new StringDialogControl(101,newname,300));
        if (dialog.process() == TRUE)
         {
            if ((v = db.views2d.match(namelist1[index1])) != 0 && db.views2d.match(newname) == 0)
            {
#ifdef USING_WIDGETS
               // check to see if there is a window up with the old name and rename it
               View3dWindowList *list = cadwindow->GetView3dList();
               View3dWindow *window=0;
               for (list->start(); window = list->next();)
                   if (dynamic_cast<View2dWindow *>(window) != 0 && strcmp(namelist1[index1],(dynamic_cast<View2dWindow *>(window))->getview2d()->getname()) == 0)
                   {
                       if(qobject_cast<QTabWidget*>((QObject*)cadwindow->subwindow->gethwnd()) !=0)
                       {
                           QTabWidget *qtabw = ((QTabWidget*)cadwindow->subwindow->gethwnd());
                           for(i=0; i < qtabw->count(); i++)
                           {
                               if(qtabw->tabText(i) == QString(namelist1[index1]))
                                   qtabw->setTabText(i,QString(newname));
                           }
                       }
                       //else if(qobject_cast<QMdiArea*>((QObject*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget()) !=0)
                       else if(qobject_cast<QMdiArea*>((QObject*)((QtMainWindow*)cadwindow->gethwnd())->MdiArea) !=0)
                       {
                           QMdiArea *qmda = (QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->MdiArea;
                           //QMdiArea *qmda = (QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget();
                           for(i=0; i < qmda->subWindowList().size(); i++)
                           {
                               if(qmda->subWindowList().at(i)->windowTitle() == QString(namelist1[index1]))
                                   qmda->subWindowList().at(i)->setWindowTitle(QString(newname));
                           }
                       }
                   }
#endif
               v->setname(newname);
            }
         }
       delete namelist1;
    }
  *cstate = ExitState;
}

class PaperSizeDialogControl : public ListDialogControl
  {private:
     int lastindex;
   public:
     PaperSizeDialogControl(int id,int n,RCCHAR **list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     PaperSizeDialogControl(int id,int n,QString *list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void PaperSizeDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void PaperSizeDialogControl::changefocus(Dialog *dialog,int)
{int index;
 RCCHAR figure[300],temp[300];
  if (dialog->currentvalue(101,&index) &&
      dialog->currentvalue(104,figure,300/*sizeof(figure)*/) &&
      lastindex != index)
    {
       if (index < View2d::nsizes - 1)
         {  ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,View2d::widths[index]);
            ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,View2d::heights[index]);
         }
       else
         {  ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,v.getreal("cd::nonstdwidth"));
            ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,v.getreal("cd::nonstdheight"));
         }

       // this will change the ending of the file name by adding the current size
       // if you have a set of different title blocks for each size you use ending in the size name
       // you will not need to browse for the file
       // title file name convention:
       // /somepath/my title drawing1-A4.cad, /somepath/my title drawing2-A4.cad etc.
       // /somepath/my title drawing-A4.cad, /somepath/my title drawing-A3.cad etc.

       //qDebug() << "figure : " << QString(figure);
       // use the platform separator
       RCCHAR sep[1] = {'/'};
       *sep = QDir::separator();
       if (strrchr(figure,'-') != 0 && (strrchr(figure,sep) == 0 || strrchr(figure,'-') > strrchr(figure,sep)))
       {
            strcpy(temp,figure);
            //qDebug() << "figure : " << QString(figure);
            //qDebug() << "temp : " << QString(temp);
            if (index < View2d::nsizes-1)
              strcpy(strrchr(temp,'-')+1,View2d::sizes[index]);
            else
              strcpy(strrchr(temp,'-')+1,"NS");
            //qDebug() << "temp : " << QString(temp);
            strcat(temp,".cad");
            //qDebug() << "temp : " << QString(temp);
            if(QFile::exists(QString(temp)))
                strcpy(figure,temp);
       }
       ((StringDialogControl *)dialog->getcontrol(104))->change(dialog,figure);
       lastindex = index;
    }
}

class TBFButtonDialogControl : public ButtonDialogControl
  {public:
     TBFButtonDialogControl(int id) : ButtonDialogControl(id)  {}
     int select(Dialog *);
  };

int TBFButtonDialogControl::select(Dialog *dialog)
{
 int i;
 RCOPENFILENAME ofn;
 RCCHAR directory[300],filename[300],filter[300];
 ResourceString rs169(NCVIEWB+169);
 ResourceString rs170(NCVIEWB+170);

  //_getcwd(directory, sizeof(directory));
    strcpy(directory,v.getstring("db::title-sheets-path"));
    if(strlen(directory) < 1)
     strcpy(directory,home.getpublicfilename("Title-Sheets"));
    else
    {
     // check if it exists
     // if not use the default
     if(!QDir(QString(directory)).exists())
         strcpy(directory,home.getpublicfilename("Title-Sheets"));
    }


  filename[0] = '\0';
  strcpy(filter,rs169.getws());
  //for (i = 0; filter[i] != '\0'; i++)
  //  if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = (HWND)dialog->gethdlg();
  ofn.lpstrFilter =  filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile=  filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrInitialDir =  directory;
  ofn.lpstrDefExt = _RCT("CAD");
  ofn.lpstrTitle =  rs170.getws();
  ofn.Flags = 0;//OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn))
    ((StringDialogControl *)dialog->getcontrol(104))->change(dialog,ofn.lpstrFile);

  return 0;
}

int getLayoutAutoNumber(QString baseName)
{
    // if there is a 2d view with the same name replace or add a number on the end
    // look for 2d views that contain the given baseName
    // if it has a number on the end increment it
    //QStringList vnames;
    int i,j;
    //db.views2d.names(&view2dlist,n2d,index);

    for (i = 0 ; i < MaxViews ; i++)
        if (db.views3d.match(i) == NULL && db.views2d.match(i) == NULL)
            break;

    int vnum = 0;
    for (j = 0 ; j < i ; j++)
    {
        if(db.views2d.match(j) != NULL)
        {
            //vnames << QString(db.views2d.match(j)->getname());
            QString vname(db.views2d.match(j)->getname());
            if(vname.indexOf(baseName,0,Qt::CaseSensitive) >= 0)
            {
                // get any numbers after the base name
                QString ending = vname.mid(baseName.length());
                bool ok=false;
                int num = ending.toInt(&ok,0);
                if(ok)
                {
                    if(num > vnum)
                        vnum = num;
                }
            }
        }
    }
    return vnum+1;
}

/**
 * @brief window_command7
 * @param cstate
 *
 * Entry point for Create Composed Drawing Command
 *
 * KMJ: Working in the Qt Version
 *
 */
void window_command7(int *cstate,HighLevelEvent *,void **)
{
 double paperwidth,paperheight;
 Dialog dialog("ComposedDrawing_Dialog");
 DialogControl *dc;
 int viewno,index;
 RCCHAR name[300],figure[300],*attribute;
 View2d *view2d;
 ResourceString rs129(NCVIEWB+129),rs130(NCVIEWB+130),rs131(NCVIEWB+131),rs132(NCVIEWB+132);
 Point origin(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0);
 BitMask options(32),visible(MaxViews);

  // get the number of views currently defined
  int newvnum = getLayoutAutoNumber(QString(rs129.gets()));
  RCCHAR szvnum[300];
  sprintf(szvnum,"%d",newvnum);

  memset(figure,0,sizeof(RCCHAR)*300);
  strcpy(name,rs129.gets());
  strcat(name,szvnum);
  paperwidth = View2d::widths[0];
  paperheight = View2d::heights[0];
  memset(figure,0,sizeof(RCCHAR)*300);
  strcpy(figure,v.getstring("vw::titleblock"));
  index = v.getinteger("vw::papersize");

  qDebug() << QString(figure);

  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(new PaperSizeDialogControl(101,View2d::nsizes,View2d::sizes,&index));
  dialog.add(new RealDialogControl(102,&paperwidth,1.0,0.0,1E10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(new RealDialogControl(103,&paperheight,1.0,0.0,1E10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new StringDialogControl(104,figure,300));
  dialog.add(new TBFButtonDialogControl(105));

  if (dialog.process() == TRUE)
    {  if (index == View2d::nsizes - 1)
         {  v.setreal("cd::nonstdwidth",paperwidth);
            v.setreal("cd::nonstdheight",paperheight);
         } 
       v.setinteger("vw::papersize",index);
       v.setstring("vw::titleblock",figure);
       qDebug() << "figure" << QString(figure);
       qDebug() << "vw::titleblock" << QString(v.getstring("vw::titleblock"));
       if (db.views2d.match(name) != NULL)
         cadwindow->MessageBox(rs131.gets(),rs130.gets(),MB_ICONINFORMATION);
       else if ((viewno = db.getnextviewno()) < 0)
         cadwindow->MessageBox(rs132.gets(),rs130.gets(),MB_ICONINFORMATION);
       else if ((view2d = new View2d(name,paperwidth,paperheight,short(viewno))) != NULL)
         {  db.views2d.add(view2d);
            cadwindow->create2dsubwindow(view2d);
            if (strlen(figure) > 0)
              {
                /*
                 RC_STAT st;
                 if (_stat(figure,&st) != 0)
                   {  strcpy(name,figure);
                      strcpy(figure,home.getpublicfilename(name));
                   }
                   */
                 if(!QFile::exists(QString(figure)))
                 {
                    strcpy(name,figure);
                    strcpy(figure,home.getpublicfilename(name));
                 }

                 db.saveundo(UD_STARTBLOCK,NULL);
                 Figure *fig = new Figure(figure,_RCT(""),origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                 visible.set(viewno);
                 fig->setvisible(visible);
                 if (fig != NULL && fig->getdefined())
                   {  db.geometry.add(fig);
                      if ((attribute = new RCCHAR[strlen("Title Block Figure")+1]) != NULL)
                        {  strcpy(attribute,"Title Block Figure");
                           fig->addat(attribute);  
                           fig->setattribute(attribute);
                           db.saveundo(UD_INSERTATT,fig);
                           //  Add the figure annotation text as well.
                           db.header.setvisible(*fig->getvisible());
                           fig->addattributetext();
                        }
                   }
                 else
                   delete fig;
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
         }
    }
  *cstate = ExitState;
}

class DDButtonDialogControl : public ButtonDialogControl
{private:
   RCCHAR **namelist;
   int nitems;
 public:
   DDButtonDialogControl(int id,RCCHAR **nm,int ni) : ButtonDialogControl(id)  {  namelist = nm;  nitems = ni;  }
   int select(Dialog *);
};

int DDButtonDialogControl::select(Dialog *dialog)
{int i,j,d,cstate,nselecteditems,*selecteditems;
 View2d *v;
 ResourceString rs133(NCVIEWB+133),rs134(NCVIEWB+134),rs135(NCVIEWB+135);
 RCCHAR message[300],name[300];
 Entity *e;
 EntityList list,emptylist;
 
  nselecteditems = SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELCOUNT,0,0);
  selecteditems = new int[nselecteditems];
  SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELITEMS,nselecteditems,(LPARAM)selecteditems);
  for (i = 0 ; i < nselecteditems ; i++)
    {  SendDlgItemMessage(dialog->gethdlg(),100,LB_GETTEXT,selecteditems[i],(LPARAM)name);
       if ((v = db.views2d.match(name)) != NULL)
         {  if (cadwindow->viewused(v->getviewno()))
              cadwindow->MessageBox(rs134.gets(),rs133.gets(),MB_ICONINFORMATION);
            else
              {  sprintf(message,rs135.gets(),name);
                 if (cadwindow->MessageBox(message,rs133.gets(),MB_YESNO) == IDYES)
	             					{  //  Delete any entities that are just part of this composed drawing
                      for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                        {  for (j = 0 ; j < MaxViews ; j++)
                             if (e->getvisible()->test(j) && j != v->getviewno() ||
                                 ! e->getvisible()->test(j) && j == v->getviewno())
                               break;
                           if (j == MaxViews)
                             list.add(e);
                        }

                      state.setselection(list,0);
                      d = 1;
                      delete_command(&cstate,0,(void **)&d);
                      list = emptylist;

                      db.views2d.del(v);  
                      delete v;
                      }
	             }
         }
    }

  return 101;
}

/**
 * @brief window_command8
 * @param cstate
 *
 * Entry point for the Delete Drawing command
 *
 * KMJ: Working in the Qt Version
 *
 */
void window_command8(int *cstate,HighLevelEvent *,void **)
{Dialog dialog(_RCT("DeleteDrawing_Dialog"));
 int index,n;
 RCCHAR **namelist;
  db.views2d.names(&namelist,&n,&index);
  if (namelist != NULL)
    {  dialog.add(new ListDialogControl(100,n,namelist,0,0));
       dialog.add(new DDButtonDialogControl(101,namelist,n));
       if (dialog.process() == TRUE)
		       delete [] namelist;
	   }
  *cstate = ExitState;
}

struct RBWindow9_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgpoint;
    QGraphicsItem *qglines[4];
#endif
   int state;
   View *view3d;
   Zone *zone;
   Point origin,p1,p2;
   double scale,angle;
   BitMask options;
   RBWindow9_data() : options(32) {}
};

void RBWindow9(int,void *data,Point *p2,View3dSurface *vs)
{RBWindow9_data *rbdata = (RBWindow9_data *) data;
 Point xdir,ydir;

  if(vs == 0 || vs->getsurfacetype() != View2dSurf)
      return;

  if (rbdata->state > 1)
    {
      PointE p(rbdata->origin);
      //PointE p(rbdata->p1);
      p.qgi = rbdata->qgpoint;
        //p.draw(DM_NORMAL);
        p.draw(DM_NORMAL,0,vs);
      rbdata->qgpoint = p.qgi;
    }
  if (rbdata->state == 3)
    {Point p3,p4;
      p3 = rbdata->p1;  p4 = *p2;  p4.y = rbdata->p1.y;
      EntityHeader header(0,0,0,0);
      Line line(header,p3,p3+Point(cos(rbdata->angle*acos(-1.0)/180.0),sin(rbdata->angle*acos(-1.0)/180.0),0.0));
      xdir = line.nearp(*p2) - p3;
      ydir = *p2 - line.nearp(*p2);

      Line line1(header,p3,p3+xdir);
      line1.qgi = rbdata->qglines[0];
          //line1.draw(DM_INVERTDASH);
          line1.draw(DM_INVERTDASH,0,vs);
      rbdata->qglines[0] = line1.qgi;

      Line line2(header,p3+xdir,p3+xdir+ydir);
      line2.qgi = rbdata->qglines[1];
          //line2.draw(DM_INVERTDASH);
          line2.draw(DM_INVERTDASH,0,vs);
      rbdata->qglines[1] = line2.qgi;

      Line line3(header,p3+xdir+ydir,p3+ydir);
      line3.qgi = rbdata->qglines[2];
          //line3.draw(DM_INVERTDASH);
          line3.draw(DM_INVERTDASH,0,vs);
      rbdata->qglines[2] = line3.qgi;

      Line line4(header,p3+ydir,p3);
      line4.qgi = rbdata->qglines[3];
          //line4.draw(DM_INVERTDASH);
          line4.draw(DM_INVERTDASH,0,vs);
      rbdata->qglines[3] = line4.qgi;
    }
}

/**
 * @brief window_command9
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Select a 3d drawing for a composed view command
 *
 * KMJ: Working in the Qt Version
 *
 */
void window_command9(int *cstate,HighLevelEvent *event,void **data)
{RBWindow9_data *rbdata = (RBWindow9_data *) *data;
 Dialog dialog(_RCT("SelectDrawingView_Dialog"));
 DialogControl *dc;
 int index,n;
 RCCHAR **namelist,*zonelist[1],lastzonename[300];
 double t;
 ResourceString rs136(NCVIEWB+136),rs137(NCVIEWB+137),rs138(NCVIEWB+138),rs139(NCVIEWB+139),rs140(NCVIEWB+140);
 ResourceString rs141(NCVIEWB+141),rs142(NCVIEWB+142),rs143(NCVIEWB+143);
 Point pmin,pmax,xdir,ydir,p;
 Zone *zone;

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBWindow9_data;  *data = rbdata;
         if (rbdata == NULL)
           {  state.fail(Memory,rs136.gets());
              *cstate = ExitState;
           }
         else    
           {  index = 0;
              db.views3d.names(&namelist,&n,&index);
              lastzonename[0] = 0;
              if (namelist != NULL)
              {
#ifdef USING_WIDGETS
                  rbdata->qgpoint = 0;
                   for(int i=0; i < 4; i++)
                       rbdata->qglines[i] = 0;
#endif
                   qDebug() << "options 0" <<  rbdata->options.test(0);
                   qDebug() << "options 2" <<  rbdata->options.test(2);
                   rbdata->scale = v.getreal("cd::plotscale");
                   rbdata->angle = v.getreal("cd::angle");
                   dialog.add(new ViewSelectListDialogControl(100,n,namelist,&index));
                   dialog.add(new SelectViewZoneListDialogControl(101,106,0,zonelist,lastzonename,300));
                   //dialog.add(new ListDialogControl(101,0,zonelist,lastzonename,300));
                   dialog.add(dc = new RealDialogControl(106,&rbdata->scale,1.0,1.0E-20,1.0E20));
                   dialog.add(new ScrollBarDialogControl(1101,dc));
                   dialog.add(new CheckBoxDialogControl(102,&rbdata->options,OVHide));// hidden when plotting
                   dialog.add(new CheckBoxDialogControl(103,&rbdata->options,OVDashHidden));// dashed when hidden
                   dialog.add(new CheckBoxDialogControl(105,&rbdata->options,OVShade));// shade when plotting
                   dialog.add(dc = new RealDialogControl(104,&rbdata->angle));
                   //dialog.add(new ScrollBarDialogControl(1104,dc));
                   ::v.setstring("op::lastzonename",lastzonename);
                   if (dialog.process() == TRUE)
                   {
                        v.setreal("cd::plotscale",rbdata->scale);
                        v.setreal("cd::angle",rbdata->angle);
                        if ((rbdata->view3d = db.views3d.match(namelist[index])) != NULL)
                        {
                            rbdata->zone = 0;
                            strcpy(lastzonename,::v.getstring("op::lastzonename"));
                            for (rbdata->view3d->zonelist.start() ; (zone = rbdata->view3d->zonelist.next()) != 0 ;)
                            {
                               if (strcmp(lastzonename,zone->getname()) == 0)
                               {
                                   qDebug() << "zone name : " << QString(zone->getname());
                                   rbdata->zone = zone;
                                   qDebug() << "rbdata zone name : " << QString(rbdata->zone->getname());
                                   break;
                               }
                            }
                            *cstate = 2;
                        }
                        else
                          *cstate = ExitState;
                   }
                   else
                     *cstate = ExitState;
                  delete namelist;
              }
              else
                *cstate = ExitState;
           }
         break;
       case 2:
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3:
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              if (((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() != View2dSurf)
                cadwindow->MessageBox(rs138.gets(),rs137.gets(),MB_ICONINFORMATION);
              else if (fabs(rbdata->p1.x - rbdata->p2.x) < db.getptoler())
                cadwindow->MessageBox(rs139.gets(),rs137.gets(),MB_ICONINFORMATION);
              else if (fabs(rbdata->p1.y - rbdata->p2.y) < db.getptoler())
                cadwindow->MessageBox(rs140.gets(),rs137.gets(),MB_ICONINFORMATION);
              else
                {View2dWindow *window;
                 OneView *ov;
                 double umin,vmin,umax,vmax;

                   Line line1(rbdata->p1,rbdata->p1+Point(cos(rbdata->angle*acos(-1.0)/180.0),sin(rbdata->angle*acos(-1.0)/180.0),0.0));
                   xdir = line1.nearp(rbdata->p2) - rbdata->p1;
                   ydir = rbdata->p2 - line1.nearp(rbdata->p2);

                   if (xdir.cross(ydir).z < 0.0)
                     {//  Flip y direction
                        rbdata->p1 += ydir;
                        rbdata->p2 -= ydir;

                     } 

                   Line line2(rbdata->p1,rbdata->p1+Point(cos(rbdata->angle*acos(-1.0)/180.0),sin(rbdata->angle*acos(-1.0)/180.0),0.0));
                   xdir = line2.nearp(rbdata->p2) - rbdata->p1;
                   ydir = rbdata->p2 - line2.nearp(rbdata->p2);

                   if (xdir.dot(line2.direction(0.0)) < 0.0)
                     {//  Swap p1 and p2
                        p = rbdata->p1;  rbdata->p1 = rbdata->p2;  rbdata->p2 = p;
                     }

                   Line line3(rbdata->p1,rbdata->p1+Point(cos(rbdata->angle*acos(-1.0)/180.0),sin(rbdata->angle*acos(-1.0)/180.0),0.0));
                   xdir = line3.nearp(rbdata->p2) - rbdata->p1;
                   ydir = rbdata->p2 - line3.nearp(rbdata->p2);

                   window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();

                   //  Calculate the extents 
                   if(rbdata->zone == 0)
                   {
                       View3dOutput output(cadwindow->gethwnd(),0,rbdata->view3d,1.0);
                       db.geometry.extents(&output,&pmin,&pmax);
                       if (pmin.x != pmax.x)
                         {  rbdata->origin.x = (rbdata->p1.x + rbdata->p2.x) / 2.0 - ((pmax.x + pmin.x) / 2.0 / rbdata->scale * cos(rbdata->angle*acos(-1.0)/180)  - (pmax.y + pmin.y) / 2.0 / rbdata->scale * sin(rbdata->angle*acos(-1.0)/180));
                            rbdata->origin.y = (rbdata->p1.y + rbdata->p2.y) / 2.0 - ((pmax.x + pmin.x) / 2.0 / rbdata->scale * sin(rbdata->angle*acos(-1.0)/180) + (pmax.y + pmin.y) / 2.0 / rbdata->scale * cos(rbdata->angle*acos(-1.0)/180));
                         }
                       else
                         {  rbdata->origin.x = (rbdata->p1.x + rbdata->p2.x) / 2.0;
                            rbdata->origin.y = (rbdata->p1.y + rbdata->p2.y) / 2.0;
                         }

                       xdir.normalize();
                       ydir.normalize();
                       umin = (rbdata->p1 - rbdata->origin).dot(xdir) * rbdata->scale;
                       vmin = (rbdata->p1 - rbdata->origin).dot(ydir) * rbdata->scale;
                       umax = (rbdata->p2 - rbdata->origin).dot(xdir) * rbdata->scale;
                       vmax = (rbdata->p2 - rbdata->origin).dot(ydir) * rbdata->scale;

                       if (umin > umax)
                         {  t = umax;  umax = umin;  umin = t;
                         }
                       if (vmin > vmax)
                         {  t = vmax;  vmax = vmin;  vmin = t;
                         }

                       qDebug() << "options 0" <<  rbdata->options.test(0);
                       qDebug() << "options 2" <<  rbdata->options.test(2);
                       ov = new OneView(rbdata->view3d,
                            umin,
                            vmin,
                            umax,
                            vmax,
                            rbdata->scale,rbdata->origin.x,rbdata->origin.y,rbdata->angle,rbdata->options,0);

                       if (ov != NULL)
                         {  db.saveundo(UD_CHANGEVIEW2D,window->getview2d());
                            window->getview2d()->add(ov);
                            window->update();
                         }
                   }
                   else
                   {
                       // KMJ: ToDo: find a better way to do this
                       qDebug() << "rbdata zone name : " << QString(rbdata->zone->getname());
                       View3dOutput output(cadwindow->gethwnd(),0,rbdata->view3d,1.0);
                       Point pmin2,pmax2;
                       //db.geometry.extents(&output,&pmin2,&pmax2);
                       pmin = output.uvtomodel(Point(rbdata->zone->getumin(),rbdata->zone->getvmin(),0.0));
                       pmax = output.uvtomodel(Point(rbdata->zone->getumax(),rbdata->zone->getvmax(),0.0));

                       //pmin = output.uvtoscreen(Point(rbdata->zone->getumin(),rbdata->zone->getvmin(),0.0));
                       //pmax = output.uvtoscreen(Point(rbdata->zone->getumax(),rbdata->zone->getvmax(),0.0));

                       //output.coordinate3d(pmin.x,pmin.y,&pmin2);
                       //output.coordinate3d(pmax.x,pmax.y,&pmax2);
                       //pmin = pmin2;
                       //pmax = pmax2;
                       /*
                       qDebug() << "rbdata zone name : " << QString(rbdata->zone->getname());
                       View3dOutput output(cadwindow->gethwnd(),0,rbdata->view3d,1.0);
                       Point pmin2,pmax2;
                       db.geometry.extents(&output,&pmin2,&pmax2);
                       pmin = output.uvtomodel(Point(rbdata->zone->getumin(),rbdata->zone->getvmin(),0.0));
                       pmax = output.uvtomodel(Point(rbdata->zone->getumax(),rbdata->zone->getvmax(),0.0));
                       */
                       if (pmin.x != pmax.x)
                         {  rbdata->origin.x = (rbdata->p1.x + rbdata->p2.x) / 2.0 - ((pmax.x + pmin.x) / 2.0 / rbdata->scale * cos(rbdata->angle*acos(-1.0)/180)  - (pmax.y + pmin.y) / 2.0 / rbdata->scale * sin(rbdata->angle*acos(-1.0)/180));
                            rbdata->origin.y = (rbdata->p1.y + rbdata->p2.y) / 2.0 - ((pmax.x + pmin.x) / 2.0 / rbdata->scale * sin(rbdata->angle*acos(-1.0)/180) + (pmax.y + pmin.y) / 2.0 / rbdata->scale * cos(rbdata->angle*acos(-1.0)/180));
                         }
                       else
                         {  rbdata->origin.x = (rbdata->p1.x + rbdata->p2.x) / 2.0;
                            rbdata->origin.y = (rbdata->p1.y + rbdata->p2.y) / 2.0;
                         }

                       xdir.normalize();
                       ydir.normalize();
                       umin = (rbdata->p1 - rbdata->origin).dot(xdir) * rbdata->scale;
                       vmin = (rbdata->p1 - rbdata->origin).dot(ydir) * rbdata->scale;
                       umax = (rbdata->p2 - rbdata->origin).dot(xdir) * rbdata->scale;
                       vmax = (rbdata->p2 - rbdata->origin).dot(ydir) * rbdata->scale;

                       if (umin > umax)
                         {  t = umax;  umax = umin;  umin = t;
                         }
                       if (vmin > vmax)
                         {  t = vmax;  vmax = vmin;  vmin = t;
                         }

                       qDebug() << "options 0" <<  rbdata->options.test(0);
                       qDebug() << "options 2" <<  rbdata->options.test(2);
                       ov = new OneView(rbdata->view3d,
                            umin,
                            vmin,
                            umax,
                            vmax,
                            rbdata->scale,rbdata->origin.x,rbdata->origin.y,rbdata->angle,rbdata->options,0);

                       if (ov != NULL)
                       {
                            db.saveundo(UD_CHANGEVIEW2D,window->getview2d());
                            window->getview2d()->add(ov);

                           //
                           pmin = output.uvtomodel(Point(rbdata->zone->getumin(),rbdata->zone->getvmin(),0.0));
                           pmax = output.uvtomodel(Point(rbdata->zone->getumax(),rbdata->zone->getvmax(),0.0));

                           //pmin = output.uvtoscreen(Point(rbdata->zone->getumin(),rbdata->zone->getvmin(),0.0));
                           //pmax = output.uvtoscreen(Point(rbdata->zone->getumax(),rbdata->zone->getvmax(),0.0));
                           //output.coordinate3d(pmin.x,pmin.y,&pmin2);
                           //output.coordinate3d(pmax.x,pmax.y,&pmax2);
                           //pmin = pmin2;
                           //pmax = pmax2;
#if 0
                           // possible alternative scaleing, fits the smallest zone size to the viewport
                           if(pmax.x - pmin.x  >= pmax.y - pmin.y)
                           {
                               double offset = ((pmax.x - pmin.x) - (pmax.y - pmin.y)) / 2.0;
                               pmin.x = pmin.x + offset;
                               pmax.x = pmax.x - offset;
                           }
                           else if(pmax.y - pmin.y  > pmax.x - pmin.x)
                           {
                               double offset = ((pmax.y - pmin.y) - (pmax.x - pmin.x)) / 2.0;
                               pmin.y = pmin.y + offset;
                               pmax.y = pmax.y - offset;
                           }
#endif
                           double du,dv,scale,scale1,ox,oy,cosa,sina;

                           du = pmax.x - pmin.x;
                           dv = pmax.y - pmin.y;
                           scale = ov->getscale();
                           if (du / dv > (ov->getumax() - ov->getumin()) / (ov->getvmax() - ov->getvmin()))
                             dv = du / (ov->getumax() - ov->getumin()) * (ov->getvmax() - ov->getvmin());
                           else
                             du = dv * (ov->getumax() - ov->getumin()) / (ov->getvmax() - ov->getvmin());
                           scale1 = scale * du / (ov->getumax() - ov->getumin());
                           ov->setscale(scale1);

                           cosa = cos(ov->getangle()*acos(-1.0)/180.0);
                           sina = sin(ov->getangle()*acos(-1.0)/180.0);

                           ox = ov->getxpos() + cosa * (ov->getumin()+ov->getumax()) / 2.0 / scale - sina * (ov->getvmin()+ov->getvmax()) / 2.0 / scale;
                           oy = ov->getypos() + sina * (ov->getumin()+ov->getumax()) / 2.0 / scale + cosa * (ov->getvmin()+ov->getvmax()) / 2.0 / scale;

                           ox +=  - ((pmin.x+pmax.x) / 2.0 / scale1 * cosa - (pmin.y+pmax.y) / 2.0 / scale1 * sina);
                           oy +=  - ((pmin.x+pmax.x) / 2.0 / scale1 * sina + (pmin.y+pmax.y) / 2.0 / scale1 * cosa);

                           ov->setposition(ox,oy);
                           ov->setclip((pmin.x+pmax.x)/2.0-du/2.0,(pmin.y+pmax.y)/2.0-dv/2.0,
                                                     (pmin.x+pmax.x)/2.0+du/2.0,(pmin.y+pmax.y)/2.0+dv/2.0);

                           window->update();
                           View3dSubSurface *ss=0;
                           if ((ss = window->pick(rbdata->p1)) != 0)
                           {
                               ss->regenerate();
                               ss->sizeevent(0,0);
                           }
                       }
                   }
                }
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }

#ifdef USING_WIDGETS
  if(rbdata)
  {
      if(rbdata->qgpoint)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgpoint);
      delete rbdata->qgpoint;
      rbdata->qgpoint=0;
      for(int i=0; i < 4; i++)
      {
          if(rbdata->qglines[i])
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines[i]);
          delete rbdata->qglines[i];
          rbdata->qglines[i]=0;
      }
  }
#endif

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(rs141.gets());
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(rs142.gets());
       rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBWindow9,rbdata);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(rs143.gets());
       rbdata->state = 3;
       rubberband.begin(0);
       rubberband.add(RBWindow9,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void createwindow_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("CreateWindow"));
  *cstate = ExitState;
}

class ZoneListDialogControl : public ListDialogControl
  {private:
     View *view;
   public:
     ZoneListDialogControl(int id,RCCHAR *name,int l,View *v) :
         ListDialogControl(id,0,0,name,l)  {  view = v;  }
     void load(Dialog *);
  };

void ZoneListDialogControl::load(Dialog *dialog)
{Zone *zone;
  ListDialogControl::load(dialog);
  for (view->zonelist.start() ; (zone = view->zonelist.next()) != 0 ;)
    additem(dialog,zone->getname());
}

void createwindow_command1(int *cstate,HighLevelEvent *e,void **)
{View *v;
 RCCHAR zonename[256];
 Zone *zone;
 ResourceString rs279(NCMAIN+279),rs280(NCMAIN+280),rs281(NCMAIN+281);
 Dialog dialog(_RCT("Zone_Dialog"));
 ResourceString rs[] =
   {  ResourceString(NCVIEWA),ResourceString(NCVIEWA+2),
      ResourceString(NCVIEWA+4),ResourceString(NCVIEWA+5),
      ResourceString(NCVIEWA+3),ResourceString(NCVIEWA+1),
      ResourceString(NCVIEWA+6),ResourceString(NCVIEWA+7),
      ResourceString(NCVIEWA+8)
   };
  if ((v = db.views3d.match(rs[((NewCommandEvent *)e)->getcommandid()-2501].gets())) != 0)
    {  if (! v->zonelist.empty())
         {  strcpy(zonename,"");
            dialog.add(new ZoneListDialogControl(100,zonename,256,v));
            if (dialog.process() == TRUE)
              {  if (strlen(zonename) != 0)
                   {  for (v->zonelist.start() ; (zone = v->zonelist.next()) != 0 ;)
                        if (strcmp(zonename,zone->getname()) == 0)
                          break;
                      if (zone != 0)
                        {  cadwindow->create3dsubwindow(v);
                           if (cadwindow->getcurrentwindow() != 0)
                             cadwindow->getcurrentwindow()->zoom(zone->getumin(),zone->getvmin(),zone->getumax(),zone->getvmax());
                        }
                      else
                        cadwindow->MessageBox(rs279.gets(),rs280.gets(),MB_ICONINFORMATION);
                   }
                 else
                   cadwindow->create3dsubwindow(v);
              }
         }
       else
         cadwindow->create3dsubwindow(v);
    }
  else
    cadwindow->MessageBox(rs281.gets(),rs280.gets(),MB_ICONINFORMATION);
  *cstate = ExitState;
}

void resetwindow_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("ResetWindow"));
  *cstate = ExitState;
}

// kmj added pers top view (#9)
void resetwindow_command1(int *cstate,HighLevelEvent *e,void **)
{ResourceString rs282(NCMAIN+282),rs283(NCMAIN+283);
 int translate[] = {  0,2,4,5,3,1,6,7,8,9  };
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
	   cadwindow->MessageBox(rs282.gets(),rs283.gets(),MB_ICONINFORMATION);
  else
    {  db.savezoomundo(1,cadwindow->getcurrentwindow());
       cadwindow->getcurrentsurface()->getview()->standard(translate[((NewCommandEvent *)e)->getcommandid()-2601]);
       cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
  *cstate = ExitState;
}

/**
 * @brief view_command1
 * @param cstate
 *
 * 3D view create
 *
 */
void view_command1(int *cstate,HighLevelEvent *,void **)
{View *oldv,*newv;
 Entity *e;
 Dialog *dialog;
 int n,viewno,index;
 RCCHAR **namelist,string[256];
 ResourceString rs1(NCVIEWB+1),rs2(NCVIEWB+2),rs3(NCVIEWB+3);
 ResourceString rs4(NCVIEWB+4),rs144(NCVIEWB+144);
  if ((dialog = new Dialog(_RCT("CreateView_Dialog"))) == NULL)
    state.fail(Memory,rs1.gets());
  else   
    {  index = 0;
       db.views3d.names(&namelist,&n,&index);
       if (namelist != NULL)
         {
            string[0] = 0;
            dialog->add(new StringDialogControl(100,string,256));
            dialog->add(new ListDialogControl(101,n,namelist,&index));
            if (dialog->process() == TRUE)
            {
                 if (db.views3d.match(string) != NULL)
                   cadwindow->MessageBox(rs2.gets(),rs3.gets(),MB_ICONINFORMATION);
                 else if ((viewno = db.getnextviewno()) < 0)
                   cadwindow->MessageBox(rs144.gets(),rs3.gets(),MB_ICONINFORMATION);
                 else if ((oldv = db.views3d.match(namelist[index])) != NULL)
                 {
                      if ((newv = new View(string,oldv->getperspective(),viewno,oldv->geteye(),oldv->getref(),oldv->getup(),oldv->getfront(),oldv->getback())) == NULL)
                        state.fail(Memory,rs4.gets());
                      else
                      {
                           db.views3d.add(newv);
                           for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                             e->getvisible()->set(newv->getviewno(),e->getvisible()->test(oldv->getviewno()) != 0);
                           cadwindow->create3dsubwindow(newv);
                      }
                 }
            }
            delete namelist;
         }
    }
  *cstate = ExitState;
}

struct RBView2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgline,*qgline1,*qgline2;
#endif
   int state;
   Point p1,p2;
};

void RBView2(int,void *data,Point *p2,View3dSurface *)
{
   RBView2_data *rbdata = (RBView2_data *)data;
#ifdef USING_WIDGETS
   if (rbdata->state == 2)
   {
       Line line(rbdata->p1,*p2);
       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
   }
   else if (rbdata->state == 3)
   {
       Line line1(rbdata->p1,rbdata->p2);
       line1.qgi = rbdata->qgline1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgline1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(rbdata->p2,*p2);
       line2.qgi = rbdata->qgline2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgline2 = line2.qgi; rbdata->zValue = line2.zValue;
   }
#else
  if (rbdata->state == 2)
    {  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {  Line line1(rbdata->p1,rbdata->p2);  line1.draw(DM_INVERT);
       Line line2(rbdata->p2,*p2);         line2.draw(DM_INVERT);
    }
#endif
}

/**
 * @brief view_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Reset eye point, reference point and up direction
 *
 * KMJ : working in Qt version
 *
 */
void view_command2(int *cstate,HighLevelEvent *event,void **data)
{RBView2_data *rbdata = (RBView2_data *) *data;
 Point p3;
 ResourceString rs5(NCVIEWB+5),rs6(NCVIEWB+6),rs7(NCVIEWB+7);
 ResourceString rs8(NCVIEWB+8),rs9(NCVIEWB+9),rs10(NCVIEWB+10);

  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs5.gets(),rs6.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else if ((rbdata = new RBView2_data) == NULL)
           {  state.fail(Memory,rs7.gets());
              *cstate = ExitState;
           }
         else
           *cstate = 1;
#ifdef USING_WIDGETS
         rbdata->qgline=0; rbdata->qgline1=0; rbdata->qgline2=0;
         rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p3 = ((Coordinate3dEvent *) event)->getp();
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
                {  cadwindow->MessageBox(rs8.gets(),rs9.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else if (! cadwindow->getcurrentsurface()->getview()->changeview(rbdata->p1,rbdata->p2,p3-rbdata->p2))
                {  cadwindow->MessageBox(rs10.gets(),rs9.gets(),MB_ICONINFORMATION);
                   *cstate = 1;
                }
              else
                {  cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
                   *cstate = ExitState;
                }
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
    }
#ifdef USING_WIDGETS
  if(rbdata->qgline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
  delete rbdata->qgline; rbdata->qgline=0;
  if(rbdata->qgline1)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline1);
  delete rbdata->qgline1; rbdata->qgline1=0;
  if(rbdata->qgline2)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline2);
  delete rbdata->qgline2; rbdata->qgline2=0;
#endif
//
//  About to enter state actions
//
  if (*cstate != ExitState)
    rbdata->state = *cstate;
  if (*cstate >= 1 && *cstate <= 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBView2,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCVIEWB+12);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCVIEWB+13);
  else if (*cstate == 3)
    cadwindow->prompt->normalprompt(NCVIEWB+14);
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBView3_data  
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgline1,*qgline2;
#endif
   View *view;
   Point ref,up;
};

void RBView3(int draw,void *data,Point *p2,View3dSurface *)
{
  RBView3_data *rbdata = (RBView3_data *)data;
  if (draw == RB_DRAW && cadwindow->getcurrentsurface()->getview() != rbdata->view)
    {  rbdata->view = cadwindow->getcurrentsurface()->getview();
       if (rbdata->view != NULL)
         {  state.setposition(rbdata->view->geteye());
            rbdata->ref = rbdata->view->getref();
            rbdata->up = rbdata->view->getup();
         }
    }
#ifdef USING_WIDGETS
  if (rbdata->view != NULL)
  {
       Line line1(rbdata->ref,rbdata->ref + rbdata->up * (rbdata->ref - *p2).length()/4.0);

       line1.qgi= rbdata->qgline1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgline1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(rbdata->ref,*p2);

       line2.qgi= rbdata->qgline2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgline2 = line2.qgi; rbdata->zValue = line2.zValue;
  }
#else
  if (rbdata->view != NULL)
    {  Line line1(rbdata->ref,rbdata->ref + rbdata->up * (rbdata->ref - *p2).length()/4.0);  line1.draw(DM_INVERT);
       Line line2(rbdata->ref,*p2);         line2.draw(DM_INVERT);
    }
#endif
}

/**
 * @brief view_command3
 * @param cstate
 * @param event
 * @param data
 *
 * set view eye point
 *
 * KMJ : working in Qt version
 *
 */
void view_command3(int *cstate,HighLevelEvent *event,void **data)
{RBView3_data *rbdata = (RBView3_data *) *data;
 Point p;
 ResourceString rs15(NCVIEWB+15),rs16(NCVIEWB+16),rs17(NCVIEWB+17);
 ResourceString rs18(NCVIEWB+18),rs19(NCVIEWB+19),rs20(NCVIEWB+20),rs21(NCVIEWB+21);
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs15.gets(),rs16.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else if ((rbdata = new RBView3_data) == NULL)
           {  state.fail(Memory,rs17.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->view = cadwindow->getcurrentsurface()->getview();
              state.setposition(rbdata->view->geteye());
              rbdata->ref = rbdata->view->getref();
              rbdata->up = rbdata->view->getup();
              *cstate = 1;
           }
#ifdef USING_WIDGETS
         rbdata->qgline1 = 0; rbdata->qgline2=0;
         rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
                {  cadwindow->MessageBox(rs18.gets(),rs19.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else if (! cadwindow->getcurrentsurface()->getview()->changeview(0,p))
                {  cadwindow->MessageBox(rs20.gets(),rs21.gets(),MB_ICONINFORMATION);
                   *cstate = 1;
                }
              else
                {  cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
    }
#ifdef USING_WIDGETS
         if(rbdata->qgline1)
             cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline1);
         delete rbdata->qgline1; rbdata->qgline1=0;
         if(rbdata->qgline2)
             cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline2);
         delete rbdata->qgline2; rbdata->qgline2=0;
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBView3,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCVIEWB+22);
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBView4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgline1,*qgline2;
#endif
   View *view;
   Point eye,up;
};

void RBView4(int draw,void *data,Point *p2,View3dSurface *)
{
  RBView4_data *rbdata = (RBView4_data *)data;
  if (draw == RB_DRAW && cadwindow->getcurrentsurface()->getview() != rbdata->view)
    {  rbdata->view = cadwindow->getcurrentsurface()->getview();
       if (rbdata->view != NULL)
         {  state.setposition(rbdata->view->getref());
            rbdata->eye = rbdata->view->geteye();
            rbdata->up = rbdata->view->getup();
         }
    }
#ifdef USING_WIDGETS
  if (rbdata->view != NULL)
  {
       Line line1(rbdata->eye,*p2);

       line1.qgi= rbdata->qgline1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgline1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(rbdata->up*((rbdata->eye-*p2).length()/4.0)+*p2,*p2);

       line2.qgi= rbdata->qgline1; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgline1 = line2.qgi; rbdata->zValue = line2.zValue;
  }
#else
  if (rbdata->view != NULL)
    {  Line line1(rbdata->eye,*p2);  line1.draw(DM_INVERT);
       Line line2(rbdata->up*((rbdata->eye-*p2).length()/4.0)+*p2,*p2);  line2.draw(DM_INVERT);
    }
#endif
}

/**
 * @brief view_command4
 * @param cstate
 * @param event
 * @param data
 *
 * set view reference point
 *
 * KMJ : working in Qt version
 *
 */
void view_command4(int *cstate,HighLevelEvent *event,void **data)
{RBView4_data *rbdata = (RBView4_data *) *data;
 Point p;
 ResourceString rs23(NCVIEWB+23),rs24(NCVIEWB+24),rs25(NCVIEWB+25),rs26(NCVIEWB+26);
 ResourceString rs27(NCVIEWB+27),rs28(NCVIEWB+28),rs29(NCVIEWB+29);
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs23.gets(),rs24.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else if ((rbdata = new RBView4_data) == NULL)
           {  state.fail(Memory,rs25.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->view = cadwindow->getcurrentsurface()->getview();
              state.setposition(rbdata->view->getref());
              rbdata->eye = rbdata->view->geteye();
              rbdata->up = rbdata->view->getup();
              *cstate = 1;
           }
#ifdef USING_WIDGETS
         rbdata->qgline1 = 0; rbdata->qgline2=0;
         rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
                {  cadwindow->MessageBox(rs26.gets(),rs27.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else if (! cadwindow->getcurrentsurface()->getview()->changeview(1,p))
                {  cadwindow->MessageBox(rs28.gets(),rs29.gets(),MB_ICONINFORMATION);
                   *cstate = 1;
                }
              else
                {  cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
    }
#ifdef USING_WIDGETS
         if(rbdata->qgline1)
             cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline1);
         delete rbdata->qgline1; rbdata->qgline1=0;
         if(rbdata->qgline2)
             cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline2);
         delete rbdata->qgline2; rbdata->qgline2=0;
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBView4,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCVIEWB+30);
  else if (*cstate == ExitState)
    delete rbdata;
}

#ifdef USING_WIDGETS
static QGraphicsItem *RBV5_qgline=0;
#endif
void RBView5(int mode,void *data,Point *p2,View3dSurface *)
{
    Point *eye = (Point *) data;
#ifdef NEW_RUBBERB
    if(mode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    Line line(*eye,*p2);
    line.draw(DM_INVERT);
#else
    Line line(*eye,*p2);
    line.qgi = RBV5_qgline; line.zValue = QGDRAGZVALUE(QGMAXZVALUE);
    line.draw(DM_INVERT);
    RBV5_qgline = line.qgi;
#endif
}

void view_command5(int *cstate,HighLevelEvent *event,void **)
{static double height;
 static View *view;
 static View3dWindow *window;
 static Point eye,ref;
 Point up(0.0,0.0,1.0);
 ResourceString rs31(NCVIEWB+31);
 ResourceString rs32(NCVIEWB+32);
 ResourceString rs33(NCVIEWB+33);
 ResourceString rs34(NCVIEWB+34);
 ResourceString rs35(NCVIEWB+35);
 ResourceString rs36(NCVIEWB+36);
 ResourceString rs37(NCVIEWB+37);
 ResourceString rs38(NCVIEWB+38);
 Dialog dialog(rs32.gets(),rs31.gets(),&height,ScaleByLength);
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs33.gets(),rs34.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  height = v.getreal("vw::height");
              view = cadwindow->getcurrentsurface()->getview();
              window = cadwindow->getcurrentwindow();
              if (dialog.process() == TRUE)
                {  v.setreal("vw::height",height);
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  eye = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Exit) || event->isa(Abort))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  ref = ((Coordinate3dEvent *) event)->getp();
              eye.z = height;  ref.z = height;
              db.savezoomundo(1,window);
              if (! view->changeview(eye,ref,up))
                {  cadwindow->MessageBox(rs35.gets(),rs36.gets(),MB_ICONINFORMATION);
                   *cstate = 1;
                }
              else
                {  if (! view->changeview(1))
                     cadwindow->MessageBox(rs37.gets(),rs38.gets(),MB_ICONINFORMATION);
                   cadwindow->updatewindows(view);
                   *cstate = ExitState;
                }
           }
         else if (event->isa(Exit) || event->isa(Abort))
           *cstate = ExitState;
         break;
    }

#ifdef USING_WIDGETS
  if(RBV5_qgline)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(RBV5_qgline);
      delete RBV5_qgline;
      RBV5_qgline=0;
  }
#endif

  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCVIEWB+39);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 2)
    {  cadwindow->prompt->normalprompt(NCVIEWB+40);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBView5,&eye);
    }
}

void view_command6(int *cstate,HighLevelEvent *,void **)
{Dialog *dialog;
 double focaldist;
 Point eye;
 View *view;
 ResourceString rs41(NCVIEWB+41);
 ResourceString rs42(NCVIEWB+42);
 ResourceString rs43(NCVIEWB+43);
 ResourceString rs44(NCVIEWB+44);
 ResourceString rs45(NCVIEWB+45);
 ResourceString rs46(NCVIEWB+46);
 ResourceString rs47(NCVIEWB+47);
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs41.gets(),rs42.gets(),MB_ICONINFORMATION);
  else
    {  view = cadwindow->getcurrentsurface()->getview();
       focaldist = (view->getref() - view->geteye()).length();
       if ((dialog = new Dialog(rs43.gets(),rs44.gets(),&focaldist,ScaleByLength)) == NULL)
         state.fail(Memory,rs45.gets());
       else if (dialog->process() == TRUE)
         {  eye = view->getref() + (view->geteye() - view->getref()).normalize() * focaldist;
            db.savezoomundo(1,cadwindow->getcurrentwindow());
            if (! view->changeview(0,eye))
              cadwindow->MessageBox(rs46.gets(),rs47.gets(),MB_ICONINFORMATION);
            else
              cadwindow->updatewindows(view);
            delete dialog;
         }
    }
  *cstate = ExitState;
}

#ifdef USING_WIDGETS
static QGraphicsItem *RBViewClip_qgi=0;
#endif
void RBViewClip(int dm,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
  Point *p = (Point *) data;
  Line line(*p,*p2);
  line.qgi = RBViewClip_qgi;line.zValue = QGDRAGZVALUE(QGMAXZVALUE);
  line.draw(DM_INVERT);
  RBViewClip_qgi = line.qgi;
#else
  Point *p = (Point *) data;
  Line line(*p,*p2);
  line.draw(DM_INVERT);
#endif
}

void view_command7(int *cstate,HighLevelEvent *event,void **data)
{Point *rbdata = (Point *) *data;
 Dialog dialog(_RCT("ViewClip_Dialog"));
 DialogControl *dc;
 double front,back,f,b,t;
 View *view;
 int result;
 ResourceString rs48(NCVIEWB+48);
 ResourceString rs49(NCVIEWB+49);
 ResourceString rs50(NCVIEWB+50);
 ResourceString rs51(NCVIEWB+51);
 ResourceString rs52(NCVIEWB+52);

  switch (*cstate)
    {case InitialState :
       if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
         cadwindow->MessageBox(rs48.gets(),rs49.gets(),MB_ICONINFORMATION);
       else
         {  view = cadwindow->getcurrentsurface()->getview();
            front = -view->getfront();
            back = -view->getback();
            dialog.add(dc = new RealDialogControl(100,&front,ScaleByLength));
            //dialog.add(new ScrollBarDialogControl(1100,dc));
            dialog.add(dc = new RealDialogControl(101,&back,ScaleByLength));
            //dialog.add(new ScrollBarDialogControl(1101,dc));
            dialog.add(new ButtonDialogControl(102));
            if ((result = dialog.process()) == TRUE)
              {  db.savezoomundo(1,cadwindow->getcurrentwindow());
                 if (! view->changeclip(-front,-back))
                   cadwindow->MessageBox(rs51.gets(),rs52.gets(),MB_ICONINFORMATION);
                 else
                   cadwindow->updatewindows(view);
                 *cstate = ExitState;
              }
            else if (result == 102)
              { rbdata = new Point;
#ifdef USING_WIDGETS
                RBViewClip_qgi=0;
#endif
                *data = rbdata;
                *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         { *rbdata = ((Coordinate3dEvent *) event)->getp();
           *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         { if (cadwindow->getcurrentwindow() != 0 && cadwindow->getcurrentwindow()->getsurfacetype() != View2dSurf &&
               (view = cadwindow->getcurrentsurface()->getview()) != 0)
             {  db.savezoomundo(1,cadwindow->getcurrentwindow());
                f = -view->getfront();
                b = -view->getback();
                front = (b-f)*cadwindow->getcurrentwindow()->modeltoview(*rbdata).z + f;
                back = (b-f)*cadwindow->getcurrentwindow()->modeltoview(((Coordinate3dEvent *) event)->getp()).z + f;
                if (front < back)
                  {  t = front;  front = back;  back = t;
                  }
                if (! view->changeclip(-front,-back))
                   cadwindow->MessageBox(rs51.gets(),rs52.gets(),MB_ICONINFORMATION);
                 else
                   cadwindow->updatewindows(view);
                 *cstate = ExitState;
             }
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }
#ifdef USING_WIDGETS
  if(RBViewClip_qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(RBViewClip_qgi);
  delete RBViewClip_qgi; RBViewClip_qgi=0;
#endif
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(_RCT("Locate the front clip plane"));
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBViewClip,rbdata);
       cadwindow->prompt->normalprompt(_RCT("Locate the back clip plane"));
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void view_command8(int *cstate,HighLevelEvent *,void **)
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

void view_command9(int *cstate,HighLevelEvent *,void **)
{ResourceString rs62(NCVIEWB+62),rs63(NCVIEWB+63),rs64(NCVIEWB+64);
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

struct RBView10_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    Point p1,p2;
   double angle;
   RCCHAR viewname[300];
};

#ifdef NEW_RUBBERB
void RBView10(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBView10_data *rbdata = (RBView10_data *) data;
  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  Line line(rbdata->p1,*p2);
  line.draw(DM_INVERT);
}
#else
void RBView10(int,void *data,Point *p2,View3dSurface *)
{
  RBView10_data *rbdata = (RBView10_data *) data;
#ifdef USING_WIDGETS
  Line line(rbdata->p1,*p2);
  line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
  line.draw(DM_INVERT);
  rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
#else
  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
#endif
}
#endif

/**
 * @brief view_command10
 * @param cstate
 * @param event
 * @param data
 *
 * Change view by rotation
 *
 * KMJ : works in Qt version
 *
 */
void view_command10(int *cstate,HighLevelEvent *event,void **data)
{
#if 0
 // KMJ: pending
 RBView10_data *rbdata = (RBView10_data *) *data;
 Transform t;
 Entity *e;
 Point p1,p2,eye,ref,up;
 Dialog *dialog;
 View *view;
 ResourceString rs65(NCVIEWB+65),rs66(NCVIEWB+66),rs67(NCVIEWB+67);
 ResourceString rs68(NCVIEWB+68),rs69(NCVIEWB+69),rs70(NCVIEWB+70);
 ResourceString rs71(NCVIEWB+71),rs72(NCVIEWB+72),rs73(NCVIEWB+73);
 ResourceString rs74(NCVIEWB+74),rs75(NCVIEWB+75),rs76(NCVIEWB+76);
  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           cadwindow->MessageBox(rs65.gets(),rs66.gets(),MB_ICONINFORMATION);
         else if ((rbdata = new RBView10_data) == NULL)
           {  state.fail(Memory,rs67.gets());
              *cstate = ExitState;
           }
         else if ((dialog = new Dialog(rs68.gets(),rs69.gets(),&rbdata->angle,DontScale)) == NULL)
           {  state.fail(Memory,rs70.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->angle = v.getreal("vw::angle");
              strcpy(rbdata->viewname,cadwindow->getcurrentsurface()->getview()->getname());
              dialog->add(new RealDialogControl(101,&rbdata->angle));
              if (dialog->process() == TRUE)
                {  v.setreal("vw::angle",rbdata->angle);
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
              delete dialog;
           }
#ifdef USING_WIDGETS
         rbdata->qgi=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
                {  cadwindow->MessageBox(rs71.gets(),rs72.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     {  rbdata->p1 = l->getp1();  rbdata->p2 = l->getp2();
                     }
                   else
                     {  rbdata->p1 = l->getp2();  rbdata->p2 = l->getp1();
                     }
                   *cstate = 3;
                   db.savezoomundo(1,cadwindow->getcurrentwindow());
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 3;
              db.savezoomundo(1,cadwindow->getcurrentwindow());
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 3)
    {  if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       else
         {  t.rotate(rbdata->p1,rbdata->p2-rbdata->p1,-rbdata->angle * M_PI / 180.0);
            if ((view = db.views3d.match(rbdata->viewname)) != 0)
              {  eye = t.transform(view->geteye());
                 ref = t.transform(view->getref());
                 up = t.transform(view->getref()+view->getup()) - ref;
                 if (! view->changeview(eye,ref,up))
                   {  cadwindow->MessageBox(rs73.gets(),rs74.gets(),MB_ICONINFORMATION);
                      *cstate = ExitState;
                   }
                 else
                   cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
              }
         }
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
   if(rbdata->qgi)
       cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
   delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+76);
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBView10,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+77);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+75);
    }
  else if (*cstate == ExitState)
    delete rbdata;
#endif
}

/**
 * @brief view_command11
 * @param cstate
 *
 * Change view to workplane
 *
 * KMJ : working in Qt version
 */
void view_command11(int *cstate,HighLevelEvent *,void **)
{Dialog *dialog;
 double focaldist;
 Point eye,ref,up;
 View *view;
 Workplane *w;
 ResourceString rs78(NCVIEWB+78),rs79(NCVIEWB+79),rs80(NCVIEWB+80),rs81(NCVIEWB+81);
 ResourceString rs82(NCVIEWB+82),rs83(NCVIEWB+83),rs84(NCVIEWB+84),rs85(NCVIEWB+85);
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs78.gets(),rs79.gets(),MB_ICONINFORMATION);
  else if ((w = db.workplanes.getcurrent()) == NULL)
    cadwindow->MessageBox(rs80.gets(),rs79.gets(),MB_ICONINFORMATION);
  else
    {  view = cadwindow->getcurrentsurface()->getview();
       focaldist = (view->geteye()-view->getref()).length();
       if ((dialog = new Dialog(rs81.gets(),rs82.gets(),&focaldist,ScaleByLength)) == NULL)
         state.fail(Memory,rs83.gets());
       else if (dialog->process() == TRUE)
         {  ref = w->getorigin();
            eye = ref + w->getzaxis() * focaldist;
            up = w->getyaxis();
            db.savezoomundo(1,cadwindow->getcurrentwindow());
            if (! view->changeview(eye,ref,up))
              cadwindow->MessageBox(rs84.gets(),rs85.gets(),MB_ICONINFORMATION);
            else
              cadwindow->updatewindows(view);
         }
       delete dialog;
    }       
  *cstate = ExitState;
}

struct RBView12_data
  {View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
  };

void RBView12(int draw,void *data,int x,int y,View3dSurface *surface)
{RBView12_data *rbdata = (RBView12_data *) data;
 Transform t1,t2;
 double uangle,vangle;
 Point eye,ref,up;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    rbdata->last = surface;
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
      /*
       rbdata->lastselected = cadwindow->getcurrentwindow();
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
         */
    }
  if (rbdata->lastselected == NULL) return;
  uangle = -((double) y / (double) surface->getheight() - 0.5) * M_PI / 8.0;
  vangle = -((double) x / (double) surface->getwidth() - 0.5) * M_PI / 8.0;
  t1.rotate(surface->getview()->getref(),surface->getuaxis(),uangle);
  t2.rotate(surface->getview()->getref(),surface->getvaxis(),vangle);
  t1.apply(t2);
  eye = t1.transform(surface->getview()->geteye());
  ref = t1.transform(surface->getview()->getref());
  up = t1.transform(surface->getview()->getref() + surface->getview()->getup()) - ref;
  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,up))
         {  surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,up) &&
            surface->getview()->changeview(eye,ref,up))
         {  ((View3dSurface *)rbdata->bitmap)->updatematrix();  surface->updatematrix();
            rbdata->bitmap->paintevent();
            //BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
            //       rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
}

/**
 * @brief view_command12
 * @param cstate
 * @param data
 *
 * Change view by dynamic rotation
 *
 *
 */
void view_command12(int *cstate,HighLevelEvent *,void **data)
{RBView12_data *rbdata = (RBView12_data *) *data;
 ResourceString rs86(NCVIEWB+86),rs87(NCVIEWB+87),rs88(NCVIEWB+88);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView12_data) == NULL)
           {  state.fail(Memory,rs86.gets());
              *cstate = ExitState;
              break;
           }
         rbdata->bitmap = NULL;
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs87.gets(),rs88.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata->last = NULL;
              rbdata->lastselected = cadwindow->getcurrentwindow();
              if (rbdata->lastselected->getsurfacetype() == View2dSurf)
                rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
              else  
                rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
              if (! rbdata->bitmap->getdefined())
                {  delete rbdata->bitmap;
                   rbdata->bitmap = NULL;
                }
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView12,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+89);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
}

struct RBView40_data
  {View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
   Point origin,zaxis;
   double angle;
  };

void RBView40(int draw,void *data,int x,int y,View3dSurface *surface)
{RBView40_data *rbdata = (RBView40_data *) data;
 Transform t1,t2;
 Point eye,ref,up;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    {  rbdata->last = surface;
       if (surface != 0)
         rbdata->angle = ((double) y / (double) surface->getheight() - 0.5) * M_PI / 8.0;
       return;
    }
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
       rbdata->lastselected = cadwindow->getcurrentwindow();
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
    }
  if (rbdata->lastselected == NULL) return;
  t1.rotate(rbdata->origin,rbdata->zaxis,rbdata->angle);
  eye = t1.transform(surface->getview()->geteye());
  ref = t1.transform(surface->getview()->getref());
  up = t1.transform(surface->getview()->getref() + surface->getview()->getup()) - ref;
  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,up))
         {  surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,up) &&
            surface->getview()->changeview(eye,ref,up))
         {  ((View3dSurface *)rbdata->bitmap)->updatematrix();  surface->updatematrix();
            rbdata->bitmap->paintevent();
            BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
}

void view_command40(int *cstate,HighLevelEvent *event,void **data)
{RBView40_data *rbdata = (RBView40_data *) *data;
 ResourceString rs86(NCVIEWB+86),rs87(NCVIEWB+87),rs88(NCVIEWB+88);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView40_data) == NULL)
           {  state.fail(Memory,rs86.gets());
              *cstate = ExitState;
              break;
           }
         rbdata->bitmap = NULL;
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs87.gets(),rs88.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata->last = NULL;
              rbdata->lastselected = cadwindow->getcurrentwindow();
              rbdata->angle = 0.0;
              if (rbdata->lastselected->getsurfacetype() == View2dSurf)
                rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
              else  
                rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
              if (! rbdata->bitmap->getdefined())
                {  delete rbdata->bitmap;
                   rbdata->bitmap = NULL;
                }
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *)event)->getp();
              rbdata->zaxis = db.workplanes.getcurrent()->getzaxis();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Line *line;
            Circle *circle;
              state.destroyselection(1);
              if (((EntitySelectedEvent *) event)->getentity()->isa(line_entity))
                {  line = (Line *) ((EntitySelectedEvent *) event)->getentity();
                   rbdata->origin = line->getp1();
                   rbdata->zaxis = (line->getp2() - line->getp1()).normalize();
                   *cstate = 2;
                }
              else if (((EntitySelectedEvent *) event)->getentity()->isa(circle_entity))
                {  circle = (Circle *) ((EntitySelectedEvent *) event)->getentity();
                   rbdata->origin = circle->getorigin();
                   rbdata->zaxis = circle->getxaxis().cross(circle->getyaxis()).normalize();
                   *cstate = 2;
                }
           }
         else
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate the point, line, circle or arc to rotate about."));
    }
  else if (*cstate == 2)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView40,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+89);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
}

struct RBAlignComposed_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgi1,*qgi2;
#endif
   Point p1,p2;
   int state,gridstate;
   View3dSubSurface *ss;
   View2dWindow *window;
};

#ifdef NEW_RUBBERB
void RBAlignComposed(int dm,void *data,Point *p2,View3dSurface *v)
{
    RBAlignComposed_data *rbdata = (RBAlignComposed_data *) data;

    if(dm == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    if (rbdata->state == 3)
    {
         Line line(rbdata->p1,*p2);
         line.draw(DM_INVERT);
    }
    else
    {
         Line line1(rbdata->p1,rbdata->p2);
         line1.draw(DM_INVERT);

         Line line2(*p2,line1.nearp(*p2));
         line2.draw(DM_INVERT);
    }
}
#else
void RBAlignComposed(int dm,void *data,Point *p2,View3dSurface *v)
{
  RBAlignComposed_data *rbdata = (RBAlignComposed_data *) data;

  if (rbdata->state == 3)
  {
       Line line(rbdata->p1,*p2);
       line.qgi = rbdata->qgi1;
       line.draw(DM_INVERT);
       rbdata->qgi1 = line.qgi;
  }
  else 
  {
       Line line1(rbdata->p1,rbdata->p2);
       line1.qgi = rbdata->qgi1;
       line1.draw(DM_INVERT);
       rbdata->qgi1 = line1.qgi;

       Line line2(*p2,line1.nearp(*p2));
       line2.qgi = rbdata->qgi2;
       line2.draw(DM_INVERT);
       rbdata->qgi2 = line2.qgi;
  }
}
#endif

/**
 * @brief view_command42
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Align Composed drawing View command
 *
 */
void view_command42(int *cstate,HighLevelEvent *event,void **data)
{RBAlignComposed_data *rbdata = (RBAlignComposed_data *) *data;
 Point p3,p4;
 ResourceString rs295(NCVIEWB+179),rs163(NCVIEWB+163);
 Point uv1,uv2;

  switch (*cstate)
    {case InitialState :
       rbdata = new RBAlignComposed_data;  *data = rbdata;
       rbdata->gridstate = db.grid.getsnap();
#ifdef USING_WIDGETS
       rbdata->qgi1=0;
       rbdata->qgi2=0;
#endif
       db.grid.setsnap(0);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  rbdata->window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((rbdata->ss = rbdata->window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   *cstate = 2;
                 else
                   cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
            *cstate = 3;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
            *cstate = 4;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 4 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p3 = ((Coordinate3dEvent *)event)->getp();
            db.saveundo(UD_CHANGEVIEW2D,rbdata->window->getview2d());

            Line line(rbdata->p1,rbdata->p2);
            
            p4 = line.nearp(p3);
             
            rbdata->ss->getoneview()->setposition(rbdata->ss->getoneview()->getxpos()+p4.x-p3.x,rbdata->ss->getoneview()->getypos()+p4.y-p3.y);

            rbdata->window->update();
            rbdata->window->zoom(rbdata->window->getumin(),rbdata->window->getvmin(),rbdata->window->getumax(),rbdata->window->getvmax());

            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

#ifdef USING_WIDGETS
  if(rbdata)
  {
    if(rbdata->qgi1)
        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi1);
    delete rbdata->qgi1;
    rbdata->qgi1=0;
    if(rbdata->qgi2)
        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi2);
    delete rbdata->qgi2;
    rbdata->qgi2=0;
  }
#endif

  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB + 171 + *cstate);
       cadwindow->setdragviewboundary(0);
       rbdata->state = *cstate;
       if (*cstate > 2)
         {  rubberband.begin(0);
            rubberband.add(RBAlignComposed,rbdata);
         }

    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(rbdata->gridstate);
    }

}

struct RBView33_data
{
   View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
   Point oldeye,oldref,oldup,olduaxis,oldvaxis;
   double uoffset,voffset;
};

void RBView33(int draw,void *data,int x,int y,View3dSurface *surface)
{
 RBView33_data *rbdata = (RBView33_data *) data;
 Transform t1,t2;
 double uangle,vangle;
 Point eye,ref,up;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    rbdata->last = surface;
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
       rbdata->lastselected = cadwindow->getcurrentwindow();
       /*
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
            else
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
         */
       if (rbdata->lastselected != 0)
         {  rbdata->oldeye = rbdata->lastselected->getview()->geteye();
            rbdata->oldref = rbdata->lastselected->getview()->getref();
            rbdata->oldup = rbdata->lastselected->getview()->getup();
            rbdata->olduaxis = rbdata->lastselected->getuaxis();
            rbdata->oldvaxis = rbdata->lastselected->getvaxis();
            rbdata->uoffset = rbdata->voffset = 0.0;
          }
    }
  if (rbdata->lastselected == NULL) return;
  if (draw == RB_TIMER)
    {  if (x >= 0 && x < 5)
         rbdata->voffset += M_PI / 16.0;
       if (x > surface->getwidth() - 5)
         rbdata->voffset -= M_PI / 16.0;
       if (y >= 0 && y < 5)
         rbdata->uoffset += M_PI / 16.0;
       if (y > surface->getheight() - 5)
         rbdata->uoffset -= M_PI / 16.0;
    }
  uangle = -((double) y / (double) surface->getheight() - 0.5) * M_PI * 2.0 + rbdata->uoffset;
  vangle = -((double) x / (double) surface->getwidth() - 0.5) * M_PI * 2.0 + rbdata->voffset;
  t1.rotate(rbdata->oldref,rbdata->olduaxis,uangle);
  t2.rotate(rbdata->oldref,rbdata->oldvaxis,vangle);
  t1.apply(t2);
  eye = t1.transform(rbdata->oldeye);
  ref = t1.transform(rbdata->oldref);
  up = t1.transform(rbdata->oldref + rbdata->oldup) - ref;
  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,up))
         {
            surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,up) &&
            surface->getview()->changeview(eye,ref,up))
         {
            ((View3dSurface *)rbdata->bitmap)->updatematrix();
             surface->updatematrix();
             rbdata->bitmap->paintevent();
             //BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
             //       rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
}

/**
 * @brief view_command33
 * @param cstate
 * @param data
 *
 * Dynamic view rotate single rotation
 *
 * KMJ : working in the Qt version
 *
 */
void view_command33(int *cstate,HighLevelEvent *,void **data)
{
#if 0
 // KMJ: pending
 RBView33_data *rbdata = (RBView33_data *) *data;
 ResourceString rs86(NCVIEWB+86),rs87(NCVIEWB+87),rs88(NCVIEWB+88);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView33_data) == NULL)
           {  state.fail(Memory,rs86.gets());
              *cstate = ExitState;
              break;
           }
         rbdata->bitmap = NULL;
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs87.gets(),rs88.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata->last = NULL;
              rbdata->lastselected = cadwindow->getcurrentwindow();
              if (rbdata->lastselected->getsurfacetype() == View2dSurf)
                rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
              else  
                rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
              if (! rbdata->bitmap->getdefined())
                {  delete rbdata->bitmap;
                   rbdata->bitmap = NULL;
                }
              rbdata->oldeye = rbdata->lastselected->getview()->geteye();
              rbdata->oldref = rbdata->lastselected->getview()->getref();
              rbdata->oldup = rbdata->lastselected->getview()->getup();
              rbdata->olduaxis = rbdata->lastselected->getuaxis();
              rbdata->oldvaxis = rbdata->lastselected->getvaxis();
              rbdata->uoffset = rbdata->voffset = 0.0;
              db.savezoomundo(1,cadwindow->getcurrentwindow());
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView33,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+89);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
#endif
}

struct RBView13_data
  {View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
   double walkdistance;
  };

void RBView13(int draw,void *data,int x,int y,View3dSurface *surface)
{RBView13_data *rbdata = (RBView13_data *) data;
 Transform t1;
 double stepsize,vangle;
 Point eye,ref,walk;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    rbdata->last = surface;
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
       rbdata->lastselected = cadwindow->getcurrentwindow();
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
    }
  if (rbdata->lastselected == NULL) return;
  stepsize = rbdata->walkdistance * (double(y) / double(surface->getheight()) - 0.5) * 5.0;
  vangle = ((double) x / (double) surface->getwidth() - 0.5) * M_PI / 50.0;
  t1.rotate(surface->getview()->geteye(),surface->getvaxis(),-vangle);
  eye = t1.transform(surface->getview()->geteye());
  ref = t1.transform(surface->getview()->getref());
  walk = (ref - eye).normalize() * stepsize;
  walk.z = 0.0;
  eye += walk;
  ref += walk;
  if (cadwindow->getshiftstate()) eye.z += rbdata->walkdistance / 3.0;
  if (cadwindow->getcontrolstate()) eye.z -= rbdata->walkdistance / 3.0;


  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,surface->getview()->getup()))
         {  surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,surface->getview()->getup()) &&
            surface->getview()->changeview(eye,ref,surface->getview()->getup()))
         {  ((View3dSurface *)rbdata->bitmap)->updatematrix();  surface->updatematrix();
            rbdata->bitmap->paintevent();
            BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
}

void view_command13(int *cstate,HighLevelEvent *,void **data)
{
#if 0
 // KMJ: pending
 RBView13_data *rbdata = (RBView13_data *) *data;
 Dialog *dialog;
 ResourceString rs90(NCVIEWB+90),rs91(NCVIEWB+91),rs92(NCVIEWB+92),rs93(NCVIEWB+93);
 ResourceString rs94(NCVIEWB+94),rs95(NCVIEWB+95);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView13_data) == NULL)
           {  state.fail(Memory,rs90.gets());
              *cstate = ExitState;
              break;
           }
         rbdata->bitmap = NULL;
         if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs91.gets(),rs92.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata->walkdistance = v.getreal("vw::walkdistance");
              if ((dialog = new Dialog(rs93.gets(),rs94.gets(),&rbdata->walkdistance,ScaleByLength)) == NULL)
                {  state.fail(Memory,rs95.gets());
                   *cstate = ExitState;
                }
              else if (dialog->process() == TRUE)
                {  v.setreal("vw::walkdistance",rbdata->walkdistance);
                   rbdata->last = NULL;
                   rbdata->lastselected = cadwindow->getcurrentwindow();
                   if (rbdata->lastselected->getsurfacetype() == View2dSurf)
                     rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
                   else  
                     rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
                   if (! rbdata->bitmap->getdefined())
                     {  delete rbdata->bitmap;
                        rbdata->bitmap = NULL;
                     }
                   db.savezoomundo(1,cadwindow->getcurrentwindow());
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView13,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+96);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
#endif
}

struct RBView14_data
  {View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
   double walkdistance;
  };

void RBView14(int draw,void *data,int x,int y,View3dSurface *surface)
{
#if 0
 // KMJ: pending
 RBView14_data *rbdata = (RBView14_data *) data;
 Transform t1,t2;
 double uangle,vangle;
 Point eye,ref,up,walk;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    rbdata->last = surface;
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
       rbdata->lastselected = cadwindow->getcurrentwindow();
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
    }
  if (rbdata->lastselected == NULL) return;
  uangle = ((double) y / (double) surface->getheight() - 0.5) * M_PI / 50.0;
  vangle = ((double) x / (double) surface->getwidth() - 0.5) * M_PI / 50.0;
  t1.rotate(surface->getview()->geteye(),surface->getuaxis(),uangle);
  t2.rotate(surface->getview()->geteye(),surface->getvaxis(),-vangle);
  t1.apply(t2);
  eye = t1.transform(surface->getview()->geteye());
  ref = t1.transform(surface->getview()->getref());
  up = t1.transform(surface->getview()->getref() + surface->getview()->getup()) - ref;
  walk = (ref - eye).normalize() * rbdata->walkdistance;
  eye += walk;
  ref += walk;

  if (cadwindow->getshiftstate()) eye.z += rbdata->walkdistance / 3.0;
  if (cadwindow->getcontrolstate()) eye.z -= rbdata->walkdistance / 3.0;

  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,up))
         {  surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,up) &&
            surface->getview()->changeview(eye,ref,up))
         {  ((View3dSurface *)rbdata->bitmap)->updatematrix();  surface->updatematrix();
            rbdata->bitmap->paintevent();
            BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
#endif
}

void view_command14(int *cstate,HighLevelEvent *,void **data)
{
#if 0
 // KMJ: pending
 RBView14_data *rbdata = (RBView14_data *) *data;
 Dialog *dialog;
 ResourceString rs97(NCVIEWB+97),rs98(NCVIEWB+98),rs99(NCVIEWB+99),rs100(NCVIEWB+100);
 ResourceString rs101(NCVIEWB+101),rs102(NCVIEWB+102);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView14_data) == NULL)
           {  state.fail(Memory,rs97.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->bitmap = NULL;
              if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
                {  cadwindow->MessageBox(rs98.gets(),rs99.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else
                {  rbdata->walkdistance = v.getreal("vw::walkdistance");
                   if ((dialog = new Dialog(rs100.gets(),rs101.gets(),&rbdata->walkdistance,ScaleByLength)) == NULL)
                     {  state.fail(Memory,rs102.gets());
                        *cstate = ExitState;
                     }
                   else if (dialog->process() == TRUE)
                     {  v.setreal("vw::walkdistance",rbdata->walkdistance);
                        rbdata->last = NULL;
                        rbdata->lastselected = cadwindow->getcurrentwindow();
                        if (rbdata->lastselected->getsurfacetype() == View2dSurf)
                          rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected,((View2dWindow *)rbdata->lastselected)->getview2d());
                        else  
                          rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected);
                        if (! rbdata->bitmap->getdefined())
                          {  delete rbdata->bitmap;
                             rbdata->bitmap = NULL;
                          }
                        *cstate = 1;
                        db.savezoomundo(1,cadwindow->getcurrentwindow());
                     }
                   else
                     *cstate = ExitState;
                }
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView14,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+103);
    }
  else if (*cstate == ExitState)
    {  if (rbdata->bitmap != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
#endif
}

/**
 * @brief view_command15
 * @param cstate
 *
 * Entry point for the Set Plot Scale command
 *
 * KMJ : working in Qt version
 *
 */
void view_command15(int *cstate,HighLevelEvent *,void **)
{double plotscale;
 ResourceString rs104(NCVIEWB+104),rs105(NCVIEWB+105),rs106(NCVIEWB+106),rs107(NCVIEWB+107);
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs104.gets(),rs105.gets(),MB_ICONINFORMATION);
  else
    {  db.savezoomundo(1,cadwindow->getcurrentwindow());
       plotscale = cadwindow->getcurrentsurface()->getplotscale();
       Dialog dialog(_RCT("PlotScale_Dialog"));

       dialog.add(new ScaleDialogControl(100,&plotscale));
       if (dialog.process() == TRUE)
         {  InvalidateRect(cadwindow->getcurrentwindow()->gethwnd(),0,TRUE);
            cadwindow->getcurrentsurface()->setplotscale(plotscale);
            if (((View3dWindow *)cadwindow->getcurrentsurface())->getoffscreenbitmap() != 0)
              ((View3dWindow *)cadwindow->getcurrentsurface())->getoffscreenbitmap()->displaylist.clear();
            v.setreal("vw::plotscale",plotscale);

            // update the view title text
            cadwindow->getcurrentwindow()->paintevent();
         }

    }
  *cstate = ExitState;
}

/**
 * @brief view_command16
 * @param cstate
 *
 * Reset view to STANDARD
 *
 * KMJ : working in Qt version
 */
void view_command16(int *cstate,HighLevelEvent *,void **)
{
 Dialog *dialog;
 int index;
 ResourceString rs0(NCVIEWA+0),rs1(NCVIEWA+1),rs2(NCVIEWA+2),rs3(NCVIEWA+3);
 ResourceString rs4(NCVIEWA+4),rs5(NCVIEWA+5),rs6(NCVIEWA+6),rs7(NCVIEWA+7);
 ResourceString rs8(NCVIEWA+8);
 RCCHAR *namelist[] = { rs0.gets(),rs1.gets(),rs2.gets(),rs3.gets(),rs4.gets(),rs5.gets(),rs6.gets(),rs7.gets(),rs8.gets() };
 ResourceString rs108(NCVIEWB+108),rs109(NCVIEWB+109),rs110(NCVIEWB+110);
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs108.gets(),rs109.gets(),MB_ICONINFORMATION);
  else if ((dialog = new Dialog(_RCT("ResetView_Dialog"))) == NULL)
    state.fail(Memory,rs110.gets());
  else
    {  for (index = 0 ; index < 9 ; index++)
         if (_stricmp(cadwindow->getcurrentwindow()->getview()->getname(),namelist[index]) == 0)
           break;
       if (index == 9)         
         index = 0;
       dialog->add(new ListDialogControl(100,9,namelist,&index));
       if (dialog->process() == TRUE)
         {  db.savezoomundo(1,cadwindow->getcurrentwindow());
            cadwindow->getcurrentsurface()->getview()->standard(index);
            cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
         }
       delete dialog;
    }
  *cstate = ExitState;
}


void view_command17(int *cstate,HighLevelEvent *,void **)
{
    cadwindow->setdepth();
    *cstate = ExitState;
}

class DVButtonDialogControl : public ButtonDialogControl
{private:
   RCCHAR **namelist;
   int nitems;
 public:
   DVButtonDialogControl(int id,RCCHAR **nm,int ni) : ButtonDialogControl(id)  {  namelist = nm;  nitems = ni;  }
   int select(Dialog *);
};

int DVButtonDialogControl::select(Dialog *dialog)
{
  int i,used,nselecteditems,*selecteditems;
  View *v;
  View2d *view2d;
  OneView *ov;
  ResourceString rs111(NCVIEWB+111),rs112(NCVIEWB+112),rs113(NCVIEWB+113),rs114(NCVIEWB+114),rs115(NCVIEWB+115);
  RCCHAR message[300],name[300];
 
  nselecteditems = SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELCOUNT,0,0);
  selecteditems = new int[nselecteditems];
  SendDlgItemMessage(dialog->gethdlg(),100,LB_GETSELITEMS,nselecteditems,(LPARAM)selecteditems);
  for (i = 0 ; i < nselecteditems ; i++)
  {
       SendDlgItemMessage(dialog->gethdlg(),100,LB_GETTEXT,selecteditems[i],(LPARAM)name);
       if ((v = db.views3d.match(name)) != NULL)
       {
            if (cadwindow->viewused(v->getviewno()))
              cadwindow->MessageBox(rs112.gets(),rs113.gets(),MB_ICONINFORMATION);
            else
            {
                used = 0;
                for (i = 0 ; i < MaxViews ; i++)
                if ((view2d = db.views2d.match(i)) != 0)
                {
                   for (view2d->start() ; (ov = view2d->next()) != NULL ; )
                      if (strcmp(ov->getview()->getname(),name) == 0)
                        used = 1;
                }
                if (used)
                cadwindow->MessageBox("This view is used in a composed drawing and so it cannot be deleted.","Delete View",MB_OK);
                else
                {
                    sprintf(message,rs114.gets(),name);
                    if (cadwindow->MessageBox(message,rs115.gets(),MB_YESNO) == IDYES)
                    {
                        db.views3d.del(v);
                        delete v;
                    }
                }
            }
       }
  }

  return 101;
}


/**
 * @brief view_command18
 * @param cstate
 *
 * Entry point for the Delete 3d View command
 *
 *
 */
void view_command18(int *cstate,HighLevelEvent *,void **)
{int n,index;
 RCCHAR **namelist;
 Dialog dialog(_RCT("DeleteView_Dialog"));

  index = 0;
  db.views3d.names(&namelist,&n,&index);
  if (namelist != NULL)
    {  dialog.add(new ListDialogControl(100,n,namelist,0,0));
       dialog.add(new DVButtonDialogControl(101,namelist,n));
       dialog.process();
       delete namelist;
    }

  *cstate = ExitState;
}

/**
 * @brief view_command34
 * @param cstate
 * @param event
 *
 * Entry point for the Delete 3d view on composed drawing command
 */
void view_command34(int *cstate,HighLevelEvent *event,void **)
{static int gridstate;
 View2dWindow *window;
 View3dSubSurface *ss;
 ResourceString rs294(NCMAIN+294),rs163(NCVIEWB+163);
  switch (*cstate)
    {case InitialState :
       gridstate = db.grid.getsnap();
       db.grid.setsnap(0);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((ss = window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   {  db.saveundo(UD_CHANGEVIEW2D,window->getview2d());
                      window->getview2d()->del(ss->getoneview());
                      window->update();
                      window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                   }
                 else
                   cadwindow->MessageBox(rs163.gets(),rs163.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs163.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+164);
       cadwindow->setdragviewboundary(0);
    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(gridstate);
    }
}

/**
 * @brief view_command35
 * @param cstate
 * @param event
 *
 *  Entry point the pan a viewport in a composed view command
 */
void view_command35(int *cstate,HighLevelEvent *event,void **)
{static View2dWindow *window;
 static View3dSubSurface *ss;
 static Point p1;
 static int gridstate;
 Point p2;
 double scale;
 ResourceString rs295(NCVIEWB+177),rs163(NCVIEWB+163);
 double cosa,sina;
 Point uv1,uv2;

  switch (*cstate)
    {case InitialState :
       gridstate = db.grid.getsnap();
       db.grid.setsnap(0);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((ss = window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   *cstate = 2;
                 else
                   cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *)event)->getp();
            cadwindow->rubberbanduv(p1);
            cadwindow->cursoraction(1);
            *cstate = 3;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       cadwindow->cursoraction(0);
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *)event)->getp();
            db.saveundo(UD_CHANGEVIEW2D,window->getview2d());

            cosa = cos(ss->getoneview()->getangle()*acos(-1.0)/180.0);
            sina = sin(ss->getoneview()->getangle()*acos(-1.0)/180.0);

            uv1.x =  cosa * p1.x + sina * p1.y;
            uv1.y = -sina * p1.x + cosa * p1.y;

            uv2.x =  cosa * p2.x + sina * p2.y;
            uv2.y = -sina * p2.x + cosa * p2.y;

            ss->regenerate();

            ss->getoneview()->setposition(ss->getoneview()->getxpos()+p2.x-p1.x,ss->getoneview()->getypos()+p2.y-p1.y);
            scale = ss->getoneview()->getscale();
            ss->getoneview()->setclip(ss->getoneview()->getumin()+(uv1.x-uv2.x)*scale,ss->getoneview()->getvmin()+(uv1.y-uv2.y)*scale,
                                      ss->getoneview()->getumax()+(uv1.x-uv2.x)*scale,ss->getoneview()->getvmax()+(uv1.y-uv2.y)*scale);
            window->update();
            window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1 || *cstate == 2 || *cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB + 164 + *cstate);
       cadwindow->setdragviewboundary(0);
    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(gridstate);
    }

}

/**
 * @brief view_command41
 * @param cstate
 * @param event
 *
 * Entry point for Move viewport on a composed view
 *
 */
void view_command41(int *cstate,HighLevelEvent *event,void **)
{static View2dWindow *window;
 static View3dSubSurface *ss;
 static Point p1;
 static int gridstate;
 Point p2;
 ResourceString rs295(NCVIEWB+177),rs163(NCVIEWB+163);

  switch (*cstate)
    {case InitialState :
       gridstate = db.grid.getsnap();
       db.grid.setsnap(0);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((ss = window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   {  *cstate = 2;
                   }
                 else
                   cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *)event)->getp();
            cadwindow->rubberbanduv(p1);
            cadwindow->cursoraction(1);
            *cstate = 3;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       cadwindow->cursoraction(0);
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *)event)->getp();
            db.saveundo(UD_CHANGEVIEW2D,window->getview2d());
            ss->regenerate();
            ss->getoneview()->setposition(ss->getoneview()->getxpos()+p2.x-p1.x,ss->getoneview()->getypos()+p2.y-p1.y);
            window->update();
            window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate a HANDLE of the viewport to MOVE"));
       cadwindow->setdragviewboundary(0);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate the position on the layout drawing view to move"));
       cadwindow->setdragviewboundary(0);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate the new position of the layout drawing view"));
       cadwindow->setdragviewboundary(0);
    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(gridstate);
    }

}

/**
 * @brief view_command36
 * @param cstate
 * @param event
 *
 * Entry point for the Change 3d view on a composite drawing command
 *
 */
void view_command36(int *cstate,HighLevelEvent *event,void **)
{View2dWindow *window;
 View3dSubSurface *ss;
 ResourceString rs296(NCVIEWB+180),rs163(NCVIEWB+163);
 static int gridstate;
  
  switch (*cstate)
    {case InitialState :
       gridstate = db.grid.getsnap();
       db.grid.setsnap(0);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((ss = window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   {  if (ss->getoneview()->change(&db,window->getview2d()))
                        {  ss->setplotscale(ss->getoneview()->getscale());
                           ss->regenerate();
                           window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                        }
                   }
                 else
                   cadwindow->MessageBox(rs163.gets(),rs296.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs296.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+168);
       cadwindow->setdragviewboundary(0);
    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(gridstate);
    }
}

/**
 * @brief view_command38
 * @param cstate
 * @param event
 *
 * Entry point for the Fit a 3D view in a layout viewport command
 *
 */
void view_command38(int *cstate,HighLevelEvent *event,void **)
{
 static View2dWindow *window;
 View3dSubSurface *ss;
 Point pmin,pmax;
 double scale,scale1,du,dv,cosa,sina,ox,oy;
 ResourceString rs295(NCVIEWB+176),rs163(NCVIEWB+163);
 static int gridstate;

 switch (*cstate)
    {case InitialState :
       *cstate = 1;
       gridstate = db.grid.getsnap();
       db.grid.setsnap(0);
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  if (((Coordinate3dEvent *)event)->getwindow() != 0 &&
                ((Coordinate3dEvent *)event)->getwindow()->getsurfacetype() == View2dSurf)
              {  window = (View2dWindow *) ((Coordinate3dEvent *)event)->getwindow();
                 if ((ss = window->pick(((Coordinate3dEvent *)event)->getp())) != 0)
                   {  db.saveundo(UD_CHANGEVIEW2D,window->getview2d());
                      program->setbusy(1);
                      db.geometry.extents(ss,&pmin,&pmax);
                      if (! program->getinterrupt())
                        {  if (pmax.x - pmin.x > db.getptoler() && pmax.y - pmin.y > db.getptoler())
                             {  du = pmax.x - pmin.x;
                                dv = pmax.y - pmin.y;
                                scale = ss->getoneview()->getscale();
                                if (du / dv > (ss->getoneview()->getumax() - ss->getoneview()->getumin()) / (ss->getoneview()->getvmax() - ss->getoneview()->getvmin()))
                                  dv = du / (ss->getoneview()->getumax() - ss->getoneview()->getumin()) * (ss->getoneview()->getvmax() - ss->getoneview()->getvmin());
                                else
                                  du = dv * (ss->getoneview()->getumax() - ss->getoneview()->getumin()) / (ss->getoneview()->getvmax() - ss->getoneview()->getvmin());
                                scale1 = scale * du / (ss->getoneview()->getumax() - ss->getoneview()->getumin());
                                ss->getoneview()->setscale(scale1);

                                cosa = cos(ss->getoneview()->getangle()*acos(-1.0)/180.0);
                                sina = sin(ss->getoneview()->getangle()*acos(-1.0)/180.0);

                                ox = ss->getoneview()->getxpos() + cosa * (ss->getoneview()->getumin()+ss->getoneview()->getumax()) / 2.0 / scale - sina * (ss->getoneview()->getvmin()+ss->getoneview()->getvmax()) / 2.0 / scale;
                                oy = ss->getoneview()->getypos() + sina * (ss->getoneview()->getumin()+ss->getoneview()->getumax()) / 2.0 / scale + cosa * (ss->getoneview()->getvmin()+ss->getoneview()->getvmax()) / 2.0 / scale;

                                ox +=  - ((pmin.x+pmax.x) / 2.0 / scale1 * cosa - (pmin.y+pmax.y) / 2.0 / scale1 * sina);
                                oy +=  - ((pmin.x+pmax.x) / 2.0 / scale1 * sina + (pmin.y+pmax.y) / 2.0 / scale1 * cosa);

                                ss->getoneview()->setposition(ox,oy);
                                ss->getoneview()->setclip((pmin.x+pmax.x)/2.0-du/2.0,(pmin.y+pmax.y)/2.0-dv/2.0,
                                                          (pmin.x+pmax.x)/2.0+du/2.0,(pmin.y+pmax.y)/2.0+dv/2.0);
                                window->update();
                                window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                                ss->regenerate();
                                ss->setplotscale(ss->getoneview()->getscale());
                                ss->sizeevent(0,0);

                             }
                        }
                      program->setbusy(0);
          			     }
                else
                  cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
              }
            else
              cadwindow->MessageBox(rs163.gets(),rs295.gets(),MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB + 171);
       cadwindow->setdragviewboundary(0);
    }
  else
    {  cadwindow->setdragviewboundary(1);
       db.grid.setsnap(gridstate);
    }
}

/**
 * @brief view_command43
 * @param cstate
 * @param event
 *
 * Entry point for the copy layout command
 *
 */
void view_command43(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog(_RCT("CopyComposedDrawing_Dialog"));
 int index,viewno,n,j;
 RCCHAR **namelist,newname[300],oldname[300];
 ResourceString rs130(NCVIEWB+130),rs131(NCVIEWB+131),rs132(NCVIEWB+132);
 View2d *view2d,*oldview2d;
 OneView *oldov,*newov;
 Entity *copye,*e;
 BitMask options(32),*lt;
 Transform t;

  db.views2d.names(&namelist,&n,&index);
  strcpy(newname,"");
  strcpy(oldname,"");
  if (namelist != NULL)
    {  dialog.add(new StringDialogControl(100,newname,300));
       dialog.add(new ListDialogControl(101,n,namelist,oldname,300));
       if (dialog.process() == TRUE && (oldview2d = db.views2d.match(oldname)) != 0)
		       {  if (db.views2d.match(newname) != NULL)
              cadwindow->MessageBox(rs131.gets(),rs130.gets(),MB_ICONINFORMATION);
            else if ((viewno = db.getnextviewno()) < 0)
              cadwindow->MessageBox(rs132.gets(),rs130.gets(),MB_ICONINFORMATION);
            else if ((view2d = new View2d(newname,oldview2d->getpaperwidth(),oldview2d->getpaperheight(),short(viewno))) != NULL)
              {  
                 for (oldview2d->start() ; (oldov = oldview2d->next()) != 0 ; )
                   {  options.set(0,oldov->getdashhidden());
                      options.set(1,oldov->gethiddenlines());
                      options.set(2,oldov->getshaded());
                      if (oldov->getlayertable() == 0)
                        lt = 0;
                      else
                        {  lt = new BitMask(MAX_LAYERS);
                           *lt = *oldov->getlayertable();
                        }
                      newov = new OneView(oldov->getview(),oldov->getumin(),oldov->getvmin(),oldov->getumax(),oldov->getvmax(),
                                          oldov->getscale(),oldov->getxpos(),oldov->getypos(),oldov->getangle(),options,lt);
                      view2d->add(newov);
                   }

	             			//  Copy any entities that are just part of this composed drawing
                 for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                   e->setcopye(0);

                 t.identity();

                 for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                   {  for (j = 0 ; j < MaxViews ; j++)
                        if (e->getvisible()->test(j) && j != oldview2d->getviewno() ||
                            ! e->getvisible()->test(j) && j == oldview2d->getviewno())
                          break;
                      if (j == MaxViews && e->getcopye() == 0)
                        {  copye = e->clone(t); 
                           copye->getvisible()->set(oldview2d->getviewno(),0);
                           copye->getvisible()->set(viewno,1);
                           db.geometry.add(copye);                           
                        } 
                   }

                 db.views2d.add(view2d);
                 cadwindow->create2dsubwindow(view2d);


              }

         }

       delete [] namelist;

	   }
  *cstate = ExitState;
}


#if defined(VRINTERFACE)
#ifndef _USING_QT
#include "vfx195.h"
#include <regstr.h>
#include <winreg.h>
//#include <mmddk.h>
#endif

static DWORD HMDId;
static DWORD HANDId;

#define STR_LEN 256
//
//   FUNCTION: CheckDeviceStr( dword joyid, lpbyte str )
//
//   PURPOSE: Determine if the joystick id's device string contains <str>
//
//   COMMENTS:
//
UINT	CheckDeviceStr( DWORD JoyId, LPBYTE ChkStr )
{
UINT		cbjc, joydev;
JOYCAPS		pjc;
MMRESULT	ret;
BYTE		IdStr[STR_LEN];

	joydev = joyGetNumDevs();
	if( JoyId >= joydev )
		return( 0 );
	else {
		cbjc = sizeof( JOYCAPS ); 
		ret = joyGetDevCaps( JoyId, &pjc, cbjc );
		
		if( !ret ) {
			GetStringFromRegistry( JoyId, pjc.szRegKey, IdStr, STR_LEN );
			if( strstr( (const char *)IdStr, (const char *)ChkStr ) != NULL )
				return(1); // Found an HMD
			}
		}
	return( 0 );
}

/* =========================================================================
*
   Function Name: GetStringFromRegistry
   Description  : 
   Parameters   : 
   Notes        : 
 * =========================================================================
*/
#define STR_LEN 256
BOOL GetStringFromRegistry(int id, char *regKey, unsigned char *string,int)
{
	HKEY	hKey;
	int		returnVal;
	DWORD	valueType, valueSize;
	char registryKeyString[STR_LEN];
	char registryString[STR_LEN];

	sprintf(registryKeyString, REGSTR_PATH_JOYCONFIG "\\%s\\"REGSTR_KEY_JOYCURR, regKey);
	returnVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, registryKeyString, 0,KEY_QUERY_VALUE, &hKey);
	if ( returnVal != ERROR_SUCCESS ){
		return (FALSE);
		}

	sprintf(registryKeyString, REGSTR_VAL_JOYNOEMNAME, id+1);
	valueSize = sizeof(registryString);
	valueType = REG_SZ;
	returnVal = RegQueryValueEx(hKey, registryKeyString, NULL, 
				(LPDWORD)&valueType, (LPBYTE)registryString, &valueSize); 
	if ( returnVal == ERROR_SUCCESS ){
		RegCloseKey(hKey);
		sprintf(registryKeyString, REGSTR_PATH_JOYOEM "\\%s", registryString);
		returnVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, registryKeyString, 0,KEY_QUERY_VALUE, &hKey);
		if ( returnVal == ERROR_SUCCESS ){
			valueSize = sizeof(registryString);
			returnVal = RegQueryValueEx(hKey, REGSTR_VAL_JOYOEMNAME, NULL,
					(LPDWORD)&valueType,(LPBYTE)registryString, &valueSize);
			if ( returnVal == ERROR_SUCCESS ){
// Now Verify registryString...				
				strcpy( (char *)string, (char *)registryString );
				}
			}
		}
	RegCloseKey(hKey);

	return ( (returnVal==ERROR_SUCCESS) ? TRUE : FALSE );
}

// Find out if a Headset is connected via direct Input system.
//
UINT FindHMDDevice()
{ for (HMDId = 0 ; HMDId < 16 ; HMDId++)
	 if (CheckDeviceStr(HMDId,(unsigned char *)"Tracker"))
      break;
  if (HMDId == 16)
    return 0;
  for (HANDId = 0 ; HANDId < 16 ; HANDId++)
	 if (CheckDeviceStr(HANDId,(unsigned char *)"CYBERPUCK"))
      break;
  if (HANDId == 16)
    return 0;

  return 1;
}

//
//   FUNCTION: HMDPoll( dword HMDId, dword *Roll, dword *Pitch, dword *Yaw )
//
//   PURPOSE: Poll an assigned VFX1 Joystick.
//
//   COMMENTS:
//
VOID	HMDPoll(DWORD HMDId,double *roll,double *pitch,double *yaw,int *buttons)
{JOYINFO	pji;
  if (! joyGetPos(HMDId,(LPJOYINFO)&pji))
    {  *yaw   = ((double)pji.wXpos - 32768.0) / 32768.0 * 180.0;
		 *pitch = ((double)pji.wYpos - 32768.0) / 32768.0 * 180.0;
		 *roll  = ((double)pji.wZpos - 32768.0) / 32768.0 * 180.0;
       if (! joyGetPos(HANDId,(LPJOYINFO)&pji))
         *buttons = pji.wButtons;
       else
         *buttons = 0;
    }
  else
    {  *yaw   = 0;
  		 *pitch = 0;
	    *roll  = 0;
       *buttons = 0;
	 }
}

struct RBView37_data
  {View3dSurface *last;
   View3dWindow *lastselected;
   View3dOffScreenBitmap *bitmap;
   double walkdistance;
   Point eye,ref,up;
  };

void RBView37(int draw,void *data,int,int,View3dSurface *surface)
{RBView37_data *rbdata = (RBView37_data *) data;
 Transform t1,t2,t3;
 Point eye,ref,up,walk,xaxis,zaxis(0.0,0.0,1.0);
 double roll,pitch,yaw;
 int buttons;
  if (draw == RB_TIMER)
    surface = rbdata->last;
  else if (draw == RB_DRAW)
    rbdata->last = surface;
  if (draw == RB_ERASE || cadwindow->getcurrentsurface() != surface || surface == 0) return;
  if (rbdata->lastselected != cadwindow->getcurrentwindow())
    {  if (rbdata->lastselected != NULL)
         cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
       rbdata->lastselected = cadwindow->getcurrentwindow();
       delete rbdata->bitmap;  rbdata->bitmap = NULL;
       if (rbdata->lastselected != NULL)
         {  if (rbdata->lastselected()->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected));
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected));
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = NULL;
              }
         }
       if (rbdata->lastselected != 0)
         {  rbdata->eye = rbdata->lastselected->getview()->geteye();
            rbdata->ref = rbdata->lastselected->getview()->getref();
            rbdata->ref.z = rbdata->eye.z;
            rbdata->up = rbdata->lastselected->getview()->getup();
         }
    }
  if (rbdata->lastselected == NULL) return;

  HMDPoll(HMDId,&roll,&pitch,&yaw,&buttons);
  char message[300];
  sprintf(message,"Roll %.2lf Pitch %.2lf Yaw %.2lf Buttons %d",roll,pitch,yaw,buttons);
  cadwindow->prompt->normalprompt(message);

//  stepsize = rbdata->walkdistance * (double(y) / double(surface->getheight()) - 0.5) * 5.0;
//  vangle = ((double) x / (double) surface->getwidth() - 0.5) * M_PI / 50.0;
//  t1.rotate(surface->getview()->geteye(),surface->getvaxis(),-vangle);
//  eye = t1.transform(surface->getview()->geteye());
//  ref = t1.transform(surface->getview()->getref());
//  eye += walk;
//  ref += walk;
//  if (cadwindow->getshiftstate()) eye.z += rbdata->walkdistance / 3.0;
//  if (cadwindow->getcontrolstate()) eye.z -= rbdata->walkdistance / 3.0;

  xaxis = ((rbdata->ref - rbdata->eye).cross(zaxis)).normalize();

  t1.rotate(rbdata->eye,xaxis,pitch * M_PI / 180.0);
  t2.rotate(rbdata->eye,zaxis,yaw * M_PI / 180.0);
  t3.rotate(rbdata->eye,(rbdata->ref - rbdata->eye).normalize(),roll * M_PI / 180.0);
  eye = t1.transform(rbdata->eye);
  eye = t2.transform(eye);
  ref = t1.transform(rbdata->ref);
  ref = t2.transform(ref);
  up = t3.transform(rbdata->eye + rbdata->up);
  up = t1.transform(up);
  up = t2.transform(up) - rbdata->eye;
  walk = (ref - eye);
  walk.z = 0.0;
  walk = walk.normalize() * rbdata->walkdistance;
  if (buttons & 1)
    {  eye += walk;
       ref += walk;
       rbdata->eye += walk;
       rbdata->ref += walk;
    }
  else if (buttons & 2)
    {  eye -= walk;
       ref -= walk;
       rbdata->eye -= walk;
       rbdata->ref -= walk;
    }

  if (rbdata->bitmap == NULL)
    {  if (surface->getview()->changeview(eye,ref,up))
         {  surface->updatematrix();
            cadwindow->getcurrentwindow()->paintevent();
         }
    }
  else
    {  if (((View3dSurface *)rbdata->bitmap)->getview()->changeview(eye,ref,up) &&
            surface->getview()->changeview(eye,ref,up))
         {  ((View3dSurface *)rbdata->bitmap)->updatematrix();  surface->updatematrix();
            rbdata->bitmap->paintevent();
            BitBlt(surface->gethdc(),0,0,int(surface->getwidth()),int(surface->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
         }
    }
}

void view_command37(int *cstate,HighLevelEvent *,void **data)
{RBView37_data *rbdata = (RBView37_data *) *data;
 Dialog *dialog;
 ResourceString rs90(NCVIEWB+90),rs91(NCVIEWB+91),rs92(NCVIEWB+92),rs93(NCVIEWB+93);
 ResourceString rs94(NCVIEWB+94),rs95(NCVIEWB+95);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBView37_data) == NULL)
           {  state.fail(Memory,rs90.gets());
              *cstate = ExitState;
              break;
           }
         rbdata->bitmap = NULL;
         if (! FindHMDDevice())
           {  cadwindow->MessageBox("VR devices could not be found.","VR view.",MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
           {  cadwindow->MessageBox(rs91.gets(),rs92.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata->walkdistance = v.getreal("vw::walkdistance");
              if ((dialog = new Dialog(rs93.gets(),rs94.gets(),&rbdata->walkdistance)) == NULL)
                {  state.fail(Memory,rs95.gets());
                   *cstate = ExitState;
                }
              else if (dialog->process() == TRUE)
                {  v.setreal("vw::walkdistance",rbdata->walkdistance);
                   rbdata->last = NULL;
                   rbdata->lastselected = cadwindow->getcurrentwindow();
                   if (rbdata->lastselected()->getsurfacetype() == View2dSurf)
                     rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->lastselected));
                   else  
                     rbdata->bitmap = new View3dOffScreenBitmap(rbdata->lastselected));
                   if (! rbdata->bitmap->getdefined())
                     {  delete rbdata->bitmap;
                        rbdata->bitmap = NULL;
                     }
                   rbdata->eye = rbdata->lastselected->getview()->geteye();
                   rbdata->ref = rbdata->lastselected->getview()->getref();
                   rbdata->up = rbdata->lastselected->getview()->getup();
                   rbdata->ref.z = rbdata->eye.z;
                   db.savezoomundo(1,cadwindow->getcurrentwindow());
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->lastselected != NULL)
           cadwindow->updatewindows(((View3dSurface *)rbdata->lastselected)->getview());
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBView37,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+96);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
}

#else
void view_command37(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
}
#endif


struct switch_info_t
{HMONITOR hMonitor;
 WINDOWPLACEMENT wp;
 MONITORINFO mi;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
{switch_info_t *si = (switch_info_t *)dwData;
 MONITORINFO mi;
 int dx,dy;

    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor,&mi);
    if (si->hMonitor != hMonitor)
    {   dx = mi.rcMonitor.left - si->mi.rcMonitor.left;
        dy = mi.rcMonitor.top - si->mi.rcMonitor.top;
        si->wp.rcNormalPosition.left = si->wp.rcNormalPosition.left + dx;
        si->wp.rcNormalPosition.right = si->wp.rcNormalPosition.right + dx;
        si->wp.rcNormalPosition.top = si->wp.rcNormalPosition.top + dy;
        si->wp.rcNormalPosition.bottom = si->wp.rcNormalPosition.bottom + dy;
        si->wp.showCmd = SW_NORMAL;
        SetWindowPlacement(cadwindow->gethwnd(),&si->wp);
        si->wp.showCmd = SW_MAXIMIZE;
        SetWindowPlacement(cadwindow->gethwnd(),&si->wp);

        return FALSE;

    }

    return TRUE;
}


void view_switch_screens_command(int *cstate,HighLevelEvent *,void **)
{switch_info_t si;

    si.wp.length = sizeof(si.wp);
    GetWindowPlacement(cadwindow->gethwnd(),&si.wp);

    si.mi.cbSize = sizeof(si.mi);
    si.hMonitor = MonitorFromRect(&si.wp.rcNormalPosition,0);
    GetMonitorInfo(si.hMonitor,&si.mi);

    EnumDisplayMonitors(0,0,MonitorEnumProc,(LPARAM)&si);

    *cstate = ExitState;

}

/**
 * @brief view_command44
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the position viewer command
 *
 */
void view_command44(int *cstate,HighLevelEvent *event,void **data)
{
    *cstate = ExitState;

}

/**
 * @brief view_command45
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Look around command
 *
 */
void view_command45(int *cstate,HighLevelEvent *event,void **data)
{
    *cstate = ExitState;

}

/**
 * @brief view_command46
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the section plane command
 *
 */
void view_command46(int *cstate,HighLevelEvent *event,void **data)
{
    *cstate = ExitState;

}

/**
 * @brief view_command47
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Previous view state command
 * This just calls the to previous zoom command
 */
void view_command47(int *cstate,HighLevelEvent *event,void **data)
{
    // 2107,1,
    // calls - zoom_command7()
    int commandid = 2107;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
    }
    *cstate = ExitState;
}

/**
 * @brief view_command48
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the toggle to/from full screen mode view command
 * used by the view full screen button
 */
void view_command48(int *cstate,HighLevelEvent *event,void **data)
{
    if(((QtMainWindow*)cadwindow->gethwnd())->isFullScreen())
        ((QtMainWindow*)cadwindow->gethwnd())->showMaximized();
    else
        ((QtMainWindow*)cadwindow->gethwnd())->showFullScreen();
    *cstate = ExitState;
}
