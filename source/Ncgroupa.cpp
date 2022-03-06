
#include "ncwin.h"

//
//  options       Bit 0   :  Delete all children when the parent is deleted.
//
Group::Group(EntityList l,const char *c,const char *i,const BitMask &o)
 : options(32)
{
#ifdef USING_WIDGETS
  tmpclassname=0;
  tmpinstance=0;
#endif

    classname = new RCCHAR[strlen(c)+1];
    instance = new RCCHAR[strlen(i) + 1];
    if (classname == NULL || instance == NULL)
      {  delete [] classname;  delete [] instance;
         defined = 0;
         return;
      }
    strcpy(classname,c);
    strcpy(instance,i);
    list = l;
    options = o;
}

Group::Group(EntityList l,const RCCHAR *c,const RCCHAR *i,const BitMask &o)
    : options(32)
{
#ifdef USING_WIDGETS
  tmpclassname=0;
  tmpinstance=0;
#endif

  classname = new RCCHAR[strlen(c)+1];
  instance = new RCCHAR[strlen(i) + 1];
  if (classname == NULL || instance == NULL)
    {  delete [] classname;  delete [] instance;
       defined = 0;
       return;
    }
  strcpy(classname,c);
  strcpy(instance,i);
  list = l;
  options = o;
}

Group::Group(const EntityHeader &header,EntityList l,const char *c,const char *i,const BitMask &o)
  : Entity(header),options(32)
{
#ifdef USING_WIDGETS
  tmpclassname=0;
  tmpinstance=0;
#endif

    classname = new RCCHAR[strlen(c)+1];
    instance = new RCCHAR[strlen(i) + 1];
    if (classname == NULL || instance == NULL)
      {  delete [] classname;  delete [] instance;
         defined = 0;
         return;
      }
    strcpy(classname,c);
    strcpy(instance,i);
    list = l;
    options = o;
}
Group::Group(const EntityHeader &header,EntityList l,const RCCHAR *c,const RCCHAR *i,const BitMask &o)
  : Entity(header),options(32)
{
#ifdef USING_WIDGETS
  tmpclassname=0;
  tmpinstance=0;
#endif
  classname = new RCCHAR[strlen(c)+1];
  instance = new RCCHAR[strlen(i) + 1];
  if (classname == NULL || instance == NULL)
    {  delete [] classname;  delete [] instance;
       defined = 0;
       return;
    }
  strcpy(classname,c);
  strcpy(instance,i);
  list = l;
  options = o;

}

void Group::add(Entity *e)
{ db.saveundo(UD_CHANGEGROUP,this);
  if (! list.inlist(e))
    {  if (e->getposition() == 0)
         db.geometry.add(e);
       list.add(e);
       e->adda(this);
    }
}

void Group::add(int position,Entity *e)
{EntityList l;
 int i;
 Entity *eold;
  db.saveundo(UD_CHANGEGROUP,this);
  if (! list.inlist(e))
    {  for (list.start(),i = 0 ; (i < position) && (eold = list.next()) != 0 ; i++)
         l.add(eold);
       l.add(e);
       while ((eold = list.next()) != 0)
         l.add(eold);
       l.reverse();
       list.destroy();
       list = l;
       e->adda(this);
    }
}

void Group::del(Entity *e)
{ db.saveundo(UD_CHANGEGROUP,this);
  list.del(e);
  e->dela(this);
}

void Group::dbinsert(Database *db,int draw)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (db != NULL && e->getposition() == NULL) db->geometry.add(e,draw);
            e->adda(this);
         }
    }
  Entity::dbinsert(db,draw);
}

void Group::dbdelete(void)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         e->dela(this);
    }
  Entity::dbdelete();
}

void Group::dbsetassociated(void)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  e->dbsetassociated();
            e->setstatus(1);
         }
    }
}

void Group::dbtraverseassociated(EntityCallBack c,void *d)
{Entity *e;
  for (list.start() ; (e = list.next()) != 0 ; )
    {  e->dbtraverseassociated(c,d);
       c(e,d);
    }
}

Group::~Group()
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL && e->getposition() == NULL)
         delete e;
    }
  list.destroy();
  delete [] classname;
  delete [] instance;
#ifdef USING_WIDGETS
  delete [] tmpclassname;
  delete [] tmpinstance;
#endif
}

