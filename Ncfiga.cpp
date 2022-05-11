
#include "ncwin.h"
#include "figure_dialog.h"
#include "RCFigureBrowser.h"

Figure::Figure(RCCHAR *l,RCCHAR *n,Point org,Point xa,Point ya,Point s,Point r,Point si,int nc,Byte s1,Byte s2,Transform *t,const BitMask &o) :
  options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    for(int i=0; i<2; i++)
        qhndl[i]=0;
#endif
    int i;
 EntityList emptylist;
 mextents=0;
  fstyle = -1;
  if ((filename = new RCCHAR[strlen(l)+1]) != NULL)
    {  strcpy(filename,l);
       if (o.test(1) && ! o.test(FigureEnableStates))
         {  for (i = strlen(filename) ; i >= 2 ; i--)
              //if (filename[i] == '\\')
              if (filename[i] == '/')
                {  fstyle = i - 2;
                   filename[fstyle] = '2';
                   break;
                }
         }
    }
  else
    {  defined = 0;  return;
    }
  if ((altfilename = new RCCHAR[strlen(n)+1]) != NULL)
    strcpy(altfilename,n);
  else
    {  delete [] filename;  defined = 0;  return;
    }
  state1 = s1;
  state2 = s2;
  options = o;  
  options.set(FigureState);  //  Save state1 and state2 in .cad file
  options.set(FigureParametric,0);  //  Not currently loaded
  /*
  if (t != NULL)
    transform = *t;
  else
  {
      if(options.test(FigureUseSize))
          updatesizetransform(org,xa,ya,si,r);
      else
          updatetransform(org,xa,ya,s,r);
  }
  */
  missing = 0;
  disabledisplaylist = 0;
  entitylist[0] = emptylist;
  entitylist[1] = emptylist;
  entitylist[2] = emptylist;
  
  attachment[0] = attachment[1] = attachment[2] = 0;
  attachmentlength[0] = attachmentlength[1] = attachmentlength[2] = 0;

  parentscale = 1.0;

  ncopies = nc;
  size = si;

  nclipboundary = 0;  
  clipboundary = 0;  

  if (t != NULL)
    transform = *t;
  else
  {
      if(options.test(FigureUseSize))
          updatesizetransform(org,xa,ya,si,r);
      else
          updatetransform(org,xa,ya,s,r);
  }

}

RCCHAR *Figure::getcurrentfilename(void)
{ if (options.test(FigureUseAlternativeName) && altfilename != 0 && strlen(altfilename) > 0)
    return altfilename;
  else
    return filename;
}

RCCHAR *Figure::getarchitectfilename(int i)
{ if (fstyle != -1)
    filename[fstyle] = i == 0 ? '2' : i == 1 ? '3' : 'S';
  return filename;
}

Figure::Figure(const EntityHeader &header,RCCHAR *l,RCCHAR *n,Point org,Point xa,Point ya,Point s,Point r,Point si,int nc,Byte s1,Byte s2,Transform *t,const BitMask &o) :
  SurfaceEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    for(int i=0; i<2; i++)
        qhndl[i]=0;
#endif
  int i;
  EntityList emptylist;
  mextents=0;
  fstyle = -1;
  if ((filename = new RCCHAR[strlen(l)+1]) != NULL)
    {  strcpy(filename,l);
       if (o.test(1) && ! o.test(FigureEnableStates))
         {  for (i = strlen(filename) ; i >= 2 ; i--)
              // if (filename[i] == '\\')
              if (filename[i] == '/')
                {  fstyle = i - 2;
                   filename[fstyle] = '2';
                   break;
                }
         }
    }
  else
    {  defined = 0;  return;
    }
  if ((altfilename = new RCCHAR[strlen(n)+1]) != NULL)
    strcpy(altfilename,n);
  else
    {  delete [] filename;  defined = 0;  return;
    }
  state1 = s1;
  state2 = s2;
  options = o;  options.set(FigureState);  //  Save state1 and state2 in .cad file
  options.set(FigureParametric,0);  //  Not currently loaded
  /*
  if (t != NULL)
    transform = *t;
  else
  {
      if(options.test(FigureUseSize))
          updatesizetransform(org,xa,ya,si,r);
      else
          updatetransform(org,xa,ya,s,r);
  }
  */
  missing = 0;
  disabledisplaylist = 0;
  entitylist[0] = emptylist;
  entitylist[1] = emptylist;
  entitylist[2] = emptylist;

  attachment[0] = attachment[1] = attachment[2] = 0;
  attachmentlength[0] = attachmentlength[1] = attachmentlength[2] = 0;

  parentscale = 1.0;

  ncopies = nc;
  size = si;

  nclipboundary = 0;  
  clipboundary = 0;  

  if (t != NULL)
    transform = *t;
  else
  {
      if(options.test(FigureUseSize))
          updatesizetransform(org,xa,ya,si,r);
      else
          updatetransform(org,xa,ya,s,r);
  }

}

// a static function to find the path the where the given figure is
// if the given filename is empty just finds the possible path
// if no path can be found returns an empty and false
bool Figure::ResolveLibraryPath(RCCHAR *pathorname, RCCHAR *foundpath, int sizeofpath)
{
    QString pathToFind; // somewhere to start looking
    QString fileToFind; // if there is a file name in the path
    QString inName(pathorname);
    QFileInfo fileinfo(inName);
    QDir testPath(inName);

    // is the input empty?
    // just get the current default figure path
    if(inName.isEmpty())
    {
        pathToFind = QString(home.getFigureFilepath(_RCT("/")));
        if(testPath.exists(pathToFind))
        {
            strncpy(foundpath,pathToFind.data(),(RCSIZE_T)sizeofpath);
            return true;
        }
        else
        {
            strcpy(foundpath,"");
            return false;
        }
    }
    // is the input a path only?
    if(fileinfo.fileName().isEmpty())
        pathToFind = inName;
    // is the input a file name with a path?
    else
    {
        pathToFind = fileinfo.path();
        fileToFind = fileinfo.fileName();
    }
    // see if the path to find exists
    if( testPath.exists() )
    {
        strncpy(foundpath,pathToFind.data(),(RCSIZE_T)sizeofpath);
        return true;
    }
    // try each historic figure path in order
    pathToFind = QString(home.getpublicfilename(_RCT("Figures")));
    if( testPath.exists(pathToFind) )
    {
        if( !fileToFind.isEmpty() )
        {
            QString fullPathToFind(pathToFind + testPath.separator() + fileToFind);
            if( testPath.exists(fullPathToFind) )
                strncpy(foundpath,fullPathToFind.data(),(RCSIZE_T)sizeofpath);
                return true;
        }
        strncpy(foundpath,pathToFind.data(),(RCSIZE_T)sizeofpath);
        return true;
    }
    pathToFind = QString(home.getpublicfilename(_RCT("Figures-Components")));
    if( testPath.exists(pathToFind) )
    {
        if( !fileToFind.isEmpty() )
        {
            QString fullPathToFind(pathToFind + testPath.separator() + fileToFind);
            if( testPath.exists(fullPathToFind) )
                strncpy(foundpath,fullPathToFind.data(),(RCSIZE_T)sizeofpath);
                return true;
        }
        strncpy(foundpath,pathToFind.data(),(RCSIZE_T)sizeofpath);
        return true;
    }
    pathToFind = QString(home.getFigureFilepath(_RCT("/")));
    if( testPath.exists(pathToFind) )
    {
        strncpy(foundpath,pathToFind.data(),(RCSIZE_T)sizeofpath);
        return true;
    }
    strcpy(foundpath,"");
    return false;
}

void Figure::fixupreferences(Entity ** index)
{ unpacked.fixupreferences(index);
  Entity::fixupreferences(index);
}

void Figure::dbinsert(Database *db,int draw)
{Entity *e;

  //  If the figure is not loaded, then load it
  if (! options.test(FigureParametric))
    {GeneralList list;
       loadfigure(&list);
    }

  for (unpacked.start() ; (e = unpacked.next()) != 0 ; )
    {  if (db != NULL && e->getposition() == NULL) db->geometry.add(e,draw);
       e->adda(this);
    }
  Entity::dbinsert(db,draw);
}

void Figure::dbdelete(void)
{Entity *e;
 EntityList list;
  for (unpacked.start() ; (e = unpacked.next()) != 0 ; )
    e->dela(this);
  Entity::dbdelete();
}

void Figure::dbsetassociated(void)
{Entity *e;
  for (unpacked.start() ; (e = unpacked.next()) != 0 ; )
    e->setstatus(1);
}

void Figure::dbtraverseassociated(EntityCallBack c,void *d)
{Entity *e;
  for (unpacked.start() ; (e = unpacked.next()) != 0 ; )
    c(e,d);
}

void GetAssociated(Entity *e,void *d)
{ ((EntityList *)d)->add(e);
}
 

Figure::~Figure()
{Entity *e;
 int i,found;
 EntityList associatedlist,elist;

  if (defined)
    {  if (alist.length() == 1)
         {  alist.start();
            e = alist.next();
#if (VERSION != FLEXICAD)
            if (e->isa(wall_entity))
              ((Wall *)e)->del(this);
#endif
         }

       for (unpacked.start() ; ! unpacked.atend() ; )
         {  if ((e = unpacked.next()) != 0 && e->getposition() == 0)
              delete e;
         }
       unpacked.destroy();

       for (i = 0 ; i < 3 ; i++)
         {  

            // Don't delete associated entities
            for (entitylist[i].start() ; ! entitylist[i].atend() ; )
              entitylist[i].next()->setposition((OneGeneral *)1);  

            for (;;)
              {  entitylist[i].start();
                 while ((e = entitylist[i].next()) != 0 && ! e->getalist().empty()) ;
                 if (e == 0) break;
                 e->dbdelete();
                 delete e;
                 entitylist[i].GeneralList::del(e);
              }

            entitylist[i].destroy();

         } 

       //  Attachments
       for (i = 0 ; i < 3 ; i++)
         if (attachmentlength[i] > 0)  
           {EntityList dblist;
              dblist = *db.geometry.getlist();
              found = 0; 
              for (dblist.start() ; (e = dblist.next()) != 0 ; )
                if (e != this && e->isa(figure_entity) && _stricmp(((Figure *)e)->getarchitectfilename(i),getarchitectfilename(i)) == 0 &&
                      ((Figure *)e)->attachment[i] == attachment[i] && ((Figure *)e)->attachmentlength[i] < 0)
                  {  ((Figure *)e)->attachmentlength[i] = abs(((Figure *)e)->attachmentlength[i]);  //  This figure now 'owns' the attachment.
                     found = 1;
                     break; 
                  }
              if (! found)
                delete [] attachment[i];
           } 

       delete [] filename;
       delete [] altfilename;
       
       delete [] mextents;
    }
}

void Figure::updatetransform(Point org,Point xa,Point ya,Point scale,Point rotation)
{Transform t;
  transform.scale(scale.x,scale.y,scale.z);
  t.rotatex(rotation.x * M_PI / 180.0);  transform.apply(t);
  t.rotatey(rotation.y * M_PI / 180.0);  transform.apply(t);
  t.rotatez(rotation.z * M_PI / 180.0);  transform.apply(t);
  t.tocoordinatesystem(xa,ya,xa.cross(ya));  transform.apply(t);
  t.translate(org); transform.apply(t);
}

void Figure::newtransform(Point org,Point xa,Point ya,Point scale,Point rotation)
{Transform t,tnew;
  tnew.scale(scale.x,scale.y,scale.z);
  t.rotatex(rotation.x * M_PI / 180.0);  tnew.apply(t);
  t.rotatey(rotation.y * M_PI / 180.0);  tnew.apply(t);
  t.rotatez(rotation.z * M_PI / 180.0);  tnew.apply(t);
  t.tocoordinatesystem(xa,ya,xa.cross(ya));  tnew.apply(t);
  t.translate(org); tnew.apply(t);
  transform.identity();
  transform.apply(tnew);
}

// for v8
void Figure::updatesizetransform(Point org,Point xa,Point ya,Point si,Point rotation)
{
  Transform t,t1,t2;
  Point ls,scale(1.0,1.0,1.0),pmin,pmax;

  size = si;
  options.set(FigureUseSize);


  if(mextents == 0)
      extents(0,&pmin,&pmax);
  else
  {
      pmin = mextents[0];
      pmax = mextents[1];
  }
  pmax = pmax - pmin;

  scale.x = ((size.x > 0.0 && pmax.x > 0.0) ? size.x / pmax.x : 1.0);
  scale.y = ((size.y > 0.0 && pmax.y > 0.0) ? size.y / pmax.y : 1.0);
  scale.z = ((size.z > 0.0 && pmax.z > 0.0) ? size.z / pmax.z : 1.0);

  transform.scale(scale.x,scale.y,scale.z);
  t.rotatex(rotation.x * M_PI / 180.0);  transform.apply(t);
  t.rotatey(rotation.y * M_PI / 180.0);  transform.apply(t);
  t.rotatez(rotation.z * M_PI / 180.0);  transform.apply(t);
  t.tocoordinatesystem(xa,ya,xa.cross(ya)); transform.apply(t);
  t.translate(org); transform.apply(t);
}

void Figure::newsizetransform(Point org,Point xa,Point ya,Point si,Point rotation)
{
    Transform t,t1,t2;
    Point ls,scale(1.0,1.0,1.0),pmin,pmax;

    size = si;
    options.set(FigureUseSize);

    if(mextents == 0)
        extents(0,&pmin,&pmax);
    else
    {
        pmin = mextents[0];
        pmax = mextents[1];
    }
    pmax = pmax - pmin;

    scale.x = ((size.x > 0.0 && pmax.x > 0.0) ? size.x / pmax.x : 1.0);
    scale.y = ((size.y > 0.0 && pmax.y > 0.0) ? size.y / pmax.y : 1.0);
    scale.z = ((size.z > 0.0 && pmax.z > 0.0) ? size.z / pmax.z : 1.0);

  transform.scale(scale.x,scale.y,scale.z);
  t.rotatex(rotation.x * M_PI / 180.0);  transform.apply(t);
  t.rotatey(rotation.y * M_PI / 180.0);  transform.apply(t);
  t.rotatez(rotation.z * M_PI / 180.0);  transform.apply(t);
  t.tocoordinatesystem(xa,ya,xa.cross(ya));  transform.apply(t);
  t.translate(org); transform.apply(t);
}
//

struct PickCBData
{int picktype,insidew,nvertices,picked;
 double d,x1,y1,x2,y2;
 Point *pnear,*pwvertices;
 Figure *figure;
};

static void PickFigureCallback(Entity *e,View3dSurface *vs,void *d)
{PickCBData *pcb = (PickCBData *) d;
 double pd;


  pd = e->pick(vs,pcb->x1,pcb->y1,0);

  if (pd >= 0.0 && (pcb->d < 0.0 || pd < pcb->d)) 
    {  pcb->d = pd;
       if (state.getcommandid() == 2004 && pcb->pnear != 0) //  Selecting midpoints, not endpoints
         {  //  Choose a midpoint position
            if (e->isa(line_entity))
              *pcb->pnear = pcb->figure->gettransform()->transform((((Line *)e)->getp1() + ((Line *)e)->getp2()) / 2.0);
            else if (e->isa(circle_entity))
              *pcb->pnear = pcb->figure->gettransform()->transform(((Circle *)e)->getorigin());
            else if (e->isa(ellipse_entity))
              *pcb->pnear = pcb->figure->gettransform()->transform(((EllipseE *)e)->getorigin());
            else if (e->isa(text_entity))
              *pcb->pnear = pcb->figure->gettransform()->transform(((Text *)e)->point(0));
            else if (e->isa(figure_entity))
              *pcb->pnear = pcb->figure->gettransform()->transform(((Figure *)e)->getorigin());
         }  
       else if (pcb->pnear != 0 && e->islinear())
         {  if (((LinearEntity *)e)->pickendpoint(vs,0,(int)pcb->x1,(int)pcb->y1,pcb->pnear))
              *pcb->pnear = pcb->figure->gettransform()->transform(*pcb->pnear);
         }  
    }

}

double Figure::pick(View3dSurface *v,double x,double y,Point *pnear)
{PickCBData pickcbdata;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(v);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    v = window->getoffscreenbitmap();

  if (missing)
    return -1.0;

  if (pnear == NULL && v->displaylist.defined(this))
    return v->displaylist.pick(this,x,y);

  pickcbdata.x1 = x;
  pickcbdata.y1 = y;
  pickcbdata.pnear = pnear;
  pickcbdata.d = -1.0;
  pickcbdata.figure = this;

  if (pnear != 0)
    *pnear = getorigin();

  ProcessFigure(v,0,PickFigureCallback,&pickcbdata);
 
  return pickcbdata.d;
}

void Figure::SetupMatrix(View3dSurface *vs)
{Point scale,origin,xa,ya,za(0.0,0.0,1.0);

  if (vs == 0)
    return;

  if (options.test(FigureOrientXYToView))
    {//  If this is an Orient To XY View figure, modify the transform to suit the view.
       scale.setxyz(sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0)*transform.mat(1,0) + transform.mat(2,0)*transform.mat(2,0)),
                    sqrt(transform.mat(0,1)*transform.mat(0,1) + transform.mat(1,1)*transform.mat(1,1) + transform.mat(2,1)*transform.mat(2,1)),
                    sqrt(transform.mat(0,2)*transform.mat(0,2) + transform.mat(1,2)*transform.mat(1,2) + transform.mat(2,2)*transform.mat(2,2)));
       origin = getorigin();
       transform.identity();
       if (vs->getwaxis().z >= 0.99999)
         {//  Always use a standard matrix (with appropriate scales).
            transform.mat(0,0) = scale.x;        
            transform.mat(1,1) = 0.0;        
            transform.mat(2,1) = scale.z;        
            transform.mat(1,2) = -scale.y;        
            transform.mat(2,2) = 0.0;        
            transform.mat(0,3) = origin.x;
            transform.mat(1,3) = origin.y;
            transform.mat(2,3) = origin.z;
         }
       else
         {//  Rotate the figure to face the view.
            ya = za.cross(vs->getuaxis()).normalize() * scale.y;
            xa = ya.cross(za).normalize() * scale.x;
            za *= scale.z;
            transform.mat(0,0) = xa.x;
            transform.mat(1,0) = xa.y;
            transform.mat(2,0) = xa.z;
            transform.mat(0,2) = -ya.x;
            transform.mat(1,2) = -ya.y;
            transform.mat(2,2) = -ya.z;
            transform.mat(0,1) = za.x;
            transform.mat(1,1) = za.y;
            transform.mat(2,1) = za.z;
            transform.mat(0,3) = origin.x;
            transform.mat(1,3) = origin.y;
            transform.mat(2,3) = origin.z;
         }
    }     
  else if (options.test(FigureOrientToView))
    {//  If this is an Orient To View figure, modify the transform to suit the view.
       scale.setxyz(sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0)*transform.mat(1,0) + transform.mat(2,0)*transform.mat(2,0)),
                    sqrt(transform.mat(0,1)*transform.mat(0,1) + transform.mat(1,1)*transform.mat(1,1) + transform.mat(2,1)*transform.mat(2,1)),
                    sqrt(transform.mat(0,2)*transform.mat(0,2) + transform.mat(1,2)*transform.mat(1,2) + transform.mat(2,2)*transform.mat(2,2)));
       origin = getorigin();
       transform.identity();
       if (vs->getwaxis().z >= 0.99999)
         {//  Always use a standard matrix (with appropriate scales).
            transform.mat(0,0) = scale.x;        
            transform.mat(1,1) = scale.y;        
            transform.mat(2,2) = scale.z;        
            transform.mat(0,3) = origin.x;
            transform.mat(1,3) = origin.y;
            transform.mat(2,3) = origin.z;
         }
       else
         {//  Rotate the figure to face the view.
            ya = za.cross(vs->getuaxis()).normalize() * scale.y;
            xa = ya.cross(za).normalize() * scale.x;
            za *= scale.z;
            transform.mat(0,0) = xa.x;
            transform.mat(1,0) = xa.y;
            transform.mat(2,0) = xa.z;
            transform.mat(0,1) = ya.x;
            transform.mat(1,1) = ya.y;
            transform.mat(2,1) = ya.z;
            transform.mat(0,2) = za.x;
            transform.mat(1,2) = za.y;
            transform.mat(2,2) = za.z;
            transform.mat(0,3) = origin.x;
            transform.mat(1,3) = origin.y;
            transform.mat(2,3) = origin.z;
         }
    }     


}

static void PickWFigureCallback(Entity *e,View3dSurface *vs,void *d)
{PickCBData *pcb = (PickCBData *) d;

  if (! e->pick(vs,pcb->insidew,(int)pcb->x1,(int)pcb->y1,(int)pcb->x2,(int)pcb->y2))
    pcb->picked = 0;

}



int Figure::pick(View3dSurface *vs,int insidew,int x1,int y1,int x2,int y2)
{PickCBData pickcbdata;

 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (missing)
    return ! insidew;
  if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,insidew,x1,y1,x2,y2);

  pickcbdata.insidew = insidew;
  pickcbdata.x1 = x1;
  pickcbdata.y1 = y1;
  pickcbdata.x2 = x2;
  pickcbdata.y2 = y2;
  pickcbdata.picked = 1;

  ProcessFigure(vs,0,PickWFigureCallback,&pickcbdata);

  return pickcbdata.picked;
}


static void PickPWFigureCallback(Entity *e,View3dSurface *vs,void *d)
{PickCBData *pcb = (PickCBData *) d;
 int epicked;

  epicked = e->pick(vs,pcb->nvertices,pcb->pwvertices,pcb->insidew);
  if ((pcb->insidew == 0 || pcb->insidew == 1) && epicked == 0 || pcb->insidew == 2 && epicked == 1)
    pcb->picked = 0;

}

int Figure::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{PickCBData pickcbdata;

 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (missing)
    return 0;

  pickcbdata.picked = inside != 2;
  pickcbdata.insidew = inside;
  pickcbdata.nvertices = nvertices;
  pickcbdata.pwvertices = pwvertices;
  ProcessFigure(vs,0,PickPWFigureCallback,&pickcbdata);

  return pickcbdata.picked;
}

struct DisplayListCBData
{int *disableddisplaylist;
};

static void DisplayListFigureCallback(Entity *e,View3dSurface *vs,void *d)
{DisplayListCBData *dlcb = (DisplayListCBData *) d;

  vs->displaylist.changecolour(e->getcolour());
  vs->displaylist.changestyle(e->getlinestylescale(),e->getstyle());
  vs->displaylist.changeweight(e->getweight());

  // KMJ : disabling the display list for all plane fill types
  //if (e->isa(bitmap_entity) || e->isa(figure_entity) || e->isa(plane_entity) && (((Plane *)e)->getoptions().test(PlaneGradientFilled) || ((Plane *)e)->getoptions().test(PlaneBitmapFilled) || ((Plane *)e)->getoptions().test(PlaneFilled) && ((Plane *)e)->gettransparency() != 0.0))
  if (e->isa(text_entity) || e->isa(bitmap_entity) || e->isa(figure_entity) || e->isa(plane_entity))
    *dlcb->disableddisplaylist = 1;
  e->addtodisplaylist(vs,1);
}


void Figure::addtodisplaylist(View3dSurface *vs,int child)
{DisplayListCBData displaylistcbdata;
 int i,size;

  if (missing)
    return;

  if (! child && getposition() == NULL) return;

  for (i = child,size = 0 ; i < 2 ; i++)
    {  if (! child)
         {  if (i == 0)
              vs->displaylist.setcountmode();
            else
              vs->displaylist.setsavemode();
            if (! vs->displaylist.start(this,size)) return;
         }

       displaylistcbdata.disableddisplaylist = & disabledisplaylist;
       ProcessFigure(vs,0,DisplayListFigureCallback,&displaylistcbdata);

       size = vs->displaylist.getcount();

    }

  if (! child && ! vs->displaylist.end()) return;

  if (disabledisplaylist)
    vs->displaylist.del(this);

}



struct DrawCBData
{
#ifdef USING_WIDGETS
    QGraphicsItemGroup *qgi;
    Transform *t;
#endif
    Figure *fig;
    int mode;
};

static void DrawFigureCallback(Entity *e,View3dSurface *vs,void *d)
{
  DrawCBData *dcb = (DrawCBData *) d;

  if (program->getinterrupt())
       return;

  e->draw(dcb->mode,0,vs);

  if(dcb->qgi)
    if(e->qgi)
    {
        //vs->getScene()->removeItem(e->qgi);
        ((QGraphicsItemGroup*)dcb->qgi)->addToGroup(e->qgi);
        e->qgi = 0;
    }

  if(dcb->mode != DM_PRINT)
  {
      if(e->qgi && e->qgi->scene() == vs->getScene())
          e->qgi->scene()->removeItem(e->qgi);
      delete e->qgi;
      e->qgi=0;
  }


  if(e->isa(plane_entity))
       ((Plane*)e)->qgrenderplane=0;
}

#ifdef USING_WIDGETS
void Figure::drawGLlod(Transform *t,View3dSurface *vs)
{
    // draw an the lod of the figure
    // a rectangle from the bounding box
    if(mextents == 0)
        return;

    Point pts[4];
    pts[0] = mextents[0];
    pts[1] = Point(mextents[1].x,mextents[0].y,mextents[0].z);
    pts[2] = mextents[1];
    pts[3] = Point(mextents[0].x,mextents[1].y,mextents[1].z);
    Polygon3d polygon3d(pts[0],pts[1],pts[2],pts[3]);

    vs->setupGL(linestylescale,0.0,DM_SCROLLING,colour,style,weight,0);

    RCCOLORREF pcol;
    pcol = vs->getcolour();
    GLfloat fr=GetRValue(pcol)/255.0;
    GLfloat fg=GetGValue(pcol)/255.0;
    GLfloat fb=GetBValue(pcol)/255.0;
    glColor3f(fr,fg,fb);
    GLfloat lwidth = vs->curPen.widthF();
    glLineWidth(vs->curGLWeight);
    SetNoMaterial(fr,fg,fb);

    if((mextents[0] - mextents[1]).length() > 0.00001)
        polygon3d.drawGL(DM_NORMAL,t,vs);
}

