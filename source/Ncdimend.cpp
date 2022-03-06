
#include "ncwin.h"

DistanceBearingDimension::DistanceBearingDimension(Point org,Point xa,Point ya,Point start,Point end,BitMask &disp,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o),display(32)
{ p1 = start;  p2 = end;  display = disp;  cs.set(org,xa,ya);
}

DistanceBearingDimension::DistanceBearingDimension(const EntityHeader &header,Point org,Point xa,Point ya,Point start,Point end,BitMask &disp,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(header,e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o),display(32)
{ p1 = start;  p2 = end;  disp = display;
  cs.set(org,xa,ya);
}

DistanceBearingDimension::~DistanceBearingDimension()
{
}

void DistanceBearingDimension::generate(View3dSurface *v,DimensionCallBack dcb,void *data)
{Point p3,p4,p5,p6,sp1,sp2,xaxis,yaxis;
 double x2,y,cosa,a,value,plen,vlen,slen,tlen,totallen,hgt,xo,yo,plotscale;
 int i,hjust,d,m,s;
 BitMask toptions(32);
 RCCHAR string[300],tolerancestring[300];
 TextInfo *ti;
 //degree Åã = 0x00B0
 QChar degree(0x00B0);

  if (v == 0)
    return;

  textpointlist.start();
  for (i = 0 ; i < 2 ; i++)
    {   p3 = cs.frommodel(p1); p3.z = 0.0;
        p4 = cs.frommodel(p2);   p4.z = 0.0; 
        plotscale = (v == 0 || options.test(ScaleText) ? 1.0 : v->getplotscale());
        value = (p4 - p3).length();
        if (i == 0)
        {    formatvalue(string,tolerancestring,value,0);
             y = - valueinfo.height * 4.0 / 3.0 * plotscale;
        }
        else if (value > db.getptoler())
        {    cosa = (p4.x - p3.x) / value;
             if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
             a = acos(cosa);
             if (p4.y < p3.y) a = 2.0 * M_PI - a;
             a *= 180.0 / M_PI;
             a -= 90.0;
             a = -a;
             while (a < 0.0) a += 360.0;
             while (a >= 360.0) a -= 360.0;
             formatvalue(string,tolerancestring,a / scale,0);
             a += 0.5 / 3600.0;
             d = int(floor(a));
             a = (a - d) * 60.0;
             m = int(floor(a));
             a = (a - m) * 60.0;
             s = int(floor(a));
             QString fmt = QString("%1%2%3'%4\"").arg(d).arg(degree).arg(m).arg(s);
             strcpy(string,fmt);
             //sprintf(string,"%dÅã%2.2d'%2.2d\"",d,m,s);
             y = valueinfo.height / 3.0 * plotscale;
          }
        p3 = cs.tomodel(p3);  p4 = cs.tomodel(p4);
        xaxis = p4 - p3;  yaxis = cs.getxaxis().cross(cs.getyaxis()).cross(xaxis);
        xaxis.normalize();  yaxis.normalize();
        p3 = (p3 + p4) / 2.0;
        ti = (options.test(3) ? &valueinfo : &prefixinfo);  ti->options.set(9,! options.test(ScaleText));
        if (! Text::extents(v,ti->font,ti->options,prefixinfo.text,
                              ti->width,ti->height,ti->vertspacing,&plen,&hgt,&xo,&yo))
          plen = 0.0;
        valueinfo.options.set(9,! options.test(ScaleText));
        if (! options.test(2) || ! Text::extents(v,valueinfo.font,valueinfo.options,string,
                            valueinfo.width,valueinfo.height,valueinfo.vertspacing,&vlen,&hgt,&xo,&yo))
          vlen = 0.0;
        ti = (options.test(ToleranceDefined) ? &toleranceinfo : &valueinfo);  ti->options.set(9,! options.test(ScaleText));
        if (! options.test(2) || ! Text::extents(v,ti->font,ti->options,tolerancestring,
                            ti->width,ti->height,ti->vertspacing,&tlen,&hgt,&xo,&yo))
          tlen = 0.0;
        ti = (options.test(4) ? &valueinfo : &suffixinfo);  ti->options.set(9,! options.test(ScaleText));
        if (! Text::extents(v,ti->font,ti->options,suffixinfo.text,
                            ti->width,ti->height,ti->vertspacing,&slen,&hgt,&xo,&yo))
          slen = 0.0;
        if (options.test(3))
          {  ti = &valueinfo;
             hjust = 2;
          }
        else
          {  ti = &prefixinfo;
             hjust = ti->options.test(1) + ti->options.test(2) * 2;
          }
        toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
        totallen = plen + vlen + tlen + slen;
        sp1 = v->modeltoscreen(p3);  sp2 = v->modeltoscreen(p3 + xaxis);
        if (fabs(sp1.x - sp2.x) < 0.001)
          {  if (sp1.y > sp2.y)
               xaxis = -xaxis;
             sp1 = v->modeltoscreen(p3);  sp2 = v->modeltoscreen(p3 + yaxis);
             if (sp1.x < sp2.x)
               yaxis = -yaxis;
          }
        else
          {  if (sp1.x > sp2.x)
               xaxis = -xaxis;
             sp1 = v->modeltoscreen(p3);  sp2 = v->modeltoscreen(p3 + yaxis);
             if (sp1.y > sp2.y)
               yaxis = -yaxis;
          }
        switch (hjust)
          {  case 0 : x2 = -totallen / 2.0;  break;
             case 1 : x2 = -totallen / 2.0 + plen / 2.0;  break;
             case 2 : x2 = -totallen / 2.0 + plen;  break;
          }

        if (! textpointlist.atend())
          p3 += textpointlist.next();

        if (strlen(prefixinfo.text) > 0)
          {Text text1((EntityHeader &)(*this),ti->font,p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                         ti->width,ti->height,ti->vertspacing,ti->angle,0,prefixinfo.text,0,toptions);
             dcb(&text1,data);
          }
        if (options.test(2))
          {  hjust = valueinfo.options.test(1) + valueinfo.options.test(2) * 2;
             switch (hjust)
               {  case 0 : x2 = -totallen / 2.0 + plen;  break;
                  case 1 : x2 = -totallen / 2.0 + plen + vlen / 2.0;  break;
                  case 2 : x2 = -totallen / 2.0 + plen + vlen;  break;
               }
             toptions = valueinfo.options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
             if (strlen(string) > 0)
               {Text text2((EntityHeader &)(*this),valueinfo.font,p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                           valueinfo.width,valueinfo.height,valueinfo.vertspacing,valueinfo.angle,0,
                            string,0,toptions);
                  dcb(&text2,data);
               }
             if (options.test(ToleranceDefined))
               {  ti = &toleranceinfo;
                  hjust = ti->options.test(1) + ti->options.test(2) * 2;
               }
             else
               {  ti = &valueinfo;
                  hjust = 2;
               }
             toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
             switch (hjust)
               {  case 0 : x2 = -totallen / 2.0 + plen + vlen;  break;
                  case 1 : x2 = -totallen / 2.0 + plen + vlen + tlen / 2.0;  break;
                  case 2 : x2 = -totallen / 2.0 + plen + vlen + tlen;  break;
               }
             if (strlen(tolerancestring) > 0)
               {Text text3((const EntityHeader &)(*this),ti->font,p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                                ti->width,ti->height,ti->vertspacing,ti->angle,0,tolerancestring,0,toptions);
                  dcb(&text3,data);
               }
          }
        if (options.test(4))
          {  ti = &valueinfo;
             hjust = 2;
          }
        else
          {  ti = &suffixinfo;
             hjust = ti->options.test(1) + ti->options.test(2) * 2;
          }
        toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
        switch (hjust)
          {  case 0 : x2 = -totallen / 2.0 + plen + vlen + tlen;  break;
             case 1 : x2 = -totallen / 2.0 + plen + vlen + tlen + slen / 2.0;  break;
             case 2 : x2 = -totallen / 2.0 + plen + vlen + tlen + slen;  break;
          }
        if (strlen(suffixinfo.text) > 0)
          {Text text4((const EntityHeader &)(*this),ti->font,p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                           ti->width,ti->height,ti->vertspacing,ti->angle,0,suffixinfo.text,0,toptions);
             dcb(&text4,data);
          }
    }
}

int DistanceBearingDimension::npoints(void)
{ return 2;
}

Point DistanceBearingDimension::point(int n)
{ if (n == 0)
    return p1;
  else
    return p2;
}

Point DistanceBearingDimension::nearp(Point)
{ return cs.getorigin();
}

int DistanceBearingDimension::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int match1,match2;
 PointList newlist;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  match1 = sp->match(p1);  match2 = sp->match(p2);
  if (! match1 && ! match2) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0)
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus((status | 4) & ~8);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db == 0 ? 1 : db->geometry.add(c);
           }
      }
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  if (match1) p1 = t.transform(p1);
  if (match2) p2 = t.transform(p2);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int DistanceBearingDimension::move(Database *db,Transform &t,int erase)
{Point zero(0.0,0.0,0.0);
 Entity *e;
 Point p,org,xaxis,yaxis,zaxis;
 PointList l;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  status |= 1;
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() > db->getptoler() && yaxis.length() > db->getptoler())
    cs.change(org,xaxis,yaxis);
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  copye = this;
  if (getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *DistanceBearingDimension::clone(Transform &t)
{Point zero(0.0,0.0,0.0);
 Point p,p3,p4,org,xaxis,yaxis,zaxis;
 PointList l;
  if (copye != NULL) return copye;
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) return 0;
  p3 = t.transform(p3);
  p4 = t.transform(p4);
  copye = new DistanceBearingDimension(*this,org,xaxis,yaxis,p3,p4,display,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                              precision,tolprecision,term1,term2,valueinfo,toleranceinfo,prefixinfo,suffixinfo,options);
  if (! copye->getdefined())
    {  delete copye;
       copye = NULL;
        return copye;
    }
  copye->cloneattributes(this);
  return copye;
}

