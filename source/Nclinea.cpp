//
//  Implementation of the 'Point' and 'Line' class
//

#include "ncwin.h"
#include "RCGraphicsItems.h"

PointE::PointE(const EntityHeader &header,Point p1) : Entity(header)
{
#ifdef USING_WIDGETS
    qgi = 0;
#endif
    p = p1;
}

PointE::PointE(Point p1)
{
#ifdef USING_WIDGETS
    qgi = 0;
#endif
    p = p1;
}

int PointE::type(void) const
{  return point_entity;
}

int PointE::isa(int type) const
{  return point_entity == type;
}

void PointE::addtodisplaylist(View3dSurface *vs,int child)
{Point pt1,pt2;
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,17)) return;
    }
  pt1 = vs->modeltoview(p);
  pt2 = vs->modeltoview(p);
  if (vs->clip3dfb(&pt1,&pt2))
    {  pt1 = vs->viewtoscreen(pt1);
       if (! vs->displaylist.pointat(pt1.x,pt1.y)) return;
    }
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void PointE::drawGL(int drawmode,Transform *t,View3dSurface *surface)
{
   //qDebug() << "PointE::draw : this : " << this;
  Point pt1;

  // get rid of the scene version
  draw(DM_ERASE,t,surface);

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

#if 1
  if (t == NULL)
    pt1 = surface->modeltransform.transform(p);
  else
    pt1 = t->transform(surface->modeltransform.transform(p));
#else
  if (t == NULL)
    pt1 = p;
  else
     //pt1 = t->transform(p);
    pt1 = surface->modeltoview(t->transform(p));

  pt1 = surface->modeltransform.transform(pt1);
#endif

  //surface->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  surface->setupGL(linestylescale,0.0,drawmode,colour,0,weight,0);

  RCCOLORREF pcol;
  pcol = surface->getcolour();
  //glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = surface->curPen.widthF()<1.0 ? 1.0 : surface->curPen.widthF();
  glLineWidth(lwidth);

  // calculate the plane for drawing the cross
  //pt1 = surface->viewtoscreen(pt1);
  // get the vector from the point to the eye
  Point plook = surface->getview()->geteye() - surface->getview()->getref();
  // get the up vector at the point
  Point pup = surface->getview()->getup();
  // get the right vector at the point
  Point pright = -plook.cross(pup).normalize();
  // adjust the up vector
  pup = plook.cross(pright).normalize();

  // get the cross points scale the length to be constant on the screen
  Point pix0 = surface->screentomodel(Point(0.0,0.0,0.0));
  Point pix1 = surface->screentomodel(Point(1.0,0.0,0.0));
  double pxL = (pix1 - pix0).length();
  double s1 = pxL * 3.0;
  double s2 = pxL * 3.0;
  s1 += lwidth / 2.0 * pxL;
  s2 += lwidth / 2.0 * pxL;
  //double s = 0.8 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  Point sp1,sp2,sp3,sp4;
  sp1 = pt1 + (pup*s1);
  sp2 = pt1 - (pup*s2);
  sp3 = pt1 + (pright*s1);
  sp4 = pt1 - (pright*s2);

  // screen space point
  surface->lineGL(sp1.x,sp1.y,sp1.z,sp2.x,sp2.y,sp2.z);
  surface->lineGL(sp3.x,sp3.y,sp3.z,sp4.x,sp4.y,sp4.z);

  // model space point
  //surface->lineGL(pt1.x-3,pt1.y,pt1.z,pt1.x+3,pt1.y,pt1.z);
  //surface->lineGL(pt1.x,pt1.y-3,pt1.z,pt1.x,pt1.y+3,pt1.z);

  //glBegin(GL_LINES);
  //glVertex3f(pt1.x-3,pt1.y,pt1.z);
  //glVertex3f(pt1.x+3,pt1.y,pt1.z);
  //glEnd();
  //glBegin(GL_LINES);
  //glVertex3f(pt1.x,pt1.y-3,pt1.z);
  //glVertex3f(pt1.x,pt1.y+3,pt1.z);
  //glEnd();
}

void PointE::draw(int drawmode,Transform *t,View3dSurface *surface)
{
   //qDebug() << "PointE::draw : this : " << this;
    if(drawmode != DM_PRINT)
    {
        if(qgi && qgi->scene() == surface->getScene())
            surface->getScene()->removeItem(qgi);
        delete qgi;
        qgi = 0;
    }

    if(drawmode == DM_ERASE)
        return;

  Point pt1;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  if (t == NULL)
    pt1 = surface->modeltoview(p);
  else
    pt1 = surface->modeltoview(t->transform(p));
  //surface->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  surface->setup(linestylescale,0.0,drawmode,colour,0,weight,0);
  //surface->setup(linestylescale,0.0,drawmode,0,0,0,0);


  if (surface->inside(pt1))
  {
      // delete the scene lines and draw again
      // this is one way to do it
      //qDebug() << "PointE::draw : erasing  : qgi " << qgi;

      pt1 = surface->viewtoscreen(pt1);

      QGraphicsItem *qline1 = 0, *qline2 = 0;
      QList <QGraphicsItem *> glist;

      //qDebug() << "view to screen : " << pt1.x << " " << pt1.y;
      qline1 = surface->line((QGraphicsItem*)0,int(pt1.x-3),int(pt1.y),int(pt1.x+3),int(pt1.y));
      if(qline1)
          glist.append(qline1);
      qline2 = surface->line((QGraphicsItem*)0,int(pt1.x),int(pt1.y-3),int(pt1.x),int(pt1.y+3));
      if(qline2)
          glist.append(qline2);

      //qDebug() << "point drawing into surface->getScene() : " << surface->getScene();
      if(glist.size() > 0)
          qgi = surface->getScene()->createItemGroup(glist);
      if(qgi)
      {
          if(getselected())
              qgi->setZValue(QGMAXZVALUE+QGZVALINC);
          else
             qgi->setZValue(zValue);
      }

      if(qgi && dynamic_cast<View3dSubSurface*>(surface) != 0)
          if(((View3dSubSurface*)surface)->qgicp != 0 &&
             ((View3dSubSurface*)surface)->qgicp != qgi->parentItem())
              qgi->setParentItem(((View3dSubSurface*)surface)->qgicp);

  }

#ifdef NEW_RUBBERB
  // highjack the qgi for the rubberband
  if(drawmode == DM_INVERT)
  {
      if(qgi)
      {
          if(surface->qgirb == 0)
          {
              surface->qgirb = new QGraphicsItemGroup();
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);
              surface->getScene()->addItem(surface->qgirb);
          }
          else
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);
      }
      qgi=0;
  }
  else if(drawmode == DM_MARKER)
  {
      if(qgi)
      {
          if(surface->qgimk == 0)
          {
              surface->qgimk = new QGraphicsItemGroup();
              ((QGraphicsItemGroup*)surface->qgimk)->addToGroup(qgi);
              surface->getScene()->addItem(surface->qgimk);
          }
          else
              ((QGraphicsItemGroup*)surface->qgimk)->addToGroup(qgi);
      }
      qgi=0;
  }
#endif
}
#else
void PointE::draw(int drawmode, Transform *t,View3dSurface *surface)
{
    Point pt 1;
    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
    if (t == NULL)
      pt1 = surface->modeltoview(p);
    else
      pt1 = surface->modeltoview(t->transform(p));

    //surface->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
    surface->setup(linestylescale,0.0,drawmode,colour,0,weight,0);

    if (surface->inside(pt1))
    {
        pt1 = surface->viewtoscreen(pt1);
        surface->line(int(pt1.x-3),int(pt1.y),int(pt1.x+3),int(pt1.y));
        surface->line(int(pt1.x),int(pt1.y-3),int(pt1.x),int(pt1.y+3));
    }
}
#endif

int PointE::hide(HideImage *)
{
    return 1;
}

int PointE::hideGL(HideImage *)
{
    return 1;
}

double PointE::pick(View3dSurface *surface,double x,double y,Point *pos)
{double d;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(surface);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    surface = window->getoffscreenbitmap();
 
  if (pos == NULL && surface->displaylist.defined(this))
    return surface->displaylist.pick(this,x,y);
  if ((d = surface->pick(p,0,(int)x,(int)y)) >= 0.0 && pos != NULL) *pos = p;
  return d;
}

int PointE::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
  return vs->pick(p,inside,x1,y1,x2,y2);
}

int PointE::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{int picked;
   View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
picked = inside != 2;
  vs->pick(nvertices,pwvertices,p,p+Point(1.0E-8,0.0,0.0),inside,&picked);
  return picked;
}


int PointE::npoints(void)
{  return 1;
}

Point PointE::point(int)
{  return p;
}

Point PointE::nearp(Point)
{  return p;
}

int PointE::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  if (! sp->match(p)) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0 && ! e->isa(group_entity))
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0 && ! e->isa(group_entity))
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
  p = t.transform(p);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int PointE::move(Database *db,Transform &t,int erase)
{Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
  if (erase) draw(DM_ERASE);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  p = t.transform(p);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;
}

Entity *PointE::clone(Transform &t)
{ if (copye != NULL) return copye;
  copye = new PointE(*this,t.transform(p));
  copye->cloneattributes(this);
  return copye;
}

int PointE::project(Database *db,Transform &t,int)
{ if ((status & 2) == 2) return 1;
  if (! db->geometry.add(new Line(*this,p,t.transform(p)))) return 0;
  status |= 2;
  if (copye == NULL) 
    return db->geometry.add(clone(t));
  else
    return 1;
}

int PointE::save(CadFile *outfile)
{short type;
  type = point_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! p.save(outfile)) return 0;
  return 1;
}

Entity *PointE::load(int,CadFile *infile)
{PointE *point;
 EntityHeader *header;
 Point p;
  if ((header = loadheader(infile)) == NULL) return NULL;
  if (! p.load(infile)) return NULL;
  point = new PointE(*header,p);
  point->atlist = header->getatlist();
  delete header;
  return point;
}

int PointE::save(UndoFile *outfile)
{short type;
  type = point_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  return 1;
}