double Group::pick(View3dSurface *surface,double x,double y,Point *pos)
{Entity *e;
 double d,dmin;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(surface);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    surface = window->getoffscreenbitmap();
 

  if (state.groupnamemask[0] != 0 && strcmp(classname,state.groupnamemask) != 0)
    return -1;

  if (pos == NULL && surface->displaylist.defined(this))
    return surface->displaylist.pick(this,x,y);
  dmin = -1.0;
  for (list.start() ;  ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (e->isvisible(surface) && (d = e->pick(surface,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
              {  if (pos != 0 && state.getpickedentity() == 0)
                   state.setpickedentity(e);
                 dmin = d;
              }
         }
    }
  return dmin;
}

int Group::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{Entity *e;
 int count;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (state.groupnamemask[0] != 0 && strcmp(classname,state.groupnamemask) != 0)
    return 0;

  if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
  count = 0;
  for (list.start() ;  ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (e->isvisible(vs))
              {  count++;
                 if (! e->pick(vs,inside,x1,y1,x2,y2)) 
                   return 0;
              }
         }
    }
  return count > 0 ? 1 : ! inside;
}

int Group::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Entity *e;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (state.groupnamemask[0] != 0 && strcmp(classname,state.groupnamemask) != 0)
    return 0;

  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (e->isvisible(vs) && e->pick(vs,nvertices,pwvertices,inside) == 0)
              return 0;
         }
    }
  return 1;
}

void Group::addtodisplaylist(View3dSurface *,int)
{
}

#if USING_WIDGETS
char *Group::getcharclassname(void)
{
    delete [] tmpclassname;
    tmpclassname = new char[strlen(classname)+1];
    strcpy(tmpclassname,qPrintable(QString(classname)));
    return tmpclassname;

#if 0

#endif
}

char *Group::getcharinstance(void)
{
    delete [] tmpinstance;
    tmpinstance = new char[strlen(instance)+1];
    strcpy(tmpinstance,qPrintable(QString(instance)));
    return tmpinstance;
}

void Group::drawGL(int drawmode,Transform *transform,View3dSurface *surface)
{Entity *e;

  draw(DM_ERASE,transform,surface);

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  for (list.start() ; ! list.atend() ; )
    if ((e = list.next()) != NULL)
      if (e->isvisible(surface))
      {
          //e->setselected(getselected());
          //if(drawmode == DM_SELECT)
              e->drawGL(drawmode,transform,surface);
      }
}
#endif

void Group::draw(int drawmode,Transform *transform,View3dSurface *surface)
{Entity *e;

  //return;
  //if(getselected())
  //    return;
    if(drawmode == DM_ERASE)
    {
        for (list.start() ; ! list.atend() ; )
          if ((e = list.next()) != NULL)
            if (e->isvisible(surface))
            {
                e->draw(drawmode,transform,surface);
            }
        return;
    }

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  for (list.start() ; ! list.atend() ; )
    if ((e = list.next()) != NULL)
      if (e->isvisible(surface))
      {
          e->draw(drawmode,transform,surface);
      }
}

int Group::hide(HideImage *)
{ return 1;
}

int Group::hideGL(HideImage *)
{ return 1;
}

int Group::npoints(void)
{ return 0;
}

Point Group::point(int)
{Point tmp(0.0,0.0,0.0);  return tmp;
}

Point Group::nearp(Point p)
{Point p1,p2(0.0,0.0,0.0),dir;
 Entity *e;
 int defined;
  for (list.start(),defined = 0 ; (e = list.next()) != 0 ; )
    {  p1 = e->nearp(p);
       if (e->isa(line_entity))
         {  dir = p1 - ((Line *)e)->getp1();
            if (dir.length() > ((Line *)e)->length()*1.000001 || dir.dot(((Line *)e)->direction(0.0)) < -0.000001)
              continue;
         }
       if (defined && (p1 - p).length() < (p2 - p).length() || ! defined)
         p2 = p1;
       defined = 1;
    }
  return p2;
}

int Group::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{int stretched;
 EntityList newlist;
 Entity *e,*c;

  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }

  for (list.start(),stretched = 0 ; ! list.atend(); )
    {  if ((e = list.next()) != 0)
         {  e->stretchent(db,t,sp,erase);
            if ((e->getstatus() & ~ 8) != 0 ||
                e->getcopye() != 0 && (e->getcopye()->getstatus() & ~8) != 0)
              stretched = 1;
         }
    }
  if (! stretched) return 1;

  if (erase && (status & 16) == 0) draw(DM_ERASE);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus(getstatus() | 4);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db == 0 ? 1 : db->geometry.add(c);
           }
      }
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  if (e->getcopye() == NULL) 
              newlist.add(e);
            else
              newlist.add(e->getcopye());
         }
    }
  newlist.reverse();
  status |= 2 | 16;
  dbdelete();
  list.destroy();
  list = newlist;
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;
}