int DistanceBearingDimension::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

int DistanceBearingDimension::save(CadFile *outfile)
{short type;  
  type = distancebearingdimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! cs.save(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! display.save(outfile)) return 0;
  if (! outfile->write(&gap)) return 0;
  if (! outfile->write(&extension)) return 0;
  if (! outfile->write(&stackdis)) return 0;
  if (! outfile->write(&uppertol)) return 0;
  if (! outfile->write(&lowertol)) return 0;
  if (! outfile->write(&precision)) return 0;
  if (! outfile->write(&tolprecision)) return 0;
  if (! valueinfo.save(outfile)) return 0;
  if (! prefixinfo.save(outfile)) return 0;
  if (! suffixinfo.save(outfile)) return 0;
  if (! term1.save(outfile)) return 0;
  if (! term2.save(outfile)) return 0;
  options.set(ScaleDefined,scale != 1.0);
  options.set(DimensionTextYOffset,textyoffset != 0.0);
  options.set(MoveTextDefined,textpointlist.length() > 0);
  if (! outfile->write(&options)) return 0;
  if (options.test(ToleranceDefined))
    if (! toleranceinfo.save(outfile)) return 0;
  if (options.test(ScaleDefined))
    if (! outfile->write(&scale)) return 0;
  if (options.test(DimensionTextYOffset))
    if (! outfile->write(&textyoffset)) return 0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.save(outfile)) return 0;
  if (options.test(EquationDefined))
    if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *DistanceBearingDimension::load(int,CadFile *infile)
{DistanceBearingDimension *distancebearingdimension;
 EntityHeader *header;
 Point org,xaxis,yaxis,origin,startp,endp;
 double gap,extension,uppertol,lowertol,stackdis,scale,textyoffset;
 short precision,tolprecision;
 BitMask options(32),display(32);
 TerminatorInfo term1,term2;
 TextInfo ti1,ti2,ti3,ti4;
 PointList list;
 PointList textpointlist;
 RCCHAR *equation;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! org.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! startp.load(infile)) return 0;
  if (! endp.load(infile)) return 0;
  if (! infile->read(&display)) return 0;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&extension)) return 0;
  if (! infile->read(&stackdis)) return 0;
  if (! infile->read(&uppertol)) return 0;
  if (! infile->read(&lowertol)) return 0;
  if (! infile->read(&precision)) return 0;
  if (! infile->read(&tolprecision)) return 0;
  if (! ti1.load(infile)) return 0;
  if (! ti3.load(infile)) return 0;
  if (! ti4.load(infile)) return 0;
  if (! term1.load(infile)) return 0;
  if (! term2.load(infile)) return 0;
  if (! infile->read(&options)) return 0; 
  if (options.test(ToleranceDefined))
    {  if (! ti2.load(infile)) return 0;
    }
  else
    {  ti2 = ti1;
       ti2.font = new RCCHAR [RCSTRLEN(ti1.font)+1];
       ti2.text = new RCCHAR [RCSTRLEN(ti1.font)+1];
       RCSTRCPY(ti2.font,ti1.font);
       RCSTRCPY(ti2.text,ti1.text);
    }
  if (options.test(ScaleDefined))
    {  if (! infile->read(&scale)) return 0;
    }
  else
    scale = 1.0;
  if (options.test(DimensionTextYOffset))
    {  if (! infile->read(&textyoffset)) return 0;
    }
  else
    textyoffset = 0.0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.load(infile)) return 0;
  if (options.test(EquationDefined))
    {
      if(infile->getversion() == 2)
      {
          char *ctemp=0;
          if (! infile->read(&ctemp)) return 0;
          equation = new RCCHAR[strlen(ctemp)+1];
          strcpy(equation,ctemp);
          delete [] ctemp;
      }
      else
          if (! infile->read(&equation)) return 0;
    }
  else
    {  equation = new RCCHAR[1];
       equation[0] = 0;
    }

  distancebearingdimension = new DistanceBearingDimension((EntityHeader &)*header,org,xaxis,yaxis,startp,endp,display,gap,extension,stackdis,uppertol,lowertol,scale,textyoffset,
                                        precision,tolprecision,term1,term2,ti1,ti2,ti3,ti4,options);
  distancebearingdimension->atlist = header->getatlist();
  distancebearingdimension->textpointlist = textpointlist;
  distancebearingdimension->equation = equation;
  delete header;
  delete [] ti1.text;  delete [] ti1.font;
  delete [] ti2.text;  delete [] ti2.font;
  delete [] ti3.text;  delete [] ti3.font;
  return distancebearingdimension;
}