static void DrawFigureCallbackGL(Entity *e,View3dSurface *vs,void *d)
{
  DrawCBData *dcb = (DrawCBData *) d;

  if(e->type() == text_entity)
      e->drawGL(dcb->mode,0,vs);
  else
     e->drawGL(dcb->mode,dcb->t,vs);
}
#endif

void Figure::ProcessFigureEntity(int arch,double fscale,const BitMask *layertable,Entity *e,View3dSurface *vs,FigureCallback fcb,void *d)
{int layer;

  //qDebug() << "in ProcessFigureEntity";

  if (arch || options.test(FigureOriginalLayer))
    layer = e->getlayer();
  else
    layer = getlayer();  

  //qDebug() << "in ProcessFigureEntity";
  //qDebug() << "arch : " << arch;
  //qDebug() << "layer : " << layer;
  //qDebug() << "layer + state1 : " << layer + state1;
  //qDebug() << "layer + state2 : " << layer + state2;
  //qDebug() << "getvisible->test(0) : " << e->getvisible()->test(0);
  //qDebug() << "layertable->test(layer) : " << layertable->test(layer);

  if ((!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && e->getvisible()->test(0) && layertable->test(layer))
    {
       e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
       if (vs != 0)
         e->getvisible()->set(vs->getview()->getviewno());
       //qDebug() << "is a text entity : " << e->isa(text_entity);
       //qDebug() << "text label       : " << ((Text *)e)->getlabel();
       //if(e->isa(figure_entity))
       //{
       //     qDebug() << "is a figure entity";
       //     qDebug() << "figure name : " << QString(((Figure*)e)->getfilename());
       //}

       if(program->getinterrupt())
           return;

       if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) &&
           (! e->isa(lineardimension_entity) || strlen(((Dimension *)e)->getequation()) == 0))
         {
              {  if (! options.test(FigureOriginalColour))
                   {
                      if (e->isa(figure_entity))
                          ((Figure *)e)->getoptions()->clear(FigureOriginalColour);
                      e->EntityHeader::change(getcolour(),e->getlayer(),e->getstyle(),e->getweight());
                   } 
                 if (! options.test(FigureOriginalStyle))
                   {
                      if (e->isa(figure_entity))
                         ((Figure *)e)->getoptions()->clear(FigureOriginalStyle);
                      e->EntityHeader::change(e->getcolour(),e->getlayer(),getstyle(),e->getweight());
                   } 
                 if (! options.test(FigureOriginalWeight))
                   {
                      if (e->isa(figure_entity))
                          ((Figure *)e)->getoptions()->clear(FigureOriginalWeight);
                      e->EntityHeader::change(e->getcolour(),e->getlayer(),e->getstyle(),getweight());
                   } 
              }

            if (e->isa(figure_entity))
                ((Figure *)e)->parentscale = fscale;

            if(e->isa(bitmap_entity))
                ((BitMapE*)e)->usedByFigure=true;

            if (e->isa(text_entity) && ! ((Text *)e)->getoptions().test(9) && fscale > 0.0)
              {  ((Text *)e)->ScaleTextHeight(fscale);
                 fcb(e,vs,d);
                 ((Text *)e)->ScaleTextHeight(1.0/fscale);
              }
            else
              fcb(e,vs,d);

            program->tickprogressup();

         }
    }
  //qDebug() << "leaving ProcessFigureEntity";
}

void Figure::ProcessFigure(View3dSurface *vs,Transform *t,FigureCallback fcb,void *d)
{CadFile infile;
 Entity *e;
 EntityList list;
 Transform tview;
 int arch,index,i;
 Point pt1,pt2;
 double fscale;
 Point scale,origin,xa,ya,za(0.0,0.0,1.0);
 Transform lastmodeltransform;
 BitMask *layertable;
 RCCHAR text[600];

  //qDebug() << "in ProcessFigure";

  SetupMatrix(vs);
  QString dmpName(this->getfilename());
  //dmpName += "-figTransf";
  //transform.dump(dmpName.toLatin1().constData());

  layertable = vs->getdisplaylayertable() != 0 ? vs->getdisplaylayertable() : & db.dismask.layer;
  fscale = parentscale * sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0)*transform.mat(1,0) + transform.mat(2,0)*transform.mat(2,0));
  arch = options.test(1);
  index = options.test(1) && (vs->getwaxis().z <= 0.99999);

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //    qDebug() << "view T : mat[" << im << "]" << vs->modeltransform.mat.elements[im];
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(vs->modeltransform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "vs->modeltransform";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

  vs->gettransform(&tview);
  if (t != NULL)
    vs->applytransform(*t);

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //    qDebug() << "figure T : mat[" << im << "]" << transform.mat.elements[im];
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(transform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "figure transform";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

  vs->applytransform(transform);
  lastmodeltransform = vs->modeltransform;
  vs->modeltransform.apply(transform);

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //  qDebug() << "view T : mat[" << im << "]" << vs->modeltransform.mat.elements[im];

  //dmpName = QString(this->getfilename()) + "-vsmodTransf";
  //vs->modeltransform.dump(dmpName.toLatin1().constData());
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(vs->modeltransform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "vs->modeltransform.apply(transform)";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

  if (options.test(FigureElevation))
    {  //  Rotate the figure and draw it again
        Transform t;
        t.rotatex(acos(-1.0) / 2.0);
        vs->applytransform(t);
        t.apply(vs->modeltransform);
        vs->modeltransform = t;
    }    

  vs->setclipplanesvalid(0);
            
  for (i = 0 ; i == 0 || i < ncopies * options.test(FigureElevation) ; i++)
    {
      if (program->getinterrupt())
           break;

      if (options.test(FigureParametric))
         {  for (entitylist[index].end() ; (e = entitylist[index].last()) != NULL ; )
            {
                if(program->getinterrupt())
                    break;
                vs->setclipplanesvalid(0);
                ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
                if (disabledisplaylist && e->isa(figure_entity))
                    ((Figure *)e)->disabledisplaylist = disabledisplaylist;
            }
         }
       else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
         {  for (list.end() ; (e = list.last()) != NULL ; )
              ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
         }
       else
         {  if (arch && fstyle != -1)
              filename[fstyle] = RCCHAR('2' + (vs->getwaxis().z <= 0.99999));
            if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(getcurrentfilename(),"rb"))
              {  missing = 1;  break;
              } 
            if (infile.readheader())
              {  while ((e = infile.nextentity(NULL)) != NULL)
                   {
                      if(program->getinterrupt())
                          break;
                      ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
                      delete e;
                   }
                 infile.close();
              }
            else
              {  infile.close();
                 strcpy(text,filename);
                 _strlwr(text);
                 if (strstr(text,".cad") == 0)
                   {//  Insert a bitmap 
                     BitMask o(32);
                     //o.set(BitmapESameShape,1);
                     o.set(BitmapELinkFile,1);
                     o.set(BitmapETransparent,1);
                     BitMapE bitmap(filename,Point(-1.0,-1.0+options.test(FigureElevation),0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),2.0,2.0-options.test(FigureElevation),1.0,1.0,1.0,0.1,1.0,o);
                     ProcessFigureEntity(arch,fscale,layertable,&bitmap,vs,fcb,d);
                   }
              }  
         }

       if (ncopies > 1)
         {  //  Rotate the figure and draw it again
             Transform t;
             t.rotatey(acos(-1.0) / ncopies);    
             vs->applytransform(t);
             t.apply(vs->modeltransform);
             vs->modeltransform = t; 
         }    

    }

  vs->modeltransform = lastmodeltransform;
  vs->settransform(tview);
  vs->setclipplanesvalid(1);

}

void Figure::ProcessFigureGL(View3dSurface *vs,Transform *t,FigureCallback fcb,void *d)
{CadFile infile;
 Entity *e;
 EntityList list;
 Transform tview;
 int arch,index,i;
 Point pt1,pt2;
 double fscale;
 Point scale,origin,xa,ya,za(0.0,0.0,1.0);
 Transform lastmodeltransform;
 BitMask *layertable;
 RCCHAR text[600];

 //qDebug() << "in ProcessFigureGL";

 SetupMatrix(vs);

#if 0
  QString dmpName(this->getfilename());
  dmpName += "-figTransf";
  transform.dump(dmpName.toLatin1().constData());
#endif

  layertable = vs->getdisplaylayertable() != 0 ? vs->getdisplaylayertable() : & db.dismask.layer;
  fscale = parentscale * sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0)*transform.mat(1,0) + transform.mat(2,0)*transform.mat(2,0));
  arch = options.test(1);
  index = options.test(1) && (vs->getwaxis().z <= 0.99999);

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //    qDebug() << "view T : mat[" << im << "]" << vs->modeltransform.mat.elements[im];
  Point mtrans, mrot, mscale;
  Transform tv;
  vs->gettransform(&tv);
  Transform::DecomposeTransformationMatrix(tv,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "vs->transform";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //    qDebug() << "view T : mat[" << im << "]" << vs->modeltransform.mat.elements[im];
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(vs->modeltransform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "vs->modeltransform";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

  vs->gettransform(&tview);
  if (t != NULL)
    vs->applytransform(*t);

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //    qDebug() << "figure T : mat[" << im << "]" << transform.mat.elements[im];
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(transform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "figure transform";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

#ifndef _RCDEBUG
  Transform t1,t2,t3,t4;
  t1 = t1.translate(10,10,10);
  t2 = t2.translate(100,100,100);
  t3 = t3.rotatez(0.475);
  t4 = t1 * t2 * t3;
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(t4,mtrans,mrot,mscale);
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
#endif

  vs->applytransform(transform);
  lastmodeltransform = vs->modeltransform;
  //vs->modeltransform.apply(transform);
  vs->modeltransform = vs->modeltransform * transform;

#ifndef _RCDEBUG
{
  //for(int im=0; im<4; im++)
  //  qDebug() << "view T : mat[" << im << "]" << vs->modeltransform.mat.elements[im];

  //dmpName = QString(this->getfilename()) + "-vsmodTransf";
  //vs->modeltransform.dump(dmpName.toLatin1().constData());
  Point mtrans, mrot, mscale;
  Transform::DecomposeTransformationMatrix(vs->modeltransform,mtrans,mrot,mscale);
  qDebug() << "==================";
  qDebug() << "figure name : " <<  QString(this->getfilename());
  qDebug() << "vs->modeltransform.apply(transform)";
  qDebug() << "trans  x :" << mtrans.x << "y : " << mtrans.y << "z : " << mtrans.z;
  qDebug() << "rotate x :" << mrot.x << "y : " << mrot.y << "z : " << mrot.z;
  qDebug() << "scale  x :" << mscale.x << "y : " << mscale.y << "z : " << mscale.z;
  qDebug() << "==================";
}
#endif

  if (options.test(FigureElevation))
    {  //  Rotate the figure and draw it again
        Transform t;
        t.rotatex(acos(-1.0) / 2.0);
        vs->applytransform(t);
        t.apply(vs->modeltransform);
        vs->modeltransform = t;
    }

  vs->setclipplanesvalid(0);

  for (i = 0 ; i == 0 || i < ncopies * options.test(FigureElevation) ; i++)
    {  if (options.test(FigureParametric))
         {  for (entitylist[index].end() ; (e = entitylist[index].last()) != NULL ; )
            {   vs->setclipplanesvalid(0);
                ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
                if (disabledisplaylist && e->isa(figure_entity))
                    ((Figure *)e)->disabledisplaylist = disabledisplaylist;
            }
         }
       else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
         {  for (list.end() ; (e = list.last()) != NULL ; )
              ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
         }
       else
         {  if (arch && fstyle != -1)
              filename[fstyle] = RCCHAR('2' + (vs->getwaxis().z <= 0.99999));
            if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(getcurrentfilename(),"rb"))
              {  missing = 1;  break;
              }
            if (infile.readheader())
              {  while ((e = infile.nextentity(NULL)) != NULL)
                   {  ProcessFigureEntity(arch,fscale,layertable,e,vs,fcb,d);
                      delete e;
                   }
                 infile.close();
              }
            else
              {  infile.close();
                 strcpy(text,filename);
                 _strlwr(text);
                 if (strstr(text,".cad") == 0)
                   {//  Insert a bitmap
                     BitMask o(32);
                     //o.set(BitmapESameShape,1);
                     o.set(BitmapELinkFile,1);
                     o.set(BitmapETransparent,1);
                     BitMapE bitmap(filename,Point(-1.0,-1.0+options.test(FigureElevation),0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),2.0,2.0-options.test(FigureElevation),1.0,1.0,1.0,0.1,1.0,o);
                     ProcessFigureEntity(arch,fscale,layertable,&bitmap,vs,fcb,d);
                   }
              }
         }

       if (ncopies > 1)
         {  //  Rotate the figure and draw it again
             Transform t;
             t.rotatey(acos(-1.0) / ncopies);
             vs->applytransform(t);
             t.apply(vs->modeltransform);
             vs->modeltransform = t;
         }

    }

  vs->modeltransform = lastmodeltransform;
  vs->settransform(tview);
  vs->setclipplanesvalid(1);

  //qDebug() << "leaving ProcessFigureGL";
}

#ifdef USING_WIDGETS
void Figure::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
 DrawCBData drawcbdata;
 RCHRGN clipregion;

  if (missing)
    return;

  //qDebug() << "in figure drawGL";
  //qDebug() << "figure name : " << QString(this->filename);

  draw(DM_ERASE,t,vs);

  // bounding box check against the view
  // see the extent function for details
  Point bmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),bmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
  //qDebug() << "getting GL extents for a figure" << QString(filename);
  extents(0,&bmin,&bmax);
  // ignore if BB is insane??
  //if(bmin.x <= -RCDBL_MAX+0.9e38 || bmin.y <= -RCDBL_MAX+0.9e38 ||
  //   bmax.x >=  RCDBL_MAX-0.9e38 || bmin.y >= RCDBL_MAX-0.9e38 )
  //    return;
  bmin = vs->modeltoscreen(bmin);
  bmax = vs->modeltoscreen(bmax);
  if(bmax.x < 0 || bmax.y < 0 || bmin.x > vs->getwidth() || bmin.y > vs->getheight())
      return;

  clipregion = 0;

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

  vs->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

#if 0
  if (! disabledisplaylist && t == NULL && nclipboundary == 0 && ! gethandles())
  {
       if (vs->displaylist.draw(this,drawmode))
       {
           if(qgi)
               vs->getScene()->addItem(qgi);
           return;
       }
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode))
       {
           if(qgi)
               vs->getScene()->addItem(qgi);
           return;
       }
  }
#endif

#if 0
  if (nclipboundary != 0)
    {//  Set the clip boundary
       Point *v = new Point[nclipboundary];
       Point **cb = new Point *[1];
       cb[0] = v;
       memcpy(v,clipboundary,nclipboundary * sizeof(Point));

       int *nc = new int[1];
       nc[0] = nclipboundary;

       Polygon3d polygon3d(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),1,nc,cb);
       clipregion = CreateRectRgn(0,0,1,1);
       if (GetClipRgn(vs->gethdc(),clipregion) <= 0)
         {  DeleteObject(clipregion);
            clipregion = 0;
         }

       polygon3d.setclipregion(0,vs);
    }
#endif
  drawcbdata.mode = drawmode;
  drawcbdata.t = t;
  //drawcbdata.qgi = (QGraphicsItemGroup*)qgi;
  //Point pmin(RCREAL_MAX,RCREAL_MAX,RCREAL_MAX),pmax(-RCREAL_MAX,-RCREAL_MAX,-RCREAL_MAX);
  Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
#if 1
  if(drawmode == DM_SCROLLING)
  {
      if(mextents == 0)
      {
          mextents = new Point[2];
          Point pts[2];
          extents(0,&pmin,&pmax);
      }
      drawGLlod(t,vs);
  }
  else
#endif

    ProcessFigureGL(vs,t,DrawFigureCallbackGL,&drawcbdata);
    //ProcessFigure(vs,t,DrawFigureCallbackGL,&drawcbdata);

#if 0
  if(drawcbdata.qgi)
  {
      qgi = drawcbdata.qgi;
      vs->getScene()->addItem(qgi);
      drawcbdata.qgi=0;
  }
#endif
#if 0
  if (nclipboundary != 0)
  {
      SelectClipRgn(vs->gethdc(),clipregion);
      DeleteObject(clipregion);
  }
#endif

  if (gethandles())
  {
       Transform original,ident;
       Point pt1,pt2;

       original = transform;
       transform = ident;
       extents(0,&pmin,&pmax);
       transform = original;

       vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
       //vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,0,0,0,0);

       RCCOLORREF pcol;
       pcol = vs->getcolour();
       //GLfloat fr=GetRValue(pcol)/255.0;
       //GLfloat fg=GetGValue(pcol)/255.0;
       //GLfloat fb=GetBValue(pcol)/255.0;
       //glColor3f(fr,fg,fb);
       glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
       GLfloat lwidth = vs->curPen.widthF();
       //glLineWidth(lwidth<1.0?1.0:lwidth);
       glLineWidth(vs->curGLWeight);

       Point pright = vs->getuaxis();
       Point pup = vs->getvaxis();
       double sc = vs->getpixelscale() * 6.0;

       pt1 = getorigin();
       vs->cbox2dGL(pt1,pright,pup,sc,sc);

       pt2 = transform.transform(Point(pmax.x,0.0,0.0));
       vs->cbox2dGL(pt2,pright,pup,sc,sc);
  }
}
#endif

void Figure::draw(int drawmode,Transform *t,View3dSurface *vs)
{
 DrawCBData drawcbdata;
 RCHRGN clipregion;

  if (missing)
    return;

  //qDebug() << "in figure draw";
  //qDebug() << "figure name : " << QString(this->filename);

  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() != vs->getScene())
          qgi->scene()->removeItem(qgi);
      else if(qgi && vs->getScene())
          vs->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;
      drawcbdata.qgi=0;

      for(int ih=0; ih<2; ih++)
      {
          if(qhndl[ih] && qhndl[ih]->scene() == vs->getScene())
              qhndl[ih]->scene()->removeItem(qhndl[ih]);
              //vs->getScene()->removeItem(qhndl[ih]);
          delete qhndl[ih];
          qhndl[ih]=0;
      }

      //if(dynamic_cast<View3dSubSurface*>(vs) != 0)
      //    ((View3dSubSurface*)vs)->qgicp=0;
  }

  //qDebug() << "entering Figure::draw";

  if(drawmode == DM_ERASE)
  {
      // KMJ: should we clear the children first

      drawcbdata.fig = this;
      drawcbdata.mode = drawmode;
      drawcbdata.t = 0;
      drawcbdata.qgi = (QGraphicsItemGroup*)qgi;

      ProcessFigure(vs,t,DrawFigureCallback,&drawcbdata);

      return;
  }

  // bounding box check against the view
  // see the extent function for details
  Point bmin,bmax;
  extents(0,&bmin,&bmax);
  bmin = vs->modeltoscreen(bmin);
  bmax = vs->modeltoscreen(bmax);
  if(bmax.x < 0 || bmax.y < 0 || bmin.x > vs->getwidth() || bmin.y > vs->getheight())
      return;
  // end of bounding box test

  qgi = new QGraphicsItemGroup();

  clipregion = 0;

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

#if 0
  if (! disabledisplaylist && t == NULL && nclipboundary == 0 && ! gethandles())
  {
       if (vs->displaylist.draw(this,drawmode))
       {
           if(qgi)
               vs->getScene()->addItem(qgi);
           return;
       }
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode))
       {
           if(qgi)
               vs->getScene()->addItem(qgi);
           return;
       }
  }
#endif

#if 0
  if (nclipboundary != 0)
    {//  Set the clip boundary
       Point *v = new Point[nclipboundary];
       Point **cb = new Point *[1];
       cb[0] = v;
       memcpy(v,clipboundary,nclipboundary * sizeof(Point));

       int *nc = new int[1];
       nc[0] = nclipboundary;

       Polygon3d polygon3d(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),1,nc,cb);
       clipregion = CreateRectRgn(0,0,1,1);
       if (GetClipRgn(vs->gethdc(),clipregion) <= 0)
         {  DeleteObject(clipregion);
            clipregion = 0;
         }

       polygon3d.setclipregion(0,vs);

    }
#endif

  drawcbdata.fig = this;
  drawcbdata.mode = drawmode;
  drawcbdata.t = 0;
  drawcbdata.qgi = (QGraphicsItemGroup*)qgi;

  ProcessFigure(vs,t,DrawFigureCallback,&drawcbdata);

  if(drawcbdata.qgi)
  {
      qgi = drawcbdata.qgi;
      vs->getScene()->addItem(qgi);
      drawcbdata.qgi=0;
#if 0
      if(drawmode == DM_INVERT)
      {
          if(vs->qgirb == 0)
          {
              vs->qgirb = new QGraphicsItemGroup();
              vs->getScene()->addItem(vs->qgirb);
              ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
          }
          else
              ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);

          qgi=0;
      }
      else
#endif
      if(qgi && dynamic_cast<View3dSubSurface*>(vs) != 0)
      {
          //((View3dSubSurface*)vs)->qgicp=0;
          //if(qgi->parentItem() != ((View3dSubSurface*)vs)->qgicp)
          //qDebug() << "parent item : " << qgi->parentItem();
          //qDebug() << "qgicp : " << ((View3dSubSurface*)vs)->qgicp;
          if(((View3dSubSurface*)vs)->qgicp != 0 &&
             ((View3dSubSurface*)vs)->qgicp != qgi->parentItem())
            qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);
      }
  }

#if 0
  if (nclipboundary != 0)
  {
      SelectClipRgn(vs->gethdc(),clipregion);
      DeleteObject(clipregion);
  }
#endif

  if (gethandles())
  {
       QGraphicsItem *qline;
       QList<QGraphicsItem *> glist;

       Transform original,ident;
       Point pmin,pmax,pt1,pt2;

       original = transform;
       transform = ident;
       extents(0,&pmin,&pmax);
       transform = original;

       vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

       if(qhndl[0]) vs->getScene()->removeItem(qhndl[0]);
       delete qhndl[0];
       qhndl[0] = 0;

       pt1 = vs->modeltoscreen(getorigin());
       qline = vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);if(qline) glist.append(qline);
       qline = vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);if(qline) glist.append(qline);
       qline = vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);if(qline) glist.append(qline);
       qline = vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);if(qline) glist.append(qline);

       if(glist.size() > 0)
           qhndl[0] = vs->getScene()->createItemGroup(glist);
       if(qhndl[0] && getselected())
           qhndl[0]->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
       glist.clear();

       if(qhndl[1]) vs->getScene()->removeItem(qhndl[1]);
       delete qhndl[1];
       qhndl[1] = 0;

       pt2 = vs->modeltoscreen(transform.transform(Point(pmax.x,0.0,0.0)));
       qline = vs->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2);if(qline) glist.append(qline);
       qline = vs->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2);if(qline) glist.append(qline);
       qline = vs->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2);if(qline) glist.append(qline);
       qline = vs->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2);if(qline) glist.append(qline);

       if(glist.size() > 0)
           qhndl[1] = vs->getScene()->createItemGroup(glist);
       if(qhndl[1] && getselected())
           qhndl[1]->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
       glist.clear();
  }
  if(qgi && drawmode == DM_PRINT)
      qgi->setZValue(0);
  //if(qgi)
  //    qDebug() << "Figure ZValue : " << qgi->zValue();

  //qDebug() << "leaving figure draw";

}

struct HideCBData
{
    QGraphicsItem *qgi;
    HideImage *image;
};

static void HideFigureCallback(Entity *e,View3dSurface *vs,void *d)
{
    HideCBData *hcb = (HideCBData *) d;

    e->hide(hcb->image);

    if(hcb->image->qgi)
      if(e->qgi)
      {
          ((QGraphicsItemGroup*)hcb->image->qgi)->addToGroup(e->qgi);
          e->qgi = 0;
      }

    if(e->qgi)
        hcb->image->getsurface()->getScene()->removeItem(e->qgi);
    delete e->qgi;
    e->qgi=0;

}

int Figure::hide(HideImage *image)
{HideCBData hidecbdata;
 Transform timage;

  if (missing)
    return 1;

  if(image->drawmode != 1)// printing
  {
      if(qgi)
          if(image->getsurface()->getScene())
              image->getsurface()->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;
  }
  hidecbdata.qgi=0;

  qgi = new QGraphicsItemGroup();

  image->qgi = qgi;

  hidecbdata.image = image;

  image->gettransform(&timage);
  image->applytransform(transform);
  image->getsurface()->setclipplanesvalid(0);

  ProcessFigure(image->getsurface(),0,HideFigureCallback,&hidecbdata);

  image->settransform(timage);
  image->getsurface()->setclipplanesvalid(1);

  if(hidecbdata.image->qgi)
  {
      qgi = hidecbdata.image->qgi;
      image->getsurface()->getScene()->addItem(qgi);
      image->qgi=0;
  }

  return 1;
}

static void HideFigureCallbackGL(Entity *e,View3dSurface *vs,void *d)
{
    HideCBData *hcb = (HideCBData *) d;
    e->hideGL(hcb->image);
}

