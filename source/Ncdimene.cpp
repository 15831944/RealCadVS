
#include "ncwin.h"

CircularDimension::CircularDimension(Point org,Point xa,Point ya,Point pnt1,Point pnt2,BitMask &sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o),style(32)
{ p1 = pnt1;  p2 = pnt2;  style = sty;  circle = c;  cs.set(org,xa,ya);
}

CircularDimension::CircularDimension(const EntityHeader &header,Point org,Point xa,Point ya,Point pnt1,Point pnt2,BitMask &sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(header,e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o),style(32)
{ p1 = pnt1;  p2 = pnt2;  style = sty;  circle = c;
  cs.set(org,xa,ya);
}

CircularDimension::~CircularDimension()
{ if (circle != 0 && circle->getposition() == NULL) delete circle;
}

void CircularDimension::generate(View3dSurface *v,DimensionCallBack dcb,void *data)
{Point p3,p4,p5,p6,sp1,sp2,xaxis,yaxis;
 double x1,x2,y,value,denom,plen,vlen,slen,tlen,totallen,hgt,xo,yo,plotscale,xscale,yscale;
 int s,hjust;
 BitMask toptions(32);
 RCCHAR string[300],tolerancestring[300];
 TextInfo *ti;
 LineTerminator *term;


  textpointlist.start();
  s = style.test(2) * 4 + style.test(1) * 2 + style.test(0);
  plotscale = v == 0 || options.test(ScaleText) ? 1.0 : v->getplotscale();

  xaxis = cs.getxaxis();
  yaxis = cs.getyaxis();
  xscale = yscale = 1.0;

  //  If the dimension is pointing into the view, reverse the X axis.
  if (v != 0 && (xaxis.cross(yaxis)).dot(v->getwaxis()) < 0.0)
    {  xaxis = -xaxis;
       xscale = -xscale;
    }

  //  If the xaxis is not pointing to the right, point it to the right.
  if (v != 0 && (xaxis.dot(v->getuaxis()) < -0.001 ||
                 xaxis.dot(v->getuaxis()) < 0.001 && yaxis.dot(v->getuaxis()) > -0.001))
    {  xaxis = -xaxis;
       yaxis = -yaxis;
       xscale = -xscale;
       yscale = -yscale;
    }

  CoordinateSystem newcs(cs.getorigin(),xaxis,yaxis);

  if (s == 0 || s == 1 || s == 4)
    {  if (s == 0 || s == 1)
         {  p3 = newcs.frommodel(circle->getorigin());  p3.z = 0.0;
            p4 = newcs.frommodel(circle->nearp(p1));  p4.z = 0.0;
         }
       else
         {  p3 = newcs.frommodel(newcs.getorigin());  p3.z = 0.0;
            p4 = newcs.frommodel(p1);  p4.z = 0.0;
         }
       p5 = newcs.frommodel(p2);  p5.z = 0.0;
       denom = (p4.y - p3.y);
       if (options.test(DimensionFreeLeader))
         p6 = p4;      
       else
         {  if (fabs(denom) < db.getptoler())
              p6.x = (p4.x + p5.x) / 2.0;
            else
              p6.x = p3.x + (p5.y - p3.y) * (p4.x - p3.x) / denom;
            p6.y = p5.y;  p6.z = 0.0;
         }
       p3 = newcs.tomodel(p3);  p4 = newcs.tomodel(p4);  p5 = newcs.tomodel(p5);  p6 = newcs.tomodel(p6);
       Line line1((EntityHeader &)(*this),s == 4 ? p3 : p4,p6);
       dcb(&line1,data);
       Line line2((EntityHeader &)(*this),p6,p5);
       dcb(&line2,data);
       if (s == 0 || s == 4)
         {  xaxis = p3 - p4;  yaxis = newcs.getxaxis().cross(newcs.getyaxis()).cross(xaxis);
            xaxis.normalize();  yaxis.normalize();
            CoordinateSystem termcs(s == 0 ? p4 : p3,(p4 - p3).length() > (p5 - p3).length() && circle != 0? -xaxis : xaxis,yaxis);
            if ((term = LineTerminator::create(plotscale,term1)) != NULL)
              {  term->generate(this,dcb,data,&termcs,1,0.0,0.0);
                 delete term;
              }
         }
       if (s == 0 || s == 1)
         {  value = circle->getradius();
            if (s == 1)
              value *= 2;
            formatvalue(string,tolerancestring,value,0);
         }
       else
         {  strcpy(string,"");
            strcpy(tolerancestring,"");
         }
       y = - valueinfo.height / 3.0 * plotscale;

       if (circle == 0)
         options.set(3,0);  //  Never use value text options for leaders
 
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
       if (v != 0)
         {  sp1 = v->modeltoscreen(p6);  sp2 = v->modeltoscreen(p5);
         }
       else
         {  sp1 = p6;  sp2 = p5;
         }
       if (sp1.x < sp2.x)
         x1 = valueinfo.height / 3.0;
       else
         x1 = -totallen - valueinfo.height / 3.0;
       switch (hjust)
         {  case 0 : x2 = 0.0;  break;
            case 1 : x2 = plen / 2.0;  break;
            case 2 : x2 = plen;  break;
         }

       if (! textpointlist.atend())
         p5 += textpointlist.next();

       if (strlen(prefixinfo.text) > 0)
         {Text text1((EntityHeader &)(*this),ti->font,p5+newcs.getyaxis()*y+newcs.getxaxis()*(x1+x2),newcs.getxaxis(),newcs.getyaxis(),
                          ti->width,ti->height,ti->vertspacing,ti->angle,ti->halocolour,prefixinfo.text,0,toptions);
            dcb(&text1,data);
         }
       if (options.test(2))
         {  hjust = valueinfo.options.test(1) + valueinfo.options.test(2) * 2;
            switch (hjust)
              {  case 0 : x2 = plen;  break;
                 case 1 : x2 = plen + vlen / 2.0;  break;
                 case 2 : x2 = plen + vlen;  break;
              }
            toptions = valueinfo.options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
            if (strlen(string) > 0)
              {Text text2((EntityHeader &)(*this),CHAR2RCCHR(valueinfo.font),p5+newcs.getyaxis()*y+newcs.getxaxis()*(x1+x2),newcs.getxaxis(),newcs.getyaxis(),
                        valueinfo.width,valueinfo.height,valueinfo.vertspacing,valueinfo.angle,valueinfo.halocolour,
                        CHAR2RCCHR(string),0,toptions);
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
              {  case 0 : x2 = plen + vlen;  break;
                 case 1 : x2 = plen + vlen + tlen / 2.0;  break;
                 case 2 : x2 = plen + vlen + tlen;  break;
              }
            if (strlen(tolerancestring) > 0)
              {Text text3((const EntityHeader &)(*this),CHAR2RCCHR(ti->font),p5+newcs.getyaxis()*y+newcs.getxaxis()*(x1+x2),newcs.getxaxis(),newcs.getyaxis(),
                               ti->width,ti->height,ti->vertspacing,ti->angle,ti->halocolour,CHAR2RCCHR(tolerancestring),0,toptions);
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
         {  case 0 : x2 = plen + vlen + tlen;  break;
            case 1 : x2 = plen + vlen + tlen + slen / 2.0;  break;
            case 2 : x2 = plen + vlen + tlen + slen;  break;
         }
       if (strlen(suffixinfo.text) > 0)
         {Text text4((const EntityHeader &)(*this),ti->font,p5+newcs.getyaxis()*y+newcs.getxaxis()*(x1+x2),newcs.getxaxis(),newcs.getyaxis(),
                          ti->width,ti->height,ti->vertspacing,ti->angle,0,suffixinfo.text,0,toptions);
            dcb(&text4,data);
         }
   }

 if (s == 1 || s == 2 || s == 3)
   {   p3 = newcs.frommodel(circle->getorigin()); p3.z = 0.0;
       p4 = newcs.frommodel(circle->nearp(p1));   p4.z = 0.0;
       if (s == 3)
         {  p5 = newcs.frommodel(circle->getorigin());
            p3 = (p3 + p4) / 2.0;
         }
       else
         p5 = newcs.frommodel(circle->getorigin() * 2.0 - circle->nearp(p1));
       p5.z = 0.0;
       p3 = newcs.tomodel(p3);  p4 = newcs.tomodel(p4);  p5 = newcs.tomodel(p5);
       Line line1((EntityHeader &)(*this),p4,p5);
       dcb(&line1,data);
       xaxis = p4 - p5;  yaxis = newcs.getxaxis().cross(newcs.getyaxis()).cross(xaxis);
       xaxis.normalize();  yaxis.normalize();
       CoordinateSystem termcs1(p4,xaxis,yaxis);
       if ((term = LineTerminator::create(plotscale,term1)) != NULL)
         {  term->generate(this,dcb,data,&termcs1,1,0.0,0.0);
            delete term;
         }
       if (s == 1 || s == 2)
         {CoordinateSystem termcs2(p5,xaxis,yaxis);
            if ((term = LineTerminator::create(plotscale,term1)) != NULL)
              {  term->generate(this,dcb,data,&termcs2,-1,0.0,0.0);
                 delete term;
              }
         }
       if (s == 2 || s == 3)
         {  if (s == 2)
              value = circle->getradius() * 2.0;
            else
              value = circle->getradius();
            formatvalue(string,tolerancestring,value,0);
            y = valueinfo.height / 3.0;
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
            if (v != 0)
              {  sp1 = v->modeltoscreen(p3);  
                 sp2 = v->modeltoscreen(p3 + xaxis);
                 if (fabs(sp1.x - sp2.x) < 0.001)
                   {  if (sp1.y > sp2.y)
                        xaxis = -xaxis;
                      sp1 = v->modeltoscreen(p3);  
                      sp2 = v->modeltoscreen(p3 + yaxis);
                      if (sp1.x < sp2.x)
                        yaxis = -yaxis;
                   }
                 else
                   {  if (sp1.x > sp2.x)
                        xaxis = -xaxis;
                      sp1 = v->modeltoscreen(p3);  
                      sp2 = v->modeltoscreen(p3 + yaxis);
                      if (sp1.y > sp2.y)
                        yaxis = -yaxis;
                   }
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
                   {Text text2((EntityHeader &)(*this),CHAR2RCCHR(valueinfo.font),p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                               valueinfo.width,valueinfo.height,valueinfo.vertspacing,valueinfo.angle,0,
                               CHAR2RCCHR(string),0,toptions);
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
                   {Text text3((const EntityHeader &)(*this),CHAR2RCCHR(ti->font),p3+yaxis*y+xaxis*x2,xaxis,yaxis,
                                    ti->width,ti->height,ti->vertspacing,ti->angle,0,CHAR2RCCHR(tolerancestring),0,ti->options);
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
                               ti->width,ti->height,ti->vertspacing,ti->angle,0,suffixinfo.text,0,ti->options);
                 dcb(&text4,data);
              }
         }
   }

}

struct LDDrawCB_data
{
#ifdef USING_WIDGETS
    QGraphicsItemGroup *qgi;
#endif
   int drawmode;
   Transform *t;
   View3dSurface *v;
};

static int lddrawcb(Entity *entity,void *data)
{
  LDDrawCB_data *lddata = (LDDrawCB_data *) data;

  entity->draw(lddata->drawmode,lddata->t,lddata->v);
  if(entity->qgi)
      lddata->qgi->addToGroup(entity->qgi);
  entity->qgi=0;
  return 1;
}


#ifdef USING_WIDGETS
static int lddrawcbGL(Entity *entity,void *data)
{
  LDDrawCB_data *lddata = (LDDrawCB_data *) data;
#if 0
  if( entity->type() == text_entity)
  {
      entity->draw(lddata->drawmode,lddata->t,lddata->v);
      if(entity->qgi)
          lddata->qgi->addToGroup(entity->qgi);
      entity->qgi=0;
  }
  else
      entity->drawGL(lddata->drawmode,lddata->t,lddata->v);
#else
  entity->drawGL(lddata->drawmode,lddata->t,lddata->v);
#endif
  return 1;
}

void CircularDimension::drawGL(int drawmode,Transform *t,View3dSurface *v)
{
  LDDrawCB_data lddata;


  draw(DM_ERASE,t,v);

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  v->setupGL(linestylescale,0.0,drawmode,colour,Entity::style,weight,0);

  RCCOLORREF pcol = v->getcolour();
  //GLfloat fr=GetRValue(pcol)/255.0;
  //GLfloat fg=GetGValue(pcol)/255.0;
  //GLfloat fb=GetBValue(pcol)/255.0;
  //glColor3f(fr,fg,fb);
  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = v->curPen.widthF();
  glLineWidth(lwidth<1.0?1.0:lwidth);

#if 0
  if (0 && t == NULL && db.getenableautoplotscale() == 0 && ! gethandles() && prefixinfo.options.test(12) == 0 && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
    {  if (v->displaylist.draw(this,drawmode)) return;
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode)) return;
    }
#endif

  lddata.drawmode = drawmode;
  lddata.t = t;
  lddata.v = v;

  // KMJ: using qgi for text entities
  //lddata.qgi = new QGraphicsItemGroup();

  generate(v,lddrawcbGL,&lddata);

#if 0
  if(lddata.qgi)
  {
      qgi = lddata.qgi;
      v->getScene()->addItem(qgi);
  }
#endif

  if (gethandles())
  {
       Point pt1,pt2,pt3;

       v->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_HANDLE_SELECT,colour,0,0,0);
       //v->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

       RCCOLORREF pcol = v->getcolour();
       //GLfloat fr=GetRValue(pcol)/255.0;
       //GLfloat fg=GetGValue(pcol)/255.0;
       //GLfloat fb=GetBValue(pcol)/255.0;
       //glColor3f(fr,fg,fb);
       glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
       GLfloat lwidth = v->curPen.widthF();
       glLineWidth(lwidth<1.0?1.0:lwidth);

       //pt1 = v->modeltoscreen(p1);
       //pt2 = v->modeltoscreen(p2);
       //pt3 = v->modeltoscreen(cs.getorigin());

       Point pright = v->getuaxis();
       Point pup = v->getvaxis();
       double sc = v->getpixelscale() * 6.0;

       v->cbox2dGL(p1,pright,pup,sc,sc);
       v->cbox2dGL(p2,pright,pup,sc,sc);
       v->cbox2dGL(cs.getorigin(),pright,pup,sc,sc);
  }
}
#endif

void CircularDimension::draw(int drawmode,Transform *t,View3dSurface *v)
{
  LDDrawCB_data lddata;

#ifdef USING_WIDGETS

  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() == v->getScene())
          v->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      if(qgihandle && qgihandle->scene() == v->getScene())
          v->getScene()->removeItem(qgihandle);
      delete qgihandle;
      qgihandle=0;

      if(dynamic_cast<View3dSubSurface*>(v) != 0)
          ((View3dSubSurface*)v)->qgicp=0;
  }

  if(drawmode == DM_ERASE)
      return;

   qgi = new QGraphicsItemGroup();
#endif

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  v->setup(linestylescale,0.0,drawmode,colour,Entity::style,weight,0);

#if 0
  if (0 && t == NULL && db.getenableautoplotscale() == 0 && ! gethandles() && prefixinfo.options.test(12) == 0 && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
    {  if (v->displaylist.draw(this,drawmode)) return;
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode)) return;
    }
#endif

  lddata.drawmode = drawmode;
  lddata.t = t;
  lddata.v = v;

#ifdef USING_WIDGETS
  lddata.qgi = new QGraphicsItemGroup();
  generate(v,lddrawcb,&lddata);
  if(lddata.qgi)
  {
      qgi = lddata.qgi;
      v->getScene()->addItem(qgi);

      if(qgi && dynamic_cast<View3dSubSurface*>(v) != 0)
          if(((View3dSubSurface*)v)->qgicp != 0)
              qgi->setParentItem(((View3dSubSurface*)v)->qgicp);
  }
#else
  generate(v,lddrawcb,&lddata);
#endif

  if (gethandles())
  {
       Point pt1,pt2,pt3;

       QGraphicsItem *qgline=0;
       QGraphicsItemGroup *qgigroup = new QGraphicsItemGroup();

       if(qgigroup)
       {
           v->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

           pt1 = v->modeltoscreen(p1);
           pt2 = v->modeltoscreen(p2);
           pt3 = v->modeltoscreen(cs.getorigin());

           qgline = v->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);

           qgline = v->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);

           qgline = v->line(int(pt3.x)-2,int(pt3.y)-2,int(pt3.x)+2,int(pt3.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt3.x)+2,int(pt3.y)-2,int(pt3.x)+2,int(pt3.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt3.x)+2,int(pt3.y)+2,int(pt3.x)-2,int(pt3.y)+2);
           if(qgline)
               qgigroup->addToGroup(qgline);
           qgline = v->line(int(pt3.x)-2,int(pt3.y)+2,int(pt3.x)-2,int(pt3.y)-2);
           if(qgline)
               qgigroup->addToGroup(qgline);

           qgihandle = qgigroup;
           v->getScene()->addItem(qgihandle);
       }
  }

}

