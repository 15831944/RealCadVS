
#include "ncwin.h"
#include "RCPlatform.h"
#include "RCCentralWidget.h"
#include "RCView.h"
#include "trace_dialog.h"
#include "mainwindow.h"
#include "_tools.h"
//#include "RCOpenGL.h"
//#ifndef _USING_QT
#ifndef _MAC
#include <malloc.h>
#endif

void copyAlongCmdImplement(LinearEntity *entity, bool changeLocation);

// KMJ
// implemenation of the CloneInfo class
CloneInfo::CloneInfo()
{
    mode = 1;
    colour  = ::v.getinteger("df::colour");
    layer   = ::v.getinteger("df::layer");
    style   = ::v.getinteger("df::style");
    iweight = ::v.getinteger("df::weight");
    weight  = db.lineweights.weighttomm(iweight);
}
CloneInfo::CloneInfo(int m, int c, int l, int s, double w)
{
    mode = m;
    colour  = c;
    layer   = l;
    style   = s;
    weight  = w;
    iweight = db.lineweights.mmtoweight(w);
}
CloneInfo::CloneInfo(int m, int c, int l, int s, int iw)
{
    mode    = m;
    colour  = c;
    layer   = l;
    style   = s;
    iweight = iw;
    weight  = db.lineweights.weighttomm(iweight);
}
CloneInfo::CloneInfo(const CloneInfo &o)
{
    mode    = o.mode;
    colour  = o.colour;
    layer   = o.layer;
    style   = o.style;
    iweight = o.iweight;
    weight  = o.weight;
}

int AttributeList::save(CadFile *outfile)
{
 RCCHAR *attribute;
 short n;
  n = short(length());  outfile->write(&n);
  for (start() ; (attribute = next()) != NULL ; )
      if (! outfile->write(attribute)) return 0;
  return 1;
}

int AttributeList::load(CadFile *infile)
{
    char *cattribute=0;
    RCCHAR *attribute=0;
    short i,n;
    infile->read(&n);
    for (i = 0 ; i < n ; i++)
    {
        //if (! infile->read(&attribute))
        //    return 0;
        // undo/redo files are version 0
        if( infile->getversion() == 2)
        {
            if (! infile->read(&cattribute))
                return 0;
            attribute = new RCCHAR[strlen(cattribute)+1];
            strcpy(attribute,cattribute);
            delete [] cattribute;
        }
        else
        {
            if (! infile->read(&attribute))
                return 0;
        }

        add(attribute);
    }
    return 1;
}

EntityHeader::EntityHeader(int c,int l,int s,int w) : visible(MaxViews)
{
  refId = -1;
  colour = Byte(c);
  layer = short(l);
  style = Byte(s);
  linestylescale = 1.0;
  weight = Byte(w);
  visible.setandclear(end_list);
  patternlinestyle = 0;
  if (style == (Byte)255)
    {  patternlinestyle = new PatternLineStyle(v.getreal("ps::height"),v.getreal("ps::lengthscale"),v.getreal("ps::shear"),v.getreal("ps::gap"),0.0,0.0,v.getinteger("ps::orientation"),v.getinteger("ps::fitting"),v.getinteger("ps::size"),v.getstring("ps::filename"));
    }
}

EntityHeader::EntityHeader() : visible(MaxViews)
{
  refId = -1;
  colour = db.header.colour;
  layer = db.header.layer;
  style = db.header.style;
  linestylescale = 1.0;
  weight = db.header.weight;
  visible = db.header.visible;
  patternlinestyle = 0;
  if (style == (Byte)255)
    {  patternlinestyle = new PatternLineStyle(v.getreal("ps::height"),v.getreal("ps::lengthscale"),v.getreal("ps::shear"),v.getreal("ps::gap"),0.0,0.0,v.getinteger("ps::orientation"),v.getinteger("ps::fitting"),v.getinteger("ps::size"),v.getstring("ps::filename"));
    }
}

void EntityHeader::setpatternlinestyle(PatternLineStyle *pls)
{  delete patternlinestyle;  patternlinestyle = pls;
}

EntityHeader::~EntityHeader()
{ delete patternlinestyle;
}

void EntityHeader::CopyAttributes(Entity *e)
{AttributeList atlist;
  RCCHAR *a,*attribute;
  atlist = getatlist();
  for (atlist.start() ; (a = atlist.next()) != 0 ; )
    {  attribute = new RCCHAR[strlen(a)+1];
       strcpy(attribute,a);
       e->addat(attribute);
    }
}

DefaultChangedCallback defaultchangedcallback = 0;

void SetDefaultChangedCallback(DefaultChangedCallback dcc)
{ defaultchangedcallback = dcc;
}

/*
void DCCallback(int type)
{

  SetDefaultChangedCallback(DCCallback);

  if (db.header.getcolour() == 56)
    {  db.header.change(0,0,0,0);
       v.setinteger("df::colour",0);
       v.setinteger("df::layer",0);
       v.setinteger("df::style",0);
       v.setinteger("df::weight",0);
    }

}
*/


void EntityHeader::change(int c,int l,int s,int w)
{int changed;


  changed = DCLayer * (l != layer) + DCColour * (c != colour) +
            DCStyle * (s != style) + DCWeight * (w != weight);

  colour = Byte(c);
  layer = short(l);
  style = Byte(s);
  weight = Byte(w);
  if (style != (Byte)255)
    {  delete patternlinestyle;  patternlinestyle = 0;
    }
  else
    {  delete patternlinestyle;
       patternlinestyle = new PatternLineStyle(v.getreal("ps::height"),v.getreal("ps::lengthscale"),v.getreal("ps::shear"),v.getreal("ps::gap"),0.0,0.0,v.getinteger("ps::orientation"),v.getinteger("ps::fitting"),v.getinteger("ps::size"),v.getstring("ps::filename"));
    }

  if (changed && defaultchangedcallback != 0)
    defaultchangedcallback(changed);

}

int EntityHeader::issame(const EntityHeader &header)
{  return layer == header.layer && colour == header.colour &&
          style == header.style && weight == header.weight;
}

class CommandIntegerDialogControl : public IntegerDialogControl
  {public:
     CommandIntegerDialogControl(int id,int *c) : IntegerDialogControl(id,c) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
  };

void CommandIntegerDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{
  PostMessage(cadwindow->gethwnd(),WM_COMMAND,2900+id,0);
  IntegerDialogControl::lbuttondblclkevent(dialog,x,y,b);
}

class CommandRealDialogControl : public RealDialogControl
  {public:
     CommandRealDialogControl(int id,double *c) : RealDialogControl(id,c) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
  };

void CommandRealDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{ PostMessage(cadwindow->gethwnd(),WM_COMMAND,2900+id,0);
  RealDialogControl::lbuttondblclkevent(dialog,x,y,b);
}

class CommandStringDialogControl : public StringDialogControl
  {public:
     CommandStringDialogControl(int id,RCCHAR *c,int l) : StringDialogControl(id,c,l) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
  };

void CommandStringDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{ PostMessage(cadwindow->gethwnd(),WM_COMMAND,2900+id,0);
  StringDialogControl::lbuttondblclkevent(dialog,x,y,b);
}

class VerifyIntegerDialogControl : public IntegerDialogControl
{
public:
     VerifyIntegerDialogControl(int id,int *c) : IntegerDialogControl(id,c) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
};

void VerifyIntegerDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{
   int state = 1000;
   if(id==100)
   {
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        int colour = db.header.getcolour();
        general_property_command(&state,0,(void **)&colour);
        ((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
   }
   else if(id==102)
   {
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
       int style = db.header.getstyle();
       general_property_command(&state,0,(void **)&style);
       ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
   }
}

class VerifyRealDialogControl : public RealDialogControl
{
public:
     VerifyRealDialogControl(int id,double *c) : RealDialogControl(id,c) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
};

void VerifyRealDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{
   int state = 1000;
   if(id==103)
   { // weight
       WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
       int iweight = db.header.getweight();
       double weight = db.lineweights.weighttomm(iweight);
       general_property_command(&state,0,(void **)&weight);
       ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
       //((WeightAssocRealDialogControl *)dialog->getcontrol(104))->change(dialog,weight);
   }
}

class VerifyStringDialogControl : public StringDialogControl
{
public:
     VerifyStringDialogControl(int id,RCCHAR *c,int l) : StringDialogControl(id,c,l) {}
     void lbuttondblclkevent(Dialog *,int,int,int);
};

void VerifyStringDialogControl::lbuttondblclkevent(Dialog *dialog,int x,int y,int b)
{
    int state = 1000;
    if(id==101)
    { // layer
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        int layer = db.header.getlayer();
        general_property_command(&state,0,(void **)&layer);
        RCCHAR layername[300];
        if (strlen(db.layernames.getlayername(layer)) == 0)
          sprintf(layername,"%d",layer);
        else
          sprintf(layername,"%d:%ls",layer,db.layernames.getlayername(layer));
        ((StringDialogControl *)dialog->getcontrol(101))->change(dialog,layername);
    }
}

#if 1
void EntityHeader::verify(Dialog *dialog,int *c,int *l,int *s,double *w) const
{
  static RCSTRING layernames[MAX_LAYERS];
  static RCCHAR layername[300];
  //*c = colour;  dialog->add(new CommandIntegerDialogControl(100,c));
  *c = colour;
  //dialog->add(new VerifyIntegerDialogControl(100,c));
  dialog->add(new IntegerDialogControl(100,c));

  *l = layer;
  if (strlen(db.layernames.getlayername(*l)) == 0)
    sprintf(layername,"%d",*l);
  else
    sprintf(layername,"%d:%ls",*l,db.layernames.getlayername(*l));
  //dialog->add(new CommandStringDialogControl(101,layername,300));
  //dialog->add(new VerifyStringDialogControl(101,layername,300));
  dialog->add(new IntegerDialogControl(101,l));

  *s = style;
  //dialog->add(new CommandIntegerDialogControl(102,s));
  //dialog->add(new VerifyIntegerDialogControl(102,s));
  dialog->add(new IntegerDialogControl(102,s));

  *w = db.lineweights.weighttomm(weight);
  //dialog->add(new CommandRealDialogControl(103,w));
  //dialog->add(new VerifyRealDialogControl(103,w));
  dialog->add(new WeightAssocRealDialogControl(103,w,0.1));
}
#else
void EntityHeader::verify(Dialog *dialog,int *c,int *l,int *s,double *w) const
{
  static RCCHAR layername[300];
  *c = colour;  dialog->add(new CommandIntegerDialogControl(100,c));
  *l = layer;
  if (strlen(db.layernames.getlayername(*l)) == 0)
    sprintf(layername,"%d",*l);
  else
    sprintf(layername,"%d:%ls",*l,db.layernames.getlayername(*l));
  dialog->add(new CommandStringDialogControl(101,layername,300));
  *s = style;
  dialog->add(new CommandIntegerDialogControl(102,s));
  *w = db.lineweights.weighttomm(weight);
  dialog->add(new CommandRealDialogControl(103,w));
}
#endif

EntityHeader::EntityHeader(const EntityHeader &header) : visible(MaxViews)
{AttributeList aempty;
  refId = header.refId;
  layer = header.layer;
  colour = header.colour;
  style = header.style;
  linestylescale = header.linestylescale;
  weight = header.weight;
  visible = header.visible;
  atlist = aempty;
  if (header.patternlinestyle == 0)
    patternlinestyle = 0;
  else
    patternlinestyle = new PatternLineStyle(header.patternlinestyle);
  relatedlist = header.relatedlist;

}

void Entity::change(const BitMask &change,int c,int l,int s,int w)
{
  // KMJ: not sure what change bit 31 means in this case
  // maybe the entity is hidden ie not displayed???
#ifdef NEW_DRAGGING
    // nothing to when using gl
#else
  if (! change.test(31))
    draw(DM_ERASE);
#endif
  db.saveundo(UD_CHANGEHEADER,this);
  if (change.test(0)) colour = Byte(c);
  if (change.test(1)) layer = short(l);
  if (change.test(2))
    {  style = Byte(s);
       if (style != (Byte)255)
         {  delete patternlinestyle;  patternlinestyle = 0;
         }
       else
         {  delete patternlinestyle;
            patternlinestyle = new PatternLineStyle(v.getreal("ps::height"),v.getreal("ps::lengthscale"),v.getreal("ps::shear"),v.getreal("ps::gap"),0.0,0.0,v.getinteger("ps::orientation"),v.getinteger("ps::fitting"),v.getinteger("ps::size"),v.getstring("ps::filename"));
         }
    }
  if (change.test(3)) weight = Byte(w);
#ifdef NEW_DRAGGING
  // nothing to when using gl
#else
  if (! change.test(31))
    draw(DM_NORMAL);
#endif
}

//  Copy the attributes of entity e onto this entity.
void Entity::cloneattributes(Entity *e)
{AttributeList atlist;
 RCCHAR *at,*newat;
 Entity *ce;

  atlist = e->getatlist();
  for (atlist.start() ; (at = atlist.next()) != 0 ; )
    if ((newat = new RCCHAR[strlen(at)+1]) != 0)
      {  strcpy(newat,at);
         addat(newat);
         ce = copye;
         setattribute(newat);
         db.saveundo(UD_INSERTATT,this);
         copye = ce;  //  Make sure that the copied entity pointer is not modified
      }
}

void Entity::dbinsert(Database *db,int draw)
{Entity *e;
  for (relatedlist.start() ; ! relatedlist.atend() ; )
    {  if ((e = relatedlist.next()) != NULL)
         {  if (db != NULL && e->getposition() == NULL)
              db->geometry.add(e,draw);
            e->adda(this);
         }
       relatedlist.next();  //  Skip application ID
       relatedlist.next();  //  Skip relationship
    }

}

void Entity::fixupreferences(Entity ** index)
{EntityList rlist;
 Entity *e;

  //  Fixup the related entity list
  for (relatedlist.start() ; ! relatedlist.atend() ; )
    {  if ((e = relatedlist.next()) != NULL)
         rlist.add(index[long(e)]);
       e = relatedlist.next();  rlist.add(e);
       e = relatedlist.next();  rlist.add(e);
    }

  relatedlist.destroy();
  rlist.reverse();
  relatedlist = rlist;

}


int Entity::addrelationship(Entity *relatedentity,long ApplicationID,long Relationship)
{ relatedentity->adda(this);
  relatedlist.add((Entity *)Relationship);
  relatedlist.add((Entity *)ApplicationID);
  return relatedlist.add(relatedentity);
}

int Entity::deleterelationship(Entity *relatedentity,long ApplicationID,long Relationship)
{Entity *e;
 long ID,R;
 EntityList rlist;
 int deleted;

  deleted = 0;
  for (relatedlist.start() ; ! relatedlist.atend() ; )
    {  e = relatedlist.next();
       ID = (long) relatedlist.next();
       R = (long) relatedlist.next();
       if (e != relatedentity || ID != ApplicationID || R != Relationship)
         {  rlist.add(e);
            rlist.add((Entity *)ID);
            rlist.add((Entity *)R);
         }
       else
         {  deleted = 1;
            relatedentity->dela(this);
         }

    }
  relatedlist.destroy();
  rlist.reverse();
  relatedlist = rlist;

  return deleted;
}

int Entity::getrelationship(int index,Entity **relatedentity,long *ApplicationID,long *Relationship)
{int i;

  for (relatedlist.start(),i = 0 ; ! relatedlist.atend() ; i++)
    {  if (i == index)
         {  *relatedentity = relatedlist.next();
            *ApplicationID = (long) relatedlist.next();
            *Relationship = (long) relatedlist.next();
            return 1;
         }
       else
         {  relatedlist.next();
            relatedlist.next();
            relatedlist.next();
         }
    }

  return 0;
}

void Entity::dbdelete(void)
{Entity *e;
  for (relatedlist.start() ; ! relatedlist.atend() ; )
    {  if ((e = relatedlist.next()) != NULL)
         e->dela(this);
       relatedlist.next();  //  Skip application ID
       relatedlist.next();  //  Skip relationship
    }
}

SortedPointTable::~SortedPointTable()
{ list.destroy();
  delete [] sortedpoints;
}

int SortedPointTable::add(Point p)
{  return list.add(p);
}

int SortedPointTable::endlist(void)
{int i;
  n = list.length();
  if ((sortedpoints = new Point[n]) == NULL) return 0;
  for (list.start(),i = 0 ; ! list.atend() ; i++)
    sortedpoints[i] = list.next();
  list.destroy();
  return 1;
}

int SortedPointTable::match(Point p)
{int i;
  for (i = 0 ; i < n ; i++)
    if ((sortedpoints[i] - p).length() < db.getptoler()) return 1;
  return 0;
}

int Entity::stretch(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e;
 int stretched;

  stretched = 0;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    {  stretched = 1;
       if (! e->stretch(db,t,sp,erase))
         return 0;
    }

  if (! stretched)
    return stretchent(db,t,sp,erase);

  return 1;
}


int Entity::isvisible(View3dSurface *surface)
{

    //qDebug() << "isvisible()";
    if(surface != 0)
    {
        //qDebug() << "surface :" << surface->gethwnd();
        //qDebug() << "surface->getView :" << surface->getview();
        //qDebug() << "view->getviewno  :" << surface->getview()->getviewno();
        //qDebug() << "visible.test : " << visible.test(surface->getview()->getviewno());
        //qDebug() << "displaylayertable : " << surface->getdisplaylayertable();
        //qDebug() << "entity layer : " << layer;
        if(surface->getdisplaylayertable() != 0)
            {}//qDebug() << "testdisplaylayer : " << surface->testdisplaylayer(layer);
        else
            {}//qDebug() << "db.dismask.layer.test : " <<  db.dismask.layer.test(layer);
    }

    return (surface == NULL ||
			visible.test(surface->getview()->getviewno())) &&		
            (surface != 0 && surface->getdisplaylayertable() != 0 ? surface->testdisplaylayer(layer) : db.dismask.layer.test(layer)) &&
             match(db.dismask.entity) &&
             db.dismask.colour.test(colour) &&
             db.dismask.style.test(style) &&
             db.dismask.weight.test(weight);
}

int Entity::isselectable(View3dSurface *surface)
{
    return isvisible(surface) &&
          match(state.selmask.entity) &&
          match(state.tempentitymask) &&
          state.selmask.colour.test(colour) &&
         (surface != 0 && surface->getselectionlayertable() != 0 ? surface->testselectionlayer(layer) : state.selmask.layer.test(layer)) &&
          state.selmask.style.test(style) &&
          state.selmask.weight.test(weight);
}

int EntityHeader::saveheader(CadFile *outfile)
{short l;
  l = (layer&0x0fff) | 0x8000 * (patternlinestyle != 0);
  l = l | 0x4000 * (relatedlist.length() > 0);
  l = l | 0x2000 * (linestylescale != 1.0);
  if (!outfile->write(&refId)) return 0;
  if (! outfile->write(&l)) return 0;
  if (! outfile->write(&colour)) return 0;
  if (! outfile->write(&style)) return 0;
  if (! outfile->write(&weight)) return 0;
  if (! outfile->write(&visible)) return 0;
  if (patternlinestyle != 0 && ! patternlinestyle->save(outfile)) return 0;
  if (relatedlist.length() > 0)
    {Entity *e;
     short nvalues;
      long v;
       nvalues = short(relatedlist.length());
       if (! outfile->write(&nvalues)) return 0;
       nvalues = (nvalues / 3) * 3;
       for (relatedlist.start() ; ! relatedlist.atend() ; )
         {  e = relatedlist.next();
            v = (e == NULL ? 0L : outfile->getentityindex(e));
            if (! outfile->write(&v)) return 0;
            v = (long)relatedlist.next();
            if (! outfile->write(&v)) return 0;
            v = (long)relatedlist.next();
            if (! outfile->write(&v)) return 0;
         }
    }
  if (linestylescale != 1.0)
    if (! outfile->write(&linestylescale)) return 0;

  if (! atlist.save(outfile)) return 0;
  return 1;
}

EntityHeader *EntityHeader::loadheader(CadFile *infile)
{EntityHeader *header;
 PatternLineStyle *pls;
 short layer;
 long entityId;
 Byte colour,style,weight;
 BitMask visible(MaxViews);
 float linestylescale;

  if ((header = new EntityHeader) == NULL)
        return NULL;
  if (!infile->read(&entityId))
  {
	  delete header;
	  return NULL;
  }
  if (! infile->read(&layer))
  {
      delete header;
      return NULL;
  }
  if (! infile->read(&colour))
  {
      delete header;
      return NULL;
  }
  if (! infile->read(&style))
  {
      delete header;
      return NULL;
  }
  if (! infile->read(&weight))
  {
      delete header;
      return NULL;
  }
  if (! infile->read(&visible))
  {
      delete header;
      return NULL;
  }
  if (layer & 0x8000)
    {  pls = new PatternLineStyle;
       pls->load(infile);
       layer &= 0x7FFF;
    }
  else
    pls = 0;


  if (layer & 0x4000)
    {//  Load the related attribute list
     short nvalues;
     long v;
     int i;

       if (! infile->read(&nvalues))
       {
           delete header;
           return NULL;
       }
       for (i = 0 ; i < nvalues ; i++)
         {
             if (! infile->read(&v))
             {
                 delete header;
                 return NULL;
             }
            header->relatedlist.add((Entity *)v);
         }
       header->relatedlist.reverse();

       layer &= ~0x4000;
    }

  if (layer & 0x2000)
    {
        if (! infile->read(&linestylescale))
        {
            delete header;
            return NULL;
        }
       layer &= ~0x2000;
    }
  else
    linestylescale = 1.0;

  if (! header->atlist.load(infile))
  {
      delete header;
      return NULL;
  }
 
  header->refId = entityId;
  header->layer = layer;
  header->colour = colour;
  header->style = style;
  if(infile->getversion() == 2)
  {
      header->weight = (Byte)db.lineweights.weightfromold((int)weight);
  }
  else
    header->weight = weight;
  header->visible = visible;
  header->patternlinestyle = pls;
  header->linestylescale = linestylescale;
  return header;
}

int EntityHeader::saveheaderu(CadFile *outfile)
{ if (! outfile->write(&layer)) return 0;
  if (! outfile->write(&colour)) return 0;
  if (! outfile->write(&style)) return 0;
  if (! outfile->write(&weight)) return 0;
  return 1;
}

int EntityHeader::loadheaderu(CadFile *infile)
{ if (! infile->read(&layer)) return 0;
  if (! infile->read(&colour)) return 0;
  if (! infile->read(&style)) return 0;
  if (! infile->read(&weight)) return 0;
  return 1;
}

int EntityHeader::saveheaderv(CadFile *outfile)
{ if (! outfile->write(&visible)) return 0;
  return 1;
}

int EntityHeader::loadheaderv(CadFile *infile)
{ if (! infile->read(&visible)) return 0;
  return 1;
}

Entity::Entity(void)
{
#ifdef USING_WIDGETS
    zValue = QGDRAGZVALUE(QGMAXZVALUE);
    qgi = 0 ;
    //qgiLines.clear();
    //qgi3dVitems.clear();
    usedByPlane=false;
    usedByFigure=false;
#endif
  defined = 1;
  position = NULL;
  copye = NULL;
  index = 0;
  status = 0;
  selected = 0;
  id = -1;
}

Entity::Entity(const EntityHeader &header) : EntityHeader(header)
{
#ifdef USING_WIDGETS
    zValue = QGDRAGZVALUE(QGMAXZVALUE);
    qgi = 0 ;
    //qgiLines.clear();
    //qgi3dVitems.clear();
    usedByPlane=false;
    usedByFigure=false;
#endif

  EntityList empty;
  defined = 1;
  position = NULL;
  copye = NULL;
  index = 0;
  status = 0;
  selected = 0;
  id = -1;
  alist = empty;
}
#ifndef USING_WIDGETS
void Entity::putqgi(Surface *s,QGraphicsItem *gi)
{
    if(dynamic_cast<View3dSubSurface*>(s)!=0)
    {
        if(s->qsidx < qgiLines.size())
            qgiLines[s->qsidx] = gi;
        else
            qgiLines.push_back(gi);
    }
    else
    {
        if(!qgi3dVitems.contains(gi))
            qgi3dVitems.push_back(gi);
        /*
        int index = 0;
        if(s != 0)
            index = s->qsidx;
        if(s->qsidx < qgi3dVitems.size())
        {
            qgi = gi;
            qgi3dVitems[index] = qgi;
        }
        else
        {
            qgi = gi;
            qgi3dVitems.push_back(qgi);
        }
        */
    }
    qgi = gi;
}
QGraphicsItem * Entity::getqgi(Surface *s)
{
    if(dynamic_cast<View3dSubSurface*>(s)!=0)
    {
        if(qgiLines.size() > 0 && s->qsidx < qgiLines.size())
            return qgiLines[s->qsidx];
        else
            return 0;
    }
    else
    {
        for(int i=0; i<qgi3dVitems.size(); i++)
        {
            if(qgi3dVitems.at(i)->scene() == s->getScene())
            {
                qgi = qgi3dVitems.at(i);
                qgi3dVitems.removeAt(i);
                return qgi;
            }
        }
        return 0;
        /*
        int index = 0;
        if(s != 0)
            index = s->qsidx;
        if(qgi3dVitems.size() > 0 && s->qsidx < qgi3dVitems.size())
        {
            qgi = qgi3dVitems[index];
            return qgi;
        }
        else
        {
            //qgi = 0;
            return qgi;
        }
        */
    }
    return qgi;
}
#endif

Entity::~Entity()
{

  RCCHAR *at=0;
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    delete [] at;
  atlist.destroy();
  alist.destroy();

  //qDebug() << "qgi3dVitems.size() : " << qgi3dVitems.size();
}

void Entity::highlight()
{
  int hashandles;
  hashandles = selected & 128;
  selected &= 127;
  if (selected < 127)
    selected++;
  selected |= hashandles; 
 
#ifdef NEW_DRAGGING
  // KMJ : nothing to do when using opengl
  if(cadwindow != 0 && selected == 1)
      cadwindow->updateAllViews();
  /*
  if (cadwindow != 0 && selected == 1)  //  Only draw it the first time it is highlighted
    {
       cadwindow->cursoroff();
       cadwindow->drawent(DM_ERASE,NULL,this);
       cadwindow->drawent(DM_NORMAL,NULL,this);
       cadwindow->cursoron();
    }
    */
#else
  if (cadwindow != 0 && selected == 1)  //  Only draw it the first time it is highlighted
    {
       cadwindow->cursoroff();
       cadwindow->drawent(DM_ERASE,NULL,this);
       cadwindow->drawent(DM_NORMAL,NULL,this);
       cadwindow->cursoron();
    }
#endif
}

void Entity::unhighlight(void)
{int hashandles;

  hashandles = selected & 128;
  selected &= 127;
  if (selected > 0)
    selected--;
  selected |= hashandles;
#ifdef NEW_DRAGGING
  /*
  if(type() == group_entity)
  {
      //cadwindow->drawent(DM_ERASE,NULL,this);
      cadwindow->drawent(DM_NORMAL,NULL,this);
  }
  else*/
  if (cadwindow != 0)
      cadwindow->updateAllViews();
  /*
  if (cadwindow != 0)
  {  cadwindow->cursoroff();
     cadwindow->drawent(DM_ERASE,NULL,this);
     cadwindow->drawent(DM_NORMAL,NULL,this);
     cadwindow->cursoron();
  }
  */
#else
    if (cadwindow != 0)
    {  cadwindow->cursoroff();
       cadwindow->drawent(DM_ERASE,NULL,this);
       cadwindow->drawent(DM_NORMAL,NULL,this);
       cadwindow->cursoron();
    }
#endif
}

void Entity::draw(int mode,Transform *t)
{
	//if (t == nullptr) return;
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoroff();
    cadwindow->drawent(mode,t,this);
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoron();
}

void Entity::draw(int mode,Transform *t, View3dSurface *,QPainter *painter)
{
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoroff();
    cadwindow->drawent(mode,t,this,painter);
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoron();
}

void Entity::drawGL(int mode ,Transform *t)
{
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoroff();
    cadwindow->drawentGL(mode,t,this);
    if (mode != DM_INVERT && mode != DM_INVERTDASH) cadwindow->cursoron();
}


int Entity::saveunknowngeometry(CadFile *outfile)
{short t;
  t = short(type());
  if (! outfile->write(&t)) return 0;
  if (! savegeometry(outfile)) return 0;
  return 1;
}

int Entity::loadunknowngeometry(int discard,CadFile *infile,Entity **e)
{short type;
 Line line;
 Circle circle;
 Curve curve;
 EllipseE ellipse;
  infile->read(&type);
  switch (type)
    {  case 0 :
         *e = NULL;
         break;
       case line_entity :
         if (discard)
           line.loadgeometry(infile);
         else
           {  if ((*e = new Line) == NULL) return 0;
              if (! (*e)->loadgeometry(infile)) return 0;
           }
         break;
       case circle_entity :
         if (discard)
           circle.loadgeometry(infile);
         else
           {  if ((*e = new Circle) == NULL) return 0;
              if (! (*e)->loadgeometry(infile)) return 0;
           }
         break;
       case curve_entity :
         if (discard)
           curve.loadgeometry(infile);
         else
           {  if ((*e = new Curve) == NULL) return 0;
              if (! (*e)->loadgeometry(infile)) return 0;
           }
         break;
       case ellipse_entity :
         if (discard)
           ellipse.loadgeometry(infile);
         else
           {  if ((*e = new EllipseE) == NULL) return 0;
              if (! (*e)->loadgeometry(infile)) return 0;
           }
         break;
    }
  return 1;
}


Patch *UVSurfaceEntity::triangulatedclone(int showinterface)
{int nu,nv,i,j,k;
 Transform t;
 long *index,nindex,nvertices;
 Point *polygon;


    if (isa(patch_entity) && ((Patch *)this)->getoptions().test(PatchTriangulated) != 0)
    {  setcopye(0);
       return (Patch *)clone(t);
    }
    else
    {   nfacets(0.1,&nu,&nv);

        if (showinterface)
        {
            Dialog dialog("PatchTriangulate_Dialog");
            DialogControl *dc;

            dialog.add(dc = new IntegerDialogControl(100,&nu,2,1000));
            //dialog.add(new ScrollBarDialogControl(1100,dc));
            dialog.add(dc = new IntegerDialogControl(101,&nv,2,1000));
            //dialog.add(new ScrollBarDialogControl(1101,dc));

            if (dialog.process() == FALSE)
                return 0;
        }

        nvertices = (nu + 1) * (nv + 1);
        if ((polygon = new Point[nvertices]) == 0)
            return 0;

        for (i = 0 ; i <= nu ; i++)
            for (j = 0 ; j <= nv ; j++)
                polygon[i*(nv+1)+j] = position(i/double(nu),j/double(nv));

        nindex = nu * nv * 2 * 3;
        index = new long[nindex];

        k = 0;
        for (i = 0 ; i < nu ; i++)
            for (j = 0 ; j < nv ; j++)
            {   index[k++] = i*(nv + 1)+j;
                index[k++] = i*(nv + 1)+j+1;
                index[k++] = (i+1)*(nv + 1)+j+1;

                index[k++] = i*(nv + 1)+j;
                index[k++] = (i+1)*(nv + 1)+j+1;
                index[k++] = (i+1)*(nv + 1)+j;
            }

        return new Patch(*this,nvertices,polygon,nindex,index,BitMask(32));

    }




}

int InfinitePlane::hide(HideImage *)
{ return 0;
}

int InfinitePlane::stretchent(Database *,Transform &,SortedPointTable *,int)
{ return 0;
}

int InfinitePlane::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

Point InfinitePlane::nearp(Point p)
{Point tmp(0.0,0.0,0.0);
 double t,denom;
  denom = normal.dot(normal);
  if (denom > db.getptoler())
    {  t = -(normal.dot(p) + D) / denom;
       tmp = normal * t + p;
    }
  return tmp;
}

int InfinitePlane::save(CadFile *outfile)
{short type;
  type = infiniteplane_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! normal.save(outfile)) return 0;
  if (! outfile->write(&D)) return 0;
  return 1;
}

Entity *InfinitePlane::load(int,CadFile *infile)
{InfinitePlane *infiniteplane;
 EntityHeader *header;
 Point normal;
 double D;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! normal.load(infile)) return 0;
  if (! infile->read(&D)) return 0;
  infiniteplane = new InfinitePlane(*header,normal,D);
  delete header;
  return infiniteplane;
}

int InfinitePlane::save(UndoFile *outfile)
{short type;
  type = infiniteplane_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  return 1;
}

Entity *InfinitePlane::load(UndoFile *infile)
{InfinitePlane *ip;
 char bm[sizeof(BitMask)];
  if ((ip = new InfinitePlane) != NULL)
    {  memcpy(bm,&ip->visible,sizeof(bm));
       if (! infile->blockread(ip,sizeof(*ip))) return 0;
       memcpy(&ip->visible,bm,sizeof(bm));
       if (! infile->read(&ip->visible)) return 0;
    }
  return ip;
}