int DistanceBearingDimension::save(UndoFile *outfile)
{short type;
  type = distancebearingdimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
  if (! outfile->write(&display)) return 0;
  if (! outfile->write(valueinfo.text)) return 0;
  if (! outfile->write(valueinfo.font)) return 0;
  if (! outfile->write(toleranceinfo.text)) return 0;
  if (! outfile->write(toleranceinfo.font)) return 0;
  if (! outfile->write(prefixinfo.text)) return 0;
  if (! outfile->write(prefixinfo.font)) return 0;
  if (! outfile->write(suffixinfo.text)) return 0;
  if (! outfile->write(suffixinfo.font)) return 0;
  if (! outfile->write(&valueinfo.options)) return 0;
  if (! outfile->write(&prefixinfo.options)) return 0;
  if (! outfile->write(&suffixinfo.options)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *DistanceBearingDimension::load(UndoFile *infile)
{DistanceBearingDimension *ad;
 PointList empty;
 char bm[sizeof(BitMask)];
  if ((ad = new DistanceBearingDimension) != NULL)
    {AttributeList al;

       memcpy(bm,&ad->visible,sizeof(bm));
       if (! infile->blockread(ad,sizeof(*ad))) return 0;
       memcpy(&ad->visible,bm,sizeof(bm));
       if (! infile->read(&ad->visible)) return 0;
       if (! al.load(infile)) return 0;  ad->atlist = al;
       ad->textpointlist = PointList();
       if (! ad->textpointlist.load(infile)) return 0;
       if (! infile->read(&ad->display)) return 0;
       if (! infile->read(&ad->valueinfo.text)) return 0;
       if (! infile->read(&ad->valueinfo.font)) return 0;
       if (! infile->read(&ad->toleranceinfo.text)) return 0;
       if (! infile->read(&ad->toleranceinfo.font)) return 0;
       if (! infile->read(&ad->prefixinfo.text)) return 0;
       if (! infile->read(&ad->prefixinfo.font)) return 0;
       if (! infile->read(&ad->suffixinfo.text)) return 0;
       if (! infile->read(&ad->suffixinfo.font)) return 0;
       if (! infile->read(&ad->valueinfo.options)) return 0;
       if (! infile->read(&ad->prefixinfo.options)) return 0;
       if (! infile->read(&ad->suffixinfo.options)) return 0;
       if (! infile->read(&ad->options)) return 0;
       if (! infile->read(&ad->equation)) return 0;
    }
  return ad;
}

int DistanceBearingDimension::saveattributes(CadFile *outfile)
{ if (! outfile->write(&display)) return 0;
  if (! outfile->write(&gap)) return 0;
  if (! outfile->write(&extension)) return 0;
  if (! outfile->write(&stackdis)) return 0;
  if (! outfile->write(&uppertol)) return 0;
  if (! outfile->write(&lowertol)) return 0;
  if (! outfile->write(&scale)) return 0;
  if (! outfile->write(&precision)) return 0;
  if (! outfile->write(&tolprecision)) return 0;
  if (! valueinfo.save(outfile)) return 0;
  if (! prefixinfo.save(outfile)) return 0;
  if (! suffixinfo.save(outfile)) return 0;
  if (! term1.save(outfile)) return 0;
  if (! term2.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(equation)) return 0;
  return 1;
}

int DistanceBearingDimension::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  delete [] valueinfo.text;   delete [] valueinfo.font;
  delete [] prefixinfo.text;  delete [] prefixinfo.font;
  delete [] suffixinfo.text;  delete [] suffixinfo.font;
  if (! infile->read(&display)) return 0;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&extension)) return 0;
  if (! infile->read(&stackdis)) return 0;
  if (! infile->read(&uppertol)) return 0;
  if (! infile->read(&lowertol)) return 0;
  if (! infile->read(&scale)) return 0;
  if (! infile->read(&precision)) return 0;
  if (! infile->read(&tolprecision)) return 0;
  if (! valueinfo.load(infile)) return 0;
  if (! prefixinfo.load(infile)) return 0;
  if (! suffixinfo.load(infile)) return 0;
  if (! term1.load(infile)) return 0;
  if (! term2.load(infile)) return 0;
  if (! infile->read(&options)) return 0;
  if(infile->getversion() == 2)
  {
      char *ctemp=0;
      if (! infile->read(&ctemp)) return 0;
      equation = new RCCHAR[strlen(ctemp)+1];
      strcpy(equation,ctemp);
      delete [] ctemp;
  }
  else
    if (! infile->read(&equation)) return 0;
  return 1;
}