int CircularDimension::npoints(void)
{ return 2;
}

Point CircularDimension::point(int n)
{ if (n == 0)
    return p1;
  else
    return p2;
}

Point CircularDimension::nearp(Point)
{ return cs.getorigin();
}

void CircularDimension::dbinsert(Database *db,int draw)
{ if (db != NULL && circle != 0 && circle->getposition() == NULL) db->geometry.add(circle,draw);
  if (circle != 0)
    circle->adda(this);

  Entity::dbinsert(db,draw);


}

void CircularDimension::dbdelete(void)
{ if (circle != 0)
    circle->dela(this);
  Entity::dbdelete();
}

void CircularDimension::dbsetassociated(void)
{ if (circle != 0)
    circle->setstatus(1);
}

void CircularDimension::dbtraverseassociated(EntityCallBack c,void *d)
{ if (circle != 0)
    c(circle,d);
}


int CircularDimension::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int stretched,match1,match2,match3;
 PointList newlist;
 Point p;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) Entity::draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  if (circle != 0)
    {  circle->stretchent(db,t,sp,erase);
       stretched = (circle->getstatus() & ~8) != 0;
    }
  else
    stretched = 0;
  match1 = sp->match(cs.getorigin());
  match2 = sp->match(p1);
  match3 = sp->match(p2);
  if (! stretched && ! match1 && ! match2 && ! match3) return 1;
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
  if (erase && (status & 16) == 0) Entity::draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  dbdelete();
  if (circle != 0 && circle->getcopye() != NULL) circle = (Circle *) circle->getcopye();
  if (match1)
    cs.change(t.transform(cs.getorigin()),cs.getxaxis(),cs.getyaxis());
  if (match2)
    p1 = t.transform(p1);
  if (match3)
    p2 = t.transform(p2);
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int CircularDimension::move(Database *db,Transform &t,int erase)
{Point zero(0.0,0.0,0.0);
 Entity *e;
 Point p,org,xaxis,yaxis,zaxis;
 PointList l;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  if (circle != 0 && circle->getcopye() == NULL) circle->move(db,t,erase);
  if (circle != 0 && circle->getcopye() == NULL)
    {  Entity::draw(DM_NORMAL);  return 0;
    }
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  status |= 1;
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  if (circle != 0)
    circle = (Circle *) circle->getcopye();
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() > ::db.getptoler() && yaxis.length() > ::db.getptoler())
    cs.change(org,xaxis,yaxis);
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;
}