Entity *PointE::load(UndoFile *infile)
{PointE *point;
 Point p;
 char bm[sizeof(BitMask)];
  if ((point = new PointE) != NULL)
    {AttributeList al;
       memcpy(bm,&point->visible,sizeof(bm));
       if (! infile->blockread(point,sizeof(*point))) return 0;
       memcpy(&point->visible,bm,sizeof(bm));
       if (! infile->read(&point->visible)) return 0;
       if (! al.load(infile)) return 0;   point->atlist = al;
    }
  return point;
}

int PointE::saveattributes(CadFile *)
{  return 1;
}

int PointE::loadattributes(CadFile *)
{  return 1;
}

int PointE::savedxf(int blocksection,DXFOut *dxffile)
{ if (! blocksection)
    {  if (! dxffile->writestring(0, (char*)"POINT")) return 0;
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbPoint")) return 0;
       if (! dxffile->writereal(10,p.x * dxffile->getscale())) return 0;
       if (! dxffile->writereal(20,p.y * dxffile->getscale())) return 0;
       if (! dxffile->writereal(30,p.z * dxffile->getscale())) return 0;
    }
  return 1;
}

int PointE::exportiges(FILE *outfile,int section,int,int *ndirectory,int *nparameter)
{ if (section == 0)
    {  if (fprintf(outfile,"     116%8d               0       0       0       0       0 0 0 0 0D%7d\n",(*nparameter)+1,++(*ndirectory)) < 0) return 0;
       if (fprintf(outfile,"     116       0%8d       2       0                               0D%7d\n",colour,++(*ndirectory)) < 0) return 0;
       (*nparameter)++;
    }
  else 
    {  if (fprintf(outfile,"116,%18.10e,%18.10e,%18.10e;    %7dP%7d\n",p.x,p.y,p.z,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       (*ndirectory) += 2;
    }
  return 1;
}

int PointE::savegeometry(CadFile *outfile)
{ if (! p.save(outfile)) return 0;
  return 1;
}

int PointE::loadgeometry(CadFile *infile)
{ if (! p.load(infile)) return 0;
  return 1;
}

class PointVerifyButtonControl : public ButtonDialogControl
  {public:
     PointVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int PointVerifyButtonControl::select(Dialog *dialog)
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
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    return 0;
}

void PointE::verify(void)
{
 Dialog dialog("PointVerify_Dialog");
 int c,l,s;
 double w;
 Point p1;
 Workplane *wp;
 RCCHAR position[300];
  if ((wp = db.workplanes.getcurrent()) != NULL)
    p1 = wp->modeltoworkplane(p);
  else
    p1 = p;
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new PointVerifyButtonControl(1000));
  dialog.add(new PointVerifyButtonControl(1001));
  dialog.add(new PointVerifyButtonControl(1002));
  dialog.add(new PointVerifyButtonControl(1003));

  dialog.add(new StringDialogControl(104,p1.string(position),128));
  dialog.setposition(DPOK);
  if(dialog.process() == TRUE)
  {
      setcolour(c);
      setstyle(s);
      setlayer(l);
      setweight(db.lineweights.mmtoweight(w));
  }
}

int PointE::explode(void)
{ return 0;
}

void PointE::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{ if (surface == NULL)
    *pmin = *pmax = p;
  else
    *pmin = *pmax = surface->modeltouv(p);
}

int PointE::issame(int geometry,const Entity &entity) const
{PointE *pointe;
  if (! entity.isa(point_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         pointe = (PointE  *) &entity;
         return (pointe->p - p).length() < db.getptoler();
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int PointE::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int result,cstate;
 double wt;
 Point p1;
 Transform t;

    if (action == PP_CLICK)
    {   if (id == 1)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (id == 2)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (id == 3)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (id == 4)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else
            return 1;
    }
    else if (action == PP_GETVALUE)
    {   if (id == 1)
            sprintf((RCCHAR*)text,"%d",getcolour());
        else if (id == 2)
            sprintf((RCCHAR*)text,"%d",getlayer());
        else if (id == 3)
            sprintf((RCCHAR*)text,"%d",getstyle());
        else if (id == 4)
            sprintf((RCCHAR*)text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (id == 6)
            db.formatnumber((RCCHAR*)text,p.x,0);
        else if (id == 7)
            db.formatnumber((RCCHAR*)text,p.y,0);
        else if (id == 8)
            db.formatnumber((RCCHAR*)text,p.z,0);
    }
    else if (action == PP_SETVALUE) 
    {Expression e((RCCHAR*)text);
     double x;

        if (id == 6 && e.evaluate(&x))
        {   p1 = Point(x * db.getmainunitscale(),p.y,p.z);
            t.translate(p1 - p);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 7 && e.evaluate(&x))
        {   p1 = Point(p.x,x * db.getmainunitscale(),p.z);
            t.translate(p1 - p);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 8 && e.evaluate(&x))
        {   p1 = Point(p.x,p.y,x * db.getmainunitscale());
            t.translate(p1 - p);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy((RCCHAR*)text,"Entity");
            return 1;
        } 
        else if (id == 5)
        {   strcpy((RCCHAR*)text,"Point");
            return 1;
        }
        else 
        {   if (id == 1)
                strcpy((RCCHAR*)text,"Colour");
            else if (id == 2)
                strcpy((RCCHAR*)text,"Layer");
            else if (id == 3)
                strcpy((RCCHAR*)text,"Style");
            else if (id == 4)
                strcpy((RCCHAR*)text,"Weight");
            else if (id == 6)
                strcpy((RCCHAR*)text,"X");
            else if (id == 7)
                strcpy((RCCHAR*)text,"Y");
            else if (id == 8)
                strcpy((RCCHAR*)text,"Z");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 9; 
    
    return 0;
}


 
Line::Line(Point start,Point end)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

   p1 = start;
   p2 = end;
}

Line::Line(const EntityHeader &header,Point start,Point end) : LinearEntity(header)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

  p1 = start;
  p2 = end;
}

Line::Line(Entity *e,Point ep,Point p)
{Point p3,p4,p5,p6;
 double d1,d2,d3;

#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

   if (e->isa(line_entity))
    {  p1 = p;
       p2 = ((Line *)e)->nearp(p1);
    }
  else if (e->isa(circle_entity))
    {Circle *c;
     double a,cosa,d;
       c = (Circle *) e;
       d = (p - c->getorigin()).length();
       p5 = c->nearp(p);
       p6 = (c->getorigin() - p5) + c->getorigin();
       if ((p6-ep).length() < (p5-ep).length())
         p5 = p6;
       if (d <= c->getradius() || (cosa = c->getradius() / d) >= 1.0)
         {  p1 = p;
            p2 = p5;
         }
       else
         {  p1 = p;
            a =  acos(cosa) + c->angle(p);
            p3 = c->getorigin() + c->getxaxis() * (cos(a) * c->getradius()) + c->getyaxis() * (sin(a) * c->getradius());
            a = -acos(cosa) + c->angle(p);
            p4 = c->getorigin() + c->getxaxis() * (cos(a) * c->getradius()) + c->getyaxis() * (sin(a) * c->getradius());
            d1 = (p3-ep).length();  d2 = (p4-ep).length();  d3 = (p5-ep).length();
            if (d1 < d2 && d1 < d3)
              p2 = p3;
            else if (d2 < d1 && d2 < d3)
              p2 = p4;
            else
              p2 = p5;
         }
    }
  else if (e->isa(curve_entity))
    {Curve *curve = (Curve *) e;
     int normal;
     double t;
       p3 = curve->nearp(1,p,&t);
       normal = t >= 0.0;          // normal TRUE => line p3 to p is defined
       line(1,curve,ep,p);
       if (defined)
         {  if (normal && (p3 - ep).length() < (p2 - ep).length())
              p2 = p3;
         }
       else if (normal)
         {  p1 = p;
            p2 = p3;
            defined = 1;
         }
       else
         defined = 0;
    }
  else if (e->isa(ellipse_entity))
    {EllipseE *ellipse = (EllipseE *) e;
       p3 = ellipse->nearp(p);
       if ((p - ellipse->getorigin()).length() < (ellipse->position((ellipse->toparametric(ellipse->angle(p)) - ellipse->getstarta())/ellipse->getsweepa()) - ellipse->getorigin()).length())
         {  p1 = p;  p2 = p3;
         }
       else
         {  line(1,ellipse,ep,p);
            if (defined)
              {  if ((p3 - ep).length() < (p2 - ep).length())
                   p2 = p3;
              }
            else
              defined = 0;
         }
    }
  else
    defined = 0;
}

Line::Line(Entity *e1,Point ep1,Entity *e2,Point ep2)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    if (e1->isa(line_entity) && e2->isa(circle_entity) ||
      e1->isa(circle_entity) && e2->isa(line_entity))
    {Line *l;  Circle *c;
     Point p3,p4,p5,p6,dir;
       if (e1->isa(line_entity)) l = (Line *)e1; else l = (Line *)e2;
       if (e1->isa(circle_entity)) c = (Circle *)e1; else c = (Circle *)e2;
       if ((l->p1 - l->p2).length() > db.getptoler())
         {  p3 = p5 = l->nearp(c->getorigin());
            dir = l->direction(0.0) * c->getradius();
            p3 -= dir;  p4 = c->getorigin() - dir;
            p5 += dir;  p6 = c->getorigin() + dir;
            if ((p3-ep1).length() + (p4-ep2).length()  <
                (p5-ep1).length() + (p6-ep2).length())
              {  p1 = p3;  p2 = p4;
              }
            else
              {  p1 = p5;  p2 = p6;
              }
            if (e1->isa(circle_entity))
              {  p3 = p1;  p1 = p2;  p2 = p3;
              }
         }
       else
         defined = 0;
    }
  else if (e1->isa(circle_entity) && e2->isa(circle_entity))
    {Circle *c1,*c2;
     double cosa,sina,a,d,dmin,d1;
     int i;
     Point p3,p4;
       c1 = (Circle *) e1;  c2 = (Circle *) e2;
       d = (c2->getorigin()-c1->getorigin()).length();
       dmin = -1.0;
       if (d > db.getptoler())
         {  for (i = 0 ; i < 4 ; i++)
              {  if (i == 0 || i == 1) cosa = (c2->getradius() - c1->getradius()) / d;
                 else if (i == 2 || i == 3) cosa = (c1->getradius() + c2->getradius()) / d;
                 if (cosa > 1.0 || cosa < -1.0) continue;
                 if ((i % 2) == 0)
                   a = c2->angle(c1->getorigin()) + acos(cosa);
                 else
                   a = c2->angle(c1->getorigin()) - acos(cosa);
                 cosa = cos(a);  sina = sin(a);
                 p4 = c2->getorigin() + c2->getxaxis() * (cosa * c2->getradius()) + c2->getyaxis() * (sina * c2->getradius());
                 if (i == 2 || i == 3)
                   {  cosa = -cosa;  sina = -sina;
                   }
                 p3 = c1->getorigin() + c2->getxaxis() * (cosa * c1->getradius()) + c2->getyaxis() * (sina * c1->getradius());
                 d1 = (p3-ep1).length() + (p4-ep2).length();
                 if (dmin < -0.5 || d1 < dmin)
                   {  dmin = d1;  p1 = p3;  p2 = p4;
                   }
              }
         }   
       if (dmin < -0.5)
         {  p1 = c1->nearp(ep1);  p2 = c2->nearp(ep1);
         }
    }
  else
    defined = 0;
}

Line::Line(Point direction,Entity *e1,Point ep1,Entity *e2,Point ep2,double constrainlength)
{Entity *e;
 Circle *c;
 Line *l;
 Point p3,p4,n;
 int reverse;

#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

  if (e1 == NULL && e2 != NULL && e2->isa(line_entity) ||
      e2 == NULL && e1 != NULL && e1->isa(line_entity))
    {Point p3;
       if (e1 == NULL)
         {  p3 = ep1;  p4 = ep2;  e = e2;  reverse = 0;
         }
       else
         {  p3 = ep2;  p4 = ep1;  e = e1;  reverse = 1;
         }
       Line line(p3,p3+direction);
       Intersect i1(e,&line,p4);
       if (i1.nintersections() == 1)
         {  p1 = p3;  p2 = ((PointE *)i1.intersection(0))->getp();
         }
       else
         {  p1 = p3;  p2 = Line(p3,p3+direction).nearp(p4);
         }
    }
  else if (e1 == NULL && e2 != NULL && e2->isa(circle_entity) ||
           e2 == NULL && e1 != NULL && e1->isa(circle_entity))
    {Point p3;
       if (e1 == NULL)
         {  p3 = ep1;  p4 = ep2;  c = (Circle *) e2;  reverse = 0;
         }
       else
         {  p3 = ep2;  p4 = ep1;  c = (Circle *) e1;  reverse = 1;
         }
       Line line(p3,p3+direction);
       Intersect i1(c,&line,p4);
       if (i1.nintersections() == 1)
         {  p1 = p3;  p2 = ((PointE *)i1.intersection(0))->getp();
         }
       else
         {  n = direction.cross(c->getxaxis().cross(c->getyaxis())).normalize();
            if (n.length() > db.getptoler())
              {  n *= c->getradius();
                 if ((ep2 - c->getorigin() - n).length() < (ep2 - c->getorigin() + n).length())
                   p4 = c->getorigin() + n;
                 else
                   p4 = c->getorigin() - n;
                 Line line(p4,p4+direction);
                 p3 = line.nearp(p3);
                 p1 = p3;
                 p2 = p4;
              }
            else
              defined = 0;
         }
    }
  else if (e1 == NULL || e2 == NULL)
    {  defined = 0;
       return;
    }
  else if (e1->isa(line_entity) && e2->isa(circle_entity) ||
           e2->isa(line_entity) && e1->isa(circle_entity))
    {Point p3;
       if (e1->isa(line_entity))
         {  p3 = ep1;  p4 = ep2;  l = (Line *) e1;  c = (Circle *) e2;  reverse = 0;
         }
       else
         {  p3 = ep2;  p4 = ep1;  l = (Line *) e2;  c = (Circle *) e1;  reverse = 1;
         }
       Line line(ep2,ep2+direction);
       Intersect i1(c,&line,p4);
       Intersect i2(l,&line,p3);
       if (i1.nintersections() == 1 && i2.nintersections() == 1)
         {  p1 = ((PointE *)i1.intersection(0))->getp();
            p2 = ((PointE *)i2.intersection(0))->getp();
         }
       else
         {  n = direction.cross(c->getxaxis().cross(c->getyaxis())).normalize();
            if (n.length() > db.getptoler())
              {  n *= c->getradius();
                 if ((ep2 - c->getorigin() - n).length() < (ep2 - c->getorigin() + n).length())
                   p4 = c->getorigin() + n;
                 else
                   p4 = c->getorigin() - n;
                 Line line(p4,p4+direction);
                 Intersect i1(l,&line,p3);
                 if (i1.nintersections() == 1)
                   {  p1 = p4;
                      p2 = ((PointE *)i1.intersection(0))->getp();
                   }
                 else
                   defined = 0;
              }
            else
              defined = 0;
         }
    }
  else if (e1->isa(line_entity) && e2->isa(line_entity))
    {  Line line(ep2,ep2+direction);
       Intersect i1(e1,&line,ep1);
       Intersect i2(e2,&line,ep2);
       if (i1.nintersections() == 1 && i2.nintersections() == 1)
         {  p1 = ((PointE *)i1.intersection(0))->getp();
            p2 = ((PointE *)i2.intersection(0))->getp();
         }
       else
         defined = 0;
    }
  else if (e1->isa(circle_entity) && e2->isa(circle_entity))
    {  Line line1(ep2,ep2+direction);
       Intersect i1(e1,&line1,ep1);
       if (i1.nintersections() == 0)
         {  c = (Circle *)e1;
            n = direction.cross(c->getxaxis().cross(c->getyaxis())).normalize();
            if (n.length() > db.getptoler())
              {  n *= c->getradius();
                 if ((ep2 - c->getorigin() - n).length() < (ep2 - c->getorigin() + n).length())
                   p3 = c->getorigin() + n;
                 else
                   p3 = c->getorigin() - n;
              }
         }
       else
         p3 = ((PointE *)i1.intersection(0))->getp();
       Line line2(p3,p3+direction);
       Intersect i2(e2,&line2,ep2);
       if (i2.nintersections() == 1)
         {  p1 = p3;
            p2 = ((PointE *)i2.intersection(0))->getp();
         }
       else
         defined = 0;
    }
  else
    defined = 0;

  if (defined && constrainlength > 1.0E-10)
    {double length;
       length = (p2-p1).length();
       if (length > 1.0E-10)
         {  length = constrainlength * floor(length / constrainlength + 0.5);
            if (length == 0.0)
              length = constrainlength;  
            if (reverse)
              p1 = p2 + (p1 - p2).normalize() * length;
            else
              p2 = p1 + (p2 - p1).normalize() * length;
         }
    }
}

void Line::line(int,Curve *curve,Point ep,Point p)
{int nsegs,i,j;
 double t,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8,dmin,*nknot,*tbasis,toler;
 Point p3,p4,p5,pmin,pos,dir;
  curve->newbasis();
  toler = db.getptoler() * db.getptoler();
  defined = 0;
  nknot = NULL;  tbasis = NULL;
  nsegs = 50;

#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

   t = 0.0;  dt = 1.0 / double(nsegs);
  if (! curve->posdir(t,&nknot,&tbasis,&p3,&dir))
    {  defined = 0;  return;
    }
  p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d1 = p5.dot(p5);
  if (d1 < toler)
    {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
    }
  if (! curve->posdir(dt,&nknot,&tbasis,&p3,&dir))
    {  defined = 0;  return;
    }
  p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d2 = p5.dot(p5);
  if (d2 < toler)
    {  if (! defined || (p3 - ep).length() < dmin)
         {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
         }
    }
  for (i = 0,t = 2.0 * dt ; i < nsegs - 1 ; i++,t += dt)
    {  if (! curve->posdir(t,&nknot,&tbasis,&p3,&dir))
         {  defined = 0;  return;
         }
       p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d3 = p5.dot(p5);
       if (d3 < toler)
         {  if (! defined || (p3 - ep).length() < dmin)
              {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
              }
         }
       if (d1 >= d2 && d2 <= d3)
         {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
            d4 = d1;  d5 = d2;  d6 = d3;
            for (j = 0 ; j < 50 ; j++)
              {  if (! curve->posdir(t1 + (t2 - t1) / 2.0,&nknot,&tbasis,&p3,&dir))
                   {  defined = 0;  return;
                   }
                 p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4)); d7 = p5.dot(p5);
                 if (d4 >= d7 && d7 <= d5)
                   {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                      d6 = d5;  d5 = d7;
                   }
                 else
                   {  if (! curve->posdir(t3 - (t3 - t2) / 2.0,&nknot,&tbasis,&p3,&dir))
                        {  defined = 0;  return;
                        }
                      p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d8 = p5.dot(p5);
                      if (d5 >= d8 && d8 <= d6)
                        {  t1 = t2;  t2 = t3 - (t3 - t2) / 2.0;
                           d4 = d5;  d5 = d8;
                        }
                      else
                        {  t1 = t1 + (t2 - t1) / 2.0;  t3 = t3 - (t3 - t2) / 2.0;
                           d4 = d7;  d6 = d8;
                        }
                   }
              }
            if (! curve->posdir((t1 + t3) / 2.0,&nknot,&tbasis,&p3,&dir))
              {  defined = 0;  return;
              }
            p4 = p3 - p;
            p5 = p4 - dir * (dir.dot(p4));  d7 = p5.dot(p5);
            if (d7 < toler)
              {  if (! defined || (p3 - ep).length() < dmin)
                   {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
                   }
              }
         }
       d1 = d2;  d2 = d3;
    }
  curve->deletebasis();
  delete [] nknot;  delete [] tbasis;
}

void Line::line(int,EllipseE *ellipse,Point ep,Point p)
{int nsegs,i,j;
 double t,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8,dmin,toler;
 Point p3,p4,p5,pmin,pos,dir;
  toler = db.getptoler() * db.getptoler();
  defined = 0;
  nsegs = 50;

#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

  t = 0.0;  dt = 1.0 / double(nsegs);
  p3 = ellipse->position(t);
  dir = ellipse->direction(t);
  p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d1 = p5.dot(p5);
  if (d1 < toler)
    {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
    }
  p3 = ellipse->position(dt);
  dir = ellipse->direction(dt);
  p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d2 = p5.dot(p5);
  if (d2 < toler)
    {  if (! defined || (p3 - ep).length() < dmin)
         {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
         }
    }
  for (i = 0,t = 2.0 * dt ; i < nsegs - 1 ; i++,t += dt)
    {  p3 = ellipse->position(t);
       dir = ellipse->direction(t);
       p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d3 = p5.dot(p5);
       if (d3 < toler)
         {  if (! defined || (p3 - ep).length() < dmin)
              {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
              }
         }
       if (d1 >= d2 && d2 <= d3)
         {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
            d4 = d1;  d5 = d2;  d6 = d3;
            for (j = 0 ; j < 50 ; j++)
              {  p3 = ellipse->position(t1 + (t2 - t1) / 2.0);
                 dir = ellipse->direction(t1 + (t2 - t1) / 2.0);
                 p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4)); d7 = p5.dot(p5);
                 if (d4 >= d7 && d7 <= d5)
                   {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                      d6 = d5;  d5 = d7;
                   }
                 else
                   {  p3 = ellipse->position(t3 - (t3 - t2) / 2.0);
                      dir = ellipse->direction(t3 - (t3 - t2) / 2.0);
                      p4 = p3 - p;  p5 = p4 - dir * (dir.dot(p4));  d8 = p5.dot(p5);
                      if (d5 >= d8 && d8 <= d6)
                        {  t1 = t2;  t2 = t3 - (t3 - t2) / 2.0;
                           d4 = d5;  d5 = d8;
                        }
                      else
                        {  t1 = t1 + (t2 - t1) / 2.0;  t3 = t3 - (t3 - t2) / 2.0;
                           d4 = d7;  d6 = d8;
                        }
                   }
              }
            p3 = ellipse->position((t1 + t3) / 2.0);
            dir = ellipse->direction((t1 + t3) / 2.0);
            p4 = p3 - p;
            p5 = p4 - dir * (dir.dot(p4));  d7 = p5.dot(p5);
            if (d7 < toler)
              {  if (! defined || (p3 - ep).length() < dmin)
                   {  dmin = (p3 - ep).length();  p1 = p;  p2 = p3;  defined = 1;
                   }
              }
         }
       d1 = d2;  d2 = d3;
    }
}

