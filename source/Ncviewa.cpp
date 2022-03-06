
#include "ncwin.h"

PlotScales DefaultPlotScales[] = 
  {
    //{QString("Fit To Paper"),1.0}, pending
    {QString("1:1"),1.0},
    {QString("1:2"),2.0},
    {QString("1:4"),4.0},
    {QString("1:8"),8.0},
    {QString("1:10"),10.0},
    {QString("1:16"),16.0},
    {QString("1:20"),20.0},
    {QString("1:30"),30.0},
    {QString("1:40"),40.0},
    {QString("1:50"),50.0},
    {QString("1:100"),100.0},
    {QString("1:200"),200.0},
    {QString("1:500"),500.0},
    {QString("1:1000"),1000.0},
    {QString("2:1"),1.0/2.0},
    {QString("4:1"),1.0/4.0},
    {QString("8:1"),1.0/8.0},
    {QString("10:1"),1.0/10.0},
    {QString("100:1"),1.0/100.0},
    {QString("1/128\"=1'-0\"(1:1536)"),12.0 * 128.0},
    {QString("1/64\"=1'-0\"(1:768)"),12.0 * 64.0},
    {QString("1/32\"=1'-0\"(1:384)"),12.0 * 32.0},
    {QString("1/16\"=1'-0\"(1:192)"),12.0 * 16.0},
    {QString("3/32\"=1'-0\"(1:128)"),12.0 * 32.0 / 3.0},
    {QString("3/16\"=1'-0\"(1:52)"),12.0 * 16.0 / 3.0},
    {QString("1/4\"=1'-0\"(1:48)"),12.0 * 4.0},
    {QString("1/8\"=1'-0\"(1:96)"),12.0 * 8.0},
    {QString("1/2\"=1'-0\"(1:24)"),12.0 * 2.0},
    {QString("3/4\"=1'-0\"(1:16)"),12.0 * 4.0 / 3.0},
    {QString("1\"=1'-0\"(1:12)"),12.0},
    {QString("1-1/2\"=1'-0\"(1:8)"),12.0 * 2 /3.0},
    {QString("3\"=1'-0\"(1:4)"),12.0 / 3.0},
    {0,0.0}

    /*
     _RCT("1:1"),2.0,
     _RCT("1:2"),2.0,
     _RCT("1:4"),4.0,
     _RCT("1:8"),8.0,
     _RCT("1:10"),10.0,
     _RCT("1:16"),16.0,
     _RCT("1:20"),20.0,
     _RCT("1:30"),30.0,
     _RCT("1:40"),40.0,
     _RCT("1:50"),50.0,
     _RCT("1:100"),100.0,
     _RCT("2:1"),1.0/2.0,
     _RCT("4:1"),1.0/4.0,
     _RCT("8:1"),1.0/8.0,
     _RCT("10:1"),1.0/10.0,
     _RCT("100:1"),1.0/100.0,
     _RCT("1/128\" = 1'-0\""),12.0 * 128.0,
     _RCT("1/64\" = 1'-0\""),12.0 * 64.0,
     _RCT("1/32\" = 1'-0\""),12.0 * 32.0,
     _RCT("1/16\" = 1'-0\""),12.0 * 16.0,
     _RCT("3/32\" = 1'-0\""),12.0 * 32.0 / 3.0,
     _RCT("1/8\" = 1'-0\""),12.0 * 8.0,
     _RCT("3/16\" = 1'-0\""),12.0 * 16.0 / 3.0,
     _RCT("1/4\" = 1'-0\""),12.0 * 4.0,
     _RCT("1/8\" = 1'-0\""),12.0 * 8.0,
     _RCT("1/2\" = 1'-0\""),12.0 * 2.0,
     _RCT("3/4\" = 1'-0\""),12.0 * 4.0 / 3.0,
     _RCT("1\" = 1'-0\""),12.0,
     _RCT("1-1/2\" = 1'-0\""),12.0 * 2 /3.0,
     _RCT("3\" = 1'-0\""),12.0 / 3.0,
     0,0.0
         */
  };

RCCHAR *MatchPlotScale(double plotscale)
{static RCCHAR label[300];
 int i;

  for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
    if (fabs(DefaultPlotScales[i].scale - plotscale) < 1.0e-6)
      break;

  if (DefaultPlotScales[i].label == 0)
    {  sprintf(label,"1:%.2lf",plotscale);
       return label;
    }
  else
    return DefaultPlotScales[i].label.data();
  
}


View::View(const RCCHAR *n,int persp,int vn,Point e,Point r,Point u,double f,double b)
{ if ((name = new RCCHAR[strlen(n)+1]) != NULL)
    strcpy(name,n);
  else
    name = _RCT("(Null)");
  eye = e;
  ref = r;
  up = u;
  front = f;
  back = b;
  perspective = short(persp);
  viewno = short(vn);
  workplanename = 0;
  selectionlayertable = 0;
  displaylayertable = 0;
  lockedlayertable = 0;
  colour = layer = style = weight = -1;
}

View::View(const RCCHAR *n,int persp,int vn,double ex,double ey,double ez,double rx,double ry,double rz,double ux,double uy,double uz,double f,double b)
{
  if ((name = new RCCHAR[strlen(n)+1]) != NULL)
    strcpy(name,n);
  else
     name = _RCT("(Null)");
  eye.setxyz(ex,ey,ez);
  ref.setxyz(rx,ry,rz);
  up.setxyz(ux,uy,uz);
  front = f;
  back = b;
  perspective = short(persp);
  viewno = short(vn);
  workplanename = 0;
  selectionlayertable = 0;
  displaylayertable = 0;
  lockedlayertable = 0;
  colour = layer = style = weight = -1;
}

View::View(const RCCHAR *n,int view,int vn)
{ if ((name = new RCCHAR[strlen(n)+1]) != NULL)
    strcpy(name,n);
  else
     name = _RCT("(Null)");
  standard(view);
  viewno = short(vn);
  workplanename = 0;
  selectionlayertable = 0;
  displaylayertable = 0;
  lockedlayertable = 0;
  colour = layer = style = weight = -1;
}

View::~View()
{ if (strcmp(name,_RCT("(Null)")) != 0)
	 delete [] name;
}

void View::setname(RCCHAR * n)
{
    delete [] name;
    if ((name = new RCCHAR[strlen(n)+1]) != NULL)
        strcpy(name,n);
    else
         name = _RCT("(Null)");
}

void View::standard(int view)
{
    /*
    ResourceString rs0(NCVIEWA+view);
    delete [] name;
    if ((name = new RCCHAR[strlen(rs0.gets())+1]) != NULL)
        strcpy(name,rs0.gets());
      else
         name = _RCT("(Null)");
    */
    switch (view)
     {  case 0 : // Top
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   0.0,  0.0,  1.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  1.0,  0.0);
          break;
        case 1 : // Bottom
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   0.0,  0.0, -1.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  1.0,  0.0);
          break;
          case 2 : // Front
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   0.0, -1.0,  0.0);
          ref.setxyz(   0.0,  0.0,  0.0);
			 up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 3 : // Back
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   0.0,  1.0,  0.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 4 : // Left
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(  -1.0,  0.0,  0.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  0.0,  1.0);
          break;
          case 5 : // Right
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   1.0,  0.0,  0.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 6 : // Iso
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(   1.0, -1.0,  1.0);
          ref.setxyz(   0.0,  0.0,  0.0);
			 up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 7 : // Parallel
          perspective = 0;  front = -1.0E6;   back = 1.0E6;
          eye.setxyz(  -0.5,  0.2,  1.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 8 : // Perspective
          perspective = 1;  front = -117.0;   back = 10000.0;
          eye.setxyz(-100.0, 40.0, 50.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  0.0,  1.0);
          break;
        case 9 : // Perspective top
          perspective = 1;  front = -210.0;   back = 10000.0;
          eye.setxyz(   0.0,  0.0,  200.0);
          ref.setxyz(   0.0,  0.0,  0.0);
          up.setxyz (   0.0,  1.0,  0.0);
          break;
      }
}

void View::setworkplanename(RCCHAR *n)
{ delete [] workplanename;  
  workplanename = 0;
  if (n != 0)
    {  workplanename = new RCCHAR[strlen(n) + 1];
       strcpy(workplanename,n);
    }
}

int View::changeview(Point neye,Point nref,Point nup)
{Point p1;
  p1 = neye - nref;
  if (p1.length() < db.getptoler()) return 0;
  if (nup.length() < db.getptoler()) return 0;
  p1.normalize();  nup.normalize();
  if (p1.cross(nup).length() < db.getatoler()) return 0;
  if (perspective && front < 0.0)
    front = - ((neye - nref).length() * -front / (eye - ref).length());
  eye = neye;  ref = nref;  up = nup;
  return 1;
}

int View::changeview(int which,Point p)
{ switch (which)
    {  case 0 : return changeview(p,ref,up);
       case 1 : return changeview(eye,p,up);
       case 2 : return changeview(eye,ref,p);
    }
  return 1;
}

View *View::load(CadFile *infile)
{
 char *cname;
 RCCHAR *name,*workplanename;
 Point eye,ref,up;
 double front,back;
 short perspective,viewno;
 View *view;
 BitMask *options;
 BitMask *selectionlayertable;
 BitMask *displaylayertable;
 BitMask *lockedlayertable;
 short colour,layer,style,weight;
 
  if( infile->getversion() > 2 )
  {
      if (! infile->read(&name))
          return NULL;
  }
  else
  {
      if (! infile->read(&cname)) return NULL;
      name = new RCCHAR[strlen(cname)+1];
      strcpy(name,cname);
      delete [] cname;
  }
  if (! eye.load(infile)) return NULL;
  if (! ref.load(infile)) return NULL;
  if (! up.load(infile)) return NULL;
  if (! infile->read(&front)) return NULL;
  if (! infile->read(&back)) return NULL;
  if (! infile->read(&perspective)) return NULL;
  if (! infile->read(&viewno)) return NULL;
  if ((view = new View(name,perspective,viewno&~VW_LAYERINFO,eye,ref,up,front,back)) == 0) return 0;
  // KMJ temp file version -2 from Qt verion will be 3
  if (infile->getversion() >= 2 || infile->getversion() < 0)
    if (! view->zonelist.load(infile)) return 0;
  if (viewno & VW_LAYERINFO)
    {  options = new BitMask(32);
       displaylayertable = selectionlayertable = lockedlayertable = 0;   
       workplanename = 0;
       if (! infile->read(options)) return 0;
       if (options->test(0))
         {  displaylayertable = new BitMask(MAX_LAYERS);
            if (! infile->read(displaylayertable)) return 0;
            view->setdisplaylayertable(displaylayertable);
         }
       if (options->test(1))
         {  selectionlayertable = new BitMask(MAX_LAYERS);
            if (! infile->read(selectionlayertable)) return 0;
            view->setselectionlayertable(selectionlayertable);
         }
       if (options->test(2))
         {  lockedlayertable = new BitMask(MAX_LAYERS);
            if (! infile->read(lockedlayertable)) return 0;
            view->setlockedlayertable(lockedlayertable);
         }
       if (options->test(3))
         {  if (! infile->read(&workplanename)) return 0;
         }
          
       if (! infile->read(&colour)) return 0;
       if (! infile->read(&layer)) return 0;
       if (! infile->read(&style)) return 0;
       if (! infile->read(&weight)) return 0;
       viewno &= ~VW_LAYERINFO;
       view->setcolour(colour);
       view->setlayer(layer);
       view->setstyle(style);
       view->setweight(weight);
    }
  
  delete [] name;
  return view;
}

int View::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! eye.save(outfile)) return 0;
  if (! ref.save(outfile)) return 0;
  if (! up.save(outfile)) return 0;
  if (! outfile->write(&front)) return 0;
  if (! outfile->write(&back)) return 0;
  if (! outfile->write(&perspective)) return 0;
  if (selectionlayertable != 0 || displaylayertable != 0 || lockedlayertable != 0 || workplanename != 0)
    viewno |= VW_LAYERINFO;
  if (! outfile->write(&viewno)) return 0;
  if (! zonelist.save(outfile)) return 0;
  if (viewno & VW_LAYERINFO)
    {BitMask options(32);
       options.set(0,displaylayertable != 0);
       options.set(1,selectionlayertable != 0);
       options.set(2,lockedlayertable != 0);
       options.set(3,workplanename != 0);
       if (! outfile->write(&options)) return 0;
       if (options.test(0)) 
         {  if (! outfile->write(displaylayertable)) return 0;
         }
       if (options.test(1)) 
         {  if (! outfile->write(selectionlayertable)) return 0;
         }
       if (options.test(2)) 
         {  if (! outfile->write(lockedlayertable)) return 0;
         }
       if (options.test(3)) 
         {  if (! outfile->write(workplanename)) return 0;
         }
       if (! outfile->write(&colour)) return 0;
       if (! outfile->write(&layer)) return 0;
       if (! outfile->write(&style)) return 0;
       if (! outfile->write(&weight)) return 0;
       viewno &= ~VW_LAYERINFO;

    }

  return 1;
}