int Figure::hideGL(HideImage *image)
{HideCBData hidecbdata;
 Transform timage;

  if (missing)
    return 1;

  image->qgi = 0;

  hidecbdata.image = image;

  image->gettransform(&timage);
  image->applytransform(transform);
  image->getsurface()->setclipplanesvalid(0);

  ProcessFigure(image->getsurface(),0,HideFigureCallbackGL,&hidecbdata);

  image->settransform(timage);
  image->getsurface()->setclipplanesvalid(1);

  return 1;
}

void Figure::getsubfigures(StringList *slist)
{CadFile infile;
 EntityList list;
 Entity *e;
 Transform tview;
 int index;
  if (missing)
    return;
  if (options.test(FigureParametric))
    {  for (index = 0 ; index < 3 ; index++)
         for (entitylist[index].start() ; (e = entitylist[index].next()) != NULL ; )
           if (e->isa(figure_entity))
             {  slist->add(((Figure *)e)->getlibrary());
                ((Figure *)e)->getsubfigures(slist);
             }
    }
  else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(figure_entity))
             {  slist->add(((Figure *)e)->getlibrary());
                ((Figure *)e)->getsubfigures(slist);
             }
    }
  else
    {  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb"))
         {  missing = 1;  return;
         }
       if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(figure_entity))
                   {  slist->add(((Figure *)e)->getlibrary());
                      ((Figure *)e)->getsubfigures(slist);
                   }
                 delete e;
              }
         }
       infile.close();
    }
}

void Figure::getsubfigures(CStringList *slist)
{CadFile infile;
 EntityList list;
 Entity *e;
 Transform tview;
 int index;
  if (missing)
    return;
  if (options.test(FigureParametric))
    {  for (index = 0 ; index < 3 ; index++)
         for (entitylist[index].start() ; (e = entitylist[index].next()) != NULL ; )
           if (e->isa(figure_entity))
             {  slist->add(RC2C(((Figure *)e)->getlibrary()));
                ((Figure *)e)->getsubfigures(slist);
             }
    }
  else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(figure_entity))
             {  slist->add(RC2C(((Figure *)e)->getlibrary()));
                ((Figure *)e)->getsubfigures(slist);
             }
    }
  else
    {  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb"))
         {  missing = 1;  return;
         }
       if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(figure_entity))
                   {  slist->add(RC2C(((Figure *)e)->getlibrary()));
                      ((Figure *)e)->getsubfigures(slist);
                   }
                 delete e;
              }
         }
       infile.close();
    }
}

int Figure::getinfo(RCCHAR *description,RCCHAR *category,RCCHAR *cost)
{CadFile infile;
 EntityList list;
 Entity *e;
 Transform tview;
 Values values(1);
  if (options.test(1) && fstyle != -1)
    filename[fstyle] = '2';
  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb"))
    {  missing = 1;  return 0;
    }
  if (infile.readheader())
    {  while ((e = infile.nextentity(0,&values)) != NULL)
         delete e;
       infile.close();
       strcpy(description,values.getstring("in::Description"));
       strcpy(category,values.getstring("in::Category"));
       strcpy(cost,values.getstring("in::Cost"));
       return strlen(description) > 0;
    }
  return 0;
}

void Figure::addattributetext(void)
{
 CadFile infile;
 EntityList list;
 Entity *e;
 Transform tview;
 RCCHAR text[4096],label[300];
 BitMask change(32),toptions(32);
 AttributeList al;
 RCCHAR *at;

  if (options.test(1) && fstyle != -1)
    filename[fstyle] = '2';
  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb")) return;
  if (infile.readheader())
    {  while ((e = infile.nextentity(0)) != 0)
         {  if (e->isa(text_entity) && ((Text *)e)->getlabel() != 0)
              {Dialog dialog("InsertFigureText_dialog");
                 strcpy(text,((Text *)e)->gettext());
                 dialog.add(new StringDialogControl(101,text,4096));
                 dialog.add(new StringDialogControl(100,(RCCHAR*)((Text *)e)->getlabel(),strlen(((Text *)e)->getlabel())+1));
                 if (dialog.process() == TRUE)
                   {  v.setstring("tx::text",text);
                      strcpy(label,((Text *)e)->gettext());
                      change.set(30);  change.set(31);
                      ((Text *)e)->change(change,*((Text *)e),0,0,0,0,0,0,CHAR2RCCHR(text),CHAR2RCCHR(label),toptions);
                      e->setvisible(*getvisible());
                      e->move(&db,transform,0);
                      db.geometry.add(e);
                      e->adda(this);
                      unpacked.add(e);
                   }
                 else
                   delete e;
              }
            else if (! e->getatlist().empty() && e->isa(point_entity))
              {  al = e->getatlist();
                 for (al.start() ; (at = al.next()) != 0 ; )
                   if (strcmp(at,"Explode") == 0)
                     break;
                 if (at != 0)
                   {  e->setvisible(*getvisible());
                      e->move(&db,transform,0);
                      db.geometry.add(e);
                      e->adda(this);
                      unpacked.add(e);
                   }
                 else
                   delete e;
              }
            else
              delete e;
         }
       infile.close();
    }
}

int Figure::cutplane(Point normal,double D,RCCHAR *l,RCCHAR *n,Point *org,Point *xa,Point *ya,Point *s,Point *r)
{CadFile infile;
 EntityList list,plist;
 Entity *e;
 Transform tview;
 Point p1,p2;
  if (! options.test(1))  // Must be an architectural figure
    return 0;
  if (fstyle != -1)
    filename[fstyle] = '3';
  if (options.test(FigureParametric))
    {  for (entitylist[1].start() ; (e = entitylist[1].next()) != NULL ; )
         if (e->isa(line_entity))
           {  p1 = transform.transform(((LinearEntity *)e)->start());
              p2 = transform.transform(((LinearEntity *)e)->end());
              if ((normal.dot(p1) + D) * (normal.dot(p2) + D) <= 0.0)
                {  if (fstyle != -1)
                     filename[fstyle] = 'S';
                   strcpy(l,filename);  strcpy(n,altfilename);
                   org->setxyz(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
                   *org -= normal * (normal.dot(*org) + D);
                   ya->setxyz(transform.mat(0,2),transform.mat(1,2),transform.mat(2,2));
                   *xa = ya->cross(normal);
                   s->setxyz(1.0,1.0,1.0);
                   r->setxyz(0.0,0.0,0.0);
                   return 1;
                }
           }
    }
  else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(line_entity))
           {  p1 = transform.transform(((LinearEntity *)e)->start());
              p2 = transform.transform(((LinearEntity *)e)->end());
              if ((normal.dot(p1) + D) * (normal.dot(p2) + D) <= 0.0)
                {  if (fstyle != -1)
                     filename[fstyle] = 'S';
                   strcpy(l,filename);  strcpy(n,altfilename);
                   org->setxyz(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
                   *org -= normal * (normal.dot(*org) + D);
                   ya->setxyz(transform.mat(0,2),transform.mat(1,2),transform.mat(2,2));
                   *xa = ya->cross(normal);
                   s->setxyz(1.0,1.0,1.0);
                   r->setxyz(0.0,0.0,0.0);
                   return 1;
                }
           }
    }
  else
    {  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb")) return 0;
       if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  p1 = transform.transform(((LinearEntity *)e)->start());
                      p2 = transform.transform(((LinearEntity *)e)->end());
                      if ((normal.dot(p1) + D) * (normal.dot(p2) + D) <= 0.0)
                        {  if (fstyle != -1)
                             filename[fstyle] = 'S';
                           strcpy(l,filename);  strcpy(n,altfilename);
                           org->setxyz(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
                           *org -= normal * (normal.dot(*org) + D);
                           ya->setxyz(transform.mat(0,2),transform.mat(1,2),transform.mat(2,2));
                           *xa = ya->cross(normal);
                           s->setxyz(1.0,1.0,1.0);
                           r->setxyz(0.0,0.0,0.0);
                           return 1;
                        }
                   }
                 delete e;
              }
         }
       infile.close();
    }
  return 0;
}

int Figure::cutout(int threed,RCCHAR *leaf,EntityList *clist)
{CadFile infile;
 EntityList list,plist;
 Entity *e,*ep;
 Transform tview;
 AttributeList al;
 RCCHAR *at;
 int index;
  if (options.test(1) && fstyle != -1)
    filename[fstyle] = RCCHAR('2' + threed);
  index = options.test(1) && threed;
  if (options.test(FigureParametric))
    {  index = options.test(1) && threed;
       for (entitylist[index].start() ; (e = entitylist[index].next()) != NULL ; )
         if (e->isa(plane_entity) || e->isa(line_entity))
           {  al = e->getatlist();
              for (al.start() ; (at = al.next()) != 0 ; )
                if (strcmp(at,leaf) == 0)
                  {  if (e->isa(line_entity))
                       {  e->setcopye(0);
                          clist->add(e->clone(transform));
                       }
                     else if (e->isa(plane_entity))
                       {  plist = ((Plane *)e)->getlist();
                          for (plist.start() ; ! plist.atend() ; )
                            {  ep = plist.next();
                               ep->setcopye(0);
                               if (ep != 0)
                                 clist->add(ep->clone(transform));
                               else
                                 clist->add(0);
                            }
                       }
                     break;
                  }
           }
    }
  else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(plane_entity) || e->isa(line_entity))
           {  al = e->getatlist();
              for (al.start() ; (at = al.next()) != 0 ; )
                if (strcmp(at,leaf) == 0)
                  {  if (e->isa(line_entity))
                       {  e->setcopye(0);
                          clist->add(e->clone(transform));
                       }
                     else if (e->isa(plane_entity))
                       {  plist = ((Plane *)e)->getlist();
                          for (plist.start() ; ! plist.atend() ; )
                            {  ep = plist.next();
                               ep->setcopye(0);
                               if (ep != 0)
                                 clist->add(ep->clone(transform));
                               else
                                 clist->add(0);
                            }
                       }
                     break;
                  }
           }
    }
  else
    {  if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(getcurrentfilename(),"rb")) return 0;
       if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              {  if (*leaf <= '4')
                   {  if (e->isa(plane_entity) || e->isa(line_entity))
                        {  al = e->getatlist();
                           for (al.start() ; (at = al.next()) != 0 ; )
                             if (strcmp(at,leaf) == 0)
                               {  if (e->isa(line_entity))
                                    {  e->setcopye(0);
                                       clist->add(e->clone(transform));
                                    }
                                  else if (e->isa(plane_entity))
                                    {  plist = ((Plane *)e)->getlist();
                                       for (plist.start() ; ! plist.atend() ; )
                                         {  ep = plist.next();
                                            ep->setcopye(0);
                                            if (ep != 0)
                                              clist->add(ep->clone(transform));
                                            else
                                              clist->add(0);
                                         }
                                    }
                                  break;
                               }
                        }
                     }
                 else
                   {  al = e->getatlist();
                      for (al.start() ; (at = al.next()) != 0 ; )
                        if (strcmp(at,leaf) == 0)
                          {  e->setcopye(0);
                             clist->add(e->deepclone(transform));
                          }
                   }
                 delete e;
              }
         }
       infile.close();
    }
  return clist->length() > 0;
}

double Figure::cutoutlength(int threed,RCCHAR *leaf)
{CadFile infile;
 EntityList list,plist;
 Entity *e;
 Transform tview;
 AttributeList al;
 RCCHAR *at;
 double length;
 int index;
  if (options.test(1) && fstyle != -1)
    filename[fstyle] = RCCHAR('2' + threed);
  index = options.test(1) && threed;
  length = 0.0;
  if (options.test(FigureParametric))
    {  index = options.test(1) && threed;
       for (entitylist[index].start() ; (e = entitylist[index].next()) != NULL ; )
         if (e->isa(line_entity))
           {  al = e->getatlist();
              for (al.start() ; (at = al.next()) != 0 ; )
                if (strcmp(at,leaf) == 0)
                  {  if (e->isa(line_entity))
                       {  if (((Line *)e)->getp1().x > length)
                            length = ((Line *)e)->getp1().x;
                          if (((Line *)e)->getp2().x > length)
                            length = ((Line *)e)->getp2().x;
                       }
                     break;
                  }
           }
    }
  else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(line_entity))
           {  al = e->getatlist();
              for (al.start() ; (at = al.next()) != 0 ; )
                if (strcmp(at,leaf) == 0)
                  {  if (e->isa(line_entity))
                       {  if (((Line *)e)->getp1().x > length)
                            length = ((Line *)e)->getp1().x;
                          if (((Line *)e)->getp2().x > length)
                            length = ((Line *)e)->getp2().x;
                       }
                     break;
                  }
           }
    }
  else
    {  if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(getcurrentfilename(),"rb")) return 0;
       if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(line_entity))
                   {  al = e->getatlist();
                      for (al.start() ; (at = al.next()) != 0 ; )
                        if (strcmp(at,leaf) == 0)
                          {  if (((Line *)e)->getp1().x > length)
                               length = ((Line *)e)->getp1().x;
                             if (((Line *)e)->getp2().x > length)
                               length = ((Line *)e)->getp2().x;
                             break;
                          }
                   }
                 delete e;
              }
         }
       infile.close();
    }
  return length;
}

void Figure::nfacets(double,int *nu,int *nv)
{  *nu = *nv = 0;
}

struct FigureCallBackInfo
  {PolygonCallBack cb;
   void *data;
   Transform *t;
  };

static int figurecallback1(SurfaceEntity *se,void *polygon,int,void *,void *cbdata)
{FigureCallBackInfo *fcbi = (FigureCallBackInfo *) cbdata;
  ((RayTracePolygon *)polygon)->transform(fcbi->t);
  return fcbi->cb(se,polygon,0,0,fcbi->data);
}

static int figurecallback2(SurfaceEntity *se,void *polygon,int n,void *p,void *cbdata)
{FigureCallBackInfo *fcbi = (FigureCallBackInfo *) cbdata;
 Point *points = (Point *)p;
 Point ot;

  if (n == 3)
    {  points[0] = fcbi->t->transform(points[0]);
       points[1] = fcbi->t->transform(points[1]);
       points[2] = fcbi->t->transform(points[2]);
       ot = fcbi->t->transform(Point(0.0,0.0,0.0));
       points[3] = (fcbi->t->transform(points[3]) - ot).normalize();
       points[4] = (fcbi->t->transform(points[4]) - ot).normalize();
       points[5] = (fcbi->t->transform(points[5]) - ot).normalize();
    }
  else if (n == 0 && p == 0)
    ((Polygon3d *)polygon)->transform(fcbi->t);

  return fcbi->cb(se,polygon,n,p,fcbi->data);
}

static int figurecallback3(SurfaceEntity *se,void *polygon,int,void *,void *cbdata)
{FigureCallBackInfo *fcbi = (FigureCallBackInfo *) cbdata;
  ((Polygon3d *)polygon)->transform(fcbi->t);
  return fcbi->cb(se,polygon,0,0,fcbi->data);
}

int Figure::facet(const BitMask &opt,double error,int nu,int nv,View3dSurface *vs,PolygonCallBack cb,void *data)
{FigureCallBackInfo fcbi;
 CadFile infile;
 EntityList list;
 Entity *e;
 int i,stopped,layer,arch,index;
 BitMask *layertable;
 Point xa,ya,za(0.0,0.0,1.0),scale,origin;
 Transform t;

  SetupMatrix(vs);


  if (options.test(FigureElevation))
    { //  Rotate the figure and draw it again
      t.rotatex(acos(-1.0) / 2.0);    
    }    
            
  t.apply(transform);


  fcbi.cb = cb;  fcbi.data = data;  fcbi.t = & t;  stopped = 0;
  arch = options.test(1);
  layertable = &db.dismask.layer;


  for (i = 0 ; i == 0 || i < ncopies * options.test(FigureElevation) ; i++)
    {  if (options.test(FigureParametric))
         {  index = options.test(1);
            for (entitylist[index].end() ; (e = entitylist[index].last()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if (e->issurface() && (!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && layertable->test(layer))
                   {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                      if (opt.test(0))
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback1,&fcbi)) return 0;
                        }
                      else if (opt.test(2))
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback2,&fcbi)) return 0;
                        }
                      else
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback3,&fcbi)) return 0;
                        }
                   }
                 if ((stopped = program->getinterrupt()) != 0) break;
              }
         }
       else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
         {  for (list.end() ; (e = list.last()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if (e->issurface() && (!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && layertable->test(layer))
                   {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                      if (opt.test(0))
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback1,&fcbi)) return 0;
                        }
                      else if (opt.test(2))
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback2,&fcbi)) return 0;
                        }
                      else
                        {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback3,&fcbi)) return 0;
                        }
                   }
                 if ((stopped = program->getinterrupt()) != 0) break;
              }
         }
       else
         {  if (arch && fstyle != -1)
              filename[fstyle] = '3';
            index = options.test(1);
            if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(getcurrentfilename(),"rb")) return 0;
            if (infile.readheader())
              {  while ((e = infile.nextentity(NULL)) != NULL)
                   {  if (arch || options.test(FigureOriginalLayer))
                        layer = e->getlayer();
                      else
                        layer = getlayer();  
                      if (e->issurface() && (!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && layertable->test(layer))
                        {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                           if (opt.test(0))
                             {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback1,&fcbi))
                                  {  delete e;  return 0;
                                  }
                             }
                           else if (opt.test(2))
                             {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback2,&fcbi))
                                  {  delete e;  return 0;
                                  }
                             }
                           else
                             {  if (! ((SurfaceEntity *)e)->facet(opt,error,nu,nv,vs,figurecallback3,&fcbi)) 
                                  {  delete e;  return 0;
                                  }
                             }
                        }
                      delete e;
                      if ((stopped = program->getinterrupt()) != 0) break;
                   }
              }
         }
       infile.close();

       if (ncopies > 1)
         {  //  Rotate the figure and draw it again
          Transform t1;
             t1.rotatey(acos(-1.0) / ncopies);    
             t1.apply(t);
             t = t1; 
         }    

    }

  return ! stopped;

}

int GetSize(int ntriangles,Point *triangles)
{int i,size;

  for (i = 0,size = 0 ; i < ntriangles ; i++)
    {  if (triangles[size].z == RCDBL_MAX)
         size += 11;
       else
         size += 9;
    }

 return size;

}