int Group::move(Database *db,Transform &t,int erase)
{Entity *e,*g;
 EntityList newlist;

  if (copye != NULL) 
    return 1;

  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {  if ((g = clone(t)) != 0 && db != 0)
           db->geometry.add(g);
         status &= ~1;  //  Remove this entity from the list to be processed
         return g != 0;
      }

  for (list.start() ; (e = list.next()) != NULL ;  )
    {  if (e->getcopye() == NULL) e->move(db,t,erase);
       if (e->getcopye() == NULL)
         {  newlist.destroy();  draw(DM_NORMAL);  return 0;
         }
       newlist.add(e->getcopye());
    }
  newlist.reverse();
  status |= 1;
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  dbdelete();
  list.destroy();
  list = newlist;
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *Group::clone(Transform &t)
{EntityList newlist;
 Entity *e;
  if (copye != NULL) return copye;
  list.reverse();
  for (list.start() ; ! list.atend(); )
    {  e = list.next();
       if (e->getcopye() == NULL)
            e->clone(t);
       if (e->getcopye() != NULL)
            newlist.add(e->getcopye());
    }
  list.reverse();
  //newlist.reverse();
  copye = new Group(*this,newlist,classname,instance,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int Group::project(Database *db,Transform &t,int surface)
{EntityList newlist;
 Entity *e;
  if (copye != NULL) return 1;
  for (list.start() ; (e = list.next()) != NULL ; )
    {  if (e->getcopye() == NULL) e->project(db,t,surface);
       if (e->getcopye() == NULL) return 0;
       newlist.add(e->getcopye());
    }
  newlist.reverse();
  copye = new Group(newlist,classname,instance,options);
  if (copye->getdefined())
    return db->geometry.add(copye);
  else
    {  delete copye;
       copye = NULL;
       return 0;
    }
}

void Group::change(const BitMask &change,const EntityHeader &header,const RCCHAR *c,const RCCHAR *i,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  if (change.test(0)) colour = header.getcolour();
  if (change.test(1)) layer = header.getlayer();
  if (change.test(2)) style = header.getstyle();
  if (change.test(3)) weight = header.getweight();
  if (change.test(4))
    {RCCHAR *s;
       if ((s = new RCCHAR[strlen(c) + 1]) != NULL)
         {  delete [] classname;
            classname = s;
            strcpy(classname,c);
         }
    }
  if (change.test(5))
    {RCCHAR *s;
       if ((s = new RCCHAR[strlen(i) + 1]) != NULL)
         {  delete [] instance;
            instance = s;
            strcpy(instance,i);
         }
    }
  if (change.test(6)) options = o;
  draw(DM_NORMAL);
}

int Group::setup(BitMask *change,RCCHAR *c,RCCHAR *i,BitMask *)
{
 Dialog dialog("Group_Dialog");
 ResourceString rs0(NCGROUPA),varies(4107);
 RCCHAR inc[300],ini[300];

 strcpy(inc,c); strcpy(ini,i);

 dialog.add(new StringDialogControl(100,inc,128));
 dialog.add(new StringDialogControl(101,ini,128));
 if (change != NULL)
   {  dialog.title(rs0.gets());
      change->clearall();
      //dialog.add(new CheckBoxDialogControl(200,change,4));
      //dialog.add(new CheckBoxDialogControl(201,change,5));
   }
   if(dialog.process() == TRUE)
   {
     if(change != NULL)
     {
         if(strcmp(c,inc) != 0)
             change->set(4);
         if(strcmp(i,ini) != 0)
             change->set(5);
     }
     strcpy(c,inc);
     strcpy(i,ini);
     return TRUE;
   }
 return FALSE;
}

void Group::getdefaults(RCCHAR **c,RCCHAR **m,BitMask *o)
{ *c = v.getstring("gp::classname");
  *m = v.getstring("gp::instance");
  *o = *v.getbm("gp::options");
}

void Group::setdefaults(RCCHAR *c,RCCHAR *i,const BitMask &o)
{ v.setstring("gp::classname",c);
  v.setstring("gp::instance",i);
  v.setbm("gp::options",o);
}

int Group::save(CadFile *outfile)
{short type;
  type = group_entity;

  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! outfile->write(classname)) return 0;
  if (! outfile->write(instance)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

Entity *Group::load(int references,CadFile *infile)
{Group *group=0;
 char *cc=0,*ci=0;
 RCCHAR *c=0,*i=0;
 EntityHeader *header=0;
 EntityList list,junklist;
 BitMask o(32);
  if ((header = loadheader(infile)) == NULL) return NULL;
  if (references)
    {  if (! list.load(infile)) return NULL;
    }
  else
    {  if (! junklist.load(infile)) return NULL;  junklist.destroy();
    }
  if(infile->getversion() == 2)
  {
      if (! infile->read(&cc)) return NULL;
      c = new RCCHAR[strlen(cc)+1];
      strcpy(c,cc);
      delete [] cc;

      if (! infile->read(&ci)) return NULL;
      i = new RCCHAR[strlen(ci)+1];
      strcpy(i,ci);
      delete [] ci;

  }
  else
  {
      if (! infile->read(&c))
        return NULL;

      if (! infile->read(&i))
        return NULL;

  }
  //if (! infile->read(&c))
  //  return NULL;
  //if (! infile->read(&i))
  //  return NULL;
  if (! infile->read(&o)) 
    return NULL;
  group = new Group(*header,list,c,i,o);
  group->atlist = header->getatlist();
  delete header;

  return group;
}

int Group::save(UndoFile *outfile)
{short type;
#ifdef USING_WIDGETS
  delete [] tmpclassname; tmpclassname=0;
  delete [] tmpinstance; tmpinstance=0;
#endif
  type = group_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! outfile->write(classname)) return 0;
  if (! outfile->write(instance)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

Entity *Group::load(UndoFile *infile)
{Group *group;
 EntityList list;
 RCCHAR bm[sizeof(BitMask)];
  if ((group = new Group) != NULL)
    {AttributeList al;
       memcpy(bm,&group->visible,sizeof(bm));
       if (! infile->blockread(group,sizeof(*group))) return 0;
       memcpy(&group->visible,bm,sizeof(bm));
       if (! infile->read(&group->visible)) return 0;
       if (! al.load(infile)) return 0;   group->atlist = al;
       if (! list.load(infile)) return 0;
       group->list = list;
       if (! infile->read(&group->classname)) return NULL;
       if (! infile->read(&group->instance)) return NULL;
       if (! infile->read(&group->options)) return 0;
#ifdef USING_WIDGETS
       delete [] group->tmpclassname;
       group->tmpclassname = new char[strlen(group->classname)+1];
       strcpy(group->tmpclassname,group->classname);
       delete [] group->tmpinstance;
       group->tmpinstance = new char[strlen(group->instance)+1];
       strcpy(group->tmpinstance,group->instance);
#endif
    }
  return group;
}

int Group::saveattributes(CadFile *outfile)
{ if (! saveheaderu(outfile)) return 0;
  if (! outfile->write(classname)) return 0;
  if (! outfile->write(instance)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

int Group::loadattributes(CadFile *infile)
{ if (! loadheaderu(infile)) return 0;
  if (! infile->read(&classname)) return NULL;
  if (! infile->read(&instance)) return NULL;
  if (! infile->read(&options)) return 0;
  return 1;
}

int Group::savedxf(int,DXFOut *)
{  return 0;
}

int Group::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int Group::savegeometry(CadFile *outfile)
{ if (! list.save(outfile)) return 0;
  return 1;
}

int Group::loadgeometry(CadFile *infile)
{ list.destroy();
  if (! list.load(infile)) return 0;
  return 1;
}

void Group::fixupreferences(Entity ** index)
{ list.fixupreferences(index);
  Entity::fixupreferences(index);
}


class GroupVerifyButtonControl : public ButtonDialogControl
  {public:
     GroupVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int GroupVerifyButtonControl::select(Dialog *dialog)
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

void Group::verify(void)
{
 Dialog dialog("GroupVerify_Dialog");
 int c,l,s;
 //void *data;
 double w;
 //BitMask o(32);
 RCCHAR *clp,*mip,cl[300],mi[300];
 //Entity *e;

 /*
  if (strcmp(classname,"RC::Panel") == 0)
    {Dialog dialog(_RCT("Panelverify_dialog"));
       EntityHeader::verify(&dialog,&c,&l,&s,&w);
       dialog.add(new StringDialogControl(104,instance,strlen(instance)+1));
       list.start();
       e = list.next();
       if (e->isa(plane_entity))
         area = ((Plane *)e)->getarea(20,1.0);
       else
         area = 0.0;
       dialog.add(new RealDialogControl(105,&area));
       //dialog.add(new ButtonDialogControl(106));
       dialog.add(new ButtonDialogControl(107));
       dialog.add(new ButtonDialogControl(108));
       dialog.add(new ButtonDialogControl(109));
       dialog.add(new ButtonDialogControl(110));
       dialog.add(new ButtonDialogControl(111));
       dialog.add(new ButtonDialogControl(112));
       dialog.add(new ButtonDialogControl(113));
       dialog.add(new ButtonDialogControl(114));
       dialog.add(new ButtonDialogControl(115));
       button = dialog.process();
       if (button == 106)
         group_command5(&state,NULL,&data);
       else if (button == 107)
         ::state.sendevent(new NewCommandEvent(1713,0));
       else if (button == 108)
         ::state.sendevent(new NewCommandEvent(1714,0));
       else if (button == 109)
         ::state.sendevent(new NewCommandEvent(1715,0));
       else if (button == 110)
         ::state.sendevent(new NewCommandEvent(1716,0));
       else if (button == 111)
         ::state.sendevent(new NewCommandEvent(1717,0));
       else if (button == 112)
         ::state.sendevent(new NewCommandEvent(1718,0));
       else if (button == 113)
         ::state.sendevent(new NewCommandEvent(1719,0));
       else if (button == 114)
         ::state.sendevent(new NewCommandEvent(1720,0));
       else if (button == 115)
         ::state.sendevent(new NewCommandEvent(1721,0));
    }
  else
  {
  */
       //group_command5(&state,NULL,&data);
       //Dialog dialog("GroupVerify_Dialog");
       // add the verify features
       EntityHeader::verify(&dialog,&c,&l,&s,&w);
       dialog.add(new GroupVerifyButtonControl(1000));
       dialog.add(new GroupVerifyButtonControl(1001));
       dialog.add(new GroupVerifyButtonControl(1002));
       dialog.add(new GroupVerifyButtonControl(1003));

       strcpy(cl,classname); strcpy(mi,instance);
       clp = cl; mip = mi;
       dialog.add(new StringDialogControl(104,cl,128));
       dialog.add(new StringDialogControl(105,mi,128));
       //dialog.add(new ButtonDialogControl(106));
       dialog.setposition(DPOK);
       if (dialog.process() == TRUE)
       {
           db.saveundo(UD_CHANGEATTRIBUTES,this);
           draw(DM_ERASE);
           setcolour((Byte)c);
           setlayer(l);
           setstyle(s);
           setweight(db.lineweights.mmtoweight(w));
           delete [] classname;
           classname =0;
           classname = new RCCHAR[strlen(cl)+1];
           if(classname)
               strcpy(classname,cl);
           delete [] instance;
           instance = 0;
           instance = new RCCHAR[strlen(mi)+1];
           if(instance)
               strcpy(instance,mi);
           draw(DM_NORMAL);
       }
  //}
}

int Group::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(group_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

void Group::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{Entity *e;
 Point pmin1,pmax1;
 Point MAXPOINT(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX),MINPOINT(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX);
  //pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
 *pmin = MAXPOINT;
 *pmax = MINPOINT;
  list.start();
  if ((e = list.next()) == NULL)
  {
      pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
      return;
  }
  e->extents(surface,pmin,pmax);
  for ( ; (e = list.next()) != NULL ; )
    {  e->extents(surface,&pmin1,&pmax1);
       if (pmin1.x < pmin->x) pmin->x = pmin1.x;
       if (pmin1.y < pmin->y) pmin->y = pmin1.y;
       if (pmin1.z < pmin->z) pmin->z = pmin1.z;
       if (pmax1.x > pmax->x) pmax->x = pmax1.x;
       if (pmax1.y > pmax->y) pmax->y = pmax1.y;
       if (pmax1.z > pmax->z) pmax->z = pmax1.z;
    }
}

int Group::explode(void)
{ return 0;
}

//#ifdef _USING_QTCHAR
//int Group::process_property(int action,int id,void *data,RCCHAR *text)
//#else
int Group::process_property(int action,int id,void *data,RCCHAR *text)
//#endif
{BitMask change_mask(128),new_options(32);
 int result,cstate;
 double wt;

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Entity");
            return 1;
        }
    }
    else if (id-- == 0)
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
    else if (id-- == 0)
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
    else if (id-- == 0)
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
    else if (id-- == 0)
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
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Group");
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Class");
        else if (action == PP_GETVALUE)
            strcpy(text,classname);
        else if (action == PP_SETVALUE)
        {   change_mask.set(4,1);
            change(change_mask,*this,text,0,new_options);
        }
    } 
    else if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Instance");
        else if (action == PP_GETVALUE)
            strcpy(text,instance);
        else if (action == PP_SETVALUE)
        {   change_mask.set(5,1);
            change(change_mask,*this,0,text,new_options);
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}




