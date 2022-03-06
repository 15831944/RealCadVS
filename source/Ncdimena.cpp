
#include "ncwin.h"
#include "dimension_dialog.h"
#include "dimensionterminator_dialog.h"
#include "dimensioncterminator_dialog.h"
#include "dimensionvalue_dialog.h"
#include "dimensionprefix_dialog.h"
#include "dimensionprefix0_dialog.h"
#include "dimensionsuffix0_dialog.h"
#include "leader_dialog.h"
#include <QFontComboBox>


Dimension::Dimension(double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgihandle=0;
#endif

  extension = e;  gap = g;  precision = short(p);  tolprecision = short(tp);
  stackdis = s;  uppertol = ut;  lowertol = lt;  scale = sc;  textyoffset = tyo;
  term1 = t1;  term2 = t2;
  term1.length = fabs(term1.length);
  term1.height = fabs(term1.height);
  term2.length = fabs(term2.length);
  term2.height = fabs(term2.height);
  options = o;
  valueinfo = ti1;  toleranceinfo = ti2;  prefixinfo = ti3;  suffixinfo = ti4;
  if (options.test(5) && options.test(6))
    {  valueinfo.options.set(2);  valueinfo.options.clear(1);
    }

  valueinfo.text = toleranceinfo.text = prefixinfo.text = suffixinfo.text = NULL;
  valueinfo.font = toleranceinfo.font = prefixinfo.font = suffixinfo.font = NULL;
  valueinfo.font = new RCCHAR[RCSTRLEN(ti1.font)+1];
  toleranceinfo.font = new RCCHAR[RCSTRLEN(ti2.font)+1];
  prefixinfo.font = new RCCHAR[RCSTRLEN(ti3.font)+1];
  suffixinfo.font = new RCCHAR[RCSTRLEN(ti4.font)+1];
  valueinfo.text = new RCCHAR[RCSTRLEN(ti1.text)+1];
  toleranceinfo.text = new RCCHAR[RCSTRLEN(ti2.text)+1];
  prefixinfo.text = new RCCHAR[RCSTRLEN(ti3.text)+1];
  suffixinfo.text = new RCCHAR[RCSTRLEN(ti4.text)+1];

  if (valueinfo.font == NULL || prefixinfo.font == NULL ||
      suffixinfo.font == NULL || valueinfo.text == NULL ||
      prefixinfo.text == NULL || suffixinfo.text == NULL ||
      toleranceinfo.font == NULL || toleranceinfo.text == NULL)
    defined = 0;
  else
    {  
	   RCSTRCPY(valueinfo.text,ti1.text);
       RCSTRCPY(toleranceinfo.text,ti2.text);
       RCSTRCPY(prefixinfo.text,ti3.text);
       RCSTRCPY(suffixinfo.text,ti4.text);
       RCSTRCPY(valueinfo.font,ti1.font);
       RCSTRCPY(toleranceinfo.font,ti2.font);
       RCSTRCPY(prefixinfo.font,ti3.font);
       RCSTRCPY(suffixinfo.font,ti4.font);
    }

  options.set(EquationDefined,1);
  equation = new RCCHAR[1];
  equation[0] = 0;

}

Dimension::Dimension(const EntityHeader &header,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Entity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgihandle=0;
#endif
   extension = e;  gap = g;  precision = short(p);  tolprecision = short(tp);
  stackdis = s;  uppertol = ut;  lowertol = lt;  scale = sc;    textyoffset = tyo;
  term1 = t1;  term2 = t2;
  options = o;
  valueinfo = ti1;  toleranceinfo = ti2;  prefixinfo = ti3;  suffixinfo = ti4;
  valueinfo.text = toleranceinfo.text = prefixinfo.text = suffixinfo.text = NULL;
  valueinfo.font = toleranceinfo.font = prefixinfo.font = suffixinfo.font = NULL;
  valueinfo.font = new RCCHAR[strlen(ti1.font)+1];
  toleranceinfo.font = new RCCHAR[strlen(ti2.font)+1];
  prefixinfo.font = new RCCHAR[strlen(ti3.font)+1];
  suffixinfo.font = new RCCHAR[strlen(ti4.font)+1];
  valueinfo.text = new RCCHAR[strlen(ti1.text)+1];
  toleranceinfo.text = new RCCHAR[strlen(ti2.text)+1];
  prefixinfo.text = new RCCHAR[strlen(ti3.text)+1];
  suffixinfo.text = new RCCHAR[strlen(ti4.text)+1];
  if (valueinfo.font == NULL || prefixinfo.font == NULL ||
      suffixinfo.font == NULL || valueinfo.text == NULL ||
      prefixinfo.text == NULL || suffixinfo.text == NULL ||
      toleranceinfo.font == NULL || toleranceinfo.text == NULL)
    defined = 0;
  else
    {  strcpy(valueinfo.text,ti1.text);
       strcpy(toleranceinfo.text,ti2.text);
       strcpy(prefixinfo.text,ti3.text);
       strcpy(suffixinfo.text,ti4.text);
       strcpy(valueinfo.font,ti1.font);
       strcpy(toleranceinfo.font,ti2.font);
       strcpy(prefixinfo.font,ti3.font);
       strcpy(suffixinfo.font,ti4.font);
    }

  options.set(EquationDefined,1);
  equation = new RCCHAR[1];
  equation[0] = 0;
}

Dimension::~Dimension()
{ 
  delete [] valueinfo.text;  delete [] toleranceinfo.text;  
  delete [] prefixinfo.text;  delete [] suffixinfo.text;
  delete [] valueinfo.font;  delete [] toleranceinfo.font; 
  delete [] prefixinfo.font;  delete [] suffixinfo.font;
  delete [] equation;
}

struct LDPickCB_data
  {View3dSurface *v;
   double x,y;
   Point *pnear;
   double dmin;
  };

int ldpickcb(Entity *entity,void *data)
{LDPickCB_data *lddata = (LDPickCB_data *) data;
 double d;
  if ((d = entity->pick(lddata->v,lddata->x,lddata->y,lddata->pnear)) >= 0.0 && (lddata->dmin < 0.0 || d < lddata->dmin))
    lddata->dmin = d;
  return 1;
}

double Dimension::pick(View3dSurface *vs,double x,double y,Point *pnear)
{LDPickCB_data lddata;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (pnear == NULL && vs->displaylist.defined(this))
    return vs->displaylist.pick(this,x,y);
  lddata.v = vs;
  lddata.x = x;
  lddata.y = y;
  lddata.pnear = pnear;
  lddata.dmin = -1.0;
  generate(vs,ldpickcb,&lddata);
  return lddata.dmin;
}

struct LDPickTextCB_data 
  {int ntext,i,index,x,y;
   Point *p; 
   View3dSurface *vs;
  };

int ldpicktextcb(Entity *entity,void *data)
{LDPickTextCB_data *lddata = (LDPickTextCB_data *) data;
  if (entity->isa(text_entity))
    {  if (entity->pick(lddata->vs,lddata->x,lddata->y,lddata->p) >= 0.0)
         lddata->index = lddata->i / lddata->ntext;
       lddata->i++;
    }    
  return 1;
}

int Dimension::picktextindex(View3dSurface *vs,int x,int y,Point *p)
{LDPickTextCB_data lddata;
  lddata.ntext = (strlen(prefixinfo.text)>0) + (strlen(suffixinfo.text)>0) + options.test(2) + ((options.test(5) + options.test(6) * 2) != 0);
  lddata.x = x;
  lddata.y = y;
  lddata.vs = vs;
  lddata.i = 0;
  lddata.index = -1;
  lddata.p = p;
  if (lddata.ntext != 0)
    generate(vs,ldpicktextcb,&lddata);
  return lddata.index;
}

void Dimension::movetext(int index,Point p)
{Point p1,zero(0.0,0.0,0.0);
 PointList list;
 int i;

  if (position != 0)
    {  db.saveundo(UD_MOVED,this);
       draw(DM_ERASE);
       cadwindow->invalidatedisplaylist(this);
    }

  for (textpointlist.start(),i = 0 ; ! textpointlist.atend() ; i++)
    {  p1 = textpointlist.next();
       if (i == index)
         list.add(p1+p);
       else
         list.add(p1);
    }
  if (i <= index)
    {  while (i < index)
         {  list.add(zero);  i++;
         }
       list.add(p); 
    }
  list.reverse();
  textpointlist.destroy();
  textpointlist = list;

  if (position != 0)
    draw(DM_NORMAL);

  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

}

struct LDPickWinCB_data
  {View3dSurface *v;
   int insidew,x1,y1,x2,y2;
   int found;
  };

int ldpickwincb(Entity *entity,void *data)
{LDPickWinCB_data *lddata = (LDPickWinCB_data *) data;
  if (! entity->pick(lddata->v,lddata->insidew,lddata->x1,lddata->y1,lddata->x2,lddata->y2))
    lddata->found = 0;
  return 1;
}

int Dimension::pick(View3dSurface *v,int insidew,int x1,int y1,int x2,int y2)
{LDPickWinCB_data lddata;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(v);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    v = window->getoffscreenbitmap();

  if (v->displaylist.defined(this))
    return v->displaylist.pick(this,insidew,x1,y1,x2,y2);
  lddata.v = v;
  lddata.insidew = insidew;
  lddata.x1 = x1;
  lddata.y1 = y1;
  lddata.x2 = x2;
  lddata.y2 = y2;
  lddata.found = 1;
  generate(v,ldpickwincb,&lddata);
  return lddata.found;
}


struct PWPickCB_data 
  {int picked;
   int nvertices;
   int inside;
   Point *pwvertices; 
   View3dSurface *vs;
  };

int pwpickcb(Entity *entity,void *data)
{PWPickCB_data *pwdata = (PWPickCB_data *) data;
 int picked;
  picked = entity->pick(pwdata->vs,pwdata->nvertices,pwdata->pwvertices,pwdata->inside);
  if ((pwdata->inside == 0 || pwdata->inside == 1) && picked == 0)
    pwdata->picked = 0;
  else if (pwdata->inside == 2 && picked == 1)
    pwdata->picked = 1;
  return 1;
}

int Dimension::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{PWPickCB_data pwdata;

 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();

  pwdata.vs = vs;
  pwdata.nvertices = nvertices;
  pwdata.pwvertices = pwvertices;
  pwdata.inside = inside;
  pwdata.picked = inside != 2;
  generate(vs,pwpickcb,&pwdata);
  return pwdata.picked;
}

void Dimension::formatvalue(RCCHAR *svalue,RCCHAR *stolerance,double x,int formatoptions)
{int dt,tolerance;
 RCCHAR text1[300],text2[300];

    tolerance = options.test(5) + options.test(6) * 2;
    x *= scale;

    dt = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(precision);

    if (tolerance == 0)
    {   RCSTRCPY(svalue,db.formatnumber(text1,x,formatoptions));
        RCSTRCPY(stolerance,"");
    }
    else if (tolerance == 1)
    {   RCSTRCPY(svalue,db.formatnumber(text1,x,formatoptions));
        db.setnumericdisplaytolerance(tolprecision);
		strcpy(text1,"");
        //RCSPRINTF(stolerance,_RCT("\261%ls"),db.formatnumber(text1,uppertol,formatoptions));
        RCCHAR formatString[20],PlusMinus = 0xB1;
        strcpy(formatString,PlusMinus); strcat(formatString,_RCT("%ls"));
        RCSPRINTF(stolerance,formatString,db.formatnumber(text1,uppertol,formatoptions));
    }
    else if (tolerance == 2)
    {   RCSPRINTF(svalue,"");
        db.setnumericdisplaytolerance(tolprecision);
        RCSPRINTF(stolerance,"%ls\r\n%ls",db.formatnumber(text1,x+lowertol,formatoptions),db.formatnumber(text2,x+uppertol,formatoptions));
    }
    else if (tolerance == 3)
    {   RCSTRCPY(svalue,db.formatnumber(text1,x,0));
        db.setnumericdisplaytolerance(tolprecision);
        RCSPRINTF(stolerance,"%ls\r\n%ls",db.formatnumber(text1,uppertol,2+formatoptions),db.formatnumber(text2,lowertol,2+formatoptions));
    }

    db.setnumericdisplaytolerance(dt);

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

int ldaddtodisplaylistcb(Entity *entity,void *data)
{ entity->addtodisplaylist((View3dSurface *)data,1);
  return 1;
}

void Dimension::addtodisplaylist(View3dSurface *v,int child)
{int k;
 long size;
  for (k = child,size = 0 ; k < 2 ; k++)
    {  if (! child)
         {  if (k == 0)
              v->displaylist.setcountmode();
            else
              v->displaylist.setsavemode();
            if (! v->displaylist.start(this,size)) return;
         }
       generate(v,ldaddtodisplaylistcb,v);
       size = v->displaylist.getcount();
    }
  if (! child && ! v->displaylist.end()) return;
}

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

void Dimension::drawGL(int drawmode,Transform *t,View3dSurface *v)
{
  LDDrawCB_data lddata;

  draw(DM_ERASE,t,v);

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

  RCCOLORREF pcol = v->getcolour();
  //GLfloat fr=GetRValue(pcol)/255.0;
  //GLfloat fg=GetGValue(pcol)/255.0;
  //GLfloat fb=GetBValue(pcol)/255.0;
  //glColor3f(fr,fg,fb);
  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = v->curPen.widthF();
  glLineWidth(lwidth<1.0?1.0:lwidth);

/*
  if (0 && t == NULL && db.getenableautoplotscale() == 0 && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
    {  if (v->displaylist.draw(this,drawmode)) return;
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode)) return;
    }
*/

  // KMJ: using qgi for text entities
  //lddata.qgi = new QGraphicsItemGroup();

  lddata.drawmode = drawmode;
  lddata.t = t;
  lddata.v = v;
  generate(v,lddrawcbGL,&lddata);

#if 0
  if(lddata.qgi)
  {
      qgi = lddata.qgi;
      v->getScene()->addItem(qgi);
      lddata.qgi = 0;
  }
#endif
}
#endif

void Dimension::draw(int drawmode,Transform *t,View3dSurface *v)
{
  LDDrawCB_data lddata;

  //qDebug() << "entering dimension draw : ";
  //qDebug() << "entity qgi : " << qgi;

#ifdef USING_WIDGETS
  if( drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() != v->getScene())
          qgi->scene()->removeItem(qgi);
      //v->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      if(qgihandle && qgihandle->scene() != v->getScene())
          qgihandle->scene()->removeItem(qgihandle);
      //if(v->getScene())
      //        v->getScene()->removeItem(qgihandle);
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
  v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

/*
  if (0 && t == NULL && db.getenableautoplotscale() == 0 && (term1.style & 0x80) == 0 && (term2.style&0x80) == 0)
    {  if (v->displaylist.draw(this,drawmode)) return;
       if (! v->displaylist.geterrorstatus())
         addtodisplaylist(v,0);
       if (v->displaylist.draw(this,drawmode)) return;
    }
*/
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
      lddata.qgi=0;

  }

  //qDebug() << "leaving dimension draw : ";
  //qDebug() << "entity qgi : " << qgi;

#else
  generate(v,lddrawcb,&lddata);
#endif
}

int ldhidecb(Entity *entity,void *data)
{
    if (entity->isa(text_entity))
    {
       BitMask options(32),change(32);
       change.set(20);
       options.set(11);
       ((Text *)entity)->change(change,*entity,0,0.0,0.0,0.0,0.0,0,0,0,options);
    }

    entity->hide((HideImage *)data);

    if(entity->qgi)
        if(((HideImage *)data)->qgi)
            ((QGraphicsItemGroup*)((HideImage *)data)->qgi)->addToGroup(entity->qgi);

  return 1;
}

int Dimension::hide(HideImage *image)
{
    qDebug() << "entering dimension hide : ";
    //qDebug() << "entity qgi : " << qgi;
    //qDebug() << "entity qgi : " << qgi->type();

    if(image->drawmode != 1) // drawmode 1 = printing
    {
        if(qgi)
            if(image->getsurface()->getScene())
                image->getsurface()->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
        image->qgi = 0;
    }

    qgi = new QGraphicsItemGroup();

    image->qgi = qgi;

    generate(image->getsurface(),ldhidecb,image);

    if(image->qgi)
    {
        qgi = image->qgi;
        if(image->getsurface()->getScene())
            image->getsurface()->getScene()->addItem(qgi);
        image->qgi=0;
    }

    qDebug() << "leaving dimension hide : ";
    qDebug() << "entity qgi : " << qgi;

    return 1;
}

int ldhidecbGL(Entity *entity,void *data)
{
    if (entity->isa(text_entity))
    {
       BitMask options(32),change(32);
       change.set(20);// cange bit Always draw, even when hiding.
       options.set(11);// set bit Always draw, even when hiding.
       ((Text *)entity)->change(change,*entity,0,0.0,0.0,0.0,0.0,0,0,0,options);
    }

    entity->hideGL((HideImage *)data);

  return 1;
}

int Dimension::hideGL(HideImage *image)
{
    qDebug() << "entering dimension hideGL : ";

    image->qgi = 0;

    generate(image->getsurface(),ldhidecbGL,image);

    qDebug() << "leaving dimension hideGL : ";

    return 1;
}

void Dimension::change(const BitMask &change,const EntityHeader&eh,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,
                       const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(5)) extension = e;
  if (change.test(6)) gap = g;
  if (change.test(7)) precision = short(p);
  if (change.test(8)) options.set(0,o.test(0));
  if (change.test(9)) options.set(1,o.test(1));
  if (change.test(10)) stackdis = s;
  if (change.test(11))
    {  options.set(5,o.test(5));  options.set(6,o.test(6));
    }
  if (change.test(12)) uppertol = ut;
  if (change.test(13)) lowertol = lt;
  if (change.test(14)) tolprecision = short(tp);

  //if (change.test(15)) term1.style = t1.style;
  if (change.test(15)) term1.style = (t1.style&0x7f) | (term1.style&0x80);
  if (change.test(16)) term1.length = t1.length;
  if (change.test(17)) term1.height = t1.height;

  //if (change.test(20)) term2.style = t2.style;
  if (change.test(20)) term2.style = (t2.style&0x7f) | (term2.style&0x80);
  if (change.test(21)) term2.length = t2.length;
  if (change.test(22)) term2.height = t2.height; 

  if (change.test(25))
    {RCCHAR *font;
       if ((font = new RCCHAR[strlen(ti1.font)+1]) != NULL)
         {  delete [] valueinfo.font;  valueinfo.font = font;  strcpy(font,ti1.font);
            valueinfo.options.set(0,ti1.options.test(0));
         }
    }
  if (change.test(26)) valueinfo.height = ti1.height;
  if (change.test(27)) valueinfo.width = ti1.width;
  if (change.test(28)) valueinfo.angle = ti1.angle;
  if (change.test(29))
    {  valueinfo.options.set(1,ti1.options.test(1));
       valueinfo.options.set(2,ti1.options.test(2));
    }
  if (change.test(30))
    {  valueinfo.options.set(3,ti1.options.test(3));
       valueinfo.options.set(4,ti1.options.test(4));
    }
  if (change.test(31)) valueinfo.vertspacing = ti1.vertspacing;
  if (change.test(32)) valueinfo.options.set(5,ti1.options.test(5));
  if (change.test(33)) valueinfo.options.set(6,ti1.options.test(6));
  if (change.test(34)) valueinfo.options.set(7,ti1.options.test(7));
  if (change.test(35)) valueinfo.options.set(8,ti1.options.test(8));
  if (change.test(36)) options.set(2,o.test(2));

  if (change.test(40))
  {
       RCCHAR *font;
       if ((font = new RCCHAR[RCSTRLEN(ti3.font)+1]) != NULL)
       {
            delete [] prefixinfo.font;
            strcpy(font,ti3.font);
            prefixinfo.font = font;
            prefixinfo.options.set(0,ti3.options.test(0));
       }
  }
  if (change.test(41)) prefixinfo.height = ti3.height;
  if (change.test(42)) prefixinfo.width = ti3.width;
  if (change.test(43)) prefixinfo.angle = ti3.angle;
  if (change.test(44))
    {  prefixinfo.options.set(1,ti3.options.test(1));
       prefixinfo.options.set(2,ti3.options.test(2));
    }
  if (change.test(45))
    {  prefixinfo.options.set(3,ti3.options.test(3));
       prefixinfo.options.set(4,ti3.options.test(4));
    }
  if (change.test(46)) prefixinfo.vertspacing = ti3.vertspacing;
  if (change.test(47)) prefixinfo.options.set(5,ti3.options.test(5));
  if (change.test(48)) prefixinfo.options.set(6,ti3.options.test(6));
  if (change.test(49)) prefixinfo.options.set(7,ti3.options.test(7));
  if (change.test(50)) prefixinfo.options.set(8,ti3.options.test(8));

  if (change.test(55))
    {RCCHAR *font;
       if ((font = new RCCHAR[strlen(ti4.font)+1]) != NULL)
         {  delete [] suffixinfo.font;  suffixinfo.font = font;  strcpy(font,ti4.font);
            suffixinfo.options.set(0,ti4.options.test(0));
         }
    }
  if (change.test(56)) suffixinfo.height = ti4.height;
  if (change.test(57)) suffixinfo.width = ti4.width;
  if (change.test(58)) suffixinfo.angle = ti4.angle;
  if (change.test(59))
    {  suffixinfo.options.set(1,ti4.options.test(1));
       suffixinfo.options.set(2,ti4.options.test(2));
    }
  if (change.test(60))
    {  suffixinfo.options.set(3,ti4.options.test(3));
       suffixinfo.options.set(4,ti4.options.test(4));
    }
  if (change.test(61)) suffixinfo.vertspacing = ti4.vertspacing;
  if (change.test(62)) suffixinfo.options.set(5,ti4.options.test(5));
  if (change.test(63)) suffixinfo.options.set(6,ti4.options.test(6));
  if (change.test(64)) suffixinfo.options.set(7,ti4.options.test(7));
  if (change.test(65)) suffixinfo.options.set(8,ti4.options.test(8));

  if (change.test(70)) options.set(3,o.test(3));
  if (change.test(71))
    {RCCHAR *text;
       if ((text = new RCCHAR[strlen(ti3.text)+1]) != NULL)
         {  delete [] prefixinfo.text;  prefixinfo.text = text;  strcpy(text,ti3.text);
         }
    }

  if (change.test(75)) options.set(4,o.test(4));
  if (change.test(76))
    {RCCHAR *text;
       if ((text = new RCCHAR[strlen(ti4.text)+1]) != NULL)
         {  delete [] suffixinfo.text;  suffixinfo.text = text;  strcpy(text,ti4.text);
         }
    }
  if (change.test(77))
    {  options.set(7,o.test(7));
       options.set(8,o.test(8));
    }
  if (change.test(78)) options.set(9,o.test(9));
  if (change.test(79)) options.set(10,o.test(10));

  if (change.test(80))
    {RCCHAR *font;
       if ((font = new RCCHAR[strlen(ti2.font)+1]) != NULL)
         {  delete [] toleranceinfo.font;  toleranceinfo.font = font;  strcpy(font,ti2.font);
            toleranceinfo.options.set(0,ti2.options.test(0));
         }
    }
  if (change.test(81)) toleranceinfo.height = ti2.height;
  if (change.test(82)) toleranceinfo.width = ti2.width;
  if (change.test(83)) toleranceinfo.angle = ti2.angle;
  if (change.test(84))
    {  toleranceinfo.options.set(1,ti2.options.test(1));
       toleranceinfo.options.set(2,ti2.options.test(2));
    }
  if (change.test(85))
    {  toleranceinfo.options.set(3,ti2.options.test(3));
       toleranceinfo.options.set(4,ti2.options.test(4));
    }
  if (change.test(86)) toleranceinfo.vertspacing = ti2.vertspacing;
  if (change.test(87)) toleranceinfo.options.set(5,ti2.options.test(5));
  if (change.test(88)) toleranceinfo.options.set(6,ti2.options.test(6));
  if (change.test(89)) toleranceinfo.options.set(7,ti2.options.test(7));
  if (change.test(90)) toleranceinfo.options.set(8,ti2.options.test(8));

  if (change.test(91)) options.set(ToleranceDefined,o.test(ToleranceDefined));
  if (change.test(92)) options.set(GapFromDimension,o.test(GapFromDimension));
  if (change.test(93)) scale = sc;
  if (change.test(94)) options.set(ScaleText,o.test(ScaleText));
  if (change.test(95)) options.set(FixedDimensionPosition,o.test(FixedDimensionPosition));
  if (change.test(96)) prefixinfo.options.set(10,ti3.options.test(10));
  if (change.test(97)) prefixinfo.options.set(12,ti3.options.test(12));
  if (change.test(98)) prefixinfo.halocolour = ti3.halocolour;
  if (change.test(99)) textyoffset = tyo;

  if (change.test(100)) term1.style = (term1.style&0x7f) | (t1.style&0x80);
  if (change.test(101)) term1.weight = t1.weight;
  if (change.test(102)) term2.style = (term2.style&0x7f) | (t2.style&0x80);
  if (change.test(103)) term2.weight = t2.weight;
  if (change.test(104)) options.set(DimensionReverse,o.test(DimensionReverse));
  if (change.test(105)) options.set(HideLeftDimensionLine,o.test(HideLeftDimensionLine));
  if (change.test(106)) options.set(HideRightDimensionLine,o.test(HideRightDimensionLine));

  draw(DM_NORMAL);
}