int Line::type(void) const
{  return line_entity;
}

int Line::isa(int type) const
{  return line_entity == type;
}

void Line::segment(int n,int include,Point *verts)
{int i,j;
 double t,dt;
 Point p3,p4,dir;
  if (include & LE_REVERSE)
    {  p3 = p2;  p4 = p1;
    }
  else
    {  p3 = p1;  p4 = p2;
    }
  j = 0;
  if (include & LE_INCLUDESTART)
    verts[j++] = p3;
  dt = 1.0 / (double) n;
  if (! (include & LE_INCLUDEEND)) n--; 
  dir = p4 - p3;
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    verts[j++] = p3 + dir * t;
}

void Line::addtodisplaylist(View3dSurface *vs,int child)
{Point pt1,pt2;
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,34)) return;
    }
  pt1 = vs->modeltoview(p1);
  pt2 = vs->modeltoview(p2);
  if (vs->clip3dfb(&pt1,&pt2))
    {  pt1 = vs->viewtoscreen(pt1);
       pt2 = vs->viewtoscreen(pt2);
       if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
       if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
    }
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void Line::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
    Point pt1,pt2;

    // get rid of the scene version
    draw(DM_ERASE,t,vs);

    //qDebug() << "drawing a line with GL";

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

    vs->setupGL(linestylescale  ,length(),drawmode,colour,style,weight,patternlinestyle);

    RCCOLORREF pcol;
    pcol = vs->getcolour();
    //GLfloat fr=GetRValue(pcol)/255.0;
    //GLfloat fg=GetGValue(pcol)/255.0;
    //GLfloat fb=GetBValue(pcol)/255.0;
    //glColor3f(fr,fg,fb);
    SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    //glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    GLfloat lwidth = vs->curPen.widthF();
    //glLineWidth(lwidth<1.0?1.0:lwidth);
    glLineWidth(vs->curGLWeight);

    if (t == NULL)
    {
        //QString dmpName("line");
        //vs->modeltransform.dump(dmpName.toLatin1().constData());

        // test for inverse projection
        //pt1 = vs->modeltoview(p1);
        //Point spt = vs->viewtoscreen(pt1);
        //Point vpt = vs->screentoview(spt);

        //pt1 = vs->modeltoview(p1);
        //pt2 = vs->modeltoview(p2);
        pt1 = vs->modeltransform.transform(p1);
        pt2 = vs->modeltransform.transform(p2);
    }
    else
    {
         //pt1 = vs->modeltoview(t->transform(p1));
         //pt2 = vs->modeltoview(t->transform(p2));
         pt1 = t->transform(vs->modeltransform.transform(p1));
         pt2 = t->transform(vs->modeltransform.transform(p2));
    }
    //if (vs->clip3d(&pt1,&pt2))
    if (patternlinestyle != 0)
    {
        // drawing patternstyle with GL in screen space
        Point spt1 = vs->modeltoscreen(pt1);
        Point spt2 = vs->modeltoscreen(pt2);
        vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);

        // this one draws patternstyle line with GL in model space
        //vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);

    }
    /*
    else if(vs->getlinestyle())
    {
        Point spt1 = vs->modeltoscreen(pt1);
        Point spt2 = vs->modeltoscreen(pt2);
        vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);
    }
    */
    else
    {
         //pt1 = vs->viewtoscreen(pt1);
         //pt2 = vs->viewtoscreen(pt2);
         //vs->line(painter,pt1.x,pt1.y,pt2.x,pt2.y);
         vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
         /*
         if(drawmode == DM_SELECT)
         {
             double curw = vs->curGLWeight;
             SetNoMaterial(255,0,255);
             vs->curGLWeight = 0.0;
             glLineWidth(3.0);
             vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
             //SetNoMaterial(0,255,0);
             glLineWidth(1.0);
             vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
             vs->curGLWeight = curw;
         }
         else
         {
             vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
         }
         */
         // debug
         /*
         Point p1t,p2t;
         p1t = vs->modeltoscreen(pt1);
         p2t = vs->modeltoscreen(pt2);
         p1t = vs->screentomodel(Point(p1t.x,p1t.y,p1t.z));
         p2t = vs->screentomodel(Point(p2t.x,p2t.y,p2t.z));
         vs->line3dGL(p1t,p2t);
         */
    }

    if (patternlinestyle != 0)
      patternlinestyle->endGL(vs);
      // end Gl dawing in model space
      //patternlinestyle->endGL(vs);

    if (gethandles())
    {
         vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_HANDLE_SELECT,colour,0,0,0);
         //vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_SELECT,colour,0,0,0);
         RCCOLORREF pcol;
         pcol = vs->getcolour();
         glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
         GLfloat lwidth = vs->curPen.widthF();
         glLineWidth(lwidth<1.0?1.0:lwidth);

         //pt1 = vs->modeltoscreen(p1);
         //pt2 = vs->modeltoscreen(p2);

         Point pright = vs->getuaxis();
         Point pup = vs->getvaxis();
         double sc = vs->getpixelscale() * 6.0;

         vs->cbox2dGL(pt1,pright,pup,sc,sc);
         vs->cbox2dGL(pt2,pright,pup,sc,sc);

    }
}

