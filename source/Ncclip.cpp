	
#include "ncwin.h"

/**
 * @brief undo_command
 * @param cstate
 *
 * The undo command entry point
 *
 * KMJ : working in Qt version
 *
 */
void undo_command(int *cstate,HighLevelEvent *,void **)
{
#ifdef NEW_DRAGGING
    if(cadwindow)
        cadwindow->startdrawingbatch();
    db.undo();
    if(cadwindow)
        cadwindow->enddrawingbatch();
#else
  db.undo();
#endif
  *cstate = ExitState;
}

/**
 * @brief redo_command
 * @param cstate
 *
 * The redo command entry point
 *
 * KMJ : Working in Qt version
 */
void redo_command(int *cstate,HighLevelEvent *,void **)
{
#ifdef NEW_DRAGGING
    if(cadwindow)
        cadwindow->startdrawingbatch();
    db.redo();
    if(cadwindow)
        cadwindow->enddrawingbatch();
#else
  db.redo();
#endif
  *cstate = ExitState;
}

/**
 * @brief cut_command
 * @param cstate
 * @param event
 * @param data
 *
 * The Cut/Copy command entry point
 *
 * KMJ : Working in Qt version
 *
 */
void cut_command(int *cstate,HighLevelEvent *event,void **data)
{ if (*cstate == 1)
    *cstate = 2;
  copy_command(cstate,event,data);
  if (*cstate == 3)
    delete_command(cstate,event,data);
}


void AddGroup(EntityList *list,Group *g)
{EntityList glist;
 Entity *ge;

  glist = g->getlist();
  for (glist.start() ; (ge = glist.next()) != 0 ; )
    if (ge->getstatus() == 0)
      {  list->add(ge);
         ge->setstatus(1);

         if (ge->isa(group_entity) && ((Group *)ge)->deletechildren())
           AddGroup(list,(Group *)ge);

      }

}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief delete_command
 * @param cstate
 * @param hle
 * @param data
 *
 * The delete command
 *
 * KMJ : Working in Qt version
 *
 */
void delete_command(int *cstate,HighLevelEvent *hle,void **data)
{EntityList list,glist,free,others,temp;
 Entity *e,*wall,*ge;
 long n;
 int i,visible;
 ResourceString rs0(NCCLIP);
 ResourceString rsnc0(NC);
  *cstate = ExitState;
  list = state.getselection().copy();
  if (! list.empty())
    {  db.saveundo(UD_STARTBLOCK,NULL);
       db.geometry.clearstatus();
       for (list.start() ; (e = list.next()) != 0 ; )
         e->setstatus(1);
       for (list.start() ; (e = list.next()) != 0 ; )
         // v8 change. always delete the members of the group so ignore the deletechildren option
         //if (e->isa(group_entity) && ((Group *)e)->deletechildren())
         if (e->isa(group_entity))
           {  AddGroup(&list,(Group *)e);
           }
         else if (e->isa(figure_entity))
           {  glist = ((Figure *)e)->getunpacked();
              for (glist.start() ; (ge = glist.next()) != 0 ; )
                if (ge->getstatus() == 0)
                  {  list.add(ge);
                     ge->setstatus(1);
                  }
           }
         else if (e->isa(plane_entity))
           {  glist = ((Plane *)e)->getlist();
              for (glist.start() ; (ge = glist.next()) != 0 ; )
                {  visible = 0;
                   for (i = 0 ; i < MaxViews ; i++)
                     if (ge->getvisible()->test(i))
                       {  visible = 1;
                          break;
                       }
                   if (! visible)
                     {  list.add(ge);
                        ge->setstatus(1);
                     }
                }
           }

       RemoveLockedEntities(&list,1);

       cadwindow->startdrawingbatch();

       do {
           for (list.start() ;  (e = list.next()) != NULL ; )
           {
               //qDebug() << "sel ent type" << e->type();
               if (e->getalist().empty())
                 free.add(e);
               else
                 {  if (e->isa(figure_entity) && e->getalist().length() == 1)
                      {  temp = e->getalist();
                         temp.start();
                         if ((wall = temp.next())->isa(wall_entity) && ! list.inlist(wall))
                           free.add(e);
                         else
                           others.add(e);
                      }
                    else
                      others.add(e);
                 }
           }
             for (free.start(),n = 0 ; (e = free.next()) != NULL ; n++)
               {  if (e->isa(wall_entity))
                    {Entity *figure;
                     EntityList olist;
                       olist = ((Wall *)e)->getopeninglist();
                       for (olist.start() ; (figure = olist.next()) != 0 ; )
                         others.del(figure);
#if (VERSION != FLEXICAD)
                       for (db.geometry.start() ; (wall = db.geometry.next()) != 0  ; )
                         if (wall->isa(wall_entity) && ! wall->getstatus())
                           ((Wall *)wall)->unbreakwall((Wall *)e);
#endif
                    }
                  db.geometry.del(e,data == 0 || ((LONG)*data) == 0);
               }
             list.destroy();  free.destroy();
             list = others.copy();
             others.destroy();
          } while (n > 0);
       db.saveundo(UD_ENDBLOCK,NULL);
       if (hle != 0 && ! list.empty())
         {
           cadwindow->MessageBox(rs0.gets(),rsnc0.gets(),MB_ICONINFORMATION);
         }
       state.setselection(list,3);

       cadwindow->enddrawingbatch();


    }
}