int View::load(UndoFile *infile)
{ if (! eye.load(infile)) return 0;
  if (! ref.load(infile)) return 0;
  if (! up.load(infile)) return 0;
  if (! infile->read(&front)) return 0;
  if (! infile->read(&back)) return 0;
  if (! infile->read(&perspective)) return 0;
  return 1;
}

int View::save(UndoFile *outfile)
{ if (! eye.save(outfile)) return 0;
  if (! ref.save(outfile)) return 0;
  if (! up.save(outfile)) return 0;
  if (! outfile->write(&front)) return 0;
  if (! outfile->write(&back)) return 0;
  if (! outfile->write(&perspective)) return 0;
  return 1;
}

int View::changeview(int persp)
{ perspective = short(persp);
  if (perspective && -front > (eye - ref).length())
    front = - (eye - ref).length() * 0.999;
  return 1;
}

int View::changeclip(double f,double b)
{double f1;
  if (fabs(f - b) < 1.0E-5)
    return 0;
  if (perspective)
	 {  if (-f < (eye -ref).length())
         f1 = f;
       else
         f1 = -0.999 * (eye-ref).length();
       if (fabs(f1 - b) < 1.0E-5)
         return 0;
       front = f1;
		 back = b;
    }
  else
    {  front = f;  back = b;
    }
  return 1;
}

Zone::Zone(RCCHAR *n,double u1,double v1,double u2,double v2)
{ if ((name = new RCCHAR[strlen(n)+1]) != NULL)
	 strcpy(name,n);
  else
     name = _RCT("(Null)");
  umin = u1;  vmin = v1;
  umax = u2;  vmax = v2;
}

void Zone::change(double u1,double v1,double u2,double v2)
{ umin = u1;  vmin = v1;
  umax = u2;  vmax = v2;
}

Zone::~Zone()
{ if (strcmp(name,"(Null)") != 0)
	 delete [] name;
}

Zone *Zone::load(CadFile *infile)
{
 char *cname;
 RCCHAR *name;
 double umin,vmin,umax,vmax;
 Zone *zone;
 if(infile->getversion() == 2)
 {
     if (! infile->read(&cname)) return 0;
     name = new RCCHAR[strlen(cname)+1];
     strcpy(name,cname);
     delete [] cname;

 }
 else if(infile->getversion() >= 3)
 {
     if (! infile->read(&name)) return 0;
 }
  if (! infile->read(&umin)) return 0;
  if (! infile->read(&vmin)) return 0;
  if (! infile->read(&umax)) return 0;
  if (! infile->read(&vmax)) return 0;
  zone = new Zone(name,umin,vmin,umax,vmax);
  delete [] name;
  return zone;
}

int Zone::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;
  return 1;
}

int ZoneList::save(CadFile *outfile)
{short n;
 Zone *zone;
  n = short(length());  outfile->write(&n);
  for (start() ; (zone = next()) != 0 ; )
    if (! zone->save(outfile)) return 0;
  return 1;
}

int ZoneList::load(CadFile *infile)
{Zone *zone;
 short i,n;
  infile->read(&n);
  for (i = 0 ; i < n ; i++)
    {  if ((zone = Zone::load(infile)) == 0)
         return 0;
       add(zone);
    }
  reverse();
  return 1;
}

void Views3d::clear(void)
{View *v;
  for (list.start() ; (v = list.next()) != NULL ; ) delete v;
  list.destroy();
}

void Views3d::initialize(void)
{ResourceString rs0(NCVIEWA);
 ResourceString rs1(NCVIEWA+1);
 ResourceString rs2(NCVIEWA+2);
 ResourceString rs3(NCVIEWA+3);
 ResourceString rs4(NCVIEWA+4);
 ResourceString rs5(NCVIEWA+5);
 ResourceString rs6(NCVIEWA+6);
 ResourceString rs7(NCVIEWA+7);
 ResourceString rs8(NCVIEWA+8);
  clear();
  add(new View(rs0.gets(),0,0));
  add(new View(rs1.gets(),1,1));
  add(new View(rs2.gets(),2,2));
  add(new View(rs3.gets(),3,3));
  add(new View(rs4.gets(),4,4));
  add(new View(rs5.gets(),5,5));
  add(new View(rs6.gets(),6,6));
  add(new View(rs7.gets(),7,7));
  add(new View(rs8.gets(),8,8));
}

int Views3d::load(CadFile *infile,Database *database)
{View *v;
 short nviews;
 int i;
  if (! infile->read(&nviews)) return 0;
  for (i = 0 ; i < nviews ; i++)
    {  
       if ((v = View::load(infile)) == NULL)
           return 0;
       database->views3d.add(v);
    }
  database->views3d.list.reverse();
  return 1;
}

int Views3d::save(CadFile *outfile)
{short type;
 View *v;
 short nviews;
  type = view3d_record;
  if (! outfile->write(&type)) return 0;
  nviews = short(list.length());
  if (! outfile->write(&nviews)) return 0;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (! v->save(outfile)) return 0;
  return 1;
}

View *Views3d::match(const char *name)
{
    return match(CHAR2RCCHR(name));
}

View *Views3d::match(const RCCHAR *name)
{View *v;
  for (list.start() ; (v = list.next()) != NULL ; )
  {
      //qDebug() << "view match : view name :" << QString(v->getname());
      //qDebug() << "view match : name      :" << QString(name);
    if (strcmp(v->getname(),name) == 0)
        return v;
  }
  return NULL;
}

View *Views3d::match(View *view)
{View *v;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (v == view) return v;
  return NULL;
}

View *Views3d::match(const int viewno)
{View *v;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (v->getviewno() == viewno) return v;
  return NULL;
}

void Views3d::names(RCCHAR ***namelist,int *n,int *index)
{View *v;
  for (list.start(),*n = 0 ; (v = list.next()) != NULL ; (*n)++);
  if ((*namelist = new RCCHAR *[*n]) != NULL)
    {  *index = *n - 1;
       for (list.start(),*n = 0 ; (v = list.next()) != NULL ; (*n)++)
         {  (*namelist)[*n] = v->getname();
            if (cadwindow != 0 && cadwindow->getcurrentsurface() != NULL &&
                cadwindow->getcurrentsurface()->getview()->getviewno() == v->getviewno())
              *index = *n;
         }
    }
}

OneView::OneView(View *v,double umin1,double vmin1,double umax1,double vmax1,
                         double s,double xp,double yp,double a,const BitMask &o,BitMask *lt) : options(32)
{
  view = v;

  if (umin1 < umax1)
    {  umin = umin1;  umax = umax1;
    }
  else
    {  umin = umax1;  umax = umin1;
    }

  if (vmin1 < vmax1)
    {  vmin = vmin1;  vmax = vmax1;
    }
  else
    {  vmin = vmax1;  vmax = vmin1;
    }

  scale = s;
  xpos = xp;  ypos = yp;
  angle = a;
  options = o;
  layertable = lt;

  qDebug() << "o 0" << o.test(0);
  qDebug() << "o 2" << o.test(2);

  qDebug() << "options 0" << options.test(0);
  qDebug() << "options 2" << options.test(2);
}

void OneView::setclip(double umin1,double vmin1,double umax1,double vmax1)
{ umin = umin1;  vmin = vmin1;  umax = umax1;  vmax = vmax1;
}

void OneView::setposition(double xp,double yp)
{ xpos = xp;  ypos = yp;
}

void OneView::setscale(double s)
{ scale = s;
}

int OneView::verify(Database *db,View2d *view2d)
{
#ifdef USING_WIDGETS
    // this function now just pops up the viewport change dialog
    state.sendevent(new NewCommandEvent(556,0));
    return 0;
#else
  Dialog dialog(_RCT("Oneviewverify_Dialog"));
 int status,layersdefined;
 double x1,y1,x2,y2;
  x1 = xpos + umin / scale;
  y1 = ypos + vmin / scale;
  x2 = xpos + umax / scale;
  y2 = ypos + vmax / scale;
  layersdefined = layertable != 0;
  dialog.add(new StringDialogControl(100,view->getname(),strlen(view->getname())+1));
  dialog.add(new RealDialogControl(101,&xpos,ScaleByLength));
  dialog.add(new RealDialogControl(102,&ypos,ScaleByLength));
  dialog.add(new RealDialogControl(103,&x1,ScaleByLength));
  dialog.add(new RealDialogControl(104,&y1,ScaleByLength));
  dialog.add(new RealDialogControl(105,&x2,ScaleByLength));
  dialog.add(new RealDialogControl(106,&y2,ScaleByLength));
  dialog.add(new RealDialogControl(107,&scale));
  dialog.add(new RealDialogControl(113,&angle));
  dialog.add(new CheckBoxDialogControl(108,&options,0));
  dialog.add(new ButtonDialogControl(109));
  dialog.add(new ButtonDialogControl(110));
  dialog.add(new CheckBoxDialogControl(111,&layersdefined));
  dialog.add(new CheckBoxDialogControl(112,&options,2));
  dialog.add(new CheckBoxDialogControl(114,&options,OVDashHidden));
  status = dialog.process();
  if (status == 109)
    return change(db,view2d);
  else if (status == 110)
    return 2;
  else
    return 0;
#endif
}