void Line::draw(int drawmode,Transform *t,View3dSurface *vs, QPainter *painter)
{
    Point pt1,pt2;

    // get rid of the scene version
    //draw(DM_ERASE,t,vs);

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
    vs->setup(linestylescale,length(),drawmode,colour,style,weight,patternlinestyle);
    painter->setPen(vs->curPen);

    //vs->setup(linestylescale,length(),drawmode,0,0,0,0);
#if 1
    if (t == NULL && patternlinestyle == 0 && ! gethandles())
      {
         if (vs->displaylist.draw(this,drawmode,painter))
         {
             return;
         }
         if (! vs->displaylist.geterrorstatus())
           addtodisplaylist(vs,0);
         if (vs->displaylist.draw(this,drawmode,painter))
         {
             return;
         }
      }
#endif
    if (t == NULL)
    {
        pt1 = vs->modeltoview(p1);
        pt2 = vs->modeltoview(p2);
    }
    else
    {
         pt1 = vs->modeltoview(t->transform(p1));
         pt2 = vs->modeltoview(t->transform(p2));
    }
    if (vs->clip3d(&pt1,&pt2))
    {
         pt1 = vs->viewtoscreen(pt1);
         pt2 = vs->viewtoscreen(pt2);

         vs->line(painter,pt1.x,pt1.y,pt2.x,pt2.y);
    }

    if (patternlinestyle != 0)
      patternlinestyle->end();

    if (gethandles())
    {
         vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
         painter->setPen(vs->curPen);

         pt1 = vs->modeltoscreen(p1);
         pt2 = vs->modeltoscreen(p2);

         vs->line(painter,pt1.x-2,pt1.y-2,pt1.x+2,pt1.y-2);
         vs->line(painter,pt1.x+2,pt1.y-2,pt1.x+2,pt1.y+2);
         vs->line(painter,pt1.x+2,pt1.y+2,pt1.x-2,pt1.y+2);
         vs->line(painter,pt1.x-2,pt1.y+2,pt1.x-2,pt1.y-2);

         vs->line(painter,pt2.x-2,pt2.y-2,pt2.x+2,pt2.y-2);
         vs->line(painter,pt2.x+2,pt2.y-2,pt2.x+2,pt2.y+2);
         vs->line(painter,pt2.x+2,pt2.y+2,pt2.x-2,pt2.y+2);
         vs->line(painter,pt2.x-2,pt2.y+2,pt2.x-2,pt2.y-2);
    }
}

