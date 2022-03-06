
#include "ncwin.h"
#include "text_dialog.h"
#include <QFontComboBox>

extern BitMask * GetLockedLayerTable(void);

class TextVerifyButtonControl : public ButtonDialogControl
  {public:
     TextVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int TextVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,halocolour,layer,style,iweight;
    double weight;

    state = 1001;
    if (id == 1000)
    { // colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&colour);
        general_property_command(&state,0,(void **)&colour);
        ((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
    }
    else if(id == 1001)
    { // layer
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        //layer = db.header.getlayer();
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        //style = db.header.getstyle();
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        //iweight = db.header.getweight();
        //weight = db.lineweights.weighttomm(iweight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    else if(id == 1004)
    { // halo colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(121))->currentvalue(dialog,&halocolour);
        general_property_command(&state,0,(void **)&halocolour);
        if(((QSpinBox*)dialog->getcontrol(121)->getQW())->specialValueText().length() > 0)
            ((IntegerDialogControl *)dialog->getcontrol(121))->change(dialog,((QSpinBox*)dialog->getcontrol(121)->getQW())->maximum());
        ((IntegerDialogControl *)dialog->getcontrol(121))->change(dialog,halocolour);
        Colour gc(db.colourtable[halocolour].red,db.colourtable[halocolour].green,db.colourtable[halocolour].blue);
        ((ColourSampleControl *)dialog->getcontrol(125))->change(gc);
    }
    return 0;
}

/**
 * @brief Text::verify
 * the text verify dialog
 */
#if 1
void Text::verify(void)
{
#ifdef USING_WIDGETS
    // this function now just pops up the text options dialog
    ::state.sendevent(new NewCommandEvent(412,0));
#else
 Dialog dialog(_RCT("TextVerify_Dialog"));
 //Dialog dialog(_RCT("TextVerifyChange_Dialog"));
 int i,hjust,vjust,status,nfonts,fontid,hc;
 int c,l,s;
 double w;
 BitMask change(32);
 RCCHAR nfont[300],ntext[4096],nlabel[300];
 RCCHAR **fonts;
 Byte *charsets;
 RCCHAR s1[300],s2[300];
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12),rs2(NCTEXTA+2);
 RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 void *data;
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  int tristates[] = { 0,0,0,0,0,0,0,0 };

  v.setinteger("op::changecount",1);
  v.setinteger("op::change",1);

  options.test(5)==0  ? tristates[0] = 0 : tristates[0] = 1;
  options.test(6)==0  ? tristates[1] = 0 : tristates[1] = 1;
  options.test(7)==0  ? tristates[2] = 0 : tristates[2] = 1;
  options.test(8)==0  ? tristates[3] = 0 : tristates[3] = 1;
  options.test(10)==0 ? tristates[4] = 0 : tristates[4] = 1;
  options.test(9)==0  ? tristates[5] = 0 : tristates[5] = 1;
  options.test(12)==0 ? tristates[6] = 0 : tristates[6] = 1;
  options.test(13)==0 ? tristates[7] = 0 : tristates[7] = 1;

  hjust = options.test(1) + options.test(2) * 2;
  vjust = options.test(3) + options.test(4) * 2;

  if (! Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) return;
  strcpy(ntext,text);
  //dialog.add(new IntegerDialogControl(100,&c));
  //dialog.add(new IntegerDialogControl(101,&s));
  //dialog.add(new IntegerDialogControl(102,&l));
  //dialog.add(new WeightAssocRealDialogControl(102,&w));
  dialog.add(new TextVerifyButtonControl(1000));
  dialog.add(new TextVerifyButtonControl(1001));
  dialog.add(new TextVerifyButtonControl(1002));
  dialog.add(new TextVerifyButtonControl(1003));
  dialog.add(new TextVerifyButtonControl(1004));
  // set a the label flag for the dialog to pick up
  v.setinteger("tx::label",0);
  if (label == 0)
    dialog.add(new StringDialogControl(104, ntext,4096));
    //dialog.add(new StringDialogControl(104, ntext,RCSTRLEN(text)+1));
  else
  {
       v.setinteger("tx::label",1);
       strcpy(ntext,text);
       strcpy(nlabel,label);

       //dialog.add(new StringDialogControl(104, text,RCSTRLEN(text)+1));
       dialog.add(new StringDialogControl(104, ntext,300));
       dialog.add(new StringDialogControl(119, nlabel,300));
       //dialog.add(new StringDialogControl(219,rs2.gets(),strlen(rs2.gets())+1));
  }
  hc = halocolour;
  dialog.add(new FontListDialogControl(105,nfonts,fonts,&fontid));
  //dialog.add(new StringDialogControl(105, font,RCSTRLEN(font)+1));
  dialog.add(new RealDialogControl(106,&height,ScaleByLength));
  dialog.add(new RealDialogControl(107,&width,ScaleByLength));
  dialog.add(new RealDialogControl(108,&angle));
  dialog.add(new IntegerDialogControl(121,&hc));
  //strcpy(s1,hjustl[hjust]);
  //dialog.add(new StringDialogControl(109,s1,127));
  //strcpy(s2,vjustl[vjust]);
  //dialog.add(new StringDialogControl(110,s2,127));
  dialog.add(new ListDialogControl(109,3,hjustl,&hjust));
  dialog.add(new ListDialogControl(110,3,vjustl,&vjust));
  dialog.add(new RealDialogControl(111,&vertspacing));
  dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[0],false,options.test(5)));
  dialog.add(new TriStateCheckBoxDialogControl(113,&tristates[1],false,options.test(6)));
  dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[2],false,options.test(7)));
  dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[3],false,options.test(8)));
  dialog.add(new TriStateCheckBoxDialogControl(116,&tristates[4],false,options.test(10)));
  dialog.add(new TriStateCheckBoxDialogControl(120,&tristates[5],false,options.test(9)));
  dialog.add(new TriStateCheckBoxDialogControl(122,&tristates[6],false,options.test(12)));
  dialog.add(new TriStateCheckBoxDialogControl(123,&tristates[7],false,options.test(13)));
  //dialog.add(new CheckBoxDialogControl(112,&options,5));
  //dialog.add(new CheckBoxDialogControl(113,&options,6));
  //dialog.add(new CheckBoxDialogControl(114,&options,7));
  //dialog.add(new CheckBoxDialogControl(115,&options,8));
  //dialog.add(new CheckBoxDialogControl(116,&options,10));
  //dialog.add(new CheckBoxDialogControl(120,&options,9));
  //dialog.add(new CheckBoxDialogControl(122,&options,12));
  //dialog.add(new CheckBoxDialogControl(123,&options,13));
  dialog.add((DisplayDialogControl *) new TextDisplayDialogControl(124,fonts,charsets,&change));

  dialog.setposition(DPOK);
  if(dialog.process())
  {
      db.saveundo(UD_CHANGEATTRIBUTES,this);
      if (getposition() != 0)
          draw(DM_ERASE);
      cadwindow->invalidatedisplaylist(this);

      delete font;
      font = new RCCHAR[300];
      RCSTRCPY(font,fonts[fontid]);

      if (label == 0)
      {
          delete text;
          text = new RCCHAR[4096];
          strcpy(text,ntext);
      }
      else
      {
          delete text;
          text = new RCCHAR[300];
          strcpy(text,ntext);

          delete label;
          label = new RCCHAR[300];
          strcpy(label,nlabel);
      }
      halocolour = hc;

      options.set(5, tristates[0]);
      options.set(6, tristates[1]);
      options.set(7, tristates[2]);
      options.set(8, tristates[3]);
      options.set(10,tristates[4]);
      options.set(9, tristates[5]);
      options.set(12,tristates[6]);
      options.set(13,tristates[7]);


      options.set(1,hjust % 2);
      options.set(2,hjust / 2);
      options.set(3,vjust % 2);
      options.set(4,vjust / 2);

      colour = c;
      style = s;
      layer = l;
      weight = db.lineweights.mmtoweight(w);

      if (getposition() != 0)
        draw(DM_NORMAL);


      //change.setall();
      //change.clear(30);
      //change.clear(31);
      //((Text *)e)->change(change,header,font,width,height,0,angle,halocolour,label,text,options);

      for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
      delete [] fonts;
      //delete [] charsets;
  }
#endif
}
#else
void Text::verify(void)
{
 Dialog dialog(_RCT("TextVerify_Dialog"));
 int status,hjust,vjust,c,l,s,hc;
 double w;
 RCCHAR s1[300],s2[300];
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12),rs2(NCTEXTA+2);
 RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 void *data;
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  hjust = options.test(1) + options.test(2) * 2;
  vjust = options.test(3) + options.test(4) * 2;

  if (label == 0)
    dialog.add(new StringDialogControl(104, text,RCSTRLEN(text)+1));
  else
    {
       dialog.add(new StringDialogControl(104, text,RCSTRLEN(text)+1));
       dialog.add(new StringDialogControl(119, label,RCSTRLEN(label)+1));
       dialog.add(new StringDialogControl(219,rs2.gets(),strlen(rs2.gets())+1));
    }
  hc = halocolour;
  dialog.add(new StringDialogControl(105, font,RCSTRLEN(font)+1));
  dialog.add(new RealDialogControl(106,&height,ScaleByLength));
  dialog.add(new RealDialogControl(107,&width,ScaleByLength));
  dialog.add(new RealDialogControl(108,&angle));
  dialog.add(new IntegerDialogControl(121,&hc));
  strcpy(s1,hjustl[hjust]);
  dialog.add(new StringDialogControl(109,s1,127));
  strcpy(s2,vjustl[vjust]);
  dialog.add(new StringDialogControl(110,s2,127));
  dialog.add(new RealDialogControl(111,&vertspacing));
  dialog.add(new CheckBoxDialogControl(112,&options,5));
  dialog.add(new CheckBoxDialogControl(113,&options,6));
  dialog.add(new CheckBoxDialogControl(114,&options,7));
  dialog.add(new CheckBoxDialogControl(115,&options,8));
  dialog.add(new CheckBoxDialogControl(116,&options,10));
  dialog.add(new CheckBoxDialogControl(120,&options,9));
  dialog.add(new CheckBoxDialogControl(122,&options,12));
  dialog.add(new CheckBoxDialogControl(123,&options,13));

  dialog.adddisable(112);
  dialog.adddisable(113);
  dialog.adddisable(114);
  dialog.adddisable(115);
  dialog.adddisable(116);
  dialog.adddisable(120);
  dialog.adddisable(122);
  dialog.adddisable(123);

  if (GetLockedLayerTable() == 0 || GetLockedLayerTable()->test(getlayer()) == 0)
    {  dialog.add(new ButtonDialogControl(117));
       dialog.add(new ButtonDialogControl(118));
    }
  dialog.setposition(DPOK);
  status = dialog.process();
  if (status == 117)
    state.sendevent(new NewCommandEvent(412,0));
  else if (status == 118)
    text_command3(&s,NULL,&data);
}
#endif