int InfinitePlane::saveattributes(CadFile *)
{  return 1;
}

int InfinitePlane::loadattributes(CadFile *)
{  return 1;  }

int InfinitePlane::savedxf(int,DXFOut *)
{  return 1;
}

int InfinitePlane::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}


int InfinitePlane::savegeometry(CadFile *outfile)
{ outfile->write(&D);
  normal.save(outfile);
  return 1;
}

int InfinitePlane::loadgeometry(CadFile *infile)
{ infile->read(&D);
  normal.load(infile);
  return 1;
}

void InfinitePlane::verify(void)
{ResourceString rsnc0(0);
 ResourceString rs0(NCENT);
   cadwindow->MessageBox(rs0.gets(),rsnc0.gets(),MB_ICONSTOP);
}

void InfinitePlane::extents(View3dSurface *,Point *pmin,Point *pmax)
{ pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
}

int InfinitePlane::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(infiniteplane_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int InfinitePlane::explode(void)
{ return 0;
}

void EntityList::highlight(void)
{Entity *e;
    //if(length() == 0)
   //     return;
  cadwindow->startdrawingbatch();
  for (start() ; (e = next()) != NULL ; )
    e->highlight();
  cadwindow->enddrawingbatch();
}

void EntityList::unhighlight(void)
{Entity *e;
  //  if(length() == 0)
  //      return;
  cadwindow->startdrawingbatch();
  for (start() ; (e = next()) != NULL ; )
    e->unhighlight();
  cadwindow->enddrawingbatch();
}

int EntityList::save(CadFile *outfile)
{Entity *e;
 short nvalues;
 long v;
  nvalues = short(length());
  if (! outfile->write(&nvalues)) return 0;
  for (start() ; ! atend() ; )
    {  e = next();
       v = (e == NULL ? 0L : outfile->getentityindex(e));
       if (! outfile->write(&v))
         return 0;
    }
  return 1;
}

int EntityList::load(CadFile *infile)
{short nvalues;
 long v;
 int i;
  if (! infile->read(&nvalues))
    return 0;
  for (i = 0 ; i < (unsigned short)nvalues ; i++)
    {  if (! infile->read(&v))
         return 0;
       add((Entity *)v);
    }
  reverse();
  return 1;
}

int EntityList::savegeometry(CadFile *outfile)
{Entity *e;
 short nvalues,type;
  nvalues = short(length());
  if (! outfile->write(&nvalues)) return 0;
  for (start() ; ! atend() ; )
    {  e = next();
       if (e == NULL)
         {  type = 0;  outfile->write(&type);
         }
       else
         e->saveunknowngeometry(outfile);
    }
  return 1;
}

int EntityList::loadgeometry(int discard,CadFile *infile)
{short nvalues;
 Entity *e;
 int i;
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if (! Entity::loadunknowngeometry(discard,infile,&e)) return 0;
       if (! discard) add(e);
    }
  if (! discard)
    reverse();
  return 1;
}

void EntityList::fixupreferences(Entity ** index)
{OneGeneral *og;

  for (og = list ; og != NULL ; og = og->nextg)
    if (long(og->data) != 0) og->data = index[long(og->data)];

}

int DatabaseList::add(Database *db,Entity *entity,int draw)
{OneGeneral *og;
#ifdef USING_WIDGETS
    if ((og = new OneGeneral(entity,list,NULL)) != NULL)
      {  if (list != NULL) list->lastg = og;
         list = og;
         entity->setposition(list);
         if (draw & 2)
           entity->dbinsert(db,draw);
#ifdef NEW_DRAGGING
         // ? when using opengl
         if (draw & 1)
             regenGLlists();
            //cadwindow->paintall();
#else
         if (draw & 1)
           entity->draw(DM_NORMAL);
#endif
         db->setchanged();
         return 1;
      }
    else
      return 0;
#else
  if ((og = new OneGeneral(entity,list,NULL)) != NULL)
    {  if (list != NULL) list->lastg = og;
       list = og;
       entity->setposition(list);
       if (draw & 2)
         entity->dbinsert(db,draw);
       if (draw & 1)
         entity->draw(DM_NORMAL);
       db->setchanged();
       return 1;
    }
  else
    return 0;
#endif
}

void DatabaseList::del(Entity *entity,int draw)
{ if (entity->getposition()->lastg == NULL && entity->getposition()->nextg == NULL)
    list = NULL;
  else if (entity->getposition()->lastg == NULL)
    {  list = entity->getposition()->nextg;  entity->getposition()->nextg->lastg = NULL;
    }
  else if (entity->getposition()->nextg == NULL)
    entity->getposition()->lastg->nextg = NULL;
  else
    {  entity->getposition()->lastg->nextg = entity->getposition()->nextg;
       entity->getposition()->nextg->lastg = entity->getposition()->lastg;
    }
#ifdef NEW_DRAGGING
    // KMJ : nothing to do when using openGL??
  if(cadwindow !=0)
      cadwindow->updateAllViews();
#else
  if (draw)
      entity->draw(DM_ERASE);
#endif
  if (cadwindow != 0)
    cadwindow->invalidatedisplaylist(entity);
  entity->dbdelete();
  delete entity->getposition();
  delete entity;
  db.setchanged();
}

void DatabaseList::toback(Entity *e)
{OneGeneral *og,*lastg;
  for (og = list,lastg = 0 ; og != NULL ; lastg = og,og = og->nextg);
  if ((Entity *) lastg->data != e)
    {  og = e->getposition();
       if (og == list)
         list = og->nextg;
       if (og->nextg != 0)
         og->nextg->lastg = og->lastg;
       if (og->lastg != 0)
         og->lastg->nextg = og->nextg;
       og->nextg = 0;
       og->lastg = lastg;
       lastg->nextg = og;
    }
  db.setchanged();
}

void DatabaseList::tofront(Entity *e)
{OneGeneral *og;
  og = e->getposition();
  if (og != list)
    {  if (og->nextg != 0)
         og->nextg->lastg = og->lastg;
       if (og->lastg != 0)
         og->lastg->nextg = og->nextg;
       og->lastg = 0;
       og->nextg = list;
       if (list != NULL) list->lastg = og;
       list = og;
    }
  db.setchanged();
}

void DatabaseList::backward(Entity *e)
{OneGeneral *og,*ognextg;
  og = e->getposition();
  if ((ognextg = og->nextg) != 0)
    {  if (og->lastg != 0)
         og->lastg->nextg = ognextg;
       else
         list = ognextg;
       ognextg->lastg = og->lastg;
       if (ognextg->nextg != 0)
         ognextg->nextg->lastg = og;
       og->nextg = ognextg->nextg;
       ognextg->nextg = og;
       og->lastg = ognextg;
    }
  db.setchanged();
}

void DatabaseList::forward(Entity *e)
{OneGeneral *og,*oglastg;
  og = e->getposition();
  if ((oglastg = og->lastg) != 0)
    {  if (og->nextg != 0)
         og->nextg->lastg = oglastg;
       oglastg->nextg = og->nextg;
       if (oglastg->lastg != 0)
         oglastg->lastg->nextg = og;
       else
         list = og;
       og->lastg = oglastg->lastg;
       oglastg->lastg = og;
       og->nextg = oglastg;
    }
  db.setchanged();
}

#ifdef USING_WIDGETS
void GeometryDatabase::draw(int drawmode,View3dSurface *surface, QPainter *painter)
{
    Entity *e;
    //program->setbusy(1);
    for (list.end() ; (e = list.last()) != 0 ; )
    {
        if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
            e->draw(drawmode,NULL,surface,painter);
       if (program->getinterrupt())
         break;
    }
    //program->setbusy(0);
}

void GeometryDatabase::draw(int drawmode,View3dSurface *surface, QGraphicsScene *scene)
{
    Entity *e;
    //program->setbusy(1);
    for (list.end() ; (e = list.last()) != 0 ; )
    {
        if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
#ifdef USING_WIDGETS
            //e->draw(drawmode,NULL,surface,scene);
            e->draw(drawmode,NULL,surface);
#else
            e->draw(drawmode,NULL,surface);
#endif
       if (program->getinterrupt())
         break;
    }
    //program->setbusy(0);
}

#ifdef USING_WIDGETS

void debugGraphicsItemChildren(QGraphicsItem *gitem)
{
    QList<QGraphicsItem *> children = gitem->childItems();
    for(int icqg=0; icqg < children.size(); icqg++)
    {
        if(qgraphicsitem_cast<QGraphicsItemGroup*>(children.at(icqg)) != 0)
        {
            debugGraphicsItemChildren(children.at(icqg));
        }
        else
        {
            qDebug() << "child item [" << icqg << "]";
            qDebug() << "zValue : " << children.at(icqg)->zValue();
            qDebug() << "g type : " << children.at(icqg)->type();
            qDebug() << "e type : " << children.at(icqg)->data(0).toInt();
            qDebug() << "------------";
        }
    }
}

void GeometryDatabase::drawforQtPrint(int drawmode,View3dSurface *surface, QGraphicsScene *scene)
{
    Entity *e;
    Point p1,p2,p3,p4,o,xaxis,yaxis;

    //program->setbusy(1);
    if(surface->getsurfacetype() == View3dSurf)
    {
        QGMAXZVALUE=0;
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
           if (program->getinterrupt())
             break;
        }
    }
    else if(surface->getsurfacetype() == View2dSurf)
    {
        //qDebug() << "surface width : " << surface->getwidth();
        //qDebug() << "surface height : " << surface->getheight();
        //qDebug() << "surface uvwidth : " << surface->getumax() - surface->getumin();
        //qDebug() << "surface uvheight : " << surface->getvmax() - surface->getvmin();

        //qDebug() << "viewport : " << ((RCView*)surface->gethwnd())->view();
        //qDebug() << "viewport rect : " << ((RCView*)surface->gethwnd())->view()->rect();
        //qDebug() << "viewport scene rect : " << ((RCView*)surface->gethwnd())->view()->sceneRect();

        //qDebug() << "surface scene : " << surface->getScene();
        //qDebug() << "surface scene rect : " << surface->getScene()->sceneRect();

        //surface->setclip(0.0,0.0,surface->getwidth(),surface->getheight());
        //qDebug() << "View2dSurf";

        OneView *ov;
        View3dSubSurface *ss;
        for(((View2dWindow*)surface)->subsurfaces.start();(ss = (View3dSubSurface *) ((View2dWindow*)surface)->subsurfaces.next()) != NULL ; )
        {
            ss->setblackandwhite(surface->getblackandwhite());
            ss->setdisplaylayertable(ss->getoneview()->getlayertable());
            ss->setscale((ss->getumax() - ss->getumin()) / ss->getwidthmm());
            ss->setprintxdotspermm(surface->getprintxdotspermm());
            ss->setprintydotspermm(surface->getprintydotspermm());

            //qDebug() << "ss width : " << ss->getwidth();
            //qDebug() << "ss height : " << ss->getheight();
            //qDebug() << "ss uvwidth : " << ss->getumax() - ss->getumin();
            //qDebug() << "ss uvheight : " << ss->getvmax() - ss->getvmin();

            //qDebug() << "ss scene : " << ss->getScene();
            //qDebug() << "ss scene rect : " << ss->getScene()->sceneRect();

            // set clip path
            QPainterPath clipPath;
            clipPath.setFillRule(Qt::WindingFill);
            //clipPath.setFillRule(Qt::OddEvenFill);

            o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
            xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
            yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
            p1 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
            p2 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
            p3 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
            p4 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

            //qDebug() << "p1.x : " << p1.x;
            //qDebug() << "p1.y : " << p1.y;

            // the region is used for clipping
            //QRegion clipRegion(p1.x,surface->getheight()-p4.y,ss->getwidth(),ss->getheight());
            //clipPath.addRegion(clipRegion);
            // these are for debugging the frame position
            clipPath.addRect(p1.x,surface->getheight()-p4.y,ss->getwidth(),ss->getheight());
            //clipPath.moveTo(p1.x,surface->getheight()-p1.y);
            //clipPath.lineTo(p2.x,surface->getheight()-p2.y);
            //clipPath.lineTo(p3.x,surface->getheight()-p3.y);
            //clipPath.lineTo(p4.x,surface->getheight()-p4.y);
            //clipPath.lineTo(p1.x,surface->getheight()-p1.y);
            //clipPath.closeSubpath();

            QGraphicsPathItem *graphClipPath = new QGraphicsPathItem(clipPath);
            graphClipPath->setFlag(QGraphicsItem::ItemClipsChildrenToShape,true);
            graphClipPath->setFlag(QGraphicsItem::ItemClipsToShape,true);
            graphClipPath->setPen(Qt::NoPen);
            graphClipPath->setBrush(Qt::NoBrush);
            ss->qgicp = graphClipPath;
            ss->getScene()->addItem(ss->qgicp);

            //ss->cleartransform();
            ss->updatematrix();
            if(ss->getoneview()->gethiddenlines() || ss->getoneview()->getshaded())
            {

            }
            else
            {
                // forcing the graphic item zorder
                // for printing
                double zorder = 0.0;
                for (list.end(); (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                    {
                        //qDebug() << "entity type : " << e->type();
                        // setting the entity zorder
                        e->zValue = zorder;
                        e->draw(drawmode,NULL,ss);
                        if(e->qgi)
                        {
                            // for debugging the entity type being drawn
                            e->qgi->setData(0,(int)e->type());
                            // set the qgi zvalue ti force the draw order
                            e->qgi->setZValue(zorder);
                        }
                        e->zValue = 0.0;
                    }
                    if (program->getinterrupt())
                        break;
                    zorder += 1.0;
                }
            }
        }
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
           if (program->getinterrupt())
             break;
        }
    }
    //program->setbusy(0);
}

void GeometryDatabase::drawforQtPrint(int drawmode,RCView3dPrinter *surface, QGraphicsScene *scene)
{
    Entity *e;
    Point p1,p2,p3,p4,o,xaxis,yaxis;
    BitMask visible(32);

    //program->setbusy(1);
    if(surface->getsurfacetype() == View3dSurf)
     {
        //QGMAXZVALUE=0;
        int renderstyle = surface->getrepaintstyle();
        if(renderstyle == Hidden || renderstyle == ShadedHidden || renderstyle == ShadedHiddenTexture)
        {
            // hidden = 0; hiddendashed = 2
            HideImage image(surface, 0);
            image.drawmode = 1;
            image.hide();
        }
        else if(renderstyle == HiddenDashed || renderstyle == ShadedHiddenDashed )
        {
            // hidden = 0; hiddendashed = 2
            HideImage image(surface, 2);
            image.drawmode = 1;
            image.hide();
        }
        else if(renderstyle == Wireframe )
        {
            double zorder = 0.0;
            for (list.end() ; (e = list.last()) != 0 ; )
            {
                if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                {
                     e->zValue = zorder;
                     e->draw(drawmode,NULL,surface);
                }
                e->zValue = 0.0;
                //zorder += 1.0;
                if (program->getinterrupt())
                   break;
            }
        }
    }
    else if(surface->getsurfacetype() == View2dSurf)
    {
        //qDebug() << "surface width : " << surface->getwidth();
        //qDebug() << "surface height : " << surface->getheight();
        //qDebug() << "surface uvwidth : " << surface->getumax() - surface->getumin();
        //qDebug() << "surface uvheight : " << surface->getvmax() - surface->getvmin();

        //qDebug() << "viewport : " << ((RCView*)surface->gethwnd())->view();
        //qDebug() << "viewport rect : " << ((RCView*)surface->gethwnd())->view()->rect();
        //qDebug() << "viewport scene rect : " << ((RCView*)surface->gethwnd())->view()->sceneRect();

        //qDebug() << "surface scene : " << surface->getScene();
        //qDebug() << "surface scene rect : " << surface->getScene()->sceneRect();

        //surface->setclip(0.0,0.0,surface->getwidth(),surface->getheight());
        //qDebug() << "View2dSurf";
        OneView *ov;
        View3dSubSurface *ss;
        View2d *view2d =  ((RCView2dPrinter*)surface)->getview2dwindow()->getview2d();
        for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
        {
            //if(ov->getshaded() && !ov->gethiddenlines() && !ov->getdashhidden())
            //    continue;

            //if( ov->gethiddenlines() || ov->getdashhidden() )
            {
                if((ss = new View3dSubSurface(surface,ov)) != NULL)
                {
                    ss->setblackandwhite(surface->getblackandwhite());
                    ss->setdisplaylayertable(ss->getoneview()->getlayertable());
                    ss->setscale((ss->getumax() - ss->getumin()) / ss->getwidthmm());
                    ss->setprintxdotspermm(surface->getprintxdotspermm());
                    ss->setprintydotspermm(surface->getprintydotspermm());
                    ss->setScene(surface->getScene());

                    //qDebug() << "ss width : " << ss->getwidth();
                    //qDebug() << "ss height : " << ss->getheight();
                    //qDebug() << "ss uvwidth : " << ss->getumax() - ss->getumin();
                    //qDebug() << "ss uvheight : " << ss->getvmax() - ss->getvmin();

                    //qDebug() << "ss scene : " << ss->getScene();
                    //qDebug() << "ss scene rect : " << ss->getScene()->sceneRect();

                    // set clip path
                    QPainterPath clipPath;
                    clipPath.setFillRule(Qt::WindingFill);
                    //clipPath.setFillRule(Qt::OddEvenFill);

                    o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                    xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                    yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                    p1 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                    p2 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                    p3 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
                    p4 = surface->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

                    qDebug() << "fboWidth : " << ((RCView2dPrinter*)surface)->fboWidth;
                    qDebug() << "fboHeight : " << ((RCView2dPrinter*)surface)->fboHeight;
                    qDebug() << "p1.x :" << p1.x;
                    qDebug() << "p1.y :" << p1.y;
                    qDebug() << "surface->getheight()-p4.y : " << surface->getheight()-p4.y;
                    qDebug() << "ss->getwidth() : " << ss->getwidth();
                    qDebug() << "ss->getheight() : " << ss->getheight();

                    // the region is used for clipping
                    //QRegion clipRegion(p1.x,surface->getheight()-p4.y,ss->getwidth(),ss->getheight());
                    //clipPath.addRegion(clipRegion);
                    // these are for debugging the frame position
                    clipPath.addRect(p1.x,surface->getheight()-p4.y,ss->getwidth(),ss->getheight());
                    //clipPath.moveTo(p1.x,surface->getheight()-p1.y);
                    //clipPath.lineTo(p2.x,surface->getheight()-p2.y);
                    //clipPath.lineTo(p3.x,surface->getheight()-p3.y);
                    //clipPath.lineTo(p4.x,surface->getheight()-p4.y);
                    //clipPath.lineTo(p1.x,surface->getheight()-p1.y);
                    //clipPath.closeSubpath();

                    QGraphicsPathItem *graphClipPath = new QGraphicsPathItem(clipPath);
                    //graphClipPath->setFlag(QGraphicsItem::ItemClipsChildrenToShape,true);
                    //graphClipPath->setFlag(QGraphicsItem::ItemClipsToShape,true);
                    graphClipPath->setPen(Qt::NoPen);
                    graphClipPath->setBrush(Qt::NoBrush);
                    QGraphicsItemGroup *clipgroup = new QGraphicsItemGroup();
                    clipgroup->setFlag(QGraphicsItem::ItemClipsChildrenToShape,true);
                    clipgroup->setFlag(QGraphicsItem::ItemClipsToShape,true);
                    ss->qgicp = clipgroup;
                    clipgroup->addToGroup(graphClipPath);
                    //ss->qgicp = graphClipPath;
                    ss->getScene()->addItem(ss->qgicp);

                    //ss->cleartransform();
                    ss->updatematrix();
                    qDebug() << "hidden : " <<  ss->getoneview()->gethiddenlines();
                    qDebug() << "dashed : " <<  ss->getoneview()->getdashhidden();
                    qDebug() << "shaded : " <<  ss->getoneview()->getshaded();
                    if(ss->getoneview()->gethiddenlines() || ss->getoneview()->getdashhidden())
                    {
                        HideImage image(ss, ss->getoneview()->getdashhidden() * 2);// 1 = hidden; 2 = hidden dashed
                        image.drawmode = 1; // 1 = printing
                        image.hide();
                    }
                    else if( !ss->getoneview()->getshaded())
                    {
                        // forcing the graphic item zorder
                        // for printing
                        double zorder = 0.0;
                        int enlen = list.length();
                        qDebug() << "number of entities : " << enlen;
                        for (list.end(); (e = list.last()) != 0 ; )
                        {
                            //qDebug() << "entity type : " << e->type();
                            // debug
                            //visible.setall();
                            //e->setvisible(visible);
                            if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                            {
                                //qDebug() << "entity type : " << e->type();
                                // setting the entity zorder
                                e->zValue = zorder;
                                e->draw(drawmode,NULL,ss);
                                if(e->qgi)
                                {
                                    // for debugging the entity type being drawn
                                    e->qgi->setData(0,(int)e->type());
                                    // set the qgi zvalue ti force the draw order
                                    e->qgi->setZValue(zorder);
                                    e->qgi=0;
                                }
                                e->zValue = 0.0;
                            }
                            //if (program->getinterrupt())
                            //    break;
                            zorder += 1.0;
                        }
                    }
                }
                delete ss;
                ss = 0;
            }
        }
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
           //if (program->getinterrupt())
           //  break;
        }
    }
    //program->setbusy(0);
}

void GeometryDatabase::drawforQtPrintGL(int drawmode,View3dSurface *surface, QGraphicsScene *scene)
{
    Entity *e;
    //program->setbusy(1);
    if(surface->getsurfacetype() == View3dSurf)
    {
        if( surface->getrepaintstyle() == Shaded              ||
            surface->getrepaintstyle() == ShadedTexture       ||
            surface->getrepaintstyle() == ShadedHidden        ||
            surface->getrepaintstyle() == ShadedHiddenTexture ||
            surface->getrepaintstyle() == ShadedHiddenDashed  )
        {
            for (list.end() ; (e = list.last()) != 0 ; )
            {
                // KMJ: currently only rendering surface types with GL
#if 1
                if (e->issurface() && e->isvisible(surface) )
                {
                    //qDebug() << "gl shading 3d window with no list";
                    if( e->type() == plane_entity        ||
                        e->type() == ruledsurface_entity ||
                        e->type() == surfaceofrev_entity ||
                        e->type() == patch_entity  ||
                        //e->type() == figure_entity || // figures don't render
                        e->type() == group_entity ) // groups ??
                    {
                        e->drawGL(drawmode,NULL,surface);
                    }
                }
#else
                if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                    e->drawGL(drawmode,NULL,surface);
#endif
                //if (program->getinterrupt())
                //    break;
            }
        }
    }
    else if(surface->getsurfacetype() == View2dSurf)
    {
        qDebug() << "View2dSurf";
        // draw the stuff in the sub windows
        OneView *ov;
        View3dSubSurface *ss = nullptr;
        View2d *view2d =  ((RCView2dPrinter*)surface)->getview2dwindow()->getview2d();
        double fbowidth = ((RCView2dPrinter*)surface)->fboWidth;
        double fboheight = ((RCView2dPrinter*)surface)->fboHeight;

        for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
        {
            Point p1,p2,p3,p4,o,xaxis,yaxis;
            // check for view shaded setting
            if(ov->getshaded())
            {
                ss = nullptr;
                if((ss = new View3dSubSurface(surface,ov)) != NULL)
                {
                    // always use shaded texture for rendering
                    ss->setrepaintstyle(ShadedTexture);
                    ss->setblackandwhite(surface->getblackandwhite());
                    ss->setdisplaylayertable(ss->getoneview()->getlayertable());
                    //ss->setscale((ss->getumax() - ss->getumin()) / ss->getwidthmm());
                    //ss->setprintxdotspermm(surface->getprintxdotspermm());
                    //ss->setprintydotspermm(surface->getprintydotspermm());

                    o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                    xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                    yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);

                    p1 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                    p2 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                    p3 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
                    p4 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

                    // scale the view port to match the frame buffer size
                    double vpscale = fbowidth / surface->getwidth();
                    p1.x *= vpscale;
                    p1.y *= vpscale;
                    p3.x *= vpscale;
                    p3.y *= vpscale;

                    int transflag = ss->gettransform2d();
                    setGLpixelMM(ss->getpixelmodelmm());
                    ss->cleartransform();
                    ss->updatematrix();

                    View *view = ss->getview();

                    Point eye,pmin,pmax;
                    db.geometry.extents(ss,&pmin,&pmax);
                    if (view->getperspective())
                      eye = view->geteye();
                    else
                      eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

                    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                    SetViewport(ss->getumin(),ss->getvmin(),ss->getumax(),ss->getvmax());

                    qDebug() << "fboWidth : " << ((RCView2dPrinter*)surface)->fboWidth;
                    qDebug() << "fboHeight : " << ((RCView2dPrinter*)surface)->fboHeight;
                    qDebug() << "p1.x :" << p1.x;
                    qDebug() << "p1.y : " << p1.y;
                    qDebug() << "p3.x :" << p3.x;
                    qDebug() << "p3.y : " << p3.y;
                    qDebug() << "ss->getwidth() : " << ss->getwidth();
                    qDebug() << "ss->getheight() : " << ss->getheight();

                    glViewport(p1.x,p1.y,p3.x-p1.x,p3.y-p1.y);

                    SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

                    BeginScene(BS_NOCLEAR);//  Allow the clear for now | BS_NOCLEAR);
#if 1
                    // KMJ individual GL set up for each view port
                    glEnable(GL_LIGHTING);
                    glEnable(GL_DEPTH_TEST);
                    glClear( GL_DEPTH_BUFFER_BIT );

                    //dblist = *db.geometry.getlist();
                    int nlights=0;
                    Entity *e=0;
                    for (list.end() ; (e = list.last()) != NULL ; )
                    {
                      if (e->isa(light_entity) && e->isvisible(ss))
                      {
                           double red,green,blue;
                           if (((Light *)e)->getoptions().test(1))
                             {  red = green = blue = 1.0;
                             }
                           else
                             {  red = db.colourtable[e->getcolour()].red/255.0;
                                green = db.colourtable[e->getcolour()].green/255.0;
                                blue = db.colourtable[e->getcolour()].blue/255.0;
                             }

                           SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                           nlights++;
                      }
                    }

                    if (nlights == 0)
                        SetLight(eye.x,eye.y,eye.z);

#else
                    //if (nlights == 0)
                        SetLight(view->geteye().x,view->geteye().y,view->geteye().z);
                    //SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
#endif
                    for (list.end() ; (e = list.last()) != 0 ; )
                    {
                        // KMJ: currently only rendering surface types with GL
#if 1
                        if (e->issurface() && e->isvisible(ss) )
                        {
                            //qDebug() << "gl shading 3d window with no list";
                            if( e->type() == plane_entity        ||
                                e->type() == ruledsurface_entity ||
                                e->type() == surfaceofrev_entity ||
                                e->type() == patch_entity  ||
                                //e->type() == figure_entity || // figures don't render
                                e->type() == group_entity ) // groups ??
                            {
                                e->drawGL(drawmode,NULL,ss);
                            }
                        }
#else
                        if (e->isvisible(ss) && e->isa(group_entity) == 0)
                            e->drawGL(drawmode,NULL,ss);
#endif
                        /*
                        if (e->isvisible(ss) && e->isa(group_entity) == 0)
                        {
                            if(e != app->mFlashEntity)
                                e->drawGL(DM_NORMAL,NULL,ss);
                        }
                        */
                    }

                    EndScene();
                    //ss->settransform2d(transflag);
                    //ss->updatematrix();
                }
                delete ss;
            }
        }
        // draw the stuff in the 2d window
        if( surface->getrepaintstyle() == Shaded              ||
            surface->getrepaintstyle() == ShadedTexture       ||
            surface->getrepaintstyle() == ShadedHidden        ||
            surface->getrepaintstyle() == ShadedHiddenTexture ||
            surface->getrepaintstyle() == ShadedHiddenDashed  )
        {
            for (list.end() ; (e = list.last()) != 0 ; )
            {
#if 1
                        if (e->issurface() && e->isvisible(surface) )
                        {
                            //qDebug() << "gl shading 3d window with no list";
                            if( e->type() == plane_entity        ||
                                e->type() == ruledsurface_entity ||
                                e->type() == surfaceofrev_entity ||
                                e->type() == patch_entity  ||
                                //e->type() == figure_entity || // figures don't render
                                e->type() == group_entity ) // groups ??
                            {
                                e->drawGL(drawmode,NULL,surface);
                            }
                        }
#else
                        if (e->isvisible(surface) && e->isa(group_entity) == 0)
                            e->drawGL(drawmode,NULL,sssurface
#endif
                //if (e->isvisible(surface) && e->isa(group_entity))
                //    e->drawGL(drawmode,NULL,surface);
               //if (program->getinterrupt())
               //  break;
            }
        }
    }
    //program->setbusy(0);
}
#endif
#if 1 // new for clearing qgi stuff of ssubwindows similar to print for Qt
void GeometryDatabase::clearScene(int drawmode,View3dSurface *surface)
{
    Entity *e;
    Point p1,p2,p3,p4,o,xaxis,yaxis;

    //program->setbusy(1);
    if(surface->getsurfacetype() == View3dSurf)
    {
        QGMAXZVALUE=0;
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
        }
    }
    else if(surface->getsurfacetype() == View2dSurf)
    {
        View3dSubSurface *ss;
        for(((View2dWindow*)surface)->subsurfaces.start();(ss = (View3dSubSurface *) ((View2dWindow*)surface)->subsurfaces.next()) != NULL ; )
        {
            for (list.end(); (e = list.last()) != 0 ; )
            {
                if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                {
                    e->draw(drawmode,NULL,ss);
                    e->zValue = 0.0;
                }
            }
            // we don't clear each scene here
            // the sub surfaces all have the same which gets cleared at the end of this function
            if(ss->getScene())
            {
                // each ss has its own clipping rectangke graphics item
                // delete each local qgicp object
                delete ss->qgicp;
                ss->qgicp = 0;
            }
        }
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
        }
    }
    // now clear the scene
    if(surface->getScene())
        surface->getScene()->clear();
    //program->setbusy(0);
}

void GeometryDatabase::clearScene(int drawmode,RCView3dPrinter *surface)
{
    Entity *e;
    Point p1,p2,p3,p4,o,xaxis,yaxis;

    //program->setbusy(1);
    if(surface->getsurfacetype() == View3dSurf)
    {
        QGMAXZVALUE=0;
        // clear everthing except figures and groups
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
        }
    }
    else if(surface->getsurfacetype() == View2dSurf)
    {
        OneView *ov;
        View3dSubSurface *ss;
        View2d *view2d =  ((RCView2dPrinter*)surface)->getview2dwindow()->getview2d();
        //for(((View2dWindow*)surface)->subsurfaces.start();(ss = (View3dSubSurface *) ((View2dWindow*)surface)->subsurfaces.next()) != NULL ; )
        //for(((RCView2dPrinter*)surface)->getview2dwindow()->subsurfaces.start();(ss = (View3dSubSurface *) ((RCView2dPrinter*)surface)->getview2dwindow()->subsurfaces.next()) != NULL ; )
        //for(view2d->start();(ss = (View3dSubSurface *) view2d->next()) != NULL ; )
        for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
        {
            if((ss = new View3dSubSurface(surface,ov)) != NULL)
            {
                for (list.end(); (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                    {
                        e->draw(drawmode,NULL,ss);
                        e->zValue = 0.0;
                    }
                }
                // we don't clear each scene here
                // the sub surfaces all have the same which gets cleared at the end of this function
                if(ss->getScene())
                {
                    // each ss has its own clipping rectangke graphics item
                    // delete each local qgicp object
                    delete ss->qgicp;
                    ss->qgicp = 0;
                }
            }
        }
        for (list.end() ; (e = list.last()) != 0 ; )
        {
            if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
                e->draw(drawmode,NULL,surface);
        }
    }
    // now clear the scene
    if(surface->getScene())
        surface->getScene()->clear();
    //program->setbusy(0);
}
#else
void GeometryDatabase::clearScene(int drawmode,View3dSurface *surface)
{
  Entity *e;
  //program->setbusy(1);
  for (list.end() ; (e = list.last()) != 0 ; )
  {
         e->draw(drawmode,NULL,surface);
  }
  if(surface->getScene())
      surface->getScene()->clear();
  //program->setbusy(0);
}
#endif
#endif

QTimer *drawertimer=0;
int drawermode;
View3dSurface *drawersurface=0;
void entityDrawer()
{
    static int index=0;
    static EntityList *mlist=0;
    Entity *e=0;
    int i=0;
    if(mlist == 0)
        mlist = db.geometry.getlist();
    for (mlist->end(),i=0 ; (e = mlist->last()) != 0 ; i++ )
    {
        if(i == index)
        {
            if (e->isvisible(drawersurface) && (e->isa(group_entity) == 0 || e->getselected()))
            {
                e->draw(drawermode,NULL,drawersurface);
                index++;
                return;
            }
        }
    }
    drawertimer->stop();
    drawertimer->deleteLater();
}

