
#include "ncwin.h"

void text_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Text"));
  *cstate = ExitState;
}

void new_text_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("NewText"));
  *cstate = ExitState;
}

struct RBText1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
    QGraphicsItem *qgbox;
#endif
    //char font[256],txt[4096],prefix[300],suffix[300];
   RCCHAR font[256],txt[4096],prefix[300],suffix[300];
   double height,width,slant,angle;
   int start,increment;
   Byte halocolour;
   BitMask options;
   RBText1_data() : options(32)  {}
};

#ifdef NEW_RUBBERB
void RBText1(int dm,void *data,Point *p2,View3dSurface *v)
{
 RBText1_data *rbdata = (RBText1_data *) data;
 static Point xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
 RCCHAR *f,*t;
 //char *f,*t;

 if(dm == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (v != 0)
  {
       xa = v->getxaxis();
       ya = v->getyaxis();
  }
  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
  RCSTRCPY(rbdata->font,f);
  Text text(rbdata->font,*p2,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,rbdata->txt,0,rbdata->options);

  text.draw(DM_INVERT);
}
#else
void RBText1(int dm,void *data,Point *p2,View3dSurface *v)
{RBText1_data *rbdata = (RBText1_data *) data;
 static Point xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
 RCCHAR *f,*t;
 //char *f,*t;

  if (v != 0)
    {  xa = v->getxaxis();
       ya = v->getyaxis();
    }
  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
  RCSTRCPY(rbdata->font,f);
  Text text(rbdata->font,*p2,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,rbdata->txt,0,rbdata->options);

  text.qgi = rbdata->qgi; text.zValue=rbdata->zValue;
  text.draw(DM_INVERT);
  rbdata->qgi = text.qgi; rbdata->zValue=text.zValue;
}
#endif

/**
* @brief text_command1
* @param cstate
* @param event
* @param data
*
* Entry point for the Insert Text Command
*
*/
void text_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBText1_data *rbdata = (RBText1_data *) *data;
 Point p,xa,ya;
 RCCHAR *f=0,*t=0;
 ResourceString rs0(NCTEXTB+0);
 Dialog dialog(_RCT("TextInsert_Dialog"));
 DialogControl *dc;

 v.setinteger("op::textangle",1);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
  {
    case InitialState:
         rbdata = new RBText1_data;  *data = rbdata;
         rbdata->qgi=0;rbdata->zValue=QGMAXZVALUE+QGZVALINC;
         if (rbdata != 0)
           {  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
              RCSTRCPY(rbdata->txt,t);  
			  RCSTRCPY(rbdata->font,f);
              dialog.add(new StringDialogControl(100,rbdata->txt,4096));
              dialog.add(dc = new RealDialogControl(101,&rbdata->height,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(dc = new RealDialogControl(102,&rbdata->angle));
              //dialog.add(new ScrollBarDialogControl(1102,dc));
              dialog.add(new CheckBoxDialogControl(103,&rbdata->options,13));
              if (dialog.process() == TRUE)
              {
                    Text::setdefaults(rbdata->font,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->txt,rbdata->options);
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
           {  p = ((Coordinate3dEvent *)event)->getp();
              xa = ((Coordinate3dEvent *)event)->getxaxis();
              ya = ((Coordinate3dEvent *)event)->getyaxis();
              //p.x=0;
              //p.y=0;
              Text *text = new Text(rbdata->font,p,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->txt,0,rbdata->options);
              if (text != NULL && text->getdefined())
              {
                  text->zValue = rbdata->zValue;
                  db.geometry.add(text);
              }
              else
                delete text;
           }
         else
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTEXTB+1);
       rubberband.begin(0);
       rubberband.add(RBText1,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief getChangeValues
 * @param change
 * @param list
 * Returns the bumber of text entities in the list
 * looks for text entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getTextChangeValues(BitMask *change, EntityList &list,EntityHeader *hd,RCCHAR **f,double *w,double *h,double *vs,double *a,Byte *hc,RCCHAR **t,RCCHAR **l,BitMask *o)
{
    Entity *e;
    //RCCHAR font[300],tfont[300];
    //double height,width,angle,slant,vertspacing;
    //double theight,twidth,tangle,tslant,tvertspacing;
    BitMask options(32),toptions(32);
    //Byte halocolour,thalocolour;
    //EntityHeader header,theader;
    Text *texte;

    // values to be compared
    // font
    // height
    // width
    // angle
    // horiz just
    // vert just
    // vert spacing
    // height in points
    // halo colour
    // bold
    // italic
    // halo on
    // underline
    // boxed
    // outline
    // scaled

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(text_entity))
        {
            texte = (Text*)e;

            if(i==0)
            {
                hd->setcolour(texte->getcolour());
                hd->setlayer(texte->getlayer());
                hd->setstyle(texte->getstyle());
                hd->setweight(texte->getweight());

                *f = texte->getfont();
                *h = texte->getheight();
                *w = texte->getwidth();
                *a = texte->getangle();
                *vs = texte->getvertspacing();
                *hc = texte->gethalocolour();
                options = texte->getoptions();
                *o = texte->getoptions();
                *t = texte->gettext();
                *l = texte->getlabel();

                change->clearall();

                change->set(0);
                change->set(1);
                change->set(2);
                change->set(3);

                change->set(4);
                change->set(8);
                change->set(9);
                change->set(10);
                change->set(13);
                change->set(22);
                change->set(11);
                change->set(12);
                change->set(14);
                change->set(15);
                change->set(16);
                change->set(17);
                change->set(18);
                change->set(19);
                change->set(20);
                change->set(21);
                change->set(23);

            }
            else
            {
                if(hd->getcolour() != texte->getcolour())
                    change->clear(0);
                if(hd->getlayer() != texte->getlayer())
                    change->clear(1);
                if(hd->getstyle() != texte->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),texte->getweight()))
                    change->clear(3);

                if(strcmp(*f,texte->getfont()) != 0)
                { change->clear(4); }
                if(fabs(*h - texte->getheight()) > 10.0e-5)
                { change->clear(8); }
                if(fabs(*w - texte->getwidth()) > 10.e-5)
                { change->clear(9);  }
                if(fabs(*a - texte->getangle()) > 10.e-5)
                { change->clear(10); }
                if(fabs(*vs - texte->getvertspacing()) > 10.e-5)
                { change->clear(13);  }
                if(*hc != texte->gethalocolour())
                { change->clear(22); }
                toptions = texte->getoptions();
                // horiz just
                if((options.test(1) != toptions.test(1) || (options.test(2) != toptions.test(2))))
                { change->clear(11); }
                // vert just
                if((options.test(3) != toptions.test(3) || (options.test(4) != toptions.test(4))))
                { change->clear(12); }
                if(options.test(5) != toptions.test(5))
                { change->clear(14); } // bold
                if(options.test(6) != toptions.test(6))
                { change->clear(15); }// underline
                if(options.test(7) != toptions.test(7))
                { change->clear(16); }// italic
                if(options.test(8) != toptions.test(8))
                { change->clear(17); }// boxed
                if(options.test(10) != toptions.test(10))
                { change->clear(18); }// outline
                if(options.test(9)  != toptions.test(9))
                { change->clear(19);  }// scaled
                if(options.test(11) != toptions.test(11))
                { change->clear(20); }// Always draw, even when hiding.
                if(options.test(12) != toptions.test(12))
                { change->clear(21); }// halo on
                if(options.test(13) != toptions.test(13))
                { change->clear(23); }// height in points
            }
            i++;
        }
    }
    return i;
}


/**
 * @brief text_command2
 * @param cstate
 * @param event
 *
 * The Text Options Dialog Command
 *
 */
void text_command2(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 RCCHAR *f=0,font[300],*tx=0,text[4096],*lb=0,label[300];
 double width,height,slant,angle;
 BitMask options(32),change(32);
 Point zero(0.0,0.0,0.0);
 EntityHeader header;
 Entity *e;
 ResourceString rs2(NCTEXTB+2);
 int status,length;
 Text *texte;
 Byte halocolour;

  v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
  {
      list = state.getselection().copy();
       if (list.empty()) // nothing pre selected
       {
            lb=0;
            Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&tx,&options);
            RCSTRCPY(font,f);
            RCSTRCPY(text,tx);
            status = Text::setup(NULL,font,&width,&height,&slant,&angle,&halocolour,&options);
            if (status == TRUE) 
              {  Text::setdefaults(font,width,height,slant,angle,halocolour,text,options);
                 *cstate = ExitState;
              }
            else if (status == 117)
              *cstate = 1; // clone button
            else if (status == 118)
              *cstate = 3; // clone all button
            else 
              *cstate = ExitState;
       }
       else // something pre selected
       {
            lb=0;
            Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&tx,&options);
            RCSTRCPY(font,f);
            RCSTRCPY(text,tx);
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found = getTextChangeValues(&change,list,&header,&f,&width,&height,&slant,&angle,&halocolour,&tx,&lb,&options);
            RCSTRCPY(font,f);
            RCSTRCPY(text,tx);
            if(lb != 0)
                RCSTRCPY(label,lb);
            else
                RCSTRCPY(label,"");
            v.setinteger("op::changecount",found);
            status=FALSE;
            if(found == 1)
                status = Text::setup(&change,font,&width,&height,&slant,&angle,&halocolour,&options,text,label,&header);
            else if(found > 1)
                status = Text::setup(&change,font,&width,&height,&slant,&angle,&halocolour,&options,0,0,&header);
            else
                status = Text::setup(NULL,font,&width,&height,&slant,&angle,&halocolour,&options);

            if (status == TRUE)
              {

                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(text_entity))
                   {
                       if(list.length() == 1)
                           ((Text *)e)->change(change,header,font,width,height,slant,angle,halocolour,text,label,options);
                       else
                       {
                            change.clear(30);
                            change.clear(31);
                            ((Text *)e)->change(change,header,font,width,height,slant,angle,halocolour,0,0,options);
                       }
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();

                 *cstate = ExitState;   

              }
            else if (status == 117)
              *cstate = 2; // clone button
            else if (status == 118)
              *cstate = 4; // clone all button
            else 
              *cstate = ExitState;
         }
  }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
  {
      if (event->isa(EntitySelected))
      {
            state.destroyselection(1);
            texte = (Text *)((EntitySelectedEvent *) event)->getentity();
            Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&tx,&options);
            if (*cstate == 1 || *cstate == 3) // clone or clone all
            {
                Text::setdefaults(texte->getfont(),texte->getwidth(),texte->getheight(),slant,texte->getangle(),texte->gethalocolour(),text,texte->getoptions());
                 if (*cstate == 3)
                 {
                     db.header.change(texte->getcolour(),texte->getlayer(),texte->getstyle(),texte->getweight());
                      v.setinteger("df::colour",texte->getcolour());
                      v.setinteger("df::layer",texte->getlayer());
                      v.setinteger("df::style",texte->getstyle());
                      v.setinteger("df::weight",texte->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                 }
            }
            else
            {
                 if (*cstate == 4) // clone all
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 8 ; i <= 23 ; i++) change.set(i,1); // clone
                 change.set(4,1); // font

                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(text_entity))
                     ((Text *)e)->change(change,*texte,texte->getfont(),texte->getwidth(),texte->getheight(),0.0,texte->getangle(),texte->gethalocolour(),0,0,texte->getoptions());
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 state.setselection(list,0);
            }
            *cstate = ExitState;
      }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
  }


  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(text_entity,end_list);
       cadwindow->prompt->normalprompt(_RCT("Select the text entity to clone from"));
    }
  //else if(*cstate == ExitState)
  //    state.destroyselection(1);
      //state.setselection(list,0);
}