#endif

void Line::draw(int drawmode,Transform *t,View3dSurface *vs)
{
#ifdef USING_WIDGETS
  Point pt1,pt2;

  QGraphicsItem *_qgi = 0;


  //qDebug() << "Entering Line::draw() : qgi : " << qgi;
  //qDebug() << "current surface scene: " << vs->getScene();

  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() != vs->getScene())
          qgi->scene()->removeItem(qgi);
      else if(qgi && vs->getScene())
          vs->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      if(qgi1 && qgi1->scene() != vs->getScene())
        qgi1->scene()->removeItem(qgi1);
      delete qgi1;
      qgi1=0;

      if(qgi1 && qgi2->scene() != vs->getScene())
          qgi2->scene()->removeItem(qgi2);
      delete qgi2;
      qgi2=0;

  }

  //return;
  //if(drawmode == DM_ERASE || (drawmode != DM_INVERT && drawmode != DM_PRINT))
  if(drawmode == DM_ERASE)
      return;

  // KMJ: alternative method for grouping the qgi
  // i think it is too over kill for lines
#if 0
  qgi = new QGraphicsItemGroup();
#endif

  // KMJ: testing for speed up
  //if(getselected() || drawmode == DM_SELECT)
  //    return;

  //qDebug() << "drawing a line";
  //if(cadwindow->getcurrentsurface()->getrepaintstyle() != Wireframe && drawmode != DM_INVERT)
  //    return;

  if (getselected() && drawmode == DM_NORMAL)
      drawmode = DM_SELECT;
  vs->setup(linestylescale,length(),drawmode,colour,style,weight,patternlinestyle);
  //vs->setup(linestylescale,length(),drawmode,0,0,0,0);


#if 0
  if (t == NULL && patternlinestyle == 0 && ! gethandles())
    {
       if (vs->displaylist.draw(this,drawmode))
       {
           return;
       }
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode))
       {
           return;
       }
    }
#endif

  if (t == NULL)
    {  pt1 = vs->modeltoview(p1);
       pt2 = vs->modeltoview(p2);
    }
  else
    {  pt1 = vs->modeltoview(t->transform(p1));
       pt2 = vs->modeltoview(t->transform(p2));
    }

  if (vs->clip3d(&pt1,&pt2))
    {
       pt1 = vs->viewtoscreen(pt1);
       pt2 = vs->viewtoscreen(pt2);

#if 1
       qgi = vs->line((QGraphicsItem*)0,pt1.x,pt1.y,pt2.x,pt2.y);
#else
       // KMJ: alternative method for grouping the qgi
       _qgi = vs->line((QGraphicsItem*)0,pt1.x,pt1.y,pt2.x,pt2.y);
       if(_qgi)
           ((QGraphicsItemGroup*)qgi)->addToGroup(_qgi);
#endif
       //qDebug() << "Leaving Line::draw() : _qgi : " << _qgi;
    }
  if (patternlinestyle != 0)
  {
#if 1
      qgi = patternlinestyle->end(qgi);
#else
      // KMJ: alternative method for grouping the qgi
      _qgi = patternlinestyle->end(_qgi);
      if(_qgi)
          ((QGraphicsItemGroup*)qgi)->addToGroup(_qgi);
#endif
  }

  if(qgi)
  {
      if(getselected())
          qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
      else
          qgi->setZValue(zValue);

      if(qgi && dynamic_cast<View3dSubSurface*>(vs) != 0 && !usedByPlane)
      {
          if(((View3dSubSurface*)vs)->qgicp != 0 &&
             ((View3dSubSurface*)vs)->qgicp != qgi->parentItem())
           {
              // KMJ : NOTE:
              // don't make qgi null here because it may be used by a plane
              // a parent entity may use the qgi to reparent it
              // otherwise it is in the subsurface group
#if 1
              ((QGraphicsItemGroup*)((View3dSubSurface*)vs)->qgicp)->addToGroup(qgi);
              //qgi=0;
#else
              // KMJ: alternative method for adding to the viewport group
              qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);
              //qgi=0;
#endif
          }
          //else // KMJ: required for alternative method for grouping the qgi
          //    vs->getScene()->addItem(qgi);
      }
      //else // KMJ: required for alternative method for grouping the qgi
      //    vs->getScene()->addItem(qgi);
#ifdef NEW_RUBBERB
      // highjack the qgi for the rubberband
      if(drawmode == DM_INVERT)
      {
          if(qgi)
          {
              if(vs->qgirb == 0)
              {
                  vs->qgirb = new QGraphicsItemGroup();
                  vs->getScene()->addItem(vs->qgirb);
                  ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
              }
              else
                  ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
          }
          qgi=0;
      }
#endif
  }

  if (gethandles())
  {
       QGraphicsItem *qgline = 0;
       QGraphicsItemGroup *qgigroup = new QGraphicsItemGroup();

       vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL ,colour,0,0,0);
       pt1 = vs->modeltoscreen(p1);
       pt2 = vs->modeltoscreen(p2);

       if(qgigroup)
       {

           if(qgi1)
               qgi1->scene()->removeItem(qgi1);
           delete qgi1;
           qgi1=0;

           qgline = vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
           if(qgline != 0)
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
           if(qgline != 0)
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
           if(qgline != 0)
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);
           if(qgline != 0)
               qgigroup->addToGroup(qgline);

           qgi1 = qgigroup;
           vs->getScene()->addItem(qgi1);
       }

       qgigroup = new QGraphicsItemGroup();

       if(qgigroup)
       {
           if(qgi2)
               qgi2->scene()->removeItem(qgi2);
           delete qgi2;
           qgi2=0;

           qgline = vs->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2);
           if( qgline != 0 )
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2);
           if( qgline != 0 )
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2);
           if( qgline != 0 )
               qgigroup->addToGroup(qgline);
           qgline = vs->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2);
           if( qgline != 0 )
               qgigroup->addToGroup(qgline);

           qgi2 = qgigroup;
           vs->getScene()->addItem(qgi2);
       }
  }
  else
  {
      if(qgi1) qgi1->scene()->removeItem(qgi1);
      //if(qgi1) vs->getScene()->removeItem(qgi1);
      delete qgi1; qgi1=0;
      if(qgi2) qgi2->scene()->removeItem(qgi2);
      //if(qgi2) vs->getScene()->removeItem(qgi2);
      delete qgi2; qgi2=0;
  }


#else
    Point pt1,pt2;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,length(),drawmode,colour,style,weight,patternlinestyle);
  if (t == NULL && patternlinestyle == 0 && ! gethandles())
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  if (t == NULL)
    {  pt1 = vs->modeltoview(p1);
       pt2 = vs->modeltoview(p2);
    }
  else
    {  pt1 = vs->modeltoview(t->transform(p1));
       pt2 = vs->modeltoview(t->transform(p2));
    }
  if (vs->clip3d(&pt1,&pt2))
    {  pt1 = vs->viewtoscreen(pt1);
       pt2 = vs->viewtoscreen(pt2);
       vs->line(pt1.x,pt1.y,pt2.x,pt2.y);
    }
  if (patternlinestyle != 0)
    patternlinestyle->end();
  if (gethandles())
    {  vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
       pt1 = vs->modeltoscreen(p1);
       pt2 = vs->modeltoscreen(p2);
       vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
       vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
       vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
       vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);

       vs->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2);
       vs->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2);
       vs->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2);
       vs->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2);
    }
#endif
}

#ifdef USING_WIDGETS
int Line::hideGL(HideImage *image)
{
    image->getsurface()->setupGL(linestylescale,0.0,selected ? DM_SELECT : DM_NORMAL,colour,style,weight,0);
    image->hideGL(this,p1,p2);

    /*
     * // KMJ: pending
    if (selected && gethandles())
    {
         image->getsurface()->setupGL(linestylescale,0.0,DM_HANDLE_SELECT,colour,0,0,0);
         //vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_SELECT,colour,0,0,0);
         RCCOLORREF pcol;
         pcol = image->getsurface()->getcolour();
         glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
         GLfloat lwidth = image->getsurface()->curPen.widthF();
         glLineWidth(lwidth<1.0?1.0:lwidth);

         //pt1 = vs->modeltoscreen(p1);
         //pt2 = vs->modeltoscreen(p2);

         Point pright = image->getsurface()->getuaxis();
         Point pup = image->getsurface()->getvaxis();
         double sc = image->getsurface()->getpixelscale() * 6.0;

         image->getsurface()->cbox2dGL(p1,pright,pup,sc,sc);
         image->getsurface()->cbox2dGL(p2,pright,pup,sc,sc);

    }
    */

    return 1;
}
#endif

int Line::hide(HideImage *image)
{
    if(image->drawmode != 1) // 1 = printing
    {
        if(qgi)
            if(image->getsurface()->getScene())
                image->getsurface()->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
    }
    // handles ??do we see handles in hidden mode??
    if(qgi1)
        if(image->getsurface()->getScene())
            image->getsurface()->getScene()->removeItem(qgi1);
    delete qgi1;
    qgi1=0;

    if(qgi2)
        if(image->getsurface()->getScene())
            image->getsurface()->getScene()->removeItem(qgi2);
    delete qgi2;
    qgi2=0;

    qgi = new QGraphicsItemGroup();

    image->getsurface()->setup(linestylescale,0.0,selected ? DM_SELECT : DM_NORMAL,colour,style,weight,0);
    image->hide(this,p1,p2);

    if(qgi)
    {
        if(image->getsurface()->getScene())
            image->getsurface()->getScene()->addItem(qgi);
    }
    return 1;
}

double Line::pick(View3dSurface *vs,double x,double y,Point *pos)
{View3dWindow *window;
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
  if (pos == NULL && vs->displaylist.defined(this))
    return vs->displaylist.pick(this,x,y);
  return vs->pick(p1,p2,x,y,pos);
}