void GeometryDatabase::draw(int drawmode,View3dSurface *surface)
{
#if 0
    drawertimer = new QTimer();
    QObject::connect(drawertimer, &QTimer::timeout, entityDrawer);
    drawertimer->start();
#else
#if 0
    // make the graphics view update
    if(surface->gethwnd())
        ((RCView*)surface->gethwnd())->view()->update();
#else
    Entity *e;
    //int n1 = GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
    QGMAXZVALUE=0;
    //program->setbusy(1);
    for (list.end() ; (e = list.last()) != 0 ; )
    {
        if (e->isvisible(surface) && (e->isa(group_entity) == 0 || e->getselected()))
        {
            //e->zValue = QGMAXZVALUE += QGZVALINC;
            e->draw(drawmode,NULL,surface);
        }
        //if (program->getinterrupt())
        //     break;
    }
     //program->setbusy(0);
     //int n2 = GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);

    if(surface->gethwnd())
        ((RCView*)surface->gethwnd())->view()->invalidateScene();
#endif
#endif
}

#ifdef USING_WIDGETS
void GeometryDatabase::hideGL(HideImage *image)
{Entity *e;
  //program->setbusy(1);
  list.start();
  while ((e = list.next()) != NULL)
  {
      if (e->isvisible(image->getsurface()))
      {
          if (! e->hideGL(image)) break;
      }
      //if (program->getinterrupt()) break;
  }
  //program->setbusy(0);
}
#endif

void GeometryDatabase::hide(HideImage *image)
{Entity *e;
  //program->setbusy(1);
  list.start();
  while ((e = list.next()) != NULL)
    {  if (e->isvisible(image->getsurface()))
         {  if (! e->hide(image)) break;
         }
       if (program->getinterrupt()) break;
    }
  ///program->setbusy(0);
}

void GeometryDatabase::extents(View3dSurface *s,Point *pmin,Point *pmax)
{Entity *e;
/*
 Point pmin1(RCREAL_MAX,RCREAL_MAX,RCREAL_MAX),pmax1(-RCREAL_MAX,-RCREAL_MAX,-RCREAL_MAX);
 long c;
  program->setbusy(1);
  program->resetinterrupt();
  *pmin = Point(RCREAL_MAX,RCREAL_MAX,RCREAL_MAX); *pmax = Point(-RCREAL_MAX,-RCREAL_MAX,-RCREAL_MAX);
  for (list.start(),c = 0 ; (e = list.next()) != NULL ; )
    {  if (e->isvisible(s))
         {  if (c == 0)
              {  e->extents(s,pmin,pmax);
                 //if (pmin->x < -0.99E38)
                 if (pmin->x < -RCREAL_MAX)
*/
 Point MAXPOINT(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX),MINPOINT(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX);
 Point pmin1(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax1(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
 long c;
  program->setbusy(1);
  program->resetinterrupt();
  *pmin = pmin1; *pmax = pmax1;

  qDebug() << "*** Getting DB Extents ***";
  qDebug() << "# Ents " << list.length();

  for (list.start(),c = 0 ; (e = list.next()) != NULL ; )
    {  if (e->isvisible(s))
         {  if (c == 0)
              {
                 pmin1 = MINPOINT; pmax1 = MAXPOINT;
                 e->extents(s,&pmin1,&pmax1);
                 //if (pmin->x < -0.99E38)
                 if (pmin1.x <= -RCDBL_MAX+0.9e38 || pmax1.x >= RCDBL_MAX-0.9e38 ||
                     pmin1.y <= -RCDBL_MAX+0.9e38 || pmax1.y >= RCDBL_MAX-0.9e38 ||
                     pmin1.z <= -RCDBL_MAX+0.9e38 || pmax1.z >= RCDBL_MAX-0.9e38)
                   c--;
                 else
                 {
                     *pmin = pmin1;
                     *pmax = pmax1;
                 }
              }
            else
              {
/*
              e->extents(s,&pmin1,&pmax1);
                 //if (pmin1.x > -1.0E38 && pmax1.x > -1.0E38)
                 if (pmin1.x > -RCREAL_MAX && pmax1.y > -RCREAL_MAX &&
                     pmin1.x <  RCREAL_MAX && pmax1.y <  RCREAL_MAX )
                 //if (pmax1.x - pmin1.x < 20000000 &&
                 //    pmax1.y - pmin1.y < 20000000 )
*/
                 pmin1 = MINPOINT; pmax1 = MAXPOINT;
                 e->extents(s,&pmin1,&pmax1);
                 //if (pmin1.x > -1.0E38 && pmax1.x > -1.0E38)
                   if (pmin1.x > -RCDBL_MAX+0.9e38 && pmax1.x < RCDBL_MAX-0.9e38 &&
                       pmin1.y > -RCDBL_MAX+0.9e38 && pmax1.y < RCDBL_MAX-0.9e38 &&
                       pmin1.z > -RCDBL_MAX+0.9e38 && pmax1.z < RCDBL_MAX-0.9e38 )
                   {
                      if (pmin1.x < pmin->x) pmin->x = pmin1.x;
                      if (pmin1.y < pmin->y) pmin->y = pmin1.y;
                      if (pmin1.z < pmin->z) pmin->z = pmin1.z;
                      if (pmax1.x > pmax->x) pmax->x = pmax1.x;
                      if (pmax1.y > pmax->y) pmax->y = pmax1.y;
                      if (pmax1.z > pmax->z) pmax->z = pmax1.z;
                   }
                   else
                       qDebug() << "oops";

                   /*
                   if(pmax1.y > 800.0)
                   {
                       qDebug() << "oops";
                       pmin1 = MINPOINT; pmax1 = MAXPOINT;
                       e->extents(s,&pmin1,&pmax1);
                   }
                   */
              }

              qDebug() << "*** Entity bounding box ****";
              qDebug() << "Ent # "   << c;
              qDebug() << "EntType " << e->type();
              qDebug() << "pmin1.x " << pmin->x;
              qDebug() << "pmin1.y " << pmin->y;
              qDebug() << "pmin1.z " << pmin->z;
              qDebug() << "pmax1.x " << pmax->x;
              qDebug() << "pmax1.y " << pmax->y;
              qDebug() << "pmax1.z " << pmax->z;
              qDebug() << "****************************";

              c++;
            // debug
         }
       if (program->getinterrupt()) break;
    }
  qDebug() << "**************************";
  program->setbusy(0);
}

void GeometryDatabase::clearstatus(void)
{Entity *e;
 EntityList l;
  l = list;  //  Make a copy of the list pointer, so that we don't interrupt a traverse enclosing this function
  for (l.start() ; (e = l.next()) != NULL ; )
    {  e->setcopye(NULL);
       e->setstatus(0);
    }
}

Entity *GeometryDatabase::pick(View3dSurface *surface,int interrupt,int snap,int x,int y,Entity **secondentity,Point *position)
{double dmin1,dmin2,d;
 Entity *e,*ge,*epicked;
 Point *p,p1;
 BitMask entity1(32),entity2(32);
 int i,pickfirst;
 POINT cp1,cp2;


//DWORD ticks = GetTickCount();


  GetCursorPos(&cp1);

  if (position != 0)
    state.setpickedentity(0);

  pickfirst = GetPrivateProfileInt("Configuration","PickFirst",0,home.getinifilename());

  if (secondentity != 0)
    *secondentity = 0;

  if (snap)
    {  entity1 = state.selmask.entity;
       entity2 = state.tempentitymask;
       state.selmask.entity.set(linear_mask);
       state.tempentitymask.set(linear_mask);
       state.selmask.entity.set(point_entity);
       state.tempentitymask.set(point_entity);
       state.selmask.entity.set(wall_entity);
       state.tempentitymask.set(wall_entity);
       state.selmask.entity.set(beam_entity);
       state.tempentitymask.set(beam_entity);
    }
  y = int(surface->getheight()) - 1 - y;
  //dmin1 = 4.0;  dmin2 = 4.0;  epicked = NULL;
  dmin1 = db.getstoler();  dmin2 = db.getstoler();  epicked = NULL;
  if (position == NULL) p = NULL; else p = & p1;
  if (state.selmask.entity.test(group_entity))
    for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
      {  if (e->isa(group_entity) &&
             e->isselectable(surface) &&
             (d = e->pick(surface,x,y,p)) >= 0.0
             && (d < dmin1 || d < dmin2) &&
             ! state.excludelist.inlist(e))
           {EntityList alist;
            int parent;
              alist = e->getalist();
              parent = 0;
              for (alist.start() ; (ge = alist.next()) != NULL ; )
                if (ge->isa(group_entity) && ge->isselectable(surface)  && ! state.excludelist.inlist(ge) && ((Group *)ge)->getlist().inlist(e))
                  parent = 1;
              if (! parent)
                {  if (d < dmin1)
                     {  //  Closest entity becomes next closest entity
                        dmin2 = dmin1;
                        if (secondentity != 0)
                          *secondentity = epicked;
                        dmin1 = d;
                        if (pickfirst)
                          dmin1 = 0.0;
                        if (position != NULL)
                          *position = *p;
                        epicked = e;
                     }
                   else if (d < dmin2)
                     {  dmin2 = d;
                        if (secondentity != 0)
                          *secondentity = e;
                     }

                }
           }

         if (interrupt && (i % 100) == 0)
           {  GetCursorPos(&cp2);
              if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
                {  if (snap)
                     {  state.selmask.entity = entity1;
                        state.tempentitymask = entity2;
                     }
                   return epicked;
                }
           }

      }
  if (state.selmask.entity.test(xyz_mask) || state.selmask.entity.test(point_entity))
    for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
      {  if (e->isa(point_entity) &&
             e->isselectable(surface) &&
             (d = e->pick(surface,x,y,p)) >= 0.0 &&
             (d < dmin1 || d < dmin2)  && !
             state.excludelist.inlist(e))
           {  if (d < dmin1)
                {  //  Closest entity becomes next closest entity
                   dmin2 = dmin1;
                   if (secondentity != 0)
                     *secondentity = epicked;
                   dmin1 = d;
                   if (pickfirst)
                     dmin1 = 0.0;
                   if (position != NULL)
                     *position = *p;
                   epicked = e;
                }
              else if (d < dmin2)
                {  dmin2 = d;
                   if (secondentity != 0)
                     *secondentity = e;
                }

           }

         if (interrupt && (i % 100) == 0)
           {  GetCursorPos(&cp2);
              if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
                {  if (snap)
                     {  state.selmask.entity = entity1;
                        state.tempentitymask = entity2;
                     }
                   return epicked;
                }
           }

      }
  if (epicked != NULL)
    {  if (snap)
         {  state.selmask.entity = entity1;
            state.tempentitymask = entity2;
         }
       return epicked;
    }
  for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
    {  if (! e->isa(group_entity) &&
           e->isselectable(surface) &&
           (d = e->pick(surface,x,y,p)) >= 0.0 &&
           (d < dmin1 || d < dmin2) && !
           state.excludelist.inlist(e))
         {  if (d < dmin1)
              {  //  Closest entity becomes next closest entity
                 dmin2 = dmin1;
                 if (secondentity != 0)
                   *secondentity = epicked;
                 dmin1 = d;
                 if (pickfirst)
                   dmin1 = 0.0;
                 if (position != NULL)
                   *position = *p;
                 epicked = e;
              }
            else if (d < dmin2)
              {  dmin2 = d;
                 if (secondentity != 0)
                   *secondentity = e;
              }

         }
       if (interrupt && (i % 100) == 0)
         {  GetCursorPos(&cp2);
            if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
              {  if (snap)
                   {  state.selmask.entity = entity1;
                      state.tempentitymask = entity2;
                   }
                 return epicked;
              }
         }
    }

  if (snap)
    {  state.selmask.entity = entity1;
       state.tempentitymask = entity2;
    }

//if (! interrupt)
//{
//  DWORD tticks = GetTickCount() - ticks;
//  FILE *dfile = fopen("c:\\debug.log","a");
//  fprintf(dfile,"Pick Time : %d\n",tticks);
//  fclose(dfile);
//}

  return epicked;
}

Entity *GeometryDatabase::pickNext(View3dWindow *surface,int interrupt,int snap,int x,int y,Entity **secondentity,Point *position)
{double dmin1,dmin2,d;
 Entity *e,*ge,*epicked;
 Point *p,p1;
 BitMask entity1(32),entity2(32);
 int i,pickfirst;
 POINT cp1,cp2;
 bool skipRemaining = false;

  GetCursorPos(&cp1);

  if (position != 0)
    state.setpickedentity(0);

  pickfirst = GetPrivateProfileInt("Configuration","PickFirst",0,home.getinifilename());

  if (secondentity != 0)
    *secondentity = 0;

  if (snap)
    {  entity1 = state.selmask.entity;
       entity2 = state.tempentitymask;
       state.selmask.entity.set(linear_mask);
       state.tempentitymask.set(linear_mask);
       state.selmask.entity.set(point_entity);
       state.tempentitymask.set(point_entity);
       state.selmask.entity.set(wall_entity);
       state.tempentitymask.set(wall_entity);
       state.selmask.entity.set(beam_entity);
       state.tempentitymask.set(beam_entity);
    }
  y = int(surface->getheight()) - 1 - y;
  //dmin1 = 4.0;  dmin2 = 4.0;  epicked = NULL;
  dmin1 = db.getstoler();  dmin2 = db.getstoler();  epicked = NULL;
  if (position == NULL) p = NULL; else p = & p1;
  if (state.selmask.entity.test(group_entity))
    for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
      {  if (e->isa(group_entity) &&
             e->isselectable(surface) &&
             (d = e->pick(surface,x,y,p)) >= 0.0
             && (d < dmin1 || d < dmin2) &&
             ! state.excludelist.inlist(e))
           {EntityList alist;
            int parent;
              alist = e->getalist();
              parent = 0;
              for (alist.start() ; (ge = alist.next()) != NULL ; )
                if (ge->isa(group_entity) && ge->isselectable(surface)  && ! state.excludelist.inlist(ge) && ((Group *)ge)->getlist().inlist(e))
                  parent = 1;
              if (!parent)
                {  if (d < dmin1)
                     {
                      if (surface->getlastselectedentity() != e)
                      {
                          epicked = e;
                      }
                      else if (epicked == NULL)
                      {
                          epicked = e;
                      }
                      else
                      {
                          skipRemaining = true;
                          break;
                      }
                     }

                }
           }

         if (interrupt && (i % 100) == 0)
           {  GetCursorPos(&cp2);
              if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
                {  if (snap)
                     {  state.selmask.entity = entity1;
                        state.tempentitymask = entity2;
                     }
                   return epicked;
                }
           }

      }
  if (!skipRemaining && (state.selmask.entity.test(xyz_mask) || state.selmask.entity.test(point_entity)))
    for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
      {  if (e->isa(point_entity) &&
             e->isselectable(surface) &&
             (d = e->pick(surface,x,y,p)) >= 0.0 &&
             (d < dmin1 || d < dmin2)  && !
             state.excludelist.inlist(e))
           {  if (d < dmin1)
                {
                    if (surface->getlastselectedentity() != e)
                    {
                        epicked = e;
                    }
                    else if (epicked == NULL)
                    {
                        epicked = e;
                    }
                    else
                    {
                        skipRemaining = true;
                        break;
                    }
                }

           }

         if (interrupt && (i % 100) == 0)
           {  GetCursorPos(&cp2);
              if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
                {  if (snap)
                     {  state.selmask.entity = entity1;
                        state.tempentitymask = entity2;
                     }
                   return epicked;
                }
           }

      }
  if (epicked != NULL)
    {  if (snap)
         {  state.selmask.entity = entity1;
            state.tempentitymask = entity2;
         }
       //return epicked;
    }
  if (!skipRemaining)
  {
      for (list.start(),i = 0 ; (e = list.next()) != NULL ; i++)
        {  if (! e->isa(group_entity) &&
               e->isselectable(surface) &&
               (d = e->pick(surface,x,y,p)) >= 0.0 &&
               (d < dmin1 || d < dmin2) && !
               state.excludelist.inlist(e))
             {  if (d < dmin1)
                  {
                      if (surface->getlastselectedentity() != e)
                      {
                          epicked = e;
                      }
                      else if (epicked == NULL)
                      {
                          epicked = e;
                      }
                      else
                      {
                          skipRemaining = true;
                          break;
                      }
                  }

             }
           if (interrupt && (i % 100) == 0)
             {  GetCursorPos(&cp2);
                if (abs(cp1.x - cp2.x) > 2 || abs(cp1.y - cp2.y) > 2)
                  {  if (snap)
                       {  state.selmask.entity = entity1;
                          state.tempentitymask = entity2;
                       }
                     return epicked;
                  }
             }
        }
      if (snap)
        {  state.selmask.entity = entity1;
           state.tempentitymask = entity2;
        }
    }

//if (! interrupt)
//{
//  DWORD tticks = GetTickCount() - ticks;
//  FILE *dfile = fopen("c:\\debug.log","a");
//  fprintf(dfile,"Pick Time : %d\n",tticks);
//  fclose(dfile);
//}

  return epicked;
}

Entity *GeometryDatabase::pickhor(View3dSurface *surface,int x,int y,Point *start,Point *end,Point *uv1,Point *uv2,double *side)
{double dmin,d;
 Entity *e,*epicked;
 BitMask entity1(32),entity2(32);
  y = int(surface->getheight()) - 1 - y;
  dmin = -1.0;  epicked = NULL;
  for (list.start() ; (e = list.next()) != NULL ; )
    if (e->islinear() && e->isselectable(surface) && ! state.excludelist.inlist(e) &&
         (d = ((LinearEntity *)e)->pickhor(surface,x,y,0,0,0,0,side)) >= 0.0 && (d < dmin || dmin < 0.0))
      {  dmin = d;
         epicked = e;
      }
  if (epicked != 0 && start != 0)
    ((LinearEntity *)epicked)->pickhor(surface,x,y,start,end,uv1,uv2,side);
  return epicked;
}

int GeometryDatabase::pickoriginpoint(View3dSurface *surface,int snap,int x,int y,Point *p)
{int end;
 Entity *e;
  for (list.start() ; (e = list.next()) != NULL ; )
    if ((end = e->pickoriginpoint(surface,snap,x,y,p)) != 0)
      return end;
  return 0;
}

int GeometryDatabase::pickmodelspaceoriginpoint(View3dSurface *surface,int snap,int x,int y,Point *p)
{
    double d1;
    int end;
    Point origin(0.0,0.0,0.0);

    end = 0;
    if ((d1 = surface->pick(origin,snap,x,y)) >= 0.0)
    {
        end = 3;  *p = origin;
    }

    return end;
}

LinearEntity *GeometryDatabase::trace(View3dSurface *vs,Loops *loops,Point end,Point uv1,Point uv2,double side,Point *newend,Point *uv3,Point *uv4,double *angle)
{double angle1;
 int same;
 Entity *e,*e1,*epicked;
 Point newend1,uv5,uv6,org,xaxis,yaxis,normal;
 EntityList l;
  *angle = -1.0;  epicked = 0;
  for (list.start() ; (e = list.next()) != NULL ; )
    if (e->getstatus() == 0 && e->islinear() && e->isselectable(vs) && ! state.excludelist.inlist(e) &&
         ((LinearEntity *)e)->trace(vs,end,uv1,uv2,side,&newend1,&uv5,&uv6,&angle1) != 0 && (angle1 < *angle || epicked == 0))
      {  l = loops->getloops();
         same = 0;
         for (l.start() ; ! l.atend() ; )
           {  e1 = l.next();
              if (e1->issame(Geometry,*e))
                {  same = 1;  break;
                }
           }
         if (same == 0 && loops->linearorplanar(e))
           {  epicked = e;
              *newend = newend1;
              *uv3 = uv5;
              *uv4 = uv6;
              *angle = angle1;
           }
      }
  return (LinearEntity *)epicked;
}



void RBFlashEntity(int draw,void *data,int x,int y,View3dSurface *surface)
{
 Entity *e = (Entity *) data;
 int i;

  if (draw == RB_TIMER)
  {
       //i = (GetTickCount() / 350) % 3;
       //i = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
       //e->draw(i == 0 ? DM_FLASH1 : i == 1 ? DM_FLASH2 : DM_FLASH3);
       cadwindow->updateAllViews();
  }
}

/**
 * KMJ these functions are for the Qt version of the trace dialog
 */
static int traceYesNo = -1;
void gotTraceYes()
{
    traceYesNo =  IDYES;
}

void gotTraceNo()
{
    traceYesNo = IDNO;
}

void gotTraceCancel()
{
    traceYesNo =  FALSE;
}

// the timer call back function
// this updated the windows and flashes the next entity
void FlashTraceEntity()
{
    cadwindow->updateAllViews();
}

// the dialog loop function
// the dialog is set to modal but we pump the event loop from this function
// to make the entity flash while the dialog is up
void getTraceYesOrNo()
{
    static Trace_Dialog *dlg=0;

    if(dlg == 0)
    {
        dlg = new Trace_Dialog();
        QObject::connect(dlg,&Trace_Dialog::gotYes,gotTraceYes);
        QObject::connect(dlg,&Trace_Dialog::gotNo,gotTraceNo);
        QObject::connect(dlg,&Trace_Dialog::rejected,gotTraceCancel);
    }
    // the dialog is made modal here
    // KMJ : TODO : think about using a nonmodal so the user can
    // zoom and scroll the screen during the command??
    traceYesNo = -1;
    dlg->setModal(true);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
    // loop here until user chooses yes or no or cancel
    // with the dialog
    int count=1000;
    while(traceYesNo == -1)
    {
        // pump the event loop regularly
        //adjust this number for better response
        if(count == 1000)
        {
            app->processEvents();
            count=0;
        }
        count++;
    }
    dlg->hide();
}
// KMJ end of new trace dialog functions

// this function has been changed to use OpenGL to flash the branch entity
// instead of using the rubberband call back
// different techniques could also be used
int GeometryDatabase::trace(LinearEntity *starte,EntityList *tlist)
{Point p1,startp,endp;
 Entity *e;
 LinearEntity *le1,*le2;
 double ptoler;
 EntityList connected;
 int same,i,n,result;
 //Dialog dialog(_RCT("Trace_dialog"));

  //dialog.add(new ButtonDialogControl(IDYES));
  //dialog.add(new ButtonDialogControl(IDNO));
  ptoler = db.getptoler();
  tlist->add(starte);
  startp = starte->start();
  endp = starte->end();
  for (list.start() ; (e = list.next()) != NULL ; )
    e->setstatus(0);
  starte->setstatus(1);
  for (i = 0 ; i < 2 ; i++)
    {  for (;;)
         {  for (list.start() ; (e = list.next()) != NULL ; )
              if (e->getstatus() == 0 && e->islinear() && e->isselectable(0) && ! state.excludelist.inlist(e))
                {  le1 = (LinearEntity *) e;
                   if ((endp - le1->start()).length() < ptoler ||
                       (endp - le1->end()).length() < ptoler)
                     {  same = 0;
                        for (connected.start() ; (le2 = (LinearEntity *) connected.next()) != 0 ; )
                          if (le1->issame(Geometry,*le2))
                            {  same = 1;  break;
                            }
                        for (tlist->start() ; (le2 = (LinearEntity *) tlist->next()) != 0 ; )
                          if (le1->issame(Geometry,*le2))
                            {  same = 1;  break;
                            }
                        if (! same)
                          connected.add(le1);
                     }
                }
            n = 0;
            if (connected.length() == 1)
              {  connected.start();   le1 = (LinearEntity *) connected.next();
                 n = 1;
                 tlist->add(le1);
                 if ((endp - le1->start()).length() < ptoler)
                   endp = le1->end();
                 else
                   endp = le1->start();
                 le1->setstatus(1);
                 if ((startp - endp).length() < ptoler)
                   {  connected.destroy();  return 1;
                   }
              }
            else if (connected.length() > 1)
              {  tlist->highlight();
                 for (; n == 0 ;)
                   for (connected.start() ; (le1 = (LinearEntity *) connected.next()) != 0 ; )
                     {
                       // this is the current branch entity
                       // it will flash for the user
                        app->mFlashEntity = le1;
                        le1->highlight();
                        // not using the rubberband technique
                        // for opengl display
                        //rubberband.begin(0);
                        //rubberband.add(RBFlashEntity,le1);

                        // this timer is used to flash the branch entity
                        // it just forces update of all the windows
                        // gl takes care of the colours
                        FlashTraceEntity();
                        app->mFlashTimer = new QTimer(app);
                        QObject::connect(app->mFlashTimer,&QTimer::timeout,FlashTraceEntity);
                        app->mFlashTimer->start(10);

                        //result = dialog.process();
                        // put up the dialog and wait for the user to click yes or no

                        getTraceYesOrNo();

                        // reset everything and remove the timer
                        // KMJ : the timer could be a permanent member
                        // we would then just switch it on and off

                        //rubberband.end(0);
                        le1->unhighlight();
                        app->mFlashEntity=0;
                        app->mFlashTimer->stop();
                        delete app->mFlashTimer;
                        app->mFlashTimer=0;

                        if (traceYesNo == FALSE)
                        {
                             tlist->unhighlight();
                             tlist->destroy();
                             connected.destroy();
                             return 0;
                        }
                        if (traceYesNo == IDYES)
                        {
                             n = 1;
                             tlist->add(le1);
                             if ((endp - le1->start()).length() < ptoler)
                               endp = le1->end();
                             else
                               endp = le1->start();
                             le1->setstatus(1);
                             if ((startp - endp).length() < ptoler)
                               {  tlist->unhighlight();
                                  connected.destroy();
                                  return 1;
                               }
                             break;
                        }
                     }
                 tlist->unhighlight();
              }
            connected.destroy();
            if (n == 0)
              break;
         }
       tlist->reverse();
       p1 = startp;  startp = endp;  endp = p1;
    }
  return 1;
}

int GeometryDatabase::pick(View3dSurface *surface,int nvertices,Point *pwvertices,int inside,EntityList *entitylist)
{Entity *e;

  list.start();
  while ((e = list.next()) != NULL)
    {  if (e->isselectable(surface) && e->pick(surface,nvertices,pwvertices,inside) && ! state.excludelist.inlist(e))
         entitylist->add(e);
    }

  return entitylist->length() > 0;
}

EntityList GeometryDatabase::pickall(View3dSurface *surface)
{
    EntityList selection;
    Entity *e;
    list.start();
    while ((e = list.next()) != NULL)
    {
        if(surface == 0)
        {
            int visible=0;
            for (int i = 0 ; i < MaxViews ; i++)
            {
                if (e->getvisible()->test(i))
                    visible=1;
            }
            if(visible == 1)
                selection.add(e);
        }
        else
        {
            if (e->isselectable(surface)/* && ! state.excludelist.inlist(e)*/)
                selection.add(e);
        }
    }
    return selection;
}

EntityList GeometryDatabase::pick(View3dSurface *surface,int interrupt,int x1,int y1,int x2,int y2)
{EntityList selection;
 Entity *e;
 int inside,t;
  inside = 1;
  if (x1 > x2) { t = x1;  x1 = x2;  x2 = t;   inside = 0; }//fg.2022

  y1 = int(surface->getheight()) - 1 - y1;
  y2 = int(surface->getheight()) - 1 - y2;
  if (y1 > y2) {  t = y1;  y1 = y2;  y2 = t;  }
  // allow user to drag rectangle in different direction fg.2022
  x1 = min(x1, x2); x2 = max(x1, x2);
  y1 = min(y1, y2); y2 = max(y1, y2);
  if (GetPrivateProfileInt("Configuration","DisableReverseSelection",0,home.getinifilename()) != 0)
    inside = 1;

  list.start();
  while ((e = list.next()) != NULL)
    {  if (e->isselectable(surface) && e->pick(surface,inside,x1,y1,x2,y2) && ! state.excludelist.inlist(e))
         selection.add(e);
    }
  return selection;
}

GeometryDatabase::~GeometryDatabase()
{ clear();
  delete [] idtoentity;
}

void GeometryDatabase::clear(void)
{Entity *e;

  state.sethandledentity(0);
  state.setpickedentity(0);

  for (;;)
    {  list.start();
       while ((e = list.next()) != 0 && ! e->getalist().empty()) ;
       if (e == 0) break;
       e->dbdelete();
       delete e;
       list.GeneralList::del(e);
    }
  list.destroy();
  nextid = 1;
}

void GeometryDatabase::reverse(void)
{ list.reverse();
}

int GeometryDatabase::save(CadFile *outfile)
{Entity *e;
 long index;
  list.reverse();
  for (list.start(),index = 1 ; (e = list.next()) != NULL ; index++)
    e->setindex(index);
  for (list.start() ; (e = list.next()) != NULL ; )
    if (! e->save(outfile))
      {  list.reverse();  return 0;
      }
  list.reverse();
  return 1;
}

int GeometryDatabase::save(CadFile *outfile,EntityList slist)
{Entity *e;
 long index;
  for (list.start() ; (e = list.next()) != NULL ; )
    e->setstatus(0);
  for (slist.start() ; (e = slist.next()) != NULL ; )
    {  e->setstatus(1);
       e->dbsetassociated();
    }
  for (list.start(),index = 1 ; (e = list.next()) != NULL ; )
    if (e->getstatus()) e->setindex(index++);
  for (list.start() ; (e = list.next()) != NULL ; )
    if (e->getstatus())
      {  if (! e->save(outfile)) return 0;
      }
  return 1;
}

int Database::loadgeometry(CadFile *infile,Transform *t)
{short type,previous;
 long nentities,flength,percent,oldpercent,c;
 Entity *e;
 RCCHAR mess[300];
 EntityList list;
 Point oldorigin(0.0,0.0,0.0);
 ResourceString rsnc0(0);
 ResourceString rs1(NCENT+1);
 ResourceString rs2(NCENT+2);
 ResourceString rs13(NCENT+13);
 Transform t1;
  flength = infile->length();
  for (geometry.start() ; (e = geometry.next()) != NULL ; )
    e->setstatus(1);
  nentities = 0;  previous = -1;
  oldpercent = 0;
  sprintf(mess,rs13.gets(),oldpercent);
  cadwindow->prompt->normalprompt(mess);
  c = 0;
  while (infile->read(&type))
    {  c++;
       if (type >= 999 && type != origin_record)
         break;
       else if (type < 1000)
         {  if (classfunctions.entityload(type) == NULL)
              {  sprintf(mess,rs1.gets(),type,previous);
                 cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                 cadwindow->prompt->normalprompt(NCWIN+1);
                 return 0;
              }
            else if ((e = classfunctions.entityload(type)(1,infile)) != NULL)
              {  e->setindex(++nentities);
                 list.add(e);
              }
            else
              {  sprintf(mess,rs2.gets(),type,previous);
                 cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                 cadwindow->prompt->normalprompt(NCWIN+1);
                 return 0;
              }
         }
       else if (type == origin_record)
         oldorigin.load(infile);
       else
         {  sprintf(mess,rs1.gets(),type,previous);
            cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
            cadwindow->prompt->normalprompt(NCWIN+1);
            return 0;
         }
       previous = type;
       percent = long(infile->position() / double(flength) * 100.0);
       if (percent != oldpercent)
         {  oldpercent = percent;
            sprintf(mess,rs13.gets(),oldpercent);
            cadwindow->prompt->normalprompt(mess);
         }
    }

  for (list.start() ; (e = list.next()) != 0 ; )
    geometry.add(e,0);

  geometry.fixupreferences(this,nentities);
  if (t != 0) {
	  t1.translate(-oldorigin);
	  t->apply(t1);
  }
  geometry.clearstatus();
  for (list.start() ; (e = list.next()) != NULL ; )
    e->setstatus(1);
  if (t != 0) {
	  for (list.start(); (e = list.next()) != NULL; )		  
		 if (e->getalist().empty() ) e->move(this, *t, 0);
	  for (list.start(); (e = list.next()) != NULL; )		  
		if (!e->getalist().empty()) e->move(this, *t, 0);
	  for (list.start(); (e = list.next()) != NULL; )
		  e->draw(DM_NORMAL);
  }
  list.destroy();
  cadwindow->prompt->normalprompt(NCWIN+1);
  return 1;
}

void Database::setname(const char *n)
{
    setname(CHAR2RCCHR(n));
}
void Database::setname(QString n)
{
    setname(n.data());
}

void Database::setname(const RCCHAR *n)
{
  strcpy(name,n);
  if (_stricmp(name,"Untitled") != 0)
    {  v.setstring("fi::lastfilename",name);
       WritePrivateProfileString(_RCT("Strings"),_RCT("fi::lastfilename"),name,home.getinifilename());
    }
}

int GeometryDatabase::fixupreferences(Database *db,long nentities)
{Entity * HUGE *index,*e;
 ResourceString rsnc0(NC);
 ResourceString rs3(NCENT+3);
  if ((index = new Entity * HUGE [nentities+1]) == NULL)
    {  cadwindow->MessageBox(rs3.gets(),rsnc0.gets(),MB_ICONSTOP);
       return 0;
    }
  for (list.start() ; (e = list.next()) != NULL ; )
    if (! e->getstatus()) index[e->getindex()] = e;
  for (list.start() ; (e = list.next()) != NULL ; )
    if (! e->getstatus())
      {  e->fixupreferences(index);
         e->dbinsert(db);
      }
  delete [] index;

  return 1;
}

GeometryDatabase::GeometryDatabase()
{  nextid = 1;
   tablesize = 10000;
   idtoentity = new Entity * HUGE [tablesize];
   for (int i = 0; i < tablesize; i++)
	   idtoentity[i] = 0;
}

#define EXTEND_SIZE 10000

int GeometryDatabase::add(Entity *entity,int draw,int undo)
{Entity * HUGE *newtable;
 long i;
 int status;
 rubberband.drawing=0;
  if (entity == 0)
    return 0;
  if (! entity->getdefined())
    {  delete entity;
       return 0;
    }
  
  if (entity->getid() < 0 )
	  entity->setid(nextid++);

  while (idtoentity != NULL && entity->getid() >= tablesize)
    {  if ((newtable = new Entity * HUGE [tablesize + EXTEND_SIZE]) != NULL)
         { 			
			for (i = 0 ; i < tablesize ; i++)
              newtable[i] = idtoentity[i];
			for (i = tablesize; i < tablesize + EXTEND_SIZE; i++)
				newtable[i] = 0;
            tablesize += EXTEND_SIZE;
         }
       delete [] idtoentity;
       idtoentity = newtable;
    }
  if (idtoentity != NULL)
    idtoentity[entity->getid()] = entity;
  if ((status = list.add(&db,entity,draw)) != 0 && undo)
    db.saveundo(UD_INSERT,entity);
  return status;
}

void GeometryDatabase::del(Entity *entity,int draw,int undo)
{Entity *e;
  if (entity == state.gethandledentity())
    state.sethandledentity(0);
  if (entity->isa(figure_entity) && entity->getalist().length() == 1)
    {EntityList temp;
       temp = entity->getalist();
       temp.start();
       e = temp.next();
#if (VERSION != FLEXICAD)
       if (e->isa(wall_entity))
         ((Wall *)e)->del((Figure *)entity);
#endif
    }
  if (entity->getposition() != 0 && entity->getalist().empty())
    {  if (undo)
         db.saveundo(UD_DELETE,entity);
       idtoentity[entity->getid()] = 0;
       list.del(entity,draw);
    }
  else
    entity->unhighlight();
}

Entity *GeometryDatabase::getentity(long id)
{ return idtoentity[id];
}

// KMJ: reset the colour table
// type for future use to provide the possibilty
// to initialise with different standard types
void Database::initColourTable(int /*type*/)
{
    int i;

    colourtable[0].set(255,0,0);
    colourtable[1].set(0,255,0);
    colourtable[2].set(0,0,255);
    colourtable[3].set(255,255,0);
    colourtable[4].set(0,255,255);
    colourtable[5].set(255,0,255);
    colourtable[6].set(127,0,0);
    colourtable[7].set(0,127,0);
    colourtable[8].set(0,0,127);
    if (::v.getreal("wn::backgroundcolour.red") == 0.0 && ::v.getreal("wn::backgroundcolour.green") == 0.0 && ::v.getreal("wn::backgroundcolour.blue") == 0.0)
      colourtable[9].set(255,255,255);
    else
      colourtable[9].set(0,0,0);
    for (i = 10 ; i <= 244 ; i++)
      colourtable[i].sethls(i < 122 + 10 ? (i-10)/122.0*360.0 : (i-10-122)/122.0*360.0,i < 122 + 9 ? 0.5 : 0.25,1.00);
    for (i = 245 ; i <= 255 ; i++)
    {
        Colour colour;
        colour.red = colour.green = colour.blue = int(255.0 * double(i-245)/10.0);
        colourtable[i].set(colour.red,colour.green,colour.blue);
    }
    colourtable[255].set(255,255,255);

    // debug
    qDebug() << "debug : initColourTable()";
    for (i = 0 ; i <= 255 ; i++)
    {
        RCCOLORREF c; colourtable[i].get(&c);
        //qDebug() << "c::" << i << "=" << c;
    }
    //
}

void Database::clear(void)
{RCCHAR key[256];
 int i;
 ResourceString rsnc3(NC+3);
  // debug
  v.debug();
  //
  v.initialize();
  // debug
  v.debug();
  //
  atoler = v.getreal("db::atoler");
  ptoler = v.getreal("db::ptoler");
  stoler = v.getreal("db::stoler");
  dtoler = v.getreal("db::dtoler");
  ttoler = v.getreal("db::ttoler");
  minimumlineweight = v.getreal("db::minimumlineweight");
  numericdisplaytolerance = v.getinteger("db::numericdisplaytolerance");
  dragdetail = v.getinteger("db::dragdetail");
  dynamiccursor = v.getinteger("wn::dynamiccursor");
  enableautoplotscale = v.getinteger("vw::enableautoplotscale");
  autoplotscale = v.getreal("vw::autoplotscale");
  // don't allow a black or white select colour
  selectcolourred = v.getreal("db::selectcolour.red");
  selectcolourgreen = v.getreal("db::selectcolour.green");
  selectcolourblue = v.getreal("db::selectcolour.blue");
  if(selectcolourred == 0.0 && selectcolourgreen == 0.0 && selectcolourblue == 0.0)
  {
      v.setreal("db::selectcolour.red",1.0);
      v.setreal("db::selectcolour.green",0.0);
      v.setreal("db::selectcolour.blue",1.0);
  }
  if(selectcolourred == 1.0 && selectcolourgreen == 1.0 && selectcolourblue == 1.0)
  {
      v.setreal("db::selectcolour.red",1.0);
      v.setreal("db::selectcolour.green",0.0);
      v.setreal("db::selectcolour.blue",1.0);
  }
  selectcolourred = v.getreal("db::selectcolour.red");
  selectcolourgreen = v.getreal("db::selectcolour.green");
  selectcolourblue = v.getreal("db::selectcolour.blue");
  //
  state.destroyselection(0);
  if (cadwindow != 0)
    cadwindow->destroyall();
  geometry.clear();
  views3d.clear();
  views2d.clear();
  workplanes.clear();
  grid.clear();
  strcpy(name,rsnc3.gets());
  strcpy(title,rsnc3.gets());
  header.change(0,0,0,0);


  undofile.close();
  undofile.open(undofilename,_RCT("w+b"));
  redofile.close();
  redofile.open(redofilename,_RCT("w+b"));
  viewundofile.close();
  viewundofile.open(viewundofilename,_RCT("w+b"));

  mainmenu.selectionchanged(0);
  mainmenu.pasteredoundo(0);
  undoing = 0;
  if (cadwindow != 0)
    cadwindow->updatetitle();
  changed = 0;
  dismask.entity.setandclear(uv_mask,xyz_mask,linear_mask,surface_mask,uvsurface_mask,lineardimension_entity,angulardimension_entity,
                              distancebearingdimension_entity,circulardimension_entity,end_list);
  dismask.colour.setall();
  dismask.layer.setall();
  dismask.style.setall();
  dismask.weight.setall();
  snapmask.setall();
  state.selmask.entity.setandclear(uv_mask,xyz_mask,undo_mask,linear_mask,surface_mask,uvsurface_mask,
      lineardimension_entity,angulardimension_entity,
      distancebearingdimension_entity,circulardimension_entity,end_list);
  state.tempentitymask = state.selmask.entity;
  state.tempentitymask.set(xyz_mask);
  state.tempentitymask.set(uv_mask);

  state.selmask.colour.setall();
  state.selmask.layer.setall();
  state.selmask.style.setall();
  state.selmask.weight.setall();

  initColourTable();

  layernames.clear();

  //  Load the default layernames as well
  for (i = 0 ; i < 1000 ; i++)
    if (strlen(db.layernames.getlayername(i)) == 0)
      {RCCHAR key[300],value[300];
         sprintf(key,"Layer%d",i);
         GetPrivateProfileString(_RCT("LayerNames"),key,_RCT(""),value,300,home.getinifilename());
         db.layernames.setlayername(i,value);
      }


  // load the ini colour table
  loadColourTable(home.getinifilename());

  /*
  for (i = 0 ; i < 256 ; i++)
  {
      long c;
      sprintf(key,"cl::%d",i);
      if ((c = v.getlong(key)) != -MAXLONG)
      {
         colourtable[i].set(c);
      }
  }
  */

  for (i = 0 ; i < 256 ; i++)
    linestyles.del(i);

  lineweights.initialize();

  //  Setup the unit names
  setupunits();

}

Database::~Database()
{
DebugTraceLine(__FILE__,__LINE__);
  undofile.close();
  redofile.close();
  viewundofile.close();
DebugTraceLine(__FILE__,__LINE__);
  _unlink(viewundofilename);
DebugTraceLine(__FILE__,__LINE__);
  _unlink(undofilename);
DebugTraceLine(__FILE__,__LINE__);
  _unlink(redofilename);
DebugTraceLine(__FILE__,__LINE__);
copyAlogMap.clear();
}

extern void start_architect(void);
extern void clearnctask(void);

void Database::initialize(void)
{
  ResourceString rs0(NCVIEWA);
  LineStyle *ls;
  Point zero(0.0,0.0,0.0);
  clear();
  views2d.initialize();
  views3d.initialize();
  workplanes.initialize();
  grid.initialize();
  state.setposition(zero);

  requiresopengl = 0;

  linestyles = LineStyles();

  ls = new LineStyle();
  ls->add(4.0);  ls->add(4.0);
  linestyles.set(1,ls);
  ls = new LineStyle();
  ls->add(2.0);  ls->add(2.0);
  linestyles.set(2,ls);
  ls = new LineStyle();
  ls->add(4.0);  ls->add(2.0);
  linestyles.set(3,ls);
  ls = new LineStyle();
  ls->add(1.5);  ls->add(10.0);
  ls->add(1.5);  ls->add(3.0);
  linestyles.set(4,ls);
  ls = new LineStyle();
  ls->add(1.5);  ls->add(10.0);
  ls->add(1.5);  ls->add(3.0);
  ls->add(1.5);  ls->add(3.0);
  linestyles.set(5,ls);
  ls = new LineStyle();
  ls->add(1.0);  ls->add(1.0);
  linestyles.set(6,ls);
  ls = new LineStyle();
  ls->add(1.0);  ls->add(2.0);
  linestyles.set(7,ls);

  //  Load the dashed line styles as well
  linestyles.load(home.getinifilename());

  //  Load the default values.
  header.change(v.getinteger("df::colour"),v.getinteger("df::layer"),v.getinteger("df::style"),v.getinteger("df::weight"));

  /*
  // KMJ : pending
  clearnctask();
  if (v.getinteger("ar::active") != 0)
    {int s;
       architect_command0(&s,0,0);
       start_architect();
    }
  else
  */
    {
      if (db.views3d.match(rs0.gets()) != NULL)
      {
          if(cadwindow != NULL)
          {
            ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget)->debugView3dList();
            ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget)->debug();
            cadwindow->create3dsubwindow(db.views3d.match(rs0.gets()));
            ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget)->debugView3dList();
            ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget)->debug();
            cadwindow->paintall();
          }
      }
    }
}