void Figure::triangulate(double error, Transform *)
{CadFile infile;
 EntityList list;
 Entity *e;
 int i,pass,stopped,layer,arch,index;
 BitMask *layertable;
 Point xa,ya,za(0.0,0.0,1.0),scale,origin;
 Transform t;
 int size;

 if (options.test(FigureElevation))
   { //  Rotate the figure and draw it again
     t.rotatex(acos(-1.0) / 2.0);    
   }    
            
 t.apply(transform);

 arch = options.test(1);
 layertable = &db.dismask.layer;

 for (pass = 0 ; pass < 2 ; pass++)
 {
  size = 0;
  ntriangles = 0;
  for (i = 0 ; i == 0 || i < ncopies * options.test(FigureElevation) ; i++)
    {  if (options.test(FigureParametric))
         {  index = options.test(1);
            for (entitylist[index].end() ; (e = entitylist[index].last()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if (e->issurface() && (!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && layertable->test(layer))
                   {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                      if (pass == 0)
                        ((SurfaceEntity *)e)->triangulate(error);
                      else
                        {  ntriangles += ((SurfaceEntity *)e)->getntriangles();
                           memcpy(triangles + size,((SurfaceEntity *)e)->gettriangles(),GetSize(((SurfaceEntity *)e)->getntriangles(),((SurfaceEntity *)e)->gettriangles()) * 24);
                        }
                      size += GetSize(((SurfaceEntity *)e)->getntriangles(),((SurfaceEntity *)e)->gettriangles());
                   }
                 if ((stopped = program->getinterrupt()) != 0) break;
              }
         }
       else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
         {  for (list.end() ; (e = list.last()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if (e->issurface() && (!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && layertable->test(layer))
                   {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                      if (pass == 0)
                        ((SurfaceEntity *)e)->triangulate(error);
                      else
                        {  ntriangles += ((SurfaceEntity *)e)->getntriangles();
                           memcpy(triangles + size,((SurfaceEntity *)e)->gettriangles(),GetSize(((SurfaceEntity *)e)->getntriangles(),((SurfaceEntity *)e)->gettriangles()) * 24);
                        }
                      size += GetSize(((SurfaceEntity *)e)->getntriangles(),((SurfaceEntity *)e)->gettriangles());
                   }
                 if ((stopped = program->getinterrupt()) != 0) break;
              }
         }

       if (ncopies > 1)
         {  //  Rotate the figure and draw it again
          Transform t1;
             t1.rotatey(acos(-1.0) / ncopies);    
             t1.apply(t);
             t = t1; 
         }    

    }

  if (pass == 0)
    {  triangles = new Point[size];
       ntriangles = 0;
    }
  else
    {  for (i = 0,size = 0 ; i < ntriangles ; i++)
         {  if (triangles[size].z == RCDBL_MAX)
              size += 2;

            triangles[size] = t.transform(triangles[size]);
            triangles[size+1] = t.transform(triangles[size+1]);
            triangles[size+2] = t.transform(triangles[size+2]);

            size += 9;
         }
    }

 }

}


int Figure::npoints(void)
{ return 1;
}

Point Figure::point(int)
{Point origin(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
  return origin;
}

Point Figure::nearp(Point)
{Point origin(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
  return origin;
}

int Figure::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e;
 int onwall;
 delete [] mextents; mextents=0;// for dragging LOD

  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;

  if (! sp->match(getorigin()))
    return 1;

  onwall = 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if (! e->isa(wall_entity))
      onwall = 0;
  if (! onwall)
    return 1;

  for (alist.start() ; (e = alist.next()) != 0 ; )
    if (e->isa(wall_entity))
      e->draw(DM_ERASE);

  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase && (status & 16) == 0) 
    draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  transform.apply(t);
  copye = this;
 
  for (alist.start() ; (e = alist.next()) != 0 ; )
    if (e->isa(wall_entity))
      {  cadwindow->invalidatedisplaylist(e);
         e->draw(DM_NORMAL);
      }

  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;

}


int Figure::move(Database *db,Transform &t,int erase,int save)
{Entity *e;
 EntityList newlist;
 delete [] mextents; mextents=0;// for dragging LOD

  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  for (unpacked.start() ; (e = unpacked.next()) != 0 ; )
    {  if (e->getcopye() == 0) e->move(db,t,erase,save);
       if (e->getcopye() == 0)
         {  newlist.destroy();  draw(DM_NORMAL);  return 0;
         }
       newlist.add(e->getcopye());
    }
  newlist.reverse();
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (db != 0)
    { 
	 if(save)
	   db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  dbdelete();
  unpacked.destroy();
  unpacked = newlist;
  transform.apply(t);
  
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 0;
}

Entity *Figure::clone(Transform &t)
{Transform tnew;
 Point origin,xax,yax,scale,rotation;
 int i;
 Parameter *p;
 delete [] mextents; mextents=0;// for display LOD

  if (copye != NULL) return copye;
  tnew = transform;
  tnew.apply(t);
  copye = new Figure(*this,filename,altfilename,origin,xax,yax,scale,rotation,size,ncopies,state1,state2,&tnew,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  ((Figure *)copye)->options.set(FigureParametric,0);
  copye->cloneattributes(this);

  for (i = 0 ; i < (options.test(1) ? 3 : 1) ; i++)
    {  ((Figure *)copye)->attachment[i] = attachment[i];
       ((Figure *)copye)->attachmentlength[i] = -abs(attachmentlength[i]);
    }

  for (parameterlist.start() ; (p = (Parameter *)parameterlist.next()) != 0 ; )
    ((Figure *)copye)->parameterlist.add(new Parameter(p->GetName(),p->GetValue()));
  ((Figure *)copye)->parameterlist.reverse();

  return copye;
}

int Figure::project(Database *db,Transform &t,int)
{  return db->geometry.add(clone(t));
}

void Figure::change(const BitMask &change,const EntityHeader &eh,RCCHAR *l,RCCHAR *n,Point s,Point r,Point si,int nc,Byte s1,Byte s2,const BitMask &op)
{Transform t1,t2;
 EntityList list;
 GeneralList elist;
 Point lastRotation,lastTranslation,lastScale;
 delete [] mextents; mextents=0;

 lastRotation = getrotation();
 lastScale = getscale();
 lastTranslation = getorigin();

 // test for a mirror transform
 // if tzaxis is negative the transform was mirrored

 Point origin(0.0,0.0,0.0);
 Point xaxis(1.0,0.0,0.0);
 Point yaxis(0.0,1.0,0.0);
 Point zaxis(0.0,0.0,1.0);
 Point raxis(0.0,0.0,1.0); // reflection axis
 Point torigin = transform.transform(origin);
 Point txaxis = transform.transform(xaxis); //txaxis.normalize();
 Point tyaxis = transform.transform(yaxis); //tyaxis.normalize();
 Point tzaxis = transform.transform(yaxis);
 txaxis = (txaxis-torigin).normalize();
 tyaxis = (tyaxis-torigin).normalize();
 tzaxis = txaxis.cross(tyaxis).normalize();
 raxis = (txaxis + xaxis.normalize()).normalize();
 double mirrorAngle =  acos(xaxis.dot(txaxis));
 if(txaxis.y < 0.0)
     mirrorAngle = M_PI + M_PI - mirrorAngle;
     //mirrorAngle = (M_PI * 2.0) - mirrorAngle;
 if(txaxis.z == 0.0 & txaxis.y == 0.0 && txaxis.z == 0.0)
 {
     raxis = (tyaxis + yaxis.normalize()).normalize();
 }
 qDebug() << "mirror angle : " << mirrorAngle * 180.0 / M_PI;

  db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4))
    {  delete [] filename;
       if ((filename = new RCCHAR[strlen(l)+1]) != NULL)
         strcpy(filename,l);
       missing = 0;
       loadfigure(&list);
    }
  if (change.test(5))
    {  delete [] altfilename;
       if ((altfilename = new RCCHAR[strlen(n)+1]) != NULL)
         strcpy(altfilename,n);
       missing = 0;
       loadfigure(&list);
    }
  // this is the same method as up to v6
  // adding relative values to the current transformation
  // users are shown initial values of 0.0 for rotations
  t1.identity();
  if (change.test(6))
  {
      t2.scale((s.x/lastScale.x != 0.0 ? s.x/lastScale.x : 1.0),1.0,1.0);  t1.apply(t2);
  }
  if (change.test(7))
  {
      t2.scale(1.0,(s.y/lastScale.y != 0.0 ? s.y/lastScale.y : 1.0),1.0);  t1.apply(t2);
  }
  if (change.test(8))
  {
      t2.scale(1.0,1.0,(s.z/lastScale.z != 0.0 ? s.z/lastScale.z : 1.0));  t1.apply(t2);
  }
  if (change.test(11))
  {
      t2.rotatez((r.z) * M_PI / 180.0);  t1.apply(t2);
  }
  if (change.test(10))
  {
      t2.rotatey((r.y) * M_PI / 180.0);  t1.apply(t2);
  }
  if (change.test(9))
  {
      t2.rotatex((r.x) * M_PI / 180.0);  t1.apply(t2);
  }
  t1.apply(transform);
  transform = t1;

  if (change.test(12)) 
    {  if (options.test(FigureUseAlternativeName) != op.test(FigureUseAlternativeName))
         {  options = op;
            loadfigure(&list);
         }
       else
         options = op;
       
    }
  if (change.test(13)) 
    state1 = s1;
  if (change.test(14)) 
    state2 = s2;
  if (change.test(15)) 
    options.set(FigureOriginalLayer,op.test(FigureOriginalLayer));
  if (change.test(16)) 
    options.set(FigureOriginalColour,op.test(FigureOriginalColour));
  if (change.test(17)) 
    options.set(FigureOriginalStyle,op.test(FigureOriginalStyle));
  if (change.test(18)) 
    options.set(FigureOriginalWeight,op.test(FigureOriginalWeight));
  if (change.test(19)) 
    options.set(FigureOrientToView,op.test(FigureOrientToView));

  if (change.test(20)) 
    size.x = si.x;
  if (change.test(21)) 
    size.y = si.y;
  if (change.test(22)) 
    size.z = si.z;

  if (change.test(23)) 
    options.set(FigureElevation,op.test(FigureElevation));
  if (change.test(24)) 
    options.set(FigureUseSize,op.test(FigureUseSize));

  if (change.test(25)) 
    ncopies = nc;

  loadfigure(&elist);

  //  Modify the size of the figure to suit the current defined size
  //  1.  Get actual figure extents
  //  2.  Get current figure scales
  //  3.  Update the transform matrix to get the same size
  // change test 4=filename
  // change test 20=si.x
  // change test 21=si.y
  // change test 22=si.z

  qDebug() << "Figure Transform before size change";
  transform.debug();
  qDebug();

  if ((change.test(4) || change.test(20) || change.test(21) || change.test(22)) && options.test(FigureUseSize))
  {
       Point pmax,pmin,scale;

       scale = getscale();
       t1.identity();
       t2 = *gettransform();
       settransform(t1); 
       extents(0,&pmin,&pmax);
       pmax = pmax - pmin;

       if (options.test(FigureElevation))
       {
              if (pmax.x > 0.0 && pmax.y > 0.0 && scale.x > 0.0 && scale.y > 0.0)
              {
                 t1.scale(size.x / pmax.x / scale.x,1.0,size.z / pmax.y / scale.y);
                 t2.apply(t1);
                 settransform(t2);  
              }
       }
       else
       {
             if (pmax.x > 0.0 && pmax.y > 0.0 && scale.x > 0.0 && scale.y > 0.0)
             {
                 Transform t3;
                 t3.translate(-torigin); t2.apply(t3);
                 t1.scale(size.x / pmax.x / scale.x,size.y / pmax.y / scale.y,1.0);
                 t2.apply(t1);
                 t3.identity(); t3.translate(torigin);
                 t2.apply(t3);
                 settransform(t2);
             }
       }

  }
  // show the transform
  qDebug() << "Figure Transform after change";
  transform.debug();
  qDebug();
  //
#ifdef NEW_DRAGGING
  // nothing to when using open gl
#else
  draw(DM_NORMAL);
#endif
}

/*
// defined in ncdialog.h
class FigureDisplayDialogControl : public DisplayDialogControl
  {private:
   public:
     FigureDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void FigureDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
#ifdef USING_WIDGETS
    View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hDC,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
    BitMask options(32);
    RCCHAR filename[600]={0};
    int originalcolour=0,originalstyle=0,originalweight=0;
    int l,c,st,w,ol,oc,ost,ow;
    double rw;

    if(hWnd ==0)
        return;

    // get the current defaults for the header
    ol  = db.header.getlayer();
    oc  = db.header.getcolour();
    ost = db.header.getstyle();
    ow  = db.header.getweight();
    header.change(c,l,st,w);

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
      ctl = (QGraphicsView*)((Figure_Dialog*)dialog->gethdlg())->getControl(123);
    QGraphicsScene *scene = 0;
    if(ctl)
        scene = ctl->scene();

    if(ctl && scene)
    {
        scene->clear();
        scene = ctl->scene();
        scene->setSceneRect(QRectF());

        output.setScene(scene);
        QRect qrect = ctl->rect();

        scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                ::v.getreal("wn::backgroundcolour.blue")*255.0)));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0));

          options.clearall();
          if (dialog->currentvalue(100,&c) &&
              dialog->currentvalue(101,&l) &&
              dialog->currentvalue(102,&st) &&
              dialog->currentvalue(103,&rw) &&
              dialog->currentvalue(104,&scale.x) &&
              dialog->currentvalue(105,&scale.y) &&
              dialog->currentvalue(106,&scale.z) &&
              dialog->currentvalue(107,&rotation.x) &&
              dialog->currentvalue(108,&rotation.y) &&
              dialog->currentvalue(109,&rotation.z) &&
              dialog->currentvalue(119,filename,600) &&
              dialog->currentvalue(112,&originalcolour) &&
              dialog->currentvalue(113,&originalstyle) &&
              dialog->currentvalue(114,&originalweight))
            {
#if 0
              // v7 version
               options.set(FigureLoaded);
               org.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
               PointE point(org);
               point.draw(DM_NORMAL,NULL,&output);
               Figure figure(header,_RCT("__The__Special__Figure__"),_RCT("Nigel&Helen"),
                                     org,xaxis,yaxis,scale * 0.5,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
               figure.draw(DM_NORMAL,NULL,&output);
#endif
              // v8 version
              qDebug() << "" << QString(filename);
              if(QString(filename).length() < 1)
              {
                  org.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
                  PointE point(org);
                  point.draw(DM_NORMAL,NULL,&output);

                  options.clearall();
                  options.set(FigureLoaded);
                  (originalcolour > 0) ? options.set(FigureOriginalColour) : options.clear(FigureOriginalColour);
                  (originalstyle  > 0) ? options.set(FigureOriginalStyle)  : options.clear(FigureOriginalStyle);
                  (originalweight > 0) ? options.set(FigureOriginalWeight) : options.clear(FigureOriginalWeight);

                  w = db.lineweights.mmtoweight(rw);
                  header.change((originalcolour > 0) ? oc : c,l,(originalstyle  > 0) ? ost : st ,(originalweight > 0) ? ow : w);

                  // draw a special figure for visualing
                  Figure figure1(header,_RCT("__The__Special__Figure__"),_RCT("Nigel&Helen"),org,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);

                  Point pmin,pmax;
                  figure1.extents(&output,&pmin,&pmax);
                  double du = pmax.x - pmin.x;
                  double dv = pmax.y - pmin.y;
                  if (du / dv > (output.getumax() - output.getumin()) / (output.getvmax() - output.getvmin()))
                    dv = du / (output.getumax() - output.getumin()) * (output.getvmax() - output.getvmin());
                  else
                    du = dv * (output.getumax() - output.getumin()) / (output.getvmax() - output.getvmin());
                  double pvscale = du / (output.getumax() - output.getumin());

                  scale.x /= pvscale;
                  scale.y /= pvscale;
                  scale.z /= pvscale;
                  //scale.z *= 0.5;

                  Figure figure(header,_RCT("__The__Special__Figure__"),_RCT(""),org,xaxis,yaxis,scale*0.8,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
                  figure.draw(DM_NORMAL,NULL,&output);

                  // the axis figure
                  if(fabs(rotation.x) > db.getptoler() || fabs(rotation.y) > db.getptoler() || fabs(rotation.z) > db.getptoler())
                  {
                      options.clearall();
                      options.set(FigureLoaded);
                      header.change(c,l,st,5);
                      Figure figure3(header,_RCT("__The__3DAxis__Figure__"),_RCT(""),org,xaxis,yaxis,Point(3.0,3.0,3.0),rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
                      figure3.draw(DM_INVERT,NULL,&output);
                  }
              }
              else
              {
                  org.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
                  // centre point of the viewport
                  //PointE point(org);
                  //point.draw(DM_NORMAL,NULL,&output);

                  options.clearall();
                  (originalcolour > 0) ? options.set(FigureOriginalColour) : options.clear(FigureOriginalColour);
                  (originalstyle  > 0) ? options.set(FigureOriginalStyle)  : options.clear(FigureOriginalStyle);
                  (originalweight > 0) ? options.set(FigureOriginalWeight) : options.clear(FigureOriginalWeight);

                  w = db.lineweights.mmtoweight(rw);
                  header.change((originalcolour > 0) ? oc : c,l,(originalstyle  > 0) ? ost : st ,(originalweight > 0) ? ow : w);

                  // draw the actual figure
                  Point pmin,pmax;
                  Figure figure1(header,_RCT(filename),_RCT(""),Point(0.0,0.0,0.0),xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
                  figure1.extents(0,&pmin,&pmax);

                  double s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
                  if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
                    s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
                  if (s < 0.00001)
                    s = 1.0;

                  s *= 1.1;
                  Point pscale;
                  pscale.setxyz(scale.x/s,scale.y/s,scale.x/s);
                  Point porg;
                  porg.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);

                  Figure figure2(header,_RCT(filename),_RCT(""),porg,xaxis,yaxis,pscale,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
                  db.dismask.layer.set(db.header.getlayer(),1);
                  figure2.draw(DM_NORMAL,NULL,&output);

                  // the axis figure
                  if(fabs(rotation.x) > db.getptoler() || fabs(rotation.y) > db.getptoler() || fabs(rotation.z) > db.getptoler())
                  {
                      options.clearall();
                      options.set(FigureLoaded);
                      header.change(c,l,st,5);
                      Figure figure3(header,_RCT("__The__3DAxis__Figure__"),_RCT(""),org,xaxis,yaxis,Point(3.0,3.0,3.0),rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
                      figure3.draw(DM_INVERT,NULL,&output);
                  }

                  // origin point of the figure
                  PointE point2(porg);
                  point2.draw(DM_INVERT,NULL,&output);
              }
            }
    }
#else
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
 BitMask options(32);

  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
       IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       output.moveto(0,0);
       output.lineto(rect.right-1,0);
       output.lineto(rect.right-1,rect.bottom-1);
       output.lineto(0,rect.bottom-1);
       output.lineto(0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       options.clearall();
       if (dialog->currentvalue(100,&scale.x) &&
           dialog->currentvalue(101,&scale.y) &&
           dialog->currentvalue(102,&scale.z) &&
           dialog->currentvalue(103,&rotation.x) &&
           dialog->currentvalue(104,&rotation.y) &&
           dialog->currentvalue(105,&rotation.z))
         {  options.set(FigureLoaded);
            org.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
            PointE point(org);
            point.draw(DM_NORMAL,NULL,&output);
            Figure figure(header,_RCT("__The__Special__Figure__"),_RCT("Nigel&Helen"),
                                  org,xaxis,yaxis,scale * 0.5,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
            figure.draw(DM_NORMAL,NULL,&output);
         }
    }
#endif
}


#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD)
static UINT APIENTRY PreviewHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)

{static RCCHAR lastfilename[600],filename[600],xtext[300],ytext[300],ztext[300],text[300];
 double x;
 CadFile infile;
 Entity *e;
 Point pmin,pmax,pmin1,pmax1;
 long c;
 double du,dv,aspect;
#if ! defined(_WIN32_WCE)
  if (uiMsg == 1125)
    return 0;

  // KMJ FIX THIS
  //CommDlg_OpenSave_GetFilePath(GetParent(hdlg),filename,300);

  if (uiMsg == WM_INITDIALOG)
    {  CheckDlgButton(hdlg,2001,v.getinteger("fi::preview") ? BST_CHECKED : BST_UNCHECKED);
       strcpy(lastfilename,filename);
       sprintf(xtext,"%.*lf",db.getnumericdisplaytolerance(),v.getreal("fi::scalex"));
       while (strlen(xtext) > 0 && xtext[strlen(xtext)-1] == '0') xtext[strlen(xtext)-1] = 0;
       if (strlen(xtext) > 0 && xtext[strlen(xtext)-1] == '.') xtext[strlen(xtext)-1] = 0;
       SetDlgItemText(hdlg,2003,xtext);
       sprintf(ytext,"%.*lf",db.getnumericdisplaytolerance(),v.getreal("fi::scaley"));
       while (strlen(ytext) > 0 && ytext[strlen(ytext)-1] == '0') ytext[strlen(ytext)-1] = 0;
       if (strlen(ytext) > 0 && ytext[strlen(ytext)-1] == '.') ytext[strlen(ytext)-1] = 0;
       SetDlgItemText(hdlg,2005,ytext);
       sprintf(ztext,"%.*lf",db.getnumericdisplaytolerance(),v.getreal("fi::scalez"));
       while (strlen(ztext) > 0 && ztext[strlen(ztext)-1] == '0') ztext[strlen(ztext)-1] = 0;
       if (strlen(ztext) > 0 && ztext[strlen(ztext)-1] == '.') ztext[strlen(ztext)-1] = 0;
       SetDlgItemText(hdlg,2007,ztext);
    }
  else if (uiMsg == WM_DESTROY) //  This is the best we can do - parent dialog doesn't let us know if it was cancelled or not
    {  
       v.setinteger("fi::preview",IsDlgButtonChecked(hdlg,2001) == BST_CHECKED);
       GetDlgItemText(hdlg,2003,text,300);
       if (strcmp(text,xtext) != 0)
         {Expression expression(text);
            if (expression.evaluate(&x) == 0)
              { RCMessageBox(hdlg,"X Scale is invalid.","Select Figure",MB_OK);
                 return 1;
              }
            else
              v.setreal("fi::scalex",x);
         }
       GetDlgItemText(hdlg,2005,text,300);
       if (strcmp(text,ytext) != 0)
         {Expression expression(text);
            if (expression.evaluate(&x) == 0)
              { RCMessageBox(hdlg,"Y Scale is invalid.","Select Figure",MB_OK);
                 return 1;
              }
            else
              v.setreal("fi::scaley",x);
         }
       GetDlgItemText(hdlg,2007,text,300);
       if (strcmp(text,ztext) != 0)
         {Expression expression(text);
            if (expression.evaluate(&x) == 0)
              { RCMessageBox(hdlg,"Z Scale is invalid.","Select Figure",MB_OK);
                 return 1;
              }
            else
              v.setreal("fi::scalez",x);
         }

    }
  else if (strcmp(filename,lastfilename) != 0)
    {  InvalidateRect(GetDlgItem(hdlg,2000),0,TRUE);
       strcpy(lastfilename,filename);
    }

  if (uiMsg == WM_DRAWITEM)
    {DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT *) lParam;
     ResourceString rs0(NCVIEWA);
     View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
     RECT rect;
     RCHPEN hpen,oldhpen;
     EntityHeader header(0,0,0,0);
     Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
     BitMask options(32);
     HDC hdc;
       if (uiMsg == WM_DRAWITEM)
         hdc = dis->hDC;
       else
         hdc = 0;
       View3dOutput output(GetDlgItem(hdlg,2000),hdc,&top,1.0);
       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
            IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
            rect.left = 0;  rect.top = 0;
            rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
            FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
            if (IsDlgButtonChecked(hdlg,2001) == BST_CHECKED && strlen(filename) > 0 && infile.open(filename,"rb"))
              {  SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
                 if (infile.readheader())
                   {  c = 0;
                      while ((e = infile.nextentity(NULL)) != NULL)
                        {  if (e->getvisible()->test(0))
                             {  if (c == 0)
                                  e->extents(&output,&pmin,&pmax);
                                else
                                  {  e->extents(&output,&pmin1,&pmax1);
                                     if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                                     if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                                     if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                                     if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                                     if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                                     if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                                  }
                                 c++;
                             }
                           delete e;
                        }
                      du = fabs(pmax.x - pmin.x) / 2.0;  dv = fabs(pmax.y - pmin.y) / 2.0;
                      if (du > db.getptoler() || dv > db.getptoler())
                        {  aspect = output.getwidthmm() / output.getheightmm();
                           if (du / dv > aspect) dv = du / aspect;
                           if (du / dv < aspect) du = dv * aspect;
                           pmin.x = (pmin.x + pmax.x) / 2.0 - du;
                           pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
                           pmax.x = (pmin.x + pmax.x) / 2.0 + du;
                           pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
                           output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);
                        }
                   }
                 infile.close();
                 if (infile.open(lastfilename,"rb"))
                   {  if (infile.readheader())
                        {  while ((e = infile.nextentity(NULL)) != NULL)
                             {  if (e->getvisible()->test(0))
                                  e->draw(DM_NORMAL,NULL,&output);
                                delete e;
                             }
                        }
                      infile.close();
                   }
              }
            else
              {  MoveToEx(output.gethdc(),0,rect.bottom-1,0);
                 LineTo(output.gethdc(),rect.right-1,0);
                 MoveToEx(output.gethdc(),0,0,0);
                 LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
                 SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
              }
         }
        return 1;
    }
#endif
  return 0;
}

static LPWORD lpwAlign ( LPWORD lpIn)
{ULONG ul;
  ul = (ULONG) lpIn;
  ul +=3;
  ul >>=2;
  ul <<=2;
  return (LPWORD) ul;
}

static int nCopyAnsiToWideChar (LPWORD lpWCStr, RCCHAR* lpAnsiIn)
{int cchAnsi = strlen(lpAnsiIn);
    return 0;
  //return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, lpAnsiIn, cchAnsi, (wchar_t *)lpWCStr, cchAnsi) + 1;
}
#endif

int Figure::select(RCCHAR *filename, RCCHAR *name)
{
 RCOPENFILENAME ofn;
 RCCHAR szFile[600], szFileTitle[600],title[600],szDirName[600],nfilename[600],suffix[300];
 RCUINT  i;
 RCCHAR  szFilter[256];
 CadFile infile;
 ResourceString rsnc1(NC+1);
 ResourceString rsnc2(NC+2);
 ResourceString rs0(NCFIGA);
 RCWORD  *p, *dlgtemplate;
 int   nchar;
 RCDWORD lStyle;
 QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  strcpy(suffix,"");
  strcpy(szFile,"");

  // if fi::defaultpath is not defined or does not exist
  //   use FigureBrowser/Figure_Filepath
  // if FigureBrowser/Figure_Filepath is not defined or does not exist
  //    use db::figures-components-path
  // else
  //    use fi::defaultpath

  // KMJ: History, the figure path has been changed as follows
  // v? ~ v5,v6 = Figures
  // v7 = Figures-Components
  // v7 late 2017 early 2018 = Components
  // so, legacy CAD files could have paths to...
  // Figures, Figures-Components or Components in them
  // What to do?? since CADI changed it I guess we should allow for any of them when loading a Figure/Component file
  // first we should for paths in the folowing order
  // 1. the path defined by db::figures-components-path
  // 2. Components
  // 3. Figures-Components
  // 4. Figures

  // KMJ: NOTE:
  // file paths in the ini file should be platform native file paths
  // the figure file paths internally are stored as (UNIX mac) style and must be converted to windows

  qDebug() << QString(v.getstring("fi::defaultfigure"));
  qDebug() << QString(v.getstring("fi::defaultpath"));

  if (strlen(v.getstring("fi::defaultfigure")) == 0 && strlen(v.getstring("fi::defaultpath")) == 0)
  {
      //_getcwd(szDirName, sizeof(szDirName));
      if(settings.value("FigureBrowser/Figure_Filepath","").toString().length() > 0 &&
         QDir(settings.value("FigureBrowser/Figure_Filepath","").toString()).exists())
      {
          strcpy(szFile,settings.value("FigureBrowser/Figure_Filename","").toString().data());
          strcpy(szDirName,settings.value("FigureBrowser/Figure_Filepath","").toString().data());
      }
      else
      {
#if 1
          // using the figure path resolver function
          if(!Figure::ResolveLibraryPath(_RCT(""),szDirName,600))
              strcpy(szDirName,home.getFigureFilepath(_RCT("/")));
#else
          strcpy(szDirName,v.getstring("db::figures-components-path"));
          if(strlen(szDirName) < 1)
              strcpy(szDirName,home.getpublicfilename("Figures-Components"));
          else
          {
              // check if it exists
              // if not use the default
              if(!QDir(QString(szDirName)).exists())
                  strcpy(szDirName,home.getpublicfilename("Figures-Components"));
          }
#endif
      }
  }
  else if (strlen(v.getstring("fi::defaultfigure")) == 0)
  {
    strcpy(szDirName,v.getstring("fi::defaultpath"));
    if(settings.value("FigureBrowser/Figure_Filepath","").toString() == QString(szDirName))
        strcpy(szFile,settings.value("FigureBrowser/Figure_Filename","").toString().data());
  }
  else
    {  if (v.getstring("fi::defaultfigure")[0] == '#')
         {  strcpy(filename,v.getstring("fi::defaultfigure")+1);
            strcpy(name,"");
            v.setstring("fi::defaultfigure",filename);
            if (_stricmp(nfilename,filename) == 0)
              strcat(nfilename,suffix);
            if (strlen(v.getstring("fi::defaultpath")) > 0)
              v.setstring("fi::defaultfigure",_RCT(""));
            return 1;
         }

       strcpy(szDirName,QDir::toNativeSeparators(QString(v.getstring("fi::defaultfigure"))));
       //if (strrchr(szDirName,'\\') != 0)
       //  strrchr(szDirName,'\\')[0] = 0;
       // KMJ todo: use platform seperators here
       if (strrchr(szDirName,QDir::separator().toLatin1()) != 0 )
         strrchr(szDirName,QDir::separator().toLatin1())[0] = 0;
       strcpy(szFile,QDir::toNativeSeparators(QString(v.getstring("fi::defaultfigure"))));
       if (strrchr(szFile,'.') != 0 && strchr(strrchr(szFile,'.'),'#') != 0)
         {  strcpy(suffix,strchr(strrchr(szFile,'.'),'#'));
            strchr(strrchr(szFile,'.'),'#')[0] = 0;
         }
       strcpy(nfilename,szFile);
    }

#if 1 // using the figure browser
  RCFigureBrowser filedialog(0);
  filedialog.SetFilter(_RCT("*.cad;*.CAD"));
  filedialog.SetRootDir(szDirName);
  filedialog.SetTitle(rs0.gets());
  filedialog.SetPrefix(_RCT("Figure"));
  filedialog.ShowSettingsGroup(RCFigureBrowser::figures);

  QString lastFile(szFile);
  QString lastDir(szDirName);

  lastFile = QDir::toNativeSeparators(lastFile);
  lastDir = QDir::toNativeSeparators(lastDir);

  qDebug() << "last file name : " << lastFile.mid(lastFile.lastIndexOf(QDir::separator())+1);
  qDebug() << "QSeperator()   : " << QDir::separator();
  qDebug() << "last index of  : " << lastFile.lastIndexOf(QDir::separator())+1;

  settings.setValue("FigureBrowser/Figure_Filename",lastFile.mid(lastFile.lastIndexOf(QDir::separator())+1));
  settings.setValue("FigureBrowser/Figure_Filepath",lastDir);

  BitMask options(32);
  options = *v.getbm("fi::options");

  filedialog.SetFigureOriginalColour(options.test(FigureOriginalColour));
  filedialog.SetFigureOriginalStyle(options.test(FigureOriginalStyle));
  filedialog.SetFigureOriginalLayer(options.test(FigureOriginalLayer));
  filedialog.SetFigureOriginalWeight(options.test(FigureOriginalWeight));
  filedialog.SetFigureOrientToView(options.test(FigureOrientToView));
  filedialog.SetSizeChecked(options.test(FigureUseSize));

  if(options.test(FigureUseSize))
  {
      filedialog.SetFigureXSize(v.getreal("fi::sizex"));
      filedialog.SetFigureYSize(v.getreal("fi::sizey"));
      filedialog.SetFigureZSize(v.getreal("fi::sizez"));
  }
  else
  {
      filedialog.SetFigureXScale(v.getreal("fi::scalex"));
      filedialog.SetFigureYScale(v.getreal("fi::scaley"));
      filedialog.SetFigureZScale(v.getreal("fi::scalez"));
  }

  filedialog.OnInitDialog();

  QString selFilename,selDirname;
  if(filedialog.exec())
  {
      selFilename = QString(filedialog.GetFileName());
      strcpy(filename,QDir::fromNativeSeparators(selFilename).data());
      selDirname = QString(filedialog.GetCurrentPath());
      //strcpy(name,"");
      v.setstring("fi::defaultfigure",filename);
      //if (_stricmp(filename,filename) == 0)
      //  strcat(filename,suffix);

      v.setreal("fi::diascalex",filedialog.FigureXScale());
      v.setreal("fi::diascaley",filedialog.FigureYScale());
      v.setreal("fi::diascalez",filedialog.FigureZScale());

      v.setreal("fi::diasizex",filedialog.FigureXSize());
      v.setreal("fi::diasizey",filedialog.FigureYSize());
      v.setreal("fi::diasizez",filedialog.FigureZSize());

      settings.setValue("FigureBrowser/Figure_Filename",selFilename.mid(selFilename.lastIndexOf(QDir::separator())+1));
      settings.setValue("FigureBrowser/Figure_Filepath",selDirname);

      if (strlen(v.getstring("fi::defaultpath")) > 0)
        v.setstring("fi::defaultfigure",_RCT(""));

      options.set(FigureOriginalColour,filedialog.GetFigureOriginalColour());
      options.set(FigureOriginalStyle,filedialog.GetFigureOriginalStyle());
      options.set(FigureOriginalLayer,filedialog.GetFigureOriginalLayer());
      options.set(FigureOriginalWeight,filedialog.GetFigureOriginalWeight());
      options.set(FigureOrientToView,filedialog.GetFigureOrientToView());
      // v8 enhancement
      options.set(FigureUseSize,filedialog.SizeIsChecked());
      v.setbm("fi::diaoptions",options);

      return 1;
  }
  else
      return 0;


#else
  _tcscpy(szFilter,rsnc1.getws());
  _tcscpy(szFilter,"Figure Files (*.cad);;Image files :(*.jpg *.jpg *.bmp *.png);;JPEG files (*.jpg *.jpeg);;Bitmap files (*.bmp);;PNG files (*.png)");
  //for (i = 0; szFilter[i] != '\0'; i++)
  // if (szFilter[i] == '|') szFilter[i] = '\0';

  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = szFilter;
  ofn.lpstrTitle = rs0.getws();
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.lpstrDefExt = rsnc2.getws();
  ofn.lpstrFileTitle = title;

  if ( LOBYTE(LOWORD(GetVersion())) > 3)
    {
       //ofn.lpfnHook = (RCLPOFNHOOKPROC)PreviewHookProc;
       ofn.lpfnHook = 0; // not using this callback in Qt
       //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
       // put the Qt Mode options in the high word
       ofn.Flags = MAKEWPARAM(0,QFileDialog::ExistingFile);
    }
  else
    {
       ofn.lpfnHook = 0;
       // put the Qt Mode options in the high word
       //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
       ofn.Flags = MAKEWPARAM(0,QFileDialog::ExistingFile);
    }

  ofn.hInstance = (RCHINSTANCE) dlgtemplate;

  if (GetOpenFileName(&ofn))
    {  
       strcpy(filename,ofn.lpstrFile);
       strcpy(name,"");
       v.setstring("fi::defaultfigure",filename);
       if (_stricmp(filename,filename) == 0)
         strcat(filename,suffix);

       if (strlen(v.getstring("fi::defaultpath")) > 0)
         v.setstring("fi::defaultfigure",_RCT(""));

       return 1;
    }
  else
    return 0;
#endif
}

class MDListDialogControl : public ListDialogControl
  {private:
   public:
     MDListDialogControl(int id) : ListDialogControl(id,0,(RCCHAR**)0,(int *)0) {}
     void load(Dialog *);
  };

void MDListDialogControl::load(Dialog *dialog)
{Entity *e;
  removeallitems(dialog);
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(figure_entity) && ((Figure *)e)->getmissing())
      additem(dialog,CHAR2RCCHR(((Figure *)e)->getfilename()));
}

class MDButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     MDButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };


int MDButtonDialogControl::select(Dialog *dialog)
{
    RCCHAR text[600],message[600];
    Entity *e;
    int i,n_items;
    int *items;

    n_items = SendMessage(GetDlgItem(dialog->gethdlg(),100),LB_GETSELCOUNT,0,0);
    items = new int[n_items];
    SendMessage(GetDlgItem(dialog->gethdlg(),100),LB_GETSELITEMS,n_items,(LPARAM)items);

    if (n_items == 1)
    {
        SendMessage(GetDlgItem(dialog->gethdlg(),100),LB_GETTEXT,items[0],(LPARAM)text);
        sprintf(message,"Delete all figures called '%ls'?",text);
        if (dialog->MessageBox(message,"Delete Missing Figures",MB_YESNO) != IDYES)
            return 0;
    }
    else if (n_items > 1)
    {   if (dialog->MessageBox("Delete all missing figures that have the same name as the selected figures?","Delete Missing Figures",MB_YESNO) != IDYES)
            return 0;
    }
    else
        return 0;

    for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
        if (e->isa(figure_entity))
        {   for (i = 0 ; i < n_items ; i++)
            {   SendMessage(GetDlgItem(dialog->gethdlg(),100),LB_GETTEXT,items[i],(LPARAM)text);
                if (_stricmp(text,((Figure *)e)->getfilename()) == 0)
                    break;
            }
            if (i < n_items )
            {   db.geometry.del(e);
                db.geometry.start();
            }
        }

    ((MDListDialogControl *)dialog->getcontrol(100))->load(dialog);

    return 0;
}

class DFPButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     DFPButtonDialogControl(int id) : ButtonDialogControl(id)  {  }
     int select(Dialog *);
  };

int DFPButtonDialogControl::select(Dialog *dialog)
{RCOPENFILENAME ofn;
 RCCHAR dirname[600],filename[600],filetitle[600],filter[300];
 int i;

  //_getcwd(dirname, sizeof(dirname));
  strcpy(dirname,v.getstring("db::figures-components-path"));
  if(strlen(dirname) < 1)
      strcpy(dirname,home.getpublicfilename("Figures-Components"));
  else
  {
      // check if it exists
      // if not use the default
      if(!QDir(QString(dirname)).exists())
          strcpy(dirname,home.getpublicfilename("Figures-Components"));
  }
  filename[0] = '\0';
  _tcscpy(filter,TEXT("Default figure path (*.cad)"));
  //for (i = 0; filter[i] != '\0'; i++)
  //  if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = _RCT("CAD");
  ofn.lpstrTitle = _RCT("Select a figure directory");
  //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  ofn.Flags = QFileDialog::ShowDirsOnly;;
  if (GetOpenFileName(&ofn))
    {
      // we only get a directory path
       //if (strrchr(ofn.lpstrFile,'/') != 0)
       //  strrchr(ofn.lpstrFile,'/')[0] = 0;
       ((StringDialogControl *)dialog->getcontrol(121))->change(dialog,ofn.lpstrFile);
    }
  return 0;
}

class FigureVerifyButtonControl : public ButtonDialogControl
  {public:
     FigureVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int FigureVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,gcolour,tcolour,layer,style,iweight;
    double weight,gred,ggreen,gblue,tred,tgree,tblue;

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

void FigureExtents(RCCHAR *library,View3dSurface *vs,Point *pmin,Point *pmax)
{
    BitMask options(32);
    Figure figure(library,_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,options);
    figure.extents(vs,pmin,pmax);
}

int Figure::setup(BitMask *change, Point *scale, Point *rotation, Byte *, Byte *, BitMask *o, RCCHAR *fn, RCCHAR *afn, EntityHeader *header,Point *inOrigin, Point *inSize)
{
 Dialog dialog("Figure_Dialog");
 DialogControl *dc;
 ResourceString rs1(NCFIGA+1),rsVaries(4107);
 double deltarotationx,deltarotationy,deltarotationz;
 int result,autoattachdetach,FigureUseScale=0;
 RCCHAR defaultpath[600],sorigin[300],sfilename[600],saltfilename[600];
 RCCHAR sscalex[300],sscaley[300],sscalez[300],text[300];
 RCCHAR srotationx[300],srotationy[300],srotationz[300];
 double scalex,scaley,scalez,rotationx,rotationy,rotationz;
 Point origin,size,pmin(0,0,0),pmax(1000.0,1000.0,0.0);
 int c,l,s,defColour,defLayer,defStyle;
 double wt,defWeight;
 BitMask defOptions(32),changeIn(32);
 RCCHAR defFname[600],defaltFname[600],nullString[600];
 Point defScale(1.0,1.0,1.0),defRotation(0,0,0),defSize(1000.0,1000.0,0.0);
 Point lastRotation;
 Byte defState1,defState2;
 int tristates[] = { 1,1,1,1,1,1,1 };

  autoattachdetach = v.getinteger("fi::autoattachdetach");
  int changecount = v.getinteger("op::changecount");

  EntityHeader entheader;
  if(header != 0)
      header->verify(&dialog,&c,&l,&s,&wt);
  else
      entheader.verify(&dialog,&c,&l,&s,&wt);

  strcpy(nullString,"");
  if(fn)
      strcpy(sfilename,fn);
  else
      strcpy(sfilename,nullString);

  if(afn)
      strcpy(saltfilename,afn);
  else
      strcpy(saltfilename,nullString);

  defColour = db.header.getcolour();
  defLayer  = db.header.getlayer();
  defStyle  = db.header.getstyle();
  defWeight = db.lineweights.weighttomm(db.header.getweight());
  Figure::getdefaults(&defScale,&defRotation,&defState1,&defState2,&defOptions);

  lastRotation = *rotation;

  if(inOrigin == 0 || changecount == 0 || changecount > 1)
      origin = Point(0,0,0);
  else
      origin = *inOrigin;

  FigureUseScale = (o->test(FigureUseSize) == 1) ? 0 : 1;
  tristates[5] = (o->test(FigureUseSize) == 1) ? 1 : 0;
  tristates[6] = (o->test(FigureUseSize) == 1) ? 0 : 1;

  // default size is 1 meter
  if(strlen(sfilename) > 0)
  {
      FigureExtents(sfilename,0,&pmin,&pmax);
      defSize = pmax - pmin;
  }
  if(inSize == 0 || changecount == 0)
      size = defSize; // Point(defSize.x * scale->x, defSize.y * scale->y, defSize.z * scale->z);
  else
  {
      if(inSize->x == 0.0 && inSize->y == 0.0 && inSize->z == 0.0)
          size = Point(defSize.x * scale->x, defSize.y * scale->y, defSize.z * scale->z);
      else
          size = *inSize;
  }

  // new for v8 using size so adjust the scale values
  // sizes and scale values are show to the user
  if(o->test(FigureUseSize) == 1)
  {
      scale->x = ((size.x == 0.0 || defSize.x == 0.0) ? 1.0 : size.x / defSize.x);
      scale->y = ((size.y == 0.0 || defSize.y == 0.0) ? 1.0 : size.y / defSize.y);
      scale->z = ((size.z == 0.0 || defSize.z == 0.0) ? 1.0 : size.z / defSize.z);
  }
  else
      size = Point(defSize.x * scale->x, defSize.y * scale->y, defSize.z * scale->z);

  // users are shown 0.0 values for the rotations
  rotation->x = 0.0;
  rotation->y = 0.0;
  rotation->z = 0.0;
  // cancel the rotation change flags
  if(change != NULL)
  {
    change->set(9);
    change->set(10);
    change->set(11);
  }
  //

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
          wt = 0.10;

      // file name
      if(!change->test(4))
          if(fn)
            strcpy(fn,nullString);
      // alt filename
      if(!change->test(5))
          if(afn)
            strcpy(afn,nullString);
      // scale x
      if(!change->test(6))
          scale->x = -1000000.0;
      // scale y
      if(!change->test(7))
          scale->y = -1000000.0;
      // scale z
      if(!change->test(8))
          scale->z = -1000000.0;
      // size x
      if(!change->test(20))
          size.x = -1000000.0;
      // size y
      if(!change->test(21))
          size.y = -1000000.0;
      // size z
      if(!change->test(22))
          size.z = -1000000.0;
      // rotation x
      if(!change->test(9))
          rotation->x = -360.0;
      // rotation y
      if(!change->test(10))
          rotation->y = -360.0;
      // rotation z
      if(!change->test(11))
          rotation->z = -360.0;
      // use alt name
      if(!change->test(12))
          o->set(defOptions.test(FigureUseAlternativeName));
      // original layer
      if(change->test(15))
          o->test(FigureOriginalLayer)==0 ? tristates[0] = 0 : 2;
      // original colour
      if(change->test(16))
          o->test(FigureOriginalColour)==0 ? tristates[1] = 0 : 2;
      // original style
      if(change->test(17))
          o->test(FigureOriginalStyle)==0 ? tristates[2] = 0 : 2;
      // original weight
      if(change->test(18))
          o->test(FigureOriginalWeight)==0 ? tristates[3] = 0 : 2;
      if(change->test(19))
          o->test(FigureOrientToView)==0 ? tristates[4] = 0 : 2;
      if(change->test(24))
      {
          o->test(FigureUseSize)==0 ? tristates[5] = 0 : 2;
          o->test(FigureUseSize)==0 ? tristates[6] = 2 : 0;
      }
      else
      {
          tristates[5] = 1;
          tristates[6] = 1;
      }
  }
  else
  {
      *scale = defScale;
      size = Point(defSize.x * scale->x, defSize.y * scale->y, defSize.z * scale->z);
      strcpy(sscalex,nullString);
      strcpy(sscaley,nullString);
      strcpy(sscalez,nullString);
      *rotation = defRotation;
      strcpy(srotationx,nullString);
      strcpy(srotationy,nullString);
      strcpy(srotationz,nullString);

      o->test(FigureOriginalLayer)==0 ? tristates[0] = 0 : tristates[0] = 1;
      o->test(FigureOriginalColour)==0 ? tristates[1] = 0 : tristates[1] = 1;
      o->test(FigureOriginalStyle)==0 ? tristates[2] = 0 : tristates[2] = 1;
      o->test(FigureOriginalWeight)==0 ? tristates[3] = 0 : tristates[3] = 1;
      o->test(FigureOrientToView)==0 ? tristates[4] = 0 : tristates[4] = 1;
      o->test(FigureUseSize)==0 ? tristates[5] = 0 : tristates[5] = 1;
      o->test(FigureUseSize)==0 ? tristates[6] = 1 : tristates[6] = 0;
  }


  dialog.add(dc = new RealDialogControl(104,&scale->x,0.5,-1000000,1000000,ScaleByLength));
  dialog.add(dc = new RealDialogControl(105,&scale->y,0.5,-1000000,1000000,ScaleByLength));
  dialog.add(dc = new RealDialogControl(106,&scale->z,0.5,-1000000,1000000,ScaleByLength));
  dialog.add(dc = new RealDialogControl(107,&rotation->x,1.0,-360.0,360.0));
  dialog.add(dc = new RealDialogControl(108,&rotation->y,1.0,-360.0,360.0));
  dialog.add(dc = new RealDialogControl(109,&rotation->z,1.0,-360.0,360.0));
  dialog.add(dc = new RealDialogControl(116,&size.x,1.0,-1000000,1000000,ScaleByLength));
  dialog.add(dc = new RealDialogControl(117,&size.y,1.0,-1000000,1000000,ScaleByLength));
  dialog.add(dc = new RealDialogControl(118,&size.z,1.0,-1000000,1000000,ScaleByLength));
  //dialog.add(new CheckBoxDialogControl(131,o,FigureUseSize));// this is used
  //dialog.add(new CheckBoxDialogControl(132,&FigureUseScale));// dummy not used
  dialog.add((DisplayDialogControl *) new FigureDisplayDialogControl(123));

  if(inOrigin==0)
      dialog.add(new StringDialogControl(110,nullString,300));
  else
      if(changecount == 1)
          dialog.add(new StringDialogControl(110,origin.string(sorigin),300));
      else
          dialog.add(new StringDialogControl(110,rsVaries.gets(),300));

  // now defined later in the code
  //dialog.add(new StringDialogControl(119,sfilename,strlen(sfilename)+1));
  //dialog.add(new StringDialogControl(120,saltfilename,strlen(saltfilename)+1));
  strcpy(defaultpath,v.getstring("fi::defaultpath"));
  dialog.add(new StringDialogControl(121,defaultpath,600));
  dialog.add(new DFPButtonDialogControl(1022)); // def path
  if(changecount == 0)
  {
      dialog.add(new CheckBoxDialogControl(124,&autoattachdetach));
      dialog.add(new ButtonDialogControl(125));// (116) attach all figures
      dialog.add(new ButtonDialogControl(126));// (117) detach all figures
      dialog.add(new ButtonDialogControl(127));// (110) missing
  }
  if(changecount == 1)
      dialog.add(new ButtonDialogControl(128));// modify parametric figure
  dialog.add(new ButtonDialogControl(129));// (118) clone
  dialog.add(new ButtonDialogControl(130));// (122) clone all

  /*
   * disable z value input??
  if (program->getlicensed() >= 2)
  {
       dialog.adddisable(102);
       dialog.adddisable(1102);
       dialog.adddisable(103);
       dialog.adddisable(1103);
       dialog.adddisable(104);
       dialog.adddisable(1104);
       dialog.adddisable(202);
       dialog.adddisable(203);
       dialog.adddisable(204);
       scale->z = 1.0;
       rotation->x = rotation->y = 0.0;
  }
  */


  if (change != NULL)
  {
       if(changecount == 1)
          dialog.title(_RCT("Component Verify"));
       else
         dialog.title(rs1.gets());

       v.setinteger("op::change",1);
       v.setbm("op::changemask",*change);

       // debug
       //change->setall();
       //change->clear(4);
       //change->clear(5);
       //change->clearall();

       if(changecount != 0)
       {
           dialog.add(new FigureVerifyButtonControl(1000));
           dialog.add(new FigureVerifyButtonControl(1001));
           dialog.add(new FigureVerifyButtonControl(1002));
           dialog.add(new FigureVerifyButtonControl(1003));
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
           ((RealDialogControl*)dialog.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(103))->setDefaultValue(defWeight);
       }
       // file name
       if(changecount > 1 && !change->test(4))
           dialog.add(new StringDialogControl(119,rsVaries.gets(),strlen(rsVaries.gets())));
       else
           dialog.add(new StringDialogControl(119,sfilename,strlen(sfilename)));


       // alt file name
       if(changecount > 1 && !change->test(4))
           dialog.add(new StringDialogControl(120,rsVaries.gets(),strlen(rsVaries.gets())));
       else
           dialog.add(new StringDialogControl(120,saltfilename,strlen(saltfilename)));

       // scale x
       if(changecount > 1 && !change->test(6))
       {
           //strcpy(sscalex,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(104))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(104))->setDefaultValue(defScale.x);
       }
       // scale y
       if(changecount > 1 && !change->test(7))
       {
           //strcpy(sscaley,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(105))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(105))->setDefaultValue(defScale.y);
       }
       // scale z
       if(changecount > 1 && !change->test(8))
       {
           //strcpy(sscalez,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(106))->setDefaultValue(defScale.z);
       }
       // size x
       if(changecount > 1 && !change->test(20))
       {
           ((RealDialogControl*)dialog.getcontrol(116))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(116))->setDefaultValue(defSize.x);
       }
       // size y
       if(changecount > 1 && !change->test(21))
       {
           ((RealDialogControl*)dialog.getcontrol(117))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(117))->setDefaultValue(defSize.y);
       }
       // size z
       if(changecount > 1 && !change->test(22))
       {
           ((RealDialogControl*)dialog.getcontrol(118))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(118))->setDefaultValue(defSize.z);
       }
       // rotation x
       if(changecount > 1 && !change->test(9))
       {
           //strcpy(srotationx,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defRotation.x);
       }
       // rotation y
       if(changecount > 1 && !change->test(10))
       {
           //strcpy(srotationy,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(108))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(108))->setDefaultValue(defRotation.y);
       }
       // rotation z
       if(changecount > 1 && !change->test(11))
       {
           //strcpy(srotationz,rsVaries.gets());
           ((RealDialogControl*)dialog.getcontrol(109))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(109))->setDefaultValue(defRotation.z);
       }

       // set up the tri state check boxes
       dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[0],((changecount>=1)^change->test(15)),defOptions.test(FigureOriginalLayer)));
       dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[1],((changecount>=1)^change->test(16)),defOptions.test(FigureOriginalColour)));
       dialog.add(new TriStateCheckBoxDialogControl(113,&tristates[2],((changecount>=1)^change->test(17)),defOptions.test(FigureOriginalStyle)));
       dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[3],((changecount>=1)^change->test(18)),defOptions.test(FigureOriginalWeight)));
       dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[4],((changecount>=1)^change->test(19)),defOptions.test(FigureOrientToView)));
       // change->test(24) is set if one ore more figures are selected
       qDebug() << "change bit 24 : " << change->test(24);
       // if there is a change between FigureUseSize options and only one figure is selected ((changecount>=1)^change->test(24)) = 0
       // if there is a change between FigureUseSize options and more than one figure is selected ((changecount>=1)^change->test(24)) = 1
       qDebug() << "((changecount>=1)^change->test(24)) (0=2way;1=3way) : " << ((changecount>=1)^change->test(24));
       dialog.add(dc = new TriStateCheckBoxDialogControl(131,&tristates[5],((changecount>=1)^change->test(24)),defOptions.test(FigureUseSize)));
       qDebug() << "check control 131 type : " << ((TriStateCheckBoxDialogControl*)dc)->isTriState();
       dialog.add(dc = new TriStateCheckBoxDialogControl(132,&tristates[6],((changecount>=1)^change->test(24)),!defOptions.test(FigureUseSize)));
       qDebug() << "check control 132 type : " << ((TriStateCheckBoxDialogControl*)dc)->isTriState();
  }
  else
  {
      v.setinteger("op::change",0);
      v.setbm("op::changemask",0);
      /*
       deltarotationx = v.getreal("fi::deltarotationx");
       dialog.add(dc = new RealDialogControl(116,&deltarotationx));
       deltarotationy = v.getreal("fi::deltarotationy");
       dialog.add(dc = new RealDialogControl(117,&deltarotationy));
       deltarotationz = v.getreal("fi::deltarotationz");
       dialog.add(dc = new RealDialogControl(118,&deltarotationz));
       */
       dialog.add(new StringDialogControl(119,sfilename,strlen(sfilename)+1));
       dialog.add(new StringDialogControl(120,saltfilename,strlen(saltfilename)+1));

       // set up the bi state check boxes
       dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[0],false,defOptions.test(FigureOriginalLayer)));
       dialog.add(new TriStateCheckBoxDialogControl(112,&tristates[1],false,defOptions.test(FigureOriginalColour)));
       dialog.add(new TriStateCheckBoxDialogControl(113,&tristates[2],false,defOptions.test(FigureOriginalStyle)));
       dialog.add(new TriStateCheckBoxDialogControl(114,&tristates[3],false,defOptions.test(FigureOriginalWeight)));
       dialog.add(new TriStateCheckBoxDialogControl(115,&tristates[4],false,defOptions.test(FigureOrientToView)));
       dialog.add(new TriStateCheckBoxDialogControl(131,&tristates[5],false,defOptions.test(FigureUseSize)));
       dialog.add(new TriStateCheckBoxDialogControl(132,&tristates[6],false,!defOptions.test(FigureUseSize)));

  }
  if ((result = dialog.process()) == TRUE)
  {
      if (change != NULL)
      {
          changeIn = *change;
          change->clearall();

          if(changecount == 1)
          {
              for (int i = 0 ; i < 4 ; i++)
                change->set(i,1);

              header->setcolour(c);
              header->setstyle(s);
              header->setlayer(l);
              header->setweight(db.lineweights.mmtoweight(wt));
          }
#if 0
          // clear any change flags
          // filename and altfilename will not be changed
          change->clear(4);
          change->clear(5);
          // use alt file name will not be changed
          change->clear(12);
          // state 1 and state 2 will not be changed
          change->clear(13);
          change->clear(14);
          // size will not be changed
          change->clear(20);
          change->clear(21);
          change->clear(22);
          // elevation will not be changed
          change->clear(23);
          // use size will not be changed
          change->clear(24);
          // copies will not change
          change->clear(25);
#endif
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
              header->setweight(db.lineweights.mmtoweight(wt));
          }

          if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(111))->isTriState())
              change->clear(15);
          else
          {
              change->set(15);
              o->set(FigureOriginalLayer, tristates[0]);
          }
          if(tristates[1] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(112))->isTriState())
              change->clear(16);
          else
          {
              change->set(16);
              o->set(FigureOriginalColour, tristates[1]);
          }
          if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(113))->isTriState())
              change->clear(17);
          else
          {
              change->set(17);
              o->set(FigureOriginalStyle, tristates[2]);
          }
          if(tristates[3] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(114))->isTriState())
              change->clear(18);
          else
          {
              change->set(18);
              o->set(FigureOriginalWeight, tristates[3]);
          }
          if(tristates[4] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(115))->isTriState())
              change->clear(19);
          else
          {
              change->set(19);
              o->set(FigureOrientToView, tristates[4]);
          }
          // FigureUseSize
          // if tristates[5] > 0 use size is checked
          // if tristates[6] > 0 use scale is checked
          // if both are 0 neither were clicked use scale
          // if both are 1 both are undefined make no changes
          if(tristates[5] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(131))->isTriState() &&
             tristates[6] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(131))->isTriState())
              change->clear(24);
          else
          { 
              if(tristates[5] > 0)
              {
                  change->set(24);
                  o->set(FigureUseSize, 1);
              }
              else if(tristates[6] > 0)
              {
                  change->set(24);
                  o->set(FigureUseSize, 0);
              }
              else
                  change->clear(24);
          }
          // scale x
          if(changecount > 1 && !changeIn.test(6) &&
             strlen(((RealDialogControl*)dialog.getcontrol(104))->getSpecialValueText()) != 0)
              change->clear(6);
          else
              change->set(6);
          // scale y
          if(changecount > 1 && !changeIn.test(7) &&
             strlen(((RealDialogControl*)dialog.getcontrol(105))->getSpecialValueText()) != 0)
              change->clear(7);
          else
              change->set(7);
          // scale z
          if(changecount > 1 && !changeIn.test(8) &&
             strlen(((RealDialogControl*)dialog.getcontrol(106))->getSpecialValueText()) != 0)
              change->clear(8);
          else
              change->set(8);

          if(inSize != 0)
              *inSize = size;

          // size x
          if(changecount > 1 && !changeIn.test(20) &&
             strlen(((RealDialogControl*)dialog.getcontrol(116))->getSpecialValueText()) != 0)
              change->clear(20);
          else
              change->set(20);
          // size y
          if(changecount > 1 && !changeIn.test(21) &&
             strlen(((RealDialogControl*)dialog.getcontrol(117))->getSpecialValueText()) != 0)
              change->clear(21);
          else
              change->set(21);
          // size z
          if(changecount > 1 && !changeIn.test(22) &&
             strlen(((RealDialogControl*)dialog.getcontrol(118))->getSpecialValueText()) != 0)
              change->clear(22);
          else
              change->set(22);

          // rotation x
          if(changecount > 1 && !changeIn.test(9) &&
             strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0)
              change->clear(9);
          else
              change->set(9);
          // rotation y
          if(changecount > 1 && !changeIn.test(10) &&
             strlen(((RealDialogControl*)dialog.getcontrol(108))->getSpecialValueText()) != 0)
              change->clear(10);
          else
              change->set(10);
          // rotation z
          if(changecount > 1 && !changeIn.test(11) &&
             strlen(((RealDialogControl*)dialog.getcontrol(109))->getSpecialValueText()) != 0)
              change->clear(11);
          else
              change->set(11);

          v.setreal("fi::deltarotationx",deltarotationx);
          v.setreal("fi::deltarotationy",deltarotationy);
          v.setreal("fi::deltarotationz",deltarotationz);
          if(strcmp(defaultpath,v.getstring("fi::defaultpath")) != 0)
             v.setstring("fi::defaultfigure",_RCT(""));
          v.setstring("fi::defaultpath",defaultpath);

      }
      else
      {
          *inSize = size;
          o->set(FigureOriginalLayer,tristates[0]);
          o->set(FigureOriginalColour,tristates[1]);
          o->set(FigureOriginalStyle,tristates[2]);
          o->set(FigureOriginalWeight,tristates[3]);
          o->set(FigureOrientToView,tristates[4]);
          o->set(FigureUseSize,tristates[5]);

          v.setreal("fi::deltarotationx",deltarotationx);
          v.setreal("fi::deltarotationy",deltarotationy);
          v.setreal("fi::deltarotationz",deltarotationz);
          if(strcmp(defaultpath,v.getstring("fi::defaultpath")) != 0)
             v.setstring("fi::defaultfigure",_RCT(""));
          v.setstring("fi::defaultpath",defaultpath);
      }
      v.setinteger("fi::autoattachdetach",autoattachdetach);
       return TRUE;
  }
  else if (result == 129) // clone
    return 118;
  else if (result == 130)// clone all
    return 122;
  else if (result == 128)// parametric
    return 128;
  else if (result == 127) // missing
  {
       Dialog dialog("Missing_Dialog");
       dialog.add(new MDListDialogControl(100));
       dialog.add(new MDButtonDialogControl(101));
       dialog.add(new ButtonDialogControl(102));
       if (dialog.process() == 102)
       {//  Create text file and view it
           Entity *e;
           RCCHAR outfilename[600];
           strcpy(outfilename,home.getpublicfilename(_RCT("MissingFigures.LST")));
           RCSTRING qfn(outfilename);
           RCFILE outfile(qfn);
           if (outfile.open( QFile::WriteOnly | QIODevice::Text ))
           {
               QTextStream data(&outfile);
               for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
               {
                  if (e->isa(figure_entity) && ((Figure *)e)->getmissing())
                  {
                      data << RCSTRING(((Figure *)e)->getfilename()) << endl;
                  }
               }
               outfile.close();
               RCCHAR cmd[500];
#ifdef _MAC
               strcpy(cmd,_RCT("open -t "));
#else
               strcpy(cmd,_RCT("notepad.exe "));
#endif
               strcat(cmd,outfilename);
               qDebug() << "process start command : " << RCSTRING(cmd);
               WinExec(cmd,SW_NORMAL);
           }
       }
       return FALSE;
  }
  else if (result == 125)// attach all figures
    {Entity *e;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(figure_entity))
           ((Figure *)e)->attachfigure();
       return FALSE; 
    }
  else if (result == 126)// detach all figures
    {Entity *e;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(figure_entity))
           ((Figure *)e)->detachfigure();
       return FALSE; 
    }
  else
    return FALSE;
}