Entity *CircularDimension::clone(Transform &t)
{Point zero(0.0,0.0,0.0);
 Point p,pt,p3,p4,p5,org,xaxis,yaxis,zaxis;
 PointList l,pl;
 int i;

  if (copye != NULL) return copye;

  p5 = cs.frommodel(p2);  p5.z = 0.0;
  p5 = cs.tomodel(p5); 

  for (textpointlist.start(),i = 0 ; ! textpointlist.atend() ; i++)
    {  p = textpointlist.next();
       p = t.transform(p5) - t.transform(p5-p);
       pl.add(p);
    }
  pl.reverse();

  if (circle != 0)
    {  if (circle->getcopye() == NULL) circle->clone(t);
       if (circle->getcopye() == NULL) return NULL;
    }
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) return 0;
  p3 = t.transform(p1);  p4 = t.transform(p2);
  copye = new CircularDimension(*this,org,xaxis,yaxis,p3,p4,style,circle == 0 ? 0 : (Circle *) circle->getcopye(),extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                              precision,tolprecision,term1,term2,valueinfo,toleranceinfo,prefixinfo,suffixinfo,options);
  if (! copye->getdefined())
    {  delete copye;
       copye = NULL;
        return copye;
    }
  copye->cloneattributes(this);

  for (pl.start(),i = 0 ; ! pl.atend() ; i++)
    ((Dimension *)copye)->movetext(i,pl.next());
  pl.destroy();

  return copye;
}