int ClassFunctions::nfunctions = 100;

ClassFunctions::ClassFunctions()
{int i;
  entityloadtable = new EntityLoadFunction[nfunctions];
  entityundotable = new EntityUndoFunction[nfunctions];
  otherloadtable = new OtherLoadFunction[nfunctions];
  if (entityloadtable == NULL || otherloadtable == NULL || entityundotable == NULL)
    { RCMessageBox(NULL,_RCT("Failed to allocate entity load tables"),_RCT(""),MB_ICONSTOP);
       exit(0);
    }
  for (i = 0 ; i < nfunctions ; i++)
    {  entityloadtable[i] = NULL;
       otherloadtable[i] = NULL;
       entityundotable[i] = NULL;
    }
}

ClassFunctions::~ClassFunctions()
{ delete [] entityloadtable;
  delete [] entityundotable;
  delete [] otherloadtable;
}

int ClassFunctions::registerentityload(int index,EntityLoadFunction elf)
{ if (index >= nfunctions) return 0;
  entityloadtable[index] = elf;
  return 1;
}

int ClassFunctions::registerentityundo(int index,EntityUndoFunction euf)
{ if (index >= nfunctions) return 0;
  entityundotable[index] = euf;
  return 1;
}

int ClassFunctions::registerotherload(int index,OtherLoadFunction olf)
{ if (index - 1000 >= nfunctions) return 0;
  otherloadtable[index - 1000] = olf;
  return 1;
}

EntityLoadFunction ClassFunctions::entityload(int index)
{ return (index >= nfunctions ? NULL : entityloadtable[index]);
}

EntityUndoFunction ClassFunctions::entityundo(int index)
{ return (index >= nfunctions ? NULL : entityundotable[index]);
}

OtherLoadFunction ClassFunctions::otherload(int index)
{ return (index - 1000 >= nfunctions ? NULL : otherloadtable[index-1000]);
}

Database::Database() : header(0,0,0,0), snapmask(32)
{int i;
 RCCHAR key[256];

  DeleteCallback = 0;
  UserDeleteCallback = 0;
  InsertCallback = 0;
  MovedCallback = 0;
  VerifyCallback = 0;
  UndoCallback = 0;
  SelectionCallback = 0;
  RightClickCallback = 0;
  MessageCB = 0;
  CorePropertyCallback = 0;
  APIPropertyCallback = 0;

  TrimCommandCallback = 0;
  StartCommandCallback = 0;
  EndCommandCallback = 0;
  CopyAlogEntityCallback = 0;

  requiresopengl = 0;
  changed = 0;
  strcpy(name,"");  strcpy(title,"");
  strcpy(undofilename,home.gettempfilename(_RCT("undo")));
  undofile.open(undofilename,_RCT("w+b"));
  if (undofile.position() < 0)
   RCMessageBox(0,_RCT("The CAD Undo/Redo files are not available."),_RCT("CAD"),MB_ICONINFORMATION);
  strcpy(redofilename,home.gettempfilename(_RCT("redo")));
  redofile.open(redofilename,_RCT("w+b"));
  strcpy(viewundofilename,home.gettempfilename(_RCT("vundo")));
  viewundofile.open(viewundofilename,_RCT("w+b"));
  undoing = 0;

  dismask.entity.setandclear(uv_mask,xyz_mask,linear_mask,surface_mask,uvsurface_mask,lineardimension_entity,angulardimension_entity,
                              distancebearingdimension_entity,circulardimension_entity,end_list);
  dismask.colour.setall();
  dismask.layer.setall();
  dismask.style.setall();
  dismask.weight.setall();
  snapmask.setall();

  initColourTable();

  for (i = 0 ; i < 256 ; i++)
  {
      long c;
      sprintf(key,"cl::%d",i);
      if ((c = v.getlong(key)) != -MAXLONG)
      {
         colourtable[i].set(c);
      }
  }

  units = v.getinteger("db::units");
  setupunits();
  atoler = v.getreal("db::atoler");
  ptoler = v.getreal("db::ptoler");
  stoler = v.getreal("db::stoler");
  dtoler = v.getreal("db::dtoler");
  ttoler = v.getreal("db::ttoler");
  minimumlineweight = v.getreal("db::minimumlineweight");
  numericdisplaytolerance = v.getinteger("db::numericdisplaytolerance");
  //dragdetail = v.getinteger("db::dragdetail"); // obsolete
  dragdetail = 0; //ways high detail
  dynamiccursor = v.getinteger("wn::dynamiccursor");
  enableautoplotscale = v.getinteger("vw::enableautoplotscale");
  autoplotscale = v.getreal("vw::autoplotscale");
  lineweightdisplay = v.getinteger("db::lineweightdisplay");
  zoomstep = v.getinteger("db::zoomstep");
  selectcolourred = v.getreal("db::selectcolour.red");
  selectcolourgreen = v.getreal("db::selectcolour.green");
  selectcolourblue = v.getreal("db::selectcolour.blue");

  //  Load the default values.
  header.change(v.getinteger("df::colour"),v.getinteger("df::layer"),v.getinteger("df::style"),v.getinteger("df::weight"));

}

static double Fraction[] =
  {  0.0,
     1.0/2.0,
     1.0/3.0,
     2.0/3.0,
     1.0/4.0,
     2.0/4.0,
     3.0/4.0,
     1.0/5.0,
     2.0/5.0,
     3.0/5.0,
     4.0/5.0,
     1.0/6.0,
     2.0/6.0,
     3.0/6.0,
     4.0/6.0,
     5.0/6.0,
     1.0/64.0,
     2.0/64.0,
     3.0/64.0,
     4.0/64.0,
     5.0/64.0,
     6.0/64.0,
     7.0/64.0,
     8.0/64.0,
     9.0/64.0,
     10.0/64.0,
     11.0/64.0,
     12.0/64.0,
     13.0/64.0,
     14.0/64.0,
     15.0/64.0,
     16.0/64.0,
     17.0/64.0,
     18.0/64.0,
     19.0/64.0,
     20.0/64.0,
     21.0/64.0,
     22.0/64.0,
     23.0/64.0,
     24.0/64.0,
     25.0/64.0,
     26.0/64.0,
     27.0/64.0,
     28.0/64.0,
     29.0/64.0,
     30.0/64.0,
     31.0/64.0,
     32.0/64.0,
     33.0/64.0,
     34.0/64.0,
     35.0/64.0,
     36.0/64.0,
     37.0/64.0,
     38.0/64.0,
     39.0/64.0,
     40.0/64.0,
     41.0/64.0,
     42.0/64.0,
     43.0/64.0,
     44.0/64.0,
     45.0/64.0,
     46.0/64.0,
     47.0/64.0,
     48.0/64.0,
     49.0/64.0,
     50.0/64.0,
     51.0/64.0,
     52.0/64.0,
     53.0/64.0,
     54.0/64.0,
     55.0/64.0,
     56.0/64.0,
     57.0/64.0,
     58.0/64.0,
     59.0/64.0,
     60.0/64.0,
     61.0/64.0,
     62.0/64.0,
     63.0/64.0,
     -1.0
  };

static const char *SFraction[] =
  {  "",
     "1/2",
     "1/3",
     "2/3",
     "1/4",
     "2/4",
     "3/4",
     "1/5",
     "2/5",
     "3/5",
     "4/5",
     "1/6",
     "2/6",
     "3/6",
     "4/6",
     "5/6",
     "1/64",
     "1/32",
     "3/64",
     "1/16",
     "5/64",
     "3/32",
     "7/64",
     "1/8",
     "9/64",
     "5/32",
     "11/64",
     "3/16",
     "13/64",
     "7/32",
     "15/64",
     "1/4",
     "17/64",
     "9/32",
     "19/64",
     "5/16",
     "21/64",
     "11/32",
     "23/64",
     "3/8",
     "25/64",
     "13/32",
     "27/64",
     "7/16",
     "29/64",
     "15/32",
     "31/64",
     "1/2",
     "33/64",
     "17/32",
     "35/64",
     "9/16",
     "37/64",
     "19/32",
     "39/64",
     "5/8",
     "41/64",
     "21/32",
     "43/64",
     "11/16",
     "45/64",
     "23/32",
     "47/64",
     "3/4",
     "49/64",
     "25/32",
     "51/64",
     "13/16",
     "53/64",
     "27/32",
     "55/64",
     "7/8",
     "57/64",
     "29/32",
     "59/64",
     "15/16",
     "61/64",
     "31/32",
     "63/64",
     ""
  };


RCCHAR *Database::formatnumber(RCCHAR *buffer,double x,int showunits)
{
 double x1,x2;
 RCCHAR *sign,tsign[5],text[300],subunittext[300],*s;
 int i;

  if (showunits&2)
    sign = x < 0.0 ? strcpy(tsign,"-") : strcpy(tsign,"+");
  else
    sign = x < 0.0 ? strcpy(tsign,"-") : strcpy(tsign,"");
  x = fabs(x);
  x1 = x / mainunitscale;
  if (subunitscale == 0.0 || (showunits&FormatNumberNeverShowUnits) != 0)
    {
       sprintf(text,"%.*lf",numericdisplaytolerance,x1);
       if (enablefractions && (showunits&FormatNumberNoFractions) == 0)
         {
            if ((s = strchr(text,'.')) != 0)
              {  s++;
                 while (*s != 0 && *s == '0')
                   s++;
                 if (*s == 0)
                   *strchr(text,'.') = 0;
             }

            //  Try to match this to a fraction
            for (i = 0 ; Fraction[i] >= 0.0 ; i++)
              if (fabs(fmod(x1,1.0) - Fraction[i]) < 1.0E-5)
                {  if (x1 >= 1.0)
                     sprintf(text,"%.0lf %s",floor(x1),SFraction[i]);
                   else
                     strcpy(text,SFraction[i]);
                   break;
                }
         }
        if ((units == 3 || units == 4) && (showunits&FormatForEditField) != 0)
            sprintf(buffer,"%ls%ls%ls",sign,text,"\"");
        else
            sprintf(buffer,"%ls%ls%ls",sign,text,(showunits&FormatNumberAlwaysShowUnits) ? mainunitname : _RCT(""));
    }
  else
    {  x2 = fmod(x1,1.0) * subunitscale;
       if (numericdisplaytolerance == 0)
         sprintf(buffer,"%ls%.0lf%s",sign,x1,mainunitname);
       else
         {
            sprintf(text,"%.*lf",numericdisplaytolerance-1,x2);
            sprintf(subunittext,"%.*lf",numericdisplaytolerance-1,subunitscale);
            if (strcmp(text,subunittext) == 0)
              {  x1 = floor(x1) + 1.0;
                 x2 = fmod(x1,1.0) * subunitscale;
                 sprintf(text,"%.*lf",numericdisplaytolerance-1,x2);
              }

            if (enablefractions)
              {//  Try to match this to a fraction

                if ((s = strchr(text,'.')) != 0)
                {  s++;
                    while (*s != 0 && *s == '0')
                        s++;
                    if (*s == 0)
                        *strchr(text,'.') = 0;
                }


                 for (i = 0 ; Fraction[i] >= 0.0 &&  (showunits&FormatNumberNoFractions) == 0 ; i++)
                   if (fabs(fmod(x2,1.0) - Fraction[i]) < 1.0E-5)
                     {  if (x2 >= 1.0 & i == 0)
                          sprintf(text,"%.0lf",floor(x2));
                        else if (x2 >= 1.0)
                           sprintf(text,"%.0lf %s",floor(x2),SFraction[i]);
                        else
                          strcpy(text,SFraction[i]);
                        break;
                     }
              }

            if ((units == 3 || units == 4) && (showunits&FormatForEditField) != 0)
            {   if (floor(x1) == 0.0)
                    sprintf(buffer,_RCT("%ls%ls%ls"),sign,strlen(text) == 0 ? _RCT("0") : text,_RCT("\""));
                else
                    sprintf(buffer,_RCT("%ls%.0lf%s %ls%ls"),sign,floor(x1),_RCT("'"),strlen(text) == 0 ? _RCT("0") : text,_RCT("\""));
            }
            else
            {   if (floor(x1) == 0.0)
                    sprintf(buffer,_RCT("%ls%ls%ls"),sign,text,subunitname);
                else
                    sprintf(buffer,_RCT("%ls%.0lf%ls %ls%ls"),sign,floor(x1),mainunitname,strlen(text) == 0 ? _RCT("0") : text,subunitname);
            }

         }
    }

  return buffer;
}

void Database::setupunits(void)
{ units = v.getinteger("db::units");
  switch (units)
    {case 0 :
       mainunitscale = 1.0;
       subunitscale = 0.0;
       strcpy(mainunitname,"mm");
       strcpy(subunitname,"");
       enablefractions = 0;
       break;
     case 1 :
       mainunitscale = 1000.0;  //  mm per metre
       subunitscale = 0.0;
       strcpy(mainunitname,"m");
       strcpy(subunitname,"");
       enablefractions = 0;
       break;
     case 2 :
       mainunitscale = 25.4;  //  mm per inch
       subunitscale = 0.0;
       strcpy(mainunitname,"in");
       strcpy(subunitname,"");
       enablefractions = 1;
       break;
     case 3 :
       mainunitscale = 25.4 * 12;  //  mm per foot
       subunitscale = 12.0;        //  inches per foot
       strcpy(mainunitname,"ft");
       strcpy(subunitname,"in");
       enablefractions = 1;
       break;
     case 4 :
       mainunitscale = 25.4 * 12;  //  mm per foot
       subunitscale = 12.0;        //  inches per foot
       strcpy(mainunitname,"'");
       strcpy(subunitname,"\"");
       enablefractions = 1;
       break;
     case 5 :
       mainunitscale = 10.0;  //  mm per centremetre
       subunitscale = 0.0;
       strcpy(mainunitname,"cm");
       strcpy(subunitname,"");
       enablefractions = 0;
       break;
    }
}

void Database::SetModifyCallback(int type,ModifyEntityCallback cb)
{ if (type == ECDelete)
    DeleteCallback = cb;
  else if (type == ECUserDelete)
    UserDeleteCallback = cb;
  else if (type == ECInsert)
    InsertCallback = cb;
  else if (type == ECVerify)
    VerifyCallback = cb;
  else if (type == ECMoved)
    MovedCallback = cb;
  else if (type == ECUndo)
    UndoCallback = cb;
  else if (type == ECSelection)
    SelectionCallback = cb;
  else if (type == ECRightClick)
    RightClickCallback = cb;
}

void Database::SetPropertyCallback(int api,PropertyCallback cb)
{ if (api == ECDelete)
    APIPropertyCallback = cb;
  else
    CorePropertyCallback = cb;
}

int Database::Properties(Entity *e,int action,int id,void *data,RCCHAR *text)
{int result;
    QString qtext(text);
    if (CorePropertyCallback != 0 && (result = CorePropertyCallback(e,action,id,data,qtext.toLatin1().data())) != INT_MAX)
        return result;
    if (APIPropertyCallback != 0 && (result = APIPropertyCallback(e,action,id,data,qtext.toLatin1().data())) != INT_MAX)
        return result;
    return INT_MAX;
}

int Database::Properties(Entity *e,int action,int id,void *data,char *text)
{int result;
    if (CorePropertyCallback != 0 && (result = CorePropertyCallback(e,action,id,data,text)) != INT_MAX)
        return result;
    if (APIPropertyCallback != 0 && (result = APIPropertyCallback(e,action,id,data,text)) != INT_MAX)
        return result;
    return INT_MAX;
}


void Database::rubberbandOff()
{
    rubberband.off();
}

void Database::SetMessageCallback(MessageCallback mcb)
{ MessageCB = mcb;
}


extern void ClearAPICommandCancelled(void);


void Database::Verify(Entity *e)
{
  ClearAPICommandCancelled();
  if (VerifyCallback == 0 || VerifyCallback(e) == 0)
    e->verify();
}

void Database::Selection(Entity *e)
{ if (SelectionCallback != 0)
    SelectionCallback(e);
}

int Database::savetables(CadFile *outfile) const
{short i,type;
  type = tables_record;
  if (! outfile->write(&type)) return 0;
  for (i = 0 ; i < 256 ; i++)
    colourtable[i].save(outfile);
  linestyles.save(outfile);
  lineweights.save(outfile);
  return 1;
}

int Database::loadtables(CadFile *infile)
{int i;
  for (i = 0 ; i < 256 ; i++)
    if (! colourtable[i].load(infile)) return 0;
  if (! linestyles.load(infile)) return 0;
  if (! lineweights.load(infile)) return 0;
  return 1;
}

int Database::savemasktables(CadFile *outfile) const
{short type;
  type = masktables_record;
  if (! outfile->write(&type)) return 0;
  if (! dismask.entity.save(outfile)) return 0;
  if (! dismask.colour.save(outfile)) return 0;
  if (! dismask.layer.save(outfile)) return 0;
  if (! dismask.style.save(outfile)) return 0;
  if (! dismask.weight.save(outfile)) return 0;
  type = 1;                               //  Don't save the selection masks for now - maybe we will later
  if (! outfile->write(&type)) return 0;
  if (! lockedmask.entity.save(outfile)) return 0;
  if (! lockedmask.colour.save(outfile)) return 0;
  if (! lockedmask.layer.save(outfile)) return 0;
  if (! lockedmask.style.save(outfile)) return 0;
  if (! lockedmask.weight.save(outfile)) return 0;
  return 1;
}

int Database::loadmasktables(CadFile *infile)
{short type;
 BitMask *bm;
  if (! (bm = dismask.entity.load(infile))) return 0;  dismask.entity = *bm;  delete bm;
  if (! (bm = dismask.colour.load(infile))) return 0;  dismask.colour = *bm;  delete bm;
  if (! (bm = dismask.layer.load(infile))) return 0;  dismask.layer = *bm;  delete bm;
  if (! (bm = dismask.style.load(infile))) return 0;  dismask.style = *bm;  delete bm;
  if (! (bm = dismask.weight.load(infile))) return 0;  dismask.weight = *bm;  delete bm;
  if (! infile->read(&type)) return 0;
  if (type == 1)
    {  if (! (bm = lockedmask.entity.load(infile))) return 0;  lockedmask.entity = *bm;  delete bm;
       if (! (bm = lockedmask.colour.load(infile))) return 0;  lockedmask.colour = *bm;  delete bm;
       if (! (bm = lockedmask.layer.load(infile))) return 0;  lockedmask.layer = *bm;  delete bm;
       if (! (bm = lockedmask.style.load(infile))) return 0;  lockedmask.style = *bm;  delete bm;
       if (! (bm = lockedmask.weight.load(infile))) return 0;  lockedmask.weight = *bm;  delete bm;
    }

  return 1;
}
int Database::loadCopyAlongMap(CadFile *infile) {
	long id,count;
	copyAlongDialogData data;	
	copyAlogMap.clear();
	if (!infile->read(&count)) return 0;
	for (int i = 0; i < count; i++) {
		if (!infile->read(&id)) return 0;
		if (!data.load(infile)) return 0;
		data.level = 1;
		copyAlogMap.insert(id, data);
		if (data.undoMap.isEmpty()) {
			copyAlongUndoData undoData;
			undoData.pt = data.pt;
			undoData.copyList = data.copyList;
			copyAlogMap[id].undoMap[1] = undoData;
		}
	}
	return 1;
}

extern QPixmap grabScreenshot();
QPixmap grabScreenshot()
{
    QPixmap desktopPixmap = QPixmap(QApplication::desktop()->geometry().size());
    QPainter p(&desktopPixmap);

    for (QScreen* screen : QApplication::screens())
        p.drawPixmap(screen->geometry().topLeft(), screen->grabWindow(0));

    return desktopPixmap;
}