/**
 * @brief text_command3
 * @param cstate
 * @param event
 *
 * Entry point for the Edit Text Command
 *
 */
void text_command3(int *cstate,HighLevelEvent *event,void **)
{Point zero(0.0,0.0,0.0);
 RCCHAR text[4096],suffixtext[4096],originaltext[4096],originalsuffixtext[4096];
 BitMask options(32),tchange(32),dchange(120);
 EntityList list,lp;
 EntityHeader header;
 Entity *e;
 ResourceString rs2(NCTEXTB+2);
 ResourceString rs4(NCTEXTB+4);
 ResourceString rs5(NCTEXTB+5);
 ResourceString rs6(NCTEXTB+6);
 ResourceString rs7(NCTEXTB+7);
 Dialog *dialog;
 int textentities,dimensionentities;

 short k = (GetKeyState(VK_SHIFT) & 0x8000);
 short b = (GetKeyState(VK_RBUTTON) & 0x8000);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }
  if (event != 0 && (event->isa(Abort) || event->isa(Exit)))
    {  *cstate = ExitState;
       return;
    }

  *cstate = 1;
  if (event == 0)
    *cstate = ExitState;
  list = state.getselection();
  textentities = 0;  dimensionentities = 0;
  for (list.start() ; (e = list.next()) != NULL ; )
    {  if (e->isa(text_entity) || e->isa(circulardimension_entity))
         textentities = 1;
       if (e->isa(lineardimension_entity))
         dimensionentities = 1;
       if (! e->isa(text_entity) && ! e->isa(circulardimension_entity) && ! e->isa(lineardimension_entity))
         {  cadwindow->MessageBox(rs2.gets(),rs5.gets(),MB_ICONINFORMATION);
            return;
         }
       else if (dimensionentities && (e->isa(text_entity) || e->isa(circulardimension_entity)))
         {  cadwindow->MessageBox(_RCT("Select text entities or dimension entities."),rs5.gets(),MB_ICONINFORMATION);
            return;
         }
       else if (textentities && e->isa(lineardimension_entity))
         {  cadwindow->MessageBox(_RCT("Select text entities or dimension entities."),rs5.gets(),MB_ICONINFORMATION);
            return;
         }
 
       lp = list;
       lp.start();
       if (((Entity *)lp.peek())->isa(text_entity))
         RCSTRCPY(text,((Text *)lp.peek())->gettext());
       else
         {TextInfo *ti;
            ti = ((Dimension *)lp.peek())->getprefixtextinfo();
            if (ti->text != 0)
              RCSTRCPY(text,ti->text);
            else
              RCSTRCPY(text,"");
            RCSTRCPY(originaltext,text);
            ti = ((Dimension *)lp.peek())->getsuffixtextinfo();
            if (ti->text != 0)
              RCSTRCPY(suffixtext,ti->text);
            else
              RCSTRCPY(suffixtext,"");
            RCSTRCPY(originalsuffixtext,suffixtext);
         }
       if (textentities)
         dialog = new Dialog(_RCT("LTTextinsert_Dialog"));
       else
         dialog = new Dialog(_RCT("DimensionText_Dialog"));
       if (dialog == NULL)
         state.fail(Memory,rs6.gets());
       else
         {  dialog->title(rs7.gets());
            dialog->add(new StringDialogControl(100,text,4096));
            if (dimensionentities)  
              dialog->add(new StringDialogControl(101,suffixtext,4096));
            if (dialog->process() == TRUE)
              {TextInfo ti1,ti2;
               TerminatorInfo term;
                 tchange.set(31);
                 dchange.set(71);  dchange.set(76);
                 ti1.text = text;
                 ti2.text = suffixtext;
                 lp = list;
                 for (lp.start() ; (e = lp.next()) != NULL ; )
                   if (e->isa(text_entity))
                     ((Text *)e)->change(tchange,header,(RCCHAR*)"",0.0,0.0,0.0,0.0,0,text,(RCCHAR*)0,options);
                   else
                     ((Dimension *)e)->change(dchange,header,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,term,term,ti1,ti1,ti1,ti2,options);
                 *cstate = 1;
                 break;
              }
         }
    }

  if (event != 0)
    {  state.selmask.entity.clearandset(text_entity,lineardimension_entity,circulardimension_entity,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate the text, leader or linear dimension that contains the text to modify"));
    }

}