void Text::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{
#ifdef USING_WIDGETS
    RCHFONT oldhfont,cwoldhfont,hfont,printerhfont,printeroldhfont;
    //BYTE fbCharSet;
    int i,ih,iw,maxw,maxh,w,ll,nlines,metafile;
    double scale,oheight,rh,rw,a,l,cosa,sina,y,xmaxh,xmaxw,xw,ppscale,tmHeight,tmDescent,tmAscent;
    //TEXTMETRICW tm;
    Point zero(0.0,0.0,0.0),o,xa,ya,p1,p2,p3,p4,pt1,pt2;
    Transform tr;
    //char *s;
    RCCHAR *s;

#if 0
     if (options.test(10))
       {  if (surface == NULL)
            *pmin = *pmax = origin;
          else
            *pmin = *pmax = surface->modeltouv(origin);
          xa = xaxis;  ya = yaxis;
          tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
          xa = tr.transform(xa);
          ya = tr.transform(ya);
          o = origin;
          if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
          else
            fbCharSet = ANSI_CHARSET;
          hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                             options.test(7),options.test(6),0,fbCharSet,
                             OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

          if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
          GetTextMetricsW(cadwindow->gethdc(),&tm);

          if (tm.tmHeight == 0) tm.tmHeight = 1;
          if (options.test(9) && surface != 0)
            scale = surface->getplotscale() / double(tm.tmHeight) * height;
          else
            scale = height / double(tm.tmHeight);
          for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
          if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
          else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
          else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
          xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
          xmaxw = 0;
          y = 0;
          for (s = text ; *s!=0 ;  s += ll + 2)
            {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
               SIZE size;
               GetTextExtentPoint(cadwindow->gethdc(), s,ll,&size);
               xw = size.cx * scale;

               if (xw > xmaxw) xmaxw = xw;
                y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
               if (s[ll] == 0) break;
            }
          p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
          if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
          else if (options.test(2))
            p1 = p1 - xa * xmaxw;
          p2 = p1 + xa * xmaxw;
          p3 = p2 + ya * xmaxh;
          p4 = p3 - xa * xmaxw;
          for (i = 0 ; i < 4 ; i++)
            {  switch (i)
                 {  case 1 : p1 = p2;  break;
                    case 2 : p1 = p3;  break;
                    case 3 : p1 = p4;  break;
                 }
               if (surface != NULL)
                 p1 = surface->modeltouv(p1);
               if (p1.x < pmin->x) pmin->x = p1.x;
               if (p1.y < pmin->y) pmin->y = p1.y;
               if (p1.z < pmin->z) pmin->z = p1.z;
               if (p1.x > pmax->x) pmax->x = p1.x;
               if (p1.y > pmax->y) pmax->y = p1.y;
               if (p1.z > pmax->z) pmax->z = p1.z;
            }
          if (hfont != NULL)
            {  SelectObject(cadwindow->gethdc(),oldhfont);
               DeleteObject(hfont);
            }
       }
     else
#endif
     {
         if (surface == NULL)
         {
             *pmin = *pmax = origin;  return;
         }
          *pmin = *pmax = surface->modeltouv(origin);
          p1 = surface->modeltoscreen(origin);
          //if (options.test(0))
          //  fbCharSet = SYMBOL_CHARSET;
          //else
          //  fbCharSet = ANSI_CHARSET;
          xa = surface->modeltouv(origin + xaxis) - surface->modeltouv(origin);
          if ((l = xa.length()) < db.getptoler()) return;
          xa /= l;
          if (xa.x < -1.0) xa.x = -1.0; else if (xa.x > 1.0) xa.x = 1.0;
          a = acos(xa.x);
          if (xa.y < 0.0) a = 2.0 * M_PI - a;
          if (options.test(9))
            scale = surface->getplotscale();
          else
            scale = 1.0;
          rh = height * surface->getheight() / (surface->getvmax() - surface->getvmin()) * scale;
          if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
          rw = width * surface->getwidth() / (surface->getumax() - surface->getumin()) * scale;
          if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT; else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT; else ih = int(rh);
          if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT; else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT; else iw = int(rw);
          if (ih == 0) return;

          //hfont = CreateFontW(100,int(100 * width / (height > 0.0 ? height : 1.0)),int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
          //                   (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

          // calculate the Qt font pixel height required for the text height setting
          // calculate the point height for the font
          QString refName(font);
          QFont refFont(refName);
          refFont.setPointSize((int)100);// use 100 as points
          QFontMetrics refFm(refFont);
          int refHeight = refFm.height();// get the height as pixels for rh points
          // how many points did we get for for 1 pixel of rh
          double ptsperpx = 100.0 / refHeight;
          // how many points to get the same height in pixels
          int fontHeight = (int)(100.0 * ptsperpx);

          // set up a device independent font
          QString qfname(font);
          QFont qfont(qfname);
          qfont.setStyleStrategy(QFont::ForceOutline);
          qfont.setPointSize(fontHeight);
          // set the font weight
          if(options.test(5))
              qfont.setWeight(QFont::Bold);
          else
              qfont.setWeight(QFont::Normal);
          // set italic
          qfont.setItalic(options.test(7));
          // set underline
          qfont.setUnderline(options.test(6));

          // get the font metrics
          QFontMetricsF fm(qfont);

          // calculate the strtch factor
          // this is done by scaling the font in width
          //double fstretch = width  / (height > 0.0 ? height : 1.0);
          //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
          double fstretch = 1.0;
          if(width == 0.0)
              fstretch = 1.0;
          else
              fstretch = fabs(width);

          tmHeight = fm.height();
          tmDescent = fm.descent();
          tmAscent = fm.ascent();

          sina = sin(a + angle / 180.0 * M_PI);
          cosa = cos(a + angle / 180.0 * M_PI);

          metafile = 0;
          //metafile = GetDeviceCaps(surface->gethdc(),TECHNOLOGY) == DT_METAFILE;
          //if (hfont != NULL)
          //  {  oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
          //     if (metafile)
          //       cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
          //  }

          if (surface->getprinterhdc() != 0)
          {
              ppscale = surface->getheight() / (surface->getvmax() - surface->getvmin()) * //surface->getplotscale() *
                             cadwindow->printpreviewwindow->getprintscale() / double(GetDeviceCaps(surface->getprinterhdc(),LOGPIXELSX)) * 25.4;
               //printerhfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
               //              (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

               //printeroldhfont = (RCHFONT) SelectObject(surface->getprinterhdc(),printerhfont);
          }
          //if (metafile)
          //  GetTextMetricsW(cadwindow->gethdc(),&tm);
          //else
          //  GetTextMetricsW(surface->gethdc(),&tm);

          //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
          for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);

          if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
          else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
          else
            oheight = 0.0;
          oheight *= rh / 100.0;
          p1.x += oheight * sina;  p1.y -= oheight * cosa;
          p3 = p1;

          maxh = int(tmHeight * (nlines + (nlines - 1)*vertspacing));
          maxw = 0;

          for (s = text ; *s!=0 ;  s += ll + 1)
          {
              for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;

              if (metafile)
              {
                    //SIZE size;
                    //GetTextExtentPoint(cadwindow->gethdc(),s,ll,&size);
                    //w = size.cx;

                    w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();

               }
               else
               {
                    //SIZE size;
                    //GetTextExtentPoint(surface->gethdc(),s,ll,&size);
                    //w = size.cx;

                    w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();

                    if (surface->getprinterhdc() != 0)
                    {
                        //GetTextExtentPoint(surface->getprinterhdc(),s,ll,&size);
                        // w = int(size.cx * ppscale);
                        w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();
                        w *= ppscale;
                    }
               }
               if (w > maxw) maxw = w;
               p2 = p1;
               if (options.test(1))
               {
                   p2.x -= w * fstretch * rh / 100.0 * cosa / 2.0;  p2.y -= w * fstretch * rh / 100.0 * sina / 2.0;
               }
               else if (options.test(2))
               {
                   p2.x -= w * fstretch * rh / 100.0 * cosa;  p2.y -= w * fstretch * rh / 100.0 * sina;
               }
               p1.x += tmHeight * (1.0 + vertspacing) * sina;  p1.y -= tmHeight * (1.0 + vertspacing) * cosa;
               if (s[ll] == 0) break;
          }
          if (options.test(1))
          {
              p3.x -= maxw * fstretch * rh / 100.0 * cosa / 2.0;  p3.y -= maxw * fstretch * sina * rh / 100.0 / 2.0;
          }
          else if (options.test(2))
          {
              p3.x -= maxw * fstretch * rh / 100.0 * cosa;  p3.y -= maxw * fstretch * rh / 100.0 * sina;
          }

          maxw = int (maxw * rh / 100.0);
          maxh = int (maxh * rh / 100.0);

          for (i = 0 ; i < 4 ; i++)
          {
               switch (i)
               {
                    case 0 : p1.setxyz(p3.x,p3.y,0.0);  break;
                    case 1 : p1.setxyz(p3.x+maxw * fstretch*cosa,p3.y+maxw * fstretch*sina,0.0);  break;
                    case 2 : p1.setxyz(p3.x+maxw * fstretch*cosa+maxh*sina,p3.y+maxw * fstretch*sina-maxh*cosa,0.0);  break;
                    case 3 : p1.setxyz(p3.x+maxh*sina,p3.y-maxh*cosa,0.0);  break;
               }
               p1 = surface->screentouv(p1);
               if (p1.x < pmin->x) pmin->x = p1.x;
               if (p1.y < pmin->y) pmin->y = p1.y;
               if (p1.z < pmin->z) pmin->z = p1.z;
               if (p1.x > pmax->x) pmax->x = p1.x;
               if (p1.y > pmax->y) pmax->y = p1.y;
               if (p1.z > pmax->z) pmax->z = p1.z;
          }
          /*
          if (hfont != NULL)
            {  if (metafile)
                 SelectObject(cadwindow->gethdc(),cwoldhfont);
               SelectObject(surface->gethdc(),oldhfont);
               DeleteObject(hfont);
            }
          if (0 && surface->getprinterhdc() != 0)
            {  SelectObject(surface->getprinterhdc(),printeroldhfont);
               DeleteObject(printerhfont);
            }
          */
       }
#else
 RCHFONT oldhfont,cwoldhfont,hfont,printerhfont,printeroldhfont;
 BYTE fbCharSet;
 int i,ih,iw,maxw,maxh,w,ll,nlines,metafile;
 double scale,oheight,rh,rw,a,l,cosa,sina,y,xmaxh,xmaxw,xw,ppscale;
#ifdef _USING_QTCHAR
 TEXTMETRICW tm;
#else
 TEXTMETRIC tm;