void Figure::getdefaults(Point *scale,Point *rotation,Byte *state1,Byte *state2,BitMask *o, Point *si)
{
  scale->x = v.getreal("fi::scalex");
  scale->y = v.getreal("fi::scaley");
  scale->z = v.getreal("fi::scalez");
  // v8
  if(si != 0)
  {
      v.setreal("fi::sizex",si->x);
      v.setreal("fi::sizey",si->y);
      v.setreal("fi::sizez",si->z);
  }
  else
  {
      v.setreal("fi::sizex",0.0);
      v.setreal("fi::sizey",0.0);
      v.setreal("fi::sizez",0.0);
  }
  //
  rotation->x = v.getreal("fi::rotationx");
  rotation->y = v.getreal("fi::rotationy");
  rotation->z = v.getreal("fi::rotationz");
  *state1 = (Byte) v.getinteger("fi::state1");
  *state2 = (Byte) v.getinteger("fi::state2");
  *o = *v.getbm("fi::options");
}

void Figure::setdefaults(Point scale,Point rotation,Byte state1,Byte state2,const BitMask &o,Point *si)
{
  v.setreal("fi::scalex",scale.x);
  v.setreal("fi::scaley",scale.y);
  v.setreal("fi::scalez",scale.z);
  // v8
  if(si != 0)
  {
      v.setreal("fi::sizex",si->x);
      v.setreal("fi::sizey",si->y);
      v.setreal("fi::sizez",si->z);
  }
  else
  {
      v.setreal("fi::sizex",0.0);
      v.setreal("fi::sizey",0.0);
      v.setreal("fi::sizez",0.0);
  }
  //
  v.setreal("fi::rotationx",rotation.x);
  v.setreal("fi::rotationy",rotation.y);
  v.setreal("fi::rotationz",rotation.z);
  v.setinteger("fi::state1",state1);
  v.setinteger("fi::state2",state2);
  v.setbm("fi::options",o);
}