/*
// this is declared in ncdialog.h
class DimensionDefaults
  {public:
     double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
     int precision,tolprecision;
     BitMask options;
     TerminatorInfo term1,term2;
     TextInfo vinfo,tinfo,pinfo,sinfo;
     DimensionDefaults() : options(32) {}
     void draw(RCHWND hWnd,RCHDC hDC,int all);
  };
*/

void DimensionDefaults::draw(RCHWND hWnd,RCHDC hDC,int all)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point p1,p2,p3,origin,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 SortedPointList list;
 output.setplotscale(1);

 if(hWnd == 0)
     return;
#if 1
  //QGraphicsView *ctl = 0;
  //if(dialog->gethdlg())
  // ctl = (QGraphicsView*)((Leader_Dialog*)dialog->gethdlg())->getControl(113);
  //QGraphicsScene *scene = 0;
  //if(ctl)
  //   scene = ctl->scene();

  // the hWnd is the QGraphicsView on the dialog to be used for drawing
  // get the graphics view and the scene from the widget
  QGraphicsScene *scene = 0;
  QGraphicsView *ctl = (QGraphicsView*)hWnd;

  if(ctl)
      scene = ctl->scene();

  //if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
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

       //oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
       //IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       //FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       //output.moveto(0,0);
       //output.lineto(rect.right-1,0);
       //output.lineto(rect.right-1,rect.bottom-1);
       //output.lineto(0,rect.bottom-1);
       //output.lineto(0,0);
       //SelectObject(output.gethdc(),oldhpen);
       //DeleteObject(hpen);

       if (all)
       {
            p1.x = -(output.getumax() - output.getumin()) * 0.2;
            p1.y = -(output.getvmax() - output.getvmin()) * 0.5;
            p1.z = 0.0;
            p2.x =  (output.getumax() - output.getumin()) * 0.2;
            p2.y = -(output.getvmax() - output.getvmin()) * 0.5;
            p2.z = 0.0;
            p3.x =  (output.getumax() - output.getumin()) * 0.6;
            p3.y = -(output.getvmax() - output.getvmin()) * 0.5;
            p3.z = 0.0;
            list.add(p1);  list.add(p2);  list.add(p3);
            origin.x = output.getumin() + (output.getumax() - output.getumin()) * 0.3;
            origin.y = output.getvmin() + (output.getvmax() - output.getvmin()) * 0.7;
            origin.z = 0.0;
            options.toggle(ToleranceDefined);
            options.toggle(ScaleText);
            //EntityHeader header(c,l,s,db.lineweights.mmtoweight(w));
            LinearDimension ld(origin,xaxis,yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
            options.toggle(ToleranceDefined);
            options.toggle(ScaleText);
            ld.draw(DM_NORMAL,NULL,&output);
       }
  }
#endif
}

/*
// this is defined in ncdialog.h
class DimensionDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     DimensionDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void DimensionDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int tolerance;
  int c,dc,l,dl,s,ds,diw;
  double w,dw;
  c=db.header.getcolour();
  l=(int)db.header.getlayer();
  s=(int)db.header.getstyle();
  w=db.lineweights.weighttomm(db.header.getweight());

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);
  int tristates[] = { 1,1,1,1,1,1,1,1,1 };

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
   ctl = (QGraphicsView*)((Dimension_Dialog*)dialog->gethdlg())->getControl(214);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (
          dialog->currentvalue(100,&c ) &&
          dialog->currentvalue(101,&l ) &&
          dialog->currentvalue(102,&s ) &&
          dialog->currentvalue(103,&w ) &&

          dialog->currentvalue(200,&dd->extension) &&
          dialog->currentvalue(201,&dd->gap) &&
          dialog->currentvalue(202,&dd->precision) &&
          dialog->currentvalue(203,&tristates[0]) &&
          dialog->currentvalue(204,&tristates[1]) &&
          dialog->currentvalue(205,&dd->stackdis) &&
          dialog->currentvalue(211,&tolerance) &&
          dialog->currentvalue(212,&dd->uppertol) &&
          dialog->currentvalue(213,&dd->lowertol) &&
          dialog->currentvalue(216,&tristates[2]) &&
          dialog->currentvalue(217,&tristates[3]) &&
          dialog->currentvalue(218,&dd->scale) &&
          dialog->currentvalue(222,&dd->textyoffset) &&
          dialog->currentvalue(220,&tristates[5]) &&
          dialog->currentvalue(224,&tristates[6]) &&
          dialog->currentvalue(225,&tristates[7]) &&
          dialog->currentvalue(226,&tristates[8]))
        {
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(200))->specialValueText().isEmpty() &&
             dd->extension == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(200))->minimum() )
              dd->extension = defextension;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(201))->specialValueText().isEmpty() &&
             dd->gap == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(201))->minimum() )
              dd->gap = defgap;
          if(!((QSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(202))->specialValueText().isEmpty() &&
             dd->precision == ((QSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(202))->minimum() )
              dd->precision = defprecision;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(205))->specialValueText().isEmpty() &&
             dd->stackdis == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(205))->minimum() )
              dd->stackdis = defstackdis;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(212))->specialValueText().isEmpty() &&
             dd->uppertol == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(212))->minimum() )
              dd->uppertol = defuppertol;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(213))->specialValueText().isEmpty() &&
             dd->lowertol == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(213))->minimum() )
              dd->lowertol = deflowertol;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(218))->specialValueText().isEmpty() &&
             dd->scale == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(218))->minimum() )
              dd->scale = defscale;
          if(!((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(222))->specialValueText().isEmpty() &&
             dd->textyoffset == ((QDoubleSpinBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(222))->minimum() )
              dd->textyoffset = deftextyoffset;

          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(203))->isTristate())
              tristates[0] == 1 ? dd->options.set(0,defOptions.test(0)) : dd->options.set(0,tristates[0]);
          else
              dd->options.set(0,tristates[0]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(204))->isTristate())
              tristates[1] == 1 ? dd->options.set(1,defOptions.test(1)) : dd->options.set(1,tristates[1]);
          else
              dd->options.set(1,tristates[1]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(216))->isTristate())
              tristates[2] == 1 ? dd->options.set(ToleranceDefined,defOptions.test(ToleranceDefined)) : dd->options.set(ToleranceDefined,tristates[2]);
          else
              dd->options.set(ToleranceDefined,tristates[2]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(217))->isTristate())
              tristates[3] == 1 ? dd->options.set(GapFromDimension,defOptions.test(GapFromDimension)) : dd->options.set(GapFromDimension,tristates[3]);
          else
              dd->options.set(GapFromDimension,tristates[3]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(220))->isTristate())
              tristates[5] == 1 ? dd->options.set(FixedDimensionPosition,defOptions.test(FixedDimensionPosition)) : dd->options.set(FixedDimensionPosition,tristates[5]);
          else
              dd->options.set(FixedDimensionPosition,tristates[5]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(224))->isTristate())
              tristates[6] == 1 ? dd->options.set(DimensionReverse,defOptions.test(DimensionReverse)) : dd->options.set(DimensionReverse,tristates[6]);
          else
              dd->options.set(DimensionReverse,tristates[6]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(225))->isTristate())
              tristates[7] == 1 ? dd->options.set(HideLeftDimensionLine,defOptions.test(HideLeftDimensionLine)) : dd->options.set(HideLeftDimensionLine,tristates[7]);
          else
              dd->options.set(HideLeftDimensionLine,tristates[7]);
          if(((QCheckBox*)((Dimension_Dialog*)dialog->gethdlg())->getControl(226))->isTristate())
              tristates[8] == 1 ? dd->options.set(HideRightDimensionLine,defOptions.test(HideRightDimensionLine)) : dd->options.set(HideRightDimensionLine,tristates[8]);
          else
              dd->options.set(HideRightDimensionLine,tristates[8]);


           dd->options.set(5,tolerance % 2);
           dd->options.set(6,tolerance / 2);

           // get the current defaults
           dc=db.header.getcolour();
           dl=(int)db.header.getlayer();
           ds=(int)db.header.getstyle();
           diw=db.header.getweight();
           // set the dims defaults
           db.header.setcolour(c);
           db.header.setlayer(l);
           db.header.setstyle(s);
           db.header.setweight(db.lineweights.mmtoweight(w));

           dd->draw(ctl,hDC,1);
           //dd->draw(hWnd,hDC,1);

           // reset the defaults again
           db.header.setcolour(dc);
           db.header.setlayer(dl);
           db.header.setstyle(ds);
           db.header.setweight(diw);
      }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;
  }
}

/*
// this is declared in ncdialog.h
class NTerminatorDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     NTerminatorDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void NTerminatorDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int style,position,visible,useweight;
  double weight;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(105);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (dialog->currentvalue(100,&style) &&
          dialog->currentvalue(101,&dd->term1.length) &&
          dialog->currentvalue(102,&dd->term1.height) &&
          dialog->currentvalue(103,&position) &&
          dialog->currentvalue(104,&visible) &&
          dialog->currentvalue(106,&weight) &&
          dialog->currentvalue(107,&useweight))
      {

          if(!((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->term1.length == ((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(101))->minimum())
              dd->term1.length = defterm1.length;

          if(!((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->term1.height == ((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->term1.height = defterm1.height;

          if(!((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
             /*dd->term1.weight*/weight == db.lineweights.mmtoweight(((QDoubleSpinBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(106))->minimum()))
              dd->term1.weight = defterm1.weight;
          else
              dd->term1.weight = db.lineweights.mmtoweight(weight);

          if(((QComboBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(203))->isVisible())
             dd->options.set(9,defOptions.test(9));
          else
              dd->options.set(9,position);

          if(((QComboBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(204))->isVisible())
             dd->options.set(10,defOptions.test(10));
          else
              dd->options.set(10,visible);


          if(((QComboBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(200))->isVisible())
             style = (defterm1.style&0x7F) ;

          if(((QCheckBox*)((DimensionTerminator_Dialog*)dialog->gethdlg())->getControl(107))->isTristate())
               useweight == 2 ? useweight = 1 : useweight = 0;
           else
           {
               if(useweight != 0)// might be 2
                   useweight = 1;
               else
                   useweight = 0;
           }

           dd->term1.style = Byte(style) | (0x80 * useweight);

           //dd->options.set(9,position);
           //dd->options.set(10,visible);
           //dd->term1.weight = db.lineweights.mmtoweight(weight);
           //dd->draw(hWnd,hDC,1);
           dd->draw(ctl,hDC,1);
      }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;

  }
}

/*
// this is declared in ncdialog.h
class CTerminatorDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     CTerminatorDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void CTerminatorDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int style,useweight;
  double weight;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(103);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (dialog->currentvalue(100,&style) &&
          dialog->currentvalue(101,&dd->term2.length) &&
          dialog->currentvalue(102,&dd->term2.height) &&
          dialog->currentvalue(104,&weight) &&
          dialog->currentvalue(105,&useweight))
        {
          if(!((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->term2.length == ((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(101))->minimum())
              dd->term2.length = defterm2.length;

          if(!((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->term2.height == ((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->term2.height = defterm2.height;

          if(!((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(104))->specialValueText().isEmpty() &&
             /*dd->term2.weight*/weight == db.lineweights.mmtoweight(((QDoubleSpinBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(104))->minimum()))
              dd->term2.weight = defterm2.weight;
          else
              dd->term2.weight = db.lineweights.mmtoweight(weight);

          if(((QComboBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(200))->isVisible())
             style = (defterm2.style&0x7F) ;

          if(((QCheckBox*)((DimensionCTerminator_Dialog*)dialog->gethdlg())->getControl(105))->isTristate())
               useweight == 2 ? useweight = 1 : useweight = 0;
           else
           {
               if(useweight != 0)// might be 2
                   useweight = 1;
               else
                   useweight = 0;
           }


           dd->term2.style = Byte(style) | (0x80 * useweight);

           //dd->term2.weight = db.lineweights.mmtoweight(weight);
           dd->draw(ctl,hDC,1);
           //dd->draw(hWnd,hDC,1);
        }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;

  }
}

class DTButtonDialogControl : public ButtonDialogControl
  {public:
     DTButtonDialogControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };

/*
class WeightAssocRealDialogControl : public RealDialogControl
  {public:
     WeightAssocRealDialogControl(int id,double *v) : RealDialogControl(id,v) {}
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
  };
*/

int DTButtonDialogControl::select(Dialog *dialog)
{
 /*
 double weight;
 int state;

  state = 1000;
  weight_command(&state,0,(void **)&weight);   
  ((WeightAssocRealDialogControl *)dialog->getcontrol(id-20))->change(dialog,weight);
  if (id == 124)
    InvalidateRect(GetDlgItem(dialog->gethdlg(),103),0,TRUE);
  else if (id == 126)
    InvalidateRect(GetDlgItem(dialog->gethdlg(),105),0,TRUE);
  else if (id == 140)
    InvalidateRect(GetDlgItem(dialog->gethdlg(),113),0,TRUE);

  return 0;
  */
  int state,colour,halocolour,layer,style,iweight;
  double weight,termweight;

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
  else if(id == 1004) // for the leader dialog
  { // halo colour
      WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
      ((IntegerDialogControl *)dialog->getcontrol(125))->currentvalue(dialog,&halocolour);
      general_property_command(&state,0,(void **)&halocolour);
      ((IntegerDialogControl *)dialog->getcontrol(125))->change(dialog,halocolour);
  }
  else if(id == 1005) //  for the leader dialog
  { // terminator weight
      WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
      ((WeightAssocRealDialogControl *)dialog->getcontrol(124))->currentvalue(dialog,&termweight);
      general_property_command(&state,0,(void **)&termweight);
      ((WeightAssocRealDialogControl *)dialog->getcontrol(124))->change(dialog,termweight);
  }
  else if(id == 1006) //  for the dimension terminator subdialog1
  { // terminator weight
      WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
      ((WeightAssocRealDialogControl *)dialog->getcontrol(106))->currentvalue(dialog,&termweight);
      general_property_command(&state,0,(void **)&termweight);
      ((WeightAssocRealDialogControl *)dialog->getcontrol(106))->change(dialog,termweight);
  }
  else if(id == 1007) //  for the dimension Cterminator subdialog2
  { // terminator weight
      WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
      ((WeightAssocRealDialogControl *)dialog->getcontrol(104))->currentvalue(dialog,&termweight);
      general_property_command(&state,0,(void **)&termweight);
      ((WeightAssocRealDialogControl *)dialog->getcontrol(104))->change(dialog,termweight);
  }
  return 0;
}