int Database::save(CadFile *outfile,EntityList *slist)
{
 //long version = 2; // previous version
 //long version = -2; // temporary development version
 //long version = 3; // new version from Qt version
 long version = 4; // new version from Qt with preview in header
 Entity *e;

  loadedfigures.clear();

  if (v.getinteger("fi::autoattachdetach"))
    {  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(figure_entity))
           ((Figure *)e)->attachfigure();
    }

  //outfile->write(_RCT("CAD For Windows : Dedicated to Irina and Spot."));
  outfile->write("The Authentic CAD File.");
  outfile->write(&version);

  // KMJ write thumbnail/preview comes here
  // get the preview screen shot
  // call the preview generator here
  long previewType = v.getinteger("db::previewtype"); // -1= no preview saved; 0=screen shot type preview; 1=drawn image (saved as a figure)
  //int savePreview = v.getinteger("db::savepreview");
  QScreen *screen = QGuiApplication::primaryScreen();
  QPixmap screenshot,scaled;
  if(previewType == 1 || state.getcommandid() == 1710)
  {
      Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
      View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
      View3dBitmapOutput output(&top,1.0,256,256);
      output.setplotscale(1.0);
      output.getScene()->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255,
                                            ::v.getreal("wn::backgroundcolour.green")*255,
                                            ::v.getreal("wn::backgroundcolour.blue")*255)));

      db.geometry.extents(&output,&pmin,&pmax);

      double aspect=1;
      double du = fabs(pmax.x - pmin.x) / 1.9;
      double dv = fabs(pmax.y - pmin.y) / 1.9;

      if (du > db.getptoler() || dv > db.getptoler())
      {
           aspect = output.getwidthmm() / output.getheightmm();
           if (du / dv > aspect) dv = du / aspect;
           if (du / dv < aspect) du = dv * aspect;
           pmin.x = (pmin.x + pmax.x) / 2.0 - du;
           pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
           pmax.x = (pmin.x + pmax.x) / 2.0 + du;
           pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
           output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);
      }
      if(output.getsurfacetype() == View3dSurf)
      {
          EntityList list;
          list = *db.geometry.getlist();
          list.start();
          for (list.end() ; (e = list.last()) != 0 ; )
          {
              //if (e->isvisible(&output) && e->isa(group_entity) == 0)
              if (e->getvisible()->test(0) && e->isa(group_entity) == 0)
              {
                  int sel = e->getselected();
                  e->setselected(0);
                  //e->draw(DM_NORMAL,NULL,&output);
                  e->draw(DM_PRINT,NULL,&output);
                  e->setselected(sel);
              }
          }
      }
      output.render();
      scaled = *(QPixmap*)output.getBitmap();
      previewType = 1; // drawn image type (saved as a figure)
  }
  else if (previewType == 0 && screen)
  {
      QtMainWindow *mw = (QtMainWindow*)cadwindow->gethwnd();
      RCCentralWidget* cw =  ((QtMainWindow*)cadwindow->gethwnd())->centerWidget;
      QRect mrect = mw->geometry();
      QRect wrect = cw->geometry();

#ifdef _MAC
      QPoint p1 = cw->mapToGlobal(QPoint());
      QPoint p2 = cw->mapToGlobal(QPoint(wrect.width(),wrect.height()));
      p1.setX(p1.x()*cw->devicePixelRatio());
      p1.setY(p1.y()*cw->devicePixelRatio());
      p2.setX(p1.x()+(p2.x()*cw->devicePixelRatio()-p1.x()));
      p2.setY(p1.y()+(p2.y()*cw->devicePixelRatio()-p1.y()));
      QRect grect(p1,p2);

      // wait a bit until the save  dialogs have closed
      QTimer::singleShot(0, mw, SLOT(raise()));

#if 0
      // this is buggy replaced by the code below
      QImage im;
      if(cw->activeWindow() == nullptr )
      {
          // the rcview has gone
          // do something clever here
       }
      else
      {
        im = QImage(cw->activeWindow()->viewport()->size(),QImage::Format_RGB888);
        im = ((QGLWidget*)cw->activeWindow()->viewport())->grabFrameBuffer();
      }
#endif
      p1 = cw->mapToGlobal(QPoint());
      p2 = cw->mapToGlobal(QPoint(wrect.width(),wrect.height()));
      p1.setX(p1.x()*cw->devicePixelRatio());
      p1.setY(p1.y()*cw->devicePixelRatio());
      p2.setX(p1.x()+(p2.x()*cw->devicePixelRatio()-p1.x()));
      p2.setY(p1.y()+(p2.y()*cw->devicePixelRatio()-p1.y()));
      grect.setBottomLeft(p1);
      grect.setTopRight(p2);
      QPixmap pm = grabScreenshot();
      pm.save("debugscreenshot.png","PNG");
      screenshot = pm.copy(grect);
      screenshot.save("debugscreenshot2.png","PNG");
      // can't see wirefame without smoothing on highres displays
      scaled = screenshot.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      QImage backing(256,256,QImage::Format_RGB888);
      backing.fill(QColor(255,255,255));
      QPainter paint(&backing);
      paint.drawPixmap((256-scaled.width())/2,(256-scaled.height())/2,scaled.width(),scaled.height(),scaled,0,0,scaled.width(),scaled.height());
      scaled = QPixmap::fromImage(backing);
#else
#if 0
      // this is buggy replaced by the code below
      QImage im;
      if(cw->activeWindow() == nullptr )
      {
          // the rcview has gone
          // do something clever here
       }
      else
      {
        im = QImage(cw->activeWindow()->viewport()->size(),QImage::Format_RGB888);
        im = ((QGLWidget*)cw->activeWindow()->viewport())->grabFrameBuffer();
      }
#endif
      QPoint p1 = cw->mapToGlobal(QPoint());
      QPoint p2 = cw->mapToGlobal(QPoint(wrect.width(),wrect.height()));
      p1.setX(p1.x()*cw->devicePixelRatio());
      p1.setY(p1.y()*cw->devicePixelRatio());
      p2.setX(p1.x()+(p2.x()*cw->devicePixelRatio()-p1.x()));
      p2.setY(p1.y()+(p2.y()*cw->devicePixelRatio()-p1.y()));
      QRect grect(p1,p2);

      // wait a bit until the save  dialogs have closed
      QTimer::singleShot(0, mw, SLOT(raise()));

      QPixmap pm = grabScreenshot();
      pm.save("debugscreenshot.png","PNG");
      screenshot = pm.copy(grect);
      screenshot.save("debugscreenshot2.png","PNG");
      // can't see wirefame without smoothing on highres displays
      scaled = screenshot.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      QImage backing(256,256,QImage::Format_RGB888);
      backing.fill(QColor(255,255,255));
      QPainter paint(&backing);
      paint.drawPixmap((256-scaled.width())/2,(256-scaled.height())/2,scaled.width(),scaled.height(),scaled,0,0,scaled.width(),scaled.height());
      scaled = QPixmap::fromImage(backing);
#endif
      previewType = 0; // screen shot image type
  }
  QImage preview = scaled.toImage();

  //debug
  //scaled.save("screenshot3.png","PNG");
  // write type number just in case we want to change this in the future
  // type number -1 means that the preview has not been saved
  // type zero is rgb format image data
  // image is stored low to high pixel order (upside down)
  //if(! outfile->write(&previewType)) return 0;
  if(previewType >= 0)
  {
      preview = preview.convertToFormat(QImage::Format_RGB888);
      // debug
      //preview.save("preview.bmp","bmp");
      if(/*savePreview &&*/ preview.bits() !=0)
      {
          if(! outfile->write(&previewType)) return 0;
          // write the width and height of the preview so we can change it in the future
          long previewWidth = preview.width();
          if(! outfile->write(&previewWidth)) return 0;
          long previewHeight = preview.height();
          if(! outfile->write(&previewHeight)) return 0;
          // write the number of bytes in a pixel
          long previewBPP = 4;
          if(! outfile->write(&previewBPP)) return 0;
          // write the preview image data
          // the length of this data is width * height * bytes per pixel
          char *image = new char[previewWidth * previewHeight * previewBPP];
          memset(image,0,previewWidth * previewHeight * previewBPP);
          memcpy(image,preview.bits(),preview.byteCount());
          if(! outfile->blockwrite(image,previewWidth * previewHeight * previewBPP))
          {
              delete [] image;
              return 0;
          }
          delete [] image;
      }
      else
      {
          previewType = -1;
          if(! outfile->write(&previewType)) return 0;
          /*
          long previewWidth = 0;
          if(! outfile->write(&previewWidth)) return 0;
          long previewHeight = 0;
          if(! outfile->write(&previewHeight)) return 0;
          // write the number of bytes in a pixel
          long previewBPP = 4;
          if(! outfile->write(&previewBPP)) return 0;
          // write the preview image data
          // the length of this data is width * height * bytes per pixel
          char image[1]; image[0]=0;
          if(! outfile->blockwrite(image,previewWidth * previewHeight * previewBPP)) return 0;
          */
      }
  }
  else
  {
      previewType = -1;
      if(! outfile->write(&previewType)) return 0;
  }
  //

  if (slist == 0)
    {  if (! geometry.save(outfile)) return 0;
    }
  else
    {  if (! geometry.save(outfile,*slist)) return 0;
    }
  if (! v.save(outfile)) return 0;
  if (! views3d.save(outfile)) return 0;
  if (! views2d.save(outfile)) return 0;
  if (! workplanes.save(outfile)) return 0;
  if (! grid.save(outfile)) return 0;
  if( cadwindow != 0 )
  {
      if (! cadwindow->save(outfile))
          return 0;
  }
  if (! savetables(outfile)) return 0;
  if (! layernames.save(outfile)) return 0;
  if (GetPrivateProfileInt(_RCT("Save"),_RCT("NoMaskTables"),0,home.getinifilename()) == 0)
    {  if (! savemasktables(outfile)) return 0;
    }
  //save copyalongmap
  short type = copyAlong_record;
  if (!outfile->write(&type)) return 0;
  long mapSize = copyAlogMap.count();
  if (!outfile->write(&mapSize)) return 0;
  QMapIterator<int, copyAlongDialogData> i(copyAlogMap);
  while (i.hasNext()) {
	  i.next();
	  long id = i.key();
	  if (!outfile->write(&id)) return 0;
	  copyAlongDialogData data = i.value();
	  if (!data.save(outfile)) return 0;
  }
  changed = 0;
  return 1;
}

int Database::load(CadFile *infile)
{short type,previous;
 int c,fixedup,hidemeasuredialog,autosaveenabled;
 long nentities,percent,oldpercent,flength;
 RCCHAR mess[300];
 Entity *e;
 double autosavetimeout;
 ResourceString rsnc0(NC);
 ResourceString rs5(NCENT+5);
 ResourceString rs1(NCENT+1);
 ResourceString rs2(NCENT+2);
 ResourceString rs13(NCENT+13);

    hidemeasuredialog = v.getinteger("db::hidemeasuredialog");
    autosavetimeout = v.getreal("db::autosavetimeout");
    autosaveenabled = v.getinteger("db::autosaveenabled");

    nentities = 0;
    previous = -1;
    if (! infile->readheader())
    {   if (cadwindow != 0)
            cadwindow->MessageBox(rs5.gets(),rsnc0.gets(),MB_ICONSTOP);
        return 0;
    }
    flength = infile->length();
    oldpercent = 0;
    fixedup = 0;
    sprintf(mess,rs13.gets(),oldpercent);
    if (cadwindow != 0)
        cadwindow->prompt->normalprompt(mess);

    //  Don't save to the undo file when reading files.
    undofile.close();
    c = 0;

    while (infile->read(&type))
    {
        c++;
        if (type < 1000)
        {   if (classfunctions.entityload(type) == NULL)
            {   sprintf(mess,rs1.gets(),type,previous);
                if (cadwindow != 0)
                {   cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                    cadwindow->prompt->normalprompt(NCWIN+1);
                }
                undofile.open(undofilename,_RCT("w+b"));
                return 0;
            }
            else if ((e = classfunctions.entityload(type)(1,infile)) != NULL)
            {   e->setindex(++nentities);				
                db.geometry.add(e,0);
            }
            else
            {   sprintf(mess,rs2.gets(),type,previous);
                if (cadwindow != 0)
                {   cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                    cadwindow->prompt->normalprompt(NCWIN+1);
                }
                undofile.open(undofilename,_RCT("w+b"));
                return 0;
            }
        }
        else
        {
            if (GetPrivateProfileInt("Open","NoTables",0,home.getinifilename()) != 0)
            {   views3d.initialize();
                workplanes.initialize();
                break;  //  Skip All Tables
            }

            if (type == windows_record || type == splitter_record)
            {   geometry.fixupreferences(this,nentities);
                fixedup = 1;
            }

            if (classfunctions.otherload(type) == NULL)
            {   sprintf(mess,rs1.gets(),type,previous);
                if (cadwindow != 0)
                {   cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                    cadwindow->prompt->normalprompt(NCWIN+1);
                }
                undofile.open(undofilename,_RCT("w+b"));

                return 0;
            }
            else if (! classfunctions.otherload(type)(infile,this))
            {   sprintf(mess,rs2.gets(),type,previous);
                if (cadwindow != 0)
                {   cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                    cadwindow->prompt->normalprompt(NCWIN+1);
                }
                undofile.open(undofilename,_RCT("w+b"));
                return 0;
            }
        }
        previous = type;
        percent = long(infile->position() / double(flength) * 100.0);
        if (percent != oldpercent)
        {   oldpercent = percent;
            sprintf(mess,rs13.gets(),oldpercent);
            if (cadwindow != 0)
                cadwindow->prompt->normalprompt(mess);
        }
    }

    if (! fixedup)
        geometry.fixupreferences(this,nentities);
    changed = 0;
    if (cadwindow != 0)
        cadwindow->prompt->normalprompt(NCWIN+1);

    setdynamiccursor(v.getinteger("wn::dynamiccursor"));
    setupunits();

    undofile.open(undofilename,_RCT("w+b"));

    /*
    // KMJ TODO: fix view tabs
    cadwindow->viewtab->set_tab_index(v.getinteger("df::viewtab"));

    cadwindow->viewtab->update_window_state();
    */

    // v8 set the file paths from the local .ini settings if any
    QSettings RCSettings(QString(home.getinifilename()),QSettings::IniFormat);

    QString path(home.getFigureFilepath(_RCT("")));
    path = RCSettings.value("Strings/db::figures-components-path",path).toString();
    v.setstring("db::figures-components-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Title-Sheets")));
    path = RCSettings.value("Strings/db::title-sheets-path",path).toString();
    v.setstring("db::title-sheets-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Template-Drawings")));
    path = RCSettings.value("Strings/db::template-drawings-path",path).toString();
    v.setstring("db::template-drawings-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Images")));
    path = RCSettings.value("Strings/db::picture-images-path",path).toString();
    v.setstring("db::picture-images-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Hatch-Patterns")));
    path = RCSettings.value("Strings/db::hatch-patterns-path",path).toString();
    v.setstring("db::hatch-patterns-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Textures")));
    path = RCSettings.value("Strings/db::textures-path",path).toString();
    v.setstring("db::textures-path",path.data());

    path = QString(home.getpublicfilename(_RCT("Line-Styles")));
    path = RCSettings.value("Strings/db::line-styles-path",path).toString();
    v.setstring("db::line-styles-path",path.data());

    path = QString(home.getpublicfilename(_RCT("My Drawings")));
    path = RCSettings.value("Strings/db::my-drawings-path",path).toString();
    v.setstring("db::my-drawings-path",path.data());

    path = QString(home.getpublicfilename(_RCT("")));
    path = RCSettings.value("Strings/db::dwg-preprocessor-path",path).toString();
    v.setstring("db::dwg-preprocessor-path",path.data());

    path = QString(QDir::homePath());
    path = RCSettings.value("Strings/db::last-cad-file-path",path).toString();
    v.setstring("db::last-cad-file-path",path.data());

    path = QString(QDir::homePath());
    path = RCSettings.value("Strings/db::last-dxf-file-path",path).toString();
    v.setstring("db::last-dxf-file-path",path.data());

    path = QString(QDir::homePath());
    path = RCSettings.value("Strings/db::last-dwg-file-path",path).toString();
    v.setstring("db::last-dwg-file-path",path.data());
    // v8

    v.setinteger("db::hidemeasuredialog",hidemeasuredialog);
    v.setreal("db::autosavetimeout",autosavetimeout);
    v.setinteger("db::autosaveenabled",autosaveenabled);
	db.CopyAlogEntityCallback = &copyAlongCmdImplement;
    return 1;
}

void Database::EnableUndo(int enable)
{ if (enable)
    {  undofile.close();
       undofile.open(undofilename,_RCT("w+b"));
    }
  else
    undofile.close();
}

int Database::getnextviewno(void)
{int i;
  for (i = 0 ; i < MaxViews ; i++)
    if (views3d.match(i) == NULL && views2d.match(i) == NULL) break;
  if (i == MaxViews) i = -1;
  return i;
}

void drawwp(int drawmode,void *workplane,View3dSurface *window)
{ if (workplane != NULL && window->getsurfacetype() != View2dSurf)
    ((Workplane *)workplane)->draw(drawmode,window);
}

#define MAX_UNDODEPTH 200

//
//     db.saveundo(UD_REPEATCOUNT,&repeat_count);
//
void Database::undo(const char *name,UndoFile *ufile)
{
    undo(CHAR2RCCHR(name),ufile);
}

void Database::undo(const RCCHAR *name,UndoFile *ufile)
{int level,selected;
 short operation,type;
 long l,n,index,p,repeat_count,initial_repeat_count;
 Entity *e;
 View2d *view2d;
 Workplane *workplane;
 RCCHAR mess[300],*at,*oldat;
 //Vchar mess[300],*at,*oldat;
 EntityList deletelist[MAX_UNDODEPTH],fixuplist[MAX_UNDODEPTH],insertlist[MAX_UNDODEPTH],free,others;
 AttributeList al;
 ResourceString rsnc0(NC);
 ResourceString rs6(NCENT+6);
 ResourceString rs7(NCENT+7);

   if (ufile->closed() || ufile->position() == 0)
     {  sprintf(mess,rs6.gets(),name);
        cadwindow->MessageBox(mess,name,MB_ICONINFORMATION);
        return;
     }
   level = 0;
   repeat_count = 0;
   initial_repeat_count = 0;
   do {  p = ufile->position();
         if (! ufile->previous())
           {  ufile->close(); break;
           }
         if (! ((CadFile*)ufile)->read(&operation))
           {  ufile->close();  break;
           }
         switch (operation)
           {  
		    case UD_BOUNDINGBOX:
				cadwindow->getcurrentwindow()->boundingBox.undo(undoing);
				saveundo(UD_BOUNDINGBOX, 0);
				break;
			 case UD_REPEATCOUNT :
                if (! ufile->read(&n))
                  {  ufile->close();  break;
                  }
                if (level == 0)
                {   repeat_count = n;
                    initial_repeat_count = n;
                }
                break;
              case UD_STARTBLOCK :
//                if (level == 0)
                  {
                     for (deletelist[level].start() ;  (e = deletelist[level].next()) != NULL ; )
                       e->draw(DM_ERASE);

                     if (! fixuplist[level].empty())
                       {  for (fixuplist[level].start() ; (e = fixuplist[level].next()) != NULL ; )
                            if (geometry.idtoentity[long(e)] != 0)
                              {  geometry.idtoentity[long(e)]->fixupreferences(geometry.idtoentity);
                                 geometry.idtoentity[long(e)]->dbinsert(this);
                              }
                          for (fixuplist[level].start() ; (e = fixuplist[level].next()) != NULL ; )
                            if (geometry.idtoentity[long(e)] != 0)
                              {  cadwindow->invalidatedisplaylist(geometry.idtoentity[long(e)]);
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                                 geometry.idtoentity[long(e)]->draw(DM_NORMAL);
#endif
                               }
                          fixuplist[level].fixupreferences(geometry.idtoentity);
                          db.saveundo(UD_MANYMOVED,(Entity *)&fixuplist[level]);
                       }
                     fixuplist[level].destroy();

                     do {  for (deletelist[level].start() ;  (e = deletelist[level].next()) != NULL ; )
                             if (e->getalist().empty()) free.add(e); else others.add(e);
                           for (free.start(),n = 0 ; (e = free.next()) != NULL ; n++)
                             db.geometry.del(e,0);
                           deletelist[level].destroy();  free.destroy();
                           deletelist[level] = others.copy();
                           others.destroy();
                        } while (n > 0);
                     deletelist[level].destroy();

                     for (insertlist[level].start() ; (e = insertlist[level].next()) != NULL ; )
                       {  cadwindow->invalidatedisplaylist(e);
#ifdef NEW_DRAGGING
                         // kmj nothing todo when using opengl?
#else
                          e->draw(DM_NORMAL);
#endif
                       }
					 
                     insertlist[level].destroy();
                  }
                level--;
                db.saveundo(UD_ENDBLOCK,NULL);
                if (level == 0)
                    repeat_count--;
                break;
              case UD_ENDBLOCK :
                db.saveundo(UD_STARTBLOCK,NULL);
                level++;
                break;
              case UD_INSERTATT :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
                if (! ((CadFile*)ufile)->read(&at))
                  {  ufile->close();  break;
                  }
                al = geometry.idtoentity[index]->getatlist();
                for (al.start() ;  (oldat = al.next()) != NULL ; )
                  if (strcmp(at,oldat) == 0)
                    {  geometry.idtoentity[index]->delat(oldat);
                       geometry.idtoentity[index]->setattribute(at);
                       saveundo(UD_DELETEATT,geometry.idtoentity[index]);
                       delete [] at;
                       delete [] oldat;
                       break;
                    }
				
                break;
              case UD_DELETEATT :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
                if (! ((CadFile*)ufile)->read(&at))
                  {  ufile->close();  break;
                  }
				
                geometry.idtoentity[index]->addat(at);
                geometry.idtoentity[index]->setattribute(at);
                saveundo(UD_INSERTATT,geometry.idtoentity[index]);
                break;			                                       
              case UD_MANYMOVED :
                if (! fixuplist[level].load(ufile))
                  {  ufile->close();  break;
                  }

                for (fixuplist[level].start() ; (e = fixuplist[level].next()) != NULL ; )
                  {
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                    geometry.idtoentity[long(e)]->draw(DM_ERASE);
#endif
                     geometry.idtoentity[long(e)]->dbdelete();
                     cadwindow->invalidatedisplaylist(geometry.idtoentity[long(e)]);
                  }
                break;
			  case UD_MOVED:	  
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
				
                if (fixuplist[level].empty())
                  {
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                    geometry.idtoentity[index]->draw(DM_ERASE);
#endif
                     geometry.idtoentity[index]->dbdelete();
                  }
				
				if (copyAlogMap.contains(index)) {					
					copyAlogMap[index].unDo(undoing, geometry.idtoentity[index]->isa(circle_entity));
				}
                saveundo(UD_MOVED,geometry.idtoentity[index]);
                geometry.idtoentity[index]->loadgeometry(ufile);
                geometry.idtoentity[index]->loadheaderv(ufile);
                cadwindow->invalidatedisplaylist(geometry.idtoentity[index]);
                if (fixuplist[level].empty())
                  {  geometry.idtoentity[index]->fixupreferences(geometry.idtoentity);
                     geometry.idtoentity[index]->dbinsert(this);
                     geometry.idtoentity[index]->draw(DM_NORMAL);
                  }
                break;
              case UD_CHANGEGROUP :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
                selected = geometry.idtoentity[index]->getselected();
                geometry.idtoentity[index]->setselected(0);
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_NORMAL);
#endif
                geometry.idtoentity[index]->setselected(selected);
                saveundo(UD_CHANGEGROUP,geometry.idtoentity[index]);
                geometry.idtoentity[index]->dbdelete();
                geometry.idtoentity[index]->loadgeometry(ufile);
                geometry.idtoentity[index]->fixupreferences(geometry.idtoentity);
                geometry.idtoentity[index]->dbinsert(this);
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_NORMAL);
#endif
                break;
              case UD_CHANGEHEADER :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_ERASE);
#endif
                saveundo(UD_CHANGEHEADER,geometry.idtoentity[index]);
                geometry.idtoentity[index]->loadheaderu(ufile);
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_NORMAL);
#endif
                break;
              case UD_CHANGEATTRIBUTES :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_ERASE);
#endif
                cadwindow->invalidatedisplaylist(geometry.idtoentity[index]);
                saveundo(UD_CHANGEATTRIBUTES,geometry.idtoentity[index]);
                geometry.idtoentity[index]->loadattributes(ufile);
#ifdef NEW_DRAGGING
                // KMJ : nothing to do when using opengl?
#else
                geometry.idtoentity[index]->draw(DM_NORMAL);
#endif
                break;
              case UD_INSERT :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
                if (geometry.idtoentity == NULL)
                  {  ufile->close();  break;
                  }
                state.removefromselection(geometry.idtoentity[index]);
                if (geometry.idtoentity[index] != 0)
                  if (geometry.idtoentity[index]->getalist().empty())
                    {  if (geometry.idtoentity[index]->isa(group_entity))
                         geometry.del(geometry.idtoentity[index],0);
                       else
                         geometry.del(geometry.idtoentity[index]);
                    }
                  else
                    deletelist[level].add(geometry.idtoentity[index]);
                break;
              case UD_DELETE :
                if (! ufile->read(&index))
                  {  ufile->close();  break;
                  }
                if (! ((CadFile*)ufile)->read(&type))
                  {  ufile->close();  break;
                  }
                if (classfunctions.entityundo(type) == NULL)
                  {  sprintf(mess,rs7.gets(),type);
                     cadwindow->MessageBox(mess,rsnc0.gets(),MB_ICONSTOP);
                     ufile->close();
                     break;
                  }
                if ((e = classfunctions.entityundo(type)(ufile)) != NULL)
                  {
#ifdef USING_WIDGETS
                     e->qgi = 0;
                     //e->qgiLines = QVector<QGraphicsItem*>(1,0);
                     //e->qgi3dVitems = QVector<QGraphicsItem*>(1,0);
#endif
                     e->setselected(0);
                     e->fixupreferences(geometry.idtoentity);
                     if (level > 0)
                       {  geometry.add(e,2);
                          insertlist[level].add(e);
                       }
                     else
                       geometry.add(e);
                  }
                else
                  {  ufile->close();  break;
                  }
                break;
              case UD_CHANGEVIEW2D :
                if (! ufile->read(&l))
                  {  ufile->close();  return;
                  }
                view2d = (View2d *) l;
                if (views2d.match(view2d) != NULL)
                  {  saveundo(UD_CHANGEVIEW2D,view2d);
                     view2d->load(ufile);
                     cadwindow->updatewindows(view2d,1);
                  }
                else
                  ufile->position(p-sizeof(l));
                break;
              case UD_CHANGEWORKPLANE :
                if (! ufile->read(&l))
                  {  ufile->close();  return;
                  }
                workplane = (Workplane *) l;
                if (workplanes.match(workplane) != NULL)
                  {  saveundo(UD_CHANGEWORKPLANE,workplane);
                     cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawwp);
                     workplane->load(ufile);
                     cadwindow->drawcallback(DM_NORMAL,db.workplanes.getcurrent(),drawwp);
                  }
                else
                  ufile->position(p-sizeof(l));
                break;
           }
         if (ufile->closed()) break;
         if (! ufile->read(&l))
           {  ufile->close();  break;
           }
         if (! ufile->previous())
           {  ufile->close();  break;
           }
      }  while (repeat_count > 0 || level > 0);


    if (initial_repeat_count > 0)
        saveundo(UD_REPEATCOUNT,&initial_repeat_count);

    if (UndoCallback != 0)
        UndoCallback(0);

#ifdef NEW_DRAGGING
    // always update all views when using opengl
    cadwindow->paintall();
#else
    if (GetPrivateProfileInt("Configuration","UNDOPaintall",0,home.getinifilename()) != 0)
        cadwindow->paintall();
#endif

}

void Database::undo(void)
{ResourceString rs8(NCENT+8);
    undoing = 1;
    undo(rs8.gets(),&undofile);
    undoing = 0;
}

void Database::redo(void)
{ResourceString rs9(NCENT+9);
    undoing = 2;
    undo(rs9.gets(),&redofile);
    undoing = 0;
}

void Database::saveundo(int operation,void *d)
{int entityop;
 short s;
 long l,pos;
 UndoFile *ufile;
 int status;
 Entity *e = nullptr;

    entityop =  operation == UD_INSERT || operation == UD_DELETE ||
                operation == UD_CHANGEGROUP || operation == UD_CHANGEHEADER ||
                operation == UD_CHANGEATTRIBUTES || operation == UD_MOVED ||
                operation == UD_INSERTATT || operation == UD_DELETEATT;
    if (entityop)
    {   e = (Entity *) d;
        if (e != NULL && e->getposition() == NULL)
            return;
        //  Invalidate the triangles associated with this entity
        if (dynamic_cast<SurfaceEntity*>(e) != 0)
            ((SurfaceEntity *)e)->cleartriangles();
    }


    if (operation == UD_DELETE && DeleteCallback != 0)
        DeleteCallback((Entity *)d);
    if (undoing == 0 && operation == UD_DELETE && UserDeleteCallback != 0)
        UserDeleteCallback((Entity *)d);
    if (operation == UD_INSERT && InsertCallback != 0)
        InsertCallback((Entity *)d);

    db.setchanged();
    if (undoing == 1)
    {   ufile = &redofile;
    }
    else
    {   if (undoing == 0)
            redofile.clear();
        ufile = &undofile;
    }
    if (ufile->closed())
        return;
    pos = ufile->position();
    s = short(operation);
    if (! ((CadFile*)ufile)->write(&s))
    {   ufile->close();
        return;
    }

    if (entityop)
    {long v = e->getid();
        if (! ufile->write(&v))
        {   ufile->close();
            return;
        }
    }

    switch (operation)
    {case UD_REPEATCOUNT : status = ufile->write((long *)d);  break;
     case UD_DELETE :
#ifdef USING_WIDGETS
    {
        //QVector<QGraphicsItem*> qtmp1 = e->qgi3dVitems;
        //e->qgi3dVitems.clear();
        //QVector<QGraphicsItem*> qtmp2 = e->qgiLines;
        //e->qgiLines.clear();
        QGraphicsItem *qtmp = e->qgi;
        e->qgi=0;
        status = e->save(ufile);
        e->qgi=qtmp;
        //e->qgi3dVitems = qtmp1;
        //e->qgiLines = qtmp2;
    }
#else
        status = e->save(ufile);
#endif
        break;
	
     case UD_CHANGEHEADER : status = e->saveheaderu(ufile);  break;
     case UD_CHANGEATTRIBUTES : status = e->saveattributes(ufile);  break;	 
     case UD_MOVED : e->savegeometry(ufile);  status = e->saveheaderv(ufile);  break;
     case UD_CHANGEGROUP : status = e->savegeometry(ufile); break;
     case UD_INSERT : status = 1;  break;
     case UD_DELETEATT:
     case UD_INSERTATT : status = ((CadFile*)ufile)->write(e->getattribute());  break;
     case UD_MANYMOVED : status = ((EntityList *) d)->save(ufile);  break;
     case UD_CHANGEVIEW2D :
        l = (long) d;
        if ((status = ufile->write(&l)) != 0)
            status = ((View2d *) d)->save(ufile);
        break;
     case UD_CHANGEWORKPLANE :
        l = (long) d;
        if ((status = ufile->write(&l)) != 0)
            status = ((Workplane *) d)->save(ufile);
        break;
     default :
        status = 1;
        break;
    }
    if (! status)
    {   ufile->close();
        return;
    }
    l = -(ufile->position() + 4 - pos);
    if (! ufile->write(&l))
    {   ufile->close();
        return;
    }
}

void Database::savezoomundo(int operation,Window *w)
{View3dWindow *view3dwindow;
 short s;
 long l,pos;
 RCLONGLONG lp;
  if (w == NULL) return;
  db.setchanged();
  if (viewundofile.closed()) return;
  pos = viewundofile.position();
  s = short(operation);
  if (! viewundofile.write(&s))
    {  viewundofile.close();  return;
    }
  lp = (RCLONGLONG)w;
  if (! viewundofile.write(&lp))
    {  viewundofile.close();  return;
    }
  if (operation == 0)
    {  view3dwindow = (View3dWindow *) w;
       if (! view3dwindow->save(&viewundofile,0))
         {  viewundofile.close();  return;
         }
    }
  else if (operation == 1)
    {  view3dwindow = (View3dWindow *) w;
       if (! view3dwindow->save(&viewundofile,1))
         {  viewundofile.close();  return;
         }
    }
  else if (operation == 2)
    {  view3dwindow = (View3dWindow *) w;
       if (! view3dwindow->save(&viewundofile,0))
         {  viewundofile.close();  return;
         }
       view3dwindow = (View3dWindow *) w;
       if (! view3dwindow->save(&viewundofile,1))
         {  viewundofile.close();  return;
         }
    }

  l = -(viewundofile.position() + 4 - pos);
  if (! viewundofile.write(&l))
    {  viewundofile.close();  return;
    }
}

void Database::undoview()
{short operation;
 View3dWindow *w;
 long l,pos;
 int i;
 RCLONGLONG lp;
 ResourceString rs10(NCENT+10);
 ResourceString rs11(NCENT+11);
 int finished;
  for (finished = 0 ; ! finished ; )
    {  if (viewundofile.closed() || viewundofile.position() == 0)
         {  cadwindow->MessageBox(rs10.gets(),rs11.gets(),MB_ICONINFORMATION);
            return;
         }
       if (! viewundofile.previous())
         {  viewundofile.close();  return;
         }
       pos = viewundofile.position();
       if (! viewundofile.read(&operation))
         {  viewundofile.close();  return;
         }
       if (! viewundofile.read(&lp))
         {  viewundofile.close();  return;
         }
       if (operation == 0 || operation == 1 || operation == 2)
         {
           w = (View3dWindow *) lp;

           if (cadwindow->issubwindow(w))
              {  for (i = 0 ; i < (operation == 2) + 1 ; i++)
                   if (! w->load(&viewundofile))
                     {  viewundofile.close();  return;
                     }
                 finished = 1;
              }
         }
       viewundofile.position(pos);
    }
}