class OVLayerMaskDialogControl : public CustomDialogControl
  {public:
     int scrollid;
     int width,height,base,nacross,ndown,n,lastselected,direction,scroll;
     BitMask *layertable;
     OVLayerMaskDialogControl(int id1,int id2,int n1,int n2,int n3,BitMask *lt) :
       CustomDialogControl(id1)
         {  scrollid = id2;
            width = height = base = scroll = 0;  lastselected = -1;  layertable = lt;
            n = n1;  nacross = n2;  ndown = n3;
         }
     void set(Dialog *,int);
     void draw(Dialog *,RCHDC);
     void draw(Dialog *,RCHDC,int,int);
     void lbuttondownevent(Dialog *,int,int,int);
     void lbuttonupevent(Dialog *,int,int,int);
     void mousemoveevent(Dialog *,int,int,int);
     void timerevent(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
     void setbase(Dialog *,int);
  };

void OVLayerMaskDialogControl::set(Dialog *dialog,int value)
{ if (value)
    layertable->setall();
  else
    layertable->clearall();
  draw(dialog,NULL);
}

void OVLayerMaskDialogControl::draw(Dialog *dialog,RCHDC hDC,int box,int drawmode)
{int newdc;
 RECT rect;
 RCHFONT hfont,holdfont;
 double x,y,w,h,xo,yo;
 RCCHAR text[300];
  if (box < base || box >= base + nacross * ndown) return;
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  w = double(width - 1) / double(nacross);
  h = double(height - 1) / double(ndown);
  x = w * int((box - base) % nacross);
  y = h * int((box - base) / nacross);
  rect.left = int(x + 3);  rect.top = int(y + 3);
  rect.right = int(w * (int((box - base) % nacross) + 1) - 2);
  rect.bottom = int(h * (int((box - base) / nacross) + 1) - 2);
  if (drawmode == DM_NORMAL)
    {  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       SetTextColor(hDC,RGB(0,0,0));
    }
  else
    {RCHBRUSH hbrush;
       hbrush = CreateSolidBrush(RGB(0,0,255));
       FillRect(hDC,&rect,hbrush);
       DeleteObject(hbrush);
       SetTextColor(hDC,RGB(255,255,255));
    }
  strcpy(text,db.layernames.getlayername(box));
  hfont = CreateFont(-GetDeviceCaps(hDC,LOGPIXELSY)/9,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,_RCT("MS Sans Serif"));
  if (hfont != 0)
    holdfont = (RCHFONT)SelectObject(hDC,hfont);
#if defined(WIN32)
  SIZE size;
  GetTextExtentPoint(hDC,text,strlen(text),&size);
  xo = w / 2 - size.cx / 2;
  GetTextExtentPoint(hDC,_RCT("0"),1,&size);
  yo = h / 2 - size.cy / 2;
#else
  long te;
  te = GetTextExtent(hDC,text,strlen(text));
  xo = w / 2 - LOWORD(te) / 2;
  te = GetTextExtent(hDC,_RCT("0"),1);
  yo = h / 2 - HIWORD(te) / 2;
#endif
  SetBkMode(hDC,TRANSPARENT);
  TextOut(hDC,int(xo+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  sprintf(text,"%d",box);
  TextOut(hDC,int(5+(width-1) * double((box - base) % nacross) / double(nacross)),
              int(yo+(height-1) * double((box - base) / nacross) / double(ndown)),text,strlen(text));
  if (hfont != 0)
    {  SelectObject(hDC,holdfont);
       DeleteObject(hfont);
    }
  SetBkMode(hDC,OPAQUE);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void OVLayerMaskDialogControl::draw(Dialog *dialog,RCHDC hDC)
{int i,j,k,newdc;
 RCHPEN holdpen,hpen;
 RECT rect;
  if (width == 0 || height == 0)
    {  GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       width = rect.right;
       height = rect.bottom;
    }
  if (hDC == NULL)
    {  hDC = GetDC(GetDlgItem(dialog->gethdlg(),id));  newdc = 1;
    }
  else
    newdc = 0;
  rect.left = 0;  rect.top = 0;  rect.right = width;  rect.bottom = height;
  FillRect(hDC,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  hpen = CreatePen(PS_SOLID,1,RGB(0,0,0));
  holdpen = (RCHPEN)SelectObject(hDC,hpen);
  for (i = 0 ; i <= nacross ; i++)
    {  MoveToEx(hDC,int((width-1) * double(i) / double(nacross)),0,0);
       LineTo(hDC,int((width-1) * double(i) / double(nacross)),height);
    }
  for (i = 0 ; i <= ndown ; i++)
    {  MoveToEx(hDC,0,int((height-1) * double(i) / double(ndown)),0);
       LineTo(hDC,width,int((height-1) * double(i) / double(ndown)));
    }
  for (i = 0 ; i < ndown ; i++)
    for (j = 0 ; j < nacross ; j++)
      {  k = base + i * nacross + j;
         draw(dialog,hDC,k,(layertable->test(k) ?  DM_SELECT : DM_NORMAL));
      }
  SelectObject(hDC,holdpen);
  DeleteObject(hpen);
  if (newdc) ReleaseDC(GetDlgItem(dialog->gethdlg(),id),hDC);
}

void OVLayerMaskDialogControl::lbuttondownevent(Dialog *dialog,int x,int y,int)
{ SetCapture(hWnd);
  lastselected = base + int(double(y)/double(height)*double(ndown)) * nacross + int(double(x)/double(width)*double(nacross));
  layertable->toggle(lastselected);
  draw(dialog,NULL,lastselected,(layertable->test(lastselected) ? DM_SELECT : DM_NORMAL));
  direction = -1;
}

void OVLayerMaskDialogControl::mousemoveevent(Dialog *dialog,int x,int y,int buttons)
{int i,k,d,cx,cy;
  if (lastselected < 0) return;
  if (! (buttons & MK_LBUTTON))
    {  lastselected = -1;  return;
    }
  cx = int(double(x)/double(width)*double(nacross));
  cy = int(double(y)/double(height)*double(ndown));
  if (y < 0) cy--;
  if (cx < 0) cx = 0; else if (cx >= nacross) cx = nacross - 1;
  if (cy < 0)
    {  cy = 0;
       if (base > 0)
         {  scroll = -nacross;
            vscrollevent(dialog,0,SB_LINEUP,0);
            cy = 0;
         }
       else
         scroll = 0;
    }
  else if (cy >= ndown)
    {  cy = ndown - 1;
       if (base < n - nacross*ndown)
         {  scroll = nacross;
            vscrollevent(dialog,0,SB_LINEDOWN,0);
            cy = ndown - 1;
         }
       else
         scroll = 0;
    }
  else
    scroll = 0;
  k = base + cy * nacross + cx;
  if (k == lastselected) return;
  d = k > lastselected;
  for (i = k ; i != lastselected ; (d ? i-- : i++))
    {  layertable->toggle(i);
       draw(dialog,NULL,i,(layertable->test(i) ?  DM_SELECT : DM_NORMAL));
    }
  if (direction != -1 && d != direction)
    {  layertable->toggle(lastselected);
       draw(dialog,NULL,lastselected,(layertable->test(lastselected) ?  DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
  direction = d;
}

void OVLayerMaskDialogControl::timerevent(Dialog *dialog)
{int i,k;
  if (scroll == 0)
    return;
  else if (scroll < 0)
    {  if (base > 0)
         {  vscrollevent(dialog,0,SB_LINEUP,0);
            k = lastselected + scroll;
            if (k < 0) k = 0;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  else
    {  if (base < n - nacross * ndown)
         {  vscrollevent(dialog,0,SB_LINEDOWN,0);
            k = lastselected + scroll;
            if (k >= n) k = n-1;
         }
       else
         {  scroll = 0;
            return;
         }
    }
  for (i = k ; i != lastselected ; (i > lastselected ? i-- : i++))
    {  layertable->toggle(i);
       draw(dialog,NULL,i,(layertable->test(i) ? DM_SELECT : DM_NORMAL));
    }
  lastselected = k;
}

void OVLayerMaskDialogControl::lbuttonupevent(Dialog *,int,int,int)
{ ReleaseCapture();
  lastselected = -1;
}

void OVLayerMaskDialogControl::changefocus(Dialog *,int)
{ lastselected = -1;
}

void OVLayerMaskDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int pos)
{int start,end;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : base = n - ndown * nacross;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : base += nacross;  break;
       case SB_LINEUP : base -= nacross;  break;
       case SB_PAGEDOWN : base += nacross * ndown / 2;  break;
       case SB_PAGEUP : base -= nacross * ndown / 2;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         base = int((n / nacross - ndown) * double(pos - start) / double(end - start)) * nacross;
         break;
    }
  if (base > n - ndown * nacross) base = n - ndown * nacross;
  if (base < 0) base = 0;
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);
  draw(dialog,NULL);
}

void OVLayerMaskDialogControl::setbase(Dialog *dialog,int b)
{int start,end,pos;

  base = b;
  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  pos = int(start + (end - start) * double(base) / double(n - ndown * nacross));
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,pos,TRUE);

  draw(dialog,NULL);

}



class OVLayerMaskButtonDialogControl : public ButtonDialogControl
  {public:
     OVLayerMaskButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int OVLayerMaskButtonDialogControl::select(Dialog *dialog)
{OVLayerMaskDialogControl *ldc;

  if (id >= 105 && id <= 114)
    {  ((OVLayerMaskDialogControl *)dialog->getcontrol(100))->setbase(dialog,(id - 105)*100);
    }
  else
    {  if ((ldc = (OVLayerMaskDialogControl *)dialog->getcontrol(100)) == NULL) return 0;
       if (id == 101)
         ldc->set(dialog,255);
       else
         ldc->set(dialog,0);
    }

  return 0;
}

class OVButtonDialogControl : public ButtonDialogControl
{private:
   BitMask **layertable;
 public:
   OVButtonDialogControl(int i,BitMask **lt) :
       ButtonDialogControl(i) {  layertable = lt;  }
   int select(Dialog *);
};

#ifdef USING_WIDGETS
int OVButtonDialogControl::select(Dialog *dialog)
{
     int layer,state,layersdefined;
     BitMask *oldlayertable;
     //BitMask oldDBlayertable(MAX_LAYERS);
     BitMask *oldDBlayertable;

     // NOTE: KMJ:
     // We could use the MaskChange_Dialog directly here
     // but we are using it from the general_property_command for ow


    state = 1001;
    if(id == 108)
    { // layer
        // the general_property_command changes the DB layers
        // so copy that here while the dialog is up
        oldDBlayertable = new BitMask(MAX_LAYERS);
        *oldDBlayertable = db.dismask.layer;

        oldlayertable = *layertable;
        if (*layertable == 0)
        {
             *layertable = new BitMask(MAX_LAYERS);
             **layertable = db.dismask.layer;
        }

        db.dismask.layer = **layertable;
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        //((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        //((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);


        RCCHAR dlgReturn[300];
        GetPrivateProfileString("DialogSettings","GenProps::Returned","Cancel",dlgReturn,300,home.getinifilename());
        if(strcmp(dlgReturn,_RCT("Cancel)")) == 0)
        {
            if (oldlayertable == 0)
            {
                delete *layertable;
                *layertable = oldlayertable;
            }
        }
        else
            **layertable = db.dismask.layer;

        layersdefined = layertable != 0;
        ((CheckBoxDialogControl*)dialog->getcontrol(113))->change(dialog,layersdefined);

        // put the original DB layers back now
        db.dismask.layer = *oldDBlayertable;
    }

    return 0;
}
#else
int OVButtonDialogControl::select(Dialog *pdialog)
{
 Dialog dialog(_RCT("Layermask_Dialog"));
 DialogControl *dc;
 ResourceString rs27(NCMAIN+27);
 BitMask *oldlayertable;
  oldlayertable = *layertable;
  if (*layertable == 0)
    {  *layertable = new BitMask(MAX_LAYERS);
       **layertable = db.dismask.layer;
    }
  dialog.title(rs27.gets());
  dialog.add((OVLayerMaskDialogControl *)(dc = new OVLayerMaskDialogControl(100,1100,MAX_LAYERS,1,8,*layertable)));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new OVLayerMaskButtonDialogControl(101));
  dialog.add(new OVLayerMaskButtonDialogControl(102));

  dialog.add(new OVLayerMaskButtonDialogControl(101));
  dialog.add(new OVLayerMaskButtonDialogControl(102));
  dialog.add(new OVLayerMaskButtonDialogControl(105));
  dialog.add(new OVLayerMaskButtonDialogControl(106));
  dialog.add(new OVLayerMaskButtonDialogControl(107));
  dialog.add(new OVLayerMaskButtonDialogControl(108));
  dialog.add(new OVLayerMaskButtonDialogControl(109));
  dialog.add(new OVLayerMaskButtonDialogControl(110));
  dialog.add(new OVLayerMaskButtonDialogControl(111));
  dialog.add(new OVLayerMaskButtonDialogControl(112));
  dialog.add(new OVLayerMaskButtonDialogControl(113));
  dialog.add(new OVLayerMaskButtonDialogControl(114));


  if (dialog.process(pdialog->gethdlg()) == FALSE)
    {  if (oldlayertable == 0)
         {  delete *layertable;
            *layertable = oldlayertable;
         }
    }
  return 0;
}
#endif

int OneView::change(Database *db,View2d *view2d)
{
 Dialog dialog(_RCT("OneviewChange_Dialog"));
 DialogControl *dc;
 double t,xp,yp,s,x1,y1,x2,y2,a;
 ResourceString rs12(NCVIEWA+12),rs13(NCVIEWA+13),rs14(NCVIEWA+14);
 BitMask *templayertable;
 double cosa,sina;
 int layersdefined;

  if (layertable== 0)
      templayertable = 0;
  else
  {
      templayertable = new BitMask(MAX_LAYERS);
       *templayertable = *layertable;
  }

  layersdefined = layertable != 0;

  xp = xpos;  yp = ypos;  s = scale;  a = angle;
  cosa = cos(angle*acos(-1.0)/180.0);
  sina = sin(angle*acos(-1.0)/180.0);
  x1 = xpos + umin / scale * cosa - vmin / scale * sina;
  y1 = ypos + umin / scale * sina + vmin / scale * cosa;
  x2 = xpos + umax / scale * cosa - vmax / scale * sina;
  y2 = ypos + umax / scale * sina + vmax / scale * cosa;

  dialog.add(new StringDialogControl(112,view->getname(),strlen(view->getname())+1));
  dialog.add(dc = new RealDialogControl(100,&xp,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&yp,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&x1,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&y1,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&x2,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&y2,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,&s,1.0,1.0E-10,1.0E20));
  //dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(110,&a));
  //dialog.add(new ScrollBarDialogControl(1110,dc));
  dialog.add(new CheckBoxDialogControl(107,&options,OVHide)); // option bit 0 hiden line when plotting
  dialog.add(new OVButtonDialogControl(108,&templayertable)); // temp layer table button
  dialog.add(new CheckBoxDialogControl(109,&options,OVShade)); // option bit 2 shade when plotting
  dialog.add(new CheckBoxDialogControl(111,&options,OVDashHidden));// option bit 4 dashed hidden when plotting
  dialog.add(new CheckBoxDialogControl(113,&layersdefined)); // layers defined
  dialog.add(new ButtonDialogControl(114));
  int status = dialog.process();
  //if (dialog.process() == TRUE)
  if (status == TRUE)
  {
       if (x1 > x2) {  t = x1;  x1 = x2;  x2 = t;  }
       if (y1 > y2) {  t = y1;  y1 = y2;  y2 = t;  }
       if (fabs(x1 - x2) < db->getptoler())
         {  cadwindow->MessageBox(rs13.gets(),rs12.gets(),MB_ICONINFORMATION);
            return 0;
         }
       else if (fabs(y1 - y2) < db->getptoler())
         {  cadwindow->MessageBox(rs14.gets(),rs12.gets(),MB_ICONINFORMATION);
            return 0;
         }
       else
         {  db->saveundo(UD_CHANGEVIEW2D,view2d);
            if (fabs(s - scale) > 1.0E-6)
              {//  The scale has changed - Modify xpos and ypos as well to maintain the position of the centre of the view

                 cosa = cos(angle*acos(-1.0)/180.0);
                 sina = sin(angle*acos(-1.0)/180.0);

                 xp = xp + cosa * (umin+umax) / 2.0 / scale - sina * (vmin+vmax) / 2.0 / scale;
                 yp = yp + sina * (umin+umax) / 2.0 / scale + cosa * (vmin+vmax) / 2.0 / scale;
                 xp +=  - ((umin+umax) / 2.0 / s * cosa - (vmin+vmax) / 2.0 / s * sina);
                 yp +=  - ((umin+umax) / 2.0 / s * sina + (vmin+vmax) / 2.0 / s * cosa);

              }
            xpos = xp;  ypos = yp;  scale = s;  angle = a;

            // KMJ: this was commented out in the V7 code
            umin = ( (x1 - xp) * cosa + (y1 - yp) * sina) * scale;
            vmin = (-(x1 - xp) * sina + (y1 - yp) * cosa) * scale;

            umax = ( (x2 - xp) * cosa + (y2 - yp) * sina) * scale;
            vmax = (-(x2 - xp) * sina + (y2 - yp) * cosa) * scale;

         }
       if (layertable == 0)
         layertable = templayertable;
       else if (templayertable == 0)
         {  delete layertable;
            layertable = 0;
         }
       else
         {  *layertable = *templayertable;
            delete templayertable;
         }
       return 1;
  }
  else if(status == 114)
      return 2;
  else
  {
       delete templayertable;
       return 0;
  }
}

int OneView::save(CadFile *outfile)
{ if (! outfile->write(view->getname())) return 0;
  if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;
  if (! outfile->write(&scale)) return 0;
  if (! outfile->write(&xpos)) return 0;
  if (! outfile->write(&ypos)) return 0;
  if (layertable != 0)
    options.set(1);
  else
    options.clear(1);
  options.set(OVAngleDefined);
  if (! outfile->write(&options)) return 0;
  if (options.test(1))
    if (! outfile->write(layertable)) return 0;
  if (! outfile->write(&angle)) return 0;
  return 1;
}

OneView *OneView::load(CadFile *infile)
{double umin,vmin,umax,vmax,scale,xpos,ypos,angle;
 BitMask o(32),*lt;
 char *cname;
 RCCHAR *name;
 View *v;
 OneView *ov;
    //if (! infile->read(&name)) return 0;
    if( infile->getversion() > 2 )
    {
     if (! infile->read(&name))
        return 0;
    }
    else
    {
     if (! infile->read(&cname)) return 0;
     name = new RCCHAR[strlen(cname)+1];
     strcpy(name,cname);
     delete [] cname;
    }
  if (! infile->read(&umin)) return 0;
  if (! infile->read(&vmin)) return 0;
  if (! infile->read(&umax)) return 0;
  if (! infile->read(&vmax)) return 0;
  if (! infile->read(&scale)) return 0;
  if (! infile->read(&xpos)) return 0;
  if (! infile->read(&ypos)) return 0;
  if (! infile->read(&o)) return 0;
  if (o.test(1))
    {  lt = new BitMask(MAX_LAYERS);
       if (! infile->read(lt)) return 0;
    }
  else
    lt = 0;

  if (o.test(OVAngleDefined))
    {  if (! infile->read(&angle)) return 0;
    }
  else
    angle = 0.0;

  if ((v = db.views3d.match(name)) != NULL)
    ov = new OneView(v,umin,vmin,umax,vmax,scale,xpos,ypos,angle,o,lt);
  else if ((v = db.views3d.match(0)) != NULL)  //  Use default views if this view has been deleted.
    ov = new OneView(v,umin,vmin,umax,vmax,scale,xpos,ypos,angle,o,lt);
  else if ((v = db.views3d.match(1)) != NULL)  //  Use default views if this view has been deleted.
    ov = new OneView(v,umin,vmin,umax,vmax,scale,xpos,ypos,angle,o,lt);
  else
    ov = NULL;
  delete [] name;

  return ov;

}

int OneView::save(UndoFile *outfile)
{ if (! outfile->write(view->getname())) return 0;
  if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;
  if (! outfile->write(&scale)) return 0;
  if (! outfile->write(&xpos)) return 0;
  if (! outfile->write(&ypos)) return 0;
  if (layertable != 0)
    options.set(1);
  else
    options.clear(1);
  options.set(OVAngleDefined);
  if (! outfile->write(&options)) return 0;
  if (options.test(1))
    if (! outfile->write(layertable)) return 0;
  if (! outfile->write(&angle)) return 0;
  return 1;
}

OneView *OneView::load(UndoFile *infile)
{double umin,vmin,umax,vmax,scale,xpos,ypos,angle;
 BitMask o(32),*lt;
 RCCHAR *name;
 View *v;
 OneView *ov;
  if (! infile->read(&name)) return 0;
  if (! infile->read(&umin)) return 0;
  if (! infile->read(&vmin)) return 0;
  if (! infile->read(&umax)) return 0;
  if (! infile->read(&vmax)) return 0;
  if (! infile->read(&scale)) return 0;
  if (! infile->read(&xpos)) return 0;
  if (! infile->read(&ypos)) return 0;
  if (! infile->read(&o)) return 0;
  if (o.test(1))
    {  lt = new BitMask(MAX_LAYERS);
       if (! infile->read(lt)) return 0;
    }
  else
    lt = 0;
  if (! infile->read(&angle)) return 0;
  if ((v = db.views3d.match(name)) != NULL)
    ov = new OneView(v,umin,vmin,umax,vmax,scale,xpos,ypos,angle,o,lt);
  else
    ov = NULL;
  delete [] name;
  return ov;
}

int View2d::nsizes = 83;
QString      View2d::sizes[] = { QString("A5"),QString("A5V"),QString("A4"),QString("A4V"),
                                 QString("A3"),QString("A3V"),QString("A2"),QString("A2V"),
                                 QString("A1"),QString("A1V"),QString("A0"),QString("A0V"),
                                 QString("B5"),QString("B5V"),QString("B4"),QString("B4V"),
                                 QString("B3"),QString("B3V"),QString("B2"),QString("B2V"),
                                 QString("B1"),QString("B1V"),QString("B0"),QString("B0V"),
                                 QString("C5"),QString("C5V"),QString("C4"),QString("C4V"),
                                 QString("C3"),QString("C3V"),QString("C2"),QString("C2V"),
                                 QString("C1"),QString("C1V"),QString("C0"),QString("C0V"),
                                 QString("ANSI A"),QString("ANSI AV"),QString("ANSI B"),QString("ANSI BV"),
                                 QString("ANSI C"),QString("ANSI CV"),QString("ANSI D"),QString("ANSI DV"),
                                 QString("ANSI E"),QString("ANSI EV"), QString("ANSI F"),QString("ANSI FV"),
                                 QString("ARCH A"),QString("ARCH AV"),QString("ARCH B"),QString("ARCH BV"),
                                 QString("ARCH C"),QString("ARCH CV"),QString("ARCH D"),QString("ARCH DV"),
                                 QString("ARCH E"),QString("ARCH EV"),QString("ARCH E1"),QString("ARCH E1V"),
                                 QString("ARCH E2"),QString("ARCH E2V"),QString("ARCH E3"),QString("ARCH E3V"),
                                 QString("Letter"),QString("LetterV"),QString("Legal"),QString("LegalV"),
                                 QString("US Govt"),QString("US GovtV"),QString("Statement"),QString("StatementV"),
                                 QString("Executive"),QString("ExecutiveV"),QString("Folio"),QString("FolioV"),
                                 QString("Quarto"),QString("QuartoV"),QString("Ledger"),QString("LedgerV"),
                                 QString("Foolscap"),QString("FoolscapV"),QString("Custom")/*QString("Non standard")*/
                             };
double NEAR View2d::widths[]  = {/*A Sizes   */  210.0,148.0,297.0,210.0,420.0,297.0,594.0,420.0,841.0,594.0,1189.0, 841.0,
                                 /*B Sizes   */  250.0,176.0,353.0,250.0,500.0,353.0,707.0,500.0,1000.0,707.0,1414.0,1000.0,
                                 /*C Sizes   */  229.0,162.0,324.0,229.0,458.0,324.0,648.0,458.0,917.0,648.0,1297.0,917.0,
                                 /*ANSI Sizes*/  279.4,215.9,431.8,279.4,558.8,431.8,863.6,558.8,1117.6,863.6,711.2,1016.0,
                                 /*ARCH Sizes*/  304.8,228.6,457.2,304.8,609.6,457.2,914.4,609.6,1219.2,914.4,30*25.4,42*25.4,26*25.4,38*25.4,27*25.4,39*25.4,
                                   279.4,215.9,355.6,215.9,279.4,203.2,215.9,139.7,266.7,184.2,330.2,215.9,274.3,214.9,431.8,279.4,431.8,342.9,
                                   1000.0  
                                };
double NEAR View2d::heights[] = {/*A Sizes   */  148.0,210.0,210.0,297.0,297.0,420.0,420.0,594.0,594.0,841.0,841.0,1189.0,
                                 /*B Sizes   */  176.0,250.0,250.0,353.0,353.0,500.0,500.0,707.0,707.0,1000.0,1000.0,1414.0,
                                 /*C Sizes   */  162.0,229.0,229.0,324.0,324.0,458.0,458.0,648.0,648.0,917.0,917.0,1297.0,
                                 /*ANSI Sizes*/  215.9,279.4,279.4,431.8,431.8,558.8,558.8,863.6,863.6,1117.6,1016.0,711.2,
                                 /*ARCH Sizes*/  228.6,304.8,304.8,457.2,457.2,609.6,609.6,914.4,914.4,1219.2,42*25.4,30*25.4,38*25.4,26*25.4,39*25.4,27*25.4,
                                   215.9,279.4,215.9,355.6,203.2,279.4,139.7,215.9,184.2,266.7,215.9,330.2,215.9,274.3,279.4,431.8,342.9,431.8,
                                   1000.0  
                                };

View2d::View2d(RCCHAR *n,double w,double h,short vn)
{ if ((name = new RCCHAR[strlen(n) + 1]) != NULL)
    strcpy(name,n);
  else
    name = _RCT("(Null)");
  paperwidth = w;
  paperheight = h;
  viewno = vn;
}

void View2d::setname(RCCHAR *n)
{ delete [] name;
  if ((name = new RCCHAR[strlen(n) + 1]) != NULL)
    strcpy(name,n);
  else
    name = _RCT("(Null)");
}
 
extern void transformlist(int project,int ncopies,EntityList list,Transform t,int surface);

void View2d::change(RCCHAR *n,double width,double height,double offsetx,double offsety)
{OneView *ov;
 EntityList list;
 Entity *e;
 Transform t;

  delete [] name;
  if ((name = new RCCHAR[strlen(n) + 1]) != NULL)
    strcpy(name,n);
  else
    name = _RCT("(Null)");


  paperwidth = width;
  paperheight = height;

  //  Move any entities that are on this drawing.

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getvisible()->test(viewno))
      list.add(e);

  transformlist(0,0,list,t.translate(Point(-offsetx,-offsety,0.0)),0);

  for (views.start() ; (ov = (OneView *) views.next()) != NULL ; )
    {  ov->setposition(ov->getxpos() - offsetx,ov->getypos() - offsety);

    }


}
 
int View2d::verify(Database *db)
{
  Dialog dialog("DrawingVerify_Dialog");
 int index;
 RCCHAR pname[300];

  for (index = 0 ; index < View2d::nsizes - 1 ; index++)
    if (fabs(paperwidth-View2d::widths[index]) < 1.0 &&
        fabs(paperheight - View2d::heights[index]) < 1.0) break;

  strcpy(pname,View2d::sizes[index]);
  dialog.add(new StringDialogControl(100,name,strlen(name)+1));
  dialog.add(new StringDialogControl(101,pname,300));
  dialog.add(new RealDialogControl(102,&paperwidth,ScaleByLength));
  dialog.add(new RealDialogControl(103,&paperheight,ScaleByLength));
  dialog.add(new ButtonDialogControl(104));
  if (dialog.process() == 104)
    return change(db);
  else
    return 0;
}

class PaperSize2dDialogControl : public ListDialogControl
  {private:
     int lastindex;
   public:
     PaperSize2dDialogControl(int id,int n,RCCHAR **list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     PaperSize2dDialogControl(int id,int n,QString *list,int *index) :
       ListDialogControl(id,n,list,index) { lastindex = -1; }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void PaperSize2dDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void PaperSize2dDialogControl::changefocus(Dialog *dialog,int)
{int index;
  if (dialog->currentvalue(101,&index) && index != lastindex)
    {  if (index < View2d::nsizes - 1)
         {  ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,View2d::widths[index]);
            ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,View2d::heights[index]);
         }
       else
         {  ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,v.getreal("cd::nonstdwidth"));
            ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,v.getreal("cd::nonstdheight"));
         }
       lastindex = index;
    }
}

class TBFCButtonDialogControl : public ButtonDialogControl
  {public:
     TBFCButtonDialogControl(int id) : ButtonDialogControl(id)  {}
     int select(Dialog *);
  };

int TBFCButtonDialogControl::select(Dialog *dialog)
{
 int i;
 RCOPENFILENAME ofn;
 RCCHAR directory[300],filename[300],filter[300];
 ResourceString rs169(NCVIEWB+169);
 ResourceString rs170(NCVIEWB+170);

  //_getcwd(directory, sizeof(directory));
  strcpy(directory,v.getstring("db::title-sheets-path"));
  if(strlen(directory) < 1)
    strcpy(directory,home.getpublicfilename("Title-Sheets"));
  else
  {
    // check if it exists
    // if not use the default
    if(!QDir(QString(directory)).exists())
       strcpy(directory,home.getpublicfilename("Title-Sheets"));
  }

  filename[0] = '\0';
  strcpy(filter,rs169.getws());
  //for (i = 0; filter[i] != '\0'; i++)
  //  if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile=filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrInitialDir = directory;
  ofn.lpstrDefExt = _RCT("CAD");
  ofn.lpstrTitle =  rs170.getws();
  ofn.Flags = 0;//OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn))
    ((StringDialogControl *)dialog->getcontrol(104))->change(dialog,ofn.lpstrFile);

  return 0;
}

int View2d::change(Database *db)
{Dialog dialog(_RCT("Composeddrawing_Dialog"));
 DialogControl *dc;
 int index;
 RCCHAR newname[300],figure[300],*nn,*attribute;
 double pw,ph;
 AttributeList al;
 Entity *e;
 BitMask options(32),visible(MaxViews);

  strcpy(newname,name);
  for (index = 0 ; index < View2d::nsizes - 1 ; index++)
    if (fabs(paperwidth-View2d::widths[index]) < 1.0 &&
        fabs(paperheight - View2d::heights[index]) < 1.0) break;
  pw = paperwidth;
  ph = paperheight;
  strcpy(figure,"");
  dialog.title(_RCT("Change layout drawing"));
  dialog.add(new StringDialogControl(100,newname,255));
  dialog.add(new PaperSize2dDialogControl(101,View2d::nsizes,View2d::sizes,&index));
  dialog.add(dc = new RealDialogControl(102,&pw,1.0,0.0,1E10,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&ph,1.0,0.0,1E10,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new StringDialogControl(104,figure,300));
  dialog.add(new TBFCButtonDialogControl(105));
  if (dialog.process() == TRUE)
    {  if (index == View2d::nsizes - 1)
         {  v.setreal("cd::nonstdwidth",pw);
            v.setreal("cd::nonstdheight",ph);
         } 
       
       db->saveundo(UD_CHANGEVIEW2D,this);
       if ((nn = new RCCHAR[strlen(newname)+1]) != NULL)
         {  delete [] name;
            name = nn;
            strcpy(name,newname);
         }
       paperwidth = pw;
       paperheight = ph;

       if (strlen(figure) > 0)
         {
            //if (strchr(figure,'\\') == 0)
            if (strchr(figure,QDir::separator()) == 0)
            {
                 strcpy(name,figure);
                 strcpy(figure,home.getpublicfilename(name));
            }

            //  Delete any existing title block figure - Must be visible in this view and have the attribute "Title Block Figure"
            for (db->geometry.start() ; (e = db->geometry.next()) != 0 ; )
              if (e->isa(figure_entity) && e->getvisible()->test(viewno))
                {  al = e->getatlist();
                   for (al.start() ; (attribute = al.next()) != 0 ; )
                     {  if (strcmp(attribute,"Title Block Figure") == 0)
                          {  db->geometry.del(e);
                             break;
                          }
                     }  
                   if (attribute != 0)
                     break;
                }

            Figure *fig = new Figure(figure,_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,options);
            visible.set(viewno);
            fig->setvisible(visible);
            if (fig != NULL && fig->getdefined())
              {  db->geometry.add(fig);
                 if ((attribute = new RCCHAR[strlen(_RCT("Title Block Figure"))+1]) != NULL)
                   {  strcpy(attribute,_RCT("Title Block Figure"));
                      fig->addat(attribute);  
                      fig->setattribute(attribute);
                      db->saveundo(UD_INSERTATT,fig);
                   }
              }
            else
              delete fig;
         }

       return 1;
    }
  else
    return 0;
}

View2d::~View2d()
{OneView *ov;
  if (strcmp(name,"(Null)") != 0)
    delete [] name;
  for (views.start() ; (ov = (OneView *) views.next()) != NULL ; )
    delete ov;
  views.destroy();
}

int View2d::add(OneView *ov)
{ return views.add(ov);
}

void View2d::del(OneView *ov)
{ views.del(ov);
}

void View2d::start(void)
{ views.start();
}

OneView *View2d::next(void)
{ return (OneView *) views.next();
}

void View2d::sizeevent(double w,double h)
{ paperwidth = w;  paperheight = h;
}

int View2d::save(CadFile *outfile)
{short nviews;
 OneView *ov;
  if (! outfile->write(name)) return 0;
  if (! outfile->write(&paperwidth)) return 0;
  if (! outfile->write(&paperheight)) return 0;
  if (! outfile->write(&viewno)) return 0;
  nviews = short(views.length());
  if (! outfile->write(&nviews)) return 0;
  for (views.start() ; (ov = (OneView *) views.next()) != NULL ; )
    ov->save(outfile);
  if (! zonelist.save(outfile)) return 0;
  return 1;
}

View2d *View2d::load(CadFile *infile)
{double paperwidth,paperheight;
 char *cname;
 RCCHAR *name;
 short viewno;
 short i,nviews;
 OneView *ov;
 View2d *view2d;

  if( infile->getversion() > 2 )
  {
      if (! infile->read(&name))
         return 0;
  }
  else
  {
      if (! infile->read(&cname)) return 0;
      name = new RCCHAR[strlen(cname)+1];
      strcpy(name,cname);
      delete [] cname;
  }
  if (! infile->read(&paperwidth)) return 0;
  if (! infile->read(&paperheight)) return 0;
  if (! infile->read(&viewno)) return 0;
  if ((view2d = new View2d(name,paperwidth,paperheight,viewno)) == NULL) return 0;
  if (! infile->read(&nviews)) return 0;
  for (i = 0 ; i < nviews ; i++)
    {  if ((ov = OneView::load(infile)) == NULL) return 0;
       view2d->add(ov);
    }
  view2d->views.reverse();
  // KMJ temp file version -2 from Qt verion will be 3
  if (infile->getversion() >= 2 || infile->getversion() < 0)
    if (! view2d->zonelist.load(infile)) return 0;
  delete [] name;
  return view2d;
}

int View2d::save(UndoFile *outfile)
{short nviews;
 OneView *ov;
  if (! outfile->write(name)) return 0;
  if (! outfile->write(&paperwidth)) return 0;
  if (! outfile->write(&paperheight)) return 0;
  if (! outfile->write(&viewno)) return 0;
  nviews = short(views.length());
  if (! outfile->write(&nviews)) return 0;
  for (views.start() ; (ov = (OneView *) views.next()) != NULL ; )
    ov->save(outfile);
  return 1;
}

int View2d::load(UndoFile *infile)
{short i,nviews;
 OneView *ov;
  delete [] name;
  if (! infile->read(&name)) return 0;
  if (! infile->read(&paperwidth)) return 0;
  if (! infile->read(&paperheight)) return 0;
  if (! infile->read(&viewno)) return 0;
  if (! infile->read(&nviews)) return 0;

// Delete all the old views!
  for (views.start() ; (ov = (OneView *) views.next()) != NULL ; )
    delete ov;
  views.destroy();

//  Recreate the views.
  for (i = 0 ; i < nviews ; i++)
    {  if ((ov = OneView::load(infile)) == NULL) return 0;
       add(ov);
    }
  views.reverse();
  return 1;
}

int View2d::matchPaperName(RCCHAR *name)
{
    QString inName(name);
    for(int i=0; i<nsizes; i++)
    {
        QString thisName(sizes[i]);
        if(thisName.contains(inName,Qt::CaseInsensitive) ||
           inName.contains(thisName,Qt::CaseInsensitive))
            return i;
    }
    return -1;
}

void Views2d::clear(void)
{View2d *v;
  for (list.start() ; (v = list.next()) != NULL ; ) delete v;
  list.destroy();
}

void Views2d::initialize(void)
{ clear();
}

int Views2d::load(CadFile *infile,Database *database)
{View2d *v;
 short nviews;
 int i;
  if (! infile->read(&nviews)) return 0;
  for (i = 0 ; i < nviews ; i++)
    {  if ((v = View2d::load(infile)) == NULL) return 0;
       database->views2d.add(v);
    }
  database->views2d.list.reverse();
  return 1;
}

int Views2d::save(CadFile *outfile)
{short type;
 View2d *v;
 short nviews;
  type = view2d_record;
  if (! outfile->write(&type)) return 0;
  nviews = short(list.length());
  if (! outfile->write(&nviews)) return 0;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (! v->save(outfile)) return 0;
  return 1;
}

View2d *Views2d::match(const RCCHAR *name)
{View2d *v;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (strcmp(v->getname(),name) == 0) return v;
  return NULL;
}

View2d *Views2d::match(View2d *view2d)
{View2d *v;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (view2d == v) return v;
  return NULL;
}

View2d *Views2d::match(const int viewno)
{View2d *v;
  for (list.start() ; (v = list.next()) != NULL ; )
    if (v->getviewno() == viewno) return v;
  return NULL;
}

void Views2d::names(RCCHAR ***namelist,int *n,int *index)
{View2d *v;
  for (list.start(),*n = 0 ; (v = list.next()) != NULL ; (*n)++);
  if ((*namelist = new RCCHAR *[*n]) != NULL)
    {  *index = *n - 1;
       for (list.start(),*n = 0 ; (v = list.next()) != NULL ; (*n)++)
         {  (*namelist)[*n] = v->getname();
            if (cadwindow != 0 && cadwindow->getcurrentsurface() != NULL &&
                cadwindow->getcurrentsurface()->getview()->getviewno() == v->getviewno())
              *index = *n;
         }
    }
}

#pragma warning( disable : 4355 )  // Disable warning messages

View3dSurface::View3dSurface(View *v,double umini,double vmini,double umaxi,double vmaxi,int dl,BitMask *l)
{
#ifdef USING_WIDGETS
    qgirb=0;
    qgimk=0;
   //qDebug() << "Entering : View3dSurface::View3dSurface : ";
    israytracing=0;
#endif

  view = v;
  umin = umini;
  vmin = vmini;
  vmax = vmaxi;
  umax = umaxi;
  if (umax - umin < 1.0E-5)
    umax = umin + 1.0E-5;
  if (vmax - vmin < 1.0E-5)
    vmax = vmin + 1.0E-5;
  displaylayertable = l;
  selectionlayertable = v->getselectionlayertable();
  lockedlayertable = v->getlockedlayertable();
  plotscale = ::v.getreal("vw::plotscale");
  depth = 0.0;
  updatematrix();
  repaintstyle = Wireframe;
  dirty = 0;
  printerhdc = 0;
  setbackground(RGB(255.0,255.0,255.0));
  modeltransform.identity();
}

void View3dSurface::setView(View *v)
{
    View *tview;
    View3dWindowList *wl;
    View3dWindow *window;
    double l;

    if ((tview = db.views3d.match(v->getname())) != 0)
    {
       view = v;

       wl = cadwindow->GetView3dList();

       for (wl->start();  (window = (View3dWindow *) wl->next()) != NULL ; )
         if (window->getview() == view)
         {
             if (window->getoffscreenbitmap() != 0)
                window->getoffscreenbitmap()->displaylist.clear();
              l = (view->geteye() - view->getref()).length();
              if (view->getvisibility() == 0)
                window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
              else
                window->zoom(-l,-l,l,l);
         }
    }
}

void View3dSurface::setdirty(Point pmin,Point pmax)
{Point p;
 int i,x1,y1,x2,y2;
  p = modeltoscreen(pmax);
  p.x = p.x / getwidth() * (umax - umin) + umin;
  p.y = p.y / getheight() * (vmax - vmin) + vmin;
  dumin = dumax = p.x;
  dvmin = dvmax = p.y;
  for (i = 1 ; i < 8 ; i++)
    {  p = modeltoscreen(Point(i/4 ? pmin.x : pmax.x,(i/2)%2 ? pmin.y : pmax.y,i%2 ? pmin.z : pmax.z));
       p.x = p.x / getwidth() * (umax - umin) + umin;
       p.y = p.y / getheight() * (vmax - vmin) + vmin;
       if (p.x < dumin) dumin = p.x;
       if (p.x > dumax) dumax = p.x;
       if (p.y < dvmin) dvmin = p.y;
       if (p.y > dvmax) dvmax = p.y;
    }
  if (dumin < umin) dumin = umin;
  if (dvmin < vmin) dvmin = vmin;
  if (dumax > umax) dumax = umax;
  if (dvmax > vmax) dvmax = vmax;

  x1 = int((dumin - umin) / (umax - umin) * (getwidth()-1));
  y1 = int((dvmin - vmin) / (vmax - vmin) * (getheight()-1));
  x2 = int((dumax - umin) / (umax - umin) * (getwidth()-1));
  y2 = int((dvmax - vmin) / (vmax - vmin) * (getheight()-1));

  //  Move dumin etc onto the screen grid
  dumin = x1 / (getwidth()-1) * (umax - umin) + umin;
  dvmin = y1 / (getheight()-1) * (vmax - vmin) + vmin;
  dumax = x2 / (getwidth()-1) * (umax - umin) + umin;
  dvmax = y2 / (getheight()-1) * (vmax - vmin) + vmin;

  dirty = 1;
}

int View3dSurface::requiresopengl(void)
{ return db.getrequiresopengl();
}


int View3dSurface::inside(Point p)
{ if (view->getperspective())
    return p.y <= p.z && p.y >= -p.z &&
           p.x <= p.z && p.x >= -p.z &&
           p.z <= 1.0 && p.z >= zmin;
  else
    return p.y <= 1.0 && p.y >= 0.0 &&
           p.x <= 1.0 && p.x >= 0.0 &&
           p.z <= 1.0 && p.z >= 0.0;
}

void View3dSurface::setuvminmax(double u1,double v1,double u2,double v2)
{ umin = u1;  vmin = v1;  umax = u2;  vmax = v2;
  if (db.getenableautoplotscale() != 0 && getsurfacetype() != View2dSurf)
    plotscale = (umax - umin) / getwidthmm() * db.getautoplotscale();
  updatematrix();
}

Point View3dSurface::modeltoview(Point p)
{Point tmp;
  tmp.x = p.x * m11 + p.y * m12 + p.z * m13 + m14;
  tmp.y = p.x * m21 + p.y * m22 + p.z * m23 + m24;
  tmp.z = p.x * m31 + p.y * m32 + p.z * m33 + m34;
  return tmp;
}
/*
Point View3dSurface::viewtomodel(Point p)
{Point tmp;
  tmp.x = p.x * m11 + p.y * m12 + p.z * m13 + m14;
  tmp.y = p.x * m21 + p.y * m22 + p.z * m23 + m24;
  tmp.z = p.x * m31 + p.y * m32 + p.z * m33 + m34;
  return tmp;
}
*/
Point View3dSurface::modeltouv(Point p)
{Point tmp;
  tmp.x = uaxis.dot(p - view->getref());
  tmp.y = vaxis.dot(p - view->getref());
  tmp.z = waxis.dot(p - view->getref());
  return tmp;
}

Point View3dSurface::viewtoscreen(Point p)
{Point tmp;
  if (view->getperspective())
    {  if (p.z > 1.0E-20){
           tmp.x = p.x * a1 / p.z + a2;
            tmp.y = p.y * b1 / p.z + b2;
            tmp.z = (1.0 - zmin / p.z) / (1.0 - zmin) * 1000.0;
         }
       else
         tmp.setxyz(0.0,0.0,0.0);
    }
  else
    {  tmp.x = p.x * a1 + a2;
       tmp.y = p.y * b1 + b2;
       tmp.z = p.z * 1000.0;
    }
  return tmp;
}

Point View3dSurface::screentoview(Point p)
{
  Point tmp;
  if (view->getperspective())
  {

      //tmp.x = p.x * a1 / p.z + a2;
      //tmp.y = p.y * b1 / p.z + b2;
      //tmp.z = (1.0 - zmin / p.z) / (1.0 - zmin) * 1000.0;

      tmp.x = -((a2 - p.x * p.z) / a1 );
      // vpx = -(((spz * a2) - (vpx * a2)) / a1)
      //tmp.x = -(((p.z * a2) - (p.x * a2)) / a1);
      // vpy = -(((spz * b2) - (vpy * b2)) / b1)
      //tmp.y = -(((p.z * b2) - (p.y * b2)) / b1);
      tmp.y = -((b2 - p.y * p.z) / b1 );
      // vpz = (1000.0 * zmin) / (zmin - 1.0) * spz * 1000.0
      tmp.z = (1000.0 * zmin) / (zmin - 1.0) * p.z * 1000.0;

       //if(tmp.z > 1.0E-20)
       //    return tmp;
       //else
       //    return tmp.setxyz(0.0,0.0,0.0);
  }
  else
  {
       //tmp.x = p.x * a1 + a2;
       //tmp.y = p.y * b1 + b2;
       //tmp.z = p.z * 1000.0;

       // vpx = -((a2 - spx) / a1
       tmp.x = -((a2 - p.x) / a1);
       // vpy = -((b2 - spy) / b1
       tmp.y = -((b2 - p.y) / b1);
       // vpz = spz / 1000.0
       tmp.z = p.z / 1000.0;
  }
  return tmp;
}

Point View3dSurface::viewtomodel(Point p)
{
    return Point(0,0,0);
}

Point View3dSurface::modeltoscreen(Point p)
{Point tmp;
  if (view->getperspective())
    {  tmp.x = p.x * m11 + p.y * m12 + p.z * m13 + m14;
       tmp.y = p.x * m21 + p.y * m22 + p.z * m23 + m24;
       tmp.z = p.x * m31 + p.y * m32 + p.z * m33 + m34;
       if (fabs(tmp.z) > 1.0E-20)
         {  tmp.x = tmp.x * a1 / tmp.z + a2;
            tmp.y = tmp.y * b1 / tmp.z + b2;
            tmp.z = (1.0 - zmin / tmp.z) / (1.0 - zmin) * 1000.0;
         }
       else
         tmp.setxyz(0.0,0.0,0.0);
    }
  else
    {  tmp.x = (p.x * m11 + p.y * m12 + p.z * m13 + m14) * a1 + a2;
       tmp.y = (p.x * m21 + p.y * m22 + p.z * m23 + m24) * b1 + b2;
       tmp.z = (p.x * m31 + p.y * m32 + p.z * m33 + m34) * 1000.0;
    }
  return tmp;
}

#if 1
// TODO: need a new function which takes into account perspective
Point View3dSurface::screentomodelex(Point p)
{
  Point tmp;
  if (view->getperspective())
    {  tmp.x = p.x * m11 + p.y * m12 + p.z * m13 + m14;
       tmp.y = p.x * m21 + p.y * m22 + p.z * m23 + m24;
       tmp.z = p.x * m31 + p.y * m32 + p.z * m33 + m34;
       if (fabs(tmp.z) > 1.0E-20)
         {  tmp.x = tmp.x * a1 / tmp.z + a2;
            tmp.y = tmp.y * b1 / tmp.z + b2;
            tmp.z = (1.0 - zmin / tmp.z) / (1.0 - zmin) * 1000.0;
         }
       else
         tmp.setxyz(0.0,0.0,0.0);
    }
  else
    {  tmp.x = (p.x * m11 + p.y * m12 + p.z * m13 + m14) * a1 + a2;
       tmp.y = (p.x * m21 + p.y * m22 + p.z * m23 + m24) * b1 + b2;
       tmp.z = (p.x * m31 + p.y * m32 + p.z * m33 + m34) * 1000.0;
    }
  return tmp;
}
#endif

int View3dSurface::clip3dfb(Point *p1,Point *p2)
{int    accept,swapped,outcode1,outcode2;
 double t;
 Point  p;
  if (view->getperspective())
    {  accept = swapped = 0;
       for (;;)
         {  outcode1 = ((p1->z > 1.0) << 4)   | ((p1->z < zmin) << 5);
            outcode2 = ((p2->z > 1.0) << 4)   | ((p2->z < zmin) << 5);
            if (outcode1 == 0 && outcode2 == 0)
              {  accept = 1;  break;
              }
            if (outcode1 & outcode2) break;
            if (outcode1 == 0)
              {   outcode1 = outcode2;   swapped = ! swapped;
                  p = *p1;  *p1 = *p2;  *p2 = p;
              }
            if (outcode1 & 16)
              {  t = (1.0 - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 1.0;
              }
            else if (outcode1 & 32)
              {  t = (zmin - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = zmin;
              }
         }
    }
  else
    {  accept = swapped = 0;
       for (;;)
         {  outcode1 = ((p1->z > 1.0) << 4) | ((p1->z < 0.0) << 5);
            outcode2 = ((p2->z > 1.0) << 4) | ((p2->z < 0.0) << 5);
            if (outcode1 == 0 && outcode2 == 0)
              {  accept = 1;  break;
              }
            if (outcode1 & outcode2) break;
            if (outcode1 == 0)
              {   outcode1 = outcode2;   swapped = ! swapped;
                  p = *p1;  *p1 = *p2;  *p2 = p;
              }
            if (outcode1 & 16)
              {  t = (1.0 - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 1.0;
              }
            else if (outcode1 & 32)
              {  t = - p1->z / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 0.0;
              }
         }
    }
  if (accept && swapped)
    {  p = *p1;  *p1 = *p2;  *p2 = p;
    }
  return accept;
}

int View3dSurface::clip3d(Point *p1,Point *p2)
{
  int    accept,swapped,outcode1,outcode2;
 double t;
 Point  p;
  if (view->getperspective())
    {  accept = swapped = 0;
       for (;;)
         {  outcode1 = ((p1->y > p1->z))      | ((p1->y < -p1->z) << 1) |
                       ((p1->x > p1->z) << 2) | ((p1->x < -p1->z) << 3) |
                       ((p1->z > 1.0) << 4)   | ((p1->z < zmin) << 5);
            outcode2 = ((p2->y > p2->z))      | ((p2->y < -p2->z) << 1) |
                       ((p2->x > p2->z) << 2) | ((p2->x < -p2->z) << 3) |
                       ((p2->z > 1.0) << 4)   | ((p2->z < zmin) << 5);
            if (outcode1 == 0 && outcode2 == 0)
              {  accept = 1;  break;
              }
            if (outcode1 & outcode2) break;
            if (outcode1 == 0)
              {   outcode1 = outcode2;   swapped = ! swapped;
                  p = *p1;  *p1 = *p2;  *p2 = p;
              }
            if (outcode1 & 1)
              {  t = (p1->z - p1->y) / ((p2->y - p1->y) - (p2->z - p1->z));
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = p1->y;
              }
            else if (outcode1 & 2)
              {  t = -(p1->z + p1->y) / ((p2->y - p1->y) + (p2->z - p1->z));
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = -p1->y;
              }
            else if (outcode1 & 4)
              {  t = (p1->z - p1->x) / ((p2->x - p1->x) - (p2->z - p1->z));
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = p1->x;
              }
            else if (outcode1 & 8)
              {  t = -(p1->z + p1->x) / ((p2->x - p1->x) + (p2->z - p1->z));
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = -p1->x;
              }
            else if (outcode1 & 16)
              {  t = (1.0 - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 1.0;
              }
            else if (outcode1 & 32)
              {  t = (zmin - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = zmin;
              }
         }
    }
  else
    {
#if 1
      accept = swapped = 0;
       for (;;)
         {  outcode1 = ((p1->y > 1.0))      | ((p1->y < 0.0) << 1) |
                       ((p1->x > 1.0) << 2) | ((p1->x < 0.0) << 3) |
                       ((p1->z > 1.0) << 4) | ((p1->z < 0.0) << 5);
            outcode2 = ((p2->y > 1.0))      | ((p2->y < 0.0) << 1) |
                       ((p2->x > 1.0) << 2) | ((p2->x < 0.0) << 3) |
                       ((p2->z > 1.0) << 4) | ((p2->z < 0.0) << 5);
            if (outcode1 == 0 && outcode2 == 0)
              {  accept = 1;  break;
              }
            if (outcode1 & outcode2) break;
            if (outcode1 == 0)
              {   outcode1 = outcode2;   swapped = ! swapped;
                  p = *p1;  *p1 = *p2;  *p2 = p;
              }
            if (outcode1 & 1)
              {  t = (1.0 - p1->y) / (p2->y - p1->y);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = 1.0;
                 p1->z = p1->z + (p2->z - p1->z) * t;
              }
            else if (outcode1 & 2)
              {  t = - p1->y / (p2->y - p1->y);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = 0.0;
                 p1->z = p1->z + (p2->z - p1->z) * t;
              }
            else if (outcode1 & 4)
              {  t = (1.0 - p1->x) / (p2->x - p1->x);
                 p1->x = 1.0;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = p1->z + (p2->z - p1->z) * t;
              }
            else if (outcode1 & 8)
              {  t = - p1->x / (p2->x - p1->x);
                 p1->x = 0.0;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = p1->z + (p2->z - p1->z) * t;
              }
            else if (outcode1 & 16)
              {  t = (1.0 - p1->z) / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 1.0;
              }
            else if (outcode1 & 32)
              {  t = - p1->z / (p2->z - p1->z);
                 p1->x = p1->x + (p2->x - p1->x) * t;
                 p1->y = p1->y + (p2->y - p1->y) * t;
                 p1->z = 0.0;
              }
         }
#else
      return 1;
#endif
    }
  if (accept && swapped)
    {  p = *p1;  *p1 = *p2;  *p2 = p;
    }
  return accept;
}


void View3dSurface::line3d(Point p1,Point p2)
{
    line3d((QGraphicsItem*)0, p1, p2);
}

#ifdef USING_WIDGETS
void View3dSurface::clearScene()
{
    // clear any rubberbands
    if(qgirb)
        if(getScene())
            getScene()->removeItem(qgirb);
    delete qgirb;
    qgirb=0;

    // clear any markers
    if(qgimk)
        if(getScene())
            getScene()->removeItem(qgimk);
    delete qgimk;
    qgimk=0;

    if(getScene())
        getScene()->clear();

}

double View3dSurface::getpixelscale()
{
    return getpixelmodelmm();
    //pix0 = screentomodel(Point(0.0,0.0,0.0));
    //pix1 = screentomodel(Point(1.0,0.0,0.0));
    //return (pix1 - pix0).length();
}

QGraphicsItem * View3dSurface::line3d(QGraphicsItem *qgin,Point pt1,Point pt2)
{
    QPen pen(Qt::black);
    pen.setWidth(0);

    if(qgin)
        getScene()->removeItem(qgin);
    delete qgin;
    qgin = 0;
    QGraphicsItem *qgout = 0;

  Point p1 = modeltoview(pt1);
  Point p2 = modeltoview(pt2);
  if (clip3d(&p1,&p2))
  {
       p1 = viewtoscreen(p1);
       p2 = viewtoscreen(p2);
       qgout = line(qgin,p1.x,p1.y,p2.x,p2.y);
  }
  return qgout;
}

void View3dSurface::line3d(QPainter *painter, Point p1,Point p2)
{
  p1 = modeltoview(p1);
  p2 = modeltoview(p2);
  if (clip3d(&p1,&p2))
  {
       p1 = viewtoscreen(p1);
       p2 = viewtoscreen(p2);
       line(painter,p1.x,p1.y,p2.x,p2.y);
  }
}

#if 1
void View3dSurface::line2dGL(double x1, double y1, double x2, double y2, int transformtomodel)
{
    double x3,y3,z3,x4,y4,z4;

    if(transformtomodel)
    {
        Point glp3,glp4;
        glp3 = screentomodel(Point(x1,y1,0.0));
        glp4 = screentomodel(Point(x2,y2,0.0));

        DrawLine(glp3,glp4,curGLWeight);
        //DrawLine(glp3,glp4,curMMWeight);
        //glBegin(GL_LINES);
        //glVertex3f(glp3.x,glp3.y,glp3.z);
        //glVertex3f(glp4.x,glp4.y,glp4.z);
        //glEnd();
    }
    else if (patternlinestyle != 0)
    {
        x3 = x1;  y3 = y1;  z3 = 0; x4 = x2;  y4 = y2; z4 =0;
        lastx = x4;  lasty = y4; lastz = z4;

        patternlinestyle->drawGL(this,x3,y3,x4,y4);
    }
    else if (getlinestyle() != 0)
    {
        linestyle->drawGL(this,x1,y1,x2,y2);
    }
    else
    {
        DrawLine(Point(x1,y1,0.0),Point(x2,y2,0.0),curGLWeight);
        //DrawLine(Point(x1,y1,0.0),Point(x2,y2,0.0),curMMWeight);
        //glBegin(GL_LINES);
        //glVertex3f(x1,y1,0.0);
        //glVertex3f(x2,y2,0.0);
        //glEnd();
    }
}
#endif

void View3dSurface::line3dGL(Point p1,Point p2)
{
    Surface::lineGL(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
}

#else
void View3dSurface::line3d(Point p1,Point p2)
{
  p1 = modeltoview(p1);
  p2 = modeltoview(p2);
  if (clip3d(&p1,&p2))
  {
       p1 = viewtoscreen(p1);
       p2 = viewtoscreen(p2);
       line(p1.x,p1.y,p2.x,p2.y);
  }
}
#endif

Point View3dSurface::getxaxis(void)
{  if (db.grid.getplane() == onviewplane || getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
     return uaxis;
   else
     return db.workplanes.getcurrent()->getxaxis();
}

Point View3dSurface::getyaxis(void)
{  if (db.grid.getplane() == onviewplane || getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
     return vaxis;
   else
     return db.workplanes.getcurrent()->getyaxis();
}

Point View3dSurface::uv(int x,int y)
{Point tmp;
  tmp.x = (double) x / (double) (getwidth()-1)  * (umax - umin) + umin;
  tmp.y = (double) (getheight()-1-y) / (double) (getheight()-1) * (vmax - vmin) + vmin;
  tmp.z = v.getreal("vw::depth");
  return tmp;
}

Point View3dSurface::uvtoscreen(Point p)
{Point tmp;
  tmp.x = (p.x - umin) / (umax - umin) * (getwidth()-1);
  tmp.y = getheight() - 1 - (p.y - vmin) / (vmax - vmin) * (getheight()-1);
  tmp.z = v.getreal("vw::depth");
  return tmp;
}

Point View3dSurface::screentouv(Point p)
{Point tmp;
  tmp.x = p.x * (umax - umin) / (getwidth()-1) + umin;
  tmp.y = p.y * (vmax - vmin) / (getheight()-1) + vmin;
  tmp.z = p.z;
  return tmp;
}

Point View3dSurface::uvtomodel(Point p)
{Point tmp;
  tmp = uaxis * p.x + vaxis * p.y + waxis * p.z + view->getref();
  return tmp;
}

void View3dSurface::gettransform(Transform *t) const
{ t->mat(0,0) = m11;  t->mat(0,1) = m12;  t->mat(0,2) = m13;  t->mat(0,3) = m14;
  t->mat(1,0) = m21;  t->mat(1,1) = m22;  t->mat(1,2) = m23;  t->mat(1,3) = m24;
  t->mat(2,0) = m31;  t->mat(2,1) = m32;  t->mat(2,2) = m33;  t->mat(2,3) = m34;
  t->mat(3,0) = 0.0;  t->mat(3,1) = 0.0;  t->mat(3,2) = 0.0;  t->mat(3,3) = 1.0;
}

void View3dSurface::applytransform(const Transform &t)
{Transform tview,t1;
  gettransform(&tview);
  t1 = t;
  t1.apply(tview);
  settransform(t1);
}

void View3dSurface::settransform(Transform &t)
{ m11 = t.mat(0,0);  m12 = t.mat(0,1);  m13 = t.mat(0,2);  m14 = t.mat(0,3);
  m21 = t.mat(1,0);  m22 = t.mat(1,1);  m23 = t.mat(1,2);  m24 = t.mat(1,3);
  m31 = t.mat(2,0);  m32 = t.mat(2,1);  m33 = t.mat(2,2);  m34 = t.mat(2,3);
}

int View3dSurface::coordinate3d(int x,int y,Point *p,int fix)
{Point p2,p3;
  *p = uv(x,y);
  if (db.grid.getplane() == onviewplane || getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
    {  *p = db.grid.snappoint(*p);
       *p = uvtomodel(*p);
    }
  else
    {  *p = uvtomodel(*p);
       InfinitePlane plane(db.workplanes.getcurrent()->getzaxis(),db.workplanes.getcurrent()->getorigin() + db.workplanes.getcurrent()->getzaxis() * v.getreal("vw::depth"));
       if (view->getperspective())
         p2 = view->geteye();
       else
         p2 = *p + view->geteye() - view->getref();
       Line line(*p,p2);
       Intersect i(&line,&plane);
       if (i.nintersections() == 1)
         {  *p = ((PointE *) i.intersection(0))->getp();
            if (view->getperspective() && modeltoview(*p).z < zmin) return 0;
            *p = db.workplanes.getcurrent()->modeltoworkplane(*p);
            *p = db.grid.snappoint(*p);
            *p = db.workplanes.getcurrent()->workplanetomodel(*p);
         }
       else
         return 0;
    }
  if (fix == 1 && cadwindow != 0)
  {
      if(cadwindow->coordinateentry != 0)
        *p = cadwindow->coordinateentry->fix(*p);
  }
  return 1;
}

Point View3dSurface::screentomodel(Point p)
{
#if 1
    Point np(0,0,0);
    if(view->getperspective())
    //if(0)
    {
        /*
        Transform m,inv;
        m = setupmatrix(view->geteye().x,view->geteye().y,view->geteye().z,
                        view->getref().x,view->getref().y,view->getref().z,
                        view->getup().x,view->getup().y,view->getup().z,
                        view->getfront(),view->getback(),view->getperspective());

        //Matrix trans = m.mat.transpose();
        //glhInvertMatrixd2(trans.elements,inv.mat.elements);
        glhInvertMatrixd2(m.mat.elements,inv.mat.elements);
        np = screentouv(Point(p.x,height-p.y,p.z));
        np = inv.transform(np);
        //np = inv.transform(p);
        */
        //*
        np = screentouv(p);
        np = uvtomodel(np);
        //*/
    }
    else
    {
        np = screentouv(p);
        np = uvtomodel(np);
        //return np;
    }
    return np;
#else
    Point np = screentouv(p);
    np = uvtomodel(np);
    return np;
#endif
}

Transform& View3dSurface::setupmatrix(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int p)
{ //GLfloat m[16];
    Transform m,pers;
Point eye,ref,up,dir,right;
double d,near_plane,far_plane;
int perspective;

 eye.x = (float)xe;  eye.y = (float)ye;  eye.z = (float)ze;
 ref.x = (float)xr;  ref.y = (float)yr;  ref.z = (float)zr;
 up.x = (float)xu;  up.y = (float)yu;  up.z = (float)zu;

 dir = eye - ref;
 d = dir.length();

 perspective = p;
 if (perspective)
   {
      //near_plane = d - d / (d+front);
      //far_plane = d - d / (d+back);
      near_plane = -front;
      far_plane = -back;
      // near_plane = -(d + front);
      // far_plane = -(d + back);
   }
 else
   {  far_plane = d - front;
      near_plane = d - back;
   }

 dir = dir.normalize();
 right = up.cross(dir);
 up = dir.cross(right);
 right = right.normalize();
 up = up.normalize();

 m.mat.elements[0] = right.x;
 m.mat.elements[4] = right.y;
 m.mat.elements[8] = right.z;
 m.mat.elements[1] = up.x;
 m.mat.elements[5] = up.y;
 m.mat.elements[9] = up.z;
 m.mat.elements[2] = dir.x;
 m.mat.elements[6] = dir.y;
 m.mat.elements[10] = dir.z;
 m.mat.elements[3] = 0.0;
 m.mat.elements[7] = 0.0;
 m.mat.elements[11] = 0.0;
 m.mat.elements[15] = 1.0;

 m.mat.elements[12] = -right.dot(eye);
 m.mat.elements[13] = -up.dot(eye);
 m.mat.elements[14] = -dir.dot(eye);

 if (perspective)
   {
      pers.mat.elements[0] = (float)(2.0*near_plane/(umax-umin)); pers.mat.elements[1] = 0.0f;                                 pers.mat.elements[2] = 0.0f;                                                    pers.mat.elements[3]  = 0.0f;
      pers.mat.elements[4] = 0.0f;                                pers.mat.elements[5] = (float)(2.0*near_plane/(vmax-vmin));  pers.mat.elements[6] = 0.0f;                                                    pers.mat.elements[7]  = 0.0f;
      pers.mat.elements[8] = (float)((umin+umax)/(umax-umin));    pers.mat.elements[9] = (float)((vmax+vmin)/(vmax-vmin));     pers.mat.elements[10] = (float)(far_plane/(near_plane-far_plane));              pers.mat.elements[11] = -1.0f;
      pers.mat.elements[12]  = 0.0f;                              pers.mat.elements[13] = 0.0f;                                pers.mat.elements[14] = (float)(far_plane*near_plane/(near_plane-far_plane));   pers.mat.elements[15] = 0.0f;

      //m[0] = (float)(2.0*near_plane/(umax-umin));           m[1] = 0.0f;                                 m[2] = 0.0f;                                                        m[3]  = 0.0f;
      //m[4] = 0.0f;                                          m[5] = (float)(2.0*near_plane/(vmax-vmin));  m[6] = 0.0f;                                                        m[7]  = 0.0f;
      //m[8] = (float)((umin+umax)/(umax-umin));              m[9] = (float)((vmax+vmin)/(vmax-vmin));     m[10] = (float)(near_plane+far_plane/(near_plane-far_plane));       m[11] = -1.0f;
      //m[12]  = 0.0f;                                        m[13] = 0.0f;                                m[14] = (float)(2.0*far_plane*near_plane/(near_plane-far_plane));   m[15] = 0.0f;
 }
 else
   {  pers.mat.elements[0] = (float)(2.0/(umax-umin));           pers.mat.elements[1] = 0.0f;                               pers.mat.elements[2] = 0.0f;                                          pers.mat.elements[3]  = 0.0f;
      pers.mat.elements[4] = 0.0f;                               pers.mat.elements[5] = (float)(2.0/(vmax-vmin));           pers.mat.elements[6] = 0.0f;                                          pers.mat.elements[7]  = 0.0f;
      pers.mat.elements[8] = 0.0f;                               pers.mat.elements[9] = 0.0f;                               pers.mat.elements[10] = (float)(1.0/(near_plane-far_plane));          pers.mat.elements[11] = 0.0f;
      pers.mat.elements[12] = (float)((umin+umax)/(umin-umax));  pers.mat.elements[13] = (float)((vmax+vmin)/(vmin-vmax));  pers.mat.elements[14] = (float)(near_plane/(near_plane-far_plane));   pers.mat.elements[15] = 1.0f;
   }

 m = m.apply(pers);

 return m;
}


int View3dSurface::updatematrix(void)
{Point dop,vrp,rx,ry,rz;
 Transform t,t1,t2,t3,t4,t5,t6,tmv;
  if (view->getperspective())
    {  t1.translate(view->geteye() * -1.0);
       rz = (view->getref() - view->geteye()) * -1.0;   // ref - eye is the VPN direction
       rz /= rz.length();
       rx =  rz.cross(view->getup()) * -1.0;
       rx /= rx.length();
       uaxis = rx;
       ry = rz.cross(rx);
       ry /= ry.length();
       vaxis = ry;
       waxis = rx.cross(ry);
       t2.rotate(rx,ry,rz);
       t3.scale(1.0,1.0,-1.0);
       t = t1.apply(t2).apply(t3);
       vrp = t.transform(view->getref());
       t1.translate(view->geteye() * -1.0);
       t2.rotate(rx,ry,rz);
       t3.scale(1.0,1.0,-1.0);
       t4.shearz(-(vrp.x + 0.5 * (umin + umax)) / vrp.z,-(vrp.y + 0.5 * (vmin + vmax)) / vrp.z);
       t5.scale(2.0*vrp.z/(umax-umin)/(vrp.z+view->getback()),
                2.0*vrp.z/(vmax-vmin)/(vrp.z+view->getback()),1.0/(vrp.z+view->getback()));
       t1 = t1.apply(t2).apply(t3).apply(t4).apply(t5);
       t1.copy(&m11,&m12,&m13,&m14,&m21,&m22,&m23,&m24,&m31,&m32,&m33,&m34);
       zmin = (vrp.z + view->getfront()) / (vrp.z + view->getback());
       a1 = getwidth() / 2.0;  a2 = getwidth() / 2.0;
       b1 = getheight() / 2.0; b2 = getheight() / 2.0;
       clip.n1 =  ((view->getref() + vaxis * vmax - view->geteye()).cross(uaxis)).normalize();
       clip.d1 =  -clip.n1.dot(view->geteye());
       clip.n2 =  ((view->getref() + vaxis * vmin - view->geteye()).cross(-uaxis)).normalize();
       clip.d2 =  -clip.n2.dot(view->geteye());
       clip.n3 =  ((view->getref() + uaxis * umax - view->geteye()).cross(-vaxis)).normalize();
       clip.d3 =  -clip.n3.dot(view->geteye());
       clip.n4 =  ((view->getref() + uaxis * umin - view->geteye()).cross(vaxis)).normalize();
       clip.d4 =  -clip.n4.dot(view->geteye());
       clip.n5 = -waxis;
       clip.d5 =  waxis.dot(view->getref() - waxis * view->getfront());
       clip.n6 =  waxis;
       clip.d6 = -waxis.dot(view->getref() - waxis * view->getback());
   }
  else
    {  t1.translate(view->getref() * -1.0);
       rz = (view->getref() - view->geteye()) * -1.0;   // ref - eye is the VPN direction
       rz /= rz.length();
       rx =  rz.cross(view->getup()) * -1.0;
       rx /= rx.length();
       uaxis = rx;
       ry = rz.cross(rx);
       ry /= ry.length();
       vaxis = ry;
       waxis = rx.cross(ry);
       t2.rotate(rx,ry,rz);
       t3.scale(1.0,1.0,-1.0);
       t = t2.apply(t3);
       dop = t.transform(view->getref() - view->geteye());
       t2.rotate(rx,ry,rz);
       t3.scale(1.0,1.0,-1.0);
       t4.shearz(-dop.x / dop.z,-dop.y / dop.z);
       t5.translate(-umin,-vmin,-view->getfront());
       t6.scale(1.0/(umax-umin),1.0/(vmax-vmin),1.0/(view->getback()-view->getfront()));
       t1 = t1.apply(t2).apply(t3).apply(t4).apply(t5).apply(t6);
       t1.copy(&m11,&m12,&m13,&m14,&m21,&m22,&m23,&m24,&m31,&m32,&m33,&m34);
       a1 = getwidth();  a2 = 0.0;
       b1 = getheight(); b2 = 0.0;
       clip.n1 = -uaxis;
       clip.d1 =  uaxis.dot(view->getref() + uaxis * umax);
       clip.n2 =  uaxis;
       clip.d2 = -uaxis.dot(view->getref() + uaxis * umin);
       clip.n3 = -vaxis;
       clip.d3 =  vaxis.dot(view->getref() + vaxis * vmax);
       clip.n4 =  vaxis;
       clip.d4 = -vaxis.dot(view->getref() + vaxis * vmin);
       clip.n5 = -waxis;
       clip.d5 =  waxis.dot(view->getref() - waxis * view->getfront());
       clip.n6 =  waxis;
       clip.d6 = -waxis.dot(view->getref() - waxis * view->getback());
    }
  clipplanesvalid = 1;
  return 1;
}

double View3dSurface::pick(Point p,int snap,int x,int y)
{Point p1;
 double dx,dy,t2;
  if (snap == 100)
    {  t2 = db.getstoler();
       snap = 0;
    }
  else
    t2 = db.getstoler() * 2.5;

  p1 = modeltoview(p);
  if (inside(p1))
    {  p1 = viewtoscreen(p1);
       dx = fabs(p1.x - x);  dy = fabs(p1.y - y);
       if (snap || dx < t2 && dy < t2)
         return sqrt(dx*dx + dy*dy);
    }
  return -1.0;
}

double View3dSurface::pick(Point p1,Point p2,double x,double y,Point *pos)
{Point pt1,pt2,pt3,pt4,pt5,pt6;
  double denom,t,d,dx1,dy1,dx2,dy2,t1,t2,d1,d2,d3,toler;
  toler = db.getstoler();
  pt1 = modeltoview(p1);
  pt2 = modeltoview(p2);
  if (pos != NULL)
    {  pt3 = pt1;  pt4 = pt2;
    }
  if (clip3d(&pt1,&pt2))
    {  if (pos != NULL)
         {  pt5 = pt1;  pt6 = pt2;
         }
       pt1 = viewtoscreen(pt1);
       pt2 = viewtoscreen(pt2);
       dx1 = pt2.x - pt1.x;  dy1 = pt2.y - pt1.y;
       dx2 = x - pt1.x;      dy2 = y - pt1.y;
       denom = dx1*dx1 + dy1*dy1;
       if (denom < toler)
         {  dx1 = fabs((pt1.x+pt2.x)/2.0 - x);
            dy1 = fabs((pt1.y+pt2.y)/2.0 - y);
            if (dx1 < toler && dy1 < toler)
              {  if (pos != NULL) *pos = (p1+p2)/2.0;

                 return sqrt(dx1*dx1+dy1*dy1);
              }
            return -1.0;
         }
       t = (dx1*dx2 + dy1*dy2) / denom;
       if (t >= -0.000001 && t <= 1.000001)
         {  d = fabs(dy1 * x - dx1 * y - dy1 * pt1.x + dx1 * pt1.y) / sqrt(denom);
            if (d < toler)
              {  if (pos != NULL)
                   {  d1 = (pt4-pt3).length();
                      d2 = (pt5-pt3).length();
                      d3 = (pt6-pt3).length();
                      t1 = d2 / d1;  t2 = d3 / d1;
                      t = t1 + (t2-t1) * t;
                      *pos = p1 + (p2 - p1) * t;
                   }
                 return d;
              }
         }
    }
  return -1.0;
}

double View3dSurface::pickhor(Point p1,Point p2,int x,int y,Point *pos,double *side)
{Point pt1,pt2,pt3,pt4,pt5,pt6;
 double t,d,t1,t2,d1,d2,d3;
 const double tolerance = 0.0001;
  pt1 = modeltoview(p1);
  pt2 = modeltoview(p2);
  if (pos != 0)
    {  pt3 = pt1;  pt4 = pt2;
    }
  if (! clip3d(&pt1,&pt2))
    return -1.0;
  if (pos != 0)
    {  pt5 = pt1;  pt6 = pt2;
    }
  pt1 = viewtoscreen(pt1);
  pt2 = viewtoscreen(pt2);
  if (pt1.y < pt2.y - tolerance && pt1.y - tolerance <= y && y <= pt2.y + tolerance ||
      pt2.y < pt1.y - tolerance && pt2.y - tolerance <= y && y <= pt1.y + tolerance)
    {  t = (y - pt1.y) / (pt2.y - pt1.y);
       d = fabs(pt1.x + (y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y) - x);
       if (pos != 0)
         {  d1 = (pt4-pt3).length();
            d2 = (pt5-pt3).length();
            d3 = (pt6-pt3).length();
            t1 = d2 / d1;  t2 = d3 / d1;
            t = t1 + (t2-t1) * t;
            *pos = p1 + (p2 - p1) * t;
            if (side != 0)
              {  if ((pt1.y - pt2.y)*x     + (pt2.x - pt1.x)*y -
                     (pt1.y - pt2.y)*pt1.x - (pt2.x - pt1.x)*pt1.y < 0.0)
                   *side = -1.0;
                 else
                   *side = 1.0;
              }
         }
       return d;
   }
  return -1.0;
}

int View3dSurface::pick(Point p,int insidew,int x1,int y1,int x2,int y2)
{Point pt;
 int in;
  pt = modeltoview(p);
  in = 0;
  if (inside(pt))
    {  pt = viewtoscreen(pt);
       if (x1 < pt.x && pt.x < x2 && y1 < pt.y && pt.y < y2)
         in = 1;
    }
  else
    {  if (pt.z > 1.0 || pt.z < zmin)
         return 0;
    }
  return ! (insidew ^ in);
}

int View3dSurface::pick(int nvertices,Point *pwvertices,Point p1,Point p2,int inside,int *picked)
{int j,k,found;
 double u1,u2,u3,t1,t2,D1,llength;
 Point p,p3,p4,p5,dir1,n1,dir2;
 SortedDoubleList l;

  p1 = screentouv(modeltoscreen(p1));
  p2 = screentouv(modeltoscreen(p2));

  if (fabs(p2.x-p1.x) < 1.0E-8 && fabs(p2.y-p1.y) < 1.0E-8)
    p2.y += 1.0E-6;
  dir2 = (p2 - p1).normalize();
  llength = (p2 - p1).length();

  n1 = (Point(0.0,0.0,1.0).cross(p2 - p1)).normalize();
  D1 = -n1.dot(p1);
  
  p3 = pwvertices[0];
  u1 = n1.dot(p3) + D1;
  for (j = 1 ; j <= nvertices ; j++)
    {  p4 = pwvertices[j % nvertices];
       u2 = n1.dot(p4) + D1;
       if (u1 <= 0.0 && u2 >= 0.0 || u1 >= 0.0 && u2 <= 0.0)
         {  if (u1 == 0.0)
              { // Do nothing if u1 == 0.0
              }
            else //if (fabs(u2 - u1) > db.getptoler())
              {  for (k = j + 1,u3 = u2 ; u3 == 0.0 ; k++)
                   u3 = n1.dot(pwvertices[k % nvertices]) + D1;
                 if (u1 < 0.0 && u3 > 0.0 || u1 > 0.0 && u3 < 0.0)
                   {  p = p3 - (p4 - p3) * (u1 / (u2 - u1));
                      dir1 = p - p1;
                      if (fabs(dir2.x) >= fabs(dir2.y))
                        l.add(dir1.x / dir2.x);
                      else
                        l.add(dir1.y / dir2.y);
                   }
              }
         }
       p3 = p4;  u1 = u2;
    }

  found = 0;
  for (l.start() ; ! l.atend() ; )
    {  t1 = l.next();  if (l.atend()) break;
       t2 = l.next();
       if (t1 <= -0.00001 && t2 >= llength - 0.000001)
         found = 1;
       else if (found == 0 && t1 <= -0.00001 && t2 >= 0.0001 || t1 < llength - 0.00001 && t2 > llength + 0.00001)
         found = 2;
    }
  l.destroy();

  if ((inside == 0 || inside == 1) && found != inside)  //  All inside or all outside
    {  *picked = 0;
       return 1;  //  The line segment was not in the right place - so fail and return a result.
    }
  else if (inside == 2 && found == 2)  //  Cross
    {  *picked = 1;
       return 1;  //  The line segment crosses was not in the right place - so fail and return a result.
    }
  else
    return 0;  //  Can't decide yet - so just continue the search

}

void MainWindow::setdepth(void)
{
 double depth;
 ResourceString rs9(NCVIEWA+9);
 ResourceString rs10(NCVIEWA+10);
 ResourceString rs11(NCVIEWA+11);
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
  {
      cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONSTOP);
      return;
  }
  depth = v.getreal("vw::depth");
  Dialog dialog(rs10.gets(),rs11.gets(),&depth,ScaleByLength);
  if (dialog.process() == TRUE)
    v.setreal("vw::depth",depth);
}