/*
// this is declared in ncdialog.h
class VTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     VTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void VTextDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int hjust,vjust,placement,fontid;
  int bold,underline,italic,boxed,display;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(112);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (dialog->currentvalue(100,&fontid) &&
          dialog->currentvalue(101,&dd->vinfo.height) &&
          dialog->currentvalue(102,&dd->vinfo.width) &&
          dialog->currentvalue(103,&dd->vinfo.angle) &&
          dialog->currentvalue(104,&hjust) &&
          dialog->currentvalue(105,&vjust) &&
          dialog->currentvalue(106,&dd->vinfo.vertspacing) &&
          dialog->currentvalue(107,&bold) &&
          dialog->currentvalue(108,&underline) &&
          dialog->currentvalue(109,&italic) &&
          dialog->currentvalue(110,&boxed) &&
          dialog->currentvalue(111,&display) &&
          //dialog->currentvalue(107,&dd->vinfo.options) &&
          //dialog->currentvalue(108,&dd->vinfo.options) &&
          //dialog->currentvalue(109,&dd->vinfo.options) &&
          //dialog->currentvalue(110,&dd->vinfo.options) &&
          //dialog->currentvalue(111,&dd->options) &&
          dialog->currentvalue(113,&placement))
        {
          if( strlen(((FontListDialogControl*)dialog->getcontrol(100))->getSpecialValueText()) != 0 &&
             ((QComboBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(1000))->isVisible() )
              dd->vinfo.font = defvinfo.font;
          else
              dd->vinfo.font = fonts[fontid];

          if(!((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->vinfo.height == ((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(101))->minimum() )
              dd->vinfo.height = defvinfo.height;

          if(!((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->vinfo.width == ((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->vinfo.width = defvinfo.width;

          if(!((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(103))->specialValueText().isEmpty() &&
             dd->vinfo.angle == ((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(103))->minimum() )
              dd->vinfo.angle = defvinfo.angle;

          if(!((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
             dd->vinfo.vertspacing == ((QDoubleSpinBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
              dd->vinfo.vertspacing = defvinfo.vertspacing;

          if(((QCheckBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(107))->isTristate())
              bold == 1 ? dd->vinfo.options.set(5,defvinfo.options.test(5)) : dd->vinfo.options.set(5,bold);
          else
              dd->vinfo.options.set(5,bold);

          if(((QCheckBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(108))->isTristate())
              underline == 1 ? dd->vinfo.options.set(6,defvinfo.options.test(6)) : dd->vinfo.options.set(6,underline);
          else
              dd->vinfo.options.set(6,underline);

          if(((QCheckBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(109))->isTristate())
              italic == 1 ? dd->vinfo.options.set(7,defvinfo.options.test(7)) : dd->vinfo.options.set(7,italic);
          else
              dd->vinfo.options.set(7,italic);

          if(((QCheckBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(110))->isTristate())
              boxed == 1 ? dd->vinfo.options.set(8,defvinfo.options.test(8)) : dd->vinfo.options.set(8,boxed);
          else
              dd->vinfo.options.set(8,boxed);

          if(((QCheckBox*)((DimensionValue_Dialog*)dialog->gethdlg())->getControl(111))->isTristate())
              display == 1 ? dd->options.set(2,defOptions.test(2)) : dd->options.set(2,display);
          else
              dd->options.set(2,display);

           //dd->vinfo.options.set(5,bold);
           //dd->vinfo.options.set(6,underline);
           //dd->vinfo.options.set(7,italic);
           //dd->vinfo.options.set(8,boxed);
           //dd->options.set(2,(display!=0?1:0));

           //dd->vinfo.font = fonts[fontid];
           //dd->vinfo.options.set(0,charsets[fontid] == SYMBOL_CHARSET);
           dd->vinfo.options.set(1,hjust % 2);
           dd->vinfo.options.set(2,hjust / 2);
           dd->vinfo.options.set(3,vjust % 2);
           dd->vinfo.options.set(4,vjust / 2);
           dd->options.set(7,placement % 2);
           dd->options.set(8,placement / 2);
           dd->draw(ctl,hDC,1);
           //dd->draw(hWnd,hDC,1);
        }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;

  }
}

/*
// this is declared in ncdialog.h
class PTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     PTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void PTextDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int hjust,vjust,fontid;
  int bold,underline,italic,boxed;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);
  RCCHAR text[512];

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(111);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (dialog->currentvalue(100,&fontid) &&
          dialog->currentvalue(101,&dd->pinfo.height) &&
          dialog->currentvalue(102,&dd->pinfo.width) &&
          dialog->currentvalue(103,&dd->pinfo.angle) &&
          dialog->currentvalue(104,&hjust) &&
          dialog->currentvalue(105,&vjust) &&
          dialog->currentvalue(106,&dd->pinfo.vertspacing) &&
          dialog->currentvalue(107,&bold) &&
          dialog->currentvalue(108,&underline) &&
          dialog->currentvalue(109,&italic) &&
          dialog->currentvalue(110,&boxed))
          //dialog->currentvalue(107,&dd->pinfo.options) &&
          //dialog->currentvalue(108,&dd->pinfo.options) &&
          //dialog->currentvalue(109,&dd->pinfo.options) &&
          //dialog->currentvalue(110,&dd->pinfo.options))
        {

          if( strlen(((FontListDialogControl*)dialog->getcontrol(100))->getSpecialValueText()) != 0 &&
             ((QComboBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(1000))->isVisible() )
              dd->pinfo.font = defpinfo.font;
          else
              dd->pinfo.font = fonts[fontid];

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->pinfo.height == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->minimum() )
              dd->pinfo.height = defpinfo.height;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->pinfo.width == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->pinfo.width = defpinfo.width;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->specialValueText().isEmpty() &&
             dd->pinfo.angle == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->minimum() )
              dd->pinfo.angle = defpinfo.angle;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
             dd->pinfo.vertspacing == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
              dd->pinfo.vertspacing = defpinfo.vertspacing;

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(107))->isTristate())
              bold == 1 ? dd->pinfo.options.set(5,defpinfo.options.test(5)) : dd->pinfo.options.set(5,bold);
          else
              dd->pinfo.options.set(5,bold);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(108))->isTristate())
              underline == 1 ? dd->pinfo.options.set(6,defpinfo.options.test(6)) : dd->pinfo.options.set(6,underline);
          else
              dd->pinfo.options.set(6,underline);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(109))->isTristate())
              italic == 1 ? dd->pinfo.options.set(7,defpinfo.options.test(7)) : dd->pinfo.options.set(7,italic);
          else
              dd->pinfo.options.set(7,italic);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(110))->isTristate())
              boxed == 1 ? dd->pinfo.options.set(8,defpinfo.options.test(8)) : dd->pinfo.options.set(8,boxed);
          else
              dd->pinfo.options.set(8,boxed);

           //dd->pinfo.font = fonts[fontid];
           //dd->pinfo.options.set(0,charsets[fontid] == SYMBOL_CHARSET);
           dd->pinfo.options.set(1,hjust % 2);
           dd->pinfo.options.set(2,hjust / 2);
           dd->pinfo.options.set(3,vjust % 2);
           dd->pinfo.options.set(4,vjust / 2);
           dd->draw(ctl,hDC,1);
           //dd->draw(hWnd,hDC,1);
        }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;

  }
}

/*
// this is declared in ncdialog.h
class TTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     TTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void TTextDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int hjust,vjust,fontid;
  int bold,underline,italic,boxed,display;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(111);
    //ctl = (QGraphicsView*)((Dimension_Dialog*)dialog->gethdlg())->getControl(111);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

      if (dialog->currentvalue(100,&fontid) &&
          dialog->currentvalue(101,&dd->tinfo.height) &&
          dialog->currentvalue(102,&dd->tinfo.width) &&
          dialog->currentvalue(103,&dd->tinfo.angle) &&
          dialog->currentvalue(104,&hjust) &&
          dialog->currentvalue(105,&vjust) &&
          dialog->currentvalue(106,&dd->tinfo.vertspacing) &&
          dialog->currentvalue(107,&bold) &&
          dialog->currentvalue(108,&underline) &&
          dialog->currentvalue(109,&italic) &&
          dialog->currentvalue(110,&boxed))
          //dialog->currentvalue(107,&dd->tinfo.options) &&
          //dialog->currentvalue(108,&dd->tinfo.options) &&
          //dialog->currentvalue(109,&dd->tinfo.options) &&
          //dialog->currentvalue(110,&dd->tinfo.options))
        {
          if( strlen(((FontListDialogControl*)dialog->getcontrol(100))->getSpecialValueText()) != 0 &&
             ((QComboBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(1000))->isVisible() )
              dd->tinfo.font = deftinfo.font;
          else
              dd->tinfo.font = fonts[fontid];

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->tinfo.height == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->minimum() )
              dd->tinfo.height = deftinfo.height;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->tinfo.width == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->tinfo.width = deftinfo.width;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->specialValueText().isEmpty() &&
             dd->tinfo.angle == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->minimum() )
              dd->tinfo.angle = deftinfo.angle;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
             dd->tinfo.vertspacing == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
              dd->tinfo.vertspacing = deftinfo.vertspacing;

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(107))->isTristate())
              bold == 1 ? dd->tinfo.options.set(5,deftinfo.options.test(5)) : dd->tinfo.options.set(5,bold);
          else
              dd->tinfo.options.set(5,bold);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(108))->isTristate())
              underline == 1 ? dd->tinfo.options.set(6,deftinfo.options.test(6)) : dd->tinfo.options.set(6,underline);
          else
              dd->tinfo.options.set(6,underline);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(109))->isTristate())
              italic == 1 ? dd->tinfo.options.set(7,deftinfo.options.test(7)) : dd->tinfo.options.set(7,italic);
          else
              dd->tinfo.options.set(7,italic);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(110))->isTristate())
              boxed == 1 ? dd->tinfo.options.set(8,deftinfo.options.test(8)) : dd->tinfo.options.set(8,boxed);
          else
              dd->tinfo.options.set(8,boxed);

           //dd->tinfo.options.set(5,bold);
           //dd->tinfo.options.set(6,underline);
           //dd->tinfo.options.set(7,italic);
           //dd->tinfo.options.set(8,boxed);

           //dd->tinfo.font = fonts[fontid];
           //dd->tinfo.options.set(0,charsets[fontid] == SYMBOL_CHARSET);
           dd->tinfo.options.set(1,hjust % 2);
           dd->tinfo.options.set(2,hjust / 2);
           dd->tinfo.options.set(3,vjust % 2);
           dd->tinfo.options.set(4,vjust / 2);
           dd->draw(ctl,hDC,1);

           //dd->draw(hWnd,hDC,1);
        }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);

      delete [] defvinfo.text;  delete [] defvinfo.font;
      delete [] defpinfo.text;  delete [] defpinfo.font;
      delete [] defsinfo.text;  delete [] defsinfo.font;
      delete [] deftinfo.text;  delete [] deftinfo.font;

  }
}

/*
// this is declared in ncdialog.h
class STextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     STextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void STextDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int hjust,vjust,fontid;
  int bold,underline,italic,boxed;

  double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
  int defprecision,deftolprecision;
  TerminatorInfo defterm1,defterm2;
  TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
  BitMask defOptions(32);
  RCCHAR text[512];

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(111);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      RCCHAR *cs;
      LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return ;
      RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return ;
      RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
      if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return ;

      if (dialog->currentvalue(100,&fontid) &&
          dialog->currentvalue(101,&dd->sinfo.height) &&
          dialog->currentvalue(102,&dd->sinfo.width) &&
          dialog->currentvalue(103,&dd->sinfo.angle) &&
          dialog->currentvalue(104,&hjust) &&
          dialog->currentvalue(105,&vjust) &&
          dialog->currentvalue(106,&dd->sinfo.vertspacing) &&
          dialog->currentvalue(107,&bold) &&
          dialog->currentvalue(108,&underline) &&
          dialog->currentvalue(109,&italic) &&
          dialog->currentvalue(110,&boxed))
          //dialog->currentvalue(107,&dd->sinfo.options) &&
          //dialog->currentvalue(108,&dd->sinfo.options) &&
          //dialog->currentvalue(109,&dd->sinfo.options) &&
          //dialog->currentvalue(110,&dd->sinfo.options))
        {
          if( strlen(((FontListDialogControl*)dialog->getcontrol(100))->getSpecialValueText()) != 0 &&
             ((QComboBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(1000))->isVisible() )
              dd->sinfo.font = defsinfo.font;
          else
              dd->sinfo.font = fonts[fontid];

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->specialValueText().isEmpty() &&
             dd->sinfo.height == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(101))->minimum() )
              dd->sinfo.height = defsinfo.height;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->specialValueText().isEmpty() &&
             dd->sinfo.width == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(102))->minimum() )
              dd->sinfo.width = defsinfo.width;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->specialValueText().isEmpty() &&
             dd->sinfo.angle == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(103))->minimum() )
              dd->sinfo.angle = defsinfo.angle;

          if(!((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
             dd->sinfo.vertspacing == ((QDoubleSpinBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
              dd->sinfo.vertspacing = defsinfo.vertspacing;

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(107))->isTristate())
              bold == 1 ? dd->sinfo.options.set(5,defsinfo.options.test(5)) : dd->sinfo.options.set(5,bold);
          else
              dd->sinfo.options.set(5,bold);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(108))->isTristate())
              underline == 1 ? dd->sinfo.options.set(6,defsinfo.options.test(6)) : dd->sinfo.options.set(6,underline);
          else
              dd->sinfo.options.set(6,underline);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(109))->isTristate())
              italic == 1 ? dd->sinfo.options.set(7,defsinfo.options.test(7)) : dd->sinfo.options.set(7,italic);
          else
              dd->sinfo.options.set(7,italic);

          if(((QCheckBox*)((DimensionPrefix_Dialog*)dialog->gethdlg())->getControl(110))->isTristate())
              boxed == 1 ? dd->sinfo.options.set(8,defsinfo.options.test(8)) : dd->sinfo.options.set(8,boxed);
          else
              dd->sinfo.options.set(8,boxed);

           //dd->sinfo.font = fonts[fontid];
           //dd->sinfo.options.set(0,charsets[fontid] == SYMBOL_CHARSET);
           dd->sinfo.options.set(1,hjust % 2);
           dd->sinfo.options.set(2,hjust / 2);
           dd->sinfo.options.set(3,vjust % 2);
           dd->sinfo.options.set(4,vjust / 2);
           dd->draw(ctl,hDC,1);
           //dd->draw(hWnd,hDC,1);
        }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);
  }
}

/*
// this is declared in ncdialog.h
class PEntryDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     PEntryDisplayDialogControl(int id,DimensionDefaults *dd1) :
              DisplayDialogControl(id) {  dd = dd1;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void PEntryDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
  int useValueTextSettings=0;
  RCCHAR text[512];

  if(hWnd == 0) return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
     ctl = (QGraphicsView*)((DimensionPrefix0_Dialog*)dialog->gethdlg())->getControl(103);
  QGraphicsScene *scene = 0;
  if(ctl)
     scene = ctl->scene();

  if (ctl && scene)
  {
      if (dialog->currentvalue(100,&useValueTextSettings) &&
          dialog->currentvalue(102,dd->pinfo.text,511))
        {
          dd->options.set(3,useValueTextSettings);
        dd->draw(ctl,hDC,1);
        //dd->draw(hWnd,hDC,1);
      }
      else
        dd->draw(ctl,hDC,0);
        //dd->draw(hWnd,hDC,0);
  }
}

/*
// this is declared in ncdialog.h
class SEntryDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     SEntryDisplayDialogControl(int id,DimensionDefaults *dd1) :
              DisplayDialogControl(id) {  dd = dd1;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void SEntryDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
    int useValueTextSettings=0;
    RCCHAR text[512];

    if(hWnd == 0) return;

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
       ctl = (QGraphicsView*)((DimensionPrefix0_Dialog*)dialog->gethdlg())->getControl(103);
    QGraphicsScene *scene = 0;
    if(ctl)
       scene = ctl->scene();

    if (ctl && scene)
    {
        if (dialog->currentvalue(100,&useValueTextSettings) &&
          dialog->currentvalue(102,dd->sinfo.text,511))
        {
            dd->options.set(4,useValueTextSettings);
            dd->draw(ctl,hDC,1);
            //dd->draw(hWnd,hDC,1);
        }
        else
            dd->draw(ctl,hDC,0);
            //dd->draw(hWnd,hDC,0);
    }
}

class SubDialogButtonDialogControl : public ButtonDialogControl
{private:
   Dialog *dialog;
   DimensionDefaults *dd;
   double *weight1,*weight2;
 public:
   SubDialogButtonDialogControl(int i,Dialog *d,DimensionDefaults *dd1,double *w1,double *w2) :
       ButtonDialogControl(i) {  dialog = d;  dd = dd1;  weight1 = w1;  weight2 = w2;  }
   int select(Dialog *);
};

int SubDialogButtonDialogControl::select(Dialog *parentdialog)
{DimensionDefaults olddd;
 char prefix[512],suffix[512];
  olddd = *dd;
  strcpy(prefix,dd->pinfo.text);
  strcpy(suffix,dd->sinfo.text);
  if (dialog->process(parentdialog->gethdlg()) == FALSE)
    {  *dd = olddd;
       strcpy(dd->pinfo.text,prefix);
       strcpy(dd->sinfo.text,suffix);
       dd->term1.weight = db.lineweights.mmtoweight(*weight1);
       dd->term2.weight = db.lineweights.mmtoweight(*weight2);
       parentdialog->updatecustomcontrols();
    }
  return 0;
}

int Dimension::setup(BitMask *change,double *extension,double *gap,double *stackdis,double *uppertol,double *lowertol,double *scale,double *textyoffset,int *precision,int *tolprecision,TerminatorInfo *term1,TerminatorInfo *term2,
                     TextInfo *ti1,TextInfo *ti2,TextInfo *ti3,TextInfo *ti4,BitMask *options, EntityHeader *header)
{Dialog dialog("Dimension_Dialog");
 Dialog subdialog1("DimensionTerminator_Dialog");
 Dialog subdialog2("DimensionCTerminator_Dialog");
 Dialog textdialog1("DimensionValue_Dialog");
 Dialog textdialog2("DimensionPrefix_Dialog"); // tolerance dialog
 Dialog textdialog3("DimensionPrefix_Dialog"); // prefix dialog
 Dialog textdialog4("DimensionPrefix_Dialog"); // suffix dialog
 Dialog entrydialog1("DimensionPrefix0_Dialog");
 Dialog entrydialog2("DimensionSuffix0_Dialog");
 DialogControl *dc=0;
 int i,style1,dummystyle1,dummystyle2,style2,hjust1,vjust1,hjust2,vjust2,hjust3,vjust3,hjust4,vjust4,tolerance,status,nterms,nfonts,fontid1,fontid2,fontid3,fontid4,placement,position,visible;
 int useweight1,useweight2,dummytolerance,dummyposition,dummyvisible;
 double weight1,weight2;
 Byte *charsets=0;
 RCCHAR **termnames=0,**fonts=0,prefix[512],suffix[512],*text=0,*font=0;
 RCCHAR **dummytermnames=0;
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
 ResourceString rs13(NCDIMENA+13),rs14(NCDIMENA+14),rs15(NCDIMENA+15);
 ResourceString rs16(NCDIMENA+16);
 ResourceString rs40(NCDIMENA+40),rs41(NCDIMENA+41),rs42(NCDIMENA+42);
 ResourceString rs43(NCDIMENA+43),rs44(NCDIMENA+44),rs45(NCDIMENA+45),rs46(NCDIMENA+46);
 ResourceString rsVaries(4107),rsDefault(4108),rsOther(4109);
 RCCHAR *hjustl[]  = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *hjustlv[] = {  rsVaries.getws(),rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[]  = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 RCCHAR *vjustlv[] = {  rsVaries.getws(),rs10.getws(),rs11.getws(),rs12.getws()  };
 RCCHAR *tolerancel[] = {  rs13.getws(),rs14.getws(),rs15.getws(),rs16.getws()  };
 RCCHAR *tolerancelv[] = {  rsVaries.getws(),rs13.getws(),rs14.getws(),rs15.getws(),rs16.getws()  };
 RCCHAR *placementl[] = {  rs40.getws(),rs41.getws(),rs42.getws()  };
 RCCHAR *placementlv[] = {  rsVaries.getws(),rs40.getws(),rs41.getws(),rs42.getws()  };
 RCCHAR *positionl[] = {  rs43.getws(),rs44.getws()  };
 RCCHAR *positionlv[] = {  rsVaries.getws(),rs43.getws(),rs44.getws()  };
 RCCHAR *visiblel[] = {  rs45.getws(),rs46.getws()  };
 RCCHAR *visiblelv[] = {  rsVaries.getws(),rs45.getws(),rs46.getws()  };
 DimensionDefaults dd;
 TerminatorInfo terminfo;
 int c,l,st;
 double w;
 //
 int dummyHjust1=0,dummyVjust1=0;
 int dummyHjust2=0,dummyVjust2=0;
 int dummyHjust3=0,dummyVjust3=0;
 int dummyHjust4=0,dummyVjust4=0;
 RCCHAR *dummyfonts[] = { rsVaries.getws(),rsDefault.getws(),rsOther.getws() };
 int dummyid1=0,dummyid2=0,dummyid3=0,dummyid4=0,dummyStyle1=0,dummyPlacement=0;
 RCCHAR defFont[300];
 int defColour,defLayer,defStyle;
 double defWeight;
 double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale,deftextyoffset;
 int defprecision,deftolprecision;
 TerminatorInfo defterm1,defterm2;
 TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
 BitMask defOptions(32),changeIn(120);
 int tristates[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };

 strcpy(prefix,"");
 strcpy(suffix,"");

 EntityHeader entheader;
 if(header != 0)
     header->verify(&dialog,&c,&l,&st,&w);
 else
     entheader.verify(&dialog,&c,&l,&st,&w);

 int changecount = v.getinteger("op::changecount");

 // get the default values
 defColour = db.header.getcolour();
 defLayer  = db.header.getlayer();
 defStyle  = db.header.getstyle();
 defWeight = db.lineweights.weighttomm(db.header.getweight());
 RCCHAR *cs;
 LinearDimension::getdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&deftextyoffset,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
 if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;
 //
 dd.usePrefix1=0;
 dd.useSuffix1=0;
 dd.extension = *extension;  dd.gap = *gap;  dd.stackdis = *stackdis;  dd.scale = *scale; dd.textyoffset = *textyoffset;
 dd.uppertol = *uppertol;  dd.lowertol = *lowertol;
 dd.precision = *precision;  dd.tolprecision = *tolprecision;
 dd.term1 = *term1;  dd.term2 = *term2;
 dd.vinfo = *ti1;  dd.tinfo = *ti2;  dd.pinfo = *ti3;  dd.sinfo = *ti4;  dd.options = *options;
 dd.vinfo.options = ti1->options;  dd.tinfo.options = ti2->options; dd.pinfo.options = ti3->options;  dd.sinfo.options = ti4->options;
 tolerance = options->test(5) + options->test(6) * 2;
 dummytolerance = tolerance;
 options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
 options->toggle(ScaleText);         //  Negative logic for scale text - Default is scaled

 if(change != NULL)
 {
     // if test is true the values dont vary ie. the dialog will show the incoming values
     // otherwise false ie. the values vary so the dialog will show the *Varies* message
     // the entity values will not be modified if the dialog values are not changed
     //
     // here we are setting the controls to there default settings
     // text boxes and combo boxes will show *Varies*
     // check boxes will be in tri state mode
     if(!change->test(0)) // colour
         c = 0;
     if(!change->test(1)) // layer
         l = 0;
     if(!change->test(2)) // style
         st = 0;
     if(!change->test(3)) // weight
         w = 0.10;

     // dimension dialog
     if(!change->test(5)) // extension
         *extension = -100000000.0;
     if(!change->test(6)) // gap
         *gap = -100000000.0;
     if(!change->test(7)) // precision
         precision = 0;
     if(!change->test(10)) // stackdis
         *stackdis = -100000000.;
     if(!change->test(12)) // uppertol
         *uppertol = -100000000.0;
     if(!change->test(13)) // lowertol
         *lowertol = -100000000.0;
     if(!change->test(93)) // value scale
         *scale = 0.0;
     if(!change->test(99)) // text yoffset
         *textyoffset = -100000000.0;
     if(!change->test(11)) // tolerance
     {
         tolerance = 0;
         dummytolerance = 0;
     }
     else
     {
         tolerance = options->test(5) + options->test(6) * 2;
         dummytolerance = 0;
     }

     if(change->test(8)) // option 0 common point dimension
         options->test(0)==0  ? tristates[0] = 0 : tristates[0] = 2;
     if(change->test(9)) // option 1 stacked dimension
         options->test(1)==0  ? tristates[1] = 0 : tristates[1] = 2;
     if(change->test(91)) // option 11 tolerance defined??
         options->test(11)==0  ? tristates[2] = 0 : tristates[2] = 2;
     if(change->test(92)) // option 12 gap from dimension line
         options->test(12)==0  ? tristates[3] = 0 : tristates[3] = 2;
     if(change->test(94)) // option 14 scale text
         options->test(14)==0  ? tristates[4] = 0 : tristates[4] = 2;
     if(change->test(95)) // option 17 fixed dimension position
         options->test(17)==0  ? tristates[5] = 0 : tristates[5] = 2;
     if(change->test(104)) // option 21 dimension reversed
         options->test(21)==0  ? tristates[6] = 0 : tristates[6] = 2;
     if(change->test(105)) // option 22 hide left dimension line
         options->test(22)==0  ? tristates[7] = 0 : tristates[7] = 2;
     if(change->test(106)) // option 23 hide right dimension line
         options->test(23)==0  ? tristates[8] = 0 : tristates[8] = 2;

 }
 else
 {
    options->test(0)==0  ? tristates[0] = 0 : tristates[0] = 1;
    options->test(1)==0  ? tristates[1] = 0 : tristates[1] = 1;
    options->test(ToleranceDefined)==0  ? tristates[2] = 0 : tristates[2] = 1;
    options->test(GapFromDimension)==0  ? tristates[3] = 0 : tristates[3] = 1;
    options->test(ScaleText)==0  ? tristates[4] = 0 : tristates[4] = 1;
    options->test(FixedDimensionPosition)==0  ? tristates[5] = 0 : tristates[5] = 1;
    options->test(DimensionReverse)==0  ? tristates[6] = 0 : tristates[6] = 1;
    options->test(HideLeftDimensionLine)==0  ? tristates[7] = 0 : tristates[7] = 1;
    options->test(HideRightDimensionLine)==0  ? tristates[8] = 0 : tristates[8] = 1;
 }

 dialog.add(dc = new RealDialogControl(200,extension,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 dialog.add(dc = new RealDialogControl(201,gap,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(dc = new IntegerDialogControl(202,precision));
 //dialog.add(new ScrollBarDialogControl(1102,dc));
 //dialog.add(new CheckBoxDialogControl(203,options,0));// option 0 common point dimension
 //dialog.add(new CheckBoxDialogControl(204,options,1));// option 1 stacked dimension
 dialog.add(dc = new RealDialogControl(205,stackdis,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1105,dc));
 dialog.add(dc = new RealDialogControl(222,textyoffset,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1122,dc));
 dialog.add((DisplayDialogControl *) new DimensionDisplayDialogControl(214,&dd));
 dialog.add(new SubDialogButtonDialogControl(206,&subdialog1,&dd,&weight1,&weight2));
 dialog.add(new SubDialogButtonDialogControl(207,&textdialog1,&dd,&weight1,&weight2));
 dialog.add(new SubDialogButtonDialogControl(208,&entrydialog1,&dd,&weight1,&weight2));
 dialog.add(new SubDialogButtonDialogControl(209,&entrydialog2,&dd,&weight1,&weight2));
 dialog.add(new SubDialogButtonDialogControl(210,&subdialog2,&dd,&weight1,&weight2));
 dialog.add(new ListDialogControl(211,4,tolerancel,&tolerance));
 dialog.add(new ListDialogControl(2011,5,tolerancelv,&dummytolerance));
 dialog.add(dc = new RealDialogControl(212,uppertol,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1112,dc));
 dialog.add(dc = new RealDialogControl(213,lowertol,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1113,dc));
 dialog.add(new SubDialogButtonDialogControl(215,&textdialog2,&dd,&weight1,&weight2));
 //dialog.add(new CheckBoxDialogControl(216,options,ToleranceDefined));
 //dialog.add(new CheckBoxDialogControl(217,options,GapFromDimension));
 dialog.add(dc = new RealDialogControl(218,scale,1.0,0.0,100000.0));
 //dialog.add(new ScrollBarDialogControl(1118,dc));
 //dialog.add(new CheckBoxDialogControl(219,options,ScaleText));
 //dialog.add(new CheckBoxDialogControl(220,options,FixedDimensionPosition));
 dialog.add(new ButtonDialogControl(221));// clone button
 dialog.add(new ButtonDialogControl(223));// clone all  button
 //dialog.add(new CheckBoxDialogControl(224,options,DimensionReverse));
 //dialog.add(new CheckBoxDialogControl(225,options,HideLeftDimensionLine));
 //dialog.add(new CheckBoxDialogControl(226,options,HideRightDimensionLine));

 dialog.add(new ButtonDialogControl(228));// Modify Length button
 dialog.add(new ButtonDialogControl(229));// Modify Equation button
 dialog.add(new ButtonDialogControl(230));// Modify Value button
 dialog.add(new ButtonDialogControl(231));// Add Point button
 dialog.add(new ButtonDialogControl(232));// Delete Point button
 dialog.add(new ButtonDialogControl(233));// Move Point button

 // add the header buttons
 dialog.add(new DTButtonDialogControl(1000));
 dialog.add(new DTButtonDialogControl(1001));
 dialog.add(new DTButtonDialogControl(1002));
 dialog.add(new DTButtonDialogControl(1003));

 ResourceString rs17(NCDIMENA+17);
 subdialog1.title(rs17.gets());
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++);
 nterms = terminfo.style;
 if ((termnames = new RCCHAR *[nterms]) == NULL)
   {  options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
      options->toggle(ScaleText);
      return 0;
   }
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++)
   termnames[terminfo.style] = LineTerminator::name(terminfo);

 dummytermnames = new RCCHAR *[nterms+1];
 dummytermnames[0] = rsVaries.getws();
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++)
   dummytermnames[terminfo.style+1] = LineTerminator::name(terminfo);

 style1 = dd.term1.style&0x7F;
 useweight1 = (dd.term1.style&0x80) != 0;
 position = options->test(9);
 visible = options->test(10);
 weight1 = db.lineweights.weighttomm(term1->weight);

 // terminator dialog
 if(change != NULL)
 {
     if(!change->test(16)) // length
         term1->length = 0.0;
     if(!change->test(17)) // height
         term1->height = 0.0;
     if(!change->test(15)) // term1 style
     {
        style1 = 0;
        dummystyle1 = 0;
     }
     else
     {
         style1 = dd.term1.style&0x7F;
         dummystyle1 = 0;
     }
     if(!change->test(78)) // dimension option 9 terminators visible
     {
         position = 0;
        dummyposition = 0;
     }
     else
     {
         position = options->test(9);
         dummyposition = 0;
     }

     if(!change->test(79)) // dimension option 10 dimension line visible
     {
         visible=0;
         dummyvisible=0;
     }
     else
     {
         visible=options->test(10);
         dummyvisible=0;
     }

     if(change->test(100)) // term 1 style use weight switch
         (dd.term1.style&0x80)==0  ? tristates[9] = 0 : tristates[9] = 2;

     if(!change->test(101)) // weight
         weight1 = 0.1;
 }
 else
 {
     dummyvisible=0;
     dummyposition=0;
     dummystyle1=0;
     (dd.term1.style&0x80)==0  ? tristates[9] = 0 : tristates[9] = 1;
 }

 subdialog1.add(new ListDialogControl(100,nterms,termnames,&style1));
 subdialog1.add(new ListDialogControl(200,nterms+1,dummytermnames,&dummystyle1));
 subdialog1.add(dc = new RealDialogControl(101,&term1->length,1.0,0.0,100000.0,ScaleByLength));
 //subdialog1.add(new ScrollBarDialogControl(1101,dc));
 subdialog1.add(dc = new RealDialogControl(102,&term1->height,1.0,0.0,100000.0,ScaleByLength));
 //subdialog1.add(new ScrollBarDialogControl(1102,dc));
 subdialog1.add(new ListDialogControl(103,2,positionl,&position));
 subdialog1.add(new ListDialogControl(203,3,positionlv,&dummyposition));
 subdialog1.add(new ListDialogControl(104,2,visiblel,&visible));
 subdialog1.add(new ListDialogControl(204,3,visiblelv,&dummyvisible));
 subdialog1.add((DisplayDialogControl *) new NTerminatorDisplayDialogControl(105,&dd));
 subdialog1.add(dc = new WeightAssocRealDialogControl(106,&weight1));
 //subdialog1.add(new ScrollBarDialogControl(1106,dc));
 subdialog1.add(new DTButtonDialogControl(126));
 //subdialog1.add(new CheckBoxDialogControl(107,&useweight1));

 // add the weight dialog button
 subdialog1.add(new DTButtonDialogControl(1006));

 ResourceString rs18(NCDIMENA+18);
 subdialog2.title(rs18.gets());
 style2 = dd.term2.style & 0x7F;
 useweight2 = (dd.term2.style&0x80) != 0;
 weight2 = db.lineweights.weighttomm(term2->weight);

 // CTerminator dialog
 if(change != NULL)
 {
     if(!change->test(21)) // length
         term2->length = 0.0;
     if(!change->test(22)) // height
         term2->height = 0.0;
     if(!change->test(20)) // term1 style
     {
        style2 = 0;
        dummystyle2 = 0;
     }
     else
     {
         style2 = dd.term2.style&0x7F;
         dummystyle2 = 0;
     }

     if(change->test(102)) // term 1 style use weight switch
         (dd.term2.style&0x80)==0  ? tristates[10] = 0 : tristates[10] = 2;

     if(!change->test(102)) // weight
         weight2 = 0.1;
 }
 else
 {
      dummystyle2=0;
     (dd.term2.style&0x80)==0  ? tristates[10] = 0 : tristates[10] = 1;
 }

 subdialog2.add(new ListDialogControl(100,nterms,termnames,&style2));
 subdialog2.add(new ListDialogControl(200,nterms+1,dummytermnames,&dummystyle2));
 subdialog2.add(dc = new RealDialogControl(101,&term2->length,ScaleByLength));
 //subdialog2.add(new ScrollBarDialogControl(1101,dc));
 subdialog2.add(dc = new RealDialogControl(102,&term2->height,ScaleByLength));
 //subdialog2.add(new ScrollBarDialogControl(1102,dc));
 subdialog2.add((DisplayDialogControl *) new CTerminatorDisplayDialogControl(103,&dd));
 subdialog2.add(dc = new WeightAssocRealDialogControl(104,&weight2));
 //subdialog2.add(new ScrollBarDialogControl(1104,dc));
 subdialog2.add(new DTButtonDialogControl(124));
 //subdialog2.add(new CheckBoxDialogControl(105,&useweight2));

 // add the weight dialog button
 subdialog2.add(new DTButtonDialogControl(1007));

 if (! Text::getfonts(dd.vinfo.font,&nfonts,&fonts,&charsets,&fontid1))
   {  options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
      options->toggle(ScaleText);
      return 0;
   }
 if (fonts == NULL)
   {  options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
      options->toggle(ScaleText);
      return 0;
   }

 hjust1 = dd.vinfo.options.test(1) + dd.vinfo.options.test(2) * 2;
 vjust1 = dd.vinfo.options.test(3) + dd.vinfo.options.test(4) * 2;
 placement = options->test(7) + options->test(8) * 2;

 if(change != NULL)
 {
     if(!change->test(25)) // font
     {
         for (i = 0,fontid2 = 0 ; i < nfonts ; i++)
           if (RCSTRCMP(defvinfo.font,fonts[i]) == 0)
           {
               fontid1 = i;
               break;
           }
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(fonts[fontid1]) + 1]) != NULL)
         {
             delete [] dd.vinfo.font;
             strcpy(sc,fonts[fontid1]);
             dd.vinfo.font = sc;
         }
     }
     if(!change->test(26)) // value text height
         ti1->height = 0.0;
     if(!change->test(27)) // value text width
         ti1->width = 0.0;
     if(!change->test(28)) // value text angle
         ti1->angle = -360.0;
     if(!change->test(31)) // text vertspacing
         ti1->vertspacing = -100000000.0;
     if(!change->test(29))
     {
         hjust1 = 0;
         dummyHjust1 = 0;
     }
     else
     {
         hjust1 = dd.vinfo.options.test(1) + dd.vinfo.options.test(2) * 2;
         dummyHjust1 = 0;
     }
     if(!change->test(30))
     {
         vjust1 = 0;
         dummyVjust1 = 0;
     }
     else
     {
         vjust1 = dd.vinfo.options.test(3) + dd.vinfo.options.test(4) * 2;
         dummyVjust1 = 0;
     }
     // placement
     if(!change->test(77))
     {
         placement = 0;
         dummyPlacement = 0;
     }
     else
     {
         placement = options->test(7) + options->test(8) * 2;
         dummyPlacement = 0;
     }

     if(change->test(32)) // bold
         ti1->options.test(5)==0  ? tristates[11] = 0 : tristates[11] = 2;
     if(change->test(33)) // underline
         ti1->options.test(6)==0  ? tristates[12] = 0 : tristates[12] = 2;
     if(change->test(34)) // italic
         ti1->options.test(7)==0  ? tristates[13] = 0 : tristates[13] = 2;
     if(change->test(35)) // boxed
         ti1->options.test(8)==0  ? tristates[14] = 0 : tristates[14] = 2;
     if(change->test(36)) // display
         options->test(2)==0  ? tristates[15] = 0 : tristates[15] = 2;

 }
 else
 {
     dummyHjust1 = 0;
     dummyVjust1 = 0;
     ti1->options.test(5)==0  ? tristates[11] = 0 : tristates[11] = 1;
     ti1->options.test(6)==0  ? tristates[12] = 0 : tristates[12] = 1;
     ti1->options.test(7)==0  ? tristates[13] = 0 : tristates[13] = 1;
     ti1->options.test(8)==0  ? tristates[14] = 0 : tristates[14] = 1;
     options->test(2)==0      ? tristates[15] = 0 : tristates[15] = 1;

 }

 textdialog1.add(new FontListDialogControl(100,nfonts,fonts,&fontid1));
 textdialog1.add(new ListDialogControl(1000,2,dummyfonts,&dummyid1));
 textdialog1.add(dc = new RealDialogControl(101,&ti1->height,ScaleByLength));
 //textdialog1.add(new ScrollBarDialogControl(1101,dc));
 textdialog1.add(dc = new RealDialogControl(102,&ti1->width,ScaleByLength));
 //textdialog1.add(new ScrollBarDialogControl(1102,dc));
 textdialog1.add(dc = new RealDialogControl(103,&ti1->angle));
 //textdialog1.add(new ScrollBarDialogControl(1103,dc));
 textdialog1.add(new ListDialogControl(104,3,hjustl,&hjust1));
 textdialog1.add(new ListDialogControl(1004,4,hjustlv,&dummyHjust1));
 textdialog1.add(new ListDialogControl(105,3,vjustl,&vjust1));
 textdialog1.add(new ListDialogControl(1005,4,vjustlv,&dummyVjust1));
 textdialog1.add(dc = new RealDialogControl(106,&ti1->vertspacing,0.1));
 //textdialog1.add(new ScrollBarDialogControl(1106,dc));
 //textdialog1.add(new CheckBoxDialogControl(107,&ti1->options,5));// bold
 //textdialog1.add(new CheckBoxDialogControl(108,&ti1->options,6));// underline
 //textdialog1.add(new CheckBoxDialogControl(109,&ti1->options,7));// italic
 //textdialog1.add(new CheckBoxDialogControl(110,&ti1->options,8));// boxed
 //textdialog1.add(new CheckBoxDialogControl(111,options,2));
 textdialog1.add(new ListDialogControl(113,3,placementl,&placement));
 textdialog1.add(new ListDialogControl(1013,4,placementlv,&dummyPlacement));
 textdialog1.add((DisplayDialogControl *) new VTextDisplayDialogControl(112,&dd,fonts,charsets));

 hjust2 = dd.tinfo.options.test(1) + dd.tinfo.options.test(2) * 2;
 vjust2 = dd.tinfo.options.test(3) + dd.tinfo.options.test(4) * 2;
 for (i = 0,fontid2 = 0 ; i < nfonts ; i++)
   if (RCSTRCMP(dd.tinfo.font,fonts[i]) == 0)
     {  fontid2 = i;  break;
     }

 if(change != NULL)
 {
     if(!change->test(80)) // font
     {
         for (i = 0,fontid2 = 0 ; i < nfonts ; i++)
           if (RCSTRCMP(deftinfo.font,fonts[i]) == 0)
           {
               fontid2 = i;
               break;
           }
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(fonts[fontid2]) + 1]) != NULL)
         {
             delete [] dd.tinfo.font;
             strcpy(sc,fonts[fontid2]);
             dd.tinfo.font = sc;
         }
     }
     if(!change->test(81)) // value text height
         ti2->height = 0.0;
     if(!change->test(82)) // value text width
         ti2->width = 0.0;
     if(!change->test(83)) // value text angle
         ti2->angle = -360.0;
     if(!change->test(86)) // text vertspacing
         ti2->vertspacing = -100000000.0;
     if(!change->test(84))
     {
         hjust2 = deftinfo.options.test(1) + deftinfo.options.test(2) * 2;;
         dummyHjust2 = 0;
     }
     else
     {
         hjust2 = dd.tinfo.options.test(1) + dd.tinfo.options.test(2) * 2;
         dummyHjust2 = 0;
     }
     if(!change->test(85))
     {
         vjust2 = deftinfo.options.test(3) + deftinfo.options.test(4) * 2;;
         dummyVjust2 = 0;
     }
     else
     {
         vjust2 = dd.tinfo.options.test(3) + dd.tinfo.options.test(4) * 2;
         dummyVjust2 = 0;
     }

     if(change->test(87)) // bold
         ti2->options.test(5)==0  ? tristates[16] = 0 : tristates[16] = 2;
     if(change->test(88)) // underline
         ti2->options.test(6)==0  ? tristates[17] = 0 : tristates[17] = 2;
     if(change->test(89)) // italic
         ti2->options.test(7)==0  ? tristates[18] = 0 : tristates[18] = 2;
     if(change->test(90)) // boxed
         ti2->options.test(8)==0  ? tristates[19] = 0 : tristates[19] = 2;

 }
 else
 {
     dummyHjust2 = 0;
     dummyVjust2 = 0;
     ti2->options.test(5)==0  ? tristates[16] = 0 : tristates[16] = 1;
     ti2->options.test(6)==0  ? tristates[17] = 0 : tristates[17] = 1;
     ti2->options.test(7)==0  ? tristates[18] = 0 : tristates[18] = 1;
     ti2->options.test(8)==0  ? tristates[19] = 0 : tristates[19] = 1;

 }

 textdialog2.add(new FontListDialogControl(100,nfonts,fonts,&fontid2));
 textdialog2.add(new ListDialogControl(1000,2,dummyfonts,&dummyid2));
 textdialog2.add(dc = new RealDialogControl(101,&ti2->height,ScaleByLength));
 //textdialog2.add(new ScrollBarDialogControl(1101,dc));
 textdialog2.add(dc = new RealDialogControl(102,&ti2->width,ScaleByLength));
 //textdialog2.add(new ScrollBarDialogControl(1102,dc));
 textdialog2.add(dc = new RealDialogControl(103,&ti2->angle));
 //textdialog2.add(new ScrollBarDialogControl(1103,dc));
 textdialog2.add(new ListDialogControl(104,3,hjustl,&hjust2));
 textdialog2.add(new ListDialogControl(1004,4,hjustlv,&dummyHjust2));
 textdialog2.add(new ListDialogControl(105,3,vjustl,&vjust2));
 textdialog2.add(new ListDialogControl(1005,4,vjustlv,&dummyVjust2));
 textdialog2.add(dc = new RealDialogControl(106,&ti2->vertspacing,0.1));
 //textdialog2.add(new ScrollBarDialogControl(1106,dc));
 //textdialog2.add(new CheckBoxDialogControl(107,&ti2->options,5));
 //textdialog2.add(new CheckBoxDialogControl(108,&ti2->options,6));
 //textdialog2.add(new CheckBoxDialogControl(109,&ti2->options,7));
 //textdialog2.add(new CheckBoxDialogControl(110,&ti2->options,8));
 textdialog2.add((DisplayDialogControl *) new TTextDisplayDialogControl(111,&dd,fonts,charsets));

 hjust3 = dd.pinfo.options.test(1) + dd.pinfo.options.test(2) * 2;
 vjust3 = dd.pinfo.options.test(3) + dd.pinfo.options.test(4) * 2;
 for (i = 0,fontid3 = 0 ; i < nfonts ; i++)
   if (RCSTRCMP(dd.pinfo.font,fonts[i]) == 0)
     {  fontid3 = i;  break;
     }

 if(change != NULL)
 {
     if(!change->test(40)) // font
     {
         for (i = 0,fontid3 = 0 ; i < nfonts ; i++)
           if (RCSTRCMP(defpinfo.font,fonts[i]) == 0)
           {
               fontid3 = i;
               break;
           }
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(fonts[fontid3]) + 1]) != NULL)
         {
             //delete [] dd.pinfo.font;
             dd.pinfo.font = 0;
             strcpy(sc,fonts[fontid3]);
             dd.pinfo.font = sc;
         }
     }
     if(!change->test(41)) // value text height
         ti3->height = 0.0;
     if(!change->test(42)) // value text width
         ti3->width = 0.0;
     if(!change->test(43)) // value text angle
         ti3->angle = -360.0;
     if(!change->test(46)) // text vertspacing
         ti3->vertspacing = -100000000.0;
     if(!change->test(44))
     {
         hjust3 = defpinfo.options.test(1) + defpinfo.options.test(2) * 2;;
         dummyHjust3 = 0;
     }
     else
     {
         hjust3 = dd.pinfo.options.test(1) + dd.pinfo.options.test(2) * 2;
         dummyHjust3 = 0;
     }
     if(!change->test(45))
     {
         vjust3 = defpinfo.options.test(3) + defpinfo.options.test(4) * 2;;
         dummyVjust3 = 0;
     }
     else
     {
         vjust3 = dd.pinfo.options.test(3) + dd.pinfo.options.test(4) * 2;
         dummyVjust3 = 0;
     }

     if(change->test(47)) // bold
         ti3->options.test(5)==0  ? tristates[20] = 0 : tristates[20] = 2;
     if(change->test(48)) // underline
         ti3->options.test(6)==0  ? tristates[21] = 0 : tristates[21] = 2;
     if(change->test(49)) // italic
         ti3->options.test(7)==0  ? tristates[22] = 0 : tristates[22] = 2;
     if(change->test(50)) // boxed
         ti3->options.test(8)==0  ? tristates[23] = 0 : tristates[23] = 2;

 }
 else
 {
     dummyHjust3 = 0;
     dummyVjust3 = 0;
     ti3->options.test(5)==0  ? tristates[20] = 0 : tristates[20] = 1;
     ti3->options.test(6)==0  ? tristates[21] = 0 : tristates[21] = 1;
     ti3->options.test(7)==0  ? tristates[22] = 0 : tristates[22] = 1;
     ti3->options.test(8)==0  ? tristates[23] = 0 : tristates[23] = 1;

 }
 ResourceString rs19(NCDIMENA+19);
 textdialog3.title(rs19.gets());
 textdialog3.add(new FontListDialogControl(100,nfonts,fonts,&fontid3));
 textdialog3.add(new ListDialogControl(1000,2,dummyfonts,&dummyid3));
 textdialog3.add(dc = new RealDialogControl(101,&ti3->height,ScaleByLength));
 //textdialog3.add(new ScrollBarDialogControl(1101,dc));
 textdialog3.add(dc = new RealDialogControl(102,&ti3->width,ScaleByLength));
 //textdialog3.add(new ScrollBarDialogControl(1102,dc));
 textdialog3.add(dc = new RealDialogControl(103,&ti3->angle));
 //textdialog3.add(new ScrollBarDialogControl(1103,dc));
 textdialog3.add(new ListDialogControl(104,3,hjustl,&hjust3));
 textdialog3.add(new ListDialogControl(1004,4,hjustlv,&dummyHjust3));
 textdialog3.add(new ListDialogControl(105,3,vjustl,&vjust3));
 textdialog3.add(new ListDialogControl(1005,4,vjustlv,&dummyVjust3));
 textdialog3.add(dc = new RealDialogControl(106,&ti3->vertspacing,0.1));
 //textdialog3.add(new ScrollBarDialogControl(1106,dc));
 //textdialog3.add(new CheckBoxDialogControl(107,&ti3->options,5));
 //textdialog3.add(new CheckBoxDialogControl(108,&ti3->options,6));
 //textdialog3.add(new CheckBoxDialogControl(109,&ti3->options,7));
 //textdialog3.add(new CheckBoxDialogControl(110,&ti3->options,8));
 textdialog3.add((DisplayDialogControl *) new PTextDisplayDialogControl(111,&dd,fonts,charsets));

 hjust4 = dd.sinfo.options.test(1) + dd.sinfo.options.test(2) * 2;
 vjust4 = dd.sinfo.options.test(3) + dd.sinfo.options.test(4) * 2;
 for (i = 0,fontid4 = 0 ; i < nfonts ; i++)
   if (RCSTRCMP(dd.sinfo.font,fonts[i]) == 0)
     {  fontid4 = i;  break;
     }

 if(change != NULL)
 {
     if(!change->test(55)) // font
     {
         for (i = 0,fontid4 = 0 ; i < nfonts ; i++)
           if (RCSTRCMP(defsinfo.font,fonts[i]) == 0)
           {
               fontid4 = i;
               break;
           }
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(fonts[fontid4]) + 1]) != NULL)
         {
             //delete [] dd.sinfo.font;
             strcpy(sc,fonts[fontid4]);
             dd.sinfo.font = sc;
         }
     }
     if(!change->test(56)) // value text height
         ti4->height = 0.0;
     if(!change->test(57)) // value text width
         ti4->width = 0.0;
     if(!change->test(58)) // value text angle
         ti4->angle = -360.0;
     if(!change->test(61)) // text vertspacing
         ti4->vertspacing = -100000000.0;
     if(!change->test(59))
     {
         hjust4 = defsinfo.options.test(1) + defsinfo.options.test(2) * 2;;
         dummyHjust4 = 0;
     }
     else
     {
         hjust4 = dd.sinfo.options.test(1) + dd.sinfo.options.test(2) * 2;
         dummyHjust4 = 0;
     }
     if(!change->test(60))
     {
         vjust3 = defsinfo.options.test(3) + defsinfo.options.test(4) * 2;;
         dummyVjust4 = 0;
     }
     else
     {
         vjust3 = dd.sinfo.options.test(3) + dd.sinfo.options.test(4) * 2;
         dummyVjust4 = 0;
     }

     if(change->test(62)) // bold
         ti4->options.test(5)==0  ? tristates[25] = 0 : tristates[25] = 2;
     if(change->test(63)) // underline
         ti4->options.test(6)==0  ? tristates[26] = 0 : tristates[26] = 2;
     if(change->test(64)) // italic
         ti4->options.test(7)==0  ? tristates[27] = 0 : tristates[27] = 2;
     if(change->test(65)) // boxed
         ti4->options.test(8)==0  ? tristates[28] = 0 : tristates[28] = 2;

 }
 else
 {
     dummyHjust4 = 0;
     dummyVjust4 = 0;
     ti4->options.test(5)==0  ? tristates[25] = 0 : tristates[25] = 1;
     ti4->options.test(6)==0  ? tristates[26] = 0 : tristates[26] = 1;
     ti4->options.test(7)==0  ? tristates[27] = 0 : tristates[27] = 1;
     ti4->options.test(8)==0  ? tristates[28] = 0 : tristates[28] = 1;

 }

 ResourceString rs20(NCDIMENA+20);
 textdialog4.title(rs20.gets());
 textdialog4.add(new FontListDialogControl(100,nfonts,fonts,&fontid4));
 textdialog4.add(new ListDialogControl(1000,2,dummyfonts,&dummyid4));
 textdialog4.add(dc = new RealDialogControl(101,&ti4->height,ScaleByLength));
 //textdialog4.add(new ScrollBarDialogControl(1101,dc));
 textdialog4.add(dc = new RealDialogControl(102,&ti4->width,ScaleByLength));
 //textdialog4.add(new ScrollBarDialogControl(1102,dc));
 textdialog4.add(dc = new RealDialogControl(103,&ti4->angle));
 //textdialog4.add(new ScrollBarDialogControl(1103,dc));
 textdialog4.add(new ListDialogControl(104,3,hjustl,&hjust4));
 textdialog4.add(new ListDialogControl(1004,4,hjustlv,&dummyHjust4));
 textdialog4.add(new ListDialogControl(105,3,vjustl,&vjust4));
 textdialog4.add(new ListDialogControl(1005,4,vjustlv,&dummyVjust4));
 textdialog4.add(dc = new RealDialogControl(106,&ti4->vertspacing,0.1));
 //textdialog4.add(new ScrollBarDialogControl(1106,dc));
 //textdialog4.add(new CheckBoxDialogControl(107,&ti4->options,5));
 //textdialog4.add(new CheckBoxDialogControl(108,&ti4->options,6));
 //textdialog4.add(new CheckBoxDialogControl(109,&ti4->options,7));
 //textdialog4.add(new CheckBoxDialogControl(110,&ti4->options,8));
 textdialog4.add((DisplayDialogControl *) new STextDisplayDialogControl(111,&dd,fonts,charsets));

 strcpy(prefix,dd.pinfo.text);
 dd.pinfo.text = prefix;
 int UsePrefix1=0;
 dd.usePrefix1=0;

 if(change != NULL)
 {
    if(!change->test(71)) // prefix text
    {
         strcpy(prefix,_RCT(""));
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(prefix) + 1]) != NULL)
         {
             //delete [] ti3->text;
             strcpy(sc,prefix);
             ti3->text = sc;
         }
         UsePrefix1 = 0;
         dd.usePrefix1=0;
    }
    if(change->test(70)) // use value text style check box
        options->test(3)==0  ? tristates[24] = 0 : tristates[24] = 2;
 }
 else
 {
    options->test(3)==0  ? tristates[24] = 0 : tristates[24] = 1;
 }

 ResourceString rs21(NCDIMENA+21);
 entrydialog1.title(rs21.gets());
 //entrydialog1.add(new CheckBoxDialogControl(100,options,3));
 entrydialog1.add(new SubDialogButtonDialogControl(101,&textdialog3,&dd,&weight1,&weight2));
 entrydialog1.add(new StringDialogControl(102,prefix,511));
 entrydialog1.add((DisplayDialogControl *) new PEntryDisplayDialogControl(103,&dd));
 entrydialog1.add(new CheckBoxDialogControl(104,&dd.usePrefix1));

 strcpy(suffix,dd.sinfo.text);
 dd.sinfo.text = suffix;
 int UseSuffix1=0;
 dd.useSuffix1=0;

 if(change != NULL)
 {
    if(!change->test(76)) // suffix text
    {
         strcpy(suffix,_RCT(""));
         RCCHAR *sc;
         if ((sc = new RCCHAR[strlen(suffix) + 1]) != NULL)
         {
             //delete [] ti4->font;
             //delete [] ti4->text;
             strcpy(sc,suffix);
             //ti4->font = sc;
             ti4->text = sc;
         }
         UseSuffix1 = 0;
         dd.useSuffix1=0;
    }
    if(change->test(75)) // use value text style check box
        options->test(4)==0  ? tristates[29] = 0 : tristates[29] = 2;
 }
 else
 {
    options->test(4)==0  ? tristates[29] = 0 : tristates[29] = 1;
 }

 ResourceString rs22(NCDIMENA+22);
 entrydialog2.title(rs22.gets());
 //entrydialog2.add(new CheckBoxDialogControl(100,options,4));
 entrydialog2.add(new SubDialogButtonDialogControl(101,&textdialog4,&dd,&weight1,&weight2));
 entrydialog2.add(new StringDialogControl(102,suffix,511));
 entrydialog2.add((DisplayDialogControl *) new SEntryDisplayDialogControl(103,&dd));
 entrydialog2.add(new CheckBoxDialogControl(104,&dd.useSuffix1));

 if (change != NULL)
 {
      //change->setall();
      //change->clearall();
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

     if(changecount > 1 && !change->test(5)) // extension
     {
         ((RealDialogControl*)dialog.getcontrol(200))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(200))->setDefaultValue(defextension);
     }
     if(changecount > 1 && !change->test(6)) // gap
     {
         ((RealDialogControl*)dialog.getcontrol(201))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(201))->setDefaultValue(defgap);
     }
     if(changecount > 1 && !change->test(7)) // precision
     {
         ((IntegerDialogControl*)dialog.getcontrol(202))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(202))->setDefaultValue(defgap);
     }
     if(changecount > 1 && !change->test(10)) // stackdist
     {
         ((RealDialogControl*)dialog.getcontrol(205))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(205))->setDefaultValue(defstackdis);
     }
     if(changecount > 1 && !change->test(12)) // uppertol
     {
         ((RealDialogControl*)dialog.getcontrol(212))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(212))->setDefaultValue(defuppertol);
     }
     if(changecount > 1 && !change->test(13)) // uppertol
     {
         ((RealDialogControl*)dialog.getcontrol(213))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(213))->setDefaultValue(deflowertol);
     }
     if(changecount > 1 && !change->test(93)) // value scale
     {
         ((RealDialogControl*)dialog.getcontrol(218))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(218))->setDefaultValue(defscale);
     }
     if(changecount > 1 && !change->test(99)) // text yoffset
     {
         ((RealDialogControl*)dialog.getcontrol(222))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(222))->setDefaultValue(deftextyoffset);
     }

     dialog.add(new TriStateCheckBoxDialogControl(203,&tristates[0],changecount>=1^change->test(8),defOptions.test(0)/*,0*/));// option 0 common point dimension
     dialog.add(new TriStateCheckBoxDialogControl(204,&tristates[1],changecount>=1^change->test(9),defOptions.test(1)/*,1*/));// option 1 stacked dimension
     dialog.add(new TriStateCheckBoxDialogControl(216,&tristates[2],changecount>=1^change->test(91),defOptions.test(ToleranceDefined)/*,ToleranceDefined*/));
     dialog.add(new TriStateCheckBoxDialogControl(217,&tristates[3],changecount>=1^change->test(92),defOptions.test(GapFromDimension)/*,GapFromDimension*/));
     dialog.add(new TriStateCheckBoxDialogControl(219,&tristates[4],changecount>=1^change->test(94),defOptions.test(ScaleText)/*,ScaleText*/));
     dialog.add(new TriStateCheckBoxDialogControl(220,&tristates[5],changecount>=1^change->test(95),defOptions.test(FixedDimensionPosition)/*,FixedDimensionPosition*/));
     dialog.add(new TriStateCheckBoxDialogControl(224,&tristates[6],changecount>=1^change->test(104),defOptions.test(DimensionReverse)/*,DimensionReverse*/));
     dialog.add(new TriStateCheckBoxDialogControl(225,&tristates[7],changecount>=1^change->test(105),defOptions.test(HideLeftDimensionLine)/*,HideLeftDimensionLine*/));
     dialog.add(new TriStateCheckBoxDialogControl(226,&tristates[8],changecount>=1^change->test(106),defOptions.test(HideRightDimensionLine)/*,HideRightDimensionLine*/));

     // terminator dialog
     if(changecount > 1 && !change->test(16)) // length
     {
         ((RealDialogControl*)subdialog1.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)subdialog1.getcontrol(101))->setDefaultValue(defextension);
     }

     if(changecount > 1 && !change->test(17)) // height
     {
         ((RealDialogControl*)subdialog1.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)subdialog1.getcontrol(102))->setDefaultValue(defextension);
     }
     if(changecount > 1 && !change->test(101)) // weight
     {
         ((WeightAssocRealDialogControl*)subdialog1.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((WeightAssocRealDialogControl*)subdialog1.getcontrol(106))->setDefaultValue(defWeight);
     }

     // use tristate check boxes depending on the value
     subdialog1.add(new TriStateCheckBoxDialogControl(107,&tristates[9],changecount>=1^change->test(100),(defterm1.style&0x80)!=0));

     // terminator2 dialog
     if(changecount > 1 && !change->test(21)) // length
     {
         ((RealDialogControl*)subdialog2.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)subdialog2.getcontrol(101))->setDefaultValue(defextension);
     }
     if(changecount > 1 && !change->test(22)) // height
     {
         ((RealDialogControl*)subdialog2.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)subdialog2.getcontrol(102))->setDefaultValue(defextension);
     }
     if(changecount > 1 && !change->test(103)) // weight
     {
         ((WeightAssocRealDialogControl*)subdialog2.getcontrol(104))->setSpecialValueText(_RCT("*Varies*"));
         ((WeightAssocRealDialogControl*)subdialog2.getcontrol(104))->setDefaultValue(defWeight);
     }

     // use tristate check boxes depending on the value
     subdialog2.add(new TriStateCheckBoxDialogControl(105,&tristates[10],changecount>=1^change->test(102),(defterm2.style&0x80)!=0));

     // value text dialog (textdialog1)
     if(changecount > 1 && !change->test(25)) // font
     {
         ((FontListDialogControl*)textdialog1.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
         ((FontListDialogControl*)textdialog1.getcontrol(100))->setDefaultFont(defvinfo.font);
     }
     if(changecount > 1 && !change->test(26)) // height
     {
         ((RealDialogControl*)textdialog1.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog1.getcontrol(101))->setDefaultValue(defvinfo.height);
     }
     if(changecount > 1 && !change->test(27)) // width
     {
         ((RealDialogControl*)textdialog1.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog1.getcontrol(102))->setDefaultValue(defvinfo.width);
     }
     if(changecount > 1 && !change->test(28)) // angle
     {
         ((RealDialogControl*)textdialog1.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog1.getcontrol(103))->setDefaultValue(defvinfo.angle);
     }
     if(changecount > 1 && !change->test(31)) // vertspacing
     {
         ((RealDialogControl*)textdialog1.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog1.getcontrol(106))->setDefaultValue(defvinfo.vertspacing);
     }

     textdialog1.add(new TriStateCheckBoxDialogControl(107,&tristates[11],changecount>=1^change->test(32),ti1->options.test(5)));// bold
     textdialog1.add(new TriStateCheckBoxDialogControl(108,&tristates[12],changecount>=1^change->test(33),ti1->options.test(6)));// underline
     textdialog1.add(new TriStateCheckBoxDialogControl(109,&tristates[13],changecount>=1^change->test(34),ti1->options.test(7)));// italic
     textdialog1.add(new TriStateCheckBoxDialogControl(110,&tristates[14],changecount>=1^change->test(35),ti1->options.test(8)));// boxed
     textdialog1.add(new TriStateCheckBoxDialogControl(111,&tristates[15],changecount>=1^change->test(36),options->test(2)));// display value

     // toler text dialog (textdialog2)
     if(changecount > 1 && !change->test(80)) // font
     {
         ((FontListDialogControl*)textdialog2.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
         ((FontListDialogControl*)textdialog2.getcontrol(100))->setDefaultFont(deftinfo.font);
     }
     if(changecount > 1 && !change->test(81)) // height
     {
         ((RealDialogControl*)textdialog2.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog2.getcontrol(101))->setDefaultValue(deftinfo.height);
     }
     if(changecount > 1 && !change->test(82)) // width
     {
         ((RealDialogControl*)textdialog2.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog2.getcontrol(102))->setDefaultValue(deftinfo.width);
     }
     if(changecount > 1 && !change->test(83)) // angle
     {
         ((RealDialogControl*)textdialog2.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog2.getcontrol(103))->setDefaultValue(deftinfo.angle);
     }
     if(changecount > 1 && !change->test(86)) // vertspacing
     {
         ((RealDialogControl*)textdialog2.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog2.getcontrol(106))->setDefaultValue(deftinfo.vertspacing);
     }

     textdialog2.add(new TriStateCheckBoxDialogControl(107,&tristates[16],changecount>=1^change->test(87),ti2->options.test(5)));// bold
     textdialog2.add(new TriStateCheckBoxDialogControl(108,&tristates[17],changecount>=1^change->test(88),ti2->options.test(6)));// underline
     textdialog2.add(new TriStateCheckBoxDialogControl(109,&tristates[18],changecount>=1^change->test(89),ti2->options.test(7)));// italic
     textdialog2.add(new TriStateCheckBoxDialogControl(110,&tristates[19],changecount>=1^change->test(90),ti2->options.test(8)));// boxed

     // prefix text dialog (textdialog3)
     if(changecount > 1 && !change->test(40)) // font
     {
         ((FontListDialogControl*)textdialog3.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
         ((FontListDialogControl*)textdialog3.getcontrol(100))->setDefaultFont(defvinfo.font);
     }
     if(changecount > 1 && !change->test(41)) // height
     {
         ((RealDialogControl*)textdialog3.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog3.getcontrol(101))->setDefaultValue(defvinfo.height);
     }
     if(changecount > 1 && !change->test(42)) // width
     {
         ((RealDialogControl*)textdialog3.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog3.getcontrol(102))->setDefaultValue(defvinfo.width);
     }
     if(changecount > 1 && !change->test(43)) // angle
     {
         ((RealDialogControl*)textdialog3.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog3.getcontrol(103))->setDefaultValue(defvinfo.angle);
     }
     if(changecount > 1 && !change->test(46)) // vertspacing
     {
         ((RealDialogControl*)textdialog3.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog3.getcontrol(106))->setDefaultValue(defvinfo.vertspacing);
     }

     textdialog3.add(new TriStateCheckBoxDialogControl(107,&tristates[20],changecount>=1^change->test(47),ti3->options.test(5)));// bold
     textdialog3.add(new TriStateCheckBoxDialogControl(108,&tristates[21],changecount>=1^change->test(48),ti3->options.test(6)));// underline
     textdialog3.add(new TriStateCheckBoxDialogControl(109,&tristates[22],changecount>=1^change->test(49),ti3->options.test(7)));// italic
     textdialog3.add(new TriStateCheckBoxDialogControl(110,&tristates[23],changecount>=1^change->test(50),ti3->options.test(8)));// boxed

     // suffix text dialog (textdialog4)
     if(changecount > 1 && !change->test(55)) // font
     {
         ((FontListDialogControl*)textdialog4.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
         ((FontListDialogControl*)textdialog4.getcontrol(100))->setDefaultFont(defvinfo.font);
     }
     if(changecount > 1 && !change->test(56)) // height
     {
         ((RealDialogControl*)textdialog4.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog4.getcontrol(101))->setDefaultValue(defvinfo.height);
     }
     if(changecount > 1 && !change->test(57)) // width
     {
         ((RealDialogControl*)textdialog4.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog4.getcontrol(102))->setDefaultValue(defvinfo.width);
     }
     if(changecount > 1 && !change->test(58)) // angle
     {
         ((RealDialogControl*)textdialog4.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog4.getcontrol(103))->setDefaultValue(defvinfo.angle);
     }
     if(changecount > 1 && !change->test(67)) // vertspacing
     {
         ((RealDialogControl*)textdialog4.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)textdialog4.getcontrol(106))->setDefaultValue(defvinfo.vertspacing);
     }

     textdialog4.add(new TriStateCheckBoxDialogControl(107,&tristates[25],changecount>=1^change->test(62),ti4->options.test(5)));// bold
     textdialog4.add(new TriStateCheckBoxDialogControl(108,&tristates[26],changecount>=1^change->test(63),ti4->options.test(6)));// underline
     textdialog4.add(new TriStateCheckBoxDialogControl(109,&tristates[27],changecount>=1^change->test(64),ti4->options.test(7)));// italic
     textdialog4.add(new TriStateCheckBoxDialogControl(110,&tristates[28],changecount>=1^change->test(65),ti4->options.test(8)));// boxed

     // prefix 0 options (entrydialog1)
     // text
     if(changecount > 1 && !change->test(71))
     {
         ((StringDialogControl*)entrydialog1.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((StringDialogControl*)entrydialog1.getcontrol(102))->setDefaultValue(_RCT("*Varies*")/*defpinfo.text*/);
     }
     else if(changecount >= 1 && change->test(71))
     {
         ((StringDialogControl*)entrydialog1.getcontrol(102))->setSpecialValueText(_RCT("*Prefix will not change*"));
         ((StringDialogControl*)entrydialog1.getcontrol(102))->setDefaultValue(_RCT("*Varies*")/*defpinfo.text*/);
     }
     // tristate box
     // use value text style
     entrydialog1.add(new TriStateCheckBoxDialogControl(100,&tristates[24],changecount>=1^change->test(70),options->test(3)));

     // suffix 0 options (entrydialog2)
     // text
     if(changecount > 1 && !change->test(76))
     {
         ((StringDialogControl*)entrydialog2.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((StringDialogControl*)entrydialog2.getcontrol(102))->setDefaultValue(_RCT("*Varies*")/*defpinfo.text*/);
     }
     else if(changecount >= 1 && change->test(76))
     {
         ((StringDialogControl*)entrydialog2.getcontrol(102))->setSpecialValueText(_RCT("*Prefix will not change*"));
         ((StringDialogControl*)entrydialog2.getcontrol(102))->setDefaultValue(_RCT("*Varies*")/*defpinfo.text*/);
     }
     // tristate box
     // use value text style
     entrydialog2.add(new TriStateCheckBoxDialogControl(100,&tristates[29],changecount>=1^change->test(75),options->test(4)));

 }
 else
 {
     dialog.add(new TriStateCheckBoxDialogControl(203,&tristates[0],false,options->test(0)/*,0*/));// option 0 common point dimension
     dialog.add(new TriStateCheckBoxDialogControl(204,&tristates[1],false,options->test(1)/*,1*/));// option 1 stacked dimension
     dialog.add(new TriStateCheckBoxDialogControl(216,&tristates[2],false,options->test(ToleranceDefined)/*,ToleranceDefined*/));
     dialog.add(new TriStateCheckBoxDialogControl(217,&tristates[3],false,options->test(GapFromDimension)/*,GapFromDimension*/));
     dialog.add(new TriStateCheckBoxDialogControl(219,&tristates[4],false,options->test(ScaleText)/*,ScaleText*/));
     dialog.add(new TriStateCheckBoxDialogControl(220,&tristates[5],false,options->test(FixedDimensionPosition)/*,FixedDimensionPosition*/));
     dialog.add(new TriStateCheckBoxDialogControl(224,&tristates[6],false,options->test(DimensionReverse)/*,DimensionReverse*/));
     dialog.add(new TriStateCheckBoxDialogControl(225,&tristates[7],false,options->test(HideLeftDimensionLine)/*,HideLeftDimensionLine*/));
     dialog.add(new TriStateCheckBoxDialogControl(226,&tristates[8],false,options->test(HideRightDimensionLine)/*,HideRightDimensionLine*/));

     // terminator dialog
     subdialog1.add(new TriStateCheckBoxDialogControl(107,&tristates[9],false,(dd.term1.style&0x80)!=0));

     // CTerminator dialog
     subdialog2.add(new TriStateCheckBoxDialogControl(105,&tristates[10],false,(dd.term2.style&0x80)!=0));

     // value text dialog (textdialog1)
     textdialog1.add(new TriStateCheckBoxDialogControl(107,&tristates[11],false,ti1->options.test(5)));// bold
     textdialog1.add(new TriStateCheckBoxDialogControl(108,&tristates[12],false,ti1->options.test(6)));// underline
     textdialog1.add(new TriStateCheckBoxDialogControl(109,&tristates[13],false,ti1->options.test(7)));// italic
     textdialog1.add(new TriStateCheckBoxDialogControl(110,&tristates[14],false,ti1->options.test(8)));// boxed
     textdialog1.add(new TriStateCheckBoxDialogControl(111,&tristates[15],false,options->test(2)));// display value

     // value text dialog (textdialog2)
     textdialog2.add(new TriStateCheckBoxDialogControl(107,&tristates[16],false,ti2->options.test(5)));// bold
     textdialog2.add(new TriStateCheckBoxDialogControl(108,&tristates[17],false,ti2->options.test(6)));// underline
     textdialog2.add(new TriStateCheckBoxDialogControl(109,&tristates[18],false,ti2->options.test(7)));// italic
     textdialog2.add(new TriStateCheckBoxDialogControl(110,&tristates[19],false,ti2->options.test(8)));// boxed

     // value text dialog (textdialog3)
     textdialog3.add(new TriStateCheckBoxDialogControl(107,&tristates[20],false,ti3->options.test(5)));// bold
     textdialog3.add(new TriStateCheckBoxDialogControl(108,&tristates[21],false,ti3->options.test(6)));// underline
     textdialog3.add(new TriStateCheckBoxDialogControl(109,&tristates[22],false,ti3->options.test(7)));// italic
     textdialog3.add(new TriStateCheckBoxDialogControl(110,&tristates[23],false,ti3->options.test(8)));// boxed

     // suffix text dialog (textdialog4)
     textdialog4.add(new TriStateCheckBoxDialogControl(107,&tristates[25],false,ti4->options.test(5)));// bold
     textdialog4.add(new TriStateCheckBoxDialogControl(108,&tristates[26],false,ti4->options.test(6)));// underline
     textdialog4.add(new TriStateCheckBoxDialogControl(109,&tristates[27],false,ti4->options.test(7)));// italic
     textdialog4.add(new TriStateCheckBoxDialogControl(110,&tristates[28],false,ti4->options.test(8)));// boxed

     // prefix 0 options (entrydialog1)
     ((StringDialogControl*)entrydialog1.getcontrol(102))->setSpecialValueText(_RCT("*Prefix will not change*"));
     // tristate box
     // use value text style
     entrydialog1.add(new TriStateCheckBoxDialogControl(100,&tristates[24],false,options->test(3)));

     // suffix 0 options (entrydialog2)
     ((StringDialogControl*)entrydialog2.getcontrol(102))->setSpecialValueText(_RCT("*Prefix will not change*"));
     // tristate box
     // use value text style
     entrydialog2.add(new TriStateCheckBoxDialogControl(100,&tristates[29],false,options->test(4)));

 }

 v.setinteger("op::change",0);
 v.setbm("op::changemask",defOptions);

 if(change != NULL)
 {
     v.setinteger("op::change",1);
     v.setbm("op::changemask",*change);

     ResourceString rs23(NCDIMENA+23);
     if(changecount == 1)
         dialog.title("Dimension Verify");
     else
         dialog.title(rs23.gets());
 }

 if ((status = dialog.process()) == TRUE)
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
             header->setstyle(st);
             header->setlayer(l);
             header->setweight(db.lineweights.mmtoweight(w));
         }

         // header settings
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
             header->setstyle(st);
         }
         if( changecount > 1 && !changeIn.test(3) &&
             strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(3);
         else
         {
             change->set(3);
             header->setweight(db.lineweights.mmtoweight(w));
         }
         // extension
         if( changecount > 1 && !changeIn.test(5) &&
             strlen(((RealDialogControl*)dialog.getcontrol(200))->getSpecialValueText()) != 0 )
             change->clear(5);
         else
         {
             change->set(5);
         }
         // gap
         if( changecount > 1 && !changeIn.test(6) &&
             strlen(((RealDialogControl*)dialog.getcontrol(201))->getSpecialValueText()) != 0 )
             change->clear(6);
         else
         {
             change->set(6);
         }
         // precision
         if( changecount > 1 && !changeIn.test(7) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(202))->getSpecialValueText()) != 0 )
             change->clear(7);
         else
         {
             change->set(7);
         }
         // stackdist
         if( changecount > 1 && !changeIn.test(10) &&
             strlen(((RealDialogControl*)dialog.getcontrol(205))->getSpecialValueText()) != 0 )
             change->clear(10);
         else
         {
             change->set(10);
         }
         // uppertol
         if( changecount > 1 && !changeIn.test(12) &&
             strlen(((RealDialogControl*)dialog.getcontrol(212))->getSpecialValueText()) != 0 )
             change->clear(12);
         else
         {
             change->set(12);
         }
         // lowertol
         if( changecount > 1 && !changeIn.test(13) &&
             strlen(((RealDialogControl*)dialog.getcontrol(213))->getSpecialValueText()) != 0 )
             change->clear(13);
         else
         {
             change->set(13);
         }
         // value scale
         if( changecount > 1 && !changeIn.test(93) &&
             strlen(((RealDialogControl*)dialog.getcontrol(218))->getSpecialValueText()) != 0 )
             change->clear(93);
         else
         {
             change->set(93);
         }
         // textyoffset
         if( changecount > 1 && !changeIn.test(99) &&
             strlen(((RealDialogControl*)dialog.getcontrol(222))->getSpecialValueText()) != 0 )
             change->clear(99);
         else
         {
             change->set(99);
         }
         // tolerance style
         if(changecount > 1 && !changeIn.test(11) && dummytolerance == 0) // not changed
             change->clear(11);
         else
         {
             options->set(5,tolerance % 2);
             options->set(6,tolerance / 2);
             change->set(11);
         }
         // tristate switches
         //option 0 common point dimension
         if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(203))->isTriState())
             change->clear(8);
         else
         {
             change->set(8);
             options->set(0, tristates[0]);
         }
         // option 1 stacked dimension
         if(tristates[1] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(204))->isTriState())
             change->clear(9);
         else
         {
             change->set(9);
             options->set(1, tristates[1]);
         }
         // ToleranceDefined
         if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(216))->isTriState())
             change->clear(91);
         else
         {
             change->set(91);
             options->set(ToleranceDefined, tristates[2]);
         }
         // GapFromDimension
         if(tristates[3] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(217))->isTriState())
             change->clear(92);
         else
         {
             change->set(92);
             options->set(GapFromDimension, tristates[3]);
         }
         // ScaleText
         if(tristates[4] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(219))->isTriState())
             change->clear(94);
         else
         {
             change->set(94);
             options->set(ScaleText, tristates[4]);
         }
         // FixedDimensionPosition
         if(tristates[5] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(220))->isTriState())
             change->clear(95);
         else
         {
             change->set(95);
             options->set(FixedDimensionPosition, tristates[5]);
         }
         // DimensionReverse
         if(tristates[6] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(224))->isTriState())
             change->clear(104);
         else
         {
             change->set(104);
             options->set(DimensionReverse, tristates[6]);
         }
         // HideLeftDimensionLine
         if(tristates[7] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(225))->isTriState())
             change->clear(105);
         else
         {
             change->set(105);
             options->set(HideLeftDimensionLine, tristates[7]);
         }
         // HideRightDimensionLine
         if(tristates[8] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(226))->isTriState())
             change->clear(106);
         else
         {
             change->set(106);
             options->set(HideRightDimensionLine, tristates[8]);
         }

         // terminator dialog (subdialog1)
         // length
         if( changecount > 1 && !changeIn.test(16) &&
             strlen(((RealDialogControl*)subdialog1.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(16);
         else
             change->set(16);
         // height
         if( changecount > 1 && !changeIn.test(17) &&
             strlen(((RealDialogControl*)subdialog1.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(17);
         else
             change->set(17);
         // weight
         if( changecount > 1 && !changeIn.test(101) &&
             strlen(((WeightAssocRealDialogControl*)subdialog1.getcontrol(106))->getSpecialValueText()) != 0 )
             change->clear(101);
         else
         {
             term1->weight = db.lineweights.mmtoweight(weight1);
             change->set(101);
         }
         // position
         if( changecount > 1 && !changeIn.test(78) && dummyposition == 0)
             change->clear(78);
         else
         {
             options->set(9,position);
             change->set(78);
         }
         // dimension line visible
         if( changecount > 1 && !changeIn.test(79) && dummyvisible == 0)
             change->clear(79);
         else
         {
             options->set(10,visible);
             change->set(79);
         }
         // term style
         if( changecount > 1 && !changeIn.test(15) && dummystyle1 == 0)
             change->clear(15);
         else
         {
             term1->style = Byte((style1&0x7F) | (term1->style&0x80));
             change->set(15);
         }
         // use terminator weight
         if(tristates[9] == 1 && ((TriStateCheckBoxDialogControl*)subdialog1.getcontrol(107))->isTriState())
         {
             change->clear(100);
         }
         else
         {
             if(tristates[9] != 0)
                useweight1 = 1;
             else
                useweight1 = 0;
             term1->style = Byte((term1->style&0x7F) | (0x80 * useweight1));
             change->set(100);
         }

         // CTerminator dialog (subdialog2)
         // length
         if( changecount > 1 && !changeIn.test(21) &&
             strlen(((RealDialogControl*)subdialog2.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(21);
         else
             change->set(21);
         // height
         if( changecount > 1 && !changeIn.test(22) &&
             strlen(((RealDialogControl*)subdialog2.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(22);
         else
             change->set(22);
         // weight
         if( changecount > 1 && !changeIn.test(103) &&
             strlen(((WeightAssocRealDialogControl*)subdialog2.getcontrol(104))->getSpecialValueText()) != 0 )
             change->clear(103);
         else
         {
             term2->weight = db.lineweights.mmtoweight(weight2);
             change->set(103);
         }
         // term style
         if( changecount > 1 && !changeIn.test(20) && dummystyle2 == 0)
             change->clear(20);
         else
         {
             term2->style = Byte((style2&0x7F) | (term2->style&0x80));
             change->set(20);
         }
         // use terminator weight
         if(tristates[10] == 1 && ((TriStateCheckBoxDialogControl*)subdialog2.getcontrol(105))->isTriState())
         {
             change->clear(102);
         }
         else
         {
             if(tristates[10] != 0)
                useweight2 = 1;
             else
                useweight2 = 0;
             term2->style = Byte((term2->style&0x7F) | (0x80 * useweight2));
             change->set(102);
         }

         // value text dialog (textdialog1)
         // font
         if( changecount > 1 && !changeIn.test(25) && dummyid1 == 0 )
             change->clear(25);
         else
         {
             font = ti1->font;
             text = ti1->text;
             *ti1 = dd.vinfo;
             ti1->font = font;
             ti1->text = text;
             ti1->options = dd.vinfo.options;

             RCCHAR *sc;
             if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid1]) + 1]) != NULL)
             {
                  delete [] ti1->font;
                  strcpy(sc,fonts[fontid1]);
                  ti1->font = sc;
             }
             change->set(25);
         }
         // height
         if( changecount > 1 && !changeIn.test(26) &&
             strlen(((RealDialogControl*)textdialog1.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(26);
         else
             change->set(26);
         // width
         if( changecount > 1 && !changeIn.test(27) &&
             strlen(((RealDialogControl*)textdialog1.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(27);
         else
             change->set(27);
         // angle
         if( changecount > 1 && !changeIn.test(28) &&
             strlen(((RealDialogControl*)textdialog1.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(28);
         else
             change->set(28);
         // vertspacing
         if( changecount > 1 && !changeIn.test(31) &&
             strlen(((RealDialogControl*)textdialog1.getcontrol(106))->getSpecialValueText()) != 0 )
             change->clear(31);
         else
             change->set(31);
         // text hjust
         if(changecount > 1 && !changeIn.test(29) && dummyHjust1 == 0) // not changed
             change->clear(29);
         else
         {
             dd.vinfo.options.set(1,hjust1 % 2);
             dd.vinfo.options.set(2,hjust1 / 2);
             change->set(29);
         }
         // text vjust
         if(changecount > 1 && !changeIn.test(30) && dummyVjust1 == 0) // not changed
             change->clear(30);
         else
         {
             dd.vinfo.options.set(3,vjust1 % 2);
             dd.vinfo.options.set(4,vjust1 / 2);
             change->set(30);
         }
         // placement
         if(changecount > 1 && !changeIn.test(77) && dummyPlacement == 0) // not changed
             change->clear(77);
         else
         {
             options->set(7,placement % 2);
             options->set(8,placement / 2);
             change->set(77);
         }
         // tristate switches
         // text bold
         if(tristates[11] == 1 && ((TriStateCheckBoxDialogControl*)textdialog1.getcontrol(107))->isTriState())
             change->clear(32);
         else
         {
             change->set(32);
             ti1->options.set(5, tristates[11]);
         }
         // text underline
         if(tristates[12] == 1 && ((TriStateCheckBoxDialogControl*)textdialog1.getcontrol(108))->isTriState())
             change->clear(33);
         else
         {
             change->set(33);
             ti1->options.set(6, tristates[12]);
         }
         // text italic
         if(tristates[13] == 1 && ((TriStateCheckBoxDialogControl*)textdialog1.getcontrol(109))->isTriState())
             change->clear(34);
         else
         {
             change->set(34);
             ti1->options.set(7, tristates[13]);
         }
         // text boxed
         if(tristates[14] == 1 && ((TriStateCheckBoxDialogControl*)textdialog1.getcontrol(110))->isTriState())
             change->clear(35);
         else
         {
             change->set(35);
             ti1->options.set(8, tristates[14]);
         }
         // text display value
         if(tristates[15] == 1 && ((TriStateCheckBoxDialogControl*)textdialog1.getcontrol(111))->isTriState())
             change->clear(36);
         else
         {
             change->set(36);
             options->set(2, tristates[15]);
         }

         // tolerance text dialog (textdialog2)
         // font
         if( changecount > 1 && !changeIn.test(80) && dummyid2 == 0 )
             change->clear(80);
         else
         {
             font = ti2->font;
             text = ti2->text;
             *ti2 = dd.tinfo;
             ti2->font = font;
             ti2->text = text;
             ti2->options = dd.tinfo.options;

             RCCHAR *sc;
             if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid2]) + 1]) != NULL)
             {
                  delete [] ti2->font;
                  strcpy(sc,fonts[fontid2]);
                  ti2->font = sc;
             }
             change->set(80);
         }
         // height
         if( changecount > 1 && !changeIn.test(81) &&
             strlen(((RealDialogControl*)textdialog2.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(81);
         else
             change->set(81);
         // width
         if( changecount > 1 && !changeIn.test(82) &&
             strlen(((RealDialogControl*)textdialog2.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(82);
         else
             change->set(82);
         // angle
         if( changecount > 1 && !changeIn.test(83) &&
             strlen(((RealDialogControl*)textdialog2.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(83);
         else
             change->set(83);
         // vertspacing
         if( changecount > 1 && !changeIn.test(86) &&
             strlen(((RealDialogControl*)textdialog2.getcontrol(106))->getSpecialValueText()) != 0 )
             change->clear(86);
         else
             change->set(86);
         // text hjust
         if(changecount > 1 && !changeIn.test(84) && dummyHjust2 == 0) // not changed
             change->clear(84);
         else
         {
             dd.tinfo.options.set(1,hjust2 % 2);
             dd.tinfo.options.set(2,hjust2 / 2);
             change->set(84);
         }
         // text vjust
         if(changecount > 1 && !changeIn.test(85) && dummyVjust2 == 0) // not changed
             change->clear(85);
         else
         {
             dd.tinfo.options.set(3,vjust2 % 2);
             dd.tinfo.options.set(4,vjust2 / 2);
             change->set(85);
         }
         // tristate switches
         // text bold
         if(tristates[16] == 1 && ((TriStateCheckBoxDialogControl*)textdialog2.getcontrol(107))->isTriState())
             change->clear(87);
         else
         {
             change->set(87);
             ti2->options.set(5, tristates[16]);
         }
         // text underline
         if(tristates[17] == 1 && ((TriStateCheckBoxDialogControl*)textdialog2.getcontrol(108))->isTriState())
             change->clear(88);
         else
         {
             change->set(88);
             ti2->options.set(6, tristates[17]);
         }
         // text italic
         if(tristates[18] == 1 && ((TriStateCheckBoxDialogControl*)textdialog2.getcontrol(109))->isTriState())
             change->clear(89);
         else
         {
             change->set(89);
             ti2->options.set(7, tristates[18]);
         }
         // text boxed
         if(tristates[19] == 1 && ((TriStateCheckBoxDialogControl*)textdialog2.getcontrol(110))->isTriState())
             change->clear(90);
         else
         {
             change->set(90);
             ti2->options.set(8, tristates[19]);
         }

         // prefix text dialog (textdialog3)
         // font
         if( changecount > 1 && !changeIn.test(40) && dummyid3 == 0 )
             change->clear(40);
         else
         {
             // don't copy the text here it has a different switch
             font = ti3->font;
             text = ti3->text;
             *ti3 = dd.pinfo;
             ti3->font = font;
             ti3->text = text;
             ti3->options = dd.pinfo.options;

             RCCHAR *sc;
             if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid3]) + 1]) != NULL)
             {
                  delete [] ti3->font;
                  strcpy(sc,fonts[fontid3]);
                  ti3->font = sc;
             }
             change->set(40);
         }
         // height
         if( changecount > 1 && !changeIn.test(41) &&
             strlen(((RealDialogControl*)textdialog3.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(41);
         else
             change->set(41);
         // width
         if( changecount > 1 && !changeIn.test(42) &&
             strlen(((RealDialogControl*)textdialog3.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(42);
         else
             change->set(42);
         // angle
         if( changecount > 1 && !changeIn.test(43) &&
             strlen(((RealDialogControl*)textdialog3.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(43);
         else
             change->set(43);
         // vertspacing
         if( changecount > 1 && !changeIn.test(46) &&
             strlen(((RealDialogControl*)textdialog3.getcontrol(106))->getSpecialValueText()) != 0 )
             change->clear(46);
         else
             change->set(46);
         // text hjust
         if(changecount > 1 && !changeIn.test(44) && dummyHjust3 == 0) // not changed
             change->clear(44);
         else
         {
             dd.vinfo.options.set(1,hjust3 % 2);
             dd.vinfo.options.set(2,hjust3 / 2);
             change->set(84);
         }
         // text vjust
         if(changecount > 1 && !changeIn.test(45) && dummyVjust3 == 0) // not changed
             change->clear(45);
         else
         {
             dd.vinfo.options.set(3,vjust3 % 2);
             dd.vinfo.options.set(4,vjust3 / 2);
             change->set(45);
         }
         // tristate switches
         // text bold
         if(tristates[20] == 1 && ((TriStateCheckBoxDialogControl*)textdialog3.getcontrol(107))->isTriState())
             change->clear(47);
         else
         {
             change->set(47);
             ti3->options.set(5, tristates[20]);
         }
         // text underline
         if(tristates[21] == 1 && ((TriStateCheckBoxDialogControl*)textdialog3.getcontrol(108))->isTriState())
             change->clear(48);
         else
         {
             change->set(48);
             ti3->options.set(6, tristates[21]);
         }
         // text italic
         if(tristates[22] == 1 && ((TriStateCheckBoxDialogControl*)textdialog3.getcontrol(109))->isTriState())
             change->clear(49);
         else
         {
             change->set(49);
             ti3->options.set(7, tristates[22]);
         }
         // text boxed
         if(tristates[23] == 1 && ((TriStateCheckBoxDialogControl*)textdialog3.getcontrol(110))->isTriState())
             change->clear(50);
         else
         {
             change->set(50);
             ti3->options.set(8, tristates[23]);
         }

         // suffix text dialog (textdialog4)
         // font
         if( changecount > 1 && !changeIn.test(55) && dummyid3 == 0 )
             change->clear(55);
         else
         {
             // don't copy the text here it has a different switch
             font = ti4->font;
             text = ti4->text;
             *ti4 = dd.sinfo;
             ti4->font = font;
             ti4->text = text;
             ti4->options = dd.sinfo.options;

             RCCHAR *sc;
             if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid4]) + 1]) != NULL)
             {
                  delete [] ti4->font;
                  strcpy(sc,fonts[fontid4]);
                  ti4->font = sc;
             }
             change->set(55);
         }
         // height
         if( changecount > 1 && !changeIn.test(56) &&
             strlen(((RealDialogControl*)textdialog4.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(56);
         else
             change->set(56);
         // width
         if( changecount > 1 && !changeIn.test(57) &&
             strlen(((RealDialogControl*)textdialog4.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(57);
         else
             change->set(57);
         // angle
         if( changecount > 1 && !changeIn.test(58) &&
             strlen(((RealDialogControl*)textdialog4.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(58);
         else
             change->set(58);
         // vertspacing
         if( changecount > 1 && !changeIn.test(61) &&
             strlen(((RealDialogControl*)textdialog4.getcontrol(106))->getSpecialValueText()) != 0 )
             change->clear(61);
         else
             change->set(61);
         // text hjust
         if(changecount > 1 && !changeIn.test(59) && dummyHjust4 == 0) // not changed
             change->clear(59);
         else
         {
             dd.sinfo.options.set(1,hjust3 % 2);
             dd.sinfo.options.set(2,hjust3 / 2);
             change->set(59);
         }
         // text vjust
         if(changecount > 1 && !changeIn.test(60) && dummyVjust4 == 0) // not changed
             change->clear(60);
         else
         {
             dd.sinfo.options.set(3,vjust3 % 2);
             dd.sinfo.options.set(4,vjust3 / 2);
             change->set(60);
         }
         // tristate switches
         // text bold
         if(tristates[25] == 1 && ((TriStateCheckBoxDialogControl*)textdialog4.getcontrol(107))->isTriState())
             change->clear(62);
         else
         {
             change->set(62);
             ti3->options.set(5, tristates[25]);
         }
         // text underline
         if(tristates[26] == 1 && ((TriStateCheckBoxDialogControl*)textdialog4.getcontrol(108))->isTriState())
             change->clear(63);
         else
         {
             change->set(63);
             ti3->options.set(6, tristates[26]);
         }
         // text italic
         if(tristates[27] == 1 && ((TriStateCheckBoxDialogControl*)textdialog4.getcontrol(109))->isTriState())
             change->clear(64);
         else
         {
             change->set(64);
             ti3->options.set(7, tristates[27]);
         }
         // text boxed
         if(tristates[28] == 1 && ((TriStateCheckBoxDialogControl*)textdialog4.getcontrol(110))->isTriState())
             change->clear(65);
         else
         {
             change->set(65);
             ti3->options.set(8, tristates[28]);
         }

         // prefix 0 options (entrydialog1)
         // text
         if( changecount >= 1  && dd.usePrefix1 == 0/*strlen(prefix) == 0*/)
             change->clear(71);
         else
         {
             // if changes all selected items will get the same text
             text = ti3->text;
             RCCHAR *sc;
             if ((sc = new RCCHAR[strlen(prefix) + 1]) != NULL)
             {
                 strcpy(sc,prefix);
                 delete [] ti3->text;
                 ti3->text = sc;
             }
             change->set(71);
         }
         // tristate check box use value text style
         if(tristates[24] == 1 && ((TriStateCheckBoxDialogControl*)entrydialog1.getcontrol(100))->isTriState())
             change->clear(70);
         else
         {
             change->set(70);
             options->set(3, tristates[24]);
         }

         // suffix 0 options (entrydialog2)
         // text
         if( changecount >= 1  && dd.useSuffix1 == 0/*strlen(prefix) == 0*/)
             change->clear(76);
         else
         {
             // if changes all selected items will get the same text
             text = ti4->text;
             RCCHAR *sc;
             if ((sc = new RCCHAR[strlen(suffix) + 1]) != NULL)
             {
                 strcpy(sc,suffix);
                 delete [] ti4->text;
                 ti4->text = sc;
             }
             change->set(76);
         }
         // tristate check box use value text style
         if(tristates[29] == 1 && ((TriStateCheckBoxDialogControl*)entrydialog1.getcontrol(100))->isTriState())
             change->clear(75);
         else
         {
             change->set(75);
             options->set(4, tristates[29]);
         }

     }
     else
     {
          RCCHAR *s;
          font = ti1->font;  text = ti1->text;  *ti1 = dd.vinfo;  ti1->font = font;  ti1->text = text;
          if ((s = new RCCHAR[strlen(fonts[fontid1]) + 1]) != NULL)
            {  delete [] ti1->font;  strcpy(s,fonts[fontid1]);  ti1->font = s;
            }
          ti1->options = dd.vinfo.options;
          font = ti2->font;  text = ti2->text;  *ti2 = dd.tinfo;
          ti2->font = font;  ti2->text = text;
          if ((s = new RCCHAR[strlen(fonts[fontid2]) + 1]) != NULL)
            {  delete [] ti2->font;  strcpy(s,fonts[fontid2]);  ti2->font = s;
            }
          if ((s = new RCCHAR[strlen(dd.tinfo.text) + 1]) != NULL)
            {  strcpy(s,dd.tinfo.text);  delete [] ti2->text;  ti2->text = s;
            }
          ti2->options = dd.tinfo.options;
          font = ti3->font;  text = ti3->text;  *ti3 = dd.pinfo;
          ti3->font = font;  ti3->text = text;
          if ((s = new RCCHAR[strlen(fonts[fontid3]) + 1]) != NULL)
            {  delete [] ti3->font;  strcpy(s,fonts[fontid3]);  ti3->font = s;
            }
          if ((s = new RCCHAR[strlen(dd.pinfo.text) + 1]) != NULL)
            {  strcpy(s,dd.pinfo.text);  delete [] ti3->text;  ti3->text = s;
            }
          ti3->options = dd.pinfo.options;
          font = ti4->font;  text = ti4->text;  *ti4 = dd.sinfo;
          ti4->font = font;  ti4->text = text;
          if ((s = new RCCHAR[strlen(fonts[fontid4]) + 1]) != NULL)
            {  delete [] ti4->font;  strcpy(s,fonts[fontid4]);  ti4->font = s;
            }
          if ((s = new RCCHAR[strlen(dd.sinfo.text) + 1]) != NULL)
            {  strcpy(s,dd.sinfo.text);  delete [] ti4->text;  ti4->text = s;
            }

          //  ToDo: fixup for change stuff
          if(header != 0)
          {
              header->setcolour(c);
              header->setlayer(l);
              header->setstyle(st);
              header->setweight(db.lineweights.mmtoweight(w));
          }

          ti4->options = dd.sinfo.options;
          dd.vinfo.options.set(1,hjust1 % 2);
          dd.vinfo.options.set(2,hjust1 / 2);
          dd.vinfo.options.set(3,vjust1 % 2);
          dd.vinfo.options.set(4,vjust1 / 2);
          dd.pinfo.options.set(1,hjust2 % 2);
          dd.pinfo.options.set(2,hjust2 / 2);
          dd.pinfo.options.set(3,vjust2 % 2);
          dd.pinfo.options.set(4,vjust2 / 2);
          dd.sinfo.options.set(1,hjust3 % 2);
          dd.sinfo.options.set(2,hjust3 / 2);
          dd.sinfo.options.set(3,vjust3 % 2);
          dd.sinfo.options.set(4,vjust3 / 2);
          //term1->style = Byte(style1 | (0x80 * useweight1));
          term1->style = Byte(style1 | (0x80 * tristates[9]));
          term1->height = dd.term1.height;
          term1->length = dd.term1.length;
          term1->weight = dd.term1.weight;
          //term2->style = Byte(style2| (0x80 * useweight2));
          term2->style = Byte(style2| (0x80 * tristates[10]));
          term2->height = dd.term2.height;
          term2->length = dd.term2.length;
          term2->weight = dd.term2.weight;
          options->set(5,tolerance % 2);
          options->set(6,tolerance / 2);
          options->set(7,placement % 2);
          options->set(8,placement / 2);
          options->set(9,position);
          options->set(10,visible);

          ti1->options.set(5, tristates[11]);
          ti1->options.set(6, tristates[12]);
          ti1->options.set(7, tristates[13]);
          ti1->options.set(8, tristates[14]);
          options->set(2, tristates[15]);

          ti2->options.set(5, tristates[16]);
          ti2->options.set(6, tristates[17]);
          ti2->options.set(7, tristates[18]);
          ti2->options.set(8, tristates[19]);

          ti3->options.set(5, tristates[20]);
          ti3->options.set(6, tristates[21]);
          ti3->options.set(7, tristates[22]);
          ti3->options.set(8, tristates[23]);          
          options->set(2,tristates[24]);// prefix uses value style

          options->set(0,tristates[0]);
          options->set(1,tristates[1]);
          options->set(ToleranceDefined,tristates[2]);
          options->set(GapFromDimension,tristates[3]);
          options->set(ScaleText,tristates[4]);
          options->set(FixedDimensionPosition,tristates[5]);
          options->set(DimensionReverse,tristates[6]);
          options->set(HideLeftDimensionLine,tristates[7]);
          options->set(HideRightDimensionLine,tristates[8]);
     }
 }
 options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
 options->toggle(ScaleText);
 delete [] termnames;
 for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
 delete [] fonts;
 delete [] charsets;

 delete [] defvinfo.font; delete [] defvinfo.text;
 delete [] deftinfo.font; delete [] deftinfo.text;
 delete [] defpinfo.font; delete [] defpinfo.text;
 delete [] defsinfo.font; delete [] defsinfo.text;

 return status;
}

/*
// this is declared in ncdialog.h
class LeaderDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     LeaderDisplayDialogControl(int id,DimensionDefaults *d,RCCHAR **fs,Byte *cs) : DisplayDialogControl(id) {  dd = d;  fonts = fs;  charsets = cs; }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void LeaderDisplayDialogControl::draw(Dialog *dialog,RCHDC hdc)
{int hjust,vjust,style,fontid,halocolour,useweight;
 double weight;
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 View3dOutput output(hWnd,hdc,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 BitMask cdstyle(32);
 static RCCHAR text[20];
 double height,width,vertspacing,angle,w;

 int nfonts,defFontid;
 //RCCHAR **fonts=0;
 //Byte *charsets=0;

 RCCHAR defFont[300],*df;
 double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale;
 int defprecision,deftolprecision;
 TerminatorInfo defterm1,defterm2;
 TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
 BYTE defHalocolour;
 BitMask defOptions(32);

 int c,l,s;
 c=db.header.getcolour();
 l=(int)db.header.getlayer();
 s=(int)db.header.getstyle();
 w=db.lineweights.weighttomm(db.header.getweight());
 int tristates[] = { 1,1,1,1,1,1,1,1 };

 if(hWnd == 0)
     return;

 QGraphicsView *ctl = 0;
 if(dialog->gethdlg())
   ctl = (QGraphicsView*)((Leader_Dialog*)dialog->gethdlg())->getControl(113);
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

       //rect.left = 0;  rect.top = 0;
       //rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());

       RCCHAR *cs;
       LinearDimension::getleaderdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
       if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return;
       RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
       if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return;
       RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
       if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return;
       RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
       if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return;
       RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
       if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return;
       RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
       if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return;
       RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
       if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return;
       RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
       if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return;
       RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

       strcpy(defFont,defpinfo.font);

  if (
      dialog->currentvalue(100,&c ) &&
      dialog->currentvalue(101,&l ) &&
      dialog->currentvalue(102,&s ) &&
      dialog->currentvalue(103,&w ) &&
      dialog->currentvalue(1033,&fontid) &&
      dialog->currentvalue(105,&dd->pinfo.height) &&
      dialog->currentvalue(106,&dd->pinfo.width) &&
      dialog->currentvalue(109,&dd->pinfo.angle) &&
      dialog->currentvalue(104,&hjust) &&
      dialog->currentvalue(108,&vjust) &&
      dialog->currentvalue(107,&dd->pinfo.vertspacing) &&
      dialog->currentvalue(110,&tristates[0]) &&
      dialog->currentvalue(111,&tristates[1]) &&
      dialog->currentvalue(114,&tristates[2]) &&
      dialog->currentvalue(115,&tristates[3]) &&
      dialog->currentvalue(112,&tristates[4]) &&
      dialog->currentvalue(116,&tristates[5]) &&
      dialog->currentvalue(117,&style) &&
      dialog->currentvalue(119,&tristates[6]) &&
      dialog->currentvalue(124,&weight) &&
      dialog->currentvalue(118,&dd->term1.length) &&
      dialog->currentvalue(125,&halocolour) &&
      dialog->currentvalue(120,&dd->term1.height))
    {
      if( strlen(((FontListDialogControl*)((Leader_Dialog*)dialog->gethdlg())->getControl(1033))->getSpecialValueText()) != 0 &&
         ((QComboBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(2033))->isVisible() )
          dd->pinfo.font = defpinfo.font;
      else
          dd->pinfo.font = fonts[fontid];

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(105))->specialValueText().isEmpty() &&
         dd->pinfo.height == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(105))->minimum() )
          dd->pinfo.height = defpinfo.height;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
         dd->pinfo.width == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
          dd->pinfo.width = defpinfo.width;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(107))->specialValueText().isEmpty() &&
         dd->pinfo.vertspacing == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(107))->minimum() )
          dd->pinfo.vertspacing = defpinfo.vertspacing;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(109))->specialValueText().isEmpty() &&
         dd->pinfo.angle == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(109))->minimum() )
          dd->pinfo.angle = defpinfo.angle;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(124))->specialValueText().isEmpty() &&
         weight == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(124))->minimum() )
          dd->term1.weight = defterm1.weight;
      else
          dd->term1.weight = db.lineweights.mmtoweight(weight);

      if(!((QSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(125))->specialValueText().isEmpty() &&
         dd->pinfo.halocolour == ((QSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(125))->minimum() )
          dd->pinfo.halocolour = defpinfo.halocolour;
      else
          dd->pinfo.halocolour = (Byte)halocolour;

      if(((QComboBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(217))->isVisible())
          style = defterm1.style&0x7F;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(118))->specialValueText().isEmpty() &&
         dd->term1.length == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(118))->minimum() )
          dd->term1.length = defterm1.length;

      if(!((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(120))->specialValueText().isEmpty() &&
         dd->term1.height == ((QDoubleSpinBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(120))->minimum() )
          dd->term1.height = defterm1.height;

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(110))->isTristate())
          tristates[0] == 1 ? dd->pinfo.options.set(5,defOptions.test(5)) : dd->pinfo.options.set(5,tristates[0]);
      else
          dd->pinfo.options.set(5,tristates[0]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(111))->isTristate())
          tristates[1] == 1 ? dd->pinfo.options.set(6,defOptions.test(6)) : dd->pinfo.options.set(6,tristates[1]);
      else
          dd->pinfo.options.set(6,tristates[1]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(114))->isTristate())
          tristates[2] == 1 ? dd->pinfo.options.set(7,defOptions.test(7)) : dd->pinfo.options.set(7,tristates[2]);
      else
          dd->pinfo.options.set(7,tristates[2]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(115))->isTristate())
          tristates[3] == 1 ? dd->pinfo.options.set(8,defOptions.test(8)) : dd->pinfo.options.set(8,tristates[3]);
      else
          dd->pinfo.options.set(8,tristates[3]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(112))->isTristate())
          tristates[4] == 1 ? dd->pinfo.options.set(10,defOptions.test(10)) : dd->pinfo.options.set(10,tristates[4]);
      else
          dd->pinfo.options.set(10,tristates[4]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(116))->isTristate())
          tristates[5] == 1 ? dd->pinfo.options.set(12,defOptions.test(12)) : dd->pinfo.options.set(12,tristates[5]);
      else
          dd->pinfo.options.set(12,tristates[5]);

      if(((QCheckBox*)((Leader_Dialog*)dialog->gethdlg())->getControl(119))->isTristate())
          tristates[6] == 2 ? useweight=1 : useweight=0;
      else
      {
          if( tristates[6] != 0 )
            useweight = 1;
          else
            useweight = 0;
      }

       //dd->pinfo.options.set(5,tristates[0]);
       //dd->pinfo.options.set(6,tristates[1]);
       //dd->pinfo.options.set(7,tristates[2]);
       //dd->pinfo.options.set(8,tristates[3]);
       //dd->pinfo.options.set(10,tristates[4]);
       //dd->pinfo.options.set(12,tristates[5]);

       //dd->pinfo.options.set(0,charsets[fontid]/* == SYMBOL_CHARSET*/);
       dd->pinfo.options.set(1,hjust % 2);
       dd->pinfo.options.set(2,hjust / 2);
       dd->pinfo.options.set(3,vjust % 2);
       dd->pinfo.options.set(4,vjust / 2);
       if(dd->pinfo.vertspacing != 0.0)
           dd->pinfo.text = RCSTRCPY(text,_RCT("Leader\nOptions"));
       else
           dd->pinfo.text = RCSTRCPY(text,_RCT("Leader Options"));

       dd->term1.style = style | (0x80 * useweight);

       cdstyle.clearall();
       cdstyle.set(2);
       output.setplotscale(1.0);

       Point p1,p2;
       p1.x = output.getwidth() / 4.0;
       p1.y = output.getheight() / 2.0;
       p2.x = output.getwidth() * 3.0 / 8.0;
       p2.y = output.getheight() / 2.0;

       p1 = output.screentomodel(p1);
       p2 = output.screentomodel(p2);

       EntityHeader header(c,l,s,db.lineweights.mmtoweight(w));
       CircularDimension cd(header,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),p1/*Point(15.0,15.0,0.0)*/,
                            p2/*Point(30.0,15.0,0.0)*/,cdstyle,0,dd->extension,dd->gap,dd->stackdis,dd->uppertol,dd->lowertol,dd->scale,dd->textyoffset,dd->precision,dd->tolprecision,dd->term1,dd->term2,dd->vinfo,dd->tinfo,dd->pinfo,dd->sinfo,dd->options);

       cd.draw(DM_NORMAL,NULL,&output);
    }

  delete [] defvinfo.text;  delete [] defvinfo.font;
  delete [] defpinfo.text;  delete [] defpinfo.font;
  delete [] defsinfo.text;  delete [] defsinfo.font;
  delete [] deftinfo.text;  delete [] deftinfo.font;

 }
}

int Dimension::setupleader(BitMask *change, double *extension, double *gap, double *stackdis, double *uppertol, double *lowertol, double *scale, int *precision, int *tolprecision, TerminatorInfo *term1, TerminatorInfo *term2,
                     TextInfo *ti1, TextInfo *ti2, TextInfo *ti3, TextInfo *ti4, BitMask *options, EntityHeader *header)
{
 Dialog dialog("Leader_Dialog");
 DialogControl *dc;
 int i,c,l,s,style1,hjust,vjust,status,nterms,nfonts,fontid,halocolour,useweight1;
 double w,weight1;
 Byte *charsets=0;
 RCCHAR **termnames=0,**dummytermnames=0,**fonts=0;
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
 ResourceString rs13(NCDIMENA+13),rs14(NCDIMENA+14),rs15(NCDIMENA+15);
 ResourceString rs16(NCDIMENA+16);
 ResourceString rs40(NCDIMENA+40),rs41(NCDIMENA+41),rs42(NCDIMENA+42);
 ResourceString rs43(NCDIMENA+43),rs44(NCDIMENA+44),rs45(NCDIMENA+45),rs46(NCDIMENA+46);
 ResourceString rsVaries(4107),rsDefault(4108),rsOther(4109);
 RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *hjustlv[] = {  rsVaries.getws(),rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 RCCHAR *vjustlv[] = {  rsVaries.getws(),rs10.getws(),rs11.getws(),rs12.getws()  };
 TerminatorInfo terminfo;
 ResourceString rs17(NCDIMENA+17);
 DimensionDefaults dd;
 //
 int dummyHjust,dummyVjust;
 RCCHAR *dummyfonts[] = { rsVaries.getws(),rsDefault.getws(),rsOther.getws() };
 int dummyid=0,dummyStyle1=0;
 RCCHAR defFont[300];
 double defWeight;
 int defColour,defLayer,defStyle;
 double defextension,defgap,defstackdis,defuppertol,deflowertol,defscale;
 int defprecision,deftolprecision;
 TerminatorInfo defterm1,defterm2;
 TextInfo defvinfo,deftinfo,defpinfo,defsinfo;
 BitMask defOptions(32),changeIn(120);
 int tristates[] = { 1,1,1,1,1,1,1,1 };

 dd.extension = *extension;  dd.gap = *gap;  dd.stackdis = *stackdis;  dd.scale = *scale;
 dd.uppertol = *uppertol;  dd.lowertol = *lowertol;
 dd.precision = *precision;  dd.tolprecision = *tolprecision;
 dd.term1 = *term1;  dd.term2 = *term2;
 dd.vinfo = *ti1;  dd.tinfo = *ti2;  dd.pinfo = *ti3;  dd.sinfo = *ti4;  dd.options = *options;
 dd.vinfo.options = ti1->options;  dd.tinfo.options = ti2->options; dd.pinfo.options = ti3->options;  dd.sinfo.options = ti4->options;

 options->toggle(ScaleText);         //  Negative logic for scale text - Default is scaled

 if (! Text::getfonts(ti3->font,&nfonts,&fonts,&charsets,&fontid))
 {
      options->toggle(ScaleText);
      return 0;
 }
 if (fonts == NULL)
 {
      options->toggle(ScaleText);
      return 0;
 }

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
 RCCHAR *cs;
 LinearDimension::getleaderdefaults(&defextension,&defgap,&defstackdis,&defuppertol,&deflowertol,&defscale,&defprecision,&deftolprecision,&defterm1,&defterm2,&defvinfo,&deftinfo,&defpinfo,&defsinfo,&defOptions);
 if ((cs = new RCCHAR[RCSTRLEN(defvinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defvinfo.font);  defvinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(deftinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,deftinfo.font);  deftinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defpinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defpinfo.font);  defpinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defsinfo.font) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defsinfo.font);  defsinfo.font = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defvinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defvinfo.text);  defvinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(deftinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,deftinfo.text);  deftinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defpinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defpinfo.text);  defpinfo.text = cs;
 if ((cs = new RCCHAR[RCSTRLEN(defsinfo.text) + 1]) == NULL) return 0;
 RCSTRCPY(cs,defsinfo.text);  defsinfo.text = cs;

 strcpy(defFont,ti3->font);
 strcat(dummyfonts[1],ti3->font);

 dialog.add((DisplayDialogControl *) new LeaderDisplayDialogControl(113,&dd,fonts,charsets));
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++);
 nterms = terminfo.style;
 if ((termnames = new RCCHAR *[nterms]) == NULL)
 {
      options->toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
      options->toggle(ScaleText);
      return 0;
 }
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++)
   termnames[terminfo.style] = LineTerminator::name(terminfo);

 dummytermnames = new RCCHAR *[nterms+1];
 dummytermnames[0] = rsVaries.getws();
 for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++)
   dummytermnames[terminfo.style+1] = LineTerminator::name(terminfo);

 style1 = term1->style & 0x7F;
 useweight1 = (term1->style & 0x80) != 0;
 weight1 = db.lineweights.weighttomm(term1->weight);
 halocolour = ti3->halocolour;

 hjust = ti3->options.test(1) + ti3->options.test(2) * 2;
 vjust = ti3->options.test(3) + ti3->options.test(4) * 2;

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
     if(!change->test(0)) // colour
         c = 0;
     if(!change->test(1)) // layer
         l = 0;
     if(!change->test(2)) // style
         s = 0;
     if(!change->test(3)) // weight
         w = 0.10;

     if(!change->test(40)) // font
         strcpy(ti3->font,defFont);
     if(!change->test(41)) // text height
         ti3->height = 0.0;
     if(!change->test(42)) // text width
         ti3->width = -10000.0;
     if(!change->test(43)) // text angle
         ti3->angle = -360.0;
     if(!change->test(46)) // text vertspacing
         ti3->vertspacing = -100000000.0;
     if(!change->test(98)) // text halo colour
         halocolour = 0;
         //ti3->halocolour = 0;
     if(!change->test(44))
     {
         hjust = 0;
         dummyHjust = 0;
     }
     else
     {
         hjust = ti3->options.test(1) + ti3->options.test(2) * 2;
         dummyHjust = 0;
     }
     if(!change->test(45))
     {
         vjust = 0;
         dummyVjust = 0;
     }
     else
     {
         vjust = ti3->options.test(3) + ti3->options.test(4) * 2;
         dummyVjust = 0;
     }
     // terminator settings
     if(!change->test(16)) // term length
         term1->length = 1.0E-10;
     if(!change->test(17)) // term height
         term1->height = 1.0E-10;
     if(!change->test(101)) // term weight
         weight1 = 0.01;
     // terminator style
     if(!change->test(15))
     {
         style1 = 0;
         dummyStyle1 = 0;
     }
     else
     {
         style1 = (term1->style & 0x7F);
         dummyStyle1 = 0;
     }
     if(change->test(47)) // bold
         ti3->options.test(5)==0  ? tristates[0] = 0 : tristates[0] = 2;
     if(change->test(48)) // underline
         ti3->options.test(6)==0  ? tristates[1] = 0 : tristates[1] = 2;
     if(change->test(49)) // italic
         ti3->options.test(7)==0  ? tristates[2] = 0 : tristates[2] = 2;
     if(change->test(50)) // boxed
         ti3->options.test(8)==0  ? tristates[3] = 0 : tristates[3] = 2;
     if(change->test(96)) // outlined
         ti3->options.test(10)==0 ? tristates[4] = 0 : tristates[4] = 2;
     if(change->test(97)) // halo
         ti3->options.test(12)==0  ? tristates[5] = 0 : tristates[5] = 2;
     if(change->test(94)) // scaled text
         options->test(14)==0 ? tristates[7] = 0 : tristates[7] = 2;
     if(change->test(100)) // term use term weight
         (term1->style&0x80)==0 ? tristates[6] = 0 : tristates[6] = 2;
 }
 else
 {
     dummyHjust = 0;
     dummyVjust = 0;
     hjust = options->test(1) + ti3->options.test(2) * 2;
     vjust = options->test(3) + ti3->options.test(4) * 2;
     ti3->options.test(5)==0  ? tristates[0] = 0 : tristates[0] = 1;
     ti3->options.test(6)==0  ? tristates[1] = 0 : tristates[1] = 1;
     ti3->options.test(7)==0  ? tristates[2] = 0 : tristates[2] = 1;
     ti3->options.test(8)==0  ? tristates[3] = 0 : tristates[3] = 1;
     ti3->options.test(10)==0 ? tristates[4] = 0 : tristates[4] = 1;
     ti3->options.test(12)==0 ? tristates[5] = 0 : tristates[5] = 1;
     options->test(14)==0 ? tristates[7] = 0 : tristates[7] = 1;
     (term1->style&0x80)==0  ? tristates[6] = 0 : tristates[6] = 1;
 }

 dialog.add(new ListDialogControl(117,nterms,termnames,&style1));
 dialog.add(new ListDialogControl(217,nterms+1,dummytermnames,&dummyStyle1));
 dialog.add(dc = new RealDialogControl(118,&term1->length,1.0,0.0,1.0E38,ScaleByLength));
 //dialog.add(dc = new RealDialogControl(118,&term1->length,1.0,1.0E-10,1.0E38,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1117,dc));
 dialog.add(dc = new RealDialogControl(120,&term1->height,1.0,0.0,1.0E38,ScaleByLength));
 //dialog.add(dc = new RealDialogControl(120,&term1->height,1.0,1.0E-10,1.0E38,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1118,dc));
 //dialog.add(new ButtonDialogControl(114));// KMJ: what is this for??
 dialog.add(dc = new IntegerDialogControl(125,&halocolour,0,255));// halo colour
 //dialog.add(new ScrollBarDialogControl(1116,dc));
 dialog.add(new DTButtonDialogControl(1004)); // halo colour selector button
 dialog.add(new FontListDialogControl(1033,nfonts,fonts,&fontid));
 dialog.add(new ListDialogControl(2033,2,dummyfonts,&dummyid));
 dialog.add(dc = new RealDialogControl(105,&ti3->height,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(dc = new RealDialogControl(106,&ti3->width,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1102,dc));
 dialog.add(dc = new RealDialogControl(109,&ti3->angle));
 //dialog.add(new ScrollBarDialogControl(1103,dc));
 dialog.add(new ListDialogControl(104,3,hjustl,&hjust));
 dialog.add(new ListDialogControl(204,4,hjustlv,&dummyHjust));
 dialog.add(new ListDialogControl(108,3,vjustl,&vjust));
 dialog.add(new ListDialogControl(208,4,vjustlv,&dummyVjust));
 dialog.add(dc = new RealDialogControl(107,&ti3->vertspacing,0.1));
 //dialog.add(new ScrollBarDialogControl(1106,dc));
 //dialog.add(new CheckBoxDialogControl(110,&ti3->options,5));// bold
 //dialog.add(new CheckBoxDialogControl(111,&ti3->options,6));// underline
 //dialog.add(new CheckBoxDialogControl(114,&ti3->options,7));// italic
 //dialog.add(new CheckBoxDialogControl(115,&ti3->options,8));// boxed

 //dialog.add(new CheckBoxDialogControl(119,&useweight1));// specify terminator weight
 dialog.add(dc = new WeightAssocRealDialogControl(124,&weight1));
 //dialog.add(new ScrollBarDialogControl(1120,dc));
 dialog.add(new DTButtonDialogControl(1005)); // termintator weight selector button
 
 //dialog.add(new CheckBoxDialogControl(112,&ti3->options,10));// outlined
 //dialog.add(new CheckBoxDialogControl(116,&ti3->options,12));// halo
 //dialog.add(new CheckBoxDialogControl(121,options,ScaleText));// scaled
 dialog.add(new ButtonDialogControl(122)); // clone button
 dialog.add(new ButtonDialogControl(123)); // clone all button

 // add the header buttons
 dialog.add(new DTButtonDialogControl(1000));
 dialog.add(new DTButtonDialogControl(1001));
 dialog.add(new DTButtonDialogControl(1002));
 dialog.add(new DTButtonDialogControl(1003));
 dialog.add(new DTButtonDialogControl(1004));
 //dialog.add(new DTButtonDialogControl(1005));

 if (change != NULL)
 {
      //change->clearall();
      // for debug change later
      //change->setall();
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

     if(changecount > 1 && !change->test(40)) // font
     {
         ((FontListDialogControl*)dialog.getcontrol(1033))->setSpecialValueText(_RCT("*Varies*"));
         ((FontListDialogControl*)dialog.getcontrol(1033))->setDefaultFont(defFont);
     }
     if(changecount > 1 && !change->test(41)) // height
     {
         ((RealDialogControl*)dialog.getcontrol(105))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(105))->setDefaultValue(defpinfo.height);
     }
     if(changecount > 1 && !change->test(42)) // width
     {
         ((RealDialogControl*)dialog.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(106))->setDefaultValue(defpinfo.width);
     }
     if(changecount > 1 && !change->test(43)) // angle
     {
         ((RealDialogControl*)dialog.getcontrol(109))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(109))->setDefaultValue(defpinfo.angle);
     }
     if(changecount > 1 && !change->test(46)) // vertspacing
     {
         ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defpinfo.vertspacing);
     }
     if(changecount > 1 && !change->test(98)) // halo colour
     {
         ((IntegerDialogControl*)dialog.getcontrol(125))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(125))->setDefaultValue((int)defpinfo.halocolour);
     }
     if(changecount > 1 && !change->test(16)) // term length
     {
         ((RealDialogControl*)dialog.getcontrol(118))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(118))->setDefaultValue((int)defterm1.length);
     }
     if(changecount > 1 && !change->test(17)) // term height
     {
         ((RealDialogControl*)dialog.getcontrol(120))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(120))->setDefaultValue((int)defterm1.height);
     }
     if(changecount > 1 && !change->test(101)) // term weight
     {
         ((WeightAssocRealDialogControl*)dialog.getcontrol(124))->setSpecialValueText(_RCT("*Varies*"));
         ((WeightAssocRealDialogControl*)dialog.getcontrol(124))->setDefaultValue(db.lineweights.weighttomm((int)defterm1.weight));
     }
     // use tristate check boxes depending on the value
     dialog.add(new TriStateCheckBoxDialogControl(110,&tristates[0],changecount>=1^change->test(47),defpinfo.options.test(5)/*,5*/));  // bold
     dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[1],changecount>=1^change->test(48),defpinfo.options.test(6)/*,6*/));  // underline
     dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[2],changecount>=1^change->test(49),defpinfo.options.test(7)/*,7*/));  // italic
     dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[3],changecount>=1^change->test(50),defpinfo.options.test(8)/*,8*/));  // boxed
     dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[4],changecount>=1^change->test(96),defpinfo.options.test(10)/*,10*/)); // outline
     dialog.add(new TriStateCheckBoxDialogControl(116,&tristates[5],changecount>=1^change->test(97),defpinfo.options.test(12)/*,12*/));  // halo
     dialog.add(new TriStateCheckBoxDialogControl(119,&tristates[6],changecount>=1^change->test(100),(term1->style&0x80)/* */)); // use term weight
     dialog.add(new TriStateCheckBoxDialogControl(121,&tristates[7],changecount>=1^change->test(94),options->test(14)/*,14*/)); // scaled
 }
 else
 {
     dialog.add(new TriStateCheckBoxDialogControl(110,&tristates[0],false,ti3->options.test(5)/*,5*/));  // bold
     dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[1],false,ti3->options.test(6)/*,6*/));  // underline
     dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[2],false,ti3->options.test(7)/*,7*/));  // italic
     dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[3],false,ti3->options.test(8)/*,8*/));  // boxed
     dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[4],false,ti3->options.test(10)/*,10*/)); // outline
     dialog.add(new TriStateCheckBoxDialogControl(116,&tristates[5],false,ti3->options.test(12)/*,12*/));  // halo
     dialog.add(new TriStateCheckBoxDialogControl(119,&tristates[6],false,(term1->style&0x80)/* */)); // use term weight
     dialog.add(new TriStateCheckBoxDialogControl(121,&tristates[7],false,options->test(14)/*,14*/)); // scaled
 }

 v.setinteger("op::change",0);
 v.setbm("op::changemask",defOptions);

 if(change != NULL)
 {
     v.setinteger("op::change",1);
     v.setbm("op::changemask",*change);

     ResourceString rs23(NCDIMENA+23);
     if(changecount == 1)
         dialog.title("Leader Verify");
     else
         dialog.title(rs23.gets());
 }

 if ((status = dialog.process()) == TRUE)
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
         }

         // header settings
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
             strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(3);
         else
         {
             change->set(3);
             header->setweight(db.lineweights.mmtoweight(w));
         }

         // text settings
         // font
         if( changecount > 1 && !changeIn.test(40) && dummyid == 0 )
             change->clear(40);
         else
         {
             RCCHAR *sc;
             if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid]) + 1]) != NULL)
            {
                  delete [] ti3->font;
                  strcpy(sc,fonts[fontid]);
                  ti3->font = sc;
             }
             change->set(40);
         }
         // text height
         if( changecount > 1 && !changeIn.test(41) &&
             strlen(((RealDialogControl*)dialog.getcontrol(105))->getSpecialValueText()) != 0 ) // not changed
             change->clear(41);
         else
             change->set(41);
         // text width
         if( changecount > 1 && !changeIn.test(42) &&
             strlen(((RealDialogControl*)dialog.getcontrol(106))->getSpecialValueText()) != 0 ) // not changed
             change->clear(42);
         else
             change->set(42);
         // text angle
         if( changecount > 1 && !changeIn.test(43) &&
             strlen(((RealDialogControl*)dialog.getcontrol(109))->getSpecialValueText()) != 0 ) // not changed
             change->clear(43);
         else
             change->set(43);
         // text vertspacing
         if( changecount > 1 && !changeIn.test(46) &&
             strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0 ) // not changed
             change->clear(46);
         else
             change->set(46);
         // text halo colour
         if( changecount > 1 && !changeIn.test(98) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(125))->getSpecialValueText()) != 0 ) // not changed
             change->clear(98);
         else
         {
             ti3->halocolour = (Byte)halocolour;
             change->set(98);
         }
         // text hjust
         if(changecount > 1 && !changeIn.test(44) && dummyHjust == 0) // not changed
             change->clear(44);
         else
         {
             ti3->options.set(1,hjust % 2);
             ti3->options.set(2,hjust / 2);
             change->set(44);
         }
         // text vjust
         if(changecount > 1 && !changeIn.test(45) && dummyVjust == 0) // not changed
             change->clear(45);
         else
         {
             ti3->options.set(3,vjust % 2);
             ti3->options.set(4,vjust / 2);
             change->set(45);
         }

         // terminator settings
         // term length
         if( changecount > 1 && !changeIn.test(16) &&
             strlen(((RealDialogControl*)dialog.getcontrol(118))->getSpecialValueText()) != 0 ) // not changed
             change->clear(16);
         else
             change->set(16);
         // term height
         if( changecount > 1 && !changeIn.test(17) &&
             strlen(((RealDialogControl*)dialog.getcontrol(120))->getSpecialValueText()) != 0 ) // not changed
             change->clear(17);
         else
             change->set(17);
         // term weight
         if( changecount > 1 && !changeIn.test(101) &&
             strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(124))->getSpecialValueText()) != 0 ) // not changed
             change->clear(101);
         else
         {
             term1->weight = db.lineweights.mmtoweight(weight1);
             change->set(101);
         }

         // other settings
         ti3->options.set(0,dd.pinfo.options.test(0));

         // tristate switches
         // if the user did not change a value the tristate == 1 and the default value is used
         // otherwise the tristate value is used to set these options
         // text bold
         if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(110))->isTriState())
             change->clear(47);
         else
         {
             change->set(47);
             ti3->options.set(5, tristates[0]);
         }
         // text underline
         if(tristates[1] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(111))->isTriState())
             change->clear(48);
         else
         {
             change->set(48);
             ti3->options.set(6, tristates[2]);
         }
         // text italic
         if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(114))->isTriState())
             change->clear(49);
         else
         {
             change->set(49);
             ti3->options.set(7, tristates[2]);
         }
         // text boxed
         if(tristates[3] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(115))->isTriState())
             change->clear(50);
         else
         {
             change->set(50);
             ti3->options.set(8, tristates[3]);
         }
         // text outline
         if(tristates[4] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(112))->isTriState())
             change->clear(96);
         else
         {
             change->set(96);
             ti3->options.set(10, tristates[4]);
         }
         // text halo
         if(tristates[5] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(112))->isTriState())
             change->clear(97);
         else
         {
             change->set(97);
             ti3->options.set(12, tristates[5]);
         }
         // text scaled
         if(tristates[7] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(121))->isTriState())
             change->clear(94);
         else
         {
             change->set(94);
             options->set(14, tristates[7]);
         }
         // term style
         if(changecount > 1 && !changeIn.test(15) && dummyStyle1 == 0) // not changed
             change->clear(15);
         else
         {
             term1->style = Byte((style1&0x7F) | (term1->style&0x80));
             change->set(15);
         }
         // use term weight
         if(tristates[6] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(119))->isTriState())
         {
             change->clear(100);
         }
         else
         {
             if(tristates[6] != 0)
                useweight1 = 1;
             else
                useweight1 = 0;
             term1->style = Byte((term1->style&0x7F) | (0x80 * useweight1));
             change->set(100);
         }
     }
     else
     {
          ti3->options.set(5, tristates[0]);
          ti3->options.set(6, tristates[1]);
          ti3->options.set(7, tristates[2]);
          ti3->options.set(8, tristates[3]);
          ti3->options.set(10,tristates[4]);
          ti3->options.set(12, tristates[5]);
          options->set(14,tristates[7]);

          RCCHAR *sc;
          if ((sc = new RCCHAR[RCSTRLEN(fonts[fontid]) + 1]) != NULL)
          {
               delete [] ti3->font;
               strcpy(sc,fonts[fontid]);
               ti3->font = sc;
          }

          //  ToDo: fixup for change stuff
          if(header != 0)
          {
              header->setcolour(c);
              header->setlayer(l);
              header->setstyle(s);
              header->setweight(db.lineweights.mmtoweight(w));
          }

          ti3->options.set(0,dd.pinfo.options.test(0));
          ti3->options.set(1,hjust % 2);
          ti3->options.set(2,hjust / 2);
          ti3->options.set(3,vjust % 2);
          ti3->options.set(4,vjust / 2);
          ti3->halocolour = (Byte)halocolour;
          term1->style = Byte(style1 | (0x80 * tristates[6]));
          term1->weight = db.lineweights.mmtoweight(weight1);
    }
 }
 else
 {
     if(change != NULL)
        change->clearall();
 }

 options->toggle(ScaleText);
 delete [] termnames;
 for (i = 0 ; i < nfonts ; i++) delete [] fonts[i];
 delete [] fonts;
 delete [] charsets;

 return status;

}