int Database::loadColourTable(RCCHAR *inifilename)
{
    int i;
    RCCHAR key[300],data[600];

     for (i = 0 ; i < 256 ; i++)
     {
          sprintf(key,"cl::%d",i);
          GetPrivateProfileString(_RCT("ColourPalette"),key,_RCT("0"),data,600,inifilename);
          if (strlen(data) > 0)
          {
			  //black color value is zero and some styelset values can be zero
              if(atoi(data) >= 0)
              {
                 RCCOLORREF cref = (RCCOLORREF)atoi(data);
                 colourtable[i].red   = GetRValue(cref);
                 colourtable[i].green = GetGValue(cref);
                 colourtable[i].blue  = GetBValue(cref);
              }
          }
     }

     // debug
     qDebug() << "debug : loadColourTable()";
     for (i = 0 ; i <= 255 ; i++)
     {
         RCCOLORREF c; colourtable[i].get(&c);
         //qDebug() << "c::" << i << "=" << c;
     }
     //

     return 1;
}

int Database::saveColourTable(RCCHAR *inifilename)
{
    int i;
    RCCHAR key[300],data[600];

     for (i = 0 ; i < 256 ; i++)
     {
          RCCOLORREF cref = RGB(colourtable[i].red,colourtable[i].green,colourtable[i].blue);
          sprintf(key,"cl::%d",i);
          sprintf(data,"%d",cref);
          WritePrivateProfileString(_RCT("ColourPalette"),key,data,inifilename);
     }

     return 1;
}

// dumps some debug info to the console
void Database::debug()
{
    qDebug() << "Database Debug Output";
    qDebug() << "Title : " <<  QString(title);
    qDebug() << "Size  : " <<  geometry.list.length();
    Entity *e;
    int cnt=0;
    for (geometry.start() ; (e = geometry.next()) != NULL ; )
    {
        cnt++;
        qDebug() << "Entity (" <<  cnt << ") information";
        switch(e->type())
        {
        case point_entity:
            qDebug() << "Entity Type : Point : " << e->type();
            qDebug() << "ID          : " << ((PointE*)e)->getid();
            qDebug() << "Index       : " << ((PointE*)e)->getindex();
            qDebug() << "Layer       : " << ((PointE*)e)->getlayer();
            qDebug() << "Position    :";
            qDebug() << " x : " << ((PointE*)e)->getp().x
                     << " y : " << ((PointE*)e)->getp().y
                     << " z : " << ((PointE*)e)->getp().z;
            break;
        case line_entity:
            qDebug() << "Entity Type : Line : " << e->type();
            qDebug() << "ID          : " << ((Line*)e)->getid();
            qDebug() << "Index       : " << ((Line*)e)->getindex();
            qDebug() << "Layer       : " << ((Line*)e)->getlayer();
            qDebug() << "Start Point :";
            qDebug() << " x : " << ((Line*)e)->start().x
                     << " y : " << ((Line*)e)->start().y
                     << " z : " << ((Line*)e)->start().z;
            qDebug() << "End Point :";
            qDebug() << " x : " << ((Line*)e)->end().x
                     << " y : " << ((Line*)e)->end().y
                     << " z : " << ((Line*)e)->end().z;
            break;
        case circle_entity:
            qDebug() << "Entity Type : Circle : " << e->type();
            qDebug() << "ID          : " << ((Circle*)e)->getid();
            qDebug() << "Index       : " << ((Circle*)e)->getindex();
            qDebug() << "Layer       : " << ((Circle*)e)->getlayer();
            qDebug() << "Origin Point :";
            qDebug() << " x : " << ((Circle*)e)->getorigin().x
                     << " y : " << ((Circle*)e)->getorigin().y
                     << " z : " << ((Circle*)e)->getorigin().z;
            qDebug() << "Start Angle : " << ((Circle*)e)->getstarta();
            qDebug() << "Sweep Angle : " << ((Circle*)e)->getsweepa();
            break;
        case ellipse_entity:
            qDebug() << "Entity Type : Ellipse : " << e->type();
            qDebug() << "ID          : " << ((EllipseE*)e)->getid();
            qDebug() << "Index       : " << ((EllipseE*)e)->getindex();
            qDebug() << "Layer       : " << ((EllipseE*)e)->getlayer();
            qDebug() << "Origin Point :";
            qDebug() << " x : " << ((EllipseE*)e)->getorigin().x
                     << " y : " << ((EllipseE*)e)->getorigin().y
                     << " z : " << ((EllipseE*)e)->getorigin().z;
            qDebug() << "X Axis :";
            qDebug() << " x : " << ((EllipseE*)e)->getxaxis().x
                     << " y : " << ((EllipseE*)e)->getxaxis().y
                     << " z : " << ((EllipseE*)e)->getxaxis().z;
            qDebug() << "Y Axis :";
            qDebug() << " x : " << ((EllipseE*)e)->getyaxis().x
                     << " y : " << ((EllipseE*)e)->getyaxis().y
                     << " z : " << ((EllipseE*)e)->getyaxis().z;
            qDebug() << "Start Angle : " << ((EllipseE*)e)->getstarta();
            qDebug() << "Sweep Angle : " << ((EllipseE*)e)->getsweepa();
            qDebug() << "Start Angle : " << ((EllipseE*)e)->getmajor();
            qDebug() << "Sweep Angle : " << ((EllipseE*)e)->getminor();
            break;
        case curve_entity:
            qDebug() << "Entity Type : Curve : " << e->type();
            break;
        case text_entity :
            qDebug() << "Entity Type : Text : " << e->type();
            qDebug() << "Font        : " << QString(((Text*)e)->getfont());
            qDebug() << "Text        : " << QString(((Text*)e)->gettext());
            qDebug() << "ID          : " << ((Text*)e)->getid();
            qDebug() << "Index       : " << ((Text*)e)->getindex();
            qDebug() << "Label       : " << ((Text*)e)->getlabel();
            qDebug() << "Layer       : " << ((Text*)e)->getlayer();
            qDebug() << "Width       : " << ((Text*)e)->getwidth();
            qDebug() << "Height      : " << ((Text*)e)->getheight();
            qDebug() << "Origin      :";
            qDebug() << " x : " << ((Text*)e)->getorigin().x
                     << " y : " << ((Text*)e)->getorigin().y
                     << " z : " << ((Text*)e)->getorigin().z;
            qDebug() << "X axis      :";
            qDebug() << " x : " << ((Text*)e)->getxaxis().x
                     << " y : " << ((Text*)e)->getxaxis().y
                     << " z : " << ((Text*)e)->getxaxis().z;
            qDebug() << "Y axis      :";
            qDebug() << " x : " << ((Text*)e)->getyaxis().x
                     << " y : " << ((Text*)e)->getyaxis().y
                     << " z : " << ((Text*)e)->getyaxis().z;
            qDebug() << "Angle : " << ((Text*)e)->getangle();

            break;
        default:
            qDebug() << "Unknown entity type" << e->type();
        }
    }
}

int Colour::save(CadFile *outfile) const
{ if (!outfile->write(&red)) return 0;
  if (!outfile->write(&green)) return 0;
  if (!outfile->write(&blue)) return 0;
  return 1;
}

int Colour::load(CadFile *infile)
{ if (!infile->read(&red)) return 0;
  if (!infile->read(&green)) return 0;
  if (!infile->read(&blue)) return 0;
  return 1;
}

int Colour::load(RCCHAR *)
{
    return 0;
}

int Colour::save(RCCHAR *)
{
    return 0;
}

double Colour::value(double n1,double n2,double hue)
{double v;
  while (hue > 360.0) hue -= 360.0;
  while (hue < 0.0) hue = hue + 360.0;
  if (hue < 60.0) v = n1 + (n2 - n1) * hue / 60.0;
  else if (hue < 180.0) v = n2;
  else if (hue < 240.0) v = n1 + (n2 - n1) * (240.0 - hue) / 60.0;
  else v = n1;
  return v;
}

void Colour::sethls(double h,double l,double s)
{double m1,m2;
  if (l <= 0.5)
    m2 = l * (1.0 + s);
  else
    m2 = l + s - l * s;
  m1 = 2.0 * l - m2;
  if (s == 0.0)
    red = green = blue = 255;
  else
    {  red = int(255.5 * value(m1,m2,h + 120.0));
       green = int(255.5 * value(m1,m2,h));
       blue = int (255.5 * value(m1,m2,h - 120.0));
    }
}

void Colour::gethls(double *h,double *l,double *s)
{double max,min,diff,r_dist,g_dist,b_dist;

  max = red;
  if (red < green)
    max = green;
  if (max < blue)
    max = blue;

  min = red;
  if (red > green)
    min = green;
  if (min > blue)
    min = blue;

  diff = max - min;
  *l = (max + min) / 2 / 255.0;
  if (fabs(diff) < 0.00001)
    {  *s = 1.0;
       *h = 0;
    }
  else
    {  if (*l <= 0.5)
         *s = diff / (max + min);
       else
         *s = diff / (2*255 - max - min);

       r_dist = (max - red) / diff;
       g_dist = (max - green) / diff;
       b_dist = (max - blue) / diff;

       if (red == max)
         *h = b_dist - g_dist;
       else if (green == max)
         *h = 2 + r_dist - b_dist;
       else
         *h = 4 + g_dist - r_dist;

       (*h) *= 60;
       if (*h < 0)
         (*h) += 360.0;

    }

}

LineWeights::LineWeights()
{
  weightmm = 0;
  weightpx = 0;
  weightpx12 = 0;
  initialize();
}

void LineWeights::initialize()
{ nweights = 12;
  delete [] weightmm;
  weightmm = new double[nweights];
  //                   old weights
  weightmm[ 0] = 0.1;  // [0]  = 0.00
  weightmm[ 1] = 0.1;  // [1]  = 0.10
  weightmm[ 2] = 0.18; // [2]  = 0.18
  weightmm[ 3] = 0.25; // [3]  = 0.35
  weightmm[ 4] = 0.35; // [4]  = 0.5
  weightmm[ 5] = 0.5;  // [5]  = 0.7
  weightmm[ 6] = 0.7;  // [6]  = 1.0
  weightmm[ 7] = 1.0;  // [7]  = 1.5
  weightmm[ 8] = 1.5;  // [8]  = 2.0
  weightmm[ 9] = 2.0;  // [9]  = 2.5
  weightmm[10] = 2.5;  // [10] = 3.0
  weightmm[11] = 3.0;  // [11] = 0.25
#ifdef USING_WIDGETS
  initweightpx();
  //                   old weights
  oldweights[0]  = 0;  // [0]  = 0.00
  oldweights[1]  = 1;  // [1]  = 0.10
  oldweights[2]  = 2;  // [2]  = 0.18
  oldweights[3]  = 4;  // [3]  = 0.35
  oldweights[4]  = 5;  // [4]  = 0.5
  oldweights[5]  = 6;  // [5]  = 0.7
  oldweights[6]  = 7;  // [6]  = 1.0
  oldweights[7]  = 8;  // [7]  = 1.5
  oldweights[8]  = 9;  // [8]  = 2.0
  oldweights[9]  = 10; // [9]  = 2.5
  oldweights[10] = 11; // [10] = 3.0
  oldweights[11] = 3;  // [11] = 0.25
#endif
}

void LineWeights::clear()
{
    delete [] weightmm; weightmm = 0;
    delete [] weightpx; weightpx = 0;
    delete [] weightpx12; weightpx12 = 0;
}

#ifdef USING_WIDGETS
void LineWeights::initweightpx()
{
    delete [] weightpx;
    weightpx = new int[nweights];
    weightpx[ 0] = 1;
    weightpx[ 1] = 1;
    weightpx[ 2] = 1;
    weightpx[ 3] = 2;
    weightpx[ 4] = 3;
    weightpx[ 5] = 4;
    weightpx[ 6] = 5;
    weightpx[ 7] = 6;
    weightpx[ 8] = 7;
    weightpx[ 9] = 8;
    weightpx[10] = 9;
    weightpx[11] = 10;

    // 1200 dpi
    delete [] weightpx12;
    weightpx12 = new int[nweights];
    weightpx12[0]  = 2;  // = 0.00
    weightpx12[1]  = 2;  // = 0.10
    weightpx12[2]  = 3;  // = 0.18
    weightpx12[3]  = 4;  // = 0.25
    weightpx12[4]  = 5;  // = 0.35
    weightpx12[5]  = 6;  // = 0.5
    weightpx12[6]  = 7;  // = 0.7
    weightpx12[7]  = 8;  // = 1.0
    weightpx12[8]  = 9;  // = 1.5
    weightpx12[9]  = 10;  // = 2.0
    weightpx12[10] = 11; // = 2.5
    weightpx12[11] = 13; // = 3.0

}

int LineWeights::weighttopx(int w)
{
    if(w < 0 )
        return weightpx[1];
    else if(w > 11)
        return weightpx[11];
    else
        return weightpx[w];
}

int LineWeights::weighttopxprint(int w, int dpi)
{
    if(w < 0 )
        w = 1;
    else if(w > 11)
        w = 11;

    switch(dpi)
    {
    default:
    case 3:
    case 6:
    case 12:
    case 24:
        return weightpx12[w];
    }
}

// converting pre file version 2 weights
// to new weights
int LineWeights::weightfromold(int w)
{
    int nw;
    if(w < 0)
        nw = oldweights[0];
    else if(w > 11)
        nw = oldweights[10];
    else
        nw = oldweights[w];
    //qDebug() << "old w : " << w << " new w : " << nw << "new mm : " << weighttomm(nw);
    return nw;
}

#endif

LineWeights::~LineWeights()
{
  delete [] weightmm;
  delete [] weightpx;
  delete [] weightpx12;
}

int LineWeights::mmtoweight(double weight)
{int i,imin;
 double d,dmin;
  if (fabs(weight - db.getminimumlineweight()) < 0.001)
    return 1;
  imin = 0;  dmin = fabs(weight - weightmm[0]);
  for (i = 1 ; i < nweights ; i++)
    if ((d = fabs(weight - weightmm[i])) < dmin)
      {  dmin = d;  imin = i;
      }
  return imin;
}

double LineWeights::weighttomm(int weight)
{double w;

  if (weight <= 0)
    w = weightmm[1];
  else if (weight >= nweights)
    w = weightmm[nweights-1];
  else
    w = weightmm[weight];

  if (w < db.getminimumlineweight())
    w = db.getminimumlineweight();

  return w;

}

int LineWeights::weighttohdc(RCHDC hdc,int weight)
{int vs;
  if ((vs = GetDeviceCaps(hdc,VERTSIZE)) == 0)
    return int (4.0 * weighttomm(weight) + 0.5);
  else
    return int(double(GetDeviceCaps(hdc,VERTRES)) / double(vs) * weighttomm(weight) + 0.5);
}

int LineWeights::save(CadFile *outfile) const
{int i;
  if (! outfile->write(&nweights)) return 0;
  for (i = 0 ; i < nweights ; i++)
    if (! outfile->write(&weightmm[i])) return 0;
  return 1;
}

int LineWeights::load(CadFile *infile)
{int i;
 short n;
 double *a;
  if (! infile->read(&n)) return 0;
  if(infile->getversion() == 2)
  {
      // not using old lineweights
      // just read the stuff and ignore it
      if ((a = new double[n]) == NULL) return 0;
      for (i = 0 ; i < n ; i++)
          if (! infile->read(&a[i])) return 0;
      delete [] a;
  }
  else
  {
      if (n > nweights)
      {
           if ((a = new double[n]) == NULL) return 0;
           delete [] weightmm;
           weightmm = a;
      }
      nweights = n;
      for (i = 0 ; i < nweights ; i++)
        if (! infile->read(&weightmm[i])) return 0;
  }
  return 1;
}

bool LineWeights::isEqual(double _lhs, double _rhs)
{
    if(fabs(_lhs - _rhs) < 0.001)
        return true;
    else
        return false;
}

bool LineWeights::_isEqual(int _lhs, int _rhs)
{
    return isEqual(weighttomm(_lhs),weighttomm(_rhs));
}

bool LineWeights::isEqual(int _lhs, int _rhs)
{
    LineWeights lw;
    //return lw._isEqual(_lhs,_rhs);
    return lw.isEqual(lw.weighttomm(_lhs),lw.weighttomm(_rhs));
}

LineStyle::LineStyle()
{
    lengths.empty();
    on=0;
    pos=0;
    endsegment=0;
    scale=0;
    length=0;
}

LineStyle::LineStyle(LineStyle &other)
{
    double *x=0;
    int segment;
    double dash;
    double space;
    lengths.destroy();
    for (other.lengths.start() ; (x = (double *)other.lengths.next()) != NULL ; )
        add(*x);
/*
    for(int i=0;  i < other.getnsegments(); i++)
    {
        other.getsegment(i,&dash,&space);
        this->setsegment(i,dash,space);
    }
    */
    on = other.on;
    pos=0;
    endsegment=0;
    scale=0;
    length=0;
}

LineStyle::~LineStyle()
{double *x;
  for (lengths.start() ; (x = (double *)lengths.next()) != NULL ; )
    delete x;
  lengths.destroy();
}

int LineStyle::savedxf(int index,DXFOut *dxfout) const
{char name[256];
 double *x,len,s;
 GeneralList l;
 int i,n;

  n = lengths.length();
  if (n > 10)
    n = 10;
  sprintf(name,"LINESTYLE%d",index);
  if (! dxfout->writestring(0, (char*)"LTYPE")) return 0;
  if (! dxfout->writelong(5,1004+index)) return 0;
  if (! dxfout->writelong(330,1000)) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbSymbolTableRecord")) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbLinetypeTableRecord")) return 0;
  if (! dxfout->writestring(2,name)) return 0;
  if (! dxfout->writelong(70,64L)) return 0;
  if (! dxfout->writestring(3,name)) return 0;
  if (! dxfout->writelong(72,65L)) return 0;
  if (! dxfout->writelong(73,n)) return 0;
  l = lengths;
  for (l.start(),len = 0.0 ; (x = (double *)l.next()) != 0 ; )
    len += *x;
  if (! dxfout->writereal(40,len * dxfout->getscale())) return 0;
  for (l.start(),s = 1.0,i = 0 ; (x = (double *)l.next()) != 0 && i < n ; i++)
    {  if (! dxfout->writereal(49,*x * s * dxfout->getscale())) return 0;
       if (! dxfout->writelong(74,0)) return 0;
       s = -s;
    }
  return 1;
}

int LineStyle::save(CadFile *outfile) const
{short n;
 double *x;
 GeneralList l;
  n = short(lengths.length());
  if (! outfile->write(&n)) return 0;
  l = lengths;
  for (l.start() ; (x = (double *)l.next()) != NULL ; )
    if (! outfile->write(x)) return 0;
  return 1;
}

int LineStyle::load(CadFile *infile)
{short i,n;
 double x;
  lengths.destroy();
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! infile->read(&x)) return 0;
       add(x);
    }
  lengths.reverse();
  return 1;
}

int LineStyle::save(RCCHAR *data) const
{GeneralList l;
 double *x;
  sprintf(data,"%d,",lengths.length());
  l = lengths;
  for (l.start() ; (x = (double *)l.next()) != NULL ; )
    sprintf(data+strlen(data),"%.6lf,",*x);
  return 1;
}

int LineStyle::load(RCCHAR *data)
{int i,n;
 RCCHAR *s;
  lengths.destroy();
  s = strtok(data,_RCT(","));
  if (s != 0)
    {  n = atoi(data);
       for (i = 0 ; s != 0 && i < n ; i++)
         {  s = strtok(0,_RCT(","));
            if (s != 0)
              add(atof(s));
         }
       lengths.reverse();
    }
  return 1;
}

LineStyle *LineStyle::copy(void)
{LineStyle *newstyle;
 double *x;
  if ((newstyle = new LineStyle) == NULL) return NULL;
  for (lengths.start() ; (x = (double *)lengths.next()) != NULL ; )
    newstyle->add(*x);
  newstyle->lengths.reverse();
  return newstyle;
}

void LineStyle::setnsegments(int n)
{int i;
 double *x;
  if (n == getnsegments()) return;
  lengths.reverse();
  i = (n - getnsegments()) * 2;
  while (i > 0)
    {  add(0.0);  i--;
    }
  while (i < 0)
    {  lengths.start();
       x = (double *) lengths.next();
       lengths.del(x);
       delete x;
       i++;
    }
  lengths.reverse();
}

int LineStyle::getnsegments(void)
{ return lengths.length() / 2;
}

void LineStyle::getsegment(int segment,double *dash,double *space)
{int i;
 double *x;
  for (lengths.start(),i = 0 ; i <= (segment-1) * 2 && (x = (double *) lengths.next()) != NULL ;  i++);
  if (x != NULL)
    {  *dash = *x;
       if ((x = (double *) lengths.next()) != NULL)
         *space = *x;
       else
         *space = 0.0;
    }
  else
    *dash = *space = 0.0;
}

void LineStyle::setsegment(int segment,double dash,double space)
{int i;
 double *x;
  if (segment > getnsegments()) setnsegments(segment);
  for (lengths.start(),i = 0 ; i <= (segment-1) * 2 && (x = (double *) lengths.next()) != NULL ;  i++);
  if (x != NULL)
    {  *x = dash;
       if ((x = (double *) lengths.next()) != NULL)
         *x = space;
    }
}

void LineStyle::add(double x)
{  lengths.add(new double(x));
}

#if 0
// found in old v7 code
// version 1 has problems with some combinations
// of scale and plotscale
void LineStyle::start(double s,double l)
{   lengths.start();
    if (lengths.atend())
        endsegment = 1.0E38;
    else
        endsegment = *(double *)lengths.next();// r0
        //endsegment = *(double *)lengths.next() * s;// r1
    scale = s;
    pos = 0.0;
    on = 1;
    if (l == 0.0)
        length = 1.0e38;
    else if (l > 0.0)
       length = l * scale - endsegment;// r0
       //length = l - endsegment; // r1
}
#else
// found in old v5 code
// works better in most cases for printing
// and rubberband graphics
void LineStyle::start(double s,double l)
{
    lengths.start();
    scale = s;
    pos = 0.0;
    on = 1;
    if (lengths.atend())
        endsegment = 1.0E38;
    else
        endsegment = *(double *)lengths.next();// r0
        //endsegment = *(double *)lengths.next() * s;// r1
}
#endif

// KMJ: modified to start dashed lines in 3d with OpenGl
void LineStyle::startGL(double s,double l)
{   lengths.start();
    if (lengths.atend())
        endsegment = 1.0E38;
    else
        endsegment = *(double *)lengths.next() * s;
    scale = s;
    pos = 0.0;
    on = 1;
    if (l == 0.0)
        length = 1.0e38;
    else if (l > 0.0)
        //length = l - endsegment;
        //length = l * 1.0 / scale - endsegment;
        length = l - endsegment;

}