void Figure::setdefaults(void)
{Point scale,rotation;
  defaultsfrommatrix(Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),&scale,&rotation);
  v.setreal("fi::scalex",scale.x);
  v.setreal("fi::scaley",scale.y);
  v.setreal("fi::scalez",scale.z);
  // v8
  v.setreal("fi::sizex",size.x);
  v.setreal("fi::sizey",size.y);
  v.setreal("fi::sizez",size.z);
  //
  v.setreal("fi::rotationx",rotation.x);
  v.setreal("fi::rotationy",rotation.y);
  v.setreal("fi::rotationz",rotation.z);
  v.setinteger("fi::state1",state1);
  v.setinteger("fi::state2",state2);
  v.setbm("fi::options",options);
}

int Figure::save(CadFile *outfile)
{short type,n;
 RCCHAR exedir[600],lib1[600],name1[600],libcmp[600],namecmp[600];
 Parameter *p;
 double x;
 int i;
  type = figure_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  strcpy(exedir,home.getpublicfilename(""));  _strupr(exedir);
  strcpy(lib1,filename);
  strcpy(libcmp,QDir::toNativeSeparators(QString(filename)));  _strupr(libcmp);
  if (strncmp(exedir,libcmp,strlen(exedir)) == 0)
  {
      strcpy(lib1,"::CADHD::");  strcat(lib1,filename+strlen(exedir));
  }
  qDebug() << "lib1 : " << QString(lib1);
  strcpy(name1,altfilename);
  strcpy(namecmp,QDir::toNativeSeparators(QString(altfilename)));  _strupr(namecmp);
  if (strncmp(exedir,namecmp,strlen(exedir)) == 0)
  {
      strcpy(name1,"::CADHD::");  strcat(name1,filename+strlen(exedir));
  }
  if (! outfile->write(lib1)) return 0;
  if (! outfile->write(name1)) return 0;
  if (! transform.save(outfile)) return 0;
  options.set(FigureUnpacked,! unpacked.empty());
  options.set(FigureAttachFile,attachment[0] != 0 || attachment[1] != 0 || attachment[2] != 0);
  options.set(FigureSaveSizeInfo,ncopies != 0 || options.test(FigureUseSize));
  options.set(FigureHasClipBoundary,nclipboundary != 0);

  if (! outfile->write(&options)) return 0;
  if (options.test(2))
    {  if (! outfile->write(&state1)) return 0;
       if (! outfile->write(&state2)) return 0;
    }
  if (options.test(FigureUnpacked))
    {  if (! unpacked.save(outfile)) return 0;
    }
  if (options.test(FigureParametric))
    {  n = parameterlist.length();
       if (! outfile->write(&n)) return 0;
       for (parameterlist.start() ; (p = (Parameter *)parameterlist.next()) != 0 ; )
         {  if (! outfile->write(p->GetName())) return 0;
            x = p->GetValue();
            if (! outfile->write(&x)) return 0;
         } 
    }

  if (options.test(FigureSaveSizeInfo))
    {  if (! size.save(outfile)) 
         return 0;
       if (! outfile->write(&ncopies))
         return 0;
    }

  if (options.test(FigureHasClipBoundary))
    {  if (! outfile->write(&nclipboundary))
         return 0;
       for (i = 0 ; i < nclipboundary ; i++)
         if (! clipboundary[i].save(outfile)) 
           return 0;
    }

  //  Write out the attached figures if necessary
  if (options.test(FigureAttachFile))
    {  for (i = 0 ; i < 3 ; i++)
         {  if (! outfile->write(&attachmentlength[i])) return 0;
            if (attachmentlength[i] > 0) 
              if (! outfile->blockwrite(attachment[i],attachmentlength[i])) return 0;
         }
    }

  return 1;
}

Entity *Figure::load(int references,CadFile *infile)
{Figure *figure;
 EntityHeader *header;
 Entity *e;
 Point origin,xa,ya,scale,rotation,size;
 Transform transform;
 BitMask options(32);
 EntityList unpacked;
 GeneralList plist;
 RCCHAR *filename,lib1[1000],*name;
 Byte state1,state2;
 short n,ncopies,nclipboundary;
 double x;
 int i;
 Point *clipboundary;

  if ((header = loadheader(infile)) == NULL) 
    return 0;
  if(infile->getversion() == 2)
  {
      char *cfilename=0;
      if (! infile->read(&cfilename)) return 0;
      filename = new RCCHAR[strlen(cfilename)+1];
      strcpy(filename,cfilename);
      delete [] cfilename;
  }
  else
      if (! infile->read(&filename)) return 0;
  // this could be a file, a relative path or a full path
  // fix up the separators
  // from native seperators
  //qDebug() << "figure name : " << QString(filename);
  QString qsfilename(filename);
  QFile figurefile(qsfilename);
  qsfilename.replace("\\","/");
  qsfilename = QDir::toNativeSeparators(qsfilename);
  strcpy(filename,qsfilename);
  //qDebug() << "figure name : " << QString(filename);
  //
  if (strncmp(filename,"::CADHD::",9) == 0)
    strcpy(lib1,home.getpublicfilename(filename+9));
  else
    strcpy(lib1,filename); 
  if(infile->getversion() == 2)
  {
      char *cname=0;
      if (! infile->read(&cname)) return 0;
      name = new RCCHAR[strlen(cname)+1];
      strcpy(name,cname);
      delete [] cname;
  }
  else
      if (! infile->read(&name)) return 0;
  if (! transform.load(infile))
    return 0;
  if (! infile->read(&options)) 
    return 0;
  if (options.test(2))
    {  if (! infile->read(&state1)) 
         return 0;
       if (! infile->read(&state2)) 
         return 0;
    }
  else
    state1 = state2 = 0;
  if (options.test(FigureUnpacked))
    if (! unpacked.load(infile)) 
      return 0;
  if (options.test(FigureParametric))
  {
      if (! infile->read(&n))
         return 0;
       for (i = 0 ; i < n ; i++)
       {
            delete [] name;
           if(infile->getversion() == 2)
           {
               char *cname=0;
               if (! infile->read(&cname)) return 0;
               name = new RCCHAR[strlen(cname)+1];
               strcpy(name,cname);
               delete [] cname;
           }
           else
               if (! infile->read(&name)) return 0;
            if (! infile->read(&x)) 
              return 0;
            plist.add(new Parameter(name,x));
       }
  }

  if (options.test(FigureSaveSizeInfo))
    {  if (! size.load(infile)) 
         return 0;
       if (! infile->read(&ncopies))
         return 0;
    }
  else
    ncopies = 0;


  if (options.test(FigureHasClipBoundary))
    {  if (! infile->read(&nclipboundary))
         return 0;
       clipboundary = new Point[nclipboundary];   
       for (i = 0 ; i < nclipboundary ; i++)
         if (! clipboundary[i].load(infile)) 
           return 0;
    }
  else
    {  nclipboundary = 0;
       clipboundary = 0;
    }

  figure = new Figure(*header,lib1,name,origin,xa,ya,scale,rotation,size,ncopies,state1,state2,&transform,options);
  figure->atlist = header->getatlist();
  figure->unpacked = unpacked;
  figure->SetClipBoundary(nclipboundary,clipboundary);

  //  Read the attached figures if necessary
  if (options.test(FigureAttachFile))
    {  for (i = 0 ; i < 3 ; i++)
         {  if (! infile->read(&figure->attachmentlength[i])) 
              return 0;
            if (figure->attachmentlength[i] > 0) 
              {  figure->attachment[i] = new char[figure->attachmentlength[i]];
                 if (! infile->blockread(figure->attachment[i],figure->attachmentlength[i])) 
                   return 0;

                 //  Update any other figures that share this name.
                 EntityList dblist;
                 GeneralList elist;
                 dblist = *db.geometry.getlist();
                 for (dblist.start() ; (e = dblist.next()) != 0 ; )
                   if (e != figure && e->isa(figure_entity) && _stricmp(((Figure *)e)->getarchitectfilename(i),figure->getarchitectfilename(i)) == 0 &&
                           ((Figure *)e)->attachment[i] == 0)
                     {  ((Figure *)e)->attachment[i] = figure->attachment[i];
                        ((Figure *)e)->attachmentlength[i] = -abs(figure->attachmentlength[i]);  //  -ve length means this figure does not 'own' the attachment files.
                        ((Figure *)e)->loadfigure(&elist);
                     }

              }
            else if (figure->attachmentlength[i] < 0) 
              {  //  Try to find this figures information somewhere else
                 EntityList dblist;
                 dblist = *db.geometry.getlist();
                 for (dblist.start() ; (e = dblist.next()) != 0 ; )
                   if (e != figure && e->isa(figure_entity) && _stricmp(((Figure *)e)->getarchitectfilename(i),figure->getarchitectfilename(i)) == 0 &&
                           ((Figure *)e)->attachment[i] != 0)
                     {  figure->attachment[i] = ((Figure *)e)->attachment[i];
                        figure->attachmentlength[i] = -abs(((Figure *)e)->attachmentlength[i]);  //  -ve length means this figure does not 'own' the attachment files.
                     }
 

              }
         }
    }
  else
    {  figure->attachmentlength[0] = figure->attachmentlength[1] = figure->attachmentlength[2] = 0;
       figure->attachment[0] = figure->attachment[1] = figure->attachment[2] = 0;
    }

  if (! references)
    figure->unpacked.destroy();

  if (v.getinteger("fi::autoattachdetach"))
    {OFSTRUCT ofs;
       //  Detach the figures - but only if the figure actually exists
       if (OpenFile(figure->getcurrentfilename(),&ofs,OF_EXIST) != HFILE_ERROR)
       {
         //qDebug() << "detaching figure : " << QString(figure->getcurrentfilename());
         figure->detachfigure();
       }
       //else
       //    qDebug() << "could not detach figure : " << QString(figure->getcurrentfilename());

    }

  if (options.test(FigureParametric))
    figure->loadfigure(&plist);

  delete header;
  delete [] filename;
  delete [] name;

  return figure;
}

int Figure::save(UndoFile *outfile)
{short type,n;
 long wallindex;
 Entity *e;
 Parameter *p;
 double x;
 delete [] mextents; mextents=0;

  type = figure_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! transform.save(outfile)) return 0;
  if (! outfile->write(&state1)) return 0;
  if (! outfile->write(&state2)) return 0;
  if (! outfile->write(filename)) return 0;
  if (! outfile->write(altfilename)) return 0;
  wallindex = 0;
  for (alist.start() ; (e = alist.next()) != 0 ; )
    if (e->isa(wall_entity))
      wallindex = e->getid();
  if (! outfile->write(&wallindex)) return 0;
  if (! unpacked.save(outfile)) return 0;
  if (options.test(FigureParametric))
    {  n = parameterlist.length();
       if (! outfile->write(&n)) return 0;
       for (parameterlist.start() ; (p = (Parameter *)parameterlist.next()) != 0 ; )
         {  if (! outfile->write(p->GetName())) return 0;
            x = p->GetValue();
            if (! outfile->write(&x)) return 0;
         } 
    }

  return 1;
}

Entity *Figure::load(UndoFile *infile)
{Figure *figure;
 RCCHAR savedata[sizeof(Transform)];
 long wallindex;
 EntityList emptylist;
 GeneralList plist;
 short n;
 int i;
 RCCHAR *name;
 double x;
 RCCHAR bm[sizeof(BitMask)];

  if ((figure = new Figure) != NULL)
    {AttributeList al;
      figure->deleteExtents();
       memcpy(bm,&figure->visible,sizeof(bm));
       memcpy(savedata,&figure->transform,sizeof(Transform));
       if (! infile->blockread(figure,sizeof(*figure))) return 0;
       memcpy(&figure->visible,bm,sizeof(bm));
       if (! infile->read(&figure->visible)) return 0;
       figure->alist = emptylist;
       figure->unpacked = emptylist;
       figure->entitylist[0] = emptylist;
       figure->entitylist[1] = emptylist;
       figure->entitylist[2] = emptylist;
       if (! al.load(infile)) return 0;   figure->atlist = al;
       if (! infile->read(&figure->options)) return 0;
       memcpy(&figure->transform,savedata,sizeof(Transform));      // Fixup new addresses
       if (! figure->transform.load(infile)) return 0;
       if (! infile->read(&figure->state1)) return 0;
       if (! infile->read(&figure->state2)) return 0;
       if (! infile->read(&figure->filename)) return 0;
       if (! infile->read(&figure->altfilename)) return 0;
       if (! infile->read(&wallindex)) return 0;
#if (VERSION != FLEXICAD)
       if (wallindex != 0)
         ((Wall *)db.geometry.getentity(wallindex))->add(figure);
#endif
       figure->unpacked = emptylist;
       if (! figure->unpacked.load(infile)) return 0;

       //  Clear the attachment information
       for (i = 0 ; i < 3 ; i++)
         {  figure->attachment[i] = 0;
            figure->attachmentlength[i] = 0;
         }
       //  Reload the attachments 
       if (figure->options.test(FigureAttachFile))
         figure->attachfigure();

       if (figure->options.test(FigureParametric))
         {  if (! infile->read(&n)) return 0;
            for (i = 0 ; i < n ; i++)
              {  if (! infile->read(&name)) return 0;
                 if (! infile->read(&x)) return 0;
                 plist.add(new Parameter(name,x));
                 delete [] name;
              } 
            figure->loadfigure(&plist);
         }

    }
  return figure;
}

int Figure::saveattributes(CadFile *outfile)
{short n;
 Parameter *p;
 double x;

  if (! outfile->write(filename)) return 0;
  if (! outfile->write(altfilename)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&state1)) return 0;
  if (! outfile->write(&state2)) return 0;
  if (! transform.save(outfile)) return 0;
  if (options.test(FigureParametric))
    {  n = parameterlist.length();
       if (! outfile->write(&n)) return 0;
       for (parameterlist.start() ; (p = (Parameter *)parameterlist.next()) != 0 ; )
         {  if (! outfile->write(p->GetName())) return 0;
            x = p->GetValue();
            if (! outfile->write(&x)) return 0;
         } 
    }
  return 1;
}

int Figure::loadattributes(CadFile *infile)
{short n;
 double x;
 GeneralList plist;
 int i;
 RCCHAR *name;
  delete [] filename;
  delete [] altfilename;
  cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&filename)) return 0;
  if (! infile->read(&altfilename)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&state1)) return 0;
  if (! infile->read(&state2)) return 0;
  if (! transform.load(infile)) return 0;
  if (options.test(FigureParametric))
    {  if (! infile->read(&n)) return 0;
       for (i = 0 ; i < n ; i++)
         {  if (! infile->read(&name)) return 0;
            if (! infile->read(&x)) return 0;
            plist.add(new Parameter(name,x));
            delete [] name;
         } 
       loadfigure(&plist);
    }
  return 1;
}

extern char *FixFigureName(char *n);

int Figure::savedxf(int blocksection,DXFOut *dxffile)
{ if (blocksection)
    {CadFile infile;
     Entity *e;
     int arch,layer;
     double fscale;
     EntityList list;

       fscale = sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0)*transform.mat(1,0) + transform.mat(2,0)*transform.mat(2,0));
       arch = options.test(1);
       index = 0;//options.test(1) && (vs->getwaxis().z <= 0.99999);
       if (options.test(FigureParametric))
         {  index = 0;//options.test(1) && (vs->getwaxis().z <= 0.99999);
            for (entitylist[index].start() ; (e = entitylist[index].next()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if ((!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && e->getvisible()->test(0))// && layertable->test(layer))
                   {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                      //e->getvisible()->set(vs->getview()->getviewno());
                      if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) &&
                          (! e->isa(lineardimension_entity) || strlen(((Dimension *)e)->getequation()) == 0))
                        {  if (e->isa(figure_entity))
                             {  if (! options.test(FigureOriginalColour))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalColour);
                                     e->EntityHeader::change(getcolour(),e->getlayer(),e->getstyle(),e->getweight());
                                  } 
                                if (! options.test(FigureOriginalStyle))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalStyle);
                                     e->EntityHeader::change(e->getcolour(),e->getlayer(),getstyle(),e->getweight());
                                  } 
                                if (! options.test(FigureOriginalWeight))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalWeight);
                                     e->EntityHeader::change(e->getcolour(),e->getlayer(),e->getstyle(),getweight());
                                  } 
                             }
                          if (e->isa(text_entity) && ! ((Text *)e)->getoptions().test(9) && fscale > 0.0)
                            {  ((Text *)e)->ScaleTextHeight(fscale);
                               if (! e->savedxf(0,dxffile))
                                 return 0;
                               ((Text *)e)->ScaleTextHeight(1.0/fscale);
                            }
                          else
                            {  if (! e->savedxf(0,dxffile))
                                 return 0;
                            }
                        }                   
                   }
              }
         }
       else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
         {  for (list.start() ; (e = list.next()) != NULL ; )
              {  if (arch || options.test(FigureOriginalLayer))
                   layer = e->getlayer();
                 else
                   layer = getlayer();  
                 if ((!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && e->getvisible()->test(0))// && layertable->test(layer))
                   {  //e->getvisible()->set(vs->getview()->getviewno());
                      if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) &&
                          (! e->isa(lineardimension_entity) || strlen(((Dimension *)e)->getequation()) == 0))
                        {  if (e->isa(figure_entity))
                             {  if (! options.test(FigureOriginalColour))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalColour);
                                     e->EntityHeader::change(getcolour(),e->getlayer(),e->getstyle(),e->getweight());
                                  } 
                                if (! options.test(FigureOriginalStyle))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalStyle);
                                     e->EntityHeader::change(e->getcolour(),e->getlayer(),getstyle(),e->getweight());
                                  } 
                                if (! options.test(FigureOriginalWeight))
                                  {  ((Figure *)e)->getoptions()->clear(FigureOriginalWeight);
                                     e->EntityHeader::change(e->getcolour(),e->getlayer(),e->getstyle(),getweight());
                                  } 
                             } 
                           if (e->isa(text_entity) && ! ((Text *)e)->getoptions().test(9) && fscale > 0.0)
                             {  ((Text *)e)->ScaleTextHeight(fscale);
                                if (! e->savedxf(0,dxffile))
                                  return 0;
                                ((Text *)e)->ScaleTextHeight(1.0/fscale);
                             }
                           else
                            {  if (! e->savedxf(0,dxffile))
                                 return 0;
                            }
                        }                   
                   }
              }
         }
       else
         {  if (arch && fstyle != -1)
              filename[fstyle] = RCCHAR('2');// + (vs->getwaxis().z <= 0.99999));
            if (! infile.open((char*)attachment[index],abs(attachmentlength[index])) && ! infile.open(filename,"rb"))
              {  missing = 1;  return 0;
              } 
            if (infile.readheader())
              {  while ((e = infile.nextentity(NULL)) != NULL)
                   {  if (arch || options.test(FigureOriginalLayer))
                        layer = e->getlayer();
                      else
                        layer = getlayer();  
                      if ((!arch || arch && (layer < 800 || layer == 800 + state1 || layer == 800+state2)) && e->getvisible()->test(0))// && layertable->test(layer))
                        {  e->EntityHeader::change(options.test(FigureOriginalColour) ? e->getcolour() : getcolour(),e->getlayer(),options.test(FigureOriginalStyle) ? e->getstyle() : getstyle(),options.test(FigureOriginalWeight) ? e->getweight() : getweight());
                           //e->getvisible()->set(vs->getview()->getviewno());
                           if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) &&
                               (! e->isa(lineardimension_entity) || strlen(((Dimension *)e)->getequation()) == 0))
                             {  if (e->isa(figure_entity))
                                  {  if (options.test(FigureOriginalColour))
                                       {  ((Figure *)e)->getoptions()->clear(FigureOriginalColour);
                                          e->EntityHeader::change(getcolour(),e->getlayer(),e->getstyle(),e->getweight());
                                       } 
                                     if (options.test(FigureOriginalStyle))
                                       {  ((Figure *)e)->getoptions()->clear(FigureOriginalStyle);
                                          e->EntityHeader::change(e->getcolour(),e->getlayer(),getstyle(),e->getweight());
                                       } 
                                     if (options.test(FigureOriginalWeight))
                                       {  ((Figure *)e)->getoptions()->clear(FigureOriginalWeight);
                                          e->EntityHeader::change(e->getcolour(),e->getlayer(),e->getstyle(),getweight());
                                       } 
                                  }
                                if (e->isa(text_entity) && ! ((Text *)e)->getoptions().test(9) && fscale > 0.0)
                                  ((Text *)e)->ScaleTextHeight(fscale);
                                if (! e->savedxf(0,dxffile))
                                  return 0;
                             }                   
                        }
                      delete e;
                   }
              }
            infile.close();
         }

    }
  else
    {Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0),scale,rotation;
     char figurename[600];
       defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);
       if (! dxffile->writestring(0, (char*)"INSERT")) return 0;
       strcpy(figurename,FixFigureName(RCSTRING(filename).toLatin1().data()));
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbBlockReference")) return 0;

       if (getparameterlistlength() != 0)
         sprintf(figurename+strlen(figurename),"-%8.8lx",this);
       if (! dxffile->writestring(2,figurename)) return 0;


       xaxis.setxyz(transform.mat(0,0),transform.mat(1,0),transform.mat(2,0));
       yaxis.setxyz(transform.mat(0,1),transform.mat(1,1),transform.mat(2,1));
       zaxis.setxyz(transform.mat(0,2),transform.mat(1,2),transform.mat(2,2));

       double xscale;
       if (xaxis.cross(yaxis).dot(zaxis) < 0.0)
         {//  Figure has been mirrored
            xscale = -1.0;
            rotation.z = 180.0 - rotation.z;
         }
       else
         xscale = 1.0;

       if (! dxffile->writereal(10,transform.mat(0,3) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(20,transform.mat(1,3) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(30,transform.mat(2,3) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(41,scale.x * xscale)) return 0;
       if (! dxffile->writereal(42,scale.y)) return 0;
       if (! dxffile->writereal(43,scale.z)) return 0;
       if (! dxffile->writereal(50,rotation.z)) return 0;
    }

  return 1;
}

int Figure::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}


