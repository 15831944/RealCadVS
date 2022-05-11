
#include "ncwin.h"

LinearDimension::LinearDimension(Point org,Point xa,Point ya,SortedPointList l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,
                                 const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o)
{ list = l;  cs.set(org,xa,ya);
}

LinearDimension::LinearDimension(const EntityHeader &header,Point org,Point xa,Point ya,SortedPointList l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,
                                 const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(header,e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o)  
{ list = l;  cs.set(org,xa,ya);
}

LinearDimension::~LinearDimension()
{ list.destroy();
}


//
//  Calculate the position of the ith bit of text
Point LinearDimension::GetTextPosition(int index)
{Point p1,p2,p3,p4,p5,p6,p7,startp,textoffset,xaxis,yaxis;
 int c,common,stacked;
 double y,yfirst,xscale,yscale;
 BitMask toptions(32);

  common = options.test(0);
  stacked = options.test(1);  
  if (! common) 
    stacked = 0;

  xaxis = cs.getxaxis();
  yaxis = cs.getyaxis();
  xscale = yscale = 1.0;

  //  If the dimension is pointing into the view, reverse the X axis - Assume TOP view
  if ((xaxis.cross(yaxis)).z < 0.0)
    {  xaxis = -xaxis;
       xscale = -xscale;
    }

  //  If the xaxis is not pointing to the right, point it to the right.
  if (xaxis.x < -0.001 || xaxis.x < 0.001 && yaxis.y > -0.001)
    {  xaxis = -xaxis;
       yaxis = -yaxis;
       xscale = -xscale;
       yscale = -yscale;
    }

  CoordinateSystem newcs(cs.getorigin(),xaxis,yaxis);
  
  for (list.start(),textpointlist.start(),c = 0 ; ! list.atend() ; c++)
    {  p2 = list.next();  
       p2.x *= xscale;  p2.y *= yscale;  p2.z = 0.0;
       if (c == 1 && (options.test(9) || options.test(7) || options.test(8)))
         while (! list.atend()) list.next();
       p3 = p2;  p4 = p2;
       if (c == 0)
         {  y = 0.0;
            yfirst = p2.y;
         }
       else
         y = (c - 1) * stackdis * stacked;

       if (options.test(FixedDimensionPosition))
         {//  
            if (c == 0)
              y = (yfirst) * (yfirst > 0.0 ? -1.0 : 1.0) + stackdis;
            else
              y = (yfirst ) * (yfirst > 0.0 ? -1.0 : 1.0) + stackdis + (c-1) * stackdis * stacked;
         }

       if (p2.y < 0.0)
         {  if (options.test(GapFromDimension))
              p3.y = y - gap;
            else
              p3.y += gap;
            p4.y =  extension + y;
            if (c == 0 && stacked) p4.y += stackdis * (list.length() - 2);
         }
       else
         {  y = -y;
            if (options.test(GapFromDimension))
              p3.y = y + gap;
            else
              p3.y -= gap;
            p4.y = -extension + y;
            if (c == 0 && stacked) p4.y -= stackdis * (list.length() - 2);
         }
       p3 = newcs.tomodel(p3);  p4 = newcs.tomodel(p4);
       if (c > 0)
         {  p3 = p1;  p3.y = y;
            if (stacked)
              p3.x = startp.x;
            p4 = p2;  p4.y = y;
            p5 = (p3 + p4) / 2.0;  
            p5.y += (textyoffset + valueinfo.height) / 4.0;
            p5 = newcs.tomodel(p5);
            if (! textpointlist.atend())
              p5 += textpointlist.next();
            if (c-1 == index)
              return p5;
         }

      p1 = p2; 

   }


  return Point(0.0,0.0,0.0);

}

void LinearDimension::generate(View3dSurface *v,DimensionCallBack dcb,void *data)
{Point p1,p2,p3,p4,p5,p6,p7,startp,textoffset,xaxis,yaxis;
 int c,common,stacked,hjust;
 double x,y,plen,totallen,vlen,slen,tlen,xo,yo,hgt,plotscale,xoffset,deltax,yfirst,xscale,yscale,valueoffset,lastvalueoffset;
 RCCHAR string[300],tolerancestring[300];
 LineTerminator *term;
 BitMask toptions(32);
 TextInfo *ti;

  common = options.test(0);
  stacked = options.test(1);  
  if (! common) 
    stacked = 0;

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
  

  plotscale = (v == 0 || options.test(ScaleText) ? 1.0 : v->getplotscale());

  if (options.test(DimensionReverse))
    list.reverse();

  valueoffset = 0.0;
  lastvalueoffset = 0.0;

  for (list.start(),textpointlist.start(),c = 0 ; ! list.atend() ; c++)
    {  p2 = list.next();  
       if (options.test(EnableDimensionOffsets))
         {  if (common)
              valueoffset += lastvalueoffset;
            else
              valueoffset = lastvalueoffset;
         }
       lastvalueoffset = p2.z;

       p2.x *= xscale;  p2.y *= yscale;  p2.z = 0.0;
       if (c == 1 && (options.test(9) || options.test(7) || options.test(8)))
         while (! list.atend()) list.next();
       p3 = p2;  p4 = p2;
       if (c == 0)
         {  y = 0.0;
            yfirst = p2.y;
         }
       else
         y = (c - 1) * stackdis * stacked;

       if (options.test(FixedDimensionPosition))
         {//  
            if (c == 0)
              y = (yfirst) * (yfirst > 0.0 ? -1.0 : 1.0) + stackdis;
            else
              y = (yfirst ) * (yfirst > 0.0 ? -1.0 : 1.0) + stackdis + (c-1) * stackdis * stacked;
         }

       if (p2.y < 0.0)
         {  if (options.test(GapFromDimension))
              p3.y = y - gap * plotscale;
            else
              p3.y += gap * plotscale;
            p4.y =  extension * plotscale + y;
            if (c == 0 && stacked) p4.y += stackdis * (list.length() - 2);
         }
       else
         {  y = -y;
            if (options.test(GapFromDimension))
              p3.y = y + gap * plotscale;
            else
              p3.y -= gap * plotscale;
            p4.y = -extension * plotscale + y;
            if (c == 0 && stacked) p4.y -= stackdis * (list.length() - 2);
         }
       p3 = newcs.tomodel(p3);  p4 = newcs.tomodel(p4);
       Line line((EntityHeader &)(*this),p3,p4);  
       if ((c != 0 || !options.test(HideLeftDimensionLine)) && 
           (c != list.length()-1 || !options.test(HideRightDimensionLine)))
         dcb(&line,data);

       if (c == 0)
         startp = p2;
       else if (c > 0)
         {  p3 = p1;  p3.y = y;
            if (stacked)
              p3.x = startp.x;
            p4 = p2;  p4.y = y;
            p5 = (p3 + p4) / 2.0;  p5.y += (textyoffset + valueinfo.height) * plotscale / 4.0;

            if (common)
              formatvalue(string,tolerancestring,fabs(p4.x - startp.x) + valueoffset,0);
            else
              formatvalue(string,tolerancestring,fabs(p4.x - p3.x) + valueoffset,0);
           
			//printQChar(tolerancestring);
            //printQChar(string);
            //qDebug() << "linear dim valueinfo : " << QString(valueinfo.text);
            //qDebug() << "linear dim toleranceinfo : " << QString(toleranceinfo.text);
            //qDebug() << "linear dim prefixinfo : " << QString(prefixinfo.text);
            //qDebug() << "linear dim suffixinfo : " << QString(suffixinfo.text);

			deltax = fabs(p4.x - p3.x);

            ti = (options.test(3) ? &valueinfo : &prefixinfo);    ti->options.set(9,! options.test(ScaleText));
            if (strlen(prefixinfo.text) == 0 ||
                 ! Text::extents(v,ti->font,ti->options,prefixinfo.text,
                                ti->width,ti->height,ti->vertspacing,&plen,&hgt,&xo,&yo))
              plen = 0.0;
            valueinfo.options.set(9,! options.test(ScaleText));
            if (strlen(string) == 0 || ! options.test(2) ||
                  ! Text::extents(v,valueinfo.font,valueinfo.options,string,
                                valueinfo.width,valueinfo.height,valueinfo.vertspacing,&vlen,&hgt,&xo,&yo))
              vlen = 0.0;
            ti = (options.test(ToleranceDefined) ? &toleranceinfo : &valueinfo);  ti->options.set(9,! options.test(ScaleText));
            if (strlen(tolerancestring) == 0 || ! options.test(2) ||
                  ! Text::extents(v,ti->font,ti->options,tolerancestring,
                                ti->width,ti->height,ti->vertspacing,&tlen,&hgt,&xo,&yo))
              tlen = 0.0;

            ti = (options.test(4) ? &valueinfo : &suffixinfo);  ti->options.set(9,! options.test(ScaleText));
            if (strlen(suffixinfo.text) == 0 ||
                  ! Text::extents(v,ti->font,ti->options,suffixinfo.text,
                                ti->width,ti->height,ti->vertspacing,&slen,&hgt,&xo,&yo))
              slen = 0.0;
            totallen = plen + vlen + tlen + slen;

            p3 = newcs.tomodel(p3);  p4 = newcs.tomodel(p4);  p5 = newcs.tomodel(p5);
            Line line((EntityHeader &)(*this),p3,p4);
            if ((list.length() != 2 || deltax < totallen + term1.length * plotscale || deltax > totallen + 3.0 * term1.length * plotscale || options.test(7) || options.test(8)) && ! options.test(10))
              dcb(&line,data);

            if ((list.length() == 2 && deltax < totallen + term1.length * plotscale && ! options.test(8)) || options.test(7)) //  Left text placement
              xoffset = -deltax/2.0 - term1.length * plotscale * 1.5 - totallen  / 2.0;
            else if (options.test(8))  //  Right text placement
              xoffset =  deltax/2.0 + term1.length * plotscale * 1.5 + totallen / 2.0;
            else
              xoffset = 0.0;

            if (options.test(3))
              {  ti = &valueinfo;
                 hjust = 2;
              }
            else
              {  ti = &prefixinfo;
                 hjust = ti->options.test(1) + ti->options.test(2) * 2;
              }
            toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
            switch (hjust)
              {  case 0 : x = -totallen / 2.0;  break;
                 case 1 : x = -totallen / 2.0 + plen / 2.0;  break;
                 case 2 : x = -totallen / 2.0 + plen;  break;
              }
            x += xoffset;

            if (! textpointlist.atend())
              p5 += textpointlist.next();

            if (strlen(prefixinfo.text) > 0)
              {Text text1((EntityHeader &)(*this),ti->font,p5+xaxis*x,xaxis,yaxis,
                              ti->width,ti->height,ti->vertspacing,ti->angle,0,prefixinfo.text,0,toptions);
                 dcb(&text1,data);
              }
            if (options.test(2))
              {  hjust = valueinfo.options.test(1) + valueinfo.options.test(2) * 2;
                 switch (hjust)
                   {  case 0 : x = -totallen / 2.0 + plen;  break;
                      case 1 : x = -totallen / 2.0 + plen + vlen / 2.0;  break;
                      case 2 : x = -totallen / 2.0 + plen + vlen;  break;
                   }
                 toptions = valueinfo.options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);    toptions.set(9,! options.test(ScaleText));
                 x += xoffset;
                 if (strlen(string) > 0)
                   {Text text2((EntityHeader &)(*this),valueinfo.font,p5+xaxis*x,xaxis,yaxis,
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
                   {  case 0 : x = -totallen / 2.0 + plen + vlen;  break;
                      case 1 : x = -totallen / 2.0 + plen + vlen + tlen / 2.0;  break;
                      case 2 : x = -totallen / 2.0 + plen + vlen + tlen;  break;
                   }
                 x += xoffset;
                 if (strlen(tolerancestring) > 0)
                   {Text text3((EntityHeader &)(*this),ti->font,p5+xaxis*x,xaxis,yaxis,
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
              {  case 0 : x = -totallen / 2.0 + plen + vlen + tlen;  break;
                 case 1 : x = -totallen / 2.0 + plen + vlen + tlen + slen / 2.0;  break;
                 case 2 : x = -totallen / 2.0 + plen + vlen + tlen + slen;  break;
              }
            x += xoffset;
            if (strlen(suffixinfo.text) > 0)
              {Text text4((EntityHeader &)(*this),ti->font,p5+xaxis*x,xaxis,yaxis,
                               ti->width,ti->height,ti->vertspacing,ti->angle,0,suffixinfo.text,0,toptions);
                 dcb(&text4,data);
              }
            if ((term = LineTerminator::create(plotscale,term1)) != NULL)
              {  term->generate(this,dcb,data,&newcs,list.length() == 2 && deltax <= totallen + 3.0 * term1.length * plotscale || options.test(DimLinePosition) || common && options.test(DimensionReverse) ? int(-1 * xscale) : int(1 * xscale),p2.x,y);
                 if (! common && ! list.atend() && ! term->symmetric())
                   term->generate(this,dcb,data,&newcs,list.length() == 2 && deltax <= totallen + 3.0 * term1.length * plotscale || options.test(DimLinePosition) ? int(1 * xscale) : int(-1 * xscale),p2.x,y);
                 else if (common && stacked && ! list.atend() && ! term->symmetric())
                   term->generate(this,dcb,data,&newcs,options.test(DimensionReverse) ? 1 : -1,startp.x,y + stackdis * (1 - 2 * (p2.y > 0.0)));
                 delete term;
              }
         }

       if (c == 1 || list.length() == 1)
         {  if (common && ! stacked)
              {  if ((term = LineTerminator::create(plotscale,term2)) != NULL)
                   {  term->generate(this,dcb,data,&newcs,list.length() == 2 && deltax <= totallen + 3.0 * term1.length * plotscale || options.test(DimLinePosition) ? int(1 * xscale) : int(-1 * xscale),startp.x,y);
                      delete term;
                   }
              }
            else if ((term = LineTerminator::create(plotscale,term1)) != NULL)
              {  term->generate(this,dcb,data,&newcs,list.length() == 2 && deltax <= totallen + 3.0 * term1.length * plotscale || options.test(DimLinePosition) || options.test(DimensionReverse) ? int(1 * xscale) : int(-1 * xscale),startp.x,y);
                 delete term;
              }
         }

       if (c == 1 && (list.length() == 2 && deltax < totallen + 3.0 * term1.length * plotscale || options.test(9) || options.test(7) || options.test(8)))
         {  p6 = startp;  p6.y = y;
            p7 = startp;  p7.y = y;
            p6.x -= term1.length * plotscale * 2.0;
            if (list.length() == 2 && deltax < totallen + term1.length * plotscale && ! options.test(8) || options.test(7))
              p6.x -= totallen;
            p6 = newcs.tomodel(p6);  p7 = newcs.tomodel(p7);
            Line line1((EntityHeader &)(*this),p6,p7);
            if (options.test(9) || list.length() == 2 && deltax < totallen + 3.0 * term1.length * plotscale || options.test(7))
              dcb(&line1,data);

            p6 = p2;  p6.y = y;
            p7 = p2;  p7.y = y;
            p7.x += term1.length * plotscale * 2.0;
            if (options.test(8))
              p7.x += totallen;
            p6 = newcs.tomodel(p6);  p7 = newcs.tomodel(p7);
            Line line2((EntityHeader &)(*this),p6,p7);
            if (options.test(9) || options.test(8) || list.length() == 2 && deltax <= totallen + 3.0 * term1.length * plotscale)
              dcb(&line2,data);
         }
       p1 = p2;
    }

  if (options.test(DimensionReverse))
    list.reverse();

}

int LinearDimension::npoints(void)
{ return 1 + list.length();
}

Point LinearDimension::point(int n)
{int c;
 Point p,tmp(0.0,0.0,0.0);
  if (n == 0)
    return cs.getorigin();
  else
    {  for (list.start(),c = 1 ; ! list.atend() ; c++)
         {  p = list.next();  if (c == n) return cs.tomodel(p);
         }
    }
  return tmp;
}

Point LinearDimension::nearp(Point)
{ return cs.getorigin();
}

int LinearDimension::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int stretched;
 PointList plist;
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
  stretched = sp->match(cs.getorigin());
  if (! stretched)
    {  for (list.start() ; ! stretched && ! list.atend() ; )
         stretched = sp->match(cs.tomodel(list.next()));
    }
  if (! stretched) return 1;
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
  for (list.start() ; ! list.atend() ; )
    if (sp->match(p = cs.tomodel(list.next())))
      plist.add(t.transform(p));
    else
      plist.add(p);
  list.destroy();
  if (sp->match(cs.getorigin()))
    cs.change(t.transform(cs.getorigin()),cs.getxaxis(),cs.getyaxis());
  for (plist.start() ; ! plist.atend() ; )
    list.add(cs.frommodel(plist.next()));
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int LinearDimension::move(Database *db,Transform &t,int erase,int save)
{Point zero(0.0,0.0,0.0);
 Entity *e;
 Point origin,xaxis,yaxis,zaxis,p,pt;
 SortedPointList spl;
 PointList pl,tpl;
 int i;

  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  if (erase) Entity::draw(DM_ERASE);
  if (db != 0)
    {
	  if (save)
	   db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  status |= 1;

  tpl = textpointlist;
  for (tpl.start(),i = 0 ; ! tpl.atend() ; i++)
    {  p = tpl.next();
       pt = GetTextPosition(i);
       p = (t.transform(pt) - t.transform(pt-p)) - p;
       pl.add(p);
    }
  pl.reverse();

  origin = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() > ::db.getptoler() && yaxis.length() > ::db.getptoler())
    {  CoordinateSystem newcs(origin,xaxis,yaxis);
       for (list.start() ; ! list.atend() ; )
         spl.add(newcs.frommodel(t.transform(cs.tomodel(list.next())))); 
       list.destroy(); 
       list = spl;

       cs.change(origin,xaxis,yaxis);

    } 

  for (pl.start(),i = 0 ; ! pl.atend() ; i++)
    movetext(i,pl.next());
  pl.destroy();

  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *LinearDimension::clone(Transform &t)
{Point zero(0.0,0.0,0.0),p;
 Point origin,xaxis,yaxis,zaxis,pt;
 int i;
 SortedPointList spl;
 PointList pl,tpl;

  if (copye != NULL) return copye;

  tpl = textpointlist;
  for (tpl.start(),i = 0 ; ! tpl.atend() ; i++)
    {  p = tpl.next();
       pt = GetTextPosition(i);
       p = t.transform(pt) - t.transform(pt-p);
       pl.add(p);
    }
  pl.reverse();

  origin = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) return 0;

  CoordinateSystem newcs(origin,xaxis,yaxis);
  for (list.start() ; ! list.atend() ; )
    spl.add(newcs.frommodel(t.transform(cs.tomodel(list.next())))); 

  copye = new LinearDimension(*this,origin,xaxis,yaxis,spl,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                              precision,tolprecision,term1,term2,valueinfo,toleranceinfo,prefixinfo,suffixinfo,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);

  for (pl.start(),i = 0 ; ! pl.atend() ; i++)
    ((Dimension *)copye)->movetext(i,pl.next());
  pl.destroy();

  return copye;
}

int LinearDimension::project(Database *db,Transform &t,int)
{  return db->geometry.add(clone(t));
}

int CoordinateSystem::save(CadFile *outfile) const
{ if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  return 1;
}

int CoordinateSystem::load(CadFile *infile)
{ if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  return 1;
}

int TerminatorInfo::save(CadFile *outfile)
{ if (! outfile->write(&style)) return 0;
  if (! outfile->write(&length)) return 0;
  if (! outfile->write(&height)) return 0;
  if ((style & 0x80) != 0)
    if (! outfile->write(&weight)) return 0;
  return 1;
}

int TerminatorInfo::load(CadFile *infile)
{ if (! infile->read(&style)) return 0;
  if (! infile->read(&length)) return 0;
  if (! infile->read(&height)) return 0;
  if ((style & 0x80) != 0)
    if (! infile->read(&weight)) return 0;
  return 1;
}

int TextInfo::save(CadFile *outfile)
{short hc;
  if (! outfile->write(font)) return 0;
  if (! outfile->write(text)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&vertspacing)) return 0;
  if (! outfile->write(&options)) return 0;
  if (options.test(12))
    {  hc = halocolour;
       if (! outfile->write(&hc)) return 0;
    }

  return 1;
}

int TextInfo::load(CadFile *infile)
{
  short hc;
  if(infile->getversion() == 2)
  {
      RCCHAR *RCtemp=0;
      char *ctemp=0;

      if (! infile->read(&ctemp)) return 0;
      font = new RCCHAR[strlen(ctemp)+1];
      strcpy(font,ctemp);
      delete [] ctemp;
      if (! infile->read(&ctemp)) return 0;
      text = new RCCHAR[strlen(ctemp)+1];
      strcpy(text,ctemp);
      delete [] ctemp;
  }
  else
  {
      if (! infile->read(&font)) return 0;
      if (! infile->read(&text)) return 0;
  }
  if (! infile->read(&width)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&angle)) return 0;
  if (! infile->read(&vertspacing)) return 0;
  if (! infile->read(&options)) return 0;
  if (options.test(12))
    {  if (! infile->read(&hc)) return 0;
       halocolour = (Byte) hc;
    }
  else
    halocolour = 0;
  return 1;
}

int LinearDimension::save(CadFile *outfile)
{short type;  
  type = lineardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! cs.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
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

Entity *LinearDimension::load(int,CadFile *infile)
{LinearDimension *lineardimension;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 double gap,extension,uppertol,lowertol,stackdis,scale,textyoffset;
 short precision,tolprecision;
 BitMask options(32);
 SortedPointList list;
 PointList textpointlist;
 TerminatorInfo term1,term2;
 TextInfo ti1,ti2,ti3,ti4;
 RCCHAR *equation;
  if ((header = loadheader(infile)) == NULL) 
    return 0;
  if (! origin.load(infile)) 
    return 0;
  if (! xaxis.load(infile)) 
    return 0;
  if (! yaxis.load(infile)) 
    return 0;
  if (! list.load(infile)) 
    return 0;
  if (! infile->read(&gap))
    return 0;
  if (! infile->read(&extension)) 
    return 0;
  if (! infile->read(&stackdis)) 
    return 0;
  if (! infile->read(&uppertol)) 
    return 0;
  if (! infile->read(&lowertol)) 
    return 0;
  if (! infile->read(&precision)) 
    return 0;
  if (! infile->read(&tolprecision)) 
    return 0;
  if (! ti1.load(infile)) 
    return 0;
  if (! ti3.load(infile)) 
    return 0;
  if (! ti4.load(infile)) 
    return 0;
  if (! term1.load(infile)) 
    return 0;
  if (! term2.load(infile)) 
    return 0;
  if (! infile->read(&options)) 
    return 0; 
  if (options.test(ToleranceDefined))
    {  if (! ti2.load(infile)) 
         return 0;
    }
  else
    {  ti2 = ti1;
       ti2.font = new RCCHAR [RCSTRLEN(ti1.font)+1];
       ti2.text = new RCCHAR  [RCSTRLEN(ti1.text)+1];
       strcpy(ti2.font,ti1.font);
       strcpy(ti2.text,ti1.text);
    }
  if (options.test(ScaleDefined))
    {  if (! infile->read(&scale)) 
         return 0;
    }
  else
    scale = 1.0;
  if (options.test(DimensionTextYOffset))
    {  if (! infile->read(&textyoffset)) 
         return 0;
    }
  else
    textyoffset = 0.0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.load(infile))
      return 0;
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

  lineardimension = new LinearDimension(*header,origin,xaxis,yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                                        precision,tolprecision,term1,term2,ti1,ti2,ti3,ti4,options);
  lineardimension->atlist = header->getatlist();
  lineardimension->textpointlist = textpointlist;
  delete lineardimension->equation;
  lineardimension->equation = equation;
  delete header;
  delete [] ti1.text;  delete [] ti1.font;
  delete [] ti2.text;  delete [] ti2.font;
  delete [] ti3.text;  delete [] ti3.font;
  delete [] ti4.text;  delete [] ti4.font;
  return lineardimension;
}

int LinearDimension::save(UndoFile *outfile)
{short type;
  type = lineardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
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
  if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *LinearDimension::load(UndoFile *infile)
{LinearDimension *ld;
 SortedPointList empty;
 PointList textpointempty;
 char bm[sizeof(BitMask)];
  if ((ld = new LinearDimension) != NULL)
    {AttributeList al;
       memcpy(bm,&ld->visible,sizeof(bm));
       if (! infile->blockread(ld,sizeof(*ld))) return 0;
       memcpy(&ld->visible,bm,sizeof(bm));
       if (! infile->read(&ld->visible)) return 0;
       if (! al.load(infile)) return 0;   ld->atlist = al;
       ld->list = empty;
       if (! ld->list.load(infile)) return 0;
       ld->textpointlist = textpointempty;
       if (! ld->textpointlist.load(infile)) return 0;
       if (! infile->read(&ld->valueinfo.text)) return 0;
       if (! infile->read(&ld->valueinfo.font)) return 0;
       if (! infile->read(&ld->toleranceinfo.text)) return 0;
       if (! infile->read(&ld->toleranceinfo.font)) return 0;
       if (! infile->read(&ld->prefixinfo.text)) return 0;
       if (! infile->read(&ld->prefixinfo.font)) return 0;
       if (! infile->read(&ld->suffixinfo.text)) return 0;
       if (! infile->read(&ld->suffixinfo.font)) return 0;
       if (! infile->read(&ld->valueinfo.options)) return 0;
       if (! infile->read(&ld->prefixinfo.options)) return 0;
       if (! infile->read(&ld->suffixinfo.options)) return 0;
       if (! infile->read(&ld->options)) return 0;
       if (! infile->read(&ld->equation)) return 0;
    }
  return ld;
}

int LinearDimension::saveattributes(CadFile *outfile)
{ if (! outfile->write(&gap)) return 0;
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

int LinearDimension::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  delete [] valueinfo.text;   delete [] valueinfo.font;
  delete [] prefixinfo.text;  delete [] prefixinfo.font;
  delete [] suffixinfo.text;  delete [] suffixinfo.font;
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

int LinearDimension::savedxf(int blocksection,DXFOut *dxfout)
{
   if (! blocksection)
    {   generate(dxfout->getwindow(),dxfcb,dxfout);
    }
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

int LinearDimension::exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter)
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


int LinearDimension::savegeometry(CadFile *outfile)
{ if (! cs.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
  return 1;
}

int LinearDimension::loadgeometry(CadFile *infile)
{
  cadwindow->invalidatedisplaylist(this);
  list.destroy();
  textpointlist.destroy();
  if (! cs.load(infile)) return 0;
  if (! list.load(infile)) return 0;
  if (! textpointlist.load(infile)) return 0;
  return 1;
}

void Dimension::getdefaults(double *extension,double *gap,double *stackdis,double *uppertol,double *lowertol,double *scale,double *textyoffset,int *precision,int *tolprecision,TerminatorInfo *term1,TerminatorInfo *term2,TextInfo *textinfo1,TextInfo *textinfo2,
                            TextInfo *textinfo3,TextInfo *textinfo4,BitMask *op)
{ *extension = v.getreal("di::extension");
  *gap = v.getreal("di::gap");
  *stackdis = v.getreal("di::stackdis");
  *scale = v.getreal("di::scale");
  *textyoffset = v.getreal("di::textyoffset");
  *precision = v.getinteger("di::precision");
  *tolprecision = v.getinteger("di::tolprecision");
  *uppertol = v.getreal("di::uppertol");
  *lowertol = v.getreal("di::lowertol");
  term1->style = Byte(v.getinteger("di::term1style"));
  term1->length = v.getreal("di::term1length");
  term1->height = v.getreal("di::term1height");
  term1->weight = Byte(v.getinteger("di::term1weight"));
  term2->style = Byte(v.getinteger("di::term2style"));
  term2->length = v.getreal("di::term2length");
  term2->height = v.getreal("di::term2height");
  term2->weight = Byte(v.getinteger("di::term2weight"));
#ifdef _USING_QTCHAR
  textinfo1->font = (RCCHAR*)v.getstring("di::vtextfont",textinfo1->font);
  textinfo1->text = (RCCHAR*)v.getstring("di::vtexttext",textinfo1->text);
#else
  textinfo1->font = v.getstring("di::vtextfont");
  textinfo1->text = v.getstring("di::vtexttext");
#endif
  textinfo1->height = v.getreal("di::vtextheight");
  textinfo1->halocolour = v.getinteger("di::vhalocolour");
  textinfo1->width = v.getreal("di::vtextwidth");
  textinfo1->angle = v.getreal("di::vtextangle");
  textinfo1->vertspacing = v.getreal("di::vtextvertspacing");
  textinfo1->options = *v.getbm("di::vtextoptions");
#ifdef _USING_QTCHAR
  textinfo2->font = v.getstring("di::ttextfont",textinfo2->font);
  textinfo2->text = v.getstring("di::ttexttext",textinfo2->text);
#else
  textinfo2->font = v.getstring("di::ttextfont");
  textinfo2->text = v.getstring("di::ttexttext");
#endif
  textinfo2->height = v.getreal("di::ttextheight");
  textinfo2->halocolour = v.getinteger("di::thalocolour");
  textinfo2->width = v.getreal("di::ttextwidth");
  textinfo2->angle = v.getreal("di::ttextangle");
  textinfo2->vertspacing = v.getreal("di::ttextvertspacing");
  textinfo2->options = *v.getbm("di::ttextoptions");
#ifdef _USING_QTCHAR
  textinfo3->font = v.getstring("di::ptextfont",textinfo3->font);
  textinfo3->text = v.getstring("di::ptexttext",textinfo3->text);
#else
  textinfo3->font = v.getstring("di::ptextfont");
  textinfo3->text = v.getstring("di::ptexttext");
#endif
  textinfo3->height = v.getreal("di::ptextheight");
  textinfo3->halocolour = v.getinteger("di::phalocolour");
  textinfo3->width = v.getreal("di::ptextwidth");
  textinfo3->angle = v.getreal("di::ptextangle");
  textinfo3->vertspacing = v.getreal("di::ptextvertspacing");
  textinfo3->options = *v.getbm("di::ptextoptions");
#ifdef _USING_QTCHAR
  textinfo4->font = v.getstring("di::stextfont",textinfo4->font);
  textinfo4->text = v.getstring("di::stexttext",textinfo4->text);
#else
  textinfo4->font = v.getstring("di::stextfont");
  textinfo4->text = v.getstring("di::stexttext");
#endif
  textinfo4->height = v.getreal("di::stextheight");
  textinfo4->halocolour = v.getinteger("di::shalocolour");
  textinfo4->width = v.getreal("di::stextwidth");
  textinfo4->angle = v.getreal("di::stextangle");
  textinfo4->vertspacing = v.getreal("di::stextvertspacing");
  textinfo4->options = *v.getbm("di::stextoptions");
  *op = *v.getbm("di::options");
}

void Dimension::setdefaults(double extension, double gap, double stackdis, double uppertol, double lowertol, double scale, double textyoffset, int precision, int tolprecision, const TerminatorInfo &term1, const TerminatorInfo &term2, const TextInfo &textinfo1,
                            const TextInfo &textinfo2,
                            const TextInfo &textinfo3, const TextInfo &textinfo4, const BitMask &op)
{ v.setreal("di::extension",extension);
  v.setreal("di::gap",gap);
  v.setreal("di::stackdis",stackdis);
  v.setreal("di::uppertol",uppertol);
  v.setreal("di::lowertol",lowertol);
  v.setreal("di::scale",scale);
  v.setreal("di::textyoffset",textyoffset);
  v.setinteger("di::precision",precision);
  v.setinteger("di::tolprecision",tolprecision);
  v.setinteger("di::term1style",term1.style);
  v.setinteger("di::term1weight",term1.weight);
  v.setreal("di::term1length",term1.length);
  v.setreal("di::term1height",term1.height);
  v.setinteger("di::term2style",term2.style);
  v.setinteger("di::term2weight",term2.weight);
  v.setreal("di::term2length",term2.length);
  v.setreal("di::term2height",term2.height);
  v.setstring("di::vtextfont",textinfo1.font);
  v.setstring("di::vtexttext",textinfo1.text);
  v.setreal("di::vtextheight",textinfo1.height);
  v.setreal("di::vtextwidth",textinfo1.width);
  v.setinteger("di::vhalocolour",textinfo1.halocolour);
  v.setreal("di::vtextangle",textinfo1.angle);
  v.setreal("di::vtextvertspacing",textinfo1.vertspacing);
  v.setbm("di::vtextoptions",textinfo1.options);
  v.setstring("di::ttextfont",textinfo2.font);
  v.setstring("di::ttexttext",textinfo2.text);
  v.setreal("di::ttextheight",textinfo2.height);
  v.setinteger("di::thalocolour",textinfo2.halocolour);
  v.setreal("di::ttextwidth",textinfo2.width);
  v.setreal("di::ttextangle",textinfo2.angle);
  v.setreal("di::ttextvertspacing",textinfo2.vertspacing);
  v.setbm("di::ttextoptions",textinfo2.options);
  v.setstring("di::ptextfont",textinfo3.font);
  v.setstring("di::ptexttext",textinfo3.text);
  v.setreal("di::ptextheight",textinfo3.height);
  v.setinteger("di::phalocolour",textinfo3.halocolour);
  v.setreal("di::ptextwidth",textinfo3.width);
  v.setreal("di::ptextangle",textinfo3.angle);
  v.setreal("di::ptextvertspacing",textinfo3.vertspacing);
  v.setbm("di::ptextoptions",textinfo3.options);
  v.setstring("di::stextfont",textinfo4.font);
  v.setstring("di::stexttext",textinfo4.text);
  v.setreal("di::stextheight",textinfo4.height);
  v.setinteger("di::shalocolour",textinfo4.halocolour);
  v.setreal("di::stextwidth",textinfo4.width);
  v.setreal("di::stextangle",textinfo4.angle);
  v.setreal("di::stextvertspacing",textinfo4.vertspacing);
  v.setbm("di::stextoptions",textinfo4.options);
  v.setbm("di::options",op);
}

void Dimension::setdefaults(void)
{ 


    if (isa(circulardimension_entity) && ((CircularDimension *)this)->getcircle() == 0)
    {//  This is a leader - should the simpler leader settings

        v.setreal("le::extension",extension);
        v.setreal("le::gap",gap);
        v.setreal("le::stackdis",stackdis);
        v.setreal("le::uppertol",uppertol);
        v.setreal("le::lowertol",lowertol);
        v.setreal("le::scale",scale);
        v.setinteger("le::precision",precision);
        v.setinteger("le::tolprecision",tolprecision);
        v.setinteger("le::term1style",term1.style);
        v.setinteger("le::term1weight",term1.weight);
        v.setreal("le::term1length",term1.length);
        v.setreal("le::term1height",term1.height);
        v.setinteger("le::term2style",term2.style);
        v.setreal("le::term2length",term2.length);
        v.setreal("le::term2height",term2.height);
        v.setstring("le::vtextfont",valueinfo.font);
        v.setstring("le::vtexttext",valueinfo.text);
        v.setreal("le::vtextheight",valueinfo.height);
        v.setinteger("le::vhalocolour",valueinfo.halocolour);
        v.setreal("le::vtextwidth",valueinfo.width);
        v.setreal("le::vtextangle",valueinfo.angle);
        v.setreal("le::vtextvertspacing",valueinfo.vertspacing);
        v.setbm("le::vtextoptions",valueinfo.options);
        v.setstring("le::ttextfont",toleranceinfo.font);
        v.setstring("le::ttexttext",toleranceinfo.text);
        v.setreal("le::ttextheight",toleranceinfo.height);
        v.setinteger("le::thalocolour",toleranceinfo.halocolour);
        v.setreal("le::ttextwidth",toleranceinfo.width);
        v.setreal("le::ttextangle",toleranceinfo.angle);
        v.setreal("le::ttextvertspacing",toleranceinfo.vertspacing);
        v.setbm("le::ttextoptions",toleranceinfo.options);
        v.setstring("le::ptextfont",prefixinfo.font);
        v.setstring("le::ptexttext",prefixinfo.text);
        v.setreal("le::ptextheight",prefixinfo.height);
        v.setinteger("le::phalocolour",prefixinfo.halocolour);
        v.setreal("le::ptextwidth",prefixinfo.width);
        v.setreal("le::ptextangle",prefixinfo.angle);
        v.setreal("le::ptextvertspacing",prefixinfo.vertspacing);
        v.setbm("le::ptextoptions",prefixinfo.options);
        v.setstring("le::stextfont",suffixinfo.font);
        v.setstring("le::stexttext",suffixinfo.text);
        v.setreal("le::stextheight",suffixinfo.height);
        v.setinteger("le::shalocolour",suffixinfo.halocolour);
        v.setreal("le::stextwidth",suffixinfo.width);
        v.setreal("le::stextangle",suffixinfo.angle);
        v.setreal("le::stextvertspacing",suffixinfo.vertspacing);
        v.setbm("le::stextoptions",suffixinfo.options);
        v.setbm("le::options",options);
        return;
    }

    v.setreal("di::extension",extension);
    v.setreal("di::gap",gap);
    v.setreal("di::stackdis",stackdis);
    v.setreal("di::uppertol",uppertol);
    v.setreal("di::lowertol",lowertol);
    v.setreal("di::scale",scale);
    v.setreal("di::textyoffset",textyoffset);
    v.setinteger("di::precision",precision);
    v.setinteger("di::tolprecision",tolprecision);
    v.setinteger("di::term1style",term1.style);
    v.setinteger("di::term1weight",term1.weight);
    v.setreal("di::term1length",term1.length);
    v.setreal("di::term1height",term1.height);
    v.setinteger("di::term2style",term2.style);
    v.setinteger("di::term2weight",term2.weight);
    v.setreal("di::term2length",term2.length);
    v.setreal("di::term2height",term2.height);
    v.setstring("di::vtextfont",valueinfo.font);
    v.setstring("di::vtexttext",valueinfo.text);
    v.setreal("di::vtextheight",valueinfo.height);
    v.setreal("di::vtextwidth",valueinfo.width);
    v.setinteger("di::vhalocolour",valueinfo.halocolour);
    v.setreal("di::vtextangle",valueinfo.angle);
    v.setreal("di::vtextvertspacing",valueinfo.vertspacing);
    v.setbm("di::vtextoptions",valueinfo.options);
    v.setstring("di::ttextfont",toleranceinfo.font);
    v.setstring("di::ttexttext",toleranceinfo.text);
    v.setreal("di::ttextheight",toleranceinfo.height);
    v.setinteger("di::thalocolour",toleranceinfo.halocolour);
    v.setreal("di::ttextwidth",toleranceinfo.width);
    v.setreal("di::ttextangle",toleranceinfo.angle);
    v.setreal("di::ttextvertspacing",toleranceinfo.vertspacing);
    v.setbm("di::ttextoptions",toleranceinfo.options);
    v.setstring("di::ptextfont",prefixinfo.font);
    v.setstring("di::ptexttext",prefixinfo.text);
    v.setreal("di::ptextheight",prefixinfo.height);
    v.setinteger("di::phalocolour",prefixinfo.halocolour);
    v.setreal("di::ptextwidth",prefixinfo.width);
    v.setreal("di::ptextangle",prefixinfo.angle);
    v.setreal("di::ptextvertspacing",prefixinfo.vertspacing);
    v.setbm("di::ptextoptions",prefixinfo.options);
    v.setstring("di::stextfont",suffixinfo.font);
    v.setstring("di::stexttext",suffixinfo.text);
    v.setreal("di::stextheight",suffixinfo.height);
    v.setinteger("di::shalocolour",suffixinfo.halocolour);
    v.setreal("di::stextwidth",suffixinfo.width);
    v.setreal("di::stextangle",suffixinfo.angle);
    v.setreal("di::stextvertspacing",suffixinfo.vertspacing);
    v.setbm("di::stextoptions",suffixinfo.options);
    v.setbm("di::options",options);
}

void Dimension::getleaderdefaults(double *extension,double *gap,double *stackdis,double *uppertol,double *lowertol,double *scale,int *precision,int *tolprecision,TerminatorInfo *term1,TerminatorInfo *term2,
	TextInfo *textinfo1,TextInfo *textinfo2,TextInfo *textinfo3,TextInfo *textinfo4,BitMask *op)
{ *extension = v.getreal("le::extension");
  *gap = v.getreal("le::gap");
  *stackdis = v.getreal("le::stackdis");
  *scale = v.getreal("le::scale");
  *precision = v.getinteger("le::precision");
  *tolprecision = v.getinteger("le::tolprecision");
  *uppertol = v.getreal("le::uppertol");
  *lowertol = v.getreal("le::lowertol");
  term1->style = Byte(v.getinteger("le::term1style"));
  term1->length = v.getreal("le::term1length");
  term1->height = v.getreal("le::term1height");
  term1->weight = Byte(v.getinteger("le::term1weight"));
  term2->style = Byte(v.getinteger("le::term2style"));
  term2->length = v.getreal("le::term2length");
  term2->height = v.getreal("le::term2height");
  term2->weight = 0;
#ifdef _USING_QTCHAR
  textinfo1->font = v.getstring("le::vtextfont",textinfo1->font);
  textinfo1->text = v.getstring("le::vtexttext",textinfo1->text);
#else
  textinfo1->font = v.getstring("le::vtextfont");
  textinfo1->text = v.getstring("le::vtexttext");
#endif
  textinfo1->height = v.getreal("le::vtextheight");
  textinfo1->halocolour = v.getinteger("le::vhalocolour");
  textinfo1->width = v.getreal("le::vtextwidth");
  textinfo1->angle = v.getreal("le::vtextangle");
  textinfo1->vertspacing = v.getreal("le::vtextvertspacing");
  textinfo1->options = *v.getbm("le::vtextoptions");
#ifdef _USING_QTCHAR
  textinfo2->font = v.getstring("le::ttextfont",textinfo2->font);
  textinfo2->text = v.getstring("le::ttexttext",textinfo2->text);
#else
  textinfo2->font = v.getstring("le::ttextfont");
  textinfo2->text = v.getstring("le::ttexttext");
#endif
  textinfo2->height = v.getreal("le::ttextheight");
  textinfo2->halocolour = v.getinteger("le::thalocolour");
  textinfo2->width = v.getreal("le::ttextwidth");
  textinfo2->angle = v.getreal("le::ttextangle");
  textinfo2->vertspacing = v.getreal("le::ttextvertspacing");
  textinfo2->options = *v.getbm("le::ttextoptions");
#ifdef _USING_QTCHAR
  textinfo3->font = v.getstring(_RCT("le::ptextfont"));
  textinfo3->text = v.getstring(_RCT("le::ptexttext"));
#else
  textinfo3->font = v.getstring("le::ptextfont");
  textinfo3->text = v.getstring("le::ptexttext");
#endif
  textinfo3->height = v.getreal("le::ptextheight");
  textinfo3->halocolour = v.getinteger("le::phalocolour");
  textinfo3->width = v.getreal("le::ptextwidth");
  textinfo3->angle = v.getreal("le::ptextangle");
  textinfo3->vertspacing = v.getreal("le::ptextvertspacing");
  textinfo3->options = *v.getbm("le::ptextoptions");
#ifdef _USING_QTCHAR
  textinfo4->font = v.getstring("le::stextfont",textinfo4->font);
  textinfo4->text = v.getstring("le::stexttext",textinfo4->text);
#else
  textinfo4->font = v.getstring("le::stextfont");
  textinfo4->text = v.getstring("le::stexttext");
#endif
  textinfo4->height = v.getreal("le::stextheight");
  textinfo4->halocolour = v.getinteger("le::shalocolour");
  textinfo4->width = v.getreal("le::stextwidth");
  textinfo4->angle = v.getreal("le::stextangle");
  textinfo4->vertspacing = v.getreal("le::stextvertspacing");
  textinfo4->options = *v.getbm("le::stextoptions");
  *op = *v.getbm("le::options");
  op->set(DimensionFreeLeader,1);
  op->clear(3);  //  Dont use value text information
}

void Dimension::setleaderdefaults(double extension,double gap,double stackdis,double uppertol,double lowertol,double scale,int precision,int tolprecision,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &textinfo1,
                            const TextInfo &textinfo2,
                            const TextInfo &textinfo3,const TextInfo &textinfo4,const BitMask &op)
{
  v.setreal("le::extension",extension);
  v.setreal("le::gap",gap);
  v.setreal("le::stackdis",stackdis);
  v.setreal("le::uppertol",uppertol);
  v.setreal("le::lowertol",lowertol);
  v.setreal("le::scale",scale);
  v.setinteger("le::precision",precision);
  v.setinteger("le::tolprecision",tolprecision);
  v.setinteger("le::term1style",term1.style);
  v.setinteger("le::term1weight",term1.weight);
  v.setreal("le::term1length",term1.length);
  v.setreal("le::term1height",term1.height);
  v.setinteger("le::term2style",term2.style);
  v.setreal("le::term2length",term2.length);
  v.setreal("le::term2height",term2.height);
  v.setstring("le::vtextfont",textinfo1.font);
  v.setstring("le::vtexttext",textinfo1.text);
  v.setreal("le::vtextheight",textinfo1.height);
  v.setinteger("le::vhalocolour",textinfo1.halocolour);
  v.setreal("le::vtextwidth",textinfo1.width);
  v.setreal("le::vtextangle",textinfo1.angle);
  v.setreal("le::vtextvertspacing",textinfo1.vertspacing);
  v.setbm("le::vtextoptions",textinfo1.options);
  v.setstring("le::ttextfont",textinfo2.font);
  v.setstring("le::ttexttext",textinfo2.text);
  v.setreal("le::ttextheight",textinfo2.height);
  v.setinteger("le::thalocolour",textinfo2.halocolour);
  v.setreal("le::ttextwidth",textinfo2.width);
  v.setreal("le::ttextangle",textinfo2.angle);
  v.setreal("le::ttextvertspacing",textinfo2.vertspacing);
  v.setbm("le::ttextoptions",textinfo2.options);
  v.setstring("le::ptextfont",textinfo3.font);
  v.setstring("le::ptexttext",textinfo3.text);
  v.setreal("le::ptextheight",textinfo3.height);
  v.setinteger("le::phalocolour",textinfo3.halocolour);
  v.setreal("le::ptextwidth",textinfo3.width);
  v.setreal("le::ptextangle",textinfo3.angle);
  v.setreal("le::ptextvertspacing",textinfo3.vertspacing);
  v.setbm("le::ptextoptions",textinfo3.options);
  v.setstring("le::stextfont",textinfo4.font);
  v.setstring("le::stexttext",textinfo4.text);
  v.setreal("le::stextheight",textinfo4.height);
  v.setinteger("le::shalocolour",textinfo4.halocolour);
  v.setreal("le::stextwidth",textinfo4.width);
  v.setreal("le::stextangle",textinfo4.angle);
  v.setreal("le::stextvertspacing",textinfo4.vertspacing);
  v.setbm("le::stextoptions",textinfo4.options);
  v.setbm("le::options",op);
}

int ldexplodecb(Entity *entity,void *)
{Transform identity;
 Entity *e;
  identity.identity();
  entity->setcopye(0);
  e = entity->clone(identity);
  db.geometry.add(e);
  return 1;
}

int Dimension::explode(void)
{ if (cadwindow->getcurrentwindow() == 0)
    return 0;
  generate(cadwindow->getcurrentwindow(),ldexplodecb,0);
  return 1;
}


//
//  When the EnableDimensionOffsets bit is set in the options field, the Z value is used as a length offset.
//
void LinearDimension::setoffset(int index,double offset)
{
 SortedPointList l;
 int i;
 Point p1;

  db.saveundo(UD_MOVED,this);

  for (list.start(),i = 0 ; ! list.atend() ; i++)
    {  p1 = list.next();
       if (i == index)
         p1.z = offset;
       else if (! options.test(EnableDimensionOffsets))
         p1.z = 0.0;
       l.add(p1);
    }
  list.destroy();
  list = l;

  options.set(EnableDimensionOffsets);

}

double LinearDimension::getoffset(int index)
{Point p1;

  if (! options.test(EnableDimensionOffsets))
    return 0.0;

  for (list.start() ; ! list.atend() ; index--)
    {  p1 = list.next();
       if (index == 0.0)
         return p1.z;
    }

  return 0.0;

}

void LinearDimension::addpoint(Point p,int update)
{PointList pl;
 int added;

  if (update)
    {  Entity::draw(DM_ERASE);
       db.saveundo(UD_MOVED,this);
    } 
  cadwindow->invalidatedisplaylist(this);
  p = cs.frommodel(p);
  p.z = 0.0;  //  Make sure the offset is 0
  list.add(p);

  added = 0;
  for (list.start(),textpointlist.start() ; ! list.atend() && ! textpointlist.atend() ; )
    {  if (! added && (list.next() - cs.frommodel(p)).length() < db.getptoler())
         {  added = 1;
            pl.add(Point(0.0,0.0,0.0));
         }
       else
         pl.add(textpointlist.next());
    }
  pl.reverse();
  textpointlist.destroy();
  textpointlist = pl; 

  if (update)
    Entity::draw(DM_NORMAL);
  if (update)
    {  if (getposition() != 0 && db.MovedCallback != 0)
         db.MovedCallback(this);
    }

}

void LinearDimension::delpoint(Point p,int exactmatch,int update)
{SortedPointList l;  
 PointList pl;
 Point p1;
 int iclosest,i,deleted;
 double xclosest;

  //  Need at least two points to define the dimension
  if (list.length() < 2)
    return;

  if (update)
    {  Entity::draw(DM_ERASE);
       db.saveundo(UD_MOVED,this);
    }
  cadwindow->invalidatedisplaylist(this);

  p = cs.frommodel(p);
  p.z = 0.0;

  iclosest = -1;  xclosest = 0.0;
  for (list.start(),i = 0 ; ! list.atend() ; i++)
    {  p1 = list.next();
       if (iclosest == -1 || fabs(p.x-p1.x) < xclosest)
         {  iclosest = i;
            xclosest = fabs(p.x - p1.x);
         }
    }


  deleted = 0;
  for (list.start(),textpointlist.start(),i = 0 ; ! list.atend() ; i++)
    {  p1 = list.next();
       if (deleted || ! exactmatch && i != iclosest || exactmatch && (p - p1).length() > db.getptoler())
         {  l.add(p1);
            if (! textpointlist.atend())
              pl.add(textpointlist.next());
         } 
       else
         {  deleted = 1;
            if (! textpointlist.atend())
              textpointlist.next();
         } 
    }
  pl.reverse();
  textpointlist.destroy();
  textpointlist = pl; 

  list.destroy();
  list = l;
  if (update)
    {  Entity::draw(DM_NORMAL);
       if (getposition() != 0 && db.MovedCallback != 0)
         db.MovedCallback(this);
    }

}

int LinearDimension::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1;
 int end;
  end = 0;
  if ((d1 = vs->pick(cs.getorigin(),distance,x,y)) >= 0.0)
    {  end = 1;  *pos = cs.getorigin();
    }
  return end;
}

void LinearDimension::movehandlepoint(View3dSurface *,int drawentity,int end,Point p)
{SortedPointList l;  
 Point p1,p2;

  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) Entity::draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (end == 1)
    {  p1 = cs.frommodel(p);
       p1.x = p1.z = 0.0;
       cs.setorigin(cs.tomodel(p1));

       for (list.start() ; ! list.atend() ; )
         l.add(list.next() - p1);

       list.destroy();
       list = l;

    } 
  if (drawentity) Entity::draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
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

    // KMJ: text entities are using the qgo for now
    // ToDo: draw text with OpenGL?
#if 0
    if(entity->type() == text_entity)
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

void LinearDimension::drawGL(int drawmode,Transform *t,View3dSurface *v)
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

  // TODO: KMJ: fix display list to use Qt text????
  // just not using displaylists for now
/*
  if (t == NULL && db.getenableautoplotscale() == 0 && ! gethandles() && drawmode != DM_INVERT && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
  {
       if (v->displaylist.draw(this,drawmode))
       {
           if(qgi)
               v->getScene()->addItem(qgi);
           return;
       }
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode))
       {
           if(qgi)
               v->getScene()->addItem(qgi);
           return;
       }
  }
*/

  // KMJ: keeping the qgi for the text entities in the dimension
  //lddata.qgi = new QGraphicsItemGroup();

  lddata.drawmode = drawmode;
  lddata.t = t;
  lddata.v = v;

  generate(v,lddrawcbGL,&lddata);

  /*
  if(lddata.qgi)
  {
      qgi = lddata.qgi;
      v->getScene()->addItem(qgi);
  }
  */

  if (gethandles())
  {
      Point pt1;

      v->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_HANDLE_SELECT,colour,0,0,0);
      //v->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,Entity::style,weight,0);

      RCCOLORREF pcol = v->getcolour();
      //GLfloat fr=GetRValue(pcol)/255.0;
      //GLfloat fg=GetGValue(pcol)/255.0;
      //GLfloat fb=GetBValue(pcol)/255.0;
      //glColor3f(fr,fg,fb);
      glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
      GLfloat lwidth = v->curPen.widthF();
      glLineWidth(lwidth<1.0?1.0:lwidth);

      //pt1 = v->modeltoscreen(cs.getorigin());

      Point pright = v->getuaxis();
      Point pup = v->getvaxis();
      double sc = v->getpixelscale() * 6.0;

      v->cbox2dGL(cs.getorigin(),pright,pup,sc,sc);
      //v->cbox2dGL(pt1,v->getuaxis(),v->getvaxis(),sc,sc);

  }

}
#endif

void LinearDimension::draw(int drawmode,Transform *t,View3dSurface *v)
{
  LDDrawCB_data lddata;

  //qDebug() << "entering LinearDimension draw : ";
  //qDebug() << "entity qgi : " << qgi;

#ifdef USING_WIDGETS
  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() == v->getScene())
          v->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      if(qgihandle && qgihandle->scene() == v->getScene())
          v->getScene()->removeItem(qgi);
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
  //v->setup(linestylescale,0.0,drawmode,colour,Entity::style,weight,0);
  v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

  // TODO: KMJ: fix display list to use Qt text????
  // just not using displaylists for now
#if 0
  if (t == NULL && db.getenableautoplotscale() == 0 && ! gethandles() && drawmode != DM_INVERT && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
  {
       if (v->displaylist.draw(this,drawmode))
       {
           if(qgi)
               v->getScene()->addItem(qgi);
           return;
       }
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode))
       {
           if(qgi)
               v->getScene()->addItem(qgi);
           return;
       }
  }
#endif

  lddata.drawmode = drawmode;
  lddata.t = t;
  lddata.v = v;

#ifdef USING_WIDGETS
  lddata.qgi = (QGraphicsItemGroup*)qgi;

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
      Point pt1;

      QGraphicsItem *qgline=0;
      QGraphicsItemGroup *qgigroup = new QGraphicsItemGroup();

      if(qgigroup)
      {
           v->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,Entity::style,weight,0);

           pt1 = v->modeltoscreen(cs.getorigin());

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

           qgihandle = qgigroup;
           v->getScene()->addItem(qgihandle);
      }
  }

 // qDebug() << "leaving LinearDimension draw : ";
  //qDebug() << "entity qgi : " << qgi;

}