double Line::pickhor(View3dSurface *vs,int x,int y,Point *start,Point *end,Point *uvp1,Point *uvp2,double *side)
{Point p;
 double d;
  if (start == 0 && vs->displaylist.defined(this))
    return vs->displaylist.pickhor(this,x,y);
  d = vs->pickhor(p1,p2,x,y,&p,side);
  if (d >= 0.0 && start != 0)
    {  if ((p1 - p).length() < (p2 - p).length())
         {  *start = p2;
            *end = p1;
            *uvp1 = vs->modeltouv(p2);
            *uvp2 = vs->modeltouv(p1);
            *side = -*side;
         }
       else
         {  *start = p1;
            *end = p2;
            *uvp1 = vs->modeltouv(p1);
            *uvp2 = vs->modeltouv(p2);
         }
    }
  return d;
}

int Line::pickendpoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1,d2;
 int end;
  end = 0;
  if ((d1 = vs->pick(p1,distance,x,y)) >= 0.0)
    {  end = 1;  *pos = p1;
    }
  if ((d2 = vs->pick(p2,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = p2;
    }
  return end;
}

int Line::pickoriginpoint(View3dSurface *vs,int snap,int x,int y,Point *pos)
{double d1;
 int end;

  end = 0;
  if ((d1 = vs->pick((p1+p2)/2.0,snap,x,y)) >= 0.0)
    {  end = 3;  *pos = (p1+p2)/2.0;
    }

  return end;
}

int Line::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{ return pickendpoint(vs,distance,x,y,pos);
}

void Line::movehandlepoint(View3dSurface *,int drawentity,int end,Point p)
{ 
  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (end == 1) p1 = p;  else p2 = p;
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

extern BitMask *GetLockedLayerTable(void);

void Line::moveendpoint(int drawentity,int end,Point p)
{ 
  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (end == 3)
    p1 = p;
  else if (end == 4)
    p2 = p;
  else if (end == 1) 
    p1 = nearp(p);  
  else 
    p2 = nearp(p);
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

void Line::transform(Transform &t,int drawentity)
{ db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

double Line::gett(Point p)
{double l;
  if ((l = (p2 - p1).length()) < db.getptoler()) 
    return 0.0;
  return ((p2 - p1).dot(p - p1)) / l / l;
}

void Line::breakanddelete(int,Point p3,Point p4)
{double l,t1,t2,t;
 Point p;
 Line *line;
 Transform identity;

  if (GetLockedLayerTable()->test(layer)) return;  

  copye = NULL;
  p3 = nearp(p3);
  p4 = nearp(p4);
  if ((l = (p2 - p1).length()) < db.getptoler()) 
    return;
  t1 = ((p2 - p1).dot(p3 - p1)) / l;
  t2 = ((p2 - p1).dot(p4 - p1)) / l;
  if (t1 > t2)
    {  t = t1;  t1 = t2;  t2 = t;
       p = p3;  p3 = p4;  p4 = p;
    }
  if (t2 <= 0.0 || t1 >= l) 
    return;
  if (t1 <= 0.0 && t2 >= l) 
    return;
  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,this);
  if (t1 <= 0.0)
    moveendpoint(1,1,p4);
  else if (t2 >= l)
    moveendpoint(1,2,p3);
  else
    {  identity.identity();
       line = (Line *) clone(identity);
       moveendpoint(1,2,p3);
       line->moveendpoint(0,1,p4);
       draw(DM_NORMAL);
       db.geometry.add(line);
    }
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int Line::trace(View3dSurface *surface,Point end,Point uv1,Point uv2,double side,Point *newend,Point *uv3,Point *uv4,double *angle)
{double cosa,l1,l2;
 Point duv1,duv2;
  if ((p2 - p1).length() < db.getptoler())
    return 0;
  if ((p1 - end).length() < db.getptoler())
    {  *newend = p2;
       *uv3 = surface->modeltouv(p1);
       *uv4 = surface->modeltouv(p2);
    }
  else if ((p2 - end).length() < db.getptoler())
    {  *newend = p1;
       *uv3 = surface->modeltouv(p2);
       *uv4 = surface->modeltouv(p1);
    }
  else
    return 0;
  duv1 = uv1 - uv2;  duv1.z = 0.0;
  duv2 = *uv4 - *uv3;  duv2.z = 0.0;
  l1 = duv1.length();
  l2 = duv2.length();
  if (l1 > db.getptoler() && l2 > db.getptoler())
    {  cosa = duv1.dot(duv2) / l1 / l2;
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       *angle = acos(cosa);
       if (side * duv1.cross(duv2).z > 0.0)
         *angle = 2.0 * M_PI - *angle;
    }
  else
    *angle = 0.0;
  return 1;
}

LinearEntity *Line::divide(Point p, Point *dp)
{double t,l;
 Line *line;

   if (GetLockedLayerTable()->test(layer)) return 0;  

   line = 0;
   p = nearp(p);
   if ((l = (p2 - p1).length()) > db.getptoler())
     {  t = ((p2 - p1).dot(p) - (p2 - p1).dot(p1)) / l;
        if (t > 0.0 && t < l)
          {  draw(DM_ERASE);
             line = new Line(*this,p,p2);
             db.saveundo(UD_MOVED,this);
             cadwindow->invalidatedisplaylist(this);
             p2 = p;
             db.geometry.add(line);
#ifndef USING_WIDGETS
             if(dp !=0)
                *dp = p;
#else
             PointE point(p);
             //point.draw(DM_NORMAL);
             point.draw(DM_MARKER);
#endif
             draw(DM_NORMAL);
             if (getposition() != 0 && db.MovedCallback != 0)
               db.MovedCallback(this);
          }
     }
  return line;
}

/**
 * @brief Line::divide
 * @param n
 * @param dplist
 *
 * Divide entity inot a number of sections
 *
 * KMJ : working in Qt version
 */
void Line::divide(int n, PointList *dplist)
{int i;
 Point p3,p4,dir;

   if (GetLockedLayerTable()->test(layer)) return;  

   if ((p2 - p1).length() > db.getptoler())
     {  p3 = p1;  dir = p2 - p1;
        for (i = 1 ; i <= n ; i++)
          {  p4 = p1 + dir * (double) i / (double) n;
             if (i == 1)
               {  draw(DM_ERASE);
                  db.saveundo(UD_MOVED,this);
                  cadwindow->invalidatedisplaylist(this);
                  p2 = p4;
                  draw(DM_NORMAL);
                  if (getposition() != 0 && db.MovedCallback != 0)
                    db.MovedCallback(this);
               }
             else
               {  Line *line = new Line(*this,p3,p4);
                  db.geometry.add(line);
               }
#ifndef USING_WIDGETS
             if(dplist != 0)
                 dplist->add(p4);
#else
             PointE point(p4);
             //point.draw(DM_NORMAL);
             point.draw(DM_MARKER);
#endif
             p3 = p4;
          }
     }
}

int Line::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{  return (vs->pick(p1,inside,x1,y1,x2,y2) +
           vs->pick(p2,inside,x1,y1,x2,y2)) == 2;
}

int Line::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{int picked;
   View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
picked = inside != 2;
  vs->pick(nvertices,pwvertices,p1,p2,inside,&picked);
  return picked;
}

double Line::length(void) const
{  return (p2-p1).length();
}

Point Line::position(double x) const
{  return p1 + (p2-p1) * x;
}

Point Line::positionbylength(double l,double *x) const
{double ll;
  ll = length();
  if (ll > 1.0E-10)
    *x = l / ll;
  else
    *x = 0.0;
  return p1 + (p2-p1) * (*x);
}

Point Line::direction(double) const
{double llength;
  llength = this->length();
  if (llength > db.getptoler())
    return (p2 - p1) / llength;
  else
    {Point tmp(0.0,0.0,0.0);
       return tmp;
    }
}

Point Line::start(void)
{  return p1;
}

Point Line::end(void)
{  return p2;
}

int Line::npoints(void)
{  return 2;
}

Point Line::point(int i)
{   if (i == 0) 
        return p1; 
    else 
        return p2;
}

Point Line::nearp(Point p)
{double t,len;
 Point  dir;
 len = (p2-p1).length();
  if (len < db.getptoler())
    return (p1 + p2) / 2.0;
 dir = (p2 - p1) / len;
 t = dir.dot(p-p1);
 return p1 + dir * t;   
}

int Line::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int match1,match2;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  match1 = sp->match(p1);
  match2 = sp->match(p2);

  if (! match1 && ! match2) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0 && ! e->isa(group_entity))
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0 && ! e->isa(group_entity))
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
  if (match1)
    p1 = t.transform(p1);
  if (match2)
    p2 = t.transform(p2);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int Line::move(Database *db,Transform &t,int erase)
{Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 1;
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *Line::clone(Transform &t)
{ if (copye != NULL) return copye;
  copye = new Line(*this,t.transform(p1),t.transform(p2));
  copye->cloneattributes(this);
  return copye;
}

int Line::project(Database *db,Transform &t,int surface)
{BitMask options(32);
 EntityList list;
 Entity *e1,*e2;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point org,xa,ya,za;
 double D;
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;
 
  if ((status & 2) == 2) return 1;
  if (! db->geometry.add(e1 = new Line(*this,p1,t.transform(p1)))) return 0;
  if (! db->geometry.add(e2 = new Line(*this,p2,t.transform(p2)))) return 0;
  if (copye == NULL)
    {  if (! db->geometry.add(clone(t))) return 0;
    }
  if (surface)
    {  list.add(this);  list.add(e1);  list.add(copye);  list.add(e2);
       Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
       Loops loops(1,list);
       if (loops.planar(&org,&xa,&ya,&za,&D))
         {  Plane *surface = new Plane(*this,org,xa,ya,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
            if (surface != NULL && surface->getdefined())
              {  db->geometry.add(surface);
                 status |= 2;
              }
            else
              delete surface;
         }
    }
  return 1;
}

int Line::beginoffset(Point p,Point *zaxis,double *d,LinearEntity *enext,int *reverse,Point *startp)
{double d1,d2,d3,d4;
 Point p3;
  p3 = nearp(p);
  if (fabs(*d) < db.getptoler()) 
    {  InfinitePlane ip(*zaxis,p3);
       *d = (p3 - ip.nearp(p)).length();
    }
  if (zaxis->dot((p2-p1).cross(p - p3)) < 0.0) *zaxis = -*zaxis;
  if (enext == NULL)
    *reverse = 0;
  else
    {  d1 = (p1 - enext->start()).length();
       d2 = (p1 - enext->end()).length();
       if (d2 < d1) d1 = d2;
       d3 = (p2 - enext->start()).length();
       d4 = (p2 - enext->end()).length();
       if (d4 < d3) d3 = d4;
       if (fabs(d3) < db.getptoler() && fabs(d1) < db.getptoler() || d3 < d1)
         *reverse = 0;
       else
         {  *zaxis = -*zaxis;
            *reverse = 1;
         }
    }
  return offsetpoint(*zaxis,*d,*reverse,1,startp);
}

int Line::offset(Point zaxis,double d,int cornermode,LinearEntity *enext,int *reverse,Point *startp,EntityList *olist)
{Line *line;
 Circle *arc;
  if (enext == NULL || enext->isa(ellipse_entity) || enext->isa(curve_entity))
    {Point p3;
       if (! offsetpoint(zaxis,d,*reverse,2,&p3)) return 0;
       if ((line = new Line(*startp,p3)) == NULL) return 0;
       line->setcopye(this);
       olist->add(line);
       *startp = p3;
       *reverse = enext != 0 && (enext->position(0.0) - position(1.0-*reverse)).length() > (enext->position(1.0) - position(1.0-*reverse)).length();
    }
  else if (enext->isa(line_entity))
    {Point p3,p4,p5,yaxis1,yaxis2;
     double cosa;
     int rev;
     Line *lnext = (Line *) enext;
       yaxis1 = (zaxis.cross(((p2 - p1).normalize()))).normalize();
       if (*reverse)
         {  yaxis1 = -yaxis1;
            p3 = p1;
         }
       else
         p3 = p2;
       yaxis2 = (zaxis.cross(((lnext->p2 - lnext->p1).normalize()))).normalize();
       if ((lnext->p1 - p3).length() > (lnext->p2 - p3).length())
         {  rev = 1;
            yaxis2 = -yaxis2;
         }
       else
         rev = 0;  
       if (yaxis1.length() < db.getptoler() || yaxis2.length() < db.getptoler()) return 0;
       if (cornermode && zaxis.dot(yaxis1.cross(yaxis2)) < 0.0)
         {  if (! offsetpoint(zaxis,d,*reverse,2,&p4)) return 0;
            if (! enext->offsetpoint(zaxis,d,rev,1,&p5)) return 0;
            if ((line = new Line(*startp,p4)) == NULL) return 0;
            line->setcopye(this);
            olist->add(line);
            if ((arc = new Circle(1,p3,zaxis,p4,p5)) == NULL) return 0;
            if (arc->getdefined())
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p5;
            *reverse = rev;
         }
       else
         {  cosa = yaxis1.dot(yaxis2);
            if (cosa < -1.0) cosa = -1.0;  else if (cosa > 1.0) cosa = 1.0;
            cosa = cos(acos(cosa) / 2.0);
            if (cosa < db.getatoler()) return 0;
            p3 += ((yaxis1 + yaxis2).normalize()) * d / cosa;
            if ((line = new Line(*startp,p3)) == NULL) return 0;
            line->setcopye(this);
            olist->add(line);
            *startp = p3;
            *reverse = rev;
         }
    }
  else if (enext->isa(circle_entity))
    {Point p3,p4,p5,xaxis1,yaxis1,yaxis2;
     double x1,y1,r;
     int rev;
     Circle *cnext = (Circle *) enext;
       xaxis1 = (p2 - p1).normalize();
       yaxis1 = (zaxis.cross(xaxis1)).normalize();
       if (*reverse)
         {  yaxis1 = -yaxis1;
            p3 = p1;
         }
       else
         p3 = p2;
       p4 = p3 + yaxis1 * d;
       if ((cnext->position(0.0) - p3).length() > (cnext->position(1.0) - p3).length())
         {  yaxis2 = (zaxis.cross(-cnext->direction(1.0))).normalize();
            rev = 1;
         }
       else
         {  yaxis2 = (zaxis.cross(cnext->direction(0.0))).normalize();
            rev = 0;
         }
       if (xaxis1.length() < db.getptoler() || yaxis1.length() < db.getptoler() ||
           yaxis2.length() < db.getptoler()) return 0;
       if (! offsetpoint(zaxis,d,*reverse,2,&p4)) return 0;
       if (! enext->offsetpoint(zaxis,d,rev,1,&p5)) return 0;
       r = (p5 - cnext->getorigin()).length();
       y1 = fabs(yaxis1.dot(p4 - cnext->getorigin()));
       if (r < y1 || cornermode && zaxis.dot(yaxis1.cross(yaxis2)) < 0.0)
         {  if ((line = new Line(*startp,p4)) == NULL) return 0;
            line->setcopye(this);
            olist->add(line);
            if ((arc = new Circle(0,p3,-zaxis,p4,p5)) == NULL) return 0;
            if (arc->getdefined())
            {   olist->add(arc);
                arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p5;
            *reverse = rev;
         }
       else
         {  x1 = r * r - y1 * y1;  if (x1 < 0.0) x1 = 0.0;
            x1 = sqrt(x1);
            if (yaxis1.dot(p4 - cnext->getorigin()) < 0.0) y1 = -y1;
            if (xaxis1.dot(p3 - cnext->getorigin()) < 0.0) x1 = -x1;
            p4 = cnext->getorigin() + xaxis1 * x1 + yaxis1 * y1;
            if ((line = new Line(*startp,p4)) == NULL) return 0;
            line->setcopye(this);
            olist->add(line);
            *startp = p4;
            *reverse = rev;
         }
    }
  return 1;
}

int Line::offsetpoint(Point zaxis,double d,int reverse,int end,Point *p)
{Point xaxis,yaxis;
  yaxis = (zaxis.cross(((p2 - p1).normalize()))).normalize();
  if (yaxis.length() < db.getptoler()) return 0;
  yaxis *= d;
  if (reverse)
    {  if (end == 1)
         *p = p2 - yaxis;
       else
         *p = p1 - yaxis;
    }
  else
    {  if (end == 1)
         *p = p1 + yaxis;
       else
         *p = p2 + yaxis;
    }
  return 1;
}     

int Line::save(CadFile *outfile)
{
  short type;
  Byte terminators = 0;
  type = line_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! outfile->write(&terminators)) return 0;
  return 1;
}

Entity *Line::load(int,CadFile *infile)
{Line *line;
 EntityHeader *header;
 Point p1,p2;
 Byte terminators;
  if ((header = loadheader(infile)) == NULL) 
    return NULL;
  if (! p1.load(infile)) 
    return NULL;
  if (! p2.load(infile)) 
    return NULL;
  if (! infile->read(&terminators)) 
    return NULL;
  line = new Line(*header,p1,p2);
  line->atlist = header->getatlist();
  delete header;
  return line;
}

int Line::save(UndoFile *outfile)
{
  Line ln;
  size_t lns = sizeof(ln);
  size_t hs = sizeof(*getpatternlinestyle());
  size_t ts = sizeof(*this);
  size_t ls = sizeof(Line);

  short type;
  type = line_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  return 1;
}

Entity *Line::load(UndoFile *infile)
{Line *line;
 char bm[sizeof(BitMask)];
  if ((line = new Line) != NULL)
    {AttributeList al;
       memcpy(bm,&line->visible,sizeof(bm));
       if (! infile->blockread(line,sizeof(*line))) return 0;
       memcpy(&line->visible,bm,sizeof(bm));
       if (! infile->read(&line->visible)) return 0;
       if (! al.load(infile)) return 0;   line->atlist = al;
    }
  return line;
}

int Line::saveattributes(CadFile *)
{ return 1;
}

int Line::loadattributes(CadFile *)
{ return 1;
}

int Line::savedxf(int blocksection,DXFOut *dxffile)
{ if (! blocksection)
    {  if (! dxffile->writestring(0, (char*)"LINE")) return 0;
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbLine")) return 0;
       if (! dxffile->writereal(10,p1.x * dxffile->getscale())) return 0;
       if (! dxffile->writereal(20,p1.y * dxffile->getscale())) return 0;
       if (! dxffile->writereal(30,p1.z * dxffile->getscale())) return 0;
       if (! dxffile->writereal(11,p2.x * dxffile->getscale())) return 0;
       if (! dxffile->writereal(21,p2.y * dxffile->getscale())) return 0;
       if (! dxffile->writereal(31,p2.z * dxffile->getscale())) return 0;
    }
  return 1;
}

int Line::exportiges(FILE *outfile,int section,int,int *ndirectory,int *nparameter)
{ if (section == 0)
    {  if (fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",(*nparameter)+1,++(*ndirectory)) < 0) return 0;
       if (fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++(*ndirectory)) < 0) return 0;
       (*nparameter) += 2;
    }
  else 
    {  if (fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",p1.x,p1.y,p1.z,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       if (fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",p2.x,p2.y,p2.z,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       (*ndirectory) += 2;
    }
  return 1;
}

int Line::savegeometry(CadFile *outfile)
{ if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  return 1;
}

int Line::loadgeometry(CadFile *infile)
{ if (cadwindow != 0)
    cadwindow->invalidatedisplaylist(this);
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  return 1;
}

extern void StretchDimension(Dimension *dimension,int index,double length,EntityList *list);

class LineVerifyButtonControl : public ButtonDialogControl
  {public:
     LineVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int LineVerifyButtonControl::select(Dialog *dialog)
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
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    return 0;
}

void Line::verify(void)
{
  Dialog dialog("LineVerify_Dialog");
 int i,c,l,s;
 double length,w,a1,a2,a3,a4;
 RCCHAR direction[300],start[300],end[300],slength[300];
 Point p3,p4,p5,p6;
 Workplane *wp;
 BitMask changemask(32);

  if ((wp = db.workplanes.getcurrent()) != NULL)
    {  p3 = wp->modeltoworkplane(p1);
       p4 = wp->modeltoworkplane(p2);
    }
  else
    {  p3 = p1;  p4 = p2;
    }
  length = (p2 - p1).length();
  p5 = p4 - p3;  p5.z = 0.0;  p5.normalize();
  p6 = p4 - p3;  p6.x = sqrt(p6.x*p6.x + p6.y*p6.y);  p6.y = p6.z;  p6.z = 0.0;  p6.normalize();
  if (p5.x < -1.0) p5.x = -1.0;  else if (p5.x > 1.0) p5.x = 1.0;
  if (p6.x < -1.0) p6.x = -1.0;  else if (p6.x > 1.0) p6.x = 1.0;
  a1 = acos(p5.x)*180.0/M_PI;
  if (p5.y < 0.0) a1 =180.0 - a1;
  a2 = a1 - 180.0;
  a3 = acos(p6.x)*180.0/M_PI;
  if (p6.y < 0.0) a3 =180.0 - a3;
  a4 = a3 - 180.0;

  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new LineVerifyButtonControl(1000));
  dialog.add(new LineVerifyButtonControl(1001));
  dialog.add(new LineVerifyButtonControl(1002));
  dialog.add(new LineVerifyButtonControl(1003));

  dialog.add(new StringDialogControl(104,p3.string(start),128));
  dialog.add(new StringDialogControl(105,p4.string(end),128));
  dialog.add(new StringDialogControl(106,(p3-p4).string(direction),128));
  db.formatnumber(slength,length,1);
  //dialog.add(new StringDialogControl(107,slength,300));
  dialog.add(new RealDialogControl(107,&length,ScaleByLength));
  dialog.add(new RealDialogControl(108,&a1));
  dialog.add(new RealDialogControl(109,&a2));
  dialog.add(new RealDialogControl(110,&a3));
  dialog.add(new RealDialogControl(111,&a4));
  //dialog.add(new ButtonDialogControl(112));
  length = (p2 - p1).length();   
  dialog.setposition(DPOK);
  //if (dialog.process() == 112)
  if (dialog.process() == TRUE)
    {//  Insert a dimension, then change its length, to change the length of the line
       //double llength;
       //Dialog dialog(_RCT("Change Length"),_RCT("Length"),&llength,ScaleByLength);
       Point xaxis,yaxis,zaxis;

       //length = Line::length();
       db.saveundo(UD_STARTBLOCK,NULL);
       changemask.set(0);
       changemask.set(1);
       changemask.set(2);
       changemask.set(3);
       //setcolour(c);
       //setstyle(s);
       //setlayer(l);
       //setweight(db.lineweights.mmtoweight(w));
       change(changemask,c,l,s,db.lineweights.mmtoweight(w));

        if (cadwindow->getcurrentwindow() != 0 /*&& dialog.process() == TRUE*/)
         {  xaxis = cadwindow->getcurrentwindow()->getxaxis();
            yaxis = cadwindow->getcurrentwindow()->getyaxis();
            zaxis = xaxis.cross(yaxis);
            xaxis = Line::direction(0.0);
            if (state.getlastend() == 2)
              xaxis = -xaxis;
            yaxis = zaxis.cross(xaxis);

            state.destroyselection(1);

            SortedPointList spl;             
            CoordinateSystem cs(getp1() - yaxis * 10.0,xaxis,yaxis);
            double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
            int precision,tolprecision;
            TerminatorInfo term1,term2;
            TextInfo vinfo,tinfo,pinfo,sinfo;
            BitMask options(32);
            
            spl.add(cs.frommodel(getp1()));
            spl.add(cs.frommodel(getp2()));

            //db.saveundo(UD_STARTBLOCK,NULL);

            db.saveundo(UD_STARTBLOCK,NULL);
            LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            LinearDimension *ld = new LinearDimension(getp1() - yaxis * 10.0,xaxis,yaxis,spl,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
            for (i = 0 ; i < MaxViews ; i++)
              ld->getvisible()->set(i,0);
            db.geometry.add(ld);
            db.saveundo(UD_ENDBLOCK,NULL);

            db.saveundo(UD_STARTBLOCK,NULL);
            StretchDimension(ld,0,length,db.geometry.getlist());
            db.saveundo(UD_ENDBLOCK,NULL);

            db.saveundo(UD_STARTBLOCK,NULL);
            db.geometry.del(ld);
            db.saveundo(UD_ENDBLOCK,NULL);

            //db.saveundo(UD_ENDBLOCK,NULL);

         }

        db.saveundo(UD_ENDBLOCK,NULL);

    }

}

void Line::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{Point p3;
    //qDebug() << "in Line::extents";
    if (vs == NULL)
    {   *pmin = *pmax = p1;
        p3 = p2;
    }
    else
    {   *pmin = *pmax = vs->modeltouv(p1);
        p3 = vs->modeltouv(p2);
    }
    if (p3.x < pmin->x) 
        pmin->x = p3.x;
    if (p3.y < pmin->y) 
        pmin->y = p3.y;
    if (p3.z < pmin->z) 
        pmin->z = p3.z;
    if (p3.x > pmax->x)     
        pmax->x = p3.x;
    if (p3.y > pmax->y) 
        pmax->y = p3.y;
    if (p3.z > pmax->z) 
        pmax->z = p3.z;
}

int Line::issame(int geometry,const Entity &entity) const
{Line *line;
  if (! entity.isa(line_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         line = (Line *)&entity;
         if ((line->p1 - p1).length() < db.getptoler() &&
             (line->p2 - p2).length() < db.getptoler())
           return 1;
         else if ((line->p2 - p1).length() < db.getptoler() &&
                  (line->p1 - p2).length() < db.getptoler())
           return -1;
         else
           return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int Line::explode(void)
{ return 0;
}

void Line::reverse(void)
{Point p3;
  if (getposition() != 0)
    db.saveundo(UD_MOVED,this);
  p3 = p1;  p1 = p2;  p2 = p3;
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int Line::ison(Point p,int includeends)
{Point p3,dir;
  dir = (p2 - p1).normalize();
  p3 = nearp(p);
  return dir.dot(p3 - p1) > -2.0 && dir.dot(p3 - p2) < 2.0;
}

int Line::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int result,cstate;
 double wt;
 Point p;

    if (action == PP_CLICK)
    {   if (id == 1)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (id == 2)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (id == 3)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (id == 4)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else
            return 1;
    }
    else if (action == PP_GETVALUE)
    {   if (id == 1)
            sprintf((RCCHAR*)text,"%d",getcolour());
        else if (id == 2)
            sprintf((RCCHAR*)text,"%d",getlayer());
        else if (id == 3)
            sprintf((RCCHAR*)text,"%d",getstyle());
        else if (id == 4)
            sprintf((RCCHAR*)text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (id == 6)
            db.formatnumber((RCCHAR*)text,p1.x,FormatForEditField);
        else if (id == 7)
            db.formatnumber((RCCHAR*)text,p1.y,FormatForEditField);
        else if (id == 8)
            db.formatnumber((RCCHAR*)text,p1.z,FormatForEditField);
        else if (id == 9)
            db.formatnumber((RCCHAR*)text,p2.x,FormatForEditField);
        else if (id == 10)
            db.formatnumber((RCCHAR*)text,p2.y,FormatForEditField);
        else if (id == 11)
            db.formatnumber((RCCHAR*)text,p2.z,FormatForEditField);
        else if (id == 12)
        {   db.formatnumber((RCCHAR*)text,(p2-p1).x,0);
            return 1;
        }
        else if (id == 13)
        {   db.formatnumber((RCCHAR*)text,(p2-p1).y,0);
            return 1;
        }
        else if (id == 14)
        {   db.formatnumber((RCCHAR*)text,(p2-p1).z,0);
            return 1;
        }
        else if (id == 15)
        {   db.formatnumber((RCCHAR*)text,(p2 - p1).length(),0);
            return 1;
        }
    }
    else if (action == PP_SETVALUE)
    {Expression e((RCCHAR*)text);
     double x;

        if (id == 6 && e.evaluate(&x))
        {   p = p1;
            p.x = x * db.getmainunitscale();
            moveendpoint(1,3,p);
        }
        else if (id == 7 && e.evaluate(&x))
        {   p = p1;
            p.y = x * db.getmainunitscale();
            moveendpoint(1,3,p);
        }
        else if (id == 8 && e.evaluate(&x))
        {   p = p1;
            p.z = x * db.getmainunitscale();
            moveendpoint(1,3,p);
        }
        else if (id == 9 && e.evaluate(&x))
        {   p = p2;
            p.x = x * db.getmainunitscale();
            moveendpoint(1,4,p);
        }
        else if (id == 10 && e.evaluate(&x))
        {   p = p2;
            p.y = x * db.getmainunitscale();
            moveendpoint(1,4,p);
        }
        else if (id == 11 && e.evaluate(&x))
        {   p = p2;
            p.z = x * db.getmainunitscale();
            moveendpoint(1,4,p);
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy((RCCHAR*)text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy((RCCHAR*)text,"Line");
            return 1;
        }
        else 
        {   if (id == 1)
                strcpy((RCCHAR*)text,"Colour");
            else if (id == 2)
                strcpy((RCCHAR*)text,"Layer");
            else if (id == 3)
                strcpy((RCCHAR*)text,"Style");
            else if (id == 4)
                strcpy((RCCHAR*)text,"Weight");
            else if (id == 6)
                strcpy((RCCHAR*)text,"Start X");
            else if (id == 7)
                strcpy((RCCHAR*)text,"Start Y");
            else if (id == 8)
                strcpy((RCCHAR*)text,"Start Z");
            else if (id == 9)
                strcpy((RCCHAR*)text,"End X");
            else if (id == 10)
                strcpy((RCCHAR*)text,"End Y");
            else if (id == 11)
                strcpy((RCCHAR*)text,"End Z");
            else if (id == 12)
                strcpy((RCCHAR*)text,"DX");
            else if (id == 13)
                strcpy((RCCHAR*)text,"DY");
            else if (id == 14)
                strcpy((RCCHAR*)text,"DZ");
            else if (id == 15)
                strcpy((RCCHAR*)text,"Length");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 16; 
    
    return 0;
}