// old rc v7 version see comments in overload above
#if 0
void LineStyle::draw(Surface *s,double x1,double y1,double x2,double y2)
{
  double t,l1,l2,dx,dy,tlen;
  double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm();
  dy = (y2 - y1) / s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;// r0
  //l1 = l2 = sqrt(dx*dx + dy*dy); // r1
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
    {  s->line(x1,y1,x2,y2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {  if (endsegment - pos < l1)
         {  l1 -= endsegment - pos;
            length -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0)
              s->line(x3,y3,x4,y4,1);
            if (lengths.atend())
              {  start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {  s->line(x1,y1,x2,y2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {  pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();// r0
                 //endsegment = *(double *)lengths.next() * scale;// r1
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
              }
            x3 = x4;  y3 = y4;
         }
       else
         {  if (on || length < 0.0)
              s->line(x3,y3,x2,y2,1);
            pos += l1;
            length -= l1;
            break;
         }
    }
}
#else
void LineStyle::draw(Surface *s,double x1,double y1,double x2,double y2)
{
  double t,l1,l2,dx,dy,tlen;
  double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm();
  dy = (y2 - y1) / s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;// r0
  //l1 = l2 = sqrt(dx*dx + dy*dy); // r1
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
    {  s->line(x1,y1,x2,y2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {  if (endsegment - pos < l1)
         {
            l1 -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;

            if (on)
              s->line(x3,y3,x4,y4,1);

            if (lengths.atend())
              {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {  s->line(x1,y1,x2,y2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {
                 pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();// r0
                 //endsegment = *(double *)lengths.next() * scale;// r1
                 tlen += endsegment;
                 if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
                    s->line(x4,y4,x2,y2,1);
              }
            x3 = x4;  y3 = y4;
         }
       else
         {  if (on)
              s->line(x3,y3,x2,y2,1);
            pos += l1;
            break;
         }
    }
}
#endif

double roundDbl(double number, double fixedBase) {
    if (fixedBase != 0 && number != 0) {
        double sign = number > 0 ? 1 : -1;
        number *= sign;
        number /= fixedBase;
        int fixedPoint = (int) ceil(number);
        number = fixedPoint * fixedBase;
        number *= sign;
    }
    return number;
}

int roundingInt(int number, int fixedBase) {
    if (fixedBase != 0 && number != 0) {
        int sign = number > 0 ? 1 : -1;
        int baseSign = fixedBase > 0 ? 1 : 0;
        number *= sign;
        int fixedPoint = (number + baseSign * (fixedBase - 1)) / fixedBase;
        number = fixedPoint * fixedBase;
        number *= sign;
    }
    return number;
}

#ifdef USING_WIDGETS
// KMJ: modified to draw dashed lines in 3d with OpenGl
void LineStyle::drawGL(Surface *s,double x1,double y1,double z1,double x2,double y2,double   z2)
{
  double t,l1,l2,dx,dy,dz,tlen;
  double x3,y3,z3,x4,y4,z4;

  dx = (x2 - x1);
  dy = (y2 - y1);
  dz = (z2 - z1);
  l1 = l2 = sqrt(dx*dx + dy*dy + dz*dz);
  if (l2 < 1.0E-6)
  {
      //qDebug() << "ret 1 : l2 < 1.0E-6 : " << l2;
      return;       // Screen distance tolerance
  }
  x3 = x1;  y3 = y1; z3 = z1; tlen = 0.0;

  //qDebug() << "endsegment : " << endsegment;
  //qDebug() << "scale : " << scale;
  //qDebug() << "s->getxdotspermm() : " << s->getxdotspermm();
  //qDebug() << "s->getscale() : " << s->getscale();
  //qDebug() << "endsegment * scale / s->getxdotspermm() : " << endsegment * scale / s->getxdotspermm();
  //qDebug() << "endsegment * scale / s->getscale() * s->getxdotspermm() : " << endsegment * scale / s->getscale() * s->getxdotspermm();
  //qDebug() << "endsegment / s->getscale() * s->getxdotspermm() : " << endsegment / s->getscale() * s->getxdotspermm();
  //qDebug() << "s->getpixelmodelmm() : " << s->getpixelmodelmm();
  //qDebug() << "endsegment / s->getpixelmodelmm() : " << endsegment / s->getpixelmodelmm();

  //if (endsegment  > 0.0 && endsegment < (1.0 / s->getxdotspermm()))
  if (endsegment  > 0.0 && endsegment < 0.2)
  //if (endsegment  > 0.0 && endsegment / s->getscale() * s->getxdotspermm() < 0.3)
  //if (endsegment  > 0.0 && double(endsegment) / s->getscale() * s->getxdotspermm() < 0.3)
  //if (endsegment  > 0.0 && endsegment / s->getpixelmodelmm() < 0.2)
    {
       s->lineGL(x1,y1,z1,x2,y2,z2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {
         if (endsegment - pos < l1)
         {
            l1 -= endsegment - pos;
            length -= endsegment - pos;
            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            z4 = (z2 - z1) * t + z1;
            if (on || length < 0.0)
            {
              s->lineGL(x3,y3,z3,x4,y4,z4,1);
            }
            if (lengths.atend())
              {
                 startGL(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {
                      s->lineGL(x1,y1,z1,x2,y2,z2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {
                pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next() * scale;
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
              }
            x3 = x4;  y3 = y4; z3 = z4;
         }
       else
         {
          if (on || length < 0.0)
          {
              s->lineGL(x3,y3,z3,x2,y2,z2,1);
          }
            pos += l1;
            length -= l1;
            break;
         }
    }
}

void LineStyle::drawGL(View3dSurface *s,double x1,double y1 ,double x2,double y2 )
{
  double t,l1,l2,dx,dy,dz,tlen;
  double x3,y3,x4,y4;

  dx = (x2 - x1) / s->getxdotspermm();
  dy = (y2 - y1) / s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6)
  {
      //qDebug() << "ret 1 : l2 < 1.0E-6 : " << l2;
      return;       // Screen distance tolerance
  }
  x3 = x1;  y3 = y1; tlen = 0.0;

   if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
    {
       s->line2dGL(x1,y1,x2,y2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {
      //qDebug() << "while (l1 > 0.0) : " << l1;
      //qDebug() << "if (endsegment - pos < l1)" << "endsegment - pos : " << endsegment - pos << "l1 : " << l1;
         if (endsegment - pos < l1)
         //if (roundDbl(endsegment - pos,-s->getpixelmodelmm()) < l1)
         {
            //l1 -= roundDbl(endsegment - pos,-s->getpixelmodelmm());
            l1 -= endsegment - pos;
            // qDebug() << "l1 -= endsegment - pos : " << l1;
            //length -= roundDbl(endsegment - pos,-s->getpixelmodelmm());
            length -= endsegment - pos;
            //qDebug() << "length -= endsegment - pos : " << length;

            t = (l2 - l1) / l2;
           // qDebug() << "t = (l2 - l1) / l2 : " << "t : " << t;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0)
            {
              //qDebug() << "ret 2 : on || length < 0.0 : " << "on : " << on << "length : " << length;
              //qDebug() << "x3" << x3 << "y3" << y3 << "z3 "<< z3;
              //qDebug() << "x4" << x4 << "y4" << y4 << "z4 "<< z4;
              s->line2dGL(x3,y3,x4,y4,1);
            }
            if (lengths.atend())
              {
                //qDebug() << "if (lengths.atend())";
                 start(scale,-1.0);
                 tlen += endsegment;
                // qDebug() << "tlen += endsegment : " << "tlen : " << tlen;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {
                    // qDebug() << "ret 3 : tlen >= 0.0 && tlen <= 1.0E-6 : " << "tlen : " << tlen;
                    // qDebug() << "x1" << x1 << "y1" << y1 << "z1 "<< z1;
                    // qDebug() << "x2" << x2 << "y2" << y2 << "z2 "<< z2;
                      s->line2dGL(x1,y1,x2,y2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {
                //qDebug() << "else ";
                pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;

                 //qDebug() << "pos :" << pos;
                 //qDebug() << "on :" << on;
                 //qDebug() << "endsegment :" << endsegment;
                 //qDebug() << "tlen :" << tlen;
              }
            x3 = x4;  y3 = y4;
            //qDebug() << "x3" << x3 << "y3" << y3 << "z3 "<< z3;
            //qDebug() << "x4" << x4 << "y4" << y4 << "z4 "<< z4;
         }
       else
         {  if (on || length < 0.0)
          {
              //qDebug() << "ret 4 : on || length < 0.0 : " << "on : " << on << "length : " << length ;
              //qDebug() << "x3" << x3 << "y3" << y3 << "z3 "<< z3;
              //qDebug() << "x2" << x2 << "y2" << y2 << "z2 "<< z2;
              s->line2dGL(x3,y3,x2,y2,1);
          }
            pos += l1;
            length -= l1;
            break;
         }
    }
}

#if 0
void LineStyle::draw(QPainterPath *path,Surface *s,double x1,double y1,double x2,double y2)
{
  double t,l1,l2,dx,dy,tlen;
  double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm();
  dy = (y2 - y1) / s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6)
      return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < (1.0 / s->getxdotspermm())/*0.2*/)
    {
       //qDebug() << "out 1";
       s->line(path,x1,y1,x2,y2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {  if (endsegment - pos < l1)
         {  l1 -= endsegment - pos;
            length -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0)
            {
              //qDebug() << "out 2 : " << x3 << "," << y3 << "," << x4 << "," << y4;
              s->line(path,x3,y3,x4,y4,1);
            }
            if (lengths.atend())
              {  start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {
                      //qDebug() << "out 3";
                      s->line(path,x1,y1,x2,y2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {  pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
              }
            x3 = x4;  y3 = y4;
         }
       else
         {  if (on || length < 0.0)
            {
              //qDebug() << "out 4 : " << x3 << "," << y3 << "," << x2 << "," << y2;
              s->line(path,x3,y3,x2,y2,1);
            }
            pos += l1;
            length -= l1;
            break;
         }
    }
}
#else
void LineStyle::draw(QPainterPath *path,Surface *s,double x1,double y1,double x2,double y2)
{
  double t,l1,l2,dx,dy,tlen;
  double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm();
  dy = (y2 - y1) / s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6)
      return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < (1.0 / s->getxdotspermm())/*0.2*/)
    {
       //qDebug() << "out 1";
       s->line(path,x1,y1,x2,y2,1);
       l1 = 0.0;
    }

  while (l1 > 0.0)
    {  if (endsegment - pos < l1)
         {
            l1 -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on)
            {
              //qDebug() << "out 2 : " << x3 << "," << y3 << "," << x4 << "," << y4;
              s->line(path,x3,y3,x4,y4,1);
            }
            if (lengths.atend())
              {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                   {
                      //qDebug() << "out 3";
                      s->line(path,x1,y1,x2,y2,1);
                      l1 = 0.0;
                   }
                 tlen = 0.0;
              }
            else
              {  pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 tlen += endsegment;
                 if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < (1.0 / s->getxdotspermm())/* 0.2*/)
                    s->line(path,x4,y4,x2,y2,1);
              }
            x3 = x4;  y3 = y4;
         }
       else
         {
            if (on)
            {
              //qDebug() << "out 4 : " << x3 << "," << y3 << "," << x2 << "," << y2;
              s->line(path,x3,y3,x2,y2,1);
            }
            pos += l1;
            break;
         }
    }
}
#endif

#if 0
QGraphicsItem * LineStyle::draw(QGraphicsItem *qgrp, Surface *s,double x1,double y1,double x2,double y2)
{
 double t,l1,l2,dx,dy,tlen;
 double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm(); // r0
  dy = (y2 - y1) / s->getydotspermm(); // r0
  //dx = (x2 - x1); // r1
  //dy = (y2 - y1); // r1
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;// r0
  //l1 = l2 = sqrt(dx*dx + dy*dy);// r1
  if (l2 < 1.0E-6) return 0;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

 QGraphicsItem *qline = 0, *pgLine = 0;
 QList<QGraphicsItem *> glist;

 //qDebug() << "Entering LineStyle::draw() : qgi : " << qgrp;

 if(qgrp)
 {
     //qDebug() << "LineStyle::draw() deleting : qgrp : " << qgrp;
     s->getScene()->removeItem(qgrp);
 }
 delete qgrp;
 qgrp=0;

 //qgrp = s->getScene()->createItemGroup(glist);

 if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
 {
      qline =  s->line(pgLine,x1,y1,x2,y2,1);
      if(qline)
        glist.append(qline);
      l1 = 0.0;
 }

 while (l1 > 0.0)
 {
        if (endsegment - pos < l1)
        {
            l1 -= endsegment - pos;
           length -= endsegment - pos;

           t = (l2 - l1) / l2;
           x4 = (x2 - x1) * t + x1;
           y4 = (y2 - y1) * t + y1;
           if (on || length < 0.0)
           {
             qline =  s->line(pgLine,x3,y3,x4,y4,1);
             if(qline)
                glist.append(qline);
           }
           if (lengths.atend())
           {
                start(scale,-1.0);
                tlen += endsegment;
                if (tlen >= 0.0 && tlen <= 1.0E-6)
                {
                     qline =  s->line(pgLine,x1,y1,x2,y2,1);
                     if(qline)
                        glist.append(qline);
                     l1 = 0.0;
                }
                tlen = 0.0;
           }
           else
           {
                pos = 0.0;  on = ! on;
                endsegment = *(double *)lengths.next();// r0
                //endsegment = *(double *)lengths.next() * scale;// r1
                if (endsegment > length && length > 0.0)
                   endsegment = length;
                tlen += endsegment;
           }
           x3 = x4;  y3 = y4;
      }
      else
      {
           if (on || length < 0.0)
           {
             qline =  s->line(pgLine,x3,y3,x2,y2,1);
             if(qline)
                glist.append(qline);
           }
           pos += l1;
           length -= l1;
           break;
      }
 }
 if(glist.size() > 0)
    qgrp = s->getScene()->createItemGroup(glist);
 //qDebug() << "Leaving LineStyle::draw() : qgi : " << qline;
 return qgrp;
}
#else
QGraphicsItem * LineStyle::draw(QGraphicsItem *qgrp, Surface *s,double x1,double y1,double x2,double y2)
{
 double t,l1,l2,dx,dy,tlen;
 double x3,y3,x4,y4;
  dx = (x2 - x1) / s->getxdotspermm(); // r0
  dy = (y2 - y1) / s->getydotspermm(); // r0
  //dx = (x2 - x1); // r1
  //dy = (y2 - y1); // r1
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;// r0
  //l1 = l2 = sqrt(dx*dx + dy*dy);// r1
  if (l2 < 1.0E-6) return 0;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

 QGraphicsItem *qline = 0, *pgLine = 0;
 QList<QGraphicsItem *> glist;

 //qDebug() << "Entering LineStyle::draw() : qgi : " << qgrp;

 if(qgrp)
 {
     //qDebug() << "LineStyle::draw() deleting : qgrp : " << qgrp;
     s->getScene()->removeItem(qgrp);
 }
 delete qgrp;
 qgrp=0;

 //qgrp = s->getScene()->createItemGroup(glist);

 if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
 {
      qline =  s->line(pgLine,x1,y1,x2,y2,1);
      if(qline)
        glist.append(qline);
      l1 = 0.0;
 }

 while (l1 > 0.0)
 {
        if (endsegment - pos < l1)
        {
            l1 -= endsegment - pos;

           t = (l2 - l1) / l2;
           x4 = (x2 - x1) * t + x1;
           y4 = (y2 - y1) * t + y1;
           if (on)
           {
             qline =  s->line(pgLine,x3,y3,x4,y4,1);
             if(qline)
                glist.append(qline);
           }
           if (lengths.atend())
           {
                start(scale,-1);
                tlen += endsegment;
                if (tlen >= 0.0 && tlen <= 1.0E-6)
                {
                     qline =  s->line(pgLine,x1,y1,x2,y2,1);
                     if(qline)
                        glist.append(qline);
                     l1 = 0.0;
                }
                tlen = 0.0;
           }
           else
           {
                pos = 0.0;  on = ! on;
                endsegment = *(double *)lengths.next();// r0
                //endsegment = *(double *)lengths.next() * scale;// r1
                tlen += endsegment;
                if (endsegment > 0.0 && endsegment / scale * s->getxdotspermm() < 0.2)
                {
                    qline =  s->line(pgLine,x4,y4,x2,y2,1);
                    if(qline)
                       glist.append(qline);
                }
           }
           x3 = x4;  y3 = y4;
      }
      else
      {
           if (on)
           {
             qline =  s->line(pgLine,x3,y3,x2,y2,1);
             if(qline)
                glist.append(qline);
           }
           pos += l1;
           break;
      }
 }
 if(glist.size() > 0)
    qgrp = s->getScene()->createItemGroup(glist);
 //qDebug() << "Leaving LineStyle::draw() : qgi : " << qline;
 return qgrp;
}
#endif

#if 0
void LineStyle::draw(QPainter *qp ,double x1,double y1,double x2,double y2)
{
 double t,l1,l2,dx,dy,tlen;
 double x3,y3,x4,y4;

  double xdotspermm = double(GetDeviceCaps(cadwindow->gethdc(),HORZRES)) / double(GetDeviceCaps(cadwindow->gethdc(),HORZSIZE));
  double ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));

  //double xdotspermm = (double)s->device()->physicalDpiX() / 25.4;
  //qDebug() << "QPainter : dpmx1 : s->device()->physicalDpiX() * 25.4 : " << xdotspermm ;
  //double ydotspermm = (double)s->device()->physicalDpiY() / 25.4;
  //qDebug() << "QPainter : dpmy1 : s->device()->physicalDpiY() * 25.4 : " << ydotspermm ;
  //double xdotspermm = (double)((double)s->device()->width() / (double)s->device()->widthMM());
  //qDebug() << "QPainter : dpmx2 : (s->device()->width() / s->device()->widthMM()) : " << xdotspermm ;
  //double ydotspermm = (double)((double)s->device()->height() / (double)s->device()->heightMM());
  //qDebug() << "QPainter : dpmy2 : (s->device()->width() / s->device()->widthMM()) : " << ydotspermm ;

  dx = (x2 - x1) / xdotspermm;//s->getxdotspermm();
  dy = (y2 - y1) / ydotspermm;//s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * xdotspermm < 0.2)
  //if (endsegment > 0.0 && endsegment / scale * s->device()->physicalDpiX() * 25.4 < 0.2)
  {
       QPointF qpf1(x1,y1),qpf2(x2,y2);
       qp->drawLine(qpf1,qpf2);
       //s->line(qp,x1,y1,x2,y2);
       l1 = 0.0;
  }

  while (l1 > 0.0)
  {
      if (endsegment - pos < l1)
      {
            l1 -= endsegment - pos;
            length -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0){
                QPointF qpf1(x3,y3),qpf2(x4,y4);
                qp->drawLine(qpf1,qpf2);
                //qp->drawLine(x3,y3,x4,y4);
                //s->line(qp,x3,y3,x4,y4);
            }
            if (lengths.atend())
            {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                 {
                     QPointF qpf1(x1,y1),qpf2(x2,y2);
                     qp->drawLine(qpf1,qpf2);
                     //qp->drawLine(x1,y1,x2,y2);
                     //s->line(qp,x1,y1,x2,y2);
                      l1 = 0.0;
                 }
                 tlen = 0.0;
            }
            else
            {
                 pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
            }
            x3 = x4;  y3 = y4;
       }
       else
       {
            if (on || length < 0.0){
                QPointF qpf1(x3,y3),qpf2(x2,y2);
                qp->drawLine(qpf1,qpf2);
                //qp->drawLine(x3,y3,x2,y2);
                //s->line(qp,x3,y3,x2,y2);
            }
            pos += l1;
            length -= l1;
            break;
       }
    }
}
#else
void LineStyle::draw(QPainter *qp ,double x1,double y1,double x2,double y2)
{
 double t,l1,l2,dx,dy,tlen;
 double x3,y3,x4,y4;

  double xdotspermm = double(GetDeviceCaps(cadwindow->gethdc(),HORZRES)) / double(GetDeviceCaps(cadwindow->gethdc(),HORZSIZE));
  double ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));

  //double xdotspermm = (double)s->device()->physicalDpiX() / 25.4;
  //qDebug() << "QPainter : dpmx1 : s->device()->physicalDpiX() * 25.4 : " << xdotspermm ;
  //double ydotspermm = (double)s->device()->physicalDpiY() / 25.4;
  //qDebug() << "QPainter : dpmy1 : s->device()->physicalDpiY() * 25.4 : " << ydotspermm ;
  //double xdotspermm = (double)((double)s->device()->width() / (double)s->device()->widthMM());
  //qDebug() << "QPainter : dpmx2 : (s->device()->width() / s->device()->widthMM()) : " << xdotspermm ;
  //double ydotspermm = (double)((double)s->device()->height() / (double)s->device()->heightMM());
  //qDebug() << "QPainter : dpmy2 : (s->device()->width() / s->device()->widthMM()) : " << ydotspermm ;

  dx = (x2 - x1) / xdotspermm;//s->getxdotspermm();
  dy = (y2 - y1) / ydotspermm;//s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * xdotspermm < 0.2)
  //if (endsegment > 0.0 && endsegment / scale * s->device()->physicalDpiX() * 25.4 < 0.2)
  {
       QPointF qpf1(x1,y1),qpf2(x2,y2);
       qp->drawLine(qpf1,qpf2);
       //s->line(qp,x1,y1,x2,y2);
       l1 = 0.0;
  }

  while (l1 > 0.0)
  {
      if (endsegment - pos < l1)
      {
            l1 -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on)
            {
                QPointF qpf1(x3,y3),qpf2(x4,y4);
                qp->drawLine(qpf1,qpf2);
            }
            if (lengths.atend())
            {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                 {
                     QPointF qpf1(x1,y1),qpf2(x2,y2);
                     qp->drawLine(qpf1,qpf2);
                      l1 = 0.0;
                 }
                 tlen = 0.0;
            }
            else
            {
                 pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 tlen += endsegment;
                 if (endsegment > 0.0 && endsegment / scale * xdotspermm < 0.2)
                 {
                     QPointF qpf1(x4,y4),qpf2(x2,y2);
                     qp->drawLine(qpf1,qpf2);
                 }
            }
            x3 = x4;  y3 = y4;
       }
       else
       {
            if (on){
                QPointF qpf1(x3,y3),qpf2(x2,y2);
                qp->drawLine(qpf1,qpf2);
            }
            pos += l1;
            length -= l1;
            break;
       }
    }
}
#endif

void LineStyle::draw(QPainter *qp,Surface *s,double x1,double y1,double x2,double y2)
{
 double t,l1,l2,dx,dy,tlen;
 double x3,y3,x4,y4;

  double xdotspermm = double(GetDeviceCaps(cadwindow->gethdc(),HORZRES)) / double(GetDeviceCaps(cadwindow->gethdc(),HORZSIZE));
  double ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));

  //double xdotspermm = (double)s->device()->physicalDpiX() / 25.4;
  //qDebug() << "QPainter : dpmx1 : s->device()->physicalDpiX() * 25.4 : " << xdotspermm ;
  //double ydotspermm = (double)s->device()->physicalDpiY() / 25.4;
  //qDebug() << "QPainter : dpmy1 : s->device()->physicalDpiY() * 25.4 : " << ydotspermm ;
  //double xdotspermm = (double)((double)s->device()->width() / (double)s->device()->widthMM());
  //qDebug() << "QPainter : dpmx2 : (s->device()->width() / s->device()->widthMM()) : " << xdotspermm ;
  //double ydotspermm = (double)((double)s->device()->height() / (double)s->device()->heightMM());
  //qDebug() << "QPainter : dpmy2 : (s->device()->width() / s->device()->widthMM()) : " << ydotspermm ;

  dx = (x2 - x1) / xdotspermm;//s->getxdotspermm();
  dy = (y2 - y1) / ydotspermm;//s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * xdotspermm < 0.2)
  //if (endsegment > 0.0 && endsegment / scale * s->device()->physicalDpiX() * 25.4 < 0.2)
  {
       //QPointF qpf1(x1,y1),qpf2(x2,y2);
       //qp->drawLine(qpf1,qpf2);
       s->line(qp,x1,y1,x2,y2);
       l1 = 0.0;
  }

  while (l1 > 0.0)
  {
      if (endsegment - pos < l1)
      {
            l1 -= endsegment - pos;
            length -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0){
                //QPointF qpf1(x3,y3),qpf2(x4,y4);
                //qp->drawLine(qpf1,qpf2);
                //qp->drawLine(x3,y3,x4,y4);
                s->line(qp,x3,y3,x4,y4);
            }
            if (lengths.atend())
            {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                 {
                     //QPointF qpf1(x1,y1),qpf2(x2,y2);
                     //qp->drawLine(qpf1,qpf2);
                     //qp->drawLine(x1,y1,x2,y2);
                     s->line(qp,x1,y1,x2,y2);
                      l1 = 0.0;
                 }
                 tlen = 0.0;
            }
            else
            {
                 pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
            }
            x3 = x4;  y3 = y4;
       }
       else
       {
            if (on || length < 0.0){
                //QPointF qpf1(x3,y3),qpf2(x2,y2);
                //qp->drawLine(qpf1,qpf2);
                //qp->drawLine(x3,y3,x2,y2);
                s->line(qp,x3,y3,x2,y2);
            }
            pos += l1;
            length -= l1;
            break;
       }
    }
}

void LineStyle::draw(QGraphicsView *sv,double x1,double y1,double x2,double y2,QPen pen=QPen())
{
  double t,l1,l2,dx,dy,tlen;
  double x3,y3,x4,y4;
  QGraphicsScene *s = sv->scene();
  double xdotspermm = double(GetDeviceCaps(cadwindow->gethdc(),HORZRES)) / double(GetDeviceCaps(cadwindow->gethdc(),HORZSIZE));
  double ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));
  dx = (x2 - x1) / (double)xdotspermm;//s->getxdotspermm();
  dy = (y2 - y1) / (double)ydotspermm;//s->getydotspermm();
  l1 = l2 = sqrt(dx*dx + dy*dy) * scale;
  if (l2 < 1.0E-6) return;       // Screen distance tolerance
  x3 = x1;  y3 = y1;  tlen = 0.0;

  if (endsegment > 0.0 && endsegment / scale * xdotspermm < 0.2)
  {
       QPointF qpf1(x1,y1),qpf2(x2,y2);
       s->addLine(QLineF(qpf1,qpf2),pen);
       //s->drawLine(qpf1,qpf2);
       //s->drawLine(x1,y1,x2,y2);
       l1 = 0.0;
  }

  while (l1 > 0.0)
  {
      if (endsegment - pos < l1)
      {
            l1 -= endsegment - pos;
            length -= endsegment - pos;

            t = (l2 - l1) / l2;
            x4 = (x2 - x1) * t + x1;
            y4 = (y2 - y1) * t + y1;
            if (on || length < 0.0){
                QPointF qpf1(x3,y3),qpf2(x4,y4);
                s->addLine(QLineF(qpf1,qpf2),pen);
                //s->drawLine(qpf1,qpf2);
                //s->drawLine(x3,y3,x4,y4);
            }
            if (lengths.atend())
            {
                 start(scale,-1.0);
                 tlen += endsegment;
                 if (tlen >= 0.0 && tlen <= 1.0E-6)
                 {
                     QPointF qpf1(x1,y1),qpf2(x2,y2);
                     s->addLine(QLineF(qpf1,qpf2),pen);
                     //s->drawLine(qpf1,qpf2);
                     //s->drawLine(x1,y1,x2,y2);
                      l1 = 0.0;
                 }
                 tlen = 0.0;
            }
            else
            {
                 pos = 0.0;  on = ! on;
                 endsegment = *(double *)lengths.next();
                 if (endsegment > length && length > 0.0)
                    endsegment = length;
                 tlen += endsegment;
            }
            x3 = x4;  y3 = y4;
       }
       else
       {
            if (on || length < 0.0){
                QPointF qpf1(x3,y3),qpf2(x2,y2);
                s->addLine(QLineF(qpf1,qpf2),pen);
                //s->drawLine(qpf1,qpf2);
                //s->drawLine(x3,y3,x2,y2);
            }
            pos += l1;
            length -= l1;
            break;
       }
    }
}
#endif

PatternLineStyle::PatternLineStyle(double h,double ls,double s,double g,double a1,double a2,int o,int f,int sz,RCCHAR *fn)
{ height = h;
  lengthscale = ls;
  shear = s;
  gap = g;
  orientation = o;
  fitting = f;
  size = sz;
  strcpy(filename,fn);
  angle1 = 0.0;
  angle2 = 0.0;
  offset = 0.0;
  pos = 0.0;
  originalshape = 0;
  newshape = 0;
  lastsegment = 0;
  patternwidth = 1.0;

  lastx1 = 0.0;
  lasty1 = 0.0;
  lastz1 = 0.0;
  lastx2 = 0.0;
  lasty2 = 0.0;
  lastz2 = 0.0;
  lastx3 = 0.0;
  lasty3 = 0.0;
  lastz3 = 0.0;
  lastsurface =0;
  scale = 1.0;
}

void PatternLineStyle::GetInfo(double *h,double *ls,double *s,double *g,double *a1,double *a2,int *o,int *f,int *sz,RCCHAR *fn)
{ *h = height;
  *ls = lengthscale;
  *s = shear;
  *g = gap;
  *o = orientation;
  *f = fitting;
  *sz = size;
  strcpy(fn,filename);
  *a1 = angle1;
  *a2 = angle2;
  *o = 0;
  *f = 0;
}

PatternLineStyle::PatternLineStyle(PatternLineStyle *pls)
{ height = pls->height;
  lengthscale = pls->lengthscale;
  shear = pls->shear;
  gap = pls->gap;
  orientation = pls->orientation;
  fitting = pls->fitting;
  size = pls->size;
  strcpy(filename,pls->filename);
  angle1 = pls->angle1;
  angle2 = pls->angle2;
  offset = pls->offset;
  pos = 0.0;
  originalshape = 0;
  newshape = 0;
  lastsegment = 0;
  patternwidth = 1.0;

  lastx1 = 0.0;
  lasty1 = 0.0;
  lastz1 = 0.0;
  lastx2 = 0.0;
  lasty2 = 0.0;
  lastz2 = 0.0;
  lastx3 = 0.0;
  lasty3 = 0.0;
  lastz3 = 0.0;
  lastsurface =0;
  scale = 1.0;
}

PatternLineStyle::PatternLineStyle()
{ height = 1.0;
  lengthscale = 1.0;
  shear = 0.0;
  gap = 0.0;
  orientation = 0;
  fitting = 0;
  size = 0;
  strcpy(filename,_RCT(""));
  angle1 = 0.0;
  angle2 = 0.0;
  offset = 0.0;
  pos = 0.0;
  originalshape = 0;
  newshape = 0;
  lastsegment = 0;
  patternwidth = 1.0;

  lastx1 = 0.0;
  lasty1 = 0.0;
  lastz1 = 0.0;
  lastx2 = 0.0;
  lasty2 = 0.0;
  lastz2 = 0.0;
  lastx3 = 0.0;
  lasty3 = 0.0;
  lastz3 = 0.0;
  lastsurface =0;
  scale = 1.0;
}

PatternLineStyle::~PatternLineStyle()
{ delete [] originalshape;
  delete [] newshape;
}

#ifdef USING_WIDGETS
void PatternLineStyle::drawGL(View3dSurface *output,double x1,double y1,double x2,double y2)
{double x,y,z,x3,y3,z3,x4,y4,z4,sx,dx,dy,dz,dx1,dx2,dz1,dy1,dy2,dz2,dx3,dx4,dy3,dy4,dz3,dz4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

  if (x1 == x2 && y1 == y2)
    return;

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *=  output->getxdotspermm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       lastx2 = x1;
       lasty2 = y1;
       lastx3 = x2;
       lasty3 = y2;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30 )
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;

       l = sqrt(dx3 * dx3 + dy3 * dy3);
       dx3 /= l;
       dy3 /= l;

       l = sqrt(dx4 * dx4 + dy4 * dy4);
       dx4 /= l;
       dy4 /= l;

       l = sqrt(dx * dx + dy * dy);
       dx /= l;
       dy /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);
       dx1 /= l;
       dy1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);
       dx2 /= l;
       dy2 /= l;

       cosa = dx * dx3 + dy * dy3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {
             //output->lineGL(lastx2,lasty2,lastz2,lastx3,lasty3,lastz3,1);
             output->line2dGL(lastx2,lasty2,lastx3,lasty3,1);

             pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));// + (lastz3-lastz2)*(lastz3-lastz2));

            lastx1 = lastx2;
            lasty1 = lasty2;

            lastx2 = lastx3;
            lasty2 = lasty3;

            lastx3 = x2;
            lasty3 = y2;


            return;
         }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
         {  if (originalshape[i].y == -1.0E30)
                break;
              output->line2dGL(lastx2 - dx1 * originalshape[i].y * height * scale,
                               lasty2 - dy1 * originalshape[i].y * height * scale ,
                               lastx3 - dx2 * originalshape[i].y * height * scale,
                               lasty3 - dy2 * originalshape[i].y * height * scale,
                               1);
         }

       t1 = pos / height / (lengthscale*patternwidth) / scale ;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));
       t2 = pos / height / (lengthscale*patternwidth) / scale ;

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 1000000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 1000000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t ;
            y = lasty2 + (lasty3 - lasty2) * t ;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale   ;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale  ;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale   ;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale  ;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale  ;
                      }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale  ;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale  ;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale  ;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale ;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                     }

                   output->line2dGL(x3,y3,x4,y4,1);

                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       lastx3 = x2;
       lasty3 = y2;
    }
}

void PatternLineStyle::drawGL(Surface *output,double x1,double y1,double z1,double x2,double y2,double z2)
{double x,y,z,x3,y3,z3,x4,y4,z4,sx,dx,dy,dz,dx1,dx2,dz1,dy1,dy2,dz2,dx3,dx4,dy3,dy4,dz3,dz4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

  if (x1 == x2 && y1 == y2)
    return;

  View3dSurface *v3ds = dynamic_cast<View3dSurface *>(output);

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *=  output->getxdotspermm();
       //scale *=  output->getxdotspermm() *  output->getpixelmodelmm();
       //scale =   output->getpixelmodelmm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       //lastz1 = z1 + (z1 - z2);
       lastx2 = x1;
       lasty2 = y1;
       //lastz2 = z1;
       lastx3 = x2;
       lasty3 = y2;
       //lastz3 = z3;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30 /*&& newshape[i].z == -1.0E30*/)
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       //dz = lastz3 - lastz2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       //dz3 = lastz2 - lastz1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;
       dz4=0;//dz4 = z2 - lastz3;
       l = sqrt(dx3 * dx3 + dy3 * dy3);// + dz3 * dz3);
       dx3 /= l;
       dy3 /= l;
       dz3=0;//dz3 /= l;
       l = sqrt(dx4 * dx4 + dy4 * dy4);// + dz4 * dz4);
       dx4 /= l;
       dy4 /= l;
       dz4=0;//dz4 /= l;
       l = sqrt(dx * dx + dy * dy);// + dz * dz);
       dx /= l;
       dy /= l;
       dz=0;//dz /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;
       dz1=0;//dz1 = dz3 + dz;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;
       dz2=0;//dz2 = dz + dz4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);// + dz1 * dz1);
       dx1 /= l;
       dy1 /= l;
       dz1=0;//dz1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);// + dz2 * dz2);
       dx2 /= l;
       dy2 /= l;
       dz2=0;//dz2 /= l;

       cosa = dx * dx3 + dy * dy3;// + dz * dz3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);
       dz1=0;//dz1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;// + dz * dz4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);
       dz2=0;//dz2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {
             output->lineGL(lastx2,lasty2,lastz2,lastx3,lasty3,lastz3,1);

             pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));// + (lastz3-lastz2)*(lastz3-lastz2));

            lastx1 = lastx2;
            lasty1 = lasty2;
            //lastz1 = lastz2;
            lastx2 = lastx3;
            lasty2 = lasty3;
            //lastz2 = lastz3;
            lastx3 = x2;
            lasty3 = y2;
            //lastz3 = z3;

            return;
         }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
         {  if (originalshape[i].y == -1.0E30)
                break;
              output->lineGL(lastx2 - dx1 * originalshape[i].y * height * scale,
                             lasty2 - dy1 * originalshape[i].y * height * scale ,
                             0,//lastz2 - dz1 * originalshape[i].y * height * scale,
                             lastx3 - dx2 * originalshape[i].y * height * scale,
                             lasty3 - dy2 * originalshape[i].y * height * scale,
                             0,1);//lastz3 - dz2 * originalshape[i].y * height * scale,1);
         }

       t1 = pos / height / (lengthscale*patternwidth) / scale /* * output->getpixelmodelmm()*/;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2)); // + (lastz3-lastz2)*(lastz3-lastz2));
       t2 = pos / height / (lengthscale*patternwidth) / scale ;
       //t2 = pos / height / (lengthscale*patternwidth) / scale * output->getpixelmodelmm();

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 4000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 4000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t ;
            y = lasty2 + (lasty3 - lasty2) * t ;
            z=0;//z = lastz2 + (lastz3 - lastz2) * t;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale   ;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale  ;
                   z3 =0;//= z + dz * originalshape[j].z * height * alengthscale - dz * originalshape[j].z * height * scale;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale   ;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                   z4=0;//z4 = z + dz * originalshape[j+1].z * height * alengthscale - dz * originalshape[j+1].z * height * scale;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale  ;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale  ;

                        z3=0;//z3 = z + dz * sx * height * alengthscale - dz * originalshape[j].z * height * scale;
                     }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale  ;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                        z4=0;//z4 = z + dz * sx * height * alengthscale - dz * originalshape[j+1].z * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale  ;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale  ;
                        z3=0;//z3 = z + dz * sx * height * alengthscale - dz * originalshape[j].z * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale ;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale  ;
                        z4=0;//z4 = z + dz * sx * height * alengthscale - dz * originalshape[j+1].z * height * scale;
                     }

                   output->lineGL(x3,y3,z3,x4,y4,z4,1);
                   //
#if 1
                   if(v3ds != 0)
                   {
                        Point glp3,glp4;
                        glp3 = v3ds->screentomodel(Point(x3,y3,z3));
                        glp4 = v3ds->screentomodel(Point(x4,y4,z4));
                        output->lineGL(glp3.x,glp3.y,glp3.z,glp4.x,glp4.y,glp4.z,1);
                   }
#endif
                   //

                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       //lastz1 = lastz2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       //lastz2 = lastz3;
       lastx3 = x2;
       lasty3 = y2;
       //lastz3 = z2;

    }
}

QGraphicsItem *  PatternLineStyle::draw(QGraphicsItem *qgrp,Surface *output,double x1,double y1,double x2,double y2)
{double x,y,x3,y3,x4,y4,sx,dx,dy,dx1,dx2,dy1,dy2,dx3,dx4,dy3,dy4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

 QGraphicsItem *qline = 0, *pgLine = 0;
 QList<QGraphicsItem *> glist;

 if(qgrp)
 {
     //qDebug() << "PatternLineStyle::draw() deleting : qgrp : " << qgrp;
     output->getScene()->removeItem(qgrp);
 }
 delete qgrp;
 qgrp=0;

  if (x1 == x2 && y1 == y2)
    return 0;

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *= output->getxdotspermm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       lastx2 = x1;
       lasty2 = y1;
       lastx3 = x2;
       lasty3 = y2;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30)
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;
       l = sqrt(dx3 * dx3 + dy3 * dy3);
       dx3 /= l;
       dy3 /= l;
       l = sqrt(dx4 * dx4 + dy4 * dy4);
       dx4 /= l;
       dy4 /= l;
       l = sqrt(dx * dx + dy * dy);
       dx /= l;
       dy /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);
       dx1 /= l;
       dy1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);
       dx2 /= l;
       dy2 /= l;

       cosa = dx * dx3 + dy * dy3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {
            //output->line(lastx2,lasty2,lastx3,lasty3,1);
            qline = output->line(pgLine,lastx2,lasty2,lastx3,lasty3,1);
            if(qline)
                glist.append(qline);

            pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));

            lastx1 = lastx2;
            lasty1 = lasty2;
            lastx2 = lastx3;
            lasty2 = lasty3;
            lastx3 = x2;
            lasty3 = y2;

            if(glist.size() > 0)
               qgrp = output->getScene()->createItemGroup(glist);
            //qDebug() << "Leaving PatternLineStyle::draw() : qgi : " << qline;
            return qgrp;
       }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
           {  if (originalshape[i].y == -1.0E30)
                break;

              //output->line(lastx2 - dx1 * originalshape[i].y * height * scale,lasty2 - dy1 * originalshape[i].y * height * scale,
              //             lastx3 - dx2 * originalshape[i].y * height * scale,lasty3 - dy2 * originalshape[i].y * height * scale,1);

              qline = output->line(pgLine,lastx2 - dx1 * originalshape[i].y * height * scale,lasty2 - dy1 * originalshape[i].y * height * scale,
                           lastx3 - dx2 * originalshape[i].y * height * scale,lasty3 - dy2 * originalshape[i].y * height * scale,1);

              if(qline)
                  glist.append(qline);
         }

       t1 = pos / height / (lengthscale*patternwidth) / scale;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));
       t2 = pos / height / (lengthscale*patternwidth) / scale;

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 4000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 4000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t;
            y = lasty2 + (lasty3 - lasty2) * t;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }

                   //output->line(x3,y3,x4,y4,1);

                   qline = output->line(pgLine,x3,y3,x4,y4,1);
                   if(qline)
                       glist.append(qline);

                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       lastx3 = x2;
       lasty3 = y2;

    }
  if(glist.size() > 0)
     qgrp = output->getScene()->createItemGroup(glist);
  //qDebug() << "Leaving PatternLineStyle::draw() : qgi : " << qline;
  return qgrp;
}

void PatternLineStyle::draw(QPainter *qp,Surface *output,double x1,double y1,double x2,double y2)
{double x,y,x3,y3,x4,y4,sx,dx,dy,dx1,dx2,dy1,dy2,dx3,dx4,dy3,dy4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

  if (x1 == x2 && y1 == y2)
    return;

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *= output->getxdotspermm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       lastx2 = x1;
       lasty2 = y1;
       lastx3 = x2;
       lasty3 = y2;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30)
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;
       l = sqrt(dx3 * dx3 + dy3 * dy3);
       dx3 /= l;
       dy3 /= l;
       l = sqrt(dx4 * dx4 + dy4 * dy4);
       dx4 /= l;
       dy4 /= l;
       l = sqrt(dx * dx + dy * dy);
       dx /= l;
       dy /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);
       dx1 /= l;
       dy1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);
       dx2 /= l;
       dy2 /= l;

       cosa = dx * dx3 + dy * dy3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {
             output->line(qp,lastx2,lasty2,lastx3,lasty3,1);

             pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));

            lastx1 = lastx2;
            lasty1 = lasty2;
            lastx2 = lastx3;
            lasty2 = lasty3;
            lastx3 = x2;
            lasty3 = y2;

            return;
         }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
           {  if (originalshape[i].y == -1.0E30)
                break;
              output->line(qp,lastx2 - dx1 * originalshape[i].y * height * scale,lasty2 - dy1 * originalshape[i].y * height * scale,
                           lastx3 - dx2 * originalshape[i].y * height * scale,lasty3 - dy2 * originalshape[i].y * height * scale,1);
           }

       t1 = pos / height / (lengthscale*patternwidth) / scale;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));
       t2 = pos / height / (lengthscale*patternwidth) / scale;

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 4000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 4000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t;
            y = lasty2 + (lasty3 - lasty2) * t;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   output->line(x3,y3,x4,y4,1);
                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       lastx3 = x2;
       lasty3 = y2;

    }
}
#endif