/**
 * @brief text_command4
 * @param cstate
 * @param event
 *
 * Entry point for the Add figure text command
 *
 */
void text_command4(int *cstate,HighLevelEvent *event,void **)
{Point p,xa,ya;
 RCCHAR *f,*t,font[256],txt[4096],prompt[300],label[300];
 double height,width,slant,angle;
 BitMask options(32);
 ResourceString rs0(NCTEXTB+0);
 Byte halocolour;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState:
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *)event)->getp();
              xa = ((Coordinate3dEvent *)event)->getxaxis();
              ya = ((Coordinate3dEvent *)event)->getyaxis();
              Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
              RCSTRCPY(txt,t);  RCSTRCPY(font,f);
              Dialog *dialog = new Dialog(_RCT("FigureText_Dialog"));
              if (dialog == NULL)
                {  state.fail(Memory,rs0.gets());
                   *cstate = ExitState;
                }
              else
                {  RCSTRCPY(prompt,v.getstring("tx::prompt"));
                   RCSTRCPY(label,v.getstring("tx::label"));
                   dialog->add(new StringDialogControl(100, prompt,300));
                   dialog->add(new StringDialogControl(101, label,300));
                   if (dialog->process() == TRUE)
                     {  v.setstring("tx::prompt", prompt );
                        v.setstring("tx::label", label );
                        Text::setdefaults(font,width,height,slant,angle,halocolour,txt,options);
                        Text *text = new Text(font,p,xa,ya,width,height,slant,angle,halocolour,label,prompt,options);
                        if (text != NULL && text->getdefined())
                        {
                          text->zValue = QGMAXZVALUE+QGZVALINC;
                          db.geometry.add(text);
                        }
                        else
                          delete text;
                     }
                   delete dialog;
                }
           }
         else
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTEXTB+8);
    }
}