int CircularDimension::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

int CircularDimension::save(CadFile *outfile)
{short type;
  type = circulardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! outfile->write((const Entity **)&circle)) return 0;
  if (circle != 0 && ! circle->saveunknowngeometry(outfile)) return 0;
  if (! cs.save(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
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
  options.set(MoveTextDefined,textpointlist.length() > 0);
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&style)) return 0;
  if (options.test(ToleranceDefined))
    if (! toleranceinfo.save(outfile)) return 0;
  if (options.test(ScaleDefined))
    if (! outfile->write(&scale)) return 0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.save(outfile)) return 0;
  if (options.test(EquationDefined))
    if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *CircularDimension::load(int reference,CadFile *infile)
{CircularDimension *circulardimension;
 EntityHeader *header=0;
 Entity *e=0,*ejunk=0;
 Point org,xaxis,yaxis,origin,startp,endp,p1,p2;
 double gap,extension,uppertol,lowertol,stackdis,scale,textyoffset;
 short precision,tolprecision;
 BitMask options(32),style(32);
 TerminatorInfo term1,term2;
 TextInfo ti1,ti2,ti3,ti4;
 PointList list;
 PointList textpointlist;
 RCCHAR *equation=0;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! infile->read((Entity **)&e)) return 0;
  if (reference)
    {  if (e != 0 && ! Entity::loadunknowngeometry(1,infile,&ejunk)) return 0;
    }
  else
    {  if (e != 0 && ! Entity::loadunknowngeometry(0,infile,&e)) return 0;
    }
  if (! org.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
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
  if (! infile->read(&style)) return 0;
  if (options.test(ToleranceDefined))
    {  if (! ti2.load(infile)) return 0;
    }
  else
    {  ti2 = ti1;
       ti2.font = new RCCHAR [RCSTRLEN(ti1.font)+1];
       ti2.text = new RCCHAR[RCSTRLEN(ti1.text)+1];
       strcpy(ti2.font,ti1.font);
       strcpy(ti2.text,ti1.text);
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
    textyoffset = 1.0;
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

  circulardimension = new CircularDimension((EntityHeader &)*header,org,xaxis,yaxis,p1,p2,style,(Circle *)e,gap,extension,stackdis,uppertol,lowertol,scale,textyoffset,
                                        precision,tolprecision,term1,term2,ti1,ti2,ti3,ti4,options);
  circulardimension->atlist = header->getatlist();
  circulardimension->textpointlist = textpointlist;
  circulardimension->equation = equation;
  delete header;
  delete [] ti1.text;  delete [] ti1.font;
  delete [] ti2.text;  delete [] ti2.font;
  delete [] ti3.text;  delete [] ti3.font;
  return circulardimension;
}

int CircularDimension::save(UndoFile *outfile)
{short type;
 long v = circle != 0 ? circle->getid() : 0;
  type = circulardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
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
  if (! outfile->write(&style)) return 0;
  if (! outfile->write(&v)) return 0;
  if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *CircularDimension::load(UndoFile *infile)
{CircularDimension *ad;
 char bm[sizeof(BitMask)];
  if ((ad = new CircularDimension) != NULL)
    {AttributeList al;
       memcpy(bm,&ad->visible,sizeof(bm));
       if (! infile->blockread(ad,sizeof(*ad))) return 0;
       memcpy(&ad->visible,bm,sizeof(bm));
       if (! infile->read(&ad->visible)) return 0;
       if (! al.load(infile)) return 0;  ad->atlist = al;
       ad->textpointlist = PointList();
       if (! ad->textpointlist.load(infile)) return 0;
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
       if (! infile->read(&ad->style)) return 0;
       if (! infile->read((long *)&ad->circle)) return 0;
       if (! infile->read(&ad->equation)) return 0;
    }
  return ad;
}

int CircularDimension::saveattributes(CadFile *outfile)
{ if (! outfile->write(&gap)) return 0;
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
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&style)) return 0;
  if (! outfile->write(equation)) return 0;
  return 1;
}