int Figure::savegeometry(CadFile *outfile)
{ if (! transform.save(outfile)) return 0;
  if (! unpacked.save(outfile)) return 0;
  return 1;
}

int Figure::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! transform.load(infile)) return 0;
  unpacked.destroy();
  if (! unpacked.load(infile)) return 0;
  return 1;
}

int Figure::defaultsfrommatrix(Point,Point,Point *scale,Point *rotation)
{
#if 1
    // v7
  scale->setxyz(sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0) * transform.mat(1,0) + transform.mat(2,0) * transform.mat(2,0)),
                sqrt(transform.mat(0,1)*transform.mat(0,1) + transform.mat(1,1) * transform.mat(1,1) + transform.mat(2,1) * transform.mat(2,1)),
                sqrt(transform.mat(0,2)*transform.mat(0,2) + transform.mat(1,2) * transform.mat(1,2) + transform.mat(2,2) * transform.mat(2,2)));
  rotation->setxyz(0.0,0.0,atan2(-transform.mat(0,1),transform.mat(0,0))/acos(-1.0)*180.0);

  qDebug() << "defaultsfrommatrix";
  qDebug() << "scale x : " << scale->x;
  qDebug() << "scale y : " << scale->y;
  qDebug() << "scale z : " << scale->z;

  qDebug() << "rotation x : " << rotation->x;
  qDebug() << "rotation y : " << rotation->y;
  qDebug() << "rotation z : " << rotation->z;
  qDebug();
#else
    // v8 currently pending
    // ToDo:
    // need to get the reflection matrix out of the input matrix
    // Possible methods:
    // #1 return the figure to the origin and zero rotation in z by reversing the mirror transform:
    //    1. translate to the origin
    //    2. rotate the opposite z rotation of the mirror axis
    //    3.
    //    4. apply this to the figure transformation
    //    5. extract the scaling, rotation and translation from the resulting matrix

    Transform inmat = transform;
    Transform dbmat = inmat;
    qDebug() << "transform for defaults";
    dbmat.debug();
    qDebug();
    scale->setxyz(sqrt(transform.mat(0,0)*transform.mat(0,0) + transform.mat(1,0) * transform.mat(1,0) + transform.mat(2,0) * transform.mat(2,0)),
                  sqrt(transform.mat(0,1)*transform.mat(0,1) + transform.mat(1,1) * transform.mat(1,1) + transform.mat(2,1) * transform.mat(2,1)),
                  sqrt(transform.mat(0,2)*transform.mat(0,2) + transform.mat(1,2) * transform.mat(1,2) + transform.mat(2,2) * transform.mat(2,2)));

    qDebug() << "scale x : " << scale->x;
    qDebug() << "scale y : " << scale->y;
    qDebug() << "scale z : " << scale->z;

    Transform t1,t2,st,sti,rt;

#if 1
    // Method #1,#2 Reflection matrix processing
    // remove the reflection
    Point origin(0.0,0.0,0.0);
    Point xaxis(1.0,0.0,0.0);
    Point yaxis(0.0,1.0,0.0);
    Point zaxis(0.0,0.0,1.0);
    Point raxis(0.0,0.0,1.0); // reflection axis
    Point torigin = transform.transform(origin);
    Point txaxis = transform.transform(xaxis); //txaxis.normalize();
    Point tyaxis = transform.transform(yaxis); //tyaxis.normalize();
    Point tzaxis = transform.transform(yaxis);
    txaxis = (txaxis-torigin).normalize();
    tyaxis = (tyaxis-torigin).normalize();
    tzaxis = txaxis.cross(tyaxis).normalize();
    raxis = (txaxis + xaxis.normalize()).normalize();
    if(txaxis.z == 0.0 & txaxis.y == 0.0 && txaxis.z == 0.0)
        raxis = (tyaxis + yaxis.normalize()).normalize();
    double mirrorAngle =  acos(xaxis.dot(txaxis));
    if(txaxis.y < 0.0)
        mirrorAngle = M_PI + M_PI - mirrorAngle;
    qDebug() << "mirror angle" << mirrorAngle * 180.0 / M_PI;

#if 0
    // no good
    // reflect the figure origin back across the mirror plane
    Point ro1 = getorigin();
    Point ro2 = transform.transform(ro1);
    qDebug() << "ro1 : " << ro1.x << "," << ro1.y << "," << ro1.z;
    qDebug() << "ro2 : " << ro2.x << "," << ro2.y << "," << ro2.z;
    // get the normal vector to the line joining these points
    Point royv = (ro1 - ro2).normalize();
    Point roxv = royv.cross(zaxis).normalize();
    qDebug() << "roxv : " << roxv.x << "," << roxv.y << "," << roxv.z;
    // get the mirror angle between the axis and the normal
    mirrorAngle = acos(xaxis.dot(roxv)) * 2.0;
    if(roxv.x == 0.0 && roxv.y == 0.0 && roxv.z == 0.0)
        mirrorAngle = 0.0;
#endif

    qDebug() << "mirror angle" << mirrorAngle * 180.0 / M_PI;

    Transform m,mi;

#if 0
    if(tzaxis.z < 0.0)
    {
        // recreate the reflection matrix from the origin
        // #1 recreate the reflection parameters from the input matrix and use that to remove the original one
        // does not work
        m.identity();
        Point tp(transform(0,3),transform(1,3),transform(2,3));
        m.mirror(tp,zaxis.normalize().cross(raxis));
        //t1 = m;
        //mi.identity();
        //mi = t1.inverse();
        // #2 recreate the reflection paremeters and create the inverse reflection matrix and apply tht to the input matrix
        //mi.mirror(Point(0.0,0.0,0.0),(-zaxis).normalize().cross(raxis-Point(0.0,0.0,0.0)));
        //mi.mirror(tp,zaxis.cross(-raxis));
        //mi = m.apply(m);
        //dbmat = m;
        //qDebug() << "reflection matrix";
        //dbmat.debug();
        //qDebug();
        //dbmat = mi;
        //qDebug() << "inverted reflection matrix";
        //dbmat.debug();
        //qDebug();

        //t1 = inmat;
        //mi = t1.apply(m);
        //mi = t1 * m;

        //dbmat = mi;
        //qDebug() << "reflection matrix";
        //dbmat.debug();
        //qDebug();

        //t2 = inmat;

        // reverse the translation
        //t1.translate(inmat(0,3),inmat(1,3),inmat(2,3));
        //t1.inverse();

        // reverse the reflection axis rotation
        //t2.identity();
        //t2.rotatez(xaxis.dot(raxis));
        //t2.inverse();
        //t1.apply(t2);

        // revese the reflection around the x axis
        //t2.rotatex(M_PI);
        //t1.apply(t2);

        //t2 = inmat;
        //t2.apply(t1);

        //dbmat = t2;
        //qDebug() << "reversed reflection matrix";
        //dbmat.debug();
        //qDebug();

        Transform tr,r1,r2;
#if 0
        tr.translate(-inmat(0,3),-inmat(1,3),-inmat(2,3));
        //tr.inverse();
        //t1 = inmat;
        t1.identity();
        t1.apply(tr);
        //t1(0,3)=0.0-inmat(0,3);
        //t1(1,3)=0.0-inmat(1,3);
        //t1(2,3)=0.0-inmat(2,3);
        //t1(3,3)=1.0;
        qDebug() << "reverse translation matrix";
        t1.debug();
        qDebug();
        r1.rotatez(-mirrorAngle);
        qDebug() << "reverse rotation matrix";
        r1.debug();
        qDebug();
        t1.apply(r1);
        qDebug() << "applied rotation matrix";
        t1.debug();
        qDebug();
        r2.mirror(Point(0.0,0.0,0.0),zaxis.cross(xaxis));
        qDebug() << "mirror around xaxis matrix";
        r2.debug();
        qDebug();
        t1.apply(r2);
        qDebug() << "applied mirror matrix";
        t1.debug();
        qDebug();
        t2 = inmat;
        t2.apply(t1);
        qDebug() << "neutraliser matrix";
        t2.debug();
        qDebug();
#endif

        t1 = inmat;
        t2 = t1.tocoordinatesystem(xaxis,yaxis,zaxis);
        qDebug() << "to coord";
        t2.debug();
        qDebug();

    }
    else
        t2 = inmat;
#else
    t2 = inmat;
#endif

    // get the scaling matrix
    //t1.scale(scale->x,scale->y,scale->z);
    //st.apply(t1);

    //dbmat = st;
    //qDebug() << "scale matrix";
    //dbmat.debug();
    //qDebug();

    //sti = st.inverse();

    //dbmat = sti;
    //qDebug() << "scale inverse";
    //dbmat.debug();
    //qDebug();

    // remove the translation
    t1 = t2;
    for (int i = 0; i < 3; i++) {
        t1(i,3) = t1(3,i) = 0.0;
    }
    t1(3,3) = 1.0;

    dbmat = t1;
    qDebug() << "no translation";
    dbmat.debug();
    qDebug();

    // remove the scale
    //t2 = t1 * st;
    //t2 = t1;

    //dbmat = t2;
    //qDebug() << "rotation matrix no scale";
    //dbmat.debug();
    //qDebug();

    scale->setxyz(sqrt(t1.mat(0,0)*t1.mat(0,0) + t1.mat(1,0) * t1.mat(1,0) + t1.mat(2,0) * t1.mat(2,0)),
                  sqrt(t1.mat(0,1)*t1.mat(0,1) + t1.mat(1,1) * t1.mat(1,1) + t1.mat(2,1) * t1.mat(2,1)),
                  sqrt(t1.mat(0,2)*t1.mat(0,2) + t1.mat(1,2) * t1.mat(1,2) + t1.mat(2,2) * t1.mat(2,2)));

    // get the scaling matrix
    st.scale(scale->x,scale->y,scale->z);

    dbmat = st;
    qDebug() << "scale matrix";
    dbmat.debug();
    qDebug();

    sti = st.inverse();

    dbmat = sti;
    qDebug() << "scale inverse";
    dbmat.debug();
    qDebug();

    // remove the scale
    t2 = t1 * sti;

    dbmat = t2;
    qDebug() << "rotation matrix no scale";
    dbmat.debug();
    qDebug();

    rt = t2;

    dbmat = rt;
    qDebug() << "rotation matrix";
    dbmat.debug();
    qDebug();

#endif

    // extract the rotaion angles from the rotation matrix
    Transform::extractEulerAngleXYZ(rt,rotation->x,rotation->y,rotation->z);
    //Point transation;
    //Transform::DecomposeTransformationMatrix(transform,transation,*rotation,*scale);

    if(tzaxis.z < 0.0)
    {
        rotation->x = rotation->x;
        rotation->y = rotation->y;
        rotation->z = M_PI - rotation->z;
    }
    /*
    if(tzaxis.z < 0.0)
    {
        Transform::extractEulerAngleXYZ(rt,rotation->x,rotation->y,rotation->z);
        t2.identity();
        t1.rotatez(-rotation->z);
        t2.apply(t1);
        t1.rotatex(M_PI);
        t2.apply(t1);
        rt.apply(t2);
        Transform::extractEulerAngleXYZ(rt,rotation->x,rotation->y,rotation->z);
    }
    else
        Transform::extractEulerAngleXYZ(rt,rotation->x,rotation->y,rotation->z);
    */

    rotation->x *= 180.0 / M_PI;
    rotation->y *= 180.0 / M_PI;
    rotation->z *= 180.0 / M_PI;

    qDebug() << "defaultsfrommatrix";
    qDebug() << "scale x : " << scale->x;
    qDebug() << "scale y : " << scale->y;
    qDebug() << "scale z : " << scale->z;

    qDebug() << "rotation x : " << rotation->x;
    qDebug() << "rotation y : " << rotation->y;
    qDebug() << "rotation z : " << rotation->z;
    qDebug();

#endif
  return 1;
}

/*
// now defined in ncdialog.h
class MPListDialogControl : public ListDialogControl
  {private:
     Figure *figure;
   public:
     MPListDialogControl(Figure *figure_,int id,RCCHAR *name,int length) :
         ListDialogControl(id,0,0,CHAR2RCCHR(name),length)  {  figure = figure_;  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };
*/

void MPListDialogControl::load(Dialog *dialog)
{RCCHAR name[600]={0};
 double value;
  for (figure->firstparameter() ; figure->nextparameter(name,&value) ; )
    additem(dialog,CHAR2RCCHR(name));

  //  Initialise the field to the first parameter.
  figure->firstparameter();
  if (figure->nextparameter(name,&value))
    change(dialog,CHAR2RCCHR(name));

  changefocus(dialog);
}

void MPListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{RCCHAR name1[600],name2[600];
 double value;
  if (updatescreen != 0)
    if (dialog->currentvalue(100,name1,600))
      {  for (figure->firstparameter() ; figure->nextparameter(name2,&value) ; )
           if (strcmp(name1,name2) == 0)
             ((RealDialogControl *) dialog->getcontrol(101))->change(dialog,value);
      }
}

void Figure::verify(void)
{
#ifdef USING_WIDGETS
    // this function now just pops up the figure options dialog
    ::state.sendevent(new NewCommandEvent(413,0));
#else
  Dialog dialog("FigureVerify_Dialog");
  int status,c,l,s;
  double w;
  Point origin,scale,rotation,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
  Entity *e;
  RCCHAR sorigin[300];


  defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);
  origin = getorigin();
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new FigureVerifyButtonControl(1000));
  dialog.add(new FigureVerifyButtonControl(1001));
  dialog.add(new FigureVerifyButtonControl(1002));
  dialog.add(new FigureVerifyButtonControl(1003));

  dialog.add(new StringDialogControl(104,filename,strlen(filename)+1));
  dialog.add(new StringDialogControl(105,altfilename,strlen(altfilename)+1));
  dialog.add(new RealDialogControl(106,&scale.x));
  dialog.add(new RealDialogControl(107,&scale.y));
  dialog.add(new RealDialogControl(108,&scale.z));
  dialog.add(new RealDialogControl(109,&rotation.x));
  dialog.add(new RealDialogControl(110,&rotation.y));
  dialog.add(new RealDialogControl(111,&rotation.z));
  //dialog.add(new ButtonDialogControl(112));
  dialog.add(new ButtonDialogControl(114));
  dialog.add(new CheckBoxDialogControl(113,&options,FigureOriginalLayer));
  dialog.add(new CheckBoxDialogControl(116,&options,FigureOriginalColour));
  dialog.add(new CheckBoxDialogControl(115,&options,FigureOriginalStyle));
  dialog.add(new CheckBoxDialogControl(117,&options,FigureOriginalWeight));
  dialog.add(new CheckBoxDialogControl(119,&options,FigureOrientToView));
  dialog.add(new StringDialogControl(118,origin.string(sorigin),300));

  dialog.setposition(DPOK);
  status = dialog.process();
  if(status == TRUE)
  {
       GeneralList list;
      db.saveundo(UD_CHANGEATTRIBUTES,this);
      draw(DM_ERASE);
      cadwindow->invalidatedisplaylist(this);

      // OK button
      setcolour(c);
      setstyle(s);
      setlayer(l);
      setweight(db.lineweights.mmtoweight(w));

      db.saveundo(UD_STARTBLOCK,NULL);

      draw(DM_ERASE);
      cadwindow->invalidatedisplaylist(this);
      loadfigure(&list);
      draw(DM_NORMAL);
      for (alist.start() ; (e = alist.next()) != 0 ; )
      {
          cadwindow->invalidatedisplaylist(e);
          e->draw(DM_NORMAL);
      }
  }
  else if (status == 112) // to setup dialog not used
    ::state.sendevent(new NewCommandEvent(413,0));
  else if (status == 114)
  {
     //Dialog dialog("MODIFYPARAMETRICFIGURE_DIALOG");
     Dialog dialog("ModifyParametricFigure_Dialog");
     DialogControl *dc;
     GeneralList list;
     double length,value;
     RCCHAR name1[300],name2[300];
       //  Make sure the figure is loaded
       loadfigure(&list);
       dialog.add(new MPListDialogControl(this,100,name1,300));
       dialog.add(dc = new RealDialogControl(101,&length,ScaleByLength));
       //dialog.add(new ScrollBarDialogControl(1100,dc));
       if (dialog.process())
         {  for (firstparameter() ; nextparameter(name2,&value) ; )
              if (strcmp(name1,name2) == 0)
                list.add(new Parameter(name1,length));
              else
                list.add(new Parameter(name2,value));
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            loadfigure(&list);
            draw(DM_NORMAL);
            for (alist.start() ; (e = alist.next()) != 0 ; )
              {  cadwindow->invalidatedisplaylist(e);
                 e->draw(DM_NORMAL);
              } 

         }
    }
#endif
}