/**
 * @brief text_command5
 * @param cstate
 * @param event
 *
 * Entry point for the Edit figure text commmand
 *
 */
void text_command5(int *cstate,HighLevelEvent *event,void **)
{Point zero(0.0,0.0,0.0);
 RCCHAR label[300],prompt[300];
 BitMask options(32),change(32);
 EntityList list;
 EntityHeader header;
 Entity *e;
 ResourceString rs9(NCTEXTB+9);
 ResourceString rs10(NCTEXTB+10);
 ResourceString rs11(NCTEXTB+11);
 ResourceString rs12(NCTEXTB+12);
 ResourceString rs13(NCTEXTB+13);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }

  *cstate = ExitState;
  list = state.getselection();
  if (list.empty())
    cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
  else if (list.length() > 1)
    cadwindow->MessageBox(rs11.gets(),rs10.gets(),MB_ICONINFORMATION);
  else
    {  list.start();
       e = list.next();
       if (! e->isa(text_entity))
         {  cadwindow->MessageBox(rs12.gets(),rs10.gets(),MB_ICONINFORMATION);
            return;
         }
       if (((Text *)e)->getlabel() != 0)
         {  RCSTRCPY(label,((Text *)e)->gettext());
            RCSTRCPY(prompt,((Text *)e)->getlabel());
            Dialog dialog(_RCT("FigureText_Dialog"));
            dialog.title(rs13.gets());
            dialog.add(new StringDialogControl(100, prompt ,300));
            dialog.add(new StringDialogControl(101, label ,300));
            if (dialog.process() == TRUE)
              {  change.set(30);
                 change.set(31);
                 ((Text *)e)->change(change,header,_RCT(""),0.0,0.0,0.0,0.0,0,label,prompt,options);
              }
         }
       else
         cadwindow->MessageBox(_RCT("Cannot modify text"),rs10.gets(),MB_ICONSTOP);
    }
}