static int dxfcb(Entity *entity,void *data)
{ entity->savedxf(0,(DXFOut *)data);
  return 1;
}

int DistanceBearingDimension::savedxf(int blocksection,DXFOut *dxfout)
{ if (! blocksection)
    generate(dxfout->getwindow(),dxfcb,dxfout);
  return 1;
}

struct IgesParameters
{FILE *outfile;
 int section,options,*ndirectory,*nparameter,result;
};

static int igescb(Entity *entity,void *data)
{IgesParameters *ip = (IgesParameters *) data;
  if (entity->exportiges(ip->outfile,ip->section,ip->options,ip->ndirectory,ip->nparameter) == 0)
    ip->result = 0;
  return ip->result;
}

int DistanceBearingDimension::exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter)
{IgesParameters ip;
  ip.outfile = outfile;
  ip.section = section;
  ip.options = options;
  ip.ndirectory = ndirectory;
  ip.nparameter = nparameter;
  ip.result = 1;
  generate(cadwindow->getcurrentwindow(),igescb,&ip);
  return ip.result;
}

int DistanceBearingDimension::savegeometry(CadFile *outfile)
{ if (! cs.save(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
  return 1;
}

int DistanceBearingDimension::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  textpointlist.destroy();
  if (! cs.load(infile)) return 0;
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  if (! textpointlist.load(infile)) return 0;
  return 1;
}