/**
 * @brief copy_command
 * @param cstate
 * @param event
 *
 * Copy/Cut (paste) Command
 *
 * KMJ : Working in Qt version
 * TODO : think about making a custom QMimeData class for RealCAD modeldata
 *
 */
void copy_command(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
#ifdef USING_WIDGETS
    //QByteArray *clipdata;
#else
    HGLOBAL clipdata;
#endif
 char *cd;
 RCCHAR cbfilename[300];
 long fsize;
 FILE *infile;
 short type;
 int fstatus;
 CadFile cadfile;
  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       state.destroyselection(0);
       if (! list.empty())
         *cstate = 1;
       else
         *cstate = ExitState;
       break;
     case 1 :
     case 2 :
       if (event->isa(Coordinate3d))
         {
           qDebug() << "clip file name : " << QString(home.gettempfilename(_RCT("clipb")));
           strcpy(cbfilename,home.gettempfilename(_RCT("clipb")));
           if (cadfile.open(cbfilename,"wb"))
              {
                 type = origin_record;  cadfile.write(&type);
                 ((Coordinate3dEvent *)event)->getp().save(&cadfile);
                 db.geometry.save(&cadfile,list);
                 type = 999;
                 cadfile.write(&type);
                 fstatus = cadfile.close();
                 infile = fopen(cbfilename,"rb");
                 fseek(infile,0L,2);
                 fsize = ftell(infile);
                 fseek(infile,0L,0);
#ifdef USING_WIDGETS
                 QClipboard *clipboard = QApplication::clipboard();
                 if(clipboard)
                 {
                     cd = new char[fsize];
                     if(cd != 0)
                     {
                         if(fread(cd,(size_t)fsize,1,infile))
                         {
                             clipboard->clear();
                             QByteArray clipdata(cd,fsize);
                             QMimeData *mimedata = new QMimeData;
                             mimedata->setData("application/ISCad3d",clipdata);
                             clipboard->setMimeData(mimedata);
                         }
                     }
                     delete [] cd;
                 }
#else
                 if (OpenClipboard(cadwindow->gethwnd()))
                   {  clipdata = GlobalAlloc(0,fsize);
                      if (clipdata != NULL)
                        {  if ((cd = (RCCHAR *) GlobalLock(clipdata)) != NULL)
                             {  fread(cd,(size_t)fsize,1,infile);
                                GlobalUnlock(clipdata);
                                EmptyClipboard();
                                SetClipboardData(RegisterClipboardFormat(TEXT("ISCad3d")),clipdata);
                             }
                        }
                      CloseClipboard();
                   }
#endif
                 fclose(infile);
                 _unlink(cbfilename);
              }
         }
       state.setselection(list,1);
       if (*cstate == 2)
         *cstate = 3;
       else
         *cstate = ExitState;
    }
  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCCLIP+1);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
}

/**
 * @brief paste_command
 * @param cstate
 * @param event
 *
 * The paste command entry point
 *
 */