/**
 * @brief text_command6
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Text Counter command
 *
 */
void text_command6(int *cstate,HighLevelEvent *event,void **data)
{RBText1_data *rbdata = (RBText1_data *) *data;
 Point p,xa,ya;
 RCCHAR *f,*t;
 ResourceString rs0(NCTEXTB+0);
 Dialog dialog(_RCT("TextInsertCounter_Dialog"));
 DialogControl *dc;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState:
         rbdata = new RBText1_data;  *data = rbdata;
         rbdata->qgi=0;rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         if (rbdata != 0)
           {  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
              RCSTRCPY(rbdata->prefix,v.getstring("tx::counterprefix",rbdata->prefix));
              RCSTRCPY(rbdata->suffix,v.getstring("tx::countersuffix",rbdata->suffix));

              rbdata->start = v.getinteger("tx::startcounter");
              rbdata->increment = v.getinteger("tx::incrementcounter");
              RCSTRCPY(rbdata->font,f);
              dialog.add(new StringDialogControl(100,rbdata->prefix,300));
              dialog.add(new StringDialogControl(101,rbdata->suffix,300));
              dialog.add(dc = new IntegerDialogControl(102,&rbdata->start));
              //dialog.add(new ScrollBarDialogControl(1102,dc));
              dialog.add(dc = new IntegerDialogControl(103,&rbdata->increment));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              if (dialog.process() == TRUE)
                {  Text::setdefaults(rbdata->font,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->txt,rbdata->options);
                   v.setstring("tx::counterprefix",rbdata->prefix);
                   v.setstring("tx::countersuffix",rbdata->suffix);
                   v.setinteger("tx::startcounter",rbdata->start);
                   v.setinteger("tx::incrementcounter",rbdata->increment);

#ifdef _USING_QTCHAR
				   //RCSTRING buff;
				   //buff = RCSTRING("%1%2%3").arg(RCSTRING(rbdata->prefix)).arg(rbdata->start).arg(RCSTRING(rbdata->suffix));
				   //RCSTRCPY(rbdata->txt,buff.constData());
				   RCSPRINTF(rbdata->txt,"%ls%d%ls",rbdata->prefix,rbdata->start,rbdata->suffix);
#else
                   _stprintf((TCHAR*)rbdata->txt,"%s%d%s",rbdata->prefix,rbdata->start,rbdata->suffix);
#endif
                   rbdata->start += rbdata->increment;
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
           {  p = ((Coordinate3dEvent *)event)->getp();
              xa = ((Coordinate3dEvent *)event)->getxaxis();
              ya = ((Coordinate3dEvent *)event)->getyaxis();

              Text *text = new Text(rbdata->font,p,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->txt,0,rbdata->options);
              if (text != NULL && text->getdefined())
              {
                  text->zValue = rbdata->zValue;
                  db.geometry.add(text);
              }
              else
                delete text;
#ifdef _USING_QTCHAR
				   //RCSTRING buff;
				   //buff = RCSTRING("%1%2%3").arg(RCSTRING(rbdata->prefix)).arg(rbdata->start).arg(RCSTRING(rbdata->suffix));
				   //RCSTRCPY(rbdata->txt,buff.constData());
   				   RCSPRINTF(rbdata->txt,"%ls%d%ls",rbdata->prefix,rbdata->start,rbdata->suffix);
#else
              sprintf( rbdata->txt ,"%s%d%s",rbdata->prefix,rbdata->start,rbdata->suffix);
#endif
              rbdata->start += rbdata->increment;
           }
         else
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTEXTB+1);
       rubberband.begin(0);
       rubberband.add(RBText1,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}



struct RBText7_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
    QGraphicsItem *qgbox;
#endif
   Point p,xaxis,yaxis;
   RCCHAR font[256],txt[4096],prefix[300],suffix[300];
   double height,width,slant,angle;
   int state;
   BitMask options;
   Byte halocolour;
   RBText7_data() : options(32)  {}
};

#ifdef NEW_RUBBERB
void RBText7(int dm,void *data,Point *p2,View3dSurface *v)
{
 RBText7_data *rbdata = (RBText7_data *) data;
 Point xa(1.0,0.0,0.0),ya(0.0,1.0,0.0),xaxis,yaxis;
 RCCHAR *f,*t;

 if(dm == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (v != 0)
  {
       xa = v->getxaxis();
       ya = v->getyaxis();
  }

  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
  RCSTRCPY(rbdata->font,f);
  if (rbdata->state == 1)
  {
       Text text((RCCHAR*)rbdata->font,*p2,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,(RCCHAR*)rbdata->txt,0,rbdata->options);
       text.draw(DM_INVERT);
  }
  else
  {
       rbdata->angle = 0.0;
       xaxis = (*p2 - rbdata->p).normalize();
       yaxis = ((xa.cross(ya)).cross(xaxis)).normalize();

       if (xaxis.length() > 1.0E-10 && yaxis.length() > 1.0E-10)
       {
            rbdata->xaxis = xaxis;
            rbdata->yaxis = yaxis;
       }
       else
       {
            rbdata->xaxis = xa;
            rbdata->yaxis = ya;
       }

       Text text((RCCHAR*)rbdata->font,rbdata->p,rbdata->xaxis,rbdata->yaxis,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,(RCCHAR*)rbdata->txt,0,rbdata->options);
       text.draw(DM_INVERT);
  }
}
#else
void RBText7(int dm,void *data,Point *p2,View3dSurface *v)
{
 RBText7_data *rbdata = (RBText7_data *) data;
 Point xa(1.0,0.0,0.0),ya(0.0,1.0,0.0),xaxis,yaxis;
 RCCHAR *f,*t;

  if (v != 0)
  {
       xa = v->getxaxis();
       ya = v->getyaxis();
  }

  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
  RCSTRCPY(rbdata->font,f);
  if (rbdata->state == 1)
  {
       Text text((RCCHAR*)rbdata->font,*p2,xa,ya,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,(RCCHAR*)rbdata->txt,0,rbdata->options);

       text.qgi = rbdata->qgi; text.zValue = rbdata->zValue;
       text.draw(DM_INVERT);
       rbdata->qgi = text.qgi; rbdata->zValue = text.zValue;
  }
  else
  {
       rbdata->angle = 0.0;
       xaxis = (*p2 - rbdata->p).normalize();
       yaxis = ((xa.cross(ya)).cross(xaxis)).normalize();

       if (xaxis.length() > 1.0E-10 && yaxis.length() > 1.0E-10)
       {
            rbdata->xaxis = xaxis;
            rbdata->yaxis = yaxis;
       }
       else
       {
            rbdata->xaxis = xa;
            rbdata->yaxis = ya;
       }

       Text text((RCCHAR*)rbdata->font,rbdata->p,rbdata->xaxis,rbdata->yaxis,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,0,(RCCHAR*)rbdata->txt,0,rbdata->options);

       text.qgi = rbdata->qgi; text.zValue = rbdata->zValue;
       text.draw(DM_INVERT);
       rbdata->qgi = text.qgi; rbdata->zValue = text.zValue;
  }
}
#endif

/**
 * @brief text_command7
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Text By 2 Points Command
 * NOTE: the angle input text box does not work
 *
 */
void text_command7(int *cstate,HighLevelEvent *event,void **data)
{RBText7_data *rbdata = (RBText7_data *) *data;
 Point p,xa,ya;
 RCCHAR *f,*t;
 ResourceString rs0(NCTEXTB+0);
 Dialog dialog(_RCT("Textinsert_Dialog"));
 DialogControl *dc;

 v.setinteger("op::textangle",0);

 if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  state.sendevent(new NewCommandEvent(412,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState:
         rbdata = new RBText7_data;  *data = rbdata;
         rbdata->qgi=0;rbdata->zValue=QGMAXZVALUE+QGZVALINC;
         if (rbdata != 0)
           {  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
              RCSTRCPY(rbdata->txt,t);  RCSTRCPY(rbdata->font,f);
              dialog.add(new StringDialogControl(100,rbdata->txt,4096));
              dialog.add(dc = new RealDialogControl(101,&rbdata->height,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(new CheckBoxDialogControl(103,&rbdata->options,13));


              if (dialog.process() == TRUE)
                {  Text::setdefaults((RCCHAR*)rbdata->font,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,(RCCHAR*)rbdata->txt,rbdata->options);
                   rbdata->state = 1;
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
           {  rbdata->p = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
              rbdata->state = 2;
           }
         else
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  Text *text = new Text((RCCHAR*)rbdata->font,rbdata->p,rbdata->xaxis,rbdata->yaxis,rbdata->width,rbdata->height,rbdata->slant,rbdata->angle,rbdata->halocolour,(RCCHAR*)rbdata->txt,0,rbdata->options);
              if (text != NULL && text->getdefined())
              {
                  text->zValue = rbdata->zValue;
                  db.geometry.add(text);
              }
              else
                delete text;
              *cstate = 1;
              rbdata->state = 1;
           }
         else
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
  //if(rbdata->qgbox)
   //   cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgbox);
  //delete rbdata->qgbox; rbdata->qgbox=0;
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTEXTB+1);
       rubberband.begin(0);
       rubberband.add(RBText7,rbdata);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+68);
       rubberband.begin(0);
       rubberband.add(RBText7,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}