#endif
 Point zero(0.0,0.0,0.0),o,xa,ya,p1,p2,p3,p4,pt1,pt2;
 Transform tr;
 //char *s;
 RCCHAR *s;

  if (options.test(10))
    {  if (surface == NULL)
         *pmin = *pmax = origin;
       else
         *pmin = *pmax = surface->modeltouv(origin);
       xa = xaxis;  ya = yaxis;
       tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
       xa = tr.transform(xa);
       ya = tr.transform(ya);
       o = origin;
       if (options.test(0))
         fbCharSet = SYMBOL_CHARSET;
       else
         fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
       hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,
                          OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
       hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,
                          OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#endif
       if (hfont != NULL)
         oldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
#ifdef _USING_QTCHAR
	   GetTextMetricsW(cadwindow->gethdc(),&tm);
#else
	   GetTextMetrics(cadwindow->gethdc(),&tm);
#endif
       if (tm.tmHeight == 0) tm.tmHeight = 1;
       if (options.test(9) && surface != 0)
         scale = surface->getplotscale() / double(tm.tmHeight) * height;
       else
         scale = height / double(tm.tmHeight);
       for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       if (! options.test(3) && ! options.test(4))
         oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
       else if (options.test(3))
         oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
       else
         oheight = -(tm.tmHeight - tm.tmDescent) * scale;
       xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
       xmaxw = 0;
       y = 0;
       for (s = text ; *s!=0 ;  s += ll + 2)
         {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
            GetTextExtentPoint(cadwindow->gethdc(), s,ll,&size);
            xw = size.cx * scale;
#else
            long r;
            r = GetTextExtent(cadwindow->gethdc(),s,ll);
            xw = LOWORD(r) * scale;
#endif
            if (xw > xmaxw) xmaxw = xw;
             y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
         }
       p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
       if (options.test(1))
         p1 = p1 - xa * xmaxw / 2.0;
       else if (options.test(2))
         p1 = p1 - xa * xmaxw;
       p2 = p1 + xa * xmaxw;
       p3 = p2 + ya * xmaxh;
       p4 = p3 - xa * xmaxw;
       for (i = 0 ; i < 4 ; i++)
         {  switch (i)
              {  case 1 : p1 = p2;  break;
                 case 2 : p1 = p3;  break;
                 case 3 : p1 = p4;  break;
              }
            if (surface != NULL)
              p1 = surface->modeltouv(p1);
            if (p1.x < pmin->x) pmin->x = p1.x;
            if (p1.y < pmin->y) pmin->y = p1.y;
            if (p1.z < pmin->z) pmin->z = p1.z;
            if (p1.x > pmax->x) pmax->x = p1.x;
            if (p1.y > pmax->y) pmax->y = p1.y;
            if (p1.z > pmax->z) pmax->z = p1.z;
         }
       if (hfont != NULL)
         {  SelectObject(cadwindow->gethdc(),oldhfont);
            DeleteObject(hfont);
         }
    }
  else
    {  if (surface == NULL)
         {  *pmin = *pmax = origin;  return;
         }
       *pmin = *pmax = surface->modeltouv(origin);
       p1 = surface->modeltoscreen(origin);
       if (options.test(0))
         fbCharSet = SYMBOL_CHARSET;
       else
         fbCharSet = ANSI_CHARSET;
       xa = surface->modeltouv(origin + xaxis) - surface->modeltouv(origin);
       if ((l = xa.length()) < db.getptoler()) return;
       xa /= l;
       if (xa.x < -1.0) xa.x = -1.0; else if (xa.x > 1.0) xa.x = 1.0;
       a = acos(xa.x);
       if (xa.y < 0.0) a = 2.0 * M_PI - a;
       if (options.test(9))
         scale = surface->getplotscale();
       else
         scale = 1.0;
       rh = height * surface->getheight() / (surface->getvmax() - surface->getvmin()) * scale;
       if (options.test(13))  // Height is in points
         rh *= 25.4 / 72.0; 
       rw = width * surface->getwidth() / (surface->getumax() - surface->getumin()) * scale;
       if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT; else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT; else ih = int(rh);
       if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT; else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT; else iw = int(rw);
       if (ih == 0) return;
#ifdef _USING_QTCHAR
       hfont = CreateFontW(100,int(100 * width / (height > 0.0 ? height : 1.0)),int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
                          (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
       hfont = CreateFont(100,int(100 * width / (height > 0.0 ? height : 1.0)),int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
                          (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#endif
	   sina = sin(a + angle / 180.0 * M_PI);
       cosa = cos(a + angle / 180.0 * M_PI);
       metafile = GetDeviceCaps(surface->gethdc(),TECHNOLOGY) == DT_METAFILE;
       if (hfont != NULL)
         {  oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
            if (metafile)
              cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
         }

       if (surface->getprinterhdc() != 0)
         {  ppscale = surface->getheight() / (surface->getvmax() - surface->getvmin()) * //surface->getplotscale() *
                          cadwindow->printpreviewwindow->getprintscale() / double(GetDeviceCaps(surface->getprinterhdc(),LOGPIXELSX)) * 25.4;
#ifdef _USING_QTCHAR
            printerhfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
                          (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
            printerhfont = CreateFont(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + surface->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + surface->getrotation2d())* 10.0),
                          (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
			printeroldhfont = (RCHFONT) SelectObject(surface->getprinterhdc(),printerhfont);
         }
#ifdef _USING_QTCHAR
       if (metafile)
         GetTextMetricsW(cadwindow->gethdc(),&tm);
       else
         GetTextMetricsW(surface->gethdc(),&tm);
#else
       if (metafile)
         GetTextMetrics(cadwindow->gethdc(),&tm);
       else
         GetTextMetrics(surface->gethdc(),&tm);
#endif
       for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       if (! options.test(3) && ! options.test(4))
         oheight = -nlines * tm.tmHeight + tm.tmDescent - (nlines - 1) * tm.tmHeight * vertspacing;
       else if (options.test(3))
         oheight = -(nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2;
       else
         oheight = 0.0;
       oheight *= rh / 100.0;  
       p1.x += oheight * sina;  p1.y -= oheight * cosa;
       p3 = p1;
       maxh = int(tm.tmHeight * (nlines + (nlines - 1)*vertspacing));
       maxw = 0;
       for (s = text ; *s!=0 ;  s += ll + 2)
         {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            if (metafile)
              {
#if defined(WIN32)
                 SIZE size;
                 GetTextExtentPoint(cadwindow->gethdc(),s,ll,&size);
                 w = size.cx;

#else
                 long r;
                 r = GetTextExtent(cadwindow->gethdc(),s,ll);
                 w = LOWORD(r);
#endif
              }
            else
              {
#if defined(WIN32)
                 SIZE size;
                 GetTextExtentPoint(surface->gethdc(),s,ll,&size);
                 w = size.cx;
                 if (surface->getprinterhdc() != 0)
                   {  GetTextExtentPoint(surface->getprinterhdc(),s,ll,&size);
                      w = int(size.cx * ppscale);
                   }
#else
                 long r;
                 r = GetTextExtent(surface->gethdc(),s,ll);
                 w = LOWORD(r);
#endif
              }
            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1))
              {  p2.x -= w * rh / 100.0 * cosa / 2.0;  p2.y -= w * rh / 100.0 * sina / 2.0;
              }
            else if (options.test(2))
              {  p2.x -= w * rh / 100.0 * cosa;  p2.y -= w * rh / 100.0 * sina;
              }
            p1.x += tm.tmHeight * (1.0 + vertspacing) * sina;  p1.y -= tm.tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
         }
       if (options.test(1))
         {  p3.x -= maxw * rh / 100.0 * cosa / 2.0;  p3.y -= maxw * sina * rh / 100.0 / 2.0;
         }
       else if (options.test(2))
         {  p3.x -= maxw * rh / 100.0 * cosa;  p3.y -= maxw * rh / 100.0 * sina;
         }

       maxw = int (maxw * rh / 100.0); 
       maxh = int (maxh * rh / 100.0);

       for (i = 0 ; i < 4 ; i++)
         {  switch (i)
              {  case 0 : p1.setxyz(p3.x,p3.y,0.0);  break;
                 case 1 : p1.setxyz(p3.x+maxw*cosa,p3.y+maxw*sina,0.0);  break;
                 case 2 : p1.setxyz(p3.x+maxw*cosa+maxh*sina,p3.y+maxw*sina-maxh*cosa,0.0);  break;
                 case 3 : p1.setxyz(p3.x+maxh*sina,p3.y-maxh*cosa,0.0);  break;
              }
            p1 = surface->screentouv(p1);
            if (p1.x < pmin->x) pmin->x = p1.x;
            if (p1.y < pmin->y) pmin->y = p1.y;
            if (p1.z < pmin->z) pmin->z = p1.z;
            if (p1.x > pmax->x) pmax->x = p1.x;
            if (p1.y > pmax->y) pmax->y = p1.y;
            if (p1.z > pmax->z) pmax->z = p1.z;
         }
       if (hfont != NULL)
         {  if (metafile)
              SelectObject(cadwindow->gethdc(),cwoldhfont);
            SelectObject(surface->gethdc(),oldhfont);
            DeleteObject(hfont);
         }
       if (0 && surface->getprinterhdc() != 0)
         {  SelectObject(surface->getprinterhdc(),printeroldhfont);
            DeleteObject(printerhfont);
         }
    }
#endif
}
#ifdef _USING_QTCHAR
int Text::extents(View3dSurface *v,const char *font,const BitMask &options,const char *text,double w,double h,double vs,
                          double *wid,double *hgt,double *xo,double *yo)
{
	// NOTE: make these on the heap!
	RCCHAR rcfont[100], rctext[5000];
	LOCAL2RCHR(rcfont,(TCHAR*)font);
	LOCAL2RCHR(rctext,(TCHAR*)text);
	int ret = extents(v,rcfont,options,rctext, w,h,vs,wid,hgt,xo,yo);
	RCHR2LOCAL((TCHAR*)font,rcfont);
	RCHR2LOCAL((TCHAR*)text,rctext);
	return ret;
}
#endif
int Text::extents(View3dSurface *v,const RCCHAR *font,const BitMask &options,const RCCHAR *text,double w,double h,double vs,
                          double *wid,double *hgt,double *xo,double *yo)
{
#ifdef USING_WIDGETS
  RCHFONT cwoldhfont,oldhfont,hfont,printerhfont,printeroldhfont;
 //BYTE fbCharSet;
 //TEXTMETRICW tm;
 int ll,maxw,iw,ih,lw,nlines,metafile;
 double scale,ppscale,tmHeight,tmDescent,tmAscent,rtw;
 //const char *s;
 const RCCHAR *s;

#if 0
  if (options.test(10))
    {  for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       *wid = RCSTRLEN(s) * w;
       *hgt = h * (nlines + (nlines - 1) * vs);
       if (v == NULL) return 1;
       if (options.test(0))
         fbCharSet = SYMBOL_CHARSET;
       else
         fbCharSet = ANSI_CHARSET;
       hfont = CreateFontW(100,w == 0.0 || h == 0.0 ? 0 : int(100 * w / h),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,
                          OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

       if (hfont == NULL) return 0;
       oldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
       GetTextMetricsW(cadwindow->gethdc(),&tm);

       if (options.test(9) && v != 0)
         scale = v->getplotscale() / double(tm.tmHeight) * h;
       else
         scale = h / double(tm.tmHeight);
       maxw = 0;
       for (s = text ; *s!=0 ;  s += ll + 2)
         {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            SIZE size;
            //GetTextExtentPoint(cadwindow->gethdc(),s,ll,&size);
            GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
            lw = int(size.cx * scale);

            if (lw > maxw) maxw = lw;
            if (s[ll] == 0) break;
         }
       SIZE size;
       //GetTextExtentPoint(cadwindow->gethdc(),s,strlen(s),&size);
       GetTextExtentPoint32W(cadwindow->gethdc(), s,RCSTRLEN(s),&size);
       *hgt = size.cy * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);

       *wid = maxw * (v->getumax() - v->getumin()) / v->getwidth();
       *xo = 0.0;
       if (! options.test(3) && ! options.test(4))
         *yo = -tm.tmDescent * (v->getvmax() - v->getvmin()) / v->getheight();
       else if (options.test(3))
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight / 2;
       else
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight;
       SelectObject(cadwindow->gethdc(),oldhfont);
       DeleteObject(hfont);
    }
  else
#endif
    {
      //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
      //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
      // calculate the number of lines
      // text on windows have CR+LF line endings MAC OS X has LF line endings
      // find the line feed or carriage return or line feed and carriage return
      // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
      for (s = text,nlines = 1 ; *s!=0 ; s++)
      {
          nlines += (*s == 0x000D || *s == 0x000A) ;
          if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
              s++;
      }

       *wid = RCSTRLEN(s) * w;
       *hgt = h * (nlines + (nlines - 1) * vs);
       if (v == NULL) return 1;
       if (options.test(9))
         scale = v->getplotscale();
       else
         scale = 1.0;
       h = h * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
       if (options.test(13))  // Height is in points
         h *= 25.4 / 72.0;
       w = w * v->getwidth() / (v->getumax() - v->getumin()) * scale;
       if (h > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT; else if (h < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT; else ih = int(h);
       if (w > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT; else if (w < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT; else iw = int(w);
       if (ih == 0) return 1;
       //if (options.test(0))
       //  fbCharSet = SYMBOL_CHARSET;
       //else
       //  fbCharSet = ANSI_CHARSET;

       //hfont = CreateFontW(100,int(100*w/(h > 0.0 ? h : 1.0)),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
       //                   options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
       //if (hfont == NULL) return 0;

       // calculate the Qt font pixel height required for the text height setting
       // calculate the point height for the font
       QString refName(font);
       QFont refFont(refName);
       refFont.setPointSize((int)h);// use 100 as points
       QFontMetrics refFm(refFont);
       int refHeight = refFm.height();// get the height as pixels for rh points
       // how many points did we get for for 1 pixel of rh
       double ptsperpx = h / refHeight;
       // how many points to get the same height in pixels
       int fontHeight = (int)(h * ptsperpx);

       // set up a device independent font
       QString qfname(font);
       QFont qfont(qfname);
       qfont.setStyleStrategy(QFont::ForceOutline);
       qfont.setPointSize(fontHeight);
       // set the font weight
       if(options.test(5))
           qfont.setWeight(QFont::Bold);
       else
           qfont.setWeight(QFont::Normal);
       // set italic
       qfont.setItalic(options.test(7));
       // set underline
       qfont.setUnderline(options.test(6));

       // get the font metrics
       QFontMetricsF fm(qfont);

       // calculate the strtch factor
       // this is done by scaling the font in width
       //double fstretch = w  / (h > 0.0 ? h : 1.0);
       //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
       double fstretch = 1.0;
       if(w == 0.0)
           fstretch = 1.0;
       else
           fstretch = fabs(w);

       tmHeight = fm.height();
       tmDescent = fm.descent();
       tmAscent = fm.ascent();

       metafile = 0;
       //metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;
       //oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
       //if (metafile)
       //  cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
       if (0 && v->getprinterhdc() != 0)
       {
           ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * //v->getplotscale() *
                          cadwindow->printpreviewwindow->getprintscale() / double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) * 25.4;

            h /= ppscale;
            w /= ppscale;
            if (h > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT; else if (h < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT; else ih = int(h);
            if (w > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT; else if (w < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT; else iw = int(w);
            //printerhfont = CreateFontW(options.test(13) ? -ih : ih,iw,0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
            //                options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

            //printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
       }
       //if (metafile)
       //  GetTextMetricsW(cadwindow->gethdc(),&tm);
       //else
       //  GetTextMetricsW(v->gethdc(),&tm);

       maxw = 0;
       for (s = text ; *s!=0 ;  s += ll + 1)
       {
            //for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            for (ll = 0 ; s[ll] != 0; ll++)
            {
                if(s[ll] == 0x000D || s[ll] == 0x000A)
                {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }
            if (metafile)
            {
                 //SIZE size;
                 //GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                 //lw = size.cx;

                 lw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();

            }
            else
            {
                 //SIZE size;
                 //GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
                 //lw = size.cx;

                 //lw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();
                 QString qss = QString().append(s,ll);
                 rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                 lw = (int) rtw;

                 //if (0 && v->getprinterhdc() != 0)
                 //{
                 //     lw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s)).width();
                 //     lw *= ppscale;
                 //}

            }
            if (lw > maxw) maxw = lw;
            if (s[ll] == 0) break;
       }
       //SIZE size;
       //if (metafile)
       //  GetTextExtentPoint32W(cadwindow->gethdc(), s,RCSTRLEN(s),&size);
       //else
       //  GetTextExtentPoint32W(v->gethdc(), s,RCSTRLEN(s),&size);
       QRectF size = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,QString(s));

       //*hgt = size.height() * h / 100.0 * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
       //*wid = maxw * fstretch * h / 100.0 * (v->getumax() - v->getumin()) / v->getwidth();
       *hgt = size.height() * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
       *wid = maxw * fstretch * h / size.height() * (v->getumax() - v->getumin()) / v->getwidth();

       *xo = 0.0;
       if (! options.test(3) && ! options.test(4))
         *yo = -tmDescent  * h / size.height() * (v->getvmax() - v->getvmin()) / v->getheight();
       else if (options.test(3))
         *yo = -(nlines + (nlines - 1) * vs) * tmHeight  / 2;
       else
         *yo = -(nlines + (nlines - 1) * vs) * tmHeight ;
       /*
       if (metafile)
         SelectObject(cadwindow->gethdc(),cwoldhfont);
       SelectObject(v->gethdc(),oldhfont);
       DeleteObject(hfont);
       if (0 && v->getprinterhdc() != 0)
         {  SelectObject(v->getprinterhdc(),printeroldhfont);
            DeleteObject(printerhfont);
         }
        */
    }
  return 1;
#else
    RCHFONT cwoldhfont,oldhfont,hfont,printerhfont,printeroldhfont;
 BYTE fbCharSet;
#ifdef _USING_QTCHAR
 TEXTMETRICW tm;
#else
 TEXTMETRIC tm;
#endif
 int ll,maxw,iw,ih,lw,nlines,metafile;
 double scale,ppscale;
 //const char *s;
 const RCCHAR *s;
  if (options.test(10))
    {  for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       *wid = RCSTRLEN(s) * w;
       *hgt = h * (nlines + (nlines - 1) * vs);
       if (v == NULL) return 1;
       if (options.test(0))
         fbCharSet = SYMBOL_CHARSET;
       else
         fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
       hfont = CreateFontW(100,w == 0.0 || h == 0.0 ? 0 : int(100 * w / h),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,
                          OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
       hfont = CreateFont(100,w == 0.0 || h == 0.0 ? 0 : int(100 * w / h),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,
                          OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,(LPCSTR)font);
#endif
       if (hfont == NULL) return 0;
       oldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
#ifdef _USING_QTCHAR
       GetTextMetricsW(cadwindow->gethdc(),&tm);
#else
       GetTextMetrics(cadwindow->gethdc(),&tm);
#endif
       if (options.test(9) && v != 0)
         scale = v->getplotscale() / double(tm.tmHeight) * h;
       else
         scale = h / double(tm.tmHeight);
       maxw = 0;
       for (s = text ; *s!=0 ;  s += ll + 2)
         {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
            //GetTextExtentPoint(cadwindow->gethdc(),s,ll,&size);
            GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
            lw = int(size.cx * scale);
#else
            long r;
            r = GetTextExtent(cadwindow->gethdc(),s,ll);
            lw = LOWORD(r) * scale;
#endif
            if (lw > maxw) maxw = lw;
            if (s[ll] == 0) break;
         }
#if defined(WIN32)
       SIZE size;
       //GetTextExtentPoint(cadwindow->gethdc(),s,strlen(s),&size);
       GetTextExtentPoint32W(cadwindow->gethdc(), s,RCSTRLEN(s),&size);
       *hgt = size.cy * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
#else
       long r;
       r = GetTextExtent(cadwindow->gethdc(),s,strlen(s));
       *hgt = HIWORD(r) * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
#endif
       *wid = maxw * (v->getumax() - v->getumin()) / v->getwidth();
       *xo = 0.0;
       if (! options.test(3) && ! options.test(4))
         *yo = -tm.tmDescent * (v->getvmax() - v->getvmin()) / v->getheight();
       else if (options.test(3))
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight / 2;
       else
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight;
       SelectObject(cadwindow->gethdc(),oldhfont);
       DeleteObject(hfont);
    }
  else
    {  for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       *wid = RCSTRLEN(s) * w;
       *hgt = h * (nlines + (nlines - 1) * vs);
       if (v == NULL) return 1;
       if (options.test(9))
         scale = v->getplotscale();
       else
         scale = 1.0;
       h = h * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
       if (options.test(13))  // Height is in points
         h *= 25.4 / 72.0; 
       w = w * v->getwidth() / (v->getumax() - v->getumin()) * scale;
       if (h > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT; else if (h < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT; else ih = int(h);
       if (w > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT; else if (w < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT; else iw = int(w);
       if (ih == 0) return 1;
       if (options.test(0))
         fbCharSet = SYMBOL_CHARSET;
       else
         fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
       hfont = CreateFontW(100,int(100*w/(h > 0.0 ? h : 1.0)),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
       hfont = CreateFont(100,int(100*w/(h > 0.0 ? h : 1.0)),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                          options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,(LPCSTR)font);
#endif
       if (hfont == NULL) return 0;

       metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;
       oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
       if (metafile)
         cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
       if (0 && v->getprinterhdc() != 0)
         {  ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * //v->getplotscale() *
                          cadwindow->printpreviewwindow->getprintscale() / double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) * 25.4;

            h /= ppscale;
            w /= ppscale;
            if (h > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT; else if (h < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT; else ih = int(h);
            if (w > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT; else if (w < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT; else iw = int(w);
#ifdef _USING_QTCHAR
            printerhfont = CreateFontW(options.test(13) ? -ih : ih,iw,0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
            printerhfont = CreateFont(options.test(13) ? -ih : ih,iw,0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,(LPCSTR)font);
#endif
            printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
         }
#ifdef _USING_QTCHAR
       if (metafile)
         GetTextMetricsW(cadwindow->gethdc(),&tm);
       else
         GetTextMetricsW(v->gethdc(),&tm);
#else
       if (metafile)
         GetTextMetrics(cadwindow->gethdc(),&tm);
       else
         GetTextMetrics(v->gethdc(),&tm);
#endif
       maxw = 0;
       for (s = text ; *s!=0 ;  s += ll + 2)
         {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            if (metafile)
              {
#if defined(WIN32)
                 SIZE size;
#ifdef _USING_QTCHAR
                 GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
#else
                 GetTextExtentPoint(cadwindow->gethdc(),s,ll,&size);
#endif
                 lw = size.cx;
#else
                 long r;
                 r = GetTextExtent(cadwindow->gethdc(),s,ll);
                 lw = LOWORD(r);
#endif
              }
            else
              {
#if defined(WIN32)
                 SIZE size;
#ifdef _USING_QTCHAR
                 GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                 GetTextExtentPoint(v->gethdc(),s,ll,&size);
#endif
                 lw = size.cx;
                 if (0 && v->getprinterhdc() != 0)
                   {  
#ifdef _USING_QTCHAR
                       GetTextExtentPoint32W(v->getprinterhdc(), s,ll,&size);
#else
					   GetTextExtentPoint(v->getprinterhdc(),s,ll,&size);
#endif
                      lw = int(size.cx * ppscale);
                   }
#else
                 long r;
                 r = GetTextExtent(v->gethdc(),s,ll);
                 lw = LOWORD(r);
#endif
              }
            if (lw > maxw) maxw = lw;
            if (s[ll] == 0) break;
         }
#if defined(WIN32)
       SIZE size;
#ifdef _USING_QTCHAR
	   if (metafile)
         GetTextExtentPoint32W(cadwindow->gethdc(), s,RCSTRLEN(s),&size);
       else
         GetTextExtentPoint32W(v->gethdc(), s,RCSTRLEN(s),&size);
#else
	   if (metafile)
         GetTextExtentPoint(cadwindow->gethdc(),s,strlen(s),&size);
       else
         GetTextExtentPoint(v->gethdc(),s,strlen(s),&size);
#endif
       *hgt = size.cy * h / 100.0 * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
#else
       long r;
       if (metafile)
         r = GetTextExtent(cadwindow->gethdc(),s,strlen(s));
       else
         r = GetTextExtent(v->gethdc(),s,strlen(s));
       *hgt = HIWORD(r) * (v->getvmax() - v->getvmin()) / v->getheight() * (nlines + (nlines - 1) * vs);
#endif
       *wid = maxw * h / 100.0 * (v->getumax() - v->getumin()) / v->getwidth();
       *xo = 0.0;
       if (! options.test(3) && ! options.test(4))
         *yo = -tm.tmDescent  * h / 100.0 * (v->getvmax() - v->getvmin()) / v->getheight();
       else if (options.test(3))
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight * h / 100.0  / 2;
       else
         *yo = -(nlines + (nlines - 1) * vs) * tm.tmHeight * h / 100.0 ;
       if (metafile)
         SelectObject(cadwindow->gethdc(),cwoldhfont);
       SelectObject(v->gethdc(),oldhfont);
       DeleteObject(hfont);
       if (0 && v->getprinterhdc() != 0)
         {  SelectObject(v->getprinterhdc(),printeroldhfont);
            DeleteObject(printerhfont);
         }
    }
  return 1;
#endif
}

int Text::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(text_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

/**
 * @brief Text::explode
 * @return
 *
 * KMJ : Notes
 *
 * The Qt outline text can only return lines this means we might need to have platform dependent
 * solutions for Window and the Mac if spline curves are required at all cost!!
 *
 * The current version needs more work regarding the accuracy of the text positioning.
 * It might not be possible to clean this up using the current Qt methods.
 *
 */
int Text::explode(void)
{
#ifdef USING_WIDGETS
    Point p1,p2,p3,p4;
    int ll,nlines;
    double scale,y,oheight,xmaxh,xmaxw,xw;
    Point zero(0.0,0.0,0.0),o,xa,ya;
    RCCHAR *s;
    Transform tr;
    Entity *e;
    double a,angle1,rh,rtw,tmHeight,tmDescent,tmAscent;

     if (cadwindow->getcurrentwindow() == 0)
       return 0;
     xa = xaxis;  ya = yaxis;
     tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
     xa = tr.transform(xa);
     ya = tr.transform(ya);
     o = origin;

     // Qt font height is defined in points
     // calculate the point height for the font
     if(options.test(13))
         rh = height  * 25.4 / 72.0; // height is in points
     else
         rh = height;// / 0.352777778; // height is in millimeters so convert to points

#if 1
     double refh = rh; // use the real height as reference

     // calculate the scale from a reference font using the text height
     QString refName(font);
     QFont refFont(refName);
     //refFont.setHintingPreference(QFont::PreferNoHinting);
     //refFont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
     //refFont.setHintingPreference(QFont::PreferDefaultHinting);
     //refFont.setFixedPitch(true);
     //refFont.setKerning(true);
     refFont.setPointSize(refh*200.0);// use rh as points
     //refFont.setPointSize(refh);// use rh as points
     QFontMetricsF refFm(refFont);
     double refHeight = refFm.height();// get the height as pixels for rh points
     // how many points did we get for for 1 pixel of rh
     double factor = refh * 200.0 / refHeight;
     //double factor = refh / refHeight;
     // how many points to get the same height in pixels
     double fontHeight = refh * factor;

     // KMJ: Qt fonts must be greater than zero height
     // checking here for zero font height
     qDebug() << "fontHeight :" << fontHeight;
     if(fontHeight < 1.0)
         return 0;

     // set up a device independent font
     QString qfname(font);
     QFont qfont(qfname);
     //qfont.setHintingPreference(QFont::PreferNoHinting);
     qfont.setHintingPreference(QFont::PreferDefaultHinting);
     //qfont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
     //qfont.setFixedPitch(true);
     //qfont.setKerning(true);
     //qfont.setStyleStrategy(QFont::ForceOutline);
     qfont.setPointSize(fontHeight);
     // set the font weight
     if(options.test(5))
         qfont.setWeight(QFont::Bold);
     else
         qfont.setWeight(QFont::Normal);
     // set italic
     qfont.setItalic(options.test(7));
     // set underline
     qfont.setUnderline(options.test(6));

     // get the font metrics
     QFontMetricsF fm(qfont);
     tmHeight  = fm.height();
     tmDescent = fm.descent();
     tmAscent  = fm.ascent();

#else
     // alternative method to calculate the scale using the currrent screen font
     QGraphicsItemGroup *thisItem = (QGraphicsItemGroup*)qgi;
     QGraphicsSimpleTextItem *thisTextItem=0;
     QList<QGraphicsItem*> children = thisItem->childItems();
     foreach(QGraphicsItem *child, children )
     {
         if(qgraphicsitem_cast<QGraphicsSimpleTextItem*>(child) != 0)
         {
             thisTextItem = (QGraphicsSimpleTextItem*)child;
             break;
         }
     }
     if(thisTextItem == 0)
         return 0;
     QFont qfont(thisTextItem->font());
     QFontMetrics fm(qfont);
     tmHeight  = fm.height();
     tmDescent = fm.descent();
     tmAscent  = fm.ascent();
#endif


     if (options.test(9))
       scale = cadwindow->getcurrentwindow()->getplotscale() / double(tmHeight) * height;
     else
       scale = rh / double(tmHeight);

     double fstretch = 1.0;
     if(width == 0.0)
         fstretch = 1.0;
     else
         fstretch = fabs(width);

     //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
     // calculate the number of lines
     // text on windows have CR+LF line endings MAC OS X has LF line endings
     // find the line feed or carriage return or line feed and carriage return
     // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
     for (s = text,nlines = 1 ; *s!=0 ; s++)
     {
         nlines += (*s == 0x000D || *s == 0x000A) ;
         if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
             s++;
     }

#if 1
     if (! options.test(3) && ! options.test(4))// bottom
       oheight = ((nlines-1) * tmHeight) + (tmHeight * ((nlines - 1) * vertspacing)) * scale;
     else if (options.test(3)) // center
       oheight = ((nlines + (nlines - 1) * vertspacing) * tmHeight / 2 - tmHeight + tmDescent) * scale;
     else // top
       oheight = -(tmHeight - tmDescent) * scale;
#else
     if (! options.test(3) && ! options.test(4))// bottom
       oheight = ((nlines-1) * tmHeight  + ((nlines - 1) * vertspacing));// * scale;
     else if (options.test(3)) // center
       oheight = ((nlines + (nlines - 1) * vertspacing) * tmHeight / 2 - tmHeight + tmDescent) * scale;
     else // top
       oheight = -(tmHeight - tmDescent) * scale;
#endif

     xmaxh = (nlines * tmHeight) + (tmHeight * ((nlines - 1)*vertspacing)) * scale;
     xmaxw = 0;
     y = 0;
     for (s = text ; *s!=0 ;  s += ll + 1)
       {
         //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
         // text on windows have CR+LF line endings MAC OS X has LF line endings
         // find the line feed or carriage return or line feed and carriage return
         // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
         for (ll = 0 ; s[ll] != 0; ll++)
         {
             if(s[ll] == 0x000D || s[ll] == 0x000A)
             {
                 if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                     ll++;
                 break;
             }
         }

          QString qss = QString().append(s,ll);
          //rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,qss).width();
          rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
          //xw = rtw * scale;
          xw = rtw * scale;

          if (xw > xmaxw)
              xmaxw = xw;

          if (options.test(1))
            p1.setxyz(-xw*fstretch / 2.0,oheight - y,0.0);
          else if (options.test(2))
            p1.setxyz(-xw*fstretch,oheight - y,0.0);
          else
            p1.setxyz(0.0,oheight - y,0.0);

          p2 = p1;

          // drawing the outline ourselves
          QTransform stf;
          QString oString;
          oString.append(s,ll);
          // add the text to a path

          QPainterPath path;
          path.addText(0.0,0.0,qfont,oString);
          stf.translate(p2.x,-p2.y);
          stf.scale(fstretch, 1.0);
          // get the outline polygon
          QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
          // draw the outine from the polygon vertices
          for(int ip=0; ip<textpolys.size(); ip++)
          {
              QPolygonF thisPoly = textpolys.at(ip);
              // get a polygon
              for(int ie=0; ie<thisPoly.size()-1; ie++)
              {
                  // get an edge
                  QPointF qp1,qp2;
                  qp1 = thisPoly.at(ie);
                  qp2 = thisPoly.at(ie+1);

                  p3 = o + xa * (qp1.x() * scale) + ya * (-qp1.y() * scale);
                  p4 = o + xa * (qp2.x() * scale) + ya * (-qp2.y() * scale);

                  if ((e = new Line(*this,p3,p4)) != 0) db.geometry.add(e);
              }
          }
          //y += tmHeight * (fabs(vertspacing) > 0 ? vertspacing : 1.0) * scale;
          y += tmHeight * (1.0 + vertspacing) * scale;
          if (s[ll] == 0) break;
       }

     if (options.test(8))
     {
          p1 = o + ya * (oheight - y - tmDescent * scale + (tmHeight * (1.0 + vertspacing)) * scale);
          p1 = p1 - ya * (tmHeight / 10.0 * scale);
          if (options.test(1))
            p1 = p1 - xa * xmaxw * fstretch / 2.0 ;
          else if (options.test(2))
            p1 = p1 - xa * xmaxw * fstretch;
          p1 = p1 - xa * (tmHeight / 10.0 * scale);
          p2 = p1 + xa * xmaxw * fstretch;
          p2 = p2 + xa * (tmHeight / 5.0 * scale);
          p3 = p2 + ya * xmaxh;
          p3 = p3 + ya * (tmHeight / 5.0 * scale);
          p4 = p3 - xa * xmaxw * fstretch;
          p4 = p4 - xa * (tmHeight / 5.0 * scale);
          if ((e = new Line(*this,p1,p2)) != 0) db.geometry.add(e);
          if ((e = new Line(*this,p2,p3)) != 0) db.geometry.add(e);
          if ((e = new Line(*this,p3,p4)) != 0) db.geometry.add(e);
          if ((e = new Line(*this,p4,p1)) != 0) db.geometry.add(e);
     }

     return 1;
#else
#if ! defined(_WIN32_WCE)
 Point p1,p2,p3,p4;
 RCHFONT oldhfont,hfont;
 BYTE fbCharSet;
 int i,j,i1,i2,status,ll,nlines;
 double scale,y,oheight,xmaxh,xmaxw,xw;
 TEXTMETRIC tm;
 Point zero(0.0,0.0,0.0),o,xa,ya;
 //char *s,buffer[GGO_BUF_SIZE];
 char buffer[GGO_BUF_SIZE];
 RCCHAR *s;
 RCGLYPHMETRICS gm;
 MAT2 mat2;
 TTPOLYGONHEADER *ttph;
 TTPOLYCURVE *ttpc;
 Transform tr;
 Entity *e;
 BitMask opt(32);
 Point *polygon;
 UINT c;
  if (cadwindow->getcurrentwindow() == 0)
    return 0;
  xa = xaxis;  ya = yaxis;
  tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
  xa = tr.transform(xa);
  ya = tr.transform(ya);
  o = origin;
  mat2.eM11.value = 1;  mat2.eM11.fract = 0;
  mat2.eM12.value = 0;  mat2.eM12.fract = 0;
  mat2.eM21.value = 0;  mat2.eM21.fract = 0;
  mat2.eM22.value = 1;  mat2.eM22.fract = 0;
  if (options.test(0))
    fbCharSet = SYMBOL_CHARSET;
  else
    fbCharSet = ANSI_CHARSET;
  hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                     options.test(7),options.test(6),0,fbCharSet,
                     OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
  if (hfont != 0)
    oldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
  GetTextMetrics(cadwindow->gethdc(),&tm);
  if (tm.tmHeight == 0) tm.tmHeight = 1;
  if (options.test(9))
    scale = cadwindow->getcurrentwindow()->getplotscale() / double(tm.tmHeight) * height;
  else
    scale = height / double(tm.tmHeight);
  for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
  if (! options.test(3) && ! options.test(4))
    oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
  else if (options.test(3))
    oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
  else
    oheight = -(tm.tmHeight - tm.tmDescent) * scale;
  xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
  xmaxw = 0;
  y = 0;
  for (s = text ; *s!=0 ;  s += ll + 2)
    {  for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
       SIZE size;
       GetTextExtentPoint(cadwindow->gethdc(), s,ll,&size);
       xw = size.cx * scale;
#else
       long r;
       r = GetTextExtent(cadwindow->gethdc(),s,ll);
       xw = LOWORD(r) * scale;
#endif
       if (xw > xmaxw) xmaxw = xw;
       if (options.test(1))
         p1.setxyz(-xw / 2.0,oheight + y,0.0);
       else if (options.test(2))
         p1.setxyz(-xw,oheight + y,0.0);
       else
         p1.setxyz(0.0,oheight + y,0.0);
       for (i = 0 ; i < ll ; i += 1 + (tm.tmLastChar > 255))
         {  //CharNext
            if (tm.tmLastChar > 255)
              c = *((USHORT *)&s[i]);
            else
                c = RCCHAR2UINT(s[i]);
            status = int(GetGlyphOutline(cadwindow->gethdc(),c,GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
            i1 = 0;
            while (i1 < status)
              {  ttph = (TTPOLYGONHEADER *) (buffer + i1);
                 i2 = int(i1 + ttph->cb);
                 i1 += sizeof(TTPOLYGONHEADER);
                 p2 = p4 = o + xa * (p1.x + (ttph->pfxStart.x.value + ttph->pfxStart.x.fract / 65535.0) * scale) +
                               ya * (p1.y + (ttph->pfxStart.y.value + ttph->pfxStart.y.fract / 65535.0) * scale);
                 while (i1 < i2)
                   {  ttpc = (TTPOLYCURVE *) (buffer + i1);
                      if (ttpc->wType == TT_PRIM_QSPLINE)
                        {  polygon = new Point[ttpc->cpfx+1];
                           if (polygon != 0) polygon[0] = p2;
                        }
                      for (j = 0 ; j < ttpc->cpfx ; j++)
                        {  p3 = o + xa * (p1.x + (ttpc->apfx[j].x.value + ttpc->apfx[j].x.fract / 65535.0) * scale) +
                                    ya * (p1.y + (ttpc->apfx[j].y.value + ttpc->apfx[j].y.fract / 65535.0) * scale);
                           if (ttpc->wType == TT_PRIM_QSPLINE && polygon != 0)
                             polygon[j+1] = p3;
                           else
                             {  if ((e = new Line(*this,p2,p3)) != 0) db.geometry.add(e);
                             }
                           p2 = p3;
                        }
                      i1 += sizeof(TTPOLYCURVE) + (ttpc->cpfx - 1) * sizeof(POINTFX);
                      if (ttpc->wType == TT_PRIM_QSPLINE && polygon != 0)
                        {  Curve *curve = new Curve(ttpc->cpfx+1,2,polygon,0,0,opt);
                           if (curve != NULL && curve->getdefined())
                             db.geometry.add(curve);
                           else
                             delete curve;
                           delete polygon;
                        }
                   }
                 if ((e = new Line(*this,p2,p4)) != 0) db.geometry.add(e);
              }
            p1.x += gm.gmCellIncX * scale;
            p1.y += gm.gmCellIncY * scale;
         }
       y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
       if (s[ll] == 0) break;
    }
  if (options.test(8))
    {  p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
       if (options.test(1))
         p1 = p1 - xa * xmaxw / 2.0;
       else if (options.test(2))
         p1 = p1 - xa * xmaxw;
       p2 = p1 + xa * xmaxw;
       p3 = p2 + ya * xmaxh;
       p4 = p3 - xa * xmaxw;
       if ((e = new Line(*this,p1,p2)) != 0) db.geometry.add(e);
       if ((e = new Line(*this,p2,p3)) != 0) db.geometry.add(e);
       if ((e = new Line(*this,p3,p4)) != 0) db.geometry.add(e);
       if ((e = new Line(*this,p4,p1)) != 0) db.geometry.add(e);
    }
  if (hfont != 0)
    {  SelectObject(cadwindow->gethdc(),oldhfont);
       DeleteObject(hfont);
    }
  return 1;
#else
  return 0;  //  Windows CE does not support exploding text
#endif
#endif
}

/*
 * now defined in ncdialog.h
class TextDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR **fonts;
     Byte *charsets;
   public:
     TextDisplayDialogControl(int id,RCCHAR **fs,Byte *cs) : DisplayDialogControl(id) {  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void TextDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rsv0(NCVIEWA);
 View top(rsv0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 //RECT rect;
 //RCHPEN hpen,oldhpen;
 int fontid,hjust,vjust;
 double height,width,vertspacing,angle,w;
 int halocolour,c,l,s;
 c=db.header.getcolour();
 l=(int)db.header.getlayer();
 s=(int)db.header.getstyle();
 w=db.lineweights.weighttomm(db.header.getweight());
// EntityHeader header(db.header.getcolour(),0,0,db.header.getweight());
 Text *text;
 //ResourceString rs0(NCTEXTA);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);

 int nfonts,defFontid;
 RCCHAR **fonts=0;
 Byte *charsets=0;

 RCCHAR defFont[300],*df,*defText;
 double defWidth,defHeight,defVertspacing,defAngle;
 //double minHeight;
 BYTE defHalocolour;
 BitMask defOptions(32);
 int tristates[] = { 1,1,1,1,1,1,1,1 };

 if(hWnd == 0) return;

 QGraphicsView *ctl = 0;
 if(dialog->gethdlg())
   ctl = (QGraphicsView*)((Text_Dialog*)dialog->gethdlg())->getControl(124);
 QGraphicsScene *scene = 0;
 if(ctl)
     scene = ctl->scene();

 if(ctl && scene)
 {
       output.setScene(scene);
       scene->setSceneRect(QRectF());
       scene->clear();
       QRect qrect = ctl->rect();

       scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                                               ::v.getreal("wn::backgroundcolour.blue")*255.0)));

       output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                ::v.getreal("wn::backgroundcolour.green")*255.0,
                                ::v.getreal("wn::backgroundcolour.blue")*255.0));

       output.sizeevent(qrect.width(),qrect.height());
       scene->update();

       //output.moveto(0,0);
       //output.lineto(rect.right-1,0);
       //output.lineto(rect.right-1,rect.bottom-1);
       //output.lineto(0,rect.bottom-1);
       //output.lineto(0,0);
       //SelectObject(output.gethdc(),oldhpen);
       //DeleteObject(hpen);

       //minHeight = ((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(101))->minimum();

       Text::getdefaults(&df,&defWidth,&defHeight,&defVertspacing,&defAngle,&defHalocolour,&defText,&defOptions);
       strcpy(defFont,df);

       if (! Text::getfonts(defFont,&nfonts,&fonts,&charsets,&defFontid)) return;

       options.clearall();
       if (
           dialog->currentvalue(100,&c ) &&
           dialog->currentvalue(101,&l ) &&
           dialog->currentvalue(102,&s ) &&
           dialog->currentvalue(103,&w ) &&
           dialog->currentvalue(105,&fontid ) &&
           dialog->currentvalue(106,&height) &&
           dialog->currentvalue(107,&width) &&
           dialog->currentvalue(108,&angle) &&
           dialog->currentvalue(109,&hjust) &&
           dialog->currentvalue(110,&vjust) &&
           dialog->currentvalue(111,&vertspacing) &&
           dialog->currentvalue(112,&tristates[0]) &&
           dialog->currentvalue(113,&tristates[1]) &&
           dialog->currentvalue(114,&tristates[2]) &&
           dialog->currentvalue(115,&tristates[3]) &&
           dialog->currentvalue(116,&tristates[4]) &&
           dialog->currentvalue(122,&tristates[6]) &&
           dialog->currentvalue(123,&tristates[7]) &&
           dialog->currentvalue(121,&halocolour))
         {
           if( strlen(((FontListDialogControl*)dialog->getcontrol(105))->getSpecialValueText()) != 0 &&
              ((QComboBox*)((Text_Dialog*)dialog->gethdlg())->getControl(1005))->isVisible() )
               fontid = defFontid;

           if(!((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
              height == ((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
               height = defHeight;

           if(!((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(107))->specialValueText().isEmpty() &&
              width == ((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(107))->minimum() )
               width = defWidth;

           if(!((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(108))->specialValueText().isEmpty() &&
              angle == ((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(108))->minimum() )
               angle = defAngle;

           if(!((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(111))->specialValueText().isEmpty() &&
              vertspacing == ((QDoubleSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(111))->minimum() )
               vertspacing = defVertspacing;

           if(!((QSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(121))->specialValueText().isEmpty() &&
              halocolour == ((QSpinBox*)((Text_Dialog*)dialog->gethdlg())->getControl(121))->minimum() )
               halocolour = (int)defHalocolour;

           if(((QComboBox*)((Text_Dialog*)dialog->gethdlg())->getControl(1009))->isVisible())
               hjust = defOptions.test(1) + defOptions.test(2) * 2;

           if(((QComboBox*)((Text_Dialog*)dialog->gethdlg())->getControl(1010))->isVisible())
               vjust = defOptions.test(3) + defOptions.test(4) * 2;

           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(112))->isTristate())
               tristates[0] == 1 ? options.set(5,defOptions.test(5)) : options.set(5,tristates[0]);
           else
               options.set(5,tristates[0]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(113))->isTristate())
               tristates[1] == 1 ? options.set(6,defOptions.test(6)) : options.set(6,tristates[1]);
           else
               options.set(6,tristates[1]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(114))->isTristate())
                tristates[2] == 1 ? options.set(7,defOptions.test(7)) : options.set(7,tristates[2]);
           else
                options.set(7,tristates[2]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(115))->isTristate())
                tristates[3] == 1 ? options.set(8,defOptions.test(8)) : options.set(8,tristates[3]);
           else
               options.set(8,tristates[3]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(116))->isTristate())
               tristates[4] == 1 ? options.set(10,defOptions.test(10)) : options.set(10,tristates[4]);
           else
               options.set(10,tristates[4]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(122))->isTristate())
               tristates[6] == 1 ? options.set(12,defOptions.test(12)) : options.set(12,tristates[6]);
           else
               options.set(12,tristates[6]);
           if(((QCheckBox*)((Text_Dialog*)dialog->gethdlg())->getControl(123))->isTristate())
               tristates[7] == 1 ? options.set(13,defOptions.test(13)) : options.set(13,tristates[7]);
           else
               options.set(13,tristates[7]);

            //options.set(0,charsets[fontid] == SYMBOL_CHARSET);
            options.set(1,hjust % 2);
            options.set(2,hjust / 2);
            options.set(3,vjust % 2);
            options.set(4,vjust / 2);
            switch (hjust)
            {
                 case 0 : org.x = output.getumin() + (output.getumax() - output.getumin()) / 20.0;  break;
                 case 1 : org.x = (output.getumin() + output.getumax()) / 2.0;  break;
                 case 2 : org.x = output.getumax() - (output.getumax() - output.getumin()) / 20.0;  break;
            }
            switch (vjust)
            {
                 case 0 : org.y = output.getvmin() + (output.getvmax() - output.getvmin()) / 20.0;  break;
                 case 1 : org.y = (output.getvmin() + output.getvmax()) / 2.0;  break;
                 case 2 : org.y = output.getvmax() - (output.getvmax() - output.getvmin()) / 20.0;  break;
            }

            PointE point(org);
            point.draw(DM_NORMAL,NULL,&output);

            EntityHeader header(c,l,s,db.lineweights.mmtoweight(w));
            text = new Text(header,fonts[fontid],org,xaxis,yaxis,width,height,vertspacing,angle,(BYTE)halocolour,CHAR2RCCHR("ABCDEFGHIJK\nabcdefghijk\n1234567890"),(RCCHAR*)0,options);

            text->draw(DM_NORMAL,NULL,&output);
            delete text;
         }

         for (int i = 0 ; i < nfonts ; i++) delete [] fonts[i];
         delete [] fonts;
         //delete [] charsets;
    }

}

/*
 * defined in ncDialog.h
class FontListDialogControl : public ListDialogControl
  {private:
   public:
     QString font;
     FontListDialogControl(int id,int n,RCCHAR **l,int *f) : ListDialogControl(id,n,l,f) {}
     void load(Dialog *dialog);
     void changefocus(Dialog *dialog,int updatescreen = 1);
  };
*/
void FontListDialogControl::change(Dialog *dialog,int i)
{
    ListDialogControl::change(dialog,i);
}

void FontListDialogControl::load(Dialog *dialog)
{
    //ListDialogControl::load(dialog);
    ListDialogControl::change(dialog,*value);
}

void FontListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{
}

void FontListDialogControl::setSpecialValueText(RCCHAR *text)
{
    strcpy(specialValueText,text);
}

RCCHAR *FontListDialogControl::getSpecialValueText()
{
    return specialValueText;
}

void FontListDialogControl::setDefaultFont(RCCHAR *dfont)
{
    strcpy(defaultFont,dfont);
}

RCCHAR *FontListDialogControl::getDefaultFont()
{
    return defaultFont;
}

class TextSetupButtonControl : public ButtonDialogControl
  {public:
     TextSetupButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };

int TextSetupButtonControl::select(Dialog *dialog)
{
    int state,halocolour;

    state = 1001;
    if (id == 1004)
    { // halo colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(121))->currentvalue(dialog,&halocolour);
        general_property_command(&state,0,(void **)&halocolour);
        if(((QSpinBox*)dialog->getcontrol(121)->getQW())->specialValueText().length() > 0)
            ((IntegerDialogControl *)dialog->getcontrol(121))->change(dialog,((QSpinBox*)dialog->getcontrol(121)->getQW())->maximum());
        ((IntegerDialogControl *)dialog->getcontrol(121))->change(dialog,halocolour);
    }
    return 0;
}


/**
 * @brief Text::setup
 * @param change
 * @param font
 * @param width
 * @param height
 * @param vertspacing
 * @param angle
 * @param halocolour
 * @param options
 * @return
 * the text setup dialog command
 *
 * when entites are pre-selected their option valies are scanned.
 * if all selected entities options values are the same, these values are shown in the dialog
 * if some values are different the text "*Varies*" is shown in the dialog
 * when values vary...
 * numbers and text display"*Varies*"
 * check boxes are shown in tristate mode
 */
#if 1
int Text::setup(BitMask *change,RCCHAR *font,double *width,double *height,double *vertspacing,double *angle,Byte *halocolour,BitMask *options,RCCHAR *text,RCCHAR *label,EntityHeader *header)
{
    //Dialog dialog(_RCT("Text_Dialog"));
    Dialog dialog(_RCT("TextVerify_Dialog"));
    DialogControl *dc;
    int i,hjust,vjust,status,nfonts,fontid,hc;
    int dummyHjust,dummyVjust;
    RCCHAR **fonts=0;
    Byte *charsets=0;
    ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
    ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
    ResourceString rsVaries(4107),rsDefault(4108),rsOther(4109);
    RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
    RCCHAR *hjustlv[] = {  rsVaries.getws(),rs7.getws(),rs8.getws(),rs9.getws() };
    RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
    RCCHAR *vjustlv[] = {  rsVaries.getws(),rs10.getws(),rs11.getws(),rs12.getws() };
    RCCHAR *dummyfonts[] = { rsVaries.getws(),rsDefault.getws(),rsOther.getws() };
    int dummyid=0;
    ResourceString rs1(4111),rs2(NCTEXTA+1);

    int c,l,s;
    double w;

    RCCHAR ntext[4096],nlabel[300],defFont[300],*df,*defText;
    double defWidth,defHeight,defVertSpacing,defAngle,defWeight;
    int defColour,defLayer,defStyle;
    BYTE defHalocolour;
    BitMask defOptions(32),changeIn(32);
    int tristates[] = { 1,1,1,1,1,1,1,1 };

    EntityHeader entheader;
    if(header != 0)
        header->verify(&dialog,&c,&l,&s,&w);
    else
        entheader.verify(&dialog,&c,&l,&s,&w);

    int changecount = v.getinteger("op::changecount");

    // get the default values
    defColour = db.header.getcolour();
    defLayer  = db.header.getlayer();
    defStyle  = db.header.getstyle();
    defWeight = db.lineweights.weighttomm(db.header.getweight());
    Text::getdefaults(&df,&defWidth,&defHeight,&defVertSpacing,&defAngle,&defHalocolour,&defText,&defOptions);
    strcpy(defFont,df);
    strcat(dummyfonts[1],df);

    // look at the change bits and see what value vary among any pre-slected entities
    if(change != NULL)
    {
        // if test is true the values dont vary ie. the dialog will show the incoming values
        // otherwise false ie. the values vary so the dialog will show the *Varies* message
        // the entity values will not be modified if the dialog values are not changed
        //
        // here we are setting the controls to there default settings
        // text boxes and combo boxes will show *Varies*
        // check boxes will be in tri state mode
        if(!change->test(0))
            c = 0;
        if(!change->test(1))
            l = 0;
        if(!change->test(2))
            s = 0;
        if(!change->test(3))
            w = 0.10;

        if(!change->test(4))
            strcpy(font,defFont);
        if(!change->test(8))
            *height = 0.0;
        if(!change->test(9))
            *width = -10000.0;
        if(!change->test(10))
            *angle = -360.0;
        if(!change->test(13))
            *vertspacing = -100000000.0;
        if(!change->test(22))
            *halocolour = 0;
        if(!change->test(11))
        {
            hjust = 0;
            dummyHjust = 0;
        }
        else
        {
            hjust = options->test(1) + options->test(2) * 2;
            dummyHjust = 0;
        }
        if(!change->test(12))
        {
            vjust = 0;
            dummyVjust = 0;
        }
        else
        {
            vjust = options->test(3) + options->test(4) * 2;
            dummyVjust = 0;
        }
        if(change->test(14))
            options->test(5)==0  ? tristates[0] = 0 : tristates[0] = 2;
        if(change->test(15))
            options->test(6)==0  ? tristates[1] = 0 : tristates[1] = 2;
        if(change->test(16))
            options->test(7)==0  ? tristates[2] = 0 : tristates[2] = 2;
        if(change->test(17))
            options->test(8)==0  ? tristates[3] = 0 : tristates[3] = 2;
        if(change->test(18))
            options->test(10)==0 ? tristates[4] = 0 : tristates[4] = 2;
        if(change->test(19))
            options->test(9)==0  ? tristates[5] = 0 : tristates[5] = 2;
        if(change->test(21))
            options->test(12)==0 ? tristates[6] = 0 : tristates[6] = 2;
        if(change->test(23))
            options->test(13)==0 ? tristates[7] = 0 : tristates[7] = 2;
    }
    else
    {
        dummyHjust = 0;
        dummyVjust = 0;
        hjust = options->test(1) + options->test(2) * 2;
        vjust = options->test(3) + options->test(4) * 2;
        defOptions.test(5)==0  ? tristates[0] = 0 : tristates[0] = 1;
        defOptions.test(6)==0  ? tristates[1] = 0 : tristates[1] = 1;
        defOptions.test(7)==0  ? tristates[2] = 0 : tristates[2] = 1;
        defOptions.test(8)==0  ? tristates[3] = 0 : tristates[3] = 1;
        defOptions.test(10)==0 ? tristates[4] = 0 : tristates[4] = 1;
        defOptions.test(9)==0  ? tristates[5] = 0 : tristates[5] = 1;
        defOptions.test(12)==0 ? tristates[6] = 0 : tristates[6] = 1;
        defOptions.test(13)==0 ? tristates[7] = 0 : tristates[7] = 1;
    }

     if (! Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) return 0;

     dialog.add(new FontListDialogControl(105,nfonts,fonts,&fontid));
     dialog.add(new ListDialogControl(1005,2,dummyfonts,&dummyid));
     dialog.add(dc = new RealDialogControl(106,height,ScaleByLength));
     dialog.add(dc = new RealDialogControl(107,width,ScaleByLength));
     dialog.add(dc = new RealDialogControl(108,angle));
     dialog.add(new ListDialogControl(109,3,hjustl,&hjust));
     dialog.add(new ListDialogControl(110,3,vjustl,&vjust));
     dialog.add(dc = new RealDialogControl(111,vertspacing,0.1));
     dialog.add(new ButtonDialogControl(117)); // clone
     dialog.add(new ButtonDialogControl(118)); // clone all
     dialog.add(new TextVerifyButtonControl(1004)); // halo colour selector button
     hc = *halocolour;
     dialog.add(dc = new IntegerDialogControl(121,&hc)); // halo colour spin box
     Colour gc(db.colourtable[hc].red,db.colourtable[hc].green,db.colourtable[hc].blue);
     dialog.add(new ColourSampleControl(125,&dialog,&gc)); // halo colour sample
     dialog.add((DisplayDialogControl *) new TextDisplayDialogControl(124,fonts,charsets,change));

     if (change != NULL)
     {
         // add the verify features
         dialog.add(new TextVerifyButtonControl(1000));
         dialog.add(new TextVerifyButtonControl(1001));
         dialog.add(new TextVerifyButtonControl(1002));
         dialog.add(new TextVerifyButtonControl(1003));
         // add the text verify features if only one entity is selected
         if(changecount == 1)
         {
             //dialog.add(new TextVerifyButtonControl(1004));
             // set a the label flag for the dialog to pick up
             strcpy(ntext,text);
             v.setinteger("tx::label",0);
             if (label == 0 || strlen(label) == 0 )
               dialog.add(new StringDialogControl(104, ntext,4096));
               //dialog.add(new StringDialogControl(104, ntext,RCSTRLEN(text)+1));
             else
             {
                  v.setinteger("tx::label",1);
                  strcpy(ntext,text);
                  strcpy(nlabel,label);

                  //dialog.add(new StringDialogControl(104, text,RCSTRLEN(text)+1));
                  dialog.add(new StringDialogControl(104, ntext,4096));
                  dialog.add(new StringDialogControl(119, nlabel,300));
                  //dialog.add(new StringDialogControl(219,rs2.gets(),strlen(rs2.gets())+1));
             }
         }

         if(changecount > 1 && !change->test(0)) // colour
         {
             ((IntegerDialogControl*)dialog.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
             ((IntegerDialogControl*)dialog.getcontrol(100))->setDefaultValue(defColour);
         }
         if(changecount > 1 && !change->test(1)) // layer
         {
             ((IntegerDialogControl*)dialog.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
             ((IntegerDialogControl*)dialog.getcontrol(101))->setDefaultValue(defLayer);
         }
         if(changecount > 1 && !change->test(2)) // style
         {
             ((IntegerDialogControl*)dialog.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
             ((IntegerDialogControl*)dialog.getcontrol(102))->setDefaultValue(defStyle);
         }
         if(changecount > 1 && !change->test(3)) // weight
         {
             ((WeightAssocRealDialogControl*)dialog.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
             ((WeightAssocRealDialogControl*)dialog.getcontrol(103))->setDefaultValue(defWeight);
         }

         if(changecount > 1 && !change->test(4)) // font
         {
             ((FontListDialogControl*)dialog.getcontrol(105))->setSpecialValueText(_RCT("*Varies*"));
             ((FontListDialogControl*)dialog.getcontrol(105))->setDefaultFont(defFont);
         }
         if(changecount > 1 && !change->test(8)) // height
         {
             ((RealDialogControl*)dialog.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
             ((RealDialogControl*)dialog.getcontrol(106))->setDefaultValue(defHeight);
         }
         if(changecount > 1 && !change->test(9)) // width
         {
             ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
             ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defWidth);
         }
         if(changecount > 1 && !change->test(10)) // angle
         {
             ((RealDialogControl*)dialog.getcontrol(108))->setSpecialValueText(_RCT("*Varies*"));
             ((RealDialogControl*)dialog.getcontrol(108))->setDefaultValue(defAngle);
         }
         if(changecount > 1 && !change->test(13)) // vertspacing
         {
             ((RealDialogControl*)dialog.getcontrol(111))->setSpecialValueText(_RCT("*Varies*"));
             ((RealDialogControl*)dialog.getcontrol(111))->setDefaultValue(defVertSpacing);
         }
         if(changecount > 1 && !change->test(22)) // halo colour
         {
             ((IntegerDialogControl*)dialog.getcontrol(121))->setSpecialValueText(_RCT("*Varies*"));
             ((IntegerDialogControl*)dialog.getcontrol(121))->setDefaultValue((int)defHalocolour);
         }
         //dialog.add(new ListDialogControl(109,3,hjustl,&hjust));
         if(changecount > 1 && !change->test(11)) // hjust
             dialog.add(new ListDialogControl(1009,2,hjustlv,&dummyHjust));
         //dialog.add(new ListDialogControl(110,3,vjustl,&vjust));
         if(changecount > 1 && !change->test(12)) // hjust
             dialog.add(new ListDialogControl(1010,2,vjustlv,&dummyVjust));
         // use tristate check boxes depending on the value
         dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[0],changecount>=1^change->test(14),defOptions.test(5)/*,5*/));  // bold
         dialog.add(new TriStateCheckBoxDialogControl(113,&tristates[1],changecount>=1^change->test(15),defOptions.test(6)/*,6*/));  // underline
         dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[2],changecount>=1^change->test(16),defOptions.test(7)/*,7*/));  // italic
         dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[3],changecount>=1^change->test(17),defOptions.test(8)/*,8*/));  // boxed
         dialog.add(new TriStateCheckBoxDialogControl(116,&tristates[4],changecount>=1^change->test(18),defOptions.test(10)/*,10*/)); // outline
         dialog.add(new TriStateCheckBoxDialogControl(120,&tristates[5],changecount>=1^change->test(19),defOptions.test(9)/*,9*/));  // scaled
         dialog.add(new TriStateCheckBoxDialogControl(122,&tristates[6],changecount>=1^change->test(21),defOptions.test(12)/*,12*/)); // halo
         dialog.add(new TriStateCheckBoxDialogControl(123,&tristates[7],changecount>=1^change->test(23),defOptions.test(13)/*,13*/)); // height in points
     }
     else
     {
         // use the normal font combo box here
         //dialog.add(new FontListDialogControl(100,nfonts,fonts,&fontid));
         // use the normal hjust list
         //dialog.add(new ListDialogControl(109,3,hjustl,&hjust));
         // use the normal vjust list
         //dialog.add(new ListDialogControl(110,3,vjustl,&vjust));
         // always use tristate check boxes with only two states
         dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[0],false,defOptions.test(5)/*,5*/));  // bold
         dialog.add(new TriStateCheckBoxDialogControl(113,&tristates[1],false,defOptions.test(6)/*,6*/));  // underline
         dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[2],false,defOptions.test(7)/*,7*/));  // italic
         dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[3],false,defOptions.test(8)/*,8*/));  // boxed
         dialog.add(new TriStateCheckBoxDialogControl(116,&tristates[4],false,defOptions.test(10)/*,10*/)); // outline
         dialog.add(new TriStateCheckBoxDialogControl(120,&tristates[5],false,defOptions.test(9)/*,9*/));  // scaled
         dialog.add(new TriStateCheckBoxDialogControl(122,&tristates[6],false,defOptions.test(12)/*,12*/)); // halo
         dialog.add(new TriStateCheckBoxDialogControl(123,&tristates[7],false,defOptions.test(13)/*,13*/)); // height in points
     }

     dialog.title(rs1.gets());
     // set the change value for the dialog to read when it opens
     v.setinteger("op::change",0);
     v.setbm("op::changemask",defOptions);
     if (change != NULL)
     {
          // set the change value for the dialog to read when it opens
          v.setinteger("op::change",1);
          v.setbm("op::changemask",*change);
          if(changecount == 1)
              dialog.title(_RCT("Text Verify"));
          else
             dialog.title(rs2.gets());
     }

     status = dialog.process();
     if (status == TRUE)
     {
          if(change != NULL)
          {
              // reset the outgoing change values
              changeIn = *change;
              change->clearall();

              // set the new text value if only one entity was pre selected
              if(changecount == 1)
              {
                  for (int i = 0 ; i < 4 ; i++)
                    change->set(i,1);

                  header->setcolour(c);
                  header->setstyle(s);
                  header->setlayer(l);
                  header->setweight(db.lineweights.mmtoweight(w));

                  if (label == 0 || strlen(label) == 0 )
                  {
                      change->set(31);
                      strcpy(text,ntext);
                  }
                  else
                  {
                      change->set(31);
                      change->set(30);
                      strcpy(text,ntext);
                      strcpy(label,nlabel);
                  }

              }

              if( changecount > 1 && !changeIn.test(0) &&
                  strlen(((IntegerDialogControl*)dialog.getcontrol(100))->getSpecialValueText()) != 0 )
                  change->clear(0);
              else
              {
                  change->set(0);
                  header->setcolour(c);
              }
              if( changecount > 1 && !changeIn.test(1) &&
                  strlen(((IntegerDialogControl*)dialog.getcontrol(101))->getSpecialValueText()) != 0 )
                  change->clear(1);
              else
              {
                  change->set(1);
                  header->setlayer(l);
              }
              if( changecount > 1 && !changeIn.test(2) &&
                  strlen(((IntegerDialogControl*)dialog.getcontrol(102))->getSpecialValueText()) != 0 )
                  change->clear(2);
              else
              {
                  change->set(2);
                  header->setstyle(s);
              }
              if( changecount > 1 && !changeIn.test(3) &&
                  strlen(((IntegerDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
                  change->clear(3);
              else
              {
                  change->set(3);
                  header->setweight(db.lineweights.mmtoweight(w));
              }

               if( changecount > 1 && !changeIn.test(4) && dummyid == 0 )
                   change->clear(4);
               else
               {
                   change->set(4);
                   strcpy(font,fonts[fontid]);
               }
               if( changecount > 1 && !changeIn.test(8) &&
                   strlen(((RealDialogControl*)dialog.getcontrol(106))->getSpecialValueText()) != 0 ) // not changed
               //if(fabs(0.0 - *height) < 10.e-5) // not changed
                   change->clear(8);
               else
                   change->set(8);
               if( changecount > 1 && !changeIn.test(9) &&
                   strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0 ) // not changed
               //if(fabs(-10000.0 - *width) < 10.e-5) // not changed
                   change->clear(9);
               else
                   change->set(9);
               if( changecount > 1 && !changeIn.test(10) &&
                   strlen(((RealDialogControl*)dialog.getcontrol(108))->getSpecialValueText()) != 0 ) // not changed
               //if(fabs(-360.0 - *angle) < 10.e-5) // not changed
                   change->clear(10);
               else
                   change->set(10);
               if( changecount > 1 && !changeIn.test(13) &&
                   strlen(((RealDialogControl*)dialog.getcontrol(111))->getSpecialValueText()) != 0 ) // not changed
               //if(fabs(-10000.0 - *vertspacing) < 10.e-5) // not changed
                   change->clear(13);
               else
                   change->set(13);
               if( changecount > 1 && !changeIn.test(22) &&
                   strlen(((IntegerDialogControl*)dialog.getcontrol(121))->getSpecialValueText()) != 0 ) // not changed
                   change->clear(22);
               else
               {
                   change->set(22);
                   *halocolour = (Byte) hc;
               }
               if(changecount > 1 && !changeIn.test(11) && dummyHjust == 0) // not changed
                   change->clear(11);
               else
               {
                   //if(changecount > 1 && ((ListDialogControl*)dialog.getcontrol(109))->getLength() == 4)
                   //    hjust--;
                   options->set(1,hjust % 2);
                   options->set(2,hjust / 2);
                   change->set(11);
               }
               if(changecount > 1 && !changeIn.test(12) && dummyVjust == 0) // not changed
                   change->clear(12);
               else
               {
                   //if(changecount > 1 && ((ListDialogControl*)dialog.getcontrol(110))->getLength() == 4)
                   //    vjust--;
                   options->set(3,vjust % 2);
                   options->set(4,vjust / 2);
                   change->set(12);
               }

               // if the user did not change a value the tristate == 1 and the default value is used
               // otherwise the tristate value is used to set these options
               if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(112))->isTriState())
                   change->clear(14);
               else
               {
                   change->set(14);
                   options->set(5, tristates[0]);
               }
               if(tristates[1] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(113))->isTriState())
                   change->clear(15);
               else
               {
                   change->set(15);
                   options->set(6, tristates[1]);
               }
               if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(114))->isTriState())
                   change->clear(16);
               else
               {
                   change->set(16);
                   options->set(7, tristates[2]);
               }
               if(tristates[3] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(115))->isTriState())
                   change->clear(17);
               else
               {
                   change->set(17);
                   options->set(8, tristates[3]);
               }
               if(tristates[4] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(116))->isTriState())
                   change->clear(18);
               else
               {
                   change->set(18);
                   options->set(10, tristates[4]);
               }
               if(tristates[5] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(120))->isTriState())
                   change->clear(19);
               else
               {
                   change->set(19);
                   options->set(9, tristates[5]);
               }
               // halo on off
               if(tristates[6] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(122))->isTriState())
                   change->clear(21);
               else
               {
                   change->set(21);
                   options->set(12,tristates[6]);
               }
               // height in points
               if(tristates[7] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(123))->isTriState())
                   change->clear(23);
               else
               {
                   change->set(23);
                   options->set(13,tristates[7]);
               }
          }
          else
          {
              options->set(5, tristates[0]);
              options->set(6, tristates[1]);
              options->set(7, tristates[2]);
              options->set(8, tristates[3]);
              options->set(10,tristates[4]);
              options->set(9, tristates[5]);
              options->set(12,tristates[6]);
              options->set(13,tristates[7]);

              RCSTRCPY(font,fonts[fontid]);
              //options->set(0,charsets[fontid] == SYMBOL_CHARSET);
              options->set(0,0);
              options->set(1,hjust % 2);
              options->set(2,hjust / 2);
              options->set(3,vjust % 2);
              options->set(4,vjust / 2);
              *halocolour = (Byte) hc;
          }
    }
    else
    {
         if(change != NULL)
            change->clearall();
    }

     for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
     delete [] fonts;
     //delete [] charsets;

     return status;
}
#else
int Text::setup(BitMask *change,RCCHAR *font,double *width,double *height,double *vertspacing,double *angle,Byte *halocolour,BitMask *options)
{
#ifdef USING_WIDGETS
    Dialog dialog(_RCT("Text_Dialog"));
    DialogControl *dc;
    int i,hjust,vjust,status,nfonts,fontid,hc;
    RCCHAR **fonts;
    Byte *charsets;
    ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
    ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
    RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
    RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
    ResourceString rs1(NCTEXTA+1);


     if (! Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) return 0;
     /*
     RCCHAR och[1000];
     RCSTRCPY(och,"");
     for(int ift=0; ift<5; ift++)
     {
         RCSTRCAT(och,fonts[ift]);
         RCSTRCAT(och,CHAR2RCCHR("\r"));
     }
     if (fonts == NULL) return 0;
     */
     hjust = options->test(1) + options->test(2) * 2;
     vjust = options->test(3) + options->test(4) * 2;
     dialog.add(new FontListDialogControl(100,nfonts,fonts,&fontid));
     dialog.add(dc = new RealDialogControl(101,height,ScaleByLength));
     //dialog.add(new ScrollBarDialogControl(1101,dc));
     dialog.add(dc = new RealDialogControl(102,width,ScaleByLength));
     //dialog.add(new ScrollBarDialogControl(1102,dc));
     dialog.add(dc = new RealDialogControl(103,angle));
     //dialog.add(new ScrollBarDialogControl(1103,dc));
     dialog.add(new ListDialogControl(104,3,hjustl,&hjust));
     dialog.add(new ListDialogControl(105,3,vjustl,&vjust));
     dialog.add(dc = new RealDialogControl(106,vertspacing,0.1));
     //dialog.add(new ScrollBarDialogControl(1106,dc));
     dialog.add(new CheckBoxDialogControl(107,options,5));
     dialog.add(new CheckBoxDialogControl(108,options,6));
     dialog.add(new CheckBoxDialogControl(109,options,7));
     dialog.add(new CheckBoxDialogControl(110,options,8));
     dialog.add(new CheckBoxDialogControl(111,options,10));
     dialog.add(new CheckBoxDialogControl(112,options,9));
     dialog.add(new CheckBoxDialogControl(115,options,12));
     dialog.add(new CheckBoxDialogControl(117,options,13));
     dialog.add(new ButtonDialogControl(114));
     dialog.add(new ButtonDialogControl(118));
     hc = *halocolour;
     dialog.add(dc = new IntegerDialogControl(116,&hc));
     //dialog.add(new ScrollBarDialogControl(1116,dc));
     //dialog.add((DisplayDialogControl *) new TextDisplayDialogControl(113,fonts,charsets));
     // set the change value for the dialog to read when it opens
     v.setinteger("op::change",0);
     if (change != NULL)
       {
          // set the change value for the dialog to read when it opens
          v.setinteger("op::change",1);
          dialog.title(rs1.gets());
          change->clearall();
          dialog.add(new CheckBoxDialogControl(200,change,4));
          dialog.add(new CheckBoxDialogControl(201,change,8));
          dialog.add(new CheckBoxDialogControl(202,change,9));
          dialog.add(new CheckBoxDialogControl(203,change,10));
          dialog.add(new CheckBoxDialogControl(204,change,11));
          dialog.add(new CheckBoxDialogControl(205,change,12));
          dialog.add(new CheckBoxDialogControl(206,change,13));
          dialog.add(new CheckBoxDialogControl(207,change,14));
          dialog.add(new CheckBoxDialogControl(208,change,15));
          dialog.add(new CheckBoxDialogControl(209,change,16));
          dialog.add(new CheckBoxDialogControl(210,change,17));
          dialog.add(new CheckBoxDialogControl(211,change,18));
          dialog.add(new CheckBoxDialogControl(212,change,19));
          dialog.add(new CheckBoxDialogControl(215,change,21));
          dialog.add(new CheckBoxDialogControl(216,change,22));
          dialog.add(new CheckBoxDialogControl(217,change,23));
       }
     status = dialog.process();
     if (status == TRUE)
     {
          RCSTRCPY(font,fonts[fontid]);
          //options->set(0,charsets[fontid] == SYMBOL_CHARSET);
          options->set(0,0);
          options->set(1,hjust % 2);
          options->set(2,hjust / 2);
          options->set(3,vjust % 2);
          options->set(4,vjust / 2);
          *halocolour = (Byte) hc;
    }

     //for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
     //delete [] fonts;
     //delete [] charsets;

     return status;
#else
 Dialog dialog(_RCT("Text_Dialog"));
 DialogControl *dc;
 int i,hjust,vjust,status,nfonts,fontid,hc;
 RCCHAR **fonts;
 Byte *charsets;
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
 RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 ResourceString rs1(NCTEXTA+1);

  if (! Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) return 0;
#ifdef _USING_QTCHAR
  RCCHAR och[1000];
#else
  char och[1000];
#endif
  RCSTRCPY(och,"");
  for(int ift=0; ift<5; ift++)
  {
	  RCSTRCAT(och,fonts[ift]);
	  RCSTRCAT(och,CHAR2RCCHR("\r"));
  }
#ifdef _USING_QTCHAR
  printQChar(och);
#endif
  if (fonts == NULL) return 0;
  hjust = options->test(1) + options->test(2) * 2;
  vjust = options->test(3) + options->test(4) * 2;
#ifdef _USING_QTCHAR
  //dialog.add(new RCListDialogControl(100,nfonts,fonts,&fontid));
  dialog.add(new ListDialogControl(100,nfonts,fonts,&fontid));
#else
  dialog.add(new ListDialogControl(100,nfonts,fonts,&fontid));
#endif
  dialog.add(dc = new RealDialogControl(101,height,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,width,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,angle));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new ListDialogControl(104,3,hjustl,&hjust));
  dialog.add(new ListDialogControl(105,3,vjustl,&vjust));
  dialog.add(dc = new RealDialogControl(106,vertspacing,0.1));
  //dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(new CheckBoxDialogControl(107,options,5));
  dialog.add(new CheckBoxDialogControl(108,options,6));
  dialog.add(new CheckBoxDialogControl(109,options,7));
  dialog.add(new CheckBoxDialogControl(110,options,8));
  dialog.add(new CheckBoxDialogControl(111,options,10));
  dialog.add(new CheckBoxDialogControl(112,options,9));
  dialog.add(new CheckBoxDialogControl(115,options,12));
  dialog.add(new CheckBoxDialogControl(117,options,13));
  dialog.add(new ButtonDialogControl(114));
  dialog.add(new ButtonDialogControl(118));
  hc = *halocolour;
  dialog.add(dc = new IntegerDialogControl(116,&hc));
  //dialog.add(new ScrollBarDialogControl(1116,dc));
  dialog.add((DisplayDialogControl *) new TextDisplayDialogControl(113,fonts,charsets));
  if (change != NULL)
    {  dialog.title(rs1.gets());
       change->clearall();
       dialog.add(new CheckBoxDialogControl(200,change,4));
       dialog.add(new CheckBoxDialogControl(201,change,8));
       dialog.add(new CheckBoxDialogControl(202,change,9));
       dialog.add(new CheckBoxDialogControl(203,change,10));
       dialog.add(new CheckBoxDialogControl(204,change,11));
       dialog.add(new CheckBoxDialogControl(205,change,12));
       dialog.add(new CheckBoxDialogControl(206,change,13));
       dialog.add(new CheckBoxDialogControl(207,change,14));
       dialog.add(new CheckBoxDialogControl(208,change,15));
       dialog.add(new CheckBoxDialogControl(209,change,16));
       dialog.add(new CheckBoxDialogControl(210,change,17));
       dialog.add(new CheckBoxDialogControl(211,change,18));
       dialog.add(new CheckBoxDialogControl(212,change,19));
       dialog.add(new CheckBoxDialogControl(215,change,21));
       dialog.add(new CheckBoxDialogControl(216,change,22));
       dialog.add(new CheckBoxDialogControl(217,change,23));
    }
  status = dialog.process();
  if (status == TRUE)
    {  RCSTRCPY(font,fonts[fontid]);
       options->set(0,charsets[fontid] == SYMBOL_CHARSET);
       options->set(1,hjust % 2);
       options->set(2,hjust / 2);
       options->set(3,vjust % 2);
       options->set(4,vjust / 2);
       *halocolour = (Byte) hc;
    }

  for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
  delete [] fonts;
  delete [] charsets;

  return status;
#endif
}
#endif

#ifdef _USING_QTCHAR
void Text::getdefaults(char **f,double *w,double *h,double *s,double *a,Byte *hc,char **t,BitMask *o)
{
    //getdefaults((RCCHAR**)CHAR2RCCHR(*f),w,h,s,a,hc,(RCCHAR**)CHAR2RCCHR(*t),o);
    getdefaults((RCCHAR**)f,w,h,s,a,hc,(RCCHAR**)CHAR2RCCHR(*t),o);
}
#endif

void Text::getdefaults(RCCHAR **f,double *w,double *h,double *s,double *a,Byte *hc,RCCHAR **t,BitMask *o)
{ //*f = RCSTRCPY(*f,v.getstring("tx::font"));
#ifdef _USING_QTCHAR
  *f = (RCCHAR*)v.getstring("tx::font",*f);
#else
  *f = (RCCHAR*)v.getstring("tx::font");
#endif
  //static RCCHAR gf[300],gt[300];
  //RCSTRCPY(gf,v.getstring("tx::font"));
  //*f = gf;
  //printQChar( *f );
  *o = *v.getbm("tx::options");
  *w = v.getreal("tx::width");
  *h = v.getreal("tx::height");
  *s = v.getreal("tx::vertspacing");
  *a = v.getreal("tx::angle");
  //*t = RCSTRCPY(*t,v.getstring("tx::text"));
#ifdef _USING_QTCHAR
  *t = (RCCHAR*)v.getstring("tx::text",*t);
#else
  *t = (RCCHAR*)v.getstring("tx::text");
#endif
  //RCSTRCPY(gt,v.getstring("tx::text"));
  //*t = gt;
  //printQChar( *t );
  *hc = v.getinteger("tx::halocolour");
}

#ifdef _USING_QTCHAR
void Text::setdefaults(char *f,double w,double h,double s,double a,Byte hc,char *t,const BitMask &o)
{
    Text::setdefaults((RCCHAR*)f,w,h,s,a,hc,(RCCHAR*)t,o);
}
#endif

void Text::setdefaults(RCCHAR *f,double w,double h,double s,double a,Byte hc,RCCHAR *t,const BitMask &o)
{ v.setstring("tx::font",f);
  v.setbm("tx::options",o);
  v.setreal("tx::width",w);
  v.setreal("tx::height",h);
  v.setreal("tx::vertspacing",s);
  v.setreal("tx::angle",a);
  v.setstring("tx::text",t);
  v.setinteger("tx::halocolour",hc);
}

void Text::setdefaults(void)
{ v.setstring("tx::font",font);
  v.setbm("tx::options",options);
  v.setreal("tx::width",width);
  v.setreal("tx::height",height);
  v.setreal("tx::vertspacing",vertspacing);
  v.setreal("tx::angle",angle);
  v.setinteger("tx::halocolour",halocolour);
}

int Text::save(CadFile *outfile)
{short type;
  type = text_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  options.set(11,label != 0);
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&vertspacing)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(font)) return 0;
  if (! outfile->write(text)) return 0;
  if (options.test(11))
    if (! outfile->write(label)) return 0;
  if (options.test(12))
    {  type = halocolour;
       if (! outfile->write(&type)) return 0;
    }
  return 1;
}

Entity *Text::load(int,CadFile *infile)
{
 Text *texte;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 double width=0.,height=10.,vertspacing=0.,angle=0.;
 BitMask options(32);
 char *cfont=0,*ctext=0,*clabel=0;
 RCCHAR *font=0,*text=0,*label=0;
 short halocolour;
  if ((header = loadheader(infile)) == NULL) 
    return 0;
  if (! origin.load(infile)) 
    return 0;
  if (! xaxis.load(infile)) 
    return 0;
  if (! yaxis.load(infile)) 
    return 0;
  if (! infile->read(&options)) 
    return 0;
  if (! infile->read(&width)) 
    return 0;
  if (! infile->read(&height)) 
    return 0;
  if (! infile->read(&vertspacing)) 
    return 0;
  if (! infile->read(&angle))
    return 0;
  if( infile->getversion() == 2 )
  {
	  if (! infile->read(&cfont))
		return 0;
	  if (! infile->read(&ctext))
		return 0;
	  if (options.test(11))
	  {
		  if (! infile->read(&clabel))
			return 0;
	  }
	  else
		clabel = 0;

	  if( cfont && strlen(cfont) > 0)
	  {
		font = new RCCHAR[strlen(cfont) + 1];
		RCSTRCPY( font, cfont );
	  }
	  if( ctext && strlen(ctext) > 0)
	  {
		text = new RCCHAR[strlen(ctext) + 1];
		RCSTRCPY( text, ctext );
	  }
	  if( clabel && strlen(clabel) > 0)
	  {
		label = new RCCHAR[strlen(clabel) + 1];
		RCSTRCPY( label, clabel );
	  }
  }
  else
  {
	  if (! infile->read(&font))
		  return 0;
	  if (! infile->read(&text))
		return 0;
	  if (options.test(11))
		{  if (! infile->read(&label))
			return 0;
		}
	  else
		label = 0;
  }
  if (options.test(12))
    {  if (! infile->read(&halocolour)) 
        return 0;
    }
  else
    halocolour = 0;

  // adjust the width value
  if( infile->getversion() == 2 )
  {
      // KMJ make old stuff 0.0 for now
      width = 0.0;
      // make line spacing 0.0 for now
      //vertspacing = 0.0;
  }

  texte = new Text(*header,font,origin,xaxis,yaxis,width,height,vertspacing,angle,(Byte)halocolour,text,label,options);
  texte->atlist = header->getatlist();

  delete header; 
  // KMJ temp version number is -2
  if( infile->getversion() == 2 )
  {
	  delete [] text;
	  delete [] font;
	  delete [] label;
  }
  else
  {
      delete [] ctext;
      delete [] cfont;
      delete [] clabel;

      delete [] text;
	  delete [] font;
	  delete [] label;
  }
  return texte;
}

int Text::save(UndoFile *outfile)
{short type;
#ifdef USING_WIDGETS
    delete [] tmpfont; tmpfont=0;
    delete [] tmptext; tmptext=0;
    delete [] tmplabel; tmplabel=0;
#endif
  type = text_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  options.set(11,label != 0);
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(text)) return 0;
  if (! outfile->write(font)) return 0;
  if (options.test(11))
    if (! outfile->write(label)) return 0;
  return 1;
}

Entity *Text::load(UndoFile *infile)
{Text *text;
 char bm[sizeof(BitMask)];
  if ((text = new Text) != NULL)
    {AttributeList al;
       memcpy(bm,&text->visible,sizeof(bm));
       if (! infile->blockread(text,sizeof(*text))) return 0;
       memcpy(&text->visible,bm,sizeof(bm));
       if (! infile->read(&text->visible)) return 0;
       if (! al.load(infile)) return 0;  text->atlist = al;
       if (! infile->read(&text->options)) return 0;
       if (! infile->read(&text->text)) return 0;
       if (! infile->read(&text->font)) return 0;
       //if (! infile->read((BYTE*)&text->text)) return 0;
       //if (! infile->read((BYTE*)&text->font)) return 0;
       if (text->options.test(11))
       if (! infile->read(&text->label)) return 0;
       //if (! infile->read((BYTE*)&text->label)) return 0;
#ifdef USING_WIDGETS
       text->tmpfont = new char[strlen(text->font)+1];
       strcpy(text->tmptext,text->text);
       text->tmptext = new char[strlen(text->label)+1];
       strcpy(text->tmpfont,text->font);
       text->tmplabel = new char[strlen(text->label)+1];
       strcpy(text->tmplabel,text->label);
#endif
    }
  return text;
}

int Text::saveattributes(CadFile *outfile)
{
  if (! outfile->write(text)) return 0;
  if (! outfile->write(font)) return 0;
  //if (! outfile->write((BYTE*)text)) return 0;
  //if (! outfile->write((BYTE*)font)) return 0;
  options.set(11,label != 0);
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&vertspacing)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (options.test(11))
    if (! outfile->write(label)) return 0;
    //if (! outfile->write((BYTE*)label)) return 0;
  return 1;
}

int Text::loadattributes(CadFile *infile)
{ delete [] text;
  delete [] font;
  cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&text)) return 0;
  if (! infile->read(&font)) return 0;
  //if (! infile->read((BYTE*)&text)) return 0;
  //if (! infile->read((BYTE*)&font)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&width)) return 0;
  if (! infile->read(&vertspacing)) return 0;
  if (! infile->read(&angle)) return 0;
  if (options.test(11))
  {
      if (! infile->read(&label)) return 0;
      //if (! infile->read((BYTE*)&label)) return 0;
  }
  else
    label = 0;
  return 1;
}

int Text::savedxf(int blocksection,DXFOut *dxffile)
{Point p1,zaxis;
 double oheight,scale,rh,rw,a,cosa;
 int nlines,justification,ll,eoll;

 RCCHAR *s;

  if (! blocksection)
    {  if (options.test(9) && dxffile->getownerhandle() == -1)
         scale = dxffile->getwindow()->getplotscale();
       else
         scale = 1.0;
       cosa = cadwindow->getcurrentwindow()->getuaxis().dot(xaxis);
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       a = acos(cosa) * 180.0 / M_PI;
       if (cadwindow->getcurrentwindow()->getwaxis().dot(cadwindow->getcurrentwindow()->getuaxis().cross(xaxis)) < 0.0)
         a = 360.0 - a;
       rh = height * scale;
       if (options.test(13))  // Height is in points
         rh *= 25.4 / 72.0; 
       rw = width * scale;
       for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
       if (! options.test(3) && ! options.test(4))
         oheight = (nlines - 1) * rh * (1.0 + vertspacing);
       else if (options.test(3))
         oheight = (nlines * rh + (nlines - 1) * rh * vertspacing) / 2.0 - rh;
       else
         oheight = -rh;
       p1 = origin + yaxis * oheight;
       for (s = text ; *s!=0 ; s += ll + eoll)
         {  for (ll = 0 ;  s[ll] != '\r' && s[ll] != '\n' && s[ll] != 0 ; ll++) ;
            if (! dxffile->writestring(0, (char*)"TEXT")) return 0;
            if (! dxffile->writeheader(this)) return 0;
            if (! dxffile->writestring(100, (char*)"AcDbText")) return 0;
            if (! dxffile->writereal(10,p1.x * dxffile->getscale())) return 0;
            if (! dxffile->writereal(20,p1.y * dxffile->getscale())) return 0;
            if (! dxffile->writereal(30,p1.z * dxffile->getscale())) return 0;
            if (! dxffile->writereal(11,p1.x * dxffile->getscale())) return 0;
            if (! dxffile->writereal(21,p1.y * dxffile->getscale())) return 0;
            if (! dxffile->writereal(31,p1.z * dxffile->getscale())) return 0;
            if (! dxffile->writereal(40,rh * dxffile->getscale() * dxffile->gettextscale())) return 0;
            if (! dxffile->writestring(1,RC2C(s),ll)) return 0;
            if (rw != 0 && rh != 0)
              {  if (! dxffile->writereal(41,rw / rh)) return 0;
              }
            if (! dxffile->writereal(50,a+angle)) return 0;
            if (! dxffile->writereal(51,(options.test(7) ? 10.0 : 0.0))) return 0;
            if (! dxffile->writestring(7, (char*)"STANDARD")) return 0;
            justification = 0;
            if (options.test(1))
              justification = 1;
            else if (options.test(2))
              justification = 2;
            if (! dxffile->writelong(72,justification)) return 0;
            if (! dxffile->writestring(100, (char*)"AcDbText")) return 0;
            if (s[ll] == 0) 
              break;
            if (s[ll] == 0x0d && s[ll+1] == 0x0a)
              eoll = 2;
            else
              eoll = 1;   
            p1 -= yaxis * rh * (1.0 + vertspacing);
         }
    }
  return 1;
}

int Text::exportiges(FILE *outfile,int section,int,int *ndirectory,int *nparameter)
{char t[300];
 int base;

  if (section == 0)
    {  if (fprintf(outfile,"     212%8d               0       0       0       0       0 0 0 0 0D%7d\n",(*nparameter)+1,++(*ndirectory)) < 0) return 0;
       if (fprintf(outfile,"     212       0%8d       2       0                               0D%7d\n",colour,++(*ndirectory)) < 0) return 0;
       if (strlen(t) <= 60)
         (*nparameter) += 3;
       else
         (*nparameter) += 3 + (strlen(t) - 60 + 63) / 64;
    }
  else 
    {  if (fprintf(outfile,"212,1,%3d,%18.10e,%18.10e,1,0.0,%10.4f,%7dP%7d\n",RCSTRLEN(text),RCSTRLEN(text) * 0.6 * height,height,angle*acos(-1.0)/180.0,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       if (fprintf(outfile,"0,0,%18.10e,%18.10e,0.0,                   %7dP%7d\n",origin.x,origin.y,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       strncpy(t,(char*)text,60);  t[60] = 0;
       if (fprintf(outfile,"%-3.3dH%-61s%7dP%7d\n",RCSTRLEN(text),t,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       (*nparameter) += 3;
       base = 60;
       while (base < (signed)RCSTRLEN(text))
         {  strncpy(t,(char*)text + base,64);  t[64] = 0;
            if (base + 64 < (signed)RCSTRLEN(text))
              {  if (fprintf(outfile,"%-64s;%7dP%7d\n",t,*ndirectory + 1,++(*nparameter)) < 0) return 0;
              }
            else
              {  if (fprintf(outfile,"%-65s%7dP%7d\n",t,*ndirectory + 1,++(*nparameter)) < 0) return 0;
              }
            base += 64;
         }
       (*ndirectory) += 2;
    }

  return 1;

}

int Text::savegeometry(CadFile *outfile)
{ if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&angle)) return 0;

  return 1;
}

int Text::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&width)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&angle)) return 0;
  return 1;
}