void paste_command(int *cstate,HighLevelEvent *event,void **)
{
 //HANDLE clipdata;
//#if ! defined(WIN32)
 //GLOBALENTRY ge;
//#endif
 char *cd;
 RCCHAR cbfilename[300];
 FILE *outfile;
 CadFile cadfile;
 Point origin;
 Transform t;
 DWORD size;
 Entity *e1,*e2;
 EntityList list;
 int i;

  switch (*cstate)
    {case InitialState :
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
       {
           strcpy(cbfilename,_RCT(""));
#ifdef USING_WIDGETS
           const QClipboard *clipboard = QApplication::clipboard();
           if(clipboard != 0)
           {
               const QMimeData *mimedata = clipboard->mimeData();

               if(mimedata->hasFormat("application/ISCad3d"))
               {
                   QByteArray clipdata = mimedata->data("application/ISCad3d");

                   size = clipdata.size();

                   if(size > 0)
                   {
                       cd = clipdata.data();
                       strcpy(cbfilename,home.gettempfilename(_RCT("clipb")));
                       if ((outfile = fopen(cbfilename,"wb")) != NULL)
                       {
                           fwrite(cd,(size_t)size,1,outfile);
                           fclose(outfile);
                       }
                   }
               }
               else if(mimedata->hasImage())
               {
                   // make a BitMapE entity
                   double w,h,tr,tg,tb,trange,op;
                   BitMask o(32);
                   //unsigned char *pbuffer;
                   PixelBuffer pbuffer;

                   //QImage *qimage = new QImage(qvariant_cast<QImage>(mimedata->imageData()));
                   QImage *qimage = new QImage(clipboard->image());
                   *qimage = qimage->convertToFormat(QImage::Format_RGB888);

                   pbuffer = qimage->bits();

                   BitMapE::getdefaults(&w,&h,&tr,&tg,&tb,&trange,&op,&o);
                   o.set(BitmapESameShape);
                   //w = qimage->width();
                   //h = qimage->height();
                   //db.geometry.add(new BitMapE(db.header,0,bmi,pbuffer,((Coordinate3dEvent *)event)->getp(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),w,h,tr,tg,tb,trange,op,o));
                   db.geometry.add(new BitMapE(db.header,_RCT("$$CLIPBOARD_IMAGE$$"),qimage,pbuffer,((Coordinate3dEvent *)event)->getp(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),w,h,tr,tg,tb,trange,op,o));

               }
           }
#else
            if (OpenClipboard(cadwindow->gethwnd()))
              {  if ((clipdata = GetClipboardData(RegisterClipboardFormat(_RCT("ISCad3d")))) != NULL)
                   {
#if defined(WIN32)||defined(_MAC)
                      size = GlobalSize(clipdata);
#else
                      ge.dwSize = sizeof(ge);
                      GlobalEntryHandle(&ge,clipdata);
                      size = (size_t) ge.dwBlockSize;
#endif
                      if ((cd = (RCCHAR *) GlobalLock(clipdata)) != NULL)
                        {  if ((outfile = fopen(cbfilename = home.gettempfilename(_RCT("clipb")),"wb")) != NULL)
                             {  fwrite(cd,(size_t)size,1,outfile);
                                fclose(outfile);
                             }
                           GlobalUnlock(clipdata);
                        }
                   }
                 else if ((clipdata = GetClipboardData(CF_DIB)) != NULL)
                   {
#if defined(WIN32)||defined(_MAC)
                      size = GlobalSize(clipdata);
#else
                      ge.dwSize = sizeof(ge);
                      GlobalEntryHandle(&ge,clipdata);
                      size = (size_t) ge.dwBlockSize;
#endif
                      if ((cd = (RCCHAR *) GlobalLock(clipdata)) != NULL)
                        {
                           double w,h,tr,tg,tb,trange,op;
                           BitMask o(32);
                           int offset;
                           unsigned char *pbuffer;
                           BITMAPINFO *bmi;

                           bmi = (BITMAPINFO *) new char [sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * ((BITMAPINFOHEADER *)cd)->biClrUsed];
                           memcpy((char *)bmi,cd,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * ((BITMAPINFOHEADER *)cd)->biClrUsed);
                           pbuffer = new unsigned char [size - sizeof(BITMAPINFOHEADER) - sizeof(RGBQUAD)*((BITMAPINFOHEADER *)cd)->biClrUsed];
                           memcpy(pbuffer,cd+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*((BITMAPINFOHEADER *)cd)->biClrUsed,size-sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*((BITMAPINFOHEADER *)cd)->biClrUsed);
                           
                           offset = sizeof(BITMAPINFOHEADER);
                           if (((BITMAPINFOHEADER *)cd)->biClrUsed == 0 && ((BITMAPINFOHEADER *)cd)->biBitCount == 8)
                             ((BITMAPINFOHEADER *)cd)->biClrUsed = 256;
                           else if (((BITMAPINFOHEADER *)cd)->biClrUsed == 0 && ((BITMAPINFOHEADER *)cd)->biBitCount == 1)
                             ((BITMAPINFOHEADER *)cd)->biClrUsed = 2;
                           BitMapE::getdefaults(&w,&h,&tr,&tg,&tb,&trange,&op,&o);
                           db.geometry.add(new BitMapE(db.header,0,bmi,pbuffer,((Coordinate3dEvent *)event)->getp(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),w,h,tr,tg,tb,trange,op,o));
                           GlobalUnlock(clipdata);
                        }
                   }
                 CloseClipboard();
            }
#endif
            if (strlen(cbfilename) > 0 && cadfile.open(cbfilename,"rb"))
              {  

                cadwindow->startdrawingbatch();


                db.geometry.start();
                 e2 = db.geometry.next();
                 db.saveundo(UD_STARTBLOCK,NULL);
                 t.translate(((Coordinate3dEvent *)event)->getp());
                 db.loadgeometry(&cadfile,&t);
                 db.saveundo(UD_ENDBLOCK,NULL);
                 cadfile.close();


                 for (db.geometry.start() ; (e1 = db.geometry.next()) != e2 ; )
                   list.add(e1);
                 for (list.start() ; (e1 = list.next()) != 0 ; )
                   {  
                      for (i = 0 ; i < MaxViews ; i++)
                        if (e1->getvisible()->test(i))
                          break;

                      if (i < MaxViews)
                        {  e1->draw(DM_ERASE);
                           e1->setvisible(*db.header.getvisible());
                           e1->draw(DM_NORMAL);
                        }

                   } 
                 list.destroy(); 

                cadwindow->enddrawingbatch();


              }
            _unlink(cbfilename);
       }
       *cstate = ExitState;
    }
  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCCLIP+2);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
}