int CircularDimension::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  delete [] valueinfo.text;   delete [] valueinfo.font;
  delete [] prefixinfo.text;  delete [] prefixinfo.font;
  delete [] suffixinfo.text;  delete [] suffixinfo.font;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&extension)) return 0;
  if (! infile->read(&stackdis)) return 0;
  if (! infile->read(&uppertol)) return 0;
  if (! infile->read(&lowertol)) return 0;
  if (! infile->read(&precision)) return 0;
  if (! infile->read(&tolprecision)) return 0;
  if (! valueinfo.load(infile)) return 0;
  if (! prefixinfo.load(infile)) return 0;
  if (! suffixinfo.load(infile)) return 0;
  if (! term1.load(infile)) return 0;
  if (! term2.load(infile)) return 0;
  if (! infile->read(&options)) return 0; 
  if (! infile->read(&style)) return 0;
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

int CircularDimension::savedxf(int blocksection,DXFOut *dxfout)
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

int CircularDimension::exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter)
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


int CircularDimension::savegeometry(CadFile *outfile)
{long v = circle == 0 ? 0 : circle->getid();
  if (! cs.save(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  outfile->write(&v);
  if (! textpointlist.save(outfile)) return 0;
  return 1;
}

int CircularDimension::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  textpointlist.destroy();
  if (! cs.load(infile)) return 0;
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  infile->read((long *)&circle);
  if (! textpointlist.load(infile)) return 0;
  return 1;
}

void CircularDimension::fixupreferences(Entity * HUGE *index)
{ if (circle != 0)
    circle = (Circle *) index[long(circle)];
  Entity::fixupreferences(index);
}

int CircularDimension::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1,d2;
 int end;
  end = 0;
  if ((d1 = vs->pick(p1,distance,x,y)) >= 0.0)
    {  end = 1;  *pos = p1;
    }
  if ((d2 = vs->pick(p2,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = p2;  d1 = d2;
    }
  if ((d2 = vs->pick(cs.getorigin(),distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 3;  *pos = cs.getorigin();
    }
  return end;
}

void CircularDimension::movehandlepoint(View3dSurface *,int drawentity,int end,Point p)
{ 
    if (GetLockedLayerTable()->test(layer)) 
        return;  
    db.saveundo(UD_MOVED,this);
    if (drawentity) 
        Entity::draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(this);
    if (end == 1) 
        p1 = p;
    else if (end == 2)
        p2 = p;
    else if (end == 3)
        cs.setorigin(p);
    if (drawentity) Entity::draw(DM_NORMAL);
    if (getposition() != 0 && db.MovedCallback != 0)
        db.MovedCallback(this);
}


int CircularDimension::pickoriginpoint(View3dSurface *vs,int snap,int x,int y,Point *pos)
{double d1;
 int end;

    end = 0;
    if (circle == 0)
    {   if ((d1 = vs->pick(p1,snap,x,y)) >= 0.0)
        {   end = 1;  
            *pos = p1;
        }
        else if ((d1 = vs->pick(p2,snap,x,y)) >= 0.0)
        {   end = 2;  
            *pos = p2;
        }
        else if ((d1 = vs->pick(cs.getorigin(),snap,x,y)) >= 0.0)
        {   end = 3;  
            *pos = cs.getorigin();
        }
    }

  return end;
}