void Figure::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{Point origin(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
 RCCHAR text[600];
 EntityList list;
 Point MAXPOINT(-RCREAL_MAX,-RCREAL_MAX,-RCREAL_MAX),MINPOINT(RCREAL_MAX,RCREAL_MAX,RCREAL_MAX);
 if(mextents == 0)
     mextents = new Point[2];

    //qDebug() << "in figure extents for : " << QString(filename);
    // KMJ: NOTE: we are not just returning the origin as the extents anymore
    if ( 1 /*surface == NULL*/)
    {CadFile infile;
     int i,c;
     Point pmin1(MINPOINT),pmax1(MAXPOINT),pmin2(MINPOINT),pmax2(MAXPOINT),p;
     Entity *e;

        if (options.test(FigureParametric) )
/*        {   c = 0;
            if(entitylist[0].length() == 0)
*/
        {
            if(entitylist->length() == 0)
                return;
#if 1
            c = 0;
            for (entitylist[0].end(); (e = entitylist[0].last()) != NULL ; c++)
#else
            for (entitylist[0].end(), c=0 ; (e = entitylist[0].last()) != NULL ; /*c++*/)
#endif
            {   if (c == 0)
                {
#if 1
                    *pmin = MINPOINT; *pmax = MAXPOINT;
                    e->extents(0,pmin,pmax);
#else
                    e->extents(0,&pmin1,&pmax1);
                    if (pmin1.x <= -RCDBL_MAX+0.9e38 || pmax1.x >= RCDBL_MAX-0.9e38 ||
                        pmin1.y <= -RCDBL_MAX+0.9e38 || pmax1.y >= RCDBL_MAX-0.9e38 ||
                        pmin1.z <= -RCDBL_MAX+0.9e38 || pmax1.z >= RCDBL_MAX-0.9e38 )
                    {
                      c--;
                    }
                    else
                    {
                        *pmin = pmin1;
                        *pmax = pmax1;
                    }
#endif
                }
                else
                {
                    pmin1 = MINPOINT; pmax1 = MAXPOINT;
                    e->extents(0,&pmin1,&pmax1);
#if 0
                    if (pmin1.x < pmin->x) pmin->x = pmin1.x;
                    if (pmin1.y < pmin->y) pmin->y = pmin1.y;
                    if (pmin1.z < pmin->z) pmin->z = pmin1.z;
                    if (pmax1.x > pmax->x) pmax->x = pmax1.x;
                    if (pmax1.y > pmax->y) pmax->y = pmax1.y;
                    if (pmax1.z > pmax->z) pmax->z = pmax1.z;
#else
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
                        qDebug("oops?");
#endif
                }
#if 0
                qDebug() << "*** Figure bounding box ****";
                qDebug() << "Ent #   " << c;
                qDebug() << "EntType " << e->type();
                qDebug() << "pmin.x " << pmin->x;
                qDebug() << "pmin.y " << pmin->y;
                qDebug() << "pmin.z " << pmin->z;
                qDebug() << "pmax.x " << pmax->x;
                qDebug() << "pmax.y " << pmax->y;
                qDebug() << "pmax.z " << pmax->z;
                qDebug() << "****************************";
#endif
                //c++;
            }
        }
        else if (options.test(FigureLoaded) && loadedfigures.load(getcurrentfilename(),&list))
        {
            c = 0;
            for (list.end() ; (e = list.last()) != NULL ; c++)
            {
                if (c == 0)
                {
                    *pmin = MINPOINT; *pmax = MAXPOINT;
                    e->extents(0,pmin,pmax);
                }
                else
                { 
                    pmin1 = MINPOINT; pmax1 = MAXPOINT;
                    e->extents(0,&pmin1,&pmax1);
#if 0
                    if (pmin1.x < pmin->x) pmin->x = pmin1.x;
                    if (pmin1.y < pmin->y) pmin->y = pmin1.y;
                    if (pmin1.z < pmin->z) pmin->z = pmin1.z;
                    if (pmax1.x > pmax->x) pmax->x = pmax1.x;
                    if (pmax1.y > pmax->y) pmax->y = pmax1.y;
                    if (pmax1.z > pmax->z) pmax->z = pmax1.z;
#else
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
                        qDebug("oops?");

#endif
                }
                c++;
            }
        }
        else
        {   if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb"))
                return;
            if (infile.readheader())
            {   c = 0;
                while ((e = infile.nextentity(NULL)) != NULL)
                {   if (c == 0)
                    {
                        *pmin = MINPOINT; *pmax = MAXPOINT;
                        e->extents(0,pmin,pmax);
                    }
                    else
                    {
                        pmin1 = MINPOINT; pmax1 = MAXPOINT;
                        e->extents(0,&pmin1,&pmax1);
#if 0
                        if (pmin1.x < pmin->x) pmin->x = pmin1.x;
                        if (pmin1.y < pmin->y) pmin->y = pmin1.y;
                        if (pmin1.z < pmin->z) pmin->z = pmin1.z;
                        if (pmax1.x > pmax->x) pmax->x = pmax1.x;
                        if (pmax1.y > pmax->y) pmax->y = pmax1.y;
                        if (pmax1.z > pmax->z) pmax->z = pmax1.z;
#else
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
                            qDebug("oops?");

                        /*
                        if(c == 11)
                        {
                            qDebug("oops?");

                            pmin1 = MINPOINT; pmax1 = MAXPOINT;
                            e->extents(0,&pmin1,&pmax1);
                        }
                        */
#endif
                    }
#if 0
                if (pmin1.x <= -RCDBL_MAX+0.9e38 || pmax1.x >= RCDBL_MAX-0.9e38 ||
                    pmin1.y <= -RCDBL_MAX+0.9e38 || pmax1.y >= RCDBL_MAX-0.9e38)
                {
                    qDebug() << "*** Figure bounding box ****";
                    qDebug() << "Ent #   " << c;
                    qDebug() << "EntType " << e->type();
                    qDebug() << "pmin.x " << pmin->x;
                    qDebug() << "pmin.y " << pmin->y;
                    qDebug() << "pmin.z " << pmin->z;
                    qDebug() << "pmax.x " << pmax->x;
                    qDebug() << "pmax.y " << pmax->y;
                    qDebug() << "pmax.z " << pmax->z;
                    qDebug() << "****************************";
                    qDebug() << "oops?";
                    pmin1 = MINPOINT; pmax1 = MAXPOINT;
                    e->extents(0,&pmin1,&pmax1);
                    qDebug() << "oops?";
                }
#endif
                    delete e;
                    e=0;
                    c++;
                }
                infile.close();
            }
            else 
            {   infile.close();
                strcpy(text,filename);
                _strlwr(text);
                if (strstr(text,".cad") == 0)
                {//  Insert a bitmap 
                    pmin->setxyz(-1.0,-1.0,0.0);
                    pmax->setxyz( 1.0, 1.0,0.0);
                }
                else
                    return;
            }  
            infile.close();
        }

        for (i = 0 ; i < 8 ; i++)
        {   p.setxyz(i / 4 == 0 ? pmax->x : pmin->x,(i / 2) % 2 == 0 ? pmax->y : pmin->y,(i / 4) % 2 == 0 ? pmax->z : pmin->z);
            p = transform.transform(p);
            // debug
            //p = transform.identity().transform(p);
            //transform.dump("Figure Matrix Debug");
            // debug
            if (i == 0)
                pmax1 = pmin1 = p;
            else
            {   if (p.x < pmin1.x) pmin1.x = p.x;
                if (p.y < pmin1.y) pmin1.y = p.y;
                if (p.z < pmin1.z) pmin1.z = p.z;
                if (p.x > pmax1.x) pmax1.x = p.x;
                if (p.y > pmax1.y) pmax1.y = p.y;
                if (p.z > pmax1.z) pmax1.z = p.z;
            }
        }

        *pmin = pmin1;
        *pmax = pmax1;

        mextents[0] = *pmin; mextents[1] = *pmax;
        //qDebug() << "figure extents min x: " << pmin->x << "y: " << pmin->y << "z: " << pmin->z;
        //qDebug() << "figure extents max x: " << pmax->x << "y: " << pmax->y << "z: " << pmax->z;
        return;
    }

    *pmin = *pmax = surface->modeltouv(origin);
    delete [] mextents; mextents=0;
}

int Figure::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(figure_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

LoadedFigureInfo::LoadedFigureInfo(const RCCHAR *l,EntityList el)
{ if ((filename = new RCCHAR[strlen(l)+1]) != NULL)
    strcpy(filename,l);
  list = el;
}

LoadedFigureInfo::~LoadedFigureInfo()
{Entity *e;
  delete [] filename;
  for (list.start() ; (e = list.next()) != NULL ; )
    delete e;
  list.destroy();
}

int LoadedFigures::load(const RCCHAR *l,EntityList *el)
{LoadedFigureInfo *lfi;
 RCCHAR *filename;
 CadFile infile;
 EntityList flist;
 Entity *e;
 RCCHAR text[600];

  for (list.start() ; (lfi = list.next()) != NULL ; )
    {  filename = lfi->getfilename();
       if (filename != NULL && strcmp(filename,l) == 0)
         {  *el = lfi->getlist();
            return 1;
         }
    }
  if (infile.open(l,"rb")) 
    {  if (infile.readheader())
         {  while ((e = infile.nextentity(NULL)) != NULL)
              flist.add(e);
         }
       infile.close();
    }
  else
    {  strcpy(text,filename);
       _strlwr(text);
       if (strstr(text,".cad") == 0)
         {//  Insert a bitmap 
          BitMask o(32);
            o.set(BitmapELinkFile,1);
            o.set(BitmapETransparent,1);
            flist.add(new BitMapE(filename,Point(-1.0,-1.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),2.0,2.0,1.0,1.0,1.0,0.1,1.0,o));
         }
       else
         return 0;
    }  

  if ((lfi = new LoadedFigureInfo(l,flist)) != NULL)
    {  list.add(lfi);
       *el = flist;
       return 1;
    }
  else
    {  for (flist.start() ; (e = flist.next()) != NULL ; )
         delete e;
       flist.destroy();
       return 0;
    }
}

LoadedFigures::~LoadedFigures()
{LoadedFigureInfo *lfi;
  while (list.length() > 0)
    {  list.start();
       lfi = list.next();
       delete lfi;
       list.del(lfi);
    }
}

void LoadedFigures::clear(void)
{LoadedFigureInfo *lfi;
  while (list.length() > 1)
    {  list.start();
       lfi = list.next();
       delete lfi;
       list.del(lfi);
    }
}

LoadedFigures::LoadedFigures()
{
  LoadedFigureInfo *lfi;
  EntityList flist;
  Point p1,p2,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
  Entity *e;
  EntityHeader header(0,0,0,0);
  p1.setxyz(-15.0,-10.0,5.0);  p2.setxyz( 15.0,-10.0,5.0);
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.z = -5.0;  p2.z = -5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p2 = p1;  p2.z = 5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.setxyz( 15.0,-10.0,5.0);  p2.setxyz( 15.0, 10.0,5.0);
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.z = -5.0;  p2.z = -5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p2 = p1;  p2.z = 5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.setxyz( 15.0, 10.0,5.0);  p2.setxyz(-15.0, 10.0,5.0);
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.z = -5.0;  p2.z = -5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p2 = p1;  p2.z = 5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.setxyz(-15.0, 10.0,5.0);  p2.setxyz(-15.0,-10.0,5.0);
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.z = -5.0;  p2.z = -5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p2 = p1;  p2.z = 5.0;
  e = new Line(header,p1,p2);  if (e != NULL) flist.add(e);
  p1.setxyz(-7.5,0.0,5.0);
  e = new Circle(header,p1,xaxis,yaxis,5.0,0.0,M_PI * 2.0);  if (e != NULL) flist.add(e);
  p1.setxyz( 7.5,0.0,5.0);
  e = new Circle(header,p1,xaxis,yaxis,5.0,0.0,M_PI * 2.0);  if (e != NULL) flist.add(e);

  if ((lfi = new LoadedFigureInfo(_RCT("__The__Special__Figure__"),flist)) != NULL)
    list.add(lfi);
  else
    {
      for (flist.start() ; (e = flist.next()) != NULL ; )
         delete e;
       flist.destroy();
    }

  // KMJ a 3d axis figure
  // x axis
  EntityList alist;
  p1.setxyz(0.0,0.0,0.0);  p2.setxyz( 5.0,0.0,0.0);
  e = new Line(header,p1,p2);  if (e != NULL) alist.add(e);
  // y axis
  p1.setxyz(0.0,0.0,0.0);  p2.setxyz( 0.0,5.0,0.0);
  e = new Line(header,p1,p2);  if (e != NULL) alist.add(e);
  // z axis
  p1.setxyz(0.0,0.0,0.0);  p2.setxyz( 0.0,0.0,5.0);
  e = new Line(header,p1,p2);  if (e != NULL) alist.add(e);

  if ((lfi = new LoadedFigureInfo(_RCT("__The__3DAxis__Figure__"),alist)) != NULL)
    list.add(lfi);
  else
    {  for (alist.start() ; (e = alist.next()) != NULL ; )
         delete e;
       alist.destroy();
    }
}

int Figure::explode(void)
{CadFile infile;
 EntityList list;
 Transform t = transform;
 Entity *e,*estart;
 delete [] mextents; mextents=0;// for display LOD

  if (! infile.open((char*)attachment[0],abs(attachmentlength[0])) && ! infile.open(getcurrentfilename(),"rb")) return 0;
  if (infile.readheader())
    {  db.geometry.start();
       estart = db.geometry.next();
       db.loadgeometry(&infile,&t);
       for (db.geometry.start() ; (e = db.geometry.next()) != estart ; )
         e->setvisible(*getvisible());
    }
  infile.close();
  return 1;
}

EntityList Figure::getentitylist(void)
{EntityList list;
 CadFile infile;
 Entity *e,*eclone;
 int n;
 Entity ** index;


  if (options.test(FigureParametric))
    {
       //  Clone the existing entity list and return that
       for (entitylist[0].start() ; (e = entitylist[0].next()) != NULL ; )
         {  e->setstatus(1);
            e->setcopye(0);
         }

       for (entitylist[0].start() ; (e = entitylist[0].next()) != NULL ; )
         if ((eclone = e->clone(transform)) != 0)
            list.add(eclone);

       list.reverse();
  
       return list;
    }

  if (infile.open((char*)attachment[0],abs(attachmentlength[0])) || infile.open(getcurrentfilename(),"rb") != 0)
    {  if (infile.readheader())
         {  n = 0;
            while ((e = infile.nextentity((Database *)1)) != NULL)
              {  list.add(e);
                 e->setindex(++n);
              }  
         }
       infile.close();

       index = new Entity * [list.length()+1];
       for (list.start() ; (e = list.next()) != NULL ; )
         index[e->getindex()] = e;
       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->fixupreferences(index);
            e->dbinsert(0);
         }
       delete [] index;

       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->setstatus(1);
            e->setcopye(0);
         }

       for (list.start() ; (e = list.next()) != NULL ; )
         e->move(0,transform,0);

    }  
  return list;
         
}

/***
 * get the full count of entities in the figure
 * this is not the length of the entity list
 * it includes the count in figure entities too
 */
int Figure::getentitycount()
{
    int count=0;
    Entity *e=0;
    for (entitylist[0].start() ; (e = entitylist[0].next()) != NULL ; )
    {
        count++;
        if(e->isa(figure_entity))
            count += ((Figure*)e)->getentitycount();
    }
    return count;
}

void Figure::loadfigure(GeneralList *list)
{CadFile infile;
 int i,j,n;
 Entity *e;
 Dimension *dimension;
 Point p,origin,direction;
 Transform t;
 EntityList stretchlist;
 double length,currentlength;
 Entity ** index;
 Parameter *pv1,*pv2;
 GeneralList originallist;
 RCCHAR text[600],*at;
 AttributeList atlist;

 
  options.set(FigureParametric);
  for (list->start() ; (pv1 = (Parameter *)list->next()) != 0 ; )
    {  for (parameterlist.start() ; (pv2 = (Parameter *)parameterlist.next()) != 0 ; )
         if (strcmp(pv1->GetName(),pv2->GetName()) == 0)
           {  pv2->SetValue(pv1->GetValue());
              delete pv1;   
              break;
           }
       if (pv2 == 0)
         parameterlist.add(pv1);
    }
  list->destroy();

  //  
  strcpy(text,filename);
  _strlwr(text);
  if (strstr(text,".cad") == 0)
    {//  Insert a bitmap 
     BitMask o(32);

       entitylist[0].destroy();
       o.set(BitmapELinkFile,1);
       o.set(BitmapETransparent,1);
       entitylist[0].add(new BitMapE(filename,Point(-1.0,-1.0+options.test(FigureElevation),0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),2.0,2.0-options.test(FigureElevation),1.0,1.0,1.0,0.1,1.0,o));
      
    } 
  else 
   for (i = 0 ; i < (options.test(1) ? 3 : 1) ; i++)
    {  entitylist[i].destroy();
       if (fstyle != -1)
         filename[fstyle] = i == 0 ? '2' : i == 1 ? '3' : 'S';

       if (infile.open((char*)attachment[i],abs(attachmentlength[i])) || infile.open(getcurrentfilename(),"rb") != 0)
         {  if (infile.readheader())
              {  n = 0;
                 while ((e = infile.nextentity((Database *)1)) != NULL)
                   {  entitylist[i].add(e);
                      e->setindex(++n);
                      atlist = e->getatlist().copy();
                      for (atlist.start() ; (at = atlist.next()) != 0 ; )
                        if (_stricmp(at,"RC::ConstructionEntity") == 0)
                          e->getvisible()->set(0,0);
                   }
                 missing = 0;
              }  
         }
       else if (i == 0)
         missing = 1;
       infile.close();

       index = new Entity * [entitylist[i].length()+1];
       for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
         index[e->getindex()] = e;
       for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
         {  e->fixupreferences(index);
            e->dbinsert(0);
         }
       delete [] index;

       //  Get the original list of parameter values for this figure
       originallist.destroy();
       for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
         if (e->isdimension() && strpbrk(((Dimension *)e)->getequation(),_RCT(" +-/*()")) == 0 && strlen(((Dimension *)e)->getequation()) > 0)
           originallist.add(new Parameter(((Dimension *)e)->getequation(),(((Dimension *)e)->point(2) - ((Dimension *)e)->point(1)).dot(((Dimension *)e)->getxaxis())));

       for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
         if (e->isdimension() && strlen(((Dimension *)e)->getequation()) != 0)
           stretchlist.add(e);

       for (parameterlist.start() ; (pv1 = (Parameter *)parameterlist.next()) ; )
         {//  Now stretch the figure, changing one parameter value at a time.
            for (originallist.start() ; (pv2 = (Parameter *) originallist.next()) != 0 ; )
              if (strcmp(pv1->GetName(),pv2->GetName()) == 0)
                pv2->SetValue(pv1->GetValue());
            
            for (stretchlist.start() ; (e = stretchlist.next()) != 0 ; )
              {StringExpression expr(((Dimension *)e)->getequation(),&originallist);
               SortedPointTable sptable;
                 dimension = (Dimension *) e;
                 if (! expr.expression(&length))
                   continue;  //  Don't stretch the dimension if the equation cannot be evaluated.
 
                 if (length > 0)
                   {  origin = dimension->point(2);
                      direction = dimension->getxaxis();
                      currentlength = (dimension->point(2) - dimension->point(1)).dot(dimension->getxaxis());
                   }
                 else 
                   {  origin = dimension->point(1);
                      direction = dimension->getxaxis();
                      currentlength = -(dimension->point(2) - dimension->point(1)).dot(dimension->getxaxis());
                   }

                 for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
                   {  e->setstatus(0);
                      e->setcopye(0);
                      for (j = 0 ; j < e->npoints() ; j++)
                        {  p = e->point(j);
                           if (fabs((p - origin).dot(direction)) < db.getptoler())
                             sptable.add(p);
                        }
                   } 

                 sptable.endlist();
                 t.translate(direction * (length - currentlength));

                 for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
                   e->stretch(0,t,&sptable,0);

                 for (entitylist[i].start() ; (e = entitylist[i].next()) != NULL ; )
                   if (e != e->getcopye() && e->getcopye() != NULL)
                     {  entitylist[i].add(e->getcopye());
                        if (e->getalist().empty())
                          {AttributeList elist;
                             e->getcopye()->substituteat(e->getatlist().copy());
                             e->substituteat(elist);
                             entitylist[i].del(e);
                          }
                     }
              } 
         }

       stretchlist.destroy();
    }

}

void Figure::firstparameter()
{ if (options.test(Figure2D3D))
    entitylist[1].start();
  else
    entitylist[0].start();
}

int Figure::nextparameter(RCCHAR *name,double *value)
{ //  Make a list of all the dimensions that have a zero length equation that is a simple variable name.
 Entity *e;
 EntityList *list;

  if (options.test(Figure2D3D))
    list = & entitylist[1];
  else
    list = & entitylist[0];

  while ((e = list->next()) != NULL)
    if (e->isdimension() && strpbrk(((Dimension *)e)->getequation(),_RCT(" +-/*()")) == 0 && strlen(((Dimension *)e)->getequation()) > 0)
      {  strcpy(name,((Dimension *)e)->getequation());
         *value = (((Dimension *)e)->point(2) - ((Dimension *)e)->point(1)).dot(((Dimension *)e)->getxaxis());
         return 1;
      }

  return 0;

}

int Figure::pickoriginpoint(View3dSurface *vs,int snap,int x,int y,Point *pos)
{double d1;
 int end;

  end = 0;
  if ((d1 = vs->pick(Point(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3)),snap,x,y)) >= 0.0)
    {  end = 3;  *pos = Point(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));
    }

  return end;
}

int Figure::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1,d2;
 int end;
 Transform original,ident;
 Point p,pmin,pmax;
 delete [] mextents; mextents=0; // for display LOD

  end = 0;
  if ((d1 = vs->pick(getorigin(),distance,x,y)) >= 0.0)
    {  end = 1;  *pos = getorigin();
    }

  original = transform;
  transform = ident;
  extents(0,&pmin,&pmax);
  transform = original;
  p = transform.transform(Point(pmax.x,0.0,0.0));
  if ((d2 = vs->pick(p,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = p;  d1 = d2;
    }
  return end;
}

void Figure::movehandlepoint(View3dSurface *,int drawentity,int end,Point p)
{Point p1,p2,p3,p4,pmin,pmax;
 Transform t,original,ident;
 double s;

  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity)
      Entity::draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);

  p1 = p3 = getorigin();
  original = transform;
  transform = ident;
  extents(0,&pmin,&pmax);
  transform = original;
  p2 = p4 = transform.transform(Point(pmax.x,0.0,0.0));

  if (end == 1)
    p3 = p;
  else
    p4 = p;

  t.translate(-p1);
  transform.apply(t);
  if ((p2 - p1).cross(p4 - p3).length() > 1.0E-5)
    {  t.rotate(Point(0.0,0.0,0.0),((p2 - p1).cross(p4 - p3)).normalize(),acos((p2-p1).normalize().dot((p4 - p3).normalize())));
       transform.apply(t);
    }
  if ((p2 - p1).length() > 1.0E-5 && (p4 - p3).length() > 1.0E-5)
    {  s = (p4 - p3).length() / (p2 - p1).length();
       t.scale(s,s,s);
       // v8 adjust the size
       if(options.test(FigureUseSize))
       {
           size.x *= s;
           size.y *= s;
           size.z *= s;
       }
       //
       transform.apply(t);
    }
  t.translate(p3);
  transform.apply(t);
  
  if (drawentity) 
    Entity::draw(DM_NORMAL);

  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
  delete [] mextents; mextents=0; // for display LOD

}

void Figure::attachfigure(void)
{FILE *infile;
 int i,found;
 Entity *e;
 EntityList dblist;

  for (i = 0 ; i < (options.test(1) ? 3 : 1) ; i++)
    {  if (fstyle != -1)
         filename[fstyle] = i == 0 ? '2' : i == 1 ? '3' : 'S';

       if (attachmentlength[i] != 0)
         continue;  //  No need to reattach the attachment


       dblist = *db.geometry.getlist();
       for (dblist.start(),found = 0 ; (e = dblist.next()) != 0 ; )
         if (e != this && e != this && e->isa(figure_entity) && _stricmp(((Figure *)e)->getarchitectfilename(i),filename) == 0 &&
               ((Figure *)e)->attachment[i] != 0)
           {  attachment[i] = ((Figure *)e)->attachment[i];
              attachmentlength[i] = -abs(((Figure *)e)->attachmentlength[i]);  //  -ve length means this figure does not 'own' the attachment files.
              found = 1;
              break; 
           }

       if (found)
         continue; 

       if ((infile = fopen(getcurrentfilename(),"rb")) != 0)
         {  // Detach the figure - only if the figure was found
            if (attachmentlength[i] > 0)
              delete attachment[i];
            attachment[i] = 0;
            attachmentlength[i] = 0;

            //  Attach the figure  
            fseek(infile,0L,SEEK_END);
            attachmentlength[i] = ftell(infile);
            fseek(infile,0L,SEEK_SET);
            attachment[i] = new char[attachmentlength[i]];
            fread(attachment[i],1,attachmentlength[i],infile);
            fclose(infile);
         }

    }

}

void Figure::detachfigure(void)
{int i,j,matched;
 Entity *e;

    for (i = 0 ; i < 3 ; i++)
    {   if (attachmentlength[i] > 0)
        {EntityList list;
            list = *db.geometry.getlist();
            matched = 0;
            for (list.start() ; ! matched && (e = list.next()) != 0 ; )
                if (e->isa(figure_entity) && e != this)
                {   for (j = 0 ; j < 3 ; j++)
                        if (attachment[i] == ((Figure *)e)->attachment[j] && attachmentlength[i] == -((Figure *)e)->attachmentlength[j])
                        {   ((Figure *)e)->attachmentlength[j] = -((Figure *)e)->attachmentlength[j];  //  Make this figure the owner of the attachment
                            matched = 1;
                            break;
                        }
                }

            if (! matched)
                delete [] attachment[i];
        }

        attachment[i] = 0;
        attachmentlength[i] = 0;
    }
}

Point Figure::getscale(void)
{Point xaxis(1.0,0.0,0.0),yaxis(0.0,0.0,0.0),scale,rotation;
#if 1
  defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);
#else
  // v8
  Point translation;
  Transform::DecomposeTransformationMatrix(transform,translation,rotation,scale);
#endif
  return scale;  
}

Point Figure::getrotation(void)
{Point xaxis(1.0,0.0,0.0),yaxis(0.0,0.0,0.0),scale,rotation;
#if 1
  defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);

#else
    // v8
    Point translation;
    Transform::DecomposeTransformationMatrix(transform,translation,rotation,scale);
    rotation.x *= 180.0 / M_PI;
    rotation.y *= 180.0 / M_PI;
    rotation.z *= 180.0 / M_PI;
#endif
  return rotation;  
}

//#ifdef _USING_QTCHAR
//int Figure::process_property(int action,int id,void *data,RCCHAR *text)
//#else
int Figure::process_property(int action,int id,void *data,RCCHAR *text)
//#endif
{BitMask change_mask(32),new_options(32);
 int result,cstate;
 double wt;
 RCCHAR new_filename[600],new_name[600];

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
        else if (id == 17)
        {   strcpy(new_filename,filename);
            v.setstring("fi::defaultfigure",filename);
            if (select(new_filename,new_name))
            {   change_mask.set(4);
                change(change_mask,*this,new_filename,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
            }
        }
        else if (id >= 12 && id <= 16)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else
            return 1;

    }
    else if (action == PP_GETVALUE)
    {Point scale,rotation,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
        defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);
        if (id == 1)
            sprintf(text,"%d",getcolour());
        else if (id == 2)
            sprintf(text,"%d",getlayer());
        else if (id == 3)
            sprintf(text,"%d",getstyle());
        else if (id == 4)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (id == 6)
            sprintf(text,"%.4lf",scale.x);
        else if (id == 7)
            sprintf(text,"%.4lf",scale.y);
        else if (id == 8)
            sprintf(text,"%.4lf",scale.z);
        else if (id == 9)
            sprintf(text,"%.4lf",rotation.x);
        else if (id == 10)
            sprintf(text,"%.4lf",rotation.y);
        else if (id == 11)
            sprintf(text,"%.4lf",rotation.z);
        else if (id == 12)
            strcpy(text,options.test(FigureOriginalLayer) ? "Yes" : "No");
        else if (id == 13)
            strcpy(text,options.test(FigureOriginalStyle) ? "Yes" : "No");
        else if (id == 14)
            strcpy(text,options.test(FigureOriginalColour) ? "Yes" : "No");
        else if (id == 15)
            strcpy(text,options.test(FigureOriginalWeight) ? "Yes" : "No");
        else if (id == 16)
            strcpy(text,options.test(FigureOrientToView) ? "Yes" : "No");
        else if (id == 17)
        {   if (strrchr(filename,'\\') != 0)
                strcpy(text,strrchr(filename,'\\')+1);
            else
                strcpy(text,filename);
            if (strrchr(text,'.') != 0)
                *strrchr(text,'.') = 0;
        }

    }
    else if (action == PP_SETVALUE)
    {Point scale,rotation,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);

        defaultsfrommatrix(xaxis,yaxis,&scale,&rotation);

        if (id == 6)
        {   change_mask.set(6,1);
            scale.x = atof(text) / scale.x;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 7)
        {   change_mask.set(7,1);
            scale.y = atof(text) / scale.y;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 8)
        {   change_mask.set(8,1);
            scale.z = atof(text) / scale.z;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 9)
        {   change_mask.set(9,1);
            rotation.x = atof(text) - rotation.x;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 10)
        {   change_mask.set(10,1);
            rotation.y = atof(text) - rotation.y;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 11)
        {   change_mask.set(11,1);
            rotation.z = atof(text) - rotation.z;
            change(change_mask,*this,0,0,scale,rotation,Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 12)
        {   change_mask.set(15,1);
            new_options.set(FigureOriginalLayer,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 13)
        {   change_mask.set(17,1);
            new_options.set(FigureOriginalStyle,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 14)
        {   change_mask.set(16,1);
            new_options.set(FigureOriginalColour,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 15)
        {   change_mask.set(18,1);
            new_options.set(FigureOriginalWeight,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
        }
        else if (id == 16)
        {   change_mask.set(19,1);
            new_options.set(FigureOrientToView,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0,Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,new_options);
        }

    }
    else if (action == PP_GETLABEL) 
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Figure");
            return 1;
        }
        else 
        {   if (id == 1)
                strcpy(text,"Colour");
            else if (id == 2)
                strcpy(text,"Layer");
            else if (id == 3)
                strcpy(text,"Style");
            else if (id == 4)
                strcpy(text,"Weight"); 
            else if (id == 6)
                strcpy(text,"X Scale");
            else if (id == 7)
                strcpy(text,"Y Scale");
            else if (id == 8)
                strcpy(text,"Z Scale");
            else if (id == 9)
                strcpy(text,"X Rotation");
            else if (id == 10)
                strcpy(text,"Y Rotation");
            else if (id == 11)
                strcpy(text,"Z Rotation");
            else if (id == 12)
                strcpy(text,"Original Layer");
            else if (id == 13)
                strcpy(text,"Original Style");
            else if (id == 14)
                strcpy(text,"Original Colour");
            else if (id == 15)
                strcpy(text,"Original Weight");
            else if (id == 16)
                strcpy(text,"Orient To View");
            else if (id == 17)
                strcpy(text,"Name");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 18;
    
    return 0;
}