#ifdef _USING_QTCHARXXX
int Dimension::process_property(int action,int id,void *data, RCCHAR *text)
{BitMask change_mask(128),new_options(32);
 int i,result,cstate;
 double wt;

 RCSTRING horizontal_justification_l[]  = {  "Left", "Centre", "Right"  };
 RCSTRING vertical_justification_l[] = {  "Bottom", "Centre", "Top"  };
 RCSTRING terminator_style_l[] = {  "None",  "Arrow",  "Open arrow",  "Filled arrow",  "Dot",  "Filled Dot",  "Slash"  };
 RCSTRING tolerance_style_l[] = {  "None", "Plus/Minus",  "Dual",  "Upper/Lower"  };
 RCSTRING placement_style_l[] = {  "Centred", "To Left",  "To Right"  };

 TerminatorInfo termi;
 TextInfo texti;


    if (isa(circulardimension_entity) && ((CircularDimension *)this)->getcircle() == 0)
    {//  This is a leader - should the simplier leader settings

        if (action == PP_GETNUMPROPERTIES)
            id = 10000;

        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Entity");
                return 1;
            }
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Colour");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getcolour();
                cstate = 1001;
                colour_command(&cstate,0,(void **)&result);
                change_mask.set(0,1);
                if (result != ((Entity *)data)->getcolour())
                    ((Entity *)data)->change(change_mask,result,0,0,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%d",getcolour());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Layer");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getlayer();
                cstate = 1001;
                layer_command(&cstate,0,(void **)&result);
                change_mask.set(1,1);
                if (result != ((Entity *)data)->getlayer())
                    ((Entity *)data)->change(change_mask,0,result,0,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%d",getlayer());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Style");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getstyle();
                cstate = 1001;
                style_command(&cstate,0,(void **)&result);
                change_mask.set(2,1);
                if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%d",getstyle());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                change_mask.set(3,1);
                if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                    ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Leader");
                return 1;
            }
        }

        if (id-- == 0) 
        {  
			if (action == PP_CLICK) 
            {
				RCCHAR rctext[4096];
				Dialog dialog("LTextinsert_Dialog");
                strcpy(rctext,prefixinfo.text);
                dialog.add(new StringDialogControl(100,rctext,4096));
                if (dialog.process() == TRUE)
                {   change_mask.set(71,1);
                    texti.text = rctext;
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }   
                return 0;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text");
            else if (action == PP_GETVALUE)
                strcpy(text,prefixinfo.text);
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             RCCHAR **fonts;
             Byte *charsets;
             RCCHAR font[300];

                strcpy(text,"");             
                if (Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {  RCSTRCAT(text,_RCT("\t"));
                           RCSTRCAT(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,prefixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(40,1);
//#ifdef _USING_QTCHAR
//				RCCHAR rctext[300];
//               strcpy(rctext,text);
//                texti.font = rctext;
//#else
                texti.font = text;
//#endif
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {
				Expression e(text);
                double x;
                change_mask.set(41,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(42,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4f",prefixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(43,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&horizontal_justification_l[prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&horizontal_justification_l[i]) == 0)
                    {   change_mask.set(44,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&vertical_justification_l[prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&vertical_justification_l[i]) == 0)
                    {   change_mask.set(45,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4f",prefixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(46,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(47,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(48,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(49,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(50,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Terminator");
                return 2;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
                return 2;
            }   
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Style");
            else if (action == PP_GETVALUE)
                strcpy(text,&terminator_style_l[term1.style&0x7F]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 7 ; i++) 
                    if (strcmp(text,&terminator_style_l[i]) == 0)
                    {   change_mask.set(15,1);
                        termi.style = (term1.style & 0x80) | i;
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Length");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term1.length,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(16,1);
                if (e.evaluate(&x))
                {   termi.length = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }


        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term1.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(17,1);
                if (e.evaluate(&x))
                {   termi.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Specify Terminator Weight");
            else if (action == PP_GETVALUE)
                strcpy(text,(term1.style&0x80) != 0 ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(100,1);
                termi.style = (term1.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if ((term1.style&0x80) != 0 && id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(term1.weight);
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                termi.weight = db.lineweights.mmtoweight(wt);
                change_mask.set(101,1);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%.2lf",db.lineweights.weighttomm(term1.weight));
        }

        if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    
        return 0;

    }

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Entity");
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Colour");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Layer");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Style");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Dimension");
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Extension");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,extension,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(5,1);
            if (e.evaluate(&x))
                change(change_mask,*this,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    } 
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Gap from dimension line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(GapFromDimension) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(92,1);
            new_options.set(GapFromDimension,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Gap");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,gap,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(6,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Precision");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",precision);
        else if (action == PP_SETVALUE)
        {   change_mask.set(7,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,atoi(text),0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Fixed Position");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(FixedDimensionPosition) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(95,1);
            new_options.set(FixedDimensionPosition,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Hide Left Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(HideLeftDimensionLine) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(105,1);
            new_options.set(HideLeftDimensionLine,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Hide Right Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(HideRightDimensionLine) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(106,1);
            new_options.set(HideRightDimensionLine,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Scale Dimension");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(ScaleText) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(94,1);
            new_options.set(ScaleText,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Value Scale");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.4lf",scale);
        else if (action == PP_SETVALUE)
        {   change_mask.set(93,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,atof(text),0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Text Y Offset");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,textyoffset,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(99,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,x * db.getmainunitscale(),0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }  
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Hidden\tVisible");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Dimension Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimLineVisible) ? "Hidden" : "Visible");
        else if (action == PP_SETVALUE)
        {   change_mask.set(79,1);
            new_options.set(DimLineVisible,_stricmp(text,"Hidden") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Terminator");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Style");
        else if (action == PP_GETVALUE)
            strcpy(text,&terminator_style_l[term1.style&0x7F]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 7 ; i++) 
                if (strcmp(text,&terminator_style_l[i]) == 0)
                {   change_mask.set(15,1);
                    termi.style = (term1.style & 0x80) | i;
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Length");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,term1.length,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
        double x;   
            change_mask.set(16,1);
            if (e.evaluate(&x))
            {   termi.length = x * db.getmainunitscale();
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }
            return 1;
        }
    }


    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Height");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,term1.height,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
        double x;   
            change_mask.set(17,1);
            if (e.evaluate(&x))
            {   termi.height = x * db.getmainunitscale();
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Specify Terminator Weight");
        else if (action == PP_GETVALUE)
            strcpy(text,(term1.style&0x80) != 0 ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(100,1);
            termi.style = (term1.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if ((term1.style&0x80) != 0 && id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(term1.weight);
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            termi.weight = db.lineweights.mmtoweight(wt);
            change_mask.set(101,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(term1.weight));
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Inside\tOutside");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Dimension Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimLinePosition) ?"Outside" :  "Inside");
        else if (action == PP_SETVALUE)
        {   change_mask.set(78,1);
            new_options.set(DimLinePosition,_stricmp(text,"Inside") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Common Point");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Common");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimCommon) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(8,1);
            new_options.set(DimCommon,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (options.test(DimCommon))
    {
        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Reverse Common");
            else if (action == PP_GETVALUE)
                strcpy(text,options.test(DimensionReverse) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(104,1);
                new_options.set(DimensionReverse,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Stack");
            else if (action == PP_GETVALUE)
                strcpy(text,options.test(DimStacked) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(9,1);
                new_options.set(DimStacked,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (options.test(DimStacked) && id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Stack Distance");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,stackdis,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(10,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Style");
            else if (action == PP_GETVALUE)
                strcpy(text,&terminator_style_l[term2.style&0x7F]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 7 ; i++) 
                    if (strcmp(text,&terminator_style_l[i]) == 0)
                    {   change_mask.set(20,1);
                        termi.style = (term2.style & 0x80) | i;
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Length");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term2.length,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(21,1);
                if (e.evaluate(&x))
                {   termi.length = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }


        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term2.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(22,1);
                if (e.evaluate(&x))
                {   termi.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Specify Terminator Weight");
            else if (action == PP_GETVALUE)
                strcpy(text,(term2.style&0x80) != 0 ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(20,1);
                termi.style = (term2.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if ((term2.style&0x80) != 0 && id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(term2.weight);
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                termi.weight = db.lineweights.mmtoweight(wt);
                change_mask.set(103,1);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%.2lf",db.lineweights.weighttomm(term2.weight));
        }
 
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Tolerance");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tPlus/Minus\tDual\tUpper/Lower");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Style");
        else if (action == PP_GETVALUE)
            strcpy(text,&tolerance_style_l[options.test(DimTolerance1) + options.test(DimTolerance2) * 2]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 4 ; i++) 
                if (strcmp(text,&tolerance_style_l[i]) == 0)
                {   change_mask.set(11,1);
                    new_options.set(DimTolerance1,i&1);
                    new_options.set(DimTolerance2,i&2);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    return 1;
                }
        }
    }

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Upper Tolerance");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,uppertol,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;   
            change_mask.set(12,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    } 
    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Lower Tolerance");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,lowertol,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;   
            change_mask.set(13,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }  
    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0) 
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Precision");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",tolprecision); 
        else if (action == PP_SETVALUE)
        {   change_mask.set(14,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,atoi(text),termi,termi,texti,texti,texti,texti,new_options);
        }
    }  

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(ToleranceDefined) ? "No" : "Yes");
        else if (action == PP_SETVALUE)
        {   change_mask.set(91,1);
            new_options.set(ToleranceDefined,_stricmp(text,"Yes") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && options.test(ToleranceDefined))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,toleranceinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(80,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(81,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(82,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",toleranceinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(83,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&horizontal_justification_l[toleranceinfo.options.test(1) + toleranceinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&horizontal_justification_l[i]) == 0)
                    {   change_mask.set(84,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&vertical_justification_l[toleranceinfo.options.test(3) + toleranceinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&vertical_justification_l[i]) == 0)
                    {   change_mask.set(85,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.vertspacing,0); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(86,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(87,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(88,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(89,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(90,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Prefix");
            return 2;
        }
    }

    if (id-- == 0) 
    {   if (action == PP_CLICK) 
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Prefix Text");
        else if (action == PP_GETVALUE)
            strcpy(text,prefixinfo.text);
        else if (action == PP_SETVALUE)
        {   change_mask.set(71,1);
            texti.text = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }   
    } 

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimPrefixValueTextStyle) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(70,1);
            new_options.set(DimPrefixValueTextStyle,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (strlen(prefixinfo.text) > 0 && ! options.test(DimPrefixValueTextStyle))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,prefixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(40,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(41,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(42,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",prefixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(43,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&horizontal_justification_l[prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&horizontal_justification_l[i]) == 0)
                    {   change_mask.set(44,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&vertical_justification_l[prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&vertical_justification_l[i]) == 0)
                    {   change_mask.set(45,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",prefixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(46,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(47,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(48,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(49,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(50,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Value");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Show Value");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimShowValue) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(36,1);
            new_options.set(DimShowValue,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Centred\tTo Left\tTo Right");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Style");
        else if (action == PP_GETVALUE)
            strcpy(text,&placement_style_l[options.test(DimPlacement1) + options.test(DimPlacement2) * 2]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 3 ; i++) 
                if (strcmp(text,&placement_style_l[i]) == 0)
                {   change_mask.set(77,1);
                    new_options.set(DimPlacement1,i&1);
                    new_options.set(DimPlacement2,i&2);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    return 1;
                }
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {int i,nfonts,fontid;
         RCCHAR  **fonts;
         Byte *charsets;
         RCCHAR font[300];

            strcpy(text,"");             
            if (Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid))
            {   for (i = 0 ; i < nfonts ; i++) 
                {   if (text[0] == 0)
                        strcpy(text,fonts[i]);
                    else
                    {   strcat(text,"\t");
                        strcat(text,fonts[i]);
                    }
                    delete [] fonts[i];
                }
                delete [] fonts;
                delete [] charsets;
            }

            return 3;   //  Sort the list
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Text Font");
        else if (action == PP_GETVALUE)
            strcpy(text,valueinfo.font);
        else if (action == PP_SETVALUE)
        {   change_mask.set(25,1);
            texti.font = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,valueinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(26,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,valueinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(27,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",valueinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(28,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&horizontal_justification_l[valueinfo.options.test(1) + valueinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&horizontal_justification_l[i]) == 0)
                    {   change_mask.set(29,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&vertical_justification_l[valueinfo.options.test(3) + valueinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&vertical_justification_l[i]) == 0)
                    {   change_mask.set(30,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",valueinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(31,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(32,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(33,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(34,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(35,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }


    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Suffix");
            return 2;
        }
    }

    if (id-- == 0) 
    {   if (action == PP_CLICK) 
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Suffix Text");
        else if (action == PP_GETVALUE)
            strcpy(text,suffixinfo.text);
        else if (action == PP_SETVALUE)
        {   change_mask.set(76,1);
            texti.text = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }   
    } 

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimSuffixValueTextStyle) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(75,1);
            new_options.set(DimSuffixValueTextStyle,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (strlen(suffixinfo.text) > 0 && ! options.test(DimSuffixValueTextStyle))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,suffixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(55,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,suffixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(56,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,suffixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(57,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",suffixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(58,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&horizontal_justification_l[suffixinfo.options.test(1) + suffixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&horizontal_justification_l[i]) == 0)
                    {   change_mask.set(59,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,&vertical_justification_l[suffixinfo.options.test(3) + suffixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,&vertical_justification_l[i]) == 0)
                    {   change_mask.set(60,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",suffixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(61,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(62,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(63,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(64,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(65,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}
#endif

int Dimension::process_property(int action,int id,void *data,char *text)
{
    return process_property( action, id, data,CHAR2RCCHR(text));
}

int Dimension::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(128),new_options(32);
 int i,result,cstate;
 double wt;

  RCCHAR *horizontal_justification_l[] = {  _RCT("Left"), _RCT("Centre"), _RCT("Right")  };
 RCCHAR *vertical_justification_l[] = {  _RCT("Bottom"), _RCT("Centre"), _RCT("Top")  };
 RCCHAR *terminator_style_l[] = {  _RCT("None"),  _RCT("Arrow"),  _RCT("Open arrow"),  _RCT("Filled arrow"),  _RCT("Dot"),  _RCT("Filled Dot"),  _RCT("Slash")  };
 RCCHAR *tolerance_style_l[] = {  _RCT("None"), _RCT("Plus/Minus"),  _RCT("Dual"),  _RCT("Upper/Lower")  };
 RCCHAR *placement_style_l[] = {  _RCT("Centred"), _RCT("To Left"),  _RCT("To Right")  };

 TerminatorInfo termi;
 TextInfo texti;


    if (isa(circulardimension_entity) && ((CircularDimension *)this)->getcircle() == 0)
    {//  This is a leader - should the simplier leader settings

        if (action == PP_GETNUMPROPERTIES)
            id = 10000;

        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Entity");
                return 1;
            }
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Colour");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getcolour();
                cstate = 1001;
                colour_command(&cstate,0,(void **)&result);
                change_mask.set(0,1);
                if (result != ((Entity *)data)->getcolour())
                    ((Entity *)data)->change(change_mask,result,0,0,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf((char*)text,"%d",getcolour());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Layer");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getlayer();
                cstate = 1001;
                layer_command(&cstate,0,(void **)&result);
                change_mask.set(1,1);
                if (result != ((Entity *)data)->getlayer())
                    ((Entity *)data)->change(change_mask,0,result,0,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf((char*)text,"%d",getlayer());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Style");
            else if (action == PP_CLICK)
            {   result = ((Entity *)data)->getstyle();
                cstate = 1001;
                style_command(&cstate,0,(void **)&result);
                change_mask.set(2,1);
                if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf((char*)text,"%d",getstyle());
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                change_mask.set(3,1);
                if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                    ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf((char*)text,"%.2lf",db.lineweights.weighttomm(getweight()));
        }
        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Leader");
                return 1;
            }
        }

        if (id-- == 0) 
        {  
			if (action == PP_CLICK) 
            {
				RCCHAR rctext[4096];
				Dialog dialog("LTextinsert_Dialog");
                strcpy(rctext,prefixinfo.text);
                dialog.add(new StringDialogControl(100,rctext,4096));
                if (dialog.process() == TRUE)
                {   change_mask.set(71,1);
                    texti.text = rctext;
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }   
                return 0;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text");
            else if (action == PP_GETVALUE)
#ifndef _USING_QTCHAR
                strcpy(text,prefixinfo.text);
#else
			{
                strcpy(text,prefixinfo.text);
			}
#endif
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             RCCHAR **fonts;
             Byte *charsets;
             RCCHAR font[300];

                strcpy(text,"");             
                if (Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {  RCSTRCAT(text,"\t");
                           RCSTRCAT(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,prefixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(40,1);
#ifdef _USING_QTCHAR
				RCCHAR rctext[300];
                strcpy(rctext,text);
                texti.font = rctext;
#else
                texti.font = text;
#endif
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {
				Expression e(text);
                double x;
                change_mask.set(41,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(42,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4f",prefixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(43,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,horizontal_justification_l[prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,horizontal_justification_l[i]) == 0)
                    {   change_mask.set(44,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,vertical_justification_l[prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,vertical_justification_l[i]) == 0)
                    {   change_mask.set(45,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4f",prefixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(46,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(47,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(48,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(49,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(50,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_GETLABEL) 
            {   strcpy(text,"Terminator");
                return 2;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
                return 2;
            }   
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Style");
            else if (action == PP_GETVALUE)
                strcpy(text,terminator_style_l[term1.style&0x7F]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 7 ; i++) 
                    if (strcmp(text,terminator_style_l[i]) == 0)
                    {   change_mask.set(15,1);
                        termi.style = (term1.style & 0x80) | i;
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Length");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term1.length,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(16,1);
                if (e.evaluate(&x))
                {   termi.length = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }


        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term1.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(17,1);
                if (e.evaluate(&x))
                {   termi.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Specify Terminator Weight");
            else if (action == PP_GETVALUE)
                strcpy(text,(term1.style&0x80) != 0 ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(100,1);
                termi.style = (term1.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if ((term1.style&0x80) != 0 && id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(term1.weight);
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                termi.weight = db.lineweights.mmtoweight(wt);
                change_mask.set(101,1);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%.2lf",db.lineweights.weighttomm(term1.weight));
        }

        if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    
        return 0;

    }

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Entity");
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Colour");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Layer");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Style");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
    }
    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Dimension");
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Extension");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,extension,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(5,1);
            if (e.evaluate(&x))
                change(change_mask,*this,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    } 
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Gap from dimension line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(GapFromDimension) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(92,1);
            new_options.set(GapFromDimension,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Gap");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,gap,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(6,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Precision");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",precision);
        else if (action == PP_SETVALUE)
        {   change_mask.set(7,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,atoi(text),0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Fixed Position");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(FixedDimensionPosition) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(95,1);
            new_options.set(FixedDimensionPosition,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Hide Left Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(HideLeftDimensionLine) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(105,1);
            new_options.set(HideLeftDimensionLine,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Hide Right Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(HideRightDimensionLine) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(106,1);
            new_options.set(HideRightDimensionLine,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Scale Dimension");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(ScaleText) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(94,1);
            new_options.set(ScaleText,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Value Scale");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.4lf",scale);
        else if (action == PP_SETVALUE)
        {   change_mask.set(93,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,atof(text),0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }
    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Text Y Offset");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,textyoffset,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(99,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,x * db.getmainunitscale(),0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }  
    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Hidden\tVisible");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Dimension Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimLineVisible) ? "Hidden" : "Visible");
        else if (action == PP_SETVALUE)
        {   change_mask.set(79,1);
            new_options.set(DimLineVisible,_stricmp(text,"Hidden") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Terminator");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Style");
        else if (action == PP_GETVALUE)
            strcpy(text,terminator_style_l[term1.style&0x7F]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 7 ; i++) 
                if (strcmp(text,terminator_style_l[i]) == 0)
                {   change_mask.set(15,1);
                    termi.style = (term1.style & 0x80) | i;
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Length");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,term1.length,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
        double x;   
            change_mask.set(16,1);
            if (e.evaluate(&x))
            {   termi.length = x * db.getmainunitscale();
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }
            return 1;
        }
    }


    if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Height");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,term1.height,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
        double x;   
            change_mask.set(17,1);
            if (e.evaluate(&x))
            {   termi.height = x * db.getmainunitscale();
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Specify Terminator Weight");
        else if (action == PP_GETVALUE)
            strcpy(text,(term1.style&0x80) != 0 ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(100,1);
            termi.style = (term1.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if ((term1.style&0x80) != 0 && id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Terminator Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(term1.weight);
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            termi.weight = db.lineweights.mmtoweight(wt);
            change_mask.set(101,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(term1.weight));
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Inside\tOutside");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Dimension Line");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimLinePosition) ?"Outside" :  "Inside");
        else if (action == PP_SETVALUE)
        {   change_mask.set(78,1);
            new_options.set(DimLinePosition,_stricmp(text,"Inside") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Common Point");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Common");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimCommon) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(8,1);
            new_options.set(DimCommon,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (options.test(DimCommon))
    {
        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Reverse Common");
            else if (action == PP_GETVALUE)
                strcpy(text,options.test(DimensionReverse) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(104,1);
                new_options.set(DimensionReverse,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Stack");
            else if (action == PP_GETVALUE)
                strcpy(text,options.test(DimStacked) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(9,1);
                new_options.set(DimStacked,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (options.test(DimStacked) && id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Stack Distance");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,stackdis,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(10,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"None\tArrow\tOpen arrow\tFilled arrow\tDot\tFilled Dot\tSlash");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Style");
            else if (action == PP_GETVALUE)
                strcpy(text,terminator_style_l[term2.style&0x7F]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 7 ; i++) 
                    if (strcmp(text,terminator_style_l[i]) == 0)
                    {   change_mask.set(20,1);
                        termi.style = (term2.style & 0x80) | i;
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Length");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term2.length,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(21,1);
                if (e.evaluate(&x))
                {   termi.length = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }


        if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,term2.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(22,1);
                if (e.evaluate(&x))
                {   termi.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Specify Terminator Weight");
            else if (action == PP_GETVALUE)
                strcpy(text,(term2.style&0x80) != 0 ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(20,1);
                termi.style = (term2.style&0x7F) | (_stricmp(text,"Yes") == 0) * 0x80;
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if ((term2.style&0x80) != 0 && id-- == 0)
        {   if (action == PP_GETLABEL) 
                strcpy(text,"Terminator Weight");
            else if (action == PP_CLICK)
            {   wt = db.lineweights.weighttomm(term2.weight);
                cstate = 1001;
                weight_command(&cstate,0,(void **)&wt);
                termi.weight = db.lineweights.mmtoweight(wt);
                change_mask.set(103,1);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 0;
            }
            else if (action == PP_GETVALUE)
                sprintf(text,"%.2lf",db.lineweights.weighttomm(term2.weight));
        }
 
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Tolerance");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tPlus/Minus\tDual\tUpper/Lower");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Style");
        else if (action == PP_GETVALUE)
            strcpy(text,tolerance_style_l[options.test(DimTolerance1) + options.test(DimTolerance2) * 2]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 4 ; i++) 
                if (strcmp(text,tolerance_style_l[i]) == 0)
                {   change_mask.set(11,1);
                    new_options.set(DimTolerance1,i&1);
                    new_options.set(DimTolerance2,i&2);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    return 1;
                }
        }
    }

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Upper Tolerance");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,uppertol,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;   
            change_mask.set(12,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    } 
    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Lower Tolerance");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,lowertol,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;   
            change_mask.set(13,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0.0,0.0,0.0,0.0,x * db.getmainunitscale(),0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }
    }  
    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0) 
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Precision");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",tolprecision); 
        else if (action == PP_SETVALUE)
        {   change_mask.set(14,1);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,atoi(text),termi,termi,texti,texti,texti,texti,new_options);
        }
    }  

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(ToleranceDefined) ? "No" : "Yes");
        else if (action == PP_SETVALUE)
        {   change_mask.set(91,1);
            new_options.set(ToleranceDefined,_stricmp(text,"Yes") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (options.test(DimTolerance1) + options.test(DimTolerance2) * 2 != 0 && options.test(ToleranceDefined))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,toleranceinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(80,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(81,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(82,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",toleranceinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(83,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,horizontal_justification_l[toleranceinfo.options.test(1) + toleranceinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,horizontal_justification_l[i]) == 0)
                    {   change_mask.set(84,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,vertical_justification_l[toleranceinfo.options.test(3) + toleranceinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,vertical_justification_l[i]) == 0)
                    {   change_mask.set(85,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,toleranceinfo.vertspacing,0); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(86,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(87,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(88,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(89,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,toleranceinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(90,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Prefix");
            return 2;
        }
    }

    if (id-- == 0) 
    {   if (action == PP_CLICK) 
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Prefix Text");
        else if (action == PP_GETVALUE)
            strcpy(text,prefixinfo.text);
        else if (action == PP_SETVALUE)
        {   change_mask.set(71,1);
            texti.text = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }   
    } 

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimPrefixValueTextStyle) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(70,1);
            new_options.set(DimPrefixValueTextStyle,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (strlen(prefixinfo.text) > 0 && ! options.test(DimPrefixValueTextStyle))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,prefixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(40,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(41,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,prefixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;   
                change_mask.set(42,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",prefixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(43,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,horizontal_justification_l[prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,horizontal_justification_l[i]) == 0)
                    {   change_mask.set(44,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,vertical_justification_l[prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,vertical_justification_l[i]) == 0)
                    {   change_mask.set(45,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",prefixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(46,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(47,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(48,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(49,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(50,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Value");
            return 2;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Show Value");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimShowValue) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(36,1);
            new_options.set(DimShowValue,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Centred\tTo Left\tTo Right");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Tolerance Style");
        else if (action == PP_GETVALUE)
            strcpy(text,placement_style_l[options.test(DimPlacement1) + options.test(DimPlacement2) * 2]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 3 ; i++) 
                if (strcmp(text,placement_style_l[i]) == 0)
                {   change_mask.set(77,1);
                    new_options.set(DimPlacement1,i&1);
                    new_options.set(DimPlacement2,i&2);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    return 1;
                }
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {int i,nfonts,fontid;
         RCCHAR  **fonts;
         Byte *charsets;
         RCCHAR font[300];

            strcpy(text,"");             
            if (Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid))
            {   for (i = 0 ; i < nfonts ; i++) 
                {   if (text[0] == 0)
                        strcpy(text,fonts[i]);
                    else
                    {   strcat(text,"\t");
                        strcat(text,fonts[i]);
                    }
                    delete [] fonts[i];
                }
                delete [] fonts;
                delete [] charsets;
            }

            return 3;   //  Sort the list
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Text Font");
        else if (action == PP_GETVALUE)
            strcpy(text,valueinfo.font);
        else if (action == PP_SETVALUE)
        {   change_mask.set(25,1);
            texti.font = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,valueinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(26,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,valueinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(27,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",valueinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(28,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,horizontal_justification_l[valueinfo.options.test(1) + valueinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,horizontal_justification_l[i]) == 0)
                    {   change_mask.set(29,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,vertical_justification_l[valueinfo.options.test(3) + valueinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,vertical_justification_l[i]) == 0)
                    {   change_mask.set(30,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",valueinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(31,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(32,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(33,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(34,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,valueinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(35,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }


    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Suffix");
            return 2;
        }
    }

    if (id-- == 0) 
    {   if (action == PP_CLICK) 
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Suffix Text");
        else if (action == PP_GETVALUE)
            strcpy(text,suffixinfo.text);
        else if (action == PP_SETVALUE)
        {   change_mask.set(76,1);
            texti.text = CHAR2RCCHR(text);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
        }   
    } 

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL)  
            strcpy(text,"Use Value Text Style");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(DimSuffixValueTextStyle) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(75,1);
            new_options.set(DimSuffixValueTextStyle,_stricmp(text,"No") != 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            return 1;
        }
    }

    if (strlen(suffixinfo.text) > 0 && ! options.test(DimSuffixValueTextStyle))
    {//  Tolerance Text Styles

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {int i,nfonts,fontid;
             char **fonts;
             Byte *charsets;
             char font[300];

                strcpy(text,"");             
                if (Text::getfonts(CHAR2RCCHR(font),&nfonts,(RCCHAR***)CHAR2RCCHR(**&fonts),&charsets,&fontid))
                {   for (i = 0 ; i < nfonts ; i++) 
                    {   if (text[0] == 0)
                            strcpy(text,fonts[i]);
                        else
                        {   strcat(text,"\t");
                            strcat(text,fonts[i]);
                        }
                        delete [] fonts[i];
                    }
                    delete [] fonts;
                    delete [] charsets;
                }

                return 3;   //  Sort the list
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Font");
            else if (action == PP_GETVALUE)
                strcpy(text,suffixinfo.font);
            else if (action == PP_SETVALUE)
            {   change_mask.set(55,1);
                texti.font = CHAR2RCCHR(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,suffixinfo.height,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(56,1);
                if (e.evaluate(&x))
                {   texti.height = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,suffixinfo.width,FormatForEditField); 
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(57,1);
                if (e.evaluate(&x))
                {   texti.width = x * db.getmainunitscale();
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                }
            }   
        } 

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",suffixinfo.angle); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(58,1);
                texti.angle = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left\tCentre\tRight");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Hor Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,horizontal_justification_l[suffixinfo.options.test(1) + suffixinfo.options.test(2) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,horizontal_justification_l[i]) == 0)
                    {   change_mask.set(59,1);
                        texti.options.set(1,i&1);
                        texti.options.set(2,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Bottom\tCentre\tTop");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vert Justification");
            else if (action == PP_GETVALUE)
                strcpy(text,vertical_justification_l[suffixinfo.options.test(3) + suffixinfo.options.test(4) * 2]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 3 ; i++) 
                    if (strcmp(text,vertical_justification_l[i]) == 0)
                    {   change_mask.set(60,1);
                        texti.options.set(3,i&1);
                        texti.options.set(4,i&2);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                    }
            }
        }

        if (id-- == 0) 
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Text Vertical Spacing");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.4lf",suffixinfo.vertspacing); 
            else if (action == PP_SETVALUE)
            {   change_mask.set(61,1);
                texti.vertspacing = atof(text);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
            }   
        } 

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Bold");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(5) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(62,1);
                texti.options.set(5,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Underline");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(6) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(63,1);
                texti.options.set(6,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Italic");
            else if (action == PP_GETVALUE)
             strcpy(text,prefixinfo.options.test(7) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(64,1);
                texti.options.set(7,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

        if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"No\tYes");
                return 2;
            }
            else if (action == PP_GETLABEL)  
                strcpy(text,"Text Boxed");
            else if (action == PP_GETVALUE)
             strcpy(text,suffixinfo.options.test(8) ? "Yes" : "No");
            else if (action == PP_SETVALUE)
            {   change_mask.set(65,1);
                texti.options.set(8,_stricmp(text,"Yes") == 0);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,termi,termi,texti,texti,texti,texti,new_options);
                return 1;
            }
        }

    }

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}