void PatternLineStyle::draw(Surface *output,double x1,double y1,double x2,double y2)
{double x,y,x3,y3,x4,y4,sx,dx,dy,dx1,dx2,dy1,dy2,dx3,dx4,dy3,dy4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

  if (x1 == x2 && y1 == y2)
    return;

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *= output->getxdotspermm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       lastx2 = x1;
       lasty2 = y1;
       lastx3 = x2;
       lasty3 = y2;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30)
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;
       l = sqrt(dx3 * dx3 + dy3 * dy3);
       dx3 /= l;
       dy3 /= l;
       l = sqrt(dx4 * dx4 + dy4 * dy4);
       dx4 /= l;
       dy4 /= l;
       l = sqrt(dx * dx + dy * dy);
       dx /= l;
       dy /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);
       dx1 /= l;
       dy1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);
       dx2 /= l;
       dy2 /= l;

       cosa = dx * dx3 + dy * dy3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {  output->line(lastx2,lasty2,lastx3,lasty3,1);

            pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));

            lastx1 = lastx2;
            lasty1 = lasty2;
            lastx2 = lastx3;
            lasty2 = lasty3;
            lastx3 = x2;
            lasty3 = y2;

            return;
         }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
           {  if (originalshape[i].y == -1.0E30)
                break;
              output->line(lastx2 - dx1 * originalshape[i].y * height * scale,lasty2 - dy1 * originalshape[i].y * height * scale,
                           lastx3 - dx2 * originalshape[i].y * height * scale,lasty3 - dy2 * originalshape[i].y * height * scale,1);
           }

       t1 = pos / height / (lengthscale*patternwidth) / scale;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));
       t2 = pos / height / (lengthscale*patternwidth) / scale;

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 4000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 4000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t;
            y = lasty2 + (lasty3 - lasty2) * t;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   output->line(x3,y3,x4,y4,1);
                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       lastx3 = x2;
       lasty3 = y2;

    }
}

void PatternLineStyle::draw(QPainterPath *path, Surface *output,double x1,double y1,double x2,double y2)
{double x,y,x3,y3,x4,y4,sx,dx,dy,dx1,dx2,dy1,dy2,dx3,dx4,dy3,dy4,l,t,t1,t2,cosa,alengthscale;
 int i1,i2,i,j;

  if (x1 == x2 && y1 == y2)
    return;

  lastsurface = output;

  if (pos == -1.0E30)
    {  pos = 0.0;
       scale *= output->getxdotspermm();
       lastx1 = x1 + (x1 - x2);
       lasty1 = y1 + (y1 - y2);
       lastx2 = x1;
       lasty2 = y1;
       lastx3 = x2;
       lasty3 = y2;
       for (i = 0 ; originalshape != 0 ; i++)
         {  newshape[i] = originalshape[i];
            if (newshape[i].x == -1.0E30 && newshape[i].y == -1.0E30)
              break;
         }
    }
  else
    {
       dx = lastx3 - lastx2;
       dy = lasty3 - lasty2;
       dx3 = lastx2 - lastx1;
       dy3 = lasty2 - lasty1;
       dx4 = x2 - lastx3;
       dy4 = y2 - lasty3;
       l = sqrt(dx3 * dx3 + dy3 * dy3);
       dx3 /= l;
       dy3 /= l;
       l = sqrt(dx4 * dx4 + dy4 * dy4);
       dx4 /= l;
       dy4 /= l;
       l = sqrt(dx * dx + dy * dy);
       dx /= l;
       dy /= l;

       dx1 = -dy3 - dy;
       dy1 = dx3 + dx;

       dx2 = -dy - dy4;
       dy2 = dx + dx4;

       l = sqrt(dx1 * dx1 + dy1 * dy1);
       dx1 /= l;
       dy1 /= l;

       l = sqrt(dx2 * dx2 + dy2 * dy2);
       dx2 /= l;
       dy2 /= l;

       cosa = dx * dx3 + dy * dy3;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx1 /= cos(acos(cosa)/2.0);
       dy1 /= cos(acos(cosa)/2.0);

       cosa = dx * dx4 + dy * dy4;
       if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
       dx2 /= cos(acos(cosa)/2.0);
       dy2 /= cos(acos(cosa)/2.0);

       if (originalshape == 0)
         {  output->line(path,lastx2,lasty2,lastx3,lasty3,1);

            pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));

            lastx1 = lastx2;
            lasty1 = lasty2;
            lastx2 = lastx3;
            lasty2 = lasty3;
            lastx3 = x2;
            lasty3 = y2;

            return;
         }

       for (i = 0 ; originalshape != 0 ; i++)
         if (originalshape[i].x == -1.0E30)
           {  if (originalshape[i].y == -1.0E30)
                 break;
              output->line(path,lastx2 - dx1 * originalshape[i].y * height * scale,lasty2 - dy1 * originalshape[i].y * height * scale,
                           lastx3 - dx2 * originalshape[i].y * height * scale,lasty3 - dy2 * originalshape[i].y * height * scale,1);
           }

       t1 = pos / height / (lengthscale*patternwidth) / scale;
       pos += sqrt((lastx3-lastx2)*(lastx3-lastx2) + (lasty3-lasty2)*(lasty3-lasty2));
       t2 = pos / height / (lengthscale*patternwidth) / scale;

       if (fabs(t1) >= INT_MAX || fabs(t2) >= INT_MAX)
         i1 = i2 = 0;
       else if (t2 - t1 > 4000)
         {  i1 = int(ceil(t1));
            i2 = i1 + 4000;
         }
       else
         {  i1 = int(ceil(t1));
            i2 = int(floor(t2));
         }

       alengthscale = lengthscale * scale;

       for (i = i1 ; i <= i2 ; i++)
         {  t = double(i - t1) / double(t2 - t1);
            x = lastx2 + (lastx3 - lastx2) * t;
            y = lasty2 + (lasty3 - lasty2) * t;
            for (j = 0 ; originalshape != 0 ; j++)
              if (originalshape[j].x != -1.0E30)
                {
                   x3 = x + dx * originalshape[j].x * height * alengthscale + dy * originalshape[j].y * height * scale;
                   y3 = y + dy * originalshape[j].x * height * alengthscale - dx * originalshape[j].y * height * scale;
                   x4 = x + dx * originalshape[j+1].x * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                   y4 = y + dy * originalshape[j+1].x * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                   if (i == i1 && fabs(originalshape[j].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j].y * tan(angle1) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i1 && fabs(originalshape[j+1].x) < 0.001)
                     {//  Adjust x to account for the start angle
                        sx = -originalshape[j+1].y * tan(angle1) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j].y * tan(angle2) / alengthscale;
                        x3 = x + dx * sx * height * alengthscale  + dy * originalshape[j].y * height * scale;
                        y3 = y + dy * sx * height * alengthscale - dx * originalshape[j].y * height * scale;
                     }
                   if (i == i2-1 && fabs(originalshape[j+1].x-1.0) < 0.001)
                     {//  Adjust x to account for the end angle
                        sx = 1.0 - originalshape[j+1].y * tan(angle2) / alengthscale;
                        x4 = x + dx * sx * height * alengthscale  + dy * originalshape[j+1].y * height * scale;
                        y4 = y + dy * sx * height * alengthscale - dx * originalshape[j+1].y * height * scale;
                     }
                   output->line(path,x3,y3,x4,y4,1);
                   j++;
                }
              else if (originalshape[j].y == -1.0E30)
                break;
         }

       lastx1 = lastx2;
       lasty1 = lasty2;
       lastx2 = lastx3;
       lasty2 = lasty3;
       lastx3 = x2;
       lasty3 = y2;

    }
}

void PatternLineStyle::start(double s,double ps)
{CadFile infile;
 int nvertices;
 double xmin,xmax;

  delete [] originalshape;
  delete [] newshape;
  originalshape = 0;
  newshape = 0;

  scale = 1.0 / s ;
  if (lengthscale > 0.0 && size == 1)
    scale *= ps;

  xmin = xmax = pos = -1.0E30;
  nvertices = 0;

  // convert full path pattern name
  // remember to use native separators here!!!
  RCCHAR patternname[1000];
  convertLinePatternFileName(filename,patternname);
  qDebug() << "pattern file : " << QString(patternname);
  //strcpy(filename,patternname);
  //if (infile.open(filename,_RCT("rb")))
  //
  if (infile.open(patternname,_RCT("rb")))
    {  if (infile.readheader())
         {Entity *e;
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  if (e->getatlist().length() != 0)
                        nvertices++;
                      else
                        nvertices += 2;
                   }
                 else if (e->isa(point_entity))
                   {  if (xmin < -0.99E-30 || ((PointE *)e)->getp().x < xmin)
                        xmin = ((PointE *)e)->getp().x;
                      if (xmax < -0.99E-30 || ((PointE *)e)->getp().x > xmax)
                        xmax = ((PointE *)e)->getp().x;
                   }
                 delete e;
              }
         }
       infile.close();
    }


  if (xmax > -1.0E30 && xmin > -1.0E30)
    patternwidth = xmax - xmin;
  else
    patternwidth = 1.0;

  //if (infile.open(filename,_RCT("rb")))
    if (infile.open(patternname,_RCT("rb")))
    {
       originalshape = 0;//new Point[nvertices];
       newshape = 0;//new Point[nvertices];
       if (infile.readheader())
         {
            originalshape = new Point[nvertices+1];
            newshape = new Point[nvertices+1];
            nvertices = 0;
            Entity *e;
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  if (e->getatlist().length() != 0)
                        {  originalshape[nvertices] = ((Line *)e)->getp1();
                           originalshape[nvertices++].x = -1.0E30;
                        }
                      else
                        {  originalshape[nvertices++] = ((Line *)e)->getp1();
                           originalshape[nvertices++] = ((Line *)e)->getp2();
                        }
                      delete e;
                   }
              }
            originalshape[nvertices].x = -1.0E30;
            originalshape[nvertices].y = -1.0E30;
            //originalshape[nvertices].z = -1.0E30;
         }

       infile.close();
    }

  lastsegment = 0;

}

#ifdef USING_WIDGETS
void PatternLineStyle::startGL(double s,double ps)
{CadFile infile;
 int nvertices;
 double xmin,xmax;

  delete [] originalshape;
  delete [] newshape;
  originalshape = 0;
  newshape = 0;

  scale = 1.0 / s ;
  if (lengthscale > 0.0 && size == 1)
    scale *= ps;

  xmin = xmax = pos = -1.0E30;
  nvertices = 0;

  // convert full path pattern name
  // remember to use native separators here!!!
  RCCHAR patternname[1000];
  convertLinePatternFileName(filename,patternname);
  //strcpy(filename,patternname);
  //if (infile.open(filename,_RCT("rb")))
  //
  if (infile.open(patternname,_RCT("rb")))
    {  if (infile.readheader())
         {Entity *e;
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  if (e->getatlist().length() != 0)
                        nvertices++;
                      else
                        nvertices += 2;
                   }
                 else if (e->isa(point_entity))
                   {  if (xmin < -0.99E-30 || ((PointE *)e)->getp().x < xmin)
                        xmin = ((PointE *)e)->getp().x;
                      if (xmax < -0.99E-30 || ((PointE *)e)->getp().x > xmax)
                        xmax = ((PointE *)e)->getp().x;
                   }
                 delete e;
              }
         }
       infile.close();
    }


  if (xmax > -1.0E30 && xmin > -1.0E30)
    patternwidth = xmax - xmin;
  else
    patternwidth = 1.0;

  Transform tf;
  //tf.scale(-1.0,1.0,1.0);
  tf.scale(1.0,-1.0,1.0);
  //if (infile.open(filename,_RCT("rb")))
    if (infile.open(patternname,_RCT("rb")))
    {
       originalshape = 0;//new Point[nvertices];
       newshape = 0;//new Point[nvertices];
       if (infile.readheader())
         {
            originalshape = new Point[nvertices+1];
            newshape = new Point[nvertices+1];
            nvertices = 0;
            Entity *e;
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  if (e->getatlist().length() != 0)
                        {
                           originalshape[nvertices] = tf.transform(((Line *)e)->getp1());
                           originalshape[nvertices++].x = -1.0E30;
                        }
                      else
                        {
                           //qDebug() << "p1 before transform x : " <<  ((Line *)e)->getp1().x << " y : " << ((Line *)e)->getp1().y;
                           //qDebug() << "p2 before transform x : " <<  ((Line *)e)->getp2().x << " y : " << ((Line *)e)->getp2().y;
                           originalshape[nvertices++] = tf.transform(((Line *)e)->getp1());
                           originalshape[nvertices++] = tf.transform(((Line *)e)->getp2());
                           //qDebug() << "p1 after transform x : " <<  ((Line *)e)->getp1().x << " y : " << ((Line *)e)->getp1().y;
                           //qDebug() << "p2 after transform x : " <<  ((Line *)e)->getp2().x << " y : " << ((Line *)e)->getp2().y;
                        }
                      delete e;
                   }
              }
            originalshape[nvertices].x = -1.0E30;
            originalshape[nvertices].y = -1.0E30;
            //originalshape[nvertices].z = -1.0E30;
         }
       infile.close();
    }

  lastsegment = 0;

}

void PatternLineStyle::endGL(View3dSurface *vs)
{ lastsegment = 1;
  if (pos != -1.0E30)
    drawGL(vs,lastx3,lasty3,
              (lastx3 + (lastx3 - lastx2)),
              (lasty3 + (lasty3 - lasty2)));
}

void PatternLineStyle::endGL(void)
{ lastsegment = 1;
  if (pos != -1.0E30)
    drawGL(lastsurface,lastx3,lasty3,lastz3,
                       (lastx3 + (lastx3 - lastx2)),
                       (lasty3 + (lasty3 - lasty2)),
                       (lastz3 + (lastz3 - lastz2)));
}

QGraphicsItem * PatternLineStyle::end(QGraphicsItem *qgrp)
{
    QGraphicsItem *qline =0;

    lastsegment = 1;
    if (pos != -1.0E30)
        qline = draw(qgrp,lastsurface,(int)lastx3,(int)lasty3,(int)(lastx3 + (lastx3 - lastx2)),(int)(lasty3 + (lasty3 - lasty2)));

  return qline;
}

void PatternLineStyle::end(QPainterPath *path)
{ lastsegment = 1;
  if (pos != -1.0E30)
    draw(path, lastsurface,(int)lastx3,(int)lasty3,(int)(lastx3 + (lastx3 - lastx2)),(int)(lasty3 + (lasty3 - lasty2)));
}
#endif

void PatternLineStyle::end(void)
{ lastsegment = 1;
  if (pos != -1.0E30)
    draw(lastsurface,(int)lastx3,(int)lasty3,(int)(lastx3 + (lastx3 - lastx2)),(int)(lasty3 + (lasty3 - lasty2)));
}

int PatternLineStyle::save(CadFile *outfile) const
{ if (! outfile->write(&height)) return 0;
  if (! outfile->write(&lengthscale)) return 0;
  if (! outfile->write(&shear)) return 0;
  if (! outfile->write(&gap)) return 0;
  if (! outfile->write(&offset)) return 0;
  if (! outfile->write(&angle2)) return 0;
  if (! outfile->write(&angle1)) return 0;
  if (! outfile->write(&orientation)) return 0;
  if (! outfile->write(&fitting)) return 0;
  if (! outfile->write(&size)) return 0;
  if (! outfile->write(filename)) return 0;
  return 1;
}

int PatternLineStyle::load(CadFile *infile)
{
  char *cfn;
  RCCHAR *fn;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&lengthscale)) return 0;
  if (! infile->read(&shear)) return 0;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&offset)) return 0;
  if (! infile->read(&angle2)) return 0;
  if (! infile->read(&angle1)) return 0;
  if (! infile->read(&orientation)) return 0;
  if (! infile->read(&fitting)) return 0;
  if (! infile->read(&size)) return 0;
  if(infile->getversion() == 2)
  {
      if (! infile->read(&cfn)) return 0;
      strcpy(filename,cfn);
      delete cfn;
  }
  else
  {
      if (! infile->read(&fn)) return 0;
      strcpy(filename,fn);
      delete fn;
  }
  return 1;
}

LineStyles::LineStyles()
{int i;
  nstyles=0;
  for (i = 0 ; i < 256 ; i++)
    styles[i] = NULL;
  styles[1] = new LineStyle();
  styles[1]->add(4.0);  styles[1]->add(4.0);
  styles[2] = new LineStyle();
  styles[2]->add(2.0);  styles[2]->add(2.0);
  styles[3] = new LineStyle();
  styles[3]->add(4.0);  styles[3]->add(2.0);
  styles[4] = new LineStyle();
  styles[4]->add(1.5);  styles[4]->add(10.0);
  styles[4]->add(1.5);  styles[4]->add(3.0);
  styles[5] = new LineStyle();
  styles[5]->add(1.5);  styles[5]->add(10.0);
  styles[5]->add(1.5);  styles[5]->add(3.0);
  styles[5]->add(1.5);  styles[5]->add(3.0);
  styles[6] = new LineStyle();
  styles[6]->add(1.0);  styles[6]->add(1.0);
  styles[7] = new LineStyle();
  styles[7]->add(1.0);  styles[7]->add(2.0);

  //  Load the dashed line styles as well
  db.linestyles.load(home.getinifilename());

}

LineStyles::LineStyles(const LineStyles &other)
{
    int i;
    nstyles = other.nstyles;
    for (i = 0 ; i < 256 ; i++)
    {
      delete styles[i];
      styles[i] = other.styles[i];
    }
}

LineStyles::~LineStyles()
{int i;
  nstyles=0;
  for (i = 0 ; i < 256 ; i++)
  {
    delete styles[i];
      styles[i] = NULL;
  }
}

int LineStyles::add(LineStyle *s)
{int i;
  for (i = 1 ; i < 255 ; i++)
    if (styles[i] == NULL)
      {  styles[i] = s;
         return i;
      }
  return -1;
}

void LineStyles::set(int index,LineStyle *s)
{ if (index >= 0 && index <= 255)
    styles[index] = s;
}

void LineStyles::del(int i)
{ if (i >= 0 && i <= 255)
    delete styles[i];
  styles[i] = NULL;
}

int LineStyles::save(CadFile *outfile) const
{short n,i,defined;
  n = 256;
  if (! outfile->write(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  defined = short(styles[i] != NULL);
       if (! outfile->write(&defined)) return 0;
       if (defined) styles[i]->save(outfile);
    }
  return 1;
}

int LineStyles::save(RCCHAR *inifilename) const
{int i;
 RCCHAR key[300],data[600];

  for (i = 0 ; i < 256 ; i++)
    {  sprintf(key,"LineStyle%d",i);
       if (styles[i] == 0)
         strcpy(data,"0");
       else
         styles[i]->save(data);
       WritePrivateProfileString(_RCT("LineStyles"),key,data,inifilename);
    }

  return 1;
}

int LineStyles::savedxf(DXFOut *dxfout) const
{int i,n;
  if (! dxfout->writestring(0, (char*)"TABLE")) return 0;
  if (! dxfout->writestring(2, (char*)"LTYPE")) return 0;
  if (! dxfout->writelong(5,1000)) return 0;
  if (! dxfout->writelong(330,0)) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbSymbolTable")) return 0;
  for (i = 1,n = 1 ; i < 256 ; i++)
    n += styles[i] != 0;
  if (! dxfout->writelong(70,n)) return 0;


  if (! dxfout->writestring(0, (char*)"LTYPE")) return 0;
  if (! dxfout->writelong(5,1001)) return 0;
  if (! dxfout->writelong(330,1000)) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbSymbolTableRecord")) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbLinetypeTableRecord")) return 0;
  if (! dxfout->writestring(2, (char*)"BYBLOCK")) return 0;
  if (! dxfout->writelong(70,0)) return 0;
  if (! dxfout->writestring(3, (char*)"")) return 0;
  if (! dxfout->writelong(72,65)) return 0;
  if (! dxfout->writelong(73,0)) return 0;
  if (! dxfout->writereal(40,0.0)) return 0;

  if (! dxfout->writestring(0, (char*)"LTYPE")) return 0;
  if (! dxfout->writelong(5,1002)) return 0;
  if (! dxfout->writelong(330,1000)) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbSymbolTableRecord")) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbLinetypeTableRecord")) return 0;
  if (! dxfout->writestring(2, (char*)"BYLAYER")) return 0;
  if (! dxfout->writelong(70,0)) return 0;
  if (! dxfout->writestring(3, (char*)"")) return 0;
  if (! dxfout->writelong(72,65)) return 0;
  if (! dxfout->writelong(73,0)) return 0;
  if (! dxfout->writereal(40,0.0)) return 0;

  if (! dxfout->writestring(0, (char*)"LTYPE")) return 0;
  if (! dxfout->writelong(5,1003)) return 0;
  if (! dxfout->writelong(330,1000)) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbSymbolTableRecord")) return 0;
  if (! dxfout->writestring(100, (char*)"AcDbLinetypeTableRecord")) return 0;
  if (! dxfout->writestring(2, (char*)"CONTINUOUS")) return 0;
  if (! dxfout->writelong(70,64L)) return 0;
  if (! dxfout->writestring(3, (char*)"Solid line")) return 0;
  if (! dxfout->writelong(72,65L)) return 0;
  if (! dxfout->writelong(73,0)) return 0;
  if (! dxfout->writereal(40,0.0)) return 0;
  for (i = 1 ; i < 256 ; i++)
    if (styles[i] != 0)
      styles[i]->savedxf(i,dxfout);
  if (! dxfout->writestring(0, (char*)"ENDTAB")) return 0;
  return 1;
}

int LineStyles::load(CadFile *infile)
{short n,i,defined;
  clear();
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! infile->read(&defined)) return 0;
       if (defined)
         {  if ((styles[i] = new LineStyle) == NULL) return 0;
            if (! styles[i]->load(infile)) return 0;
         }
    }
  return 1;
}

int LineStyles::load(RCCHAR *inifilename)
{
 int i;
 RCCHAR key[300],data[600];

  for (i = 0 ; i < 256 ; i++)
    {  sprintf(key,"LineStyle%d",i);
       GetPrivateProfileString(_RCT("LineStyles"),key,_RCT(""),data,600,inifilename);
       if (strlen(data) > 0)
         {  delete styles[i];  styles[i] = 0;
            if (atoi((char*)data) > 0)//fg.2022
              {  if ((styles[i] = new LineStyle) == NULL) return 0;
                 if (! styles[i]->load(data)) return 0;
              }
         }
    }

  return 1;
}

void LineStyles::clear(void)
{int i;
  for (i = 0 ; i < 256 ; i++)
    {  delete styles[i];  styles[i] = NULL;
    }
}

LineStyle *LineStyles::indextostyle(int style)
{  if (style < 0 || style > 255) return NULL;
   return styles[style];
}

LayerNames::LayerNames()
{int i;
  for (i = 0 ; i < MAX_LAYERS ; i++)
  {
    layernames[i] = 0;
  }
  isCleared = false;
}

bool LayerNames::isEmpty() {
	return isCleared;
}

void LayerNames::clear(void)
{int i;
  for (i = 0 ; i < MAX_LAYERS ; i++)
    {
       delete [] layernames[i];
       layernames[i] = 0;
    }
  isCleared = true;
}

RCCHAR *LayerNames::getlayername(int i)
{
    /*
#ifdef _USING_QTCHAR
    if (i < 0 || i >= MAX_LAYERS || layernames[i] == 0)
        return _RCT("");
    else
    {
        if( clayernames[i] == 0 || strlen(clayernames[i]) == 0 )
        {
            clayernames[i] = new char[strlen(layernames[i])+1];
            strcpy(clayernames[i],layernames[i]);
        }
        return clayernames[i];
    }
#else
    return i < 0 || i >= MAX_LAYERS || layernames[i] == 0 ? "" : layernames[i];
#endif
*/
    return i < 0 || i >= MAX_LAYERS || layernames[i] == 0 ? (RCCHAR*)0 : layernames[i];
}

RCCHAR *LayerNames::getlayername(int i,bool f)
{
    return i < 0 || i >= MAX_LAYERS || layernames[i] == 0 ? (RCCHAR*)0 : layernames[i];
}
void LayerNames::setlayername(int i,char *name)
{
    setlayername(i,CHAR2RCCHR(name));
}

void LayerNames::setlayername(int i,RCCHAR *name)
{ if (i >= 0 && i <= MAX_LAYERS)
    {
        delete [] layernames[i];
       layernames[i] = new RCCHAR[strlen(name)+1];
       strcpy(layernames[i],name);
    }
}
void LayerNames::copyFrom(const LayerNames& src) {
	clear();
	for (int i = 0; i < MAX_LAYERS; i++) {
		RCCHAR *pName = const_cast<LayerNames&>(src).getlayername(i);
		if(strlen(pName) > 0)		
			setlayername(i,pName);
	}
	isCleared = false;
}

int LayerNames::save(CadFile *outfile) const
{short i,n,type;
  for (i = 0,n = 0 ; i < MAX_LAYERS ; i++)
    n += short(layernames[i] != 0);
  if (n > 0)
    {  type = layername_record;
       if (! outfile->write(&type)) return 0;
       if (! outfile->write(&n)) return 0;
       for (i = 0 ; i < MAX_LAYERS ; i++)
         if (layernames[i] != 0)
           {  if (! outfile->write(&i)) return 0;
              if (! outfile->write(layernames[i])) return 0;
           }
    }
  return 1;
}

int LayerNames::load(CadFile *infile)
{
 char *cname;
 RCCHAR *name;
 short i,j,n;
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! infile->read(&j)) return 0;
      if( infile->getversion() == 2)
      {
          if (! infile->read(&cname))
              return 0;
          name = new RCCHAR[strlen(cname)+1];
          strcpy(name,cname);
          delete [] cname;
      }
      else
      {
          if (! infile->read(&name))
              return 0;
      }
       setlayername(j,name);
       delete [] name;
    }
  return 1;
}

int LayerNames::savedxf(DXFOut *) const
{ return 1;
}

void SurfaceEntity::cleartriangles(void)
{ ntriangles = -1;
  delete [] triangles;
  triangles = 0;
}
bool canDeleteCopyEntities(EntityList&  list) {
	Entity* e = 0;
	QList<int> ids;
	for (list.start(); (e = list.next()) != nullptr;)
		ids.append(e->getid());
	QMapIterator<int, copyAlongDialogData> iter(db.copyAlogMap);
	while (iter.hasNext()) {
		iter.next();
		if (ids.contains(iter.key())) return false;
	}
	return true;
}
//
void DeleteCopyEntities(EntityList&  list) {	

	QMapIterator<int, copyAlongDialogData> iter(db.copyAlogMap);
	while (iter.hasNext()) {
		iter.next();
		copyAlongDialogData data = iter.value();		
		if (data.deleteCopyEntities(iter.key(), list))
			db.copyAlogMap[iter.key()] = data;		
	}
}
/*void insertEntityIntoCopyList(long index) {
	Entity* e = db.geometry.getentity(index);
	if (e != nullptr && db.copyAlogMap.contains(e->getRefId()))
		db.copyAlogMap[e->getRefId()].insertCopyEntity(index);
	
}

void deleteEntityFromCopyList(long index) {
	QMapIterator<int, copyAlongDialogData> iter(db.copyAlogMap);
	while (iter.hasNext()) {
		iter.next();
		copyAlongDialogData data = iter.value();
		if (data.deleteCopyEntity(index)) return;
	}
}*/

//
void copyAlongDialogData::unDo(int undoing, bool isCircle) {
	
	if (undoing == 1) {//undo
		if (level > 1) {
			undoMap[level-1].redoList = undoMap[level].copyList;
			undoMap[level-1].copyList = undoMap[level].undoList;
			level--;
		}
	}
	else if (undoing == 2) {//redo
		undoMap[level +1].undoList  = undoMap[level].copyList;
		undoMap[level +1].copyList  = undoMap[level].redoList;
		level++;		
	}
}
bool copyAlongDialogData::insertCopyEntity(long id) {
	if (!undoMap[level].copyList.contains(id)) {
		undoMap[level].copyList.append(id);
		return true;
	}
	return false;
}
bool copyAlongDialogData::deleteCopyEntity(long id) {
	
	for (int i = 0; i < undoMap[level].copyList.length(); i++) {
		if (undoMap[level].copyList[i] == id) {			
			undoMap[level].copyList.removeAt(i);
			n--;
			return true;
		}
	}
	return false;
}
bool copyAlongDialogData::deleteCopyEntities(int id,EntityList& list) {	
	Entity* e = 0;
	bool result = false;	
	bool doBackup = false;
	for (list.start(); (e = list.next()) != nullptr;) {
		
		if (!doBackup && undoMap[level].copyList.contains(e->getid())) {
			doBackup = true;
			undoMap[level + 1].undoList = undoMap[level].copyList;
		}

		if (deleteCopyEntity(e->getid())) {
			e->setRefId(id);
			result = true;
		}
	}
	if (result) {
		undoMap[level + 1].copyList = undoMap[level].copyList;
		level++;
	}
	return result;
}
QList<long> copyAlongDialogData::cloneList(const QList<long>& list, Transform& t) const {
	QList<long> copy;
	for (int i = 0; i < list.length(); i++) {
		Entity* e = db.geometry.getentity(list.at(i));
		if (e != NULL && e->getcopye() == NULL) {
			Entity * copyEntity = e->clone(t);
			db.geometry.add(copyEntity);
			copyEntity->setRefId(copyEntity->getid());
			copy.append(copyEntity->getid());
		}
	}
	return copy;
}
copyAlongDialogData copyAlongDialogData::clone(Transform& t,bool pathIsCircle) const {
	copyAlongDialogData copy;
	copy.n = n;
	copy.spacingMode = spacingMode;
	copy.orientation = orientation;
	copy.spacing = spacing;
	
	copy.pt = pt;
	copy.dir= dir;
	copy.ustart = ustart;
	copy.list = cloneList(list, t);	
	
	copy.level = 1;
	copy.undoMap[1].pt = undoMap[level].pt;
	copy.undoMap[1].copyList = cloneList(undoMap[level].copyList, t);
	if(pathIsCircle)
		copy.undoMap[1].pt = t.transform(copy.undoMap[1].pt);

	return copy;
}

int copyAlongDialogData::saveList(CadFile* outfile,const QList<long>& list) {
	long count = list.count();
	if (!outfile->write(&count)) return 0;
	for (int i = 0; i < count; i++)
		if (!outfile->write(&list.at(i))) return 0;	
	return 1;
}
int copyAlongDialogData::loadList(CadFile* infile, QList<long>& list) {
	long count = 0;
	list.clear();
	if (!infile->read(&count)) return 0;
	long id = 0;
	for (int i = 0; i < count; i++) {
		if (!infile->read(&id)) return 0;
		list.append(id);
	}	
	return 1;
}
//
int copyAlongDialogData::save(CadFile* outfile) {
	
	if (!outfile->write(&n)) return 0;
	if (!outfile->write(&spacingMode)) return 0;
	if (!outfile->write(&orientation)) return 0;
	if (!outfile->write(&spacing)) return 0;
	if(!undoMap[level].pt.save(outfile))  return 0;
	if (!dir.save(outfile))  return 0;
	if (!outfile->write(&ustart)) return 0;
	if(!saveList(outfile, list)) return 0;
	if(!saveList(outfile, undoMap[level].copyList)) return 0;
	return 1;
}
//
int copyAlongDialogData::load(CadFile* infile) {
	if (!infile->read(&n)) return 0;
	if (!infile->read(&spacingMode)) return 0;
	if (!infile->read(&orientation)) return 0;
	if (!infile->read(&spacing)) return 0;
	if (!pt.load(infile))  return 0;
	if (!dir.load(infile))  return 0;
	if (!infile->read(&ustart)) return 0;	
	if (!loadList(infile,list)) return 0;
	if (!loadList(infile, copyList)) return 0;
	return 1;
}
void copyAlongDialogData::move(Transform &t, int erase) {
	undoMap[level].pt = t.transform(undoMap[level].pt);		
	for (int i = 0; i < list.length(); i++) {
		Entity* e = db.geometry.getentity(list.at(i));
		if (e != nullptr) {
			e->move(&db, t, erase);
		}
	}

}
Entity* copyAlongDialogData::getEntity(int refId)  {
	for (int i = 1; i < db.geometry.tablesize; i++) { // ;
		Entity* e = db.geometry.getentity(i);
		if (e != nullptr && e->getRefId() == refId) return e;
	}
	return 0;
}
