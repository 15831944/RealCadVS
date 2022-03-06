
#include "ncwin.h"
#include "RCView.h"

extern double AngleBetween(const ::Point& v1,const ::Point& v2,const ::Point& vz);

void solid_command0(int *cstate,HighLevelEvent *,void **)
{
   cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Solid"));
  *cstate = ExitState;
}

void surface_command0(int *cstate,HighLevelEvent *,void **)
{
   cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Surface"),buttonmenus.match("Tool"));
  *cstate = ExitState;
}

void new_surface_command0(int *cstate,HighLevelEvent *,void **)
{
   cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("NewSurface"));
  *cstate = ExitState;
}

void plane_command0(int *cstate,HighLevelEvent *,void **)
{
  cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Plane"));
  *cstate = ExitState;
}

/**
 * @brief plane_command1
 * @param cstate
 * @param event
 *
 * Entry point for the Insert Plane by boundary command
 *
 * KMJ : Working in Qt version
 *
 */
void plane_command1(int *cstate,HighLevelEvent *event,void **)
{EntityList list,planelist,elist;
 Entity *e,*b;
 Plane *p;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point origin,xaxis,yaxis;
 BitMask options(32);
 ResourceString rs0(NCPLANEB),rs1(NCPLANEB+1),rs2(NCPLANEB+2);
 ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;
 
  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
  {
      ::state.sendevent(new NewCommandEvent(409,0));
       *cstate = ExitState;
       return;
  }

  list = state.getselection().copy();
  if (list.length() == 0)
    {  cadwindow->MessageBox(rs1.gets(),rs4.gets(),MB_ICONINFORMATION);
       *cstate = ExitState;
       return;
    }
  db.header.setvisible(state.getlastvisible());

  for (list.start() ; (e = list.next()) != NULL ; )
    if (e->isa(plane_entity))
      {  list.del(e);  planelist.add(e);
         elist = ((Plane *)e)->getlist();
         for (elist.start() ; ! elist.atend() ; )
           if ((b = elist.next()) != 0 && ! list.inlist(b))
             list.add(b);
      }
    else if (! e->islinear())
      {  cadwindow->MessageBox(rs0.gets(),rs4.gets(),MB_ICONINFORMATION);
         list.destroy();
         *cstate = ExitState;
         return;
      }
  Loops loops(1);
  loops.create(&list);
  if (! list.empty() || loops.getloops().empty())
    {  cadwindow->MessageBox(rs2.gets(),rs4.gets(),MB_ICONINFORMATION);
       list.destroy();
       loops.destroy();
    }
  else
    {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
       Plane *plane = new Plane(loops.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       if (plane->getdefined())
         {EntityList list;
            state.destroyselection(1);
            for (planelist.start() ; (p = (Plane *)planelist.next()) != 0 ; )
              db.geometry.del(p);
            db.geometry.add(plane);
            //db.geometry.tofront(plane);
            db.geometry.toback(plane);
            list = plane->getlist();
            for (list.start() ; ! list.atend() ; )
            {
                if ((e = list.next()) != 0)
                {
                    //db.geometry.tofront(e);
                    e->draw(DM_NORMAL);
                }
            }
              
         }
       else
         {  cadwindow->MessageBox(rs3.gets(),rs4.gets(),MB_ICONINFORMATION);
            planelist.destroy();
            delete plane;
         }
    }
  *cstate = ExitState;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);


struct RBPlane2_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
    qreal zValue;
#endif
    Point p1;
};

#ifdef NEW_RUBBERB
void RBPlane2(int dm,void *data,Point *p2,View3dSurface *)
{
    if(dm ==  RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
   Line line(((RBPlane2_data *)data)->p1,*p2);
   line.draw(DM_INVERT);
}
#else
void RBPlane2(int dm,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
    Line line(((RBPlane2_data *)data)->p1,*p2);
    line.qgi = ((RBPlane2_data *)data)->qgi;
    line.zValue = ((RBPlane2_data *)data)->zValue;
        line.draw(DM_INVERT);
    ((RBPlane2_data *)data)->qgi = line.qgi;
    ((RBPlane2_data *)data)->zValue = line.zValue;
#else
   Line line(((RBPlane2_data *)data)->p1,*p2);
   line.draw(DM_INVERT);
#endif
}
#endif
/**
 * @brief getPlaneChangeValues
 * @param change
 * @param list
 * @param hd
 * @param xs
 * @param ys
 * @param an
 * @param ca
 * @param tr
 * @param cr
 * @param cg
 * @param cb
 * @param trg
 * @param crf
 * @param pfn
 * @param o
 * @param difgcols = a bit set that show which gradation colours are different. bit:0=red is different; bit:1=green is different; bit:2=blue is different
 * @return
 */
int getPlaneChangeValues(BitMask *change,EntityList &list,EntityHeader *hd,double *xs,double *ys,double *an,double *ca,double *tr,double *cr,double *cg,double *cb,double *trg,RCCOLORREF *crf,RCCHAR **pfn, BitMask *o, BitMask *dgc)
{
    Entity *e;
    BitMask options(32),toptions(32),difgcols(3);
    int gred,ggreen,gblue,tgred,tggreen,tgblue;
    Plane *plane;

    // values to be compared

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(plane_entity))
        {
            plane = (Plane*)e;

            if(i==0)
            {
                hd->setcolour(plane->getcolour());
                hd->setlayer(plane->getlayer());
                hd->setstyle(plane->getstyle());
                hd->setweight(plane->getweight());

                *xs = plane->getxspacing();
                *ys = plane->getyspacing();
                *an = plane->getangle();
                *ca = plane->getcrossangle();
                *tr = plane->gettransparency();
                *cr = plane->getred();
                *cg = plane->getgreen();
                *cb = plane->getblue();
                *trg = plane->gettransparencyrange();
                *crf = plane->getgradientcolour();
                gred = GetRValue(*crf);
                ggreen = GetGValue(*crf);
                gblue = GetBValue(*crf);
                *pfn  = plane->getpatternfilename();
                options = plane->getoptions();
                *o = plane->getoptions();

                change->clearall();

                change->set(0);// def color
                change->set(1);// def layer
                change->set(2);// def style
                change->set(3);// def line weight

                change->set(4);// fill style
                change->set(5);// display edges
                change->set(6);// pattern filename
                change->set(7);// gradient style
                change->set(8);// gradient colour
                dgc->set(0); // red gradient colour diff switch
                dgc->set(1); // green gradient colour diff switch
                dgc->set(2); // blue gradient colour diff switch
                change->set(9);// x spacing
                change->set(10);// y spacing
                change->set(11);// angle
                change->set(12);// cross angle
                change->set(13);// transparency
                change->set(14);// transparent colour
                change->set(15);// transparency range
            }
            else
            {
                toptions = plane->getoptions();
                tgred = GetRValue(plane->getgradientcolour());
                tggreen = GetGValue(plane->getgradientcolour());
                tgblue = GetBValue(plane->getgradientcolour());

                if(hd->getcolour() != plane->getcolour())
                    change->clear(0);
                if(hd->getlayer() != plane->getlayer())
                    change->clear(1);
                if(hd->getstyle() != plane->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),plane->getweight()))
                    change->clear(3);

                if(options.test(PlaneNoFill) != toptions.test(PlaneNoFill) ||
                   options.test(PlanePatterned) != toptions.test(PlanePatterned) ||
                   options.test(PlaneBitmapFilled) != toptions.test(PlaneBitmapFilled) ||
                   options.test(PlaneFilled) != toptions.test(PlaneFilled) ||
                   options.test(PlaneGradientFilled) != toptions.test(PlaneGradientFilled) )
                    change->clear(4);// fill style
                if(options.test(PlaneDisplayEdges) != toptions.test(PlaneDisplayEdges))
                    change->clear(5);// display edges
                if(*pfn != 0 && plane->getpatternfilename() != 0)
                    if(strcmp(*pfn,plane->getpatternfilename()) != 0)
                        change->clear(6); // pattern file name
                if(options.test(PlaneGradientFilled) == toptions.test(PlaneGradientFilled) &&
                   (options.test(PlaneGradientStyle1) != toptions.test(PlaneGradientStyle1) ||
                    options.test(PlaneGradientStyle2) != toptions.test(PlaneGradientStyle2) ||
                    options.test(PlaneGradientStyle3) != toptions.test(PlaneGradientStyle3) ||
                    options.test(PlaneGradientStyle4) != toptions.test(PlaneGradientStyle4)) )
                    change->clear(7); // gradient style
                if(*crf != plane->getgradientcolour())
                {
                    change->clear(8); // gradient colour
                    // clear the colours which are different
                    if(gred != tgred)
                        dgc->clear(0);
                    if(ggreen != tggreen)
                        dgc->clear(1);
                    if(gblue != tgblue)
                        dgc->clear(2);
                }
                if(fabs(*xs - plane->getxspacing()) > 10.0e-5)
                    change->clear(9); // x spacing
                if(fabs(*ys - plane->getyspacing()) > 10.0e-5)
                    change->clear(10); // y spacing
                if(fabs(*an - plane->getangle()) > 10.0e-5)
                    change->clear(11); // angle
                if(fabs(*ca - plane->getcrossangle()) > 10.0e-5)
                    change->clear(12); // angle
                if(fabs(*tr - plane->gettransparency()) > 10.0e-5)
                    change->clear(13); // transparency
                if(fabs(*cr - plane->getred()) > 10.0e-5   ||
                   fabs(*cg - plane->getgreen()) > 10.0e-5 ||
                   fabs(*cb - plane->getblue()) > 10.0e-5)
                    change->clear(14); // tansparency colour
                if(fabs(*trg - plane->gettransparencyrange()) > 10.0e-5)
                    change->clear(15);// transparency range
            }
            i++;
        }
    }
    return i;
}

/**
 * @brief plane_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Plane options dialog box
 *
 *
 */
void plane_command2(int *cstate,HighLevelEvent *event,void **data)
{RBPlane2_data *rbdata = (RBPlane2_data *) *data;
 static EntityList list;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask change(32),options(32),dgc(3);
 EntityHeader header;
 Entity *e;
 ResourceString rs5(NCPLANEB+5),rs6(NCPLANEB+6);
 RCCHAR *pfn=0,patternfilename[300];
 int status,length;
 Plane *plane;
 RCCOLORREF gradientcolour;
 Point p1,p2;

  v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
    {
#ifdef USING_WIDGETS
            if(rbdata !=0)
                rbdata->qgi = 0;
#endif
            list = state.getselection().copy();
       if (list.empty())
       {
            options.clearall();
            Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            status = Plane::setup(NULL,&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            if (status == TRUE)
            {
                 Plane::setdefaults(xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 *cstate = ExitState;
            }
            else if (status == 131 /*120*/)// clone
              *cstate = 1;
            else if (status == 132 /*122*/) // clone all
              *cstate = 3;
            else if(status == 113) // set anchor
            {
                ::state.sendevent(new NewCommandEvent(1511,0));
                *cstate = ExitState;
            }
            else
              *cstate = ExitState;      
       }
       else
       {
            Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found = getPlaneChangeValues(&change,list,&header,&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,&pfn,&options,&dgc);
            if(patternfilename != 0 && pfn != 0)
                RCSTRCPY(patternfilename,pfn);
            v.setinteger("op::changecount",found);
            if(found == 1)
                status = Plane::setup(&change,&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options,&header,&dgc);
            else if(found > 1)
                status = Plane::setup(&change,&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options,&header,&dgc);
            else
                status = Plane::setup(0,&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            if (status == TRUE)
            {
                 list = state.getselection().copy();
                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(plane_entity))
                      ((Plane *)e)->change(change,header,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();
                 *cstate = ExitState;
            }
            else if (status == 111) // set surface properties
            {
                SurfaceProperties sp;
                if(found == 1)
                {
                    list = state.getselection().copy();
                    length = list.length();
                    RemoveLockedEntities(&list,0);

                    // change the first entity in the list
                    for (list.start() ; (e = list.next()) != NULL ; )
                    {
                      if (e->isa(plane_entity))
                      {
                          sp.verify(e,0);
                          break;
                      }
                    }
                    if (list.length() != length)
                      cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                    list.destroy();
                }
                else if(found > 1)
                {
                    if(sp.setup(1))
                    {
                        list = state.getselection().copy();
                        length = list.length();
                        RemoveLockedEntities(&list,0);

                        for (list.start() ; (e = list.next()) != NULL ; )
                          if (e->isa(plane_entity))
                            sp.change(e,0);

                        if (list.length() != length)
                          cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                        list.destroy();
                    }
                }
                *cstate = ExitState;
            }
            else if (status == 131/*120*/) // clone
              *cstate = 2;
            else if (status == 132/*122*/) // clone all
              *cstate = 4;
            else if (status == 112 /*123*/) // align
            {
                 *cstate = 5;
                 rbdata = new RBPlane2_data;  *data = rbdata;
#ifdef USING_WIDGETS
                 if(rbdata != 0)
                 {
                    rbdata->qgi = 0;
                    rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 }
#endif
            }
            else if(status == 113) // set anchor
            {
                ::state.sendevent(new NewCommandEvent(1511,0));
                *cstate = ExitState;
            }
            else 
              *cstate = ExitState;
        }
  }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
  {
      if (event->isa(EntitySelected))
      {
            state.destroyselection(1);
            plane = (Plane *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  Plane::setdefaults(plane->getxspacing(),plane->getyspacing(),plane->getangle(),plane->getcrossangle(),plane->gettransparency(),plane->getred(),plane->getgreen(),plane->getblue(),plane->gettransparencyrange(),plane->getgradientcolour(),plane->getpatternfilename(),plane->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(plane->getcolour(),plane->getlayer(),plane->getstyle(),plane->getweight());
                      v.setinteger("df::colour",plane->getcolour());
                      v.setinteger("df::layer",plane->getlayer());
                      v.setinteger("df::style",plane->getstyle());
                      v.setinteger("df::weight",plane->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              }
            else
              {  if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 4 ; i <= 13 ; i++) change.set(i,1);
                 length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(plane_entity))
                     ((Plane *)e)->change(change,*plane,plane->getxspacing(),plane->getyspacing(),plane->getangle(),plane->getcrossangle(),plane->gettransparency(),plane->getred(),plane->getgreen(),plane->getblue(),plane->gettransparencyrange(),plane->getgradientcolour(),plane->getpatternfilename(),plane->getoptions());
                 db.saveundo(UD_ENDBLOCK,NULL);
                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);
                 state.setselection(list,0);
              }
            *cstate = ExitState;
       }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
  }
  else if (*cstate == 5)
  {  if (event->isa(Coordinate3d))
         { rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
            *cstate = 6;
         }
       else if (event->isa(EntitySelected))
         {  p1 = ((Line *)(((EntitySelectedEvent *)event)->getentity()))->getp1();
            p2 = ((Line *)(((EntitySelectedEvent *)event)->getentity()))->getp2();
            //  Align the plane to p1 and p2
            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if (e->isa(plane_entity))
                {  e->draw(DM_ERASE);
                   ((Plane *)e)->AlignPlane(p1,p2);
                   e->draw(DM_NORMAL);
                }
            db.saveundo(UD_ENDBLOCK,NULL);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
  }
  else if (*cstate == 6)
  {  rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p1 = rbdata->p1;
            p2 = ((Coordinate3dEvent *)event)->getp();
            //  Align the plane to p1 and p2

            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if (e->isa(plane_entity))
                {  e->draw(DM_ERASE);
                   ((Plane *)e)->AlignPlane(p1,p2);
                   e->draw(DM_NORMAL);
                }
            db.saveundo(UD_ENDBLOCK,NULL);
            *cstate = 5;

         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
  }

#ifdef USING_WIDGETS
    if(rbdata !=0)
    {
        if(rbdata->qgi)
            cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
        delete rbdata->qgi; rbdata->qgi=0;
    }
#endif

  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(plane_entity,end_list);
       cadwindow->prompt->normalprompt("Select the plane entity to clone from");
    }
  else if (*cstate == 5)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Select the start position or the line to align to");
    }
  else if (*cstate == 6)
    {  rubberband.begin(0);
       rubberband.add(RBPlane2,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Select the end position to align to");
    }
  else if(*cstate == ExitState)
  {
      if(rbdata)
          rbdata->qgi=0;
  }

}

class PickPointItem
{
public:
    Entity *mPickEnt;
    Point mPickPoint;
    double mt;
public:
    PickPointItem()
    {
        mPickEnt = nullptr;
        mt = 0.0;
    }
    PickPointItem(Entity *e, Point pp)
    {
        mPickEnt = e;
        mPickPoint = pp;
        mt = ((LinearEntity*)mPickEnt)->gett(mPickPoint);
    }
    PickPointItem(const PickPointItem &o)
    {
        mPickEnt = o.mPickEnt;
        mPickPoint = o.mPickPoint;
        mt = o.mt;
    }
    PickPointItem& operator=(const PickPointItem &o)
    {
        mPickEnt = o.mPickEnt;
        mPickPoint = o.mPickPoint;
        mt = o.mt;
        return *this;
    }
    ~PickPointItem()
    {
        mPickEnt = nullptr;
    }
};

class IntersectPointItem
{
public:
    PointE * mPoint;
    Entity * mEnt1;
    Entity * mEnt2;
    double t1,t2;
public:
    IntersectPointItem()
    {
        mPoint = nullptr;
        mEnt1  = nullptr;
        mEnt2  = nullptr;
        t1=0.0;
        t2=0.0;
    }
    IntersectPointItem(PointE *p, Entity *e1, Entity *e2)
    {
        mPoint = p;
        mEnt1  = e1;
        mEnt2  = e2;
        t1 = ((LinearEntity*)e1)->gett(p->point(0));
        t2 = ((LinearEntity*)e2)->gett(p->point(0));
    }
    IntersectPointItem(const IntersectPointItem &o)
    {
        mPoint = o.mPoint;
        mEnt1  = o.mEnt1;
        mEnt2  = o.mEnt2;
        t1 = o.t1;
        t2 = o.t2;
    }
    IntersectPointItem& operator=(const IntersectPointItem &o)
    {
        mPoint = o.mPoint;
        mEnt1  = o.mEnt1;
        mEnt2  = o.mEnt2;
        t1 = o.t1;
        t2 = o.t2;
        return *this;
    }
    bool isDummy()
    {
        return (mEnt1 == mEnt2);
    }
    ~IntersectPointItem()
    {
        if(mPoint != nullptr)
            delete mPoint;
        mPoint = nullptr;
        mEnt1  = nullptr;
        mEnt2  = nullptr;
    }
};
typedef std::vector<IntersectPointItem*> InterPointItemList;

class sortableInterItem
{
public:
    IntersectPointItem *mInteritem;
    Entity *mEnt;
    double mt;
public:
    sortableInterItem()
    {
        mInteritem = nullptr;
        mEnt = nullptr;
        mt = 0.0;
    }
    sortableInterItem(IntersectPointItem *item, Entity *e, double t)
    {
        mInteritem = item;
        mEnt = e;
        mt = t;
    }
    sortableInterItem(const sortableInterItem &o)
    {
        mInteritem = o.mInteritem;
        mEnt = o.mEnt;
        mt = o.mt;
    }
    sortableInterItem& operator=(const sortableInterItem &o)
    {
        mInteritem = o.mInteritem;
        mEnt = o.mEnt;
        mt = o.mt;
        return *this;
    }
    ~sortableInterItem()
    {
        mInteritem = nullptr;
        mEnt = nullptr;
        mt = 0.0;
    }
};

bool sortInters(sortableInterItem a, sortableInterItem b)
{
    if(a.mEnt == b.mEnt)
    {
        if(a.mEnt == a.mInteritem->mEnt1 && b.mEnt == b.mInteritem->mEnt1)
        {
            if(a.mInteritem->t1 < b.mInteritem->t1)
                return true;
            else
                return false;
        }
        else if(a.mEnt == a.mInteritem->mEnt2 && b.mEnt == b.mInteritem->mEnt2)
        {
            if(a.mInteritem->t2 < b.mInteritem->t2)
                return true;
            else
                return false;
        }
        else if(a.mEnt == a.mInteritem->mEnt1 && b.mEnt == b.mInteritem->mEnt2)
        {
            if(a.mInteritem->t1 < b.mInteritem->t2)
                return true;
            else
                return false;
        }
        else if(a.mEnt == a.mInteritem->mEnt2 && b.mEnt == b.mInteritem->mEnt1)
        {
            if(a.mInteritem->t2 < b.mInteritem->t1)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool partitionInters(sortableInterItem item)
{
    if(item.mEnt == item.mInteritem->mEnt1)
    {
        if(item.mInteritem->t1 < item.mt)
            return true;
        else
            return false;
    }
    else if(item.mEnt == item.mInteritem->mEnt2)
    {
        if(item.mInteritem->t2 < item.mt)
            return true;
        else
            return false;
    }
    return false;
}

class PickEntItem
{
public:
    Entity *mOrigPickEnt;
    Entity *mPickEnt;
    Entity *mTrimmedEnt;
    PickPointItem mPickPointI;
    InterPointItemList mIntPointItems;
public:
    PickEntItem()
    {
        mOrigPickEnt = nullptr;
        mPickEnt = nullptr;
        mTrimmedEnt = nullptr;
        mPickPointI = PickPointItem();
        mIntPointItems.reserve(2);
    }
    PickEntItem(Entity *e, PickPointItem pp, Entity *oe=nullptr)
    {
        mOrigPickEnt = oe;
        mPickEnt = e;
        mTrimmedEnt = nullptr;
        mPickPointI = pp;
    }
    PickEntItem(const PickEntItem &o)
    {
        mOrigPickEnt = o.mOrigPickEnt;
        mPickEnt = o.mPickEnt;
        mTrimmedEnt = o.mTrimmedEnt;
        mPickPointI = o.mPickPointI;
        mIntPointItems = o.mIntPointItems;
    }
    PickEntItem& operator=(const PickEntItem &o)
    {
        mOrigPickEnt = o.mOrigPickEnt;
        mPickEnt = o.mPickEnt;
        mTrimmedEnt = o.mTrimmedEnt;
        mPickPointI = o.mPickPointI;
        mIntPointItems = o.mIntPointItems;
        return *this;
    }
    void setOriginalEnt(Entity *e)
    {
        mOrigPickEnt = e;
    }
    Entity * getOriginalEnt()
    {
        return mOrigPickEnt;
    }
    Entity * getEnt()
    {
        return mPickEnt;
    }
    Entity * getTrimEnt()
    {
        return mTrimmedEnt;
    }
    void setTrimEnt( Entity *ent)
    {
        if(mTrimmedEnt != nullptr)
        {
            EntityList *dblist = db.geometry.getlist();
            if(dblist->inlist(mTrimmedEnt))
                db.geometry.del(mTrimmedEnt);
        }
        mTrimmedEnt = ent;
    }
    Point getPickPointE()
    {
        return mPickPointI.mPickPoint;
    }
    void addIntPointItem(IntersectPointItem *ipi)
    {
        mIntPointItems.push_back(ipi);
    }
    bool containsIntpoint(IntersectPointItem *ipi)
    {
        if(mIntPointItems.at(0)->mPoint->point(0) == ipi->mPoint->point(0) ||
           mIntPointItems.at(1)->mPoint->point(0) == ipi->mPoint->point(0))
            return true;
        else
            return false;
    }
    void getClosestInters(Point &p0, Point &p1)
    {
        std::vector<sortableInterItem> sorted;
        for(uint i=0; i < mIntPointItems.size(); i++)
        {
            if(mPickEnt == mIntPointItems.at(i)->mEnt1)
            {
                sortableInterItem sitem(mIntPointItems.at(i),mPickEnt,mPickPointI.mt);
                sorted.push_back(sitem);
            }
            else if(mPickEnt == mIntPointItems.at(i)->mEnt2)
            {
                sortableInterItem sitem(mIntPointItems.at(i),mPickEnt,mPickPointI.mt);
                sorted.push_back(sitem);
            }
        }
        std::sort(sorted.begin(),sorted.end(),sortInters);

        if(mPickEnt->type() == circle_entity)
        {
            if(((Circle*)mPickEnt)->getsweepa() > 1.99999 * M_PI)
            {
                sorted.erase(sorted.begin());
                sorted.pop_back();
            }
        }
        else if(mPickEnt->type() == ellipse_entity)
        {
            if(((EllipseE*)mPickEnt)->getsweepa() > 1.99999 * M_PI)
            {
                sorted.erase(sorted.begin());
                sorted.pop_back();
            }
        }

        if(sorted.size() > 0)
        {
            std::vector<sortableInterItem>::iterator bound;
            bound = std::stable_partition(sorted.begin(),sorted.end(),partitionInters);
            if((mPickEnt->type() == circle_entity && ((Circle*)mPickEnt)->getsweepa() > 1.99999 * M_PI) ||
               (mPickEnt->type() == ellipse_entity && ((EllipseE*)mPickEnt)->getsweepa() > 1.99999 * M_PI))
            {
                if(sorted.size() > 1)
                {
                    if(bound == sorted.begin())
                    {
                        p1 = sorted.front().mInteritem->mPoint->point(0);
                        p0 = sorted.back().mInteritem->mPoint->point(0);
                    }
                    else if(bound == sorted.end())
                    {
                        p1 = sorted.back().mInteritem->mPoint->point(0);
                        p0 = sorted.front().mInteritem->mPoint->point(0);
                    }
                    else if(bound == sorted.end()-1)
                    {
                        p1 = bound->mInteritem->mPoint->point(0);
                        bound--;
                        p0 = bound->mInteritem->mPoint->point(0);
                    }
                    else
                    {
                        p1 = bound->mInteritem->mPoint->point(0);
                        bound--;
                        p0 = bound->mInteritem->mPoint->point(0);
                    }
                }
                else
                {
                    p0 = ((LinearEntity*)this->mPickEnt)->start();
                    p1 = ((LinearEntity*)this->mPickEnt)->end();
                }
            }
            else
            {
                if(sorted.size() > 1)
                {
                    if(bound == sorted.end())
                    {
                        if(mPickEnt->type() == circle_entity)
                        {
                            Point start0 = ((LinearEntity*)(bound-2)->mEnt)->start();
                            Point end0 = ((LinearEntity*)(bound-2)->mEnt)->end();
                            Point start1 = ((LinearEntity*)(bound-1)->mEnt)->start();
                            Point end1 = ((LinearEntity*)(bound-1)->mEnt)->end();
                            qDebug() << "start0: " << start0.x << "," << start0.y;
                            qDebug() << "end0: " << end0.x << "," << end0.y;
                            qDebug() << "start1: " << start1.x << "," << start1.y;
                            qDebug() << "end1: " << end1.x << "," << end1.y;
                            p0 = (bound-2)->mInteritem->mPoint->getp();
                            p1 = (bound-1)->mInteritem->mPoint->getp();
                            if(p0 == p1 &&
                              (p1 == ((LinearEntity*)(bound-1)->mEnt)->start() ||
                               p1 == ((LinearEntity*)(bound-1)->mEnt)->end()))
                            {
                                p0 = ((LinearEntity*)(bound-1)->mEnt)->start();
                                p1 = ((LinearEntity*)(bound-1)->mEnt)->end();
                            }

                            qDebug() << "p0: " << p0.x << "," << p0.y;
                            qDebug() << "p1: " << p1.x << "," << p1.y;
                        }
                        else if(mPickEnt->type() == ellipse_entity)
                        {
                            p0 = (bound-2)->mInteritem->mPoint->getp();
                            p1 = (bound-1)->mInteritem->mPoint->getp();
                            //p0 = ((LinearEntity*)(bound-1)->mEnt)->start();
                            //p1 = ((LinearEntity*)(bound-1)->mEnt)->end();
                            //double t0 = ((LinearEntity*)(bound-1)->mEnt)->gett(p0);
                            //double t1 = ((LinearEntity*)(bound-1)->mEnt)->gett(p1);
                            qDebug() << "stop";
                        }
                        else
                        {
                            double it = 0.0;
                            if((bound-1)->mEnt == (bound-1)->mInteritem->mEnt1)
                                it = (bound-1)->mInteritem->t1;
                            else
                                it = (bound-1)->mInteritem->t2;
                            if(it >= (bound-1)->mt)
                            {
                                p0 = ((LinearEntity*)(bound-1)->mEnt)->start();
                                p1 = (bound-1)->mInteritem->mPoint->getp();
                            }
                            else if(it <= (bound-1)->mt)
                            {
                                p0 = ((LinearEntity*)(bound-1)->mEnt)->end();
                                p1 = (bound-1)->mInteritem->mPoint->getp();
                            }
                        }
                    }
                    else if(bound == sorted.begin())
                    {
                        if(mPickEnt->type() == circle_entity)
                        {
                            p0 = bound->mInteritem->mPoint->getp();
                            p1 = (bound+1)->mInteritem->mPoint->getp();
                            if(p0 == p1 &&
                              (p0 == ((LinearEntity*)bound->mEnt)->start() ||
                               p0 == ((LinearEntity*)bound->mEnt)->end()))
                            {
                                p0 = ((LinearEntity*)bound->mEnt)->start();
                                p1 = ((LinearEntity*)bound->mEnt)->end();
                            }
                        }
                        else if(mPickEnt->type() == ellipse_entity)
                        {
                            p0 = (bound+1)->mInteritem->mPoint->getp();
                            p1 = (bound)->mInteritem->mPoint->getp();
                            //p0 = ((LinearEntity*)(bound)->mEnt)->start();
                            //p1 = ((LinearEntity*)(bound)->mEnt)->end();
                            //double t0 = ((LinearEntity*)(bound)->mEnt)->gett(p0);
                            //double t1 = ((LinearEntity*)(bound)->mEnt)->gett(p1);
                            qDebug() << "stop";
                        }
                        else
                        {
                            double it = 0.0;
                            if((bound)->mEnt == (bound)->mInteritem->mEnt1)
                                it = (bound)->mInteritem->t1;
                            else
                                it = (bound)->mInteritem->t2;
                            if(it >= (bound)->mt)
                            {
                                p0 = (bound)->mInteritem->mPoint->getp();
                                p1 = ((LinearEntity*)(bound)->mEnt)->start();
                            }
                            else if(it <= (bound)->mt)
                            {
                                p0 = (bound)->mInteritem->mPoint->getp();
                                p1 = ((LinearEntity*)(bound)->mEnt)->end();
                            }
                        }
                    }
                    else
                    {
                        p1 = bound->mInteritem->mPoint->point(0);
                        bound--;
                        p0 = bound->mInteritem->mPoint->point(0);
                        if(p0 == p1)
                        {
                            p0 = ((LinearEntity*)bound->mEnt)->start();
                            p1 = ((LinearEntity*)bound->mEnt)->end();
                        }
                    }
                }
                else
                {
                    qDebug() << "sorted.size() == 1";
                    double it = 0.0;
                    if(sorted[0].mEnt == sorted[0].mInteritem->mEnt1)
                        it = sorted[0].mInteritem->t1;
                    else
                        it = sorted[0].mInteritem->t2;
                    if(it >= sorted[0].mt)
                    {
                        p0 = ((LinearEntity*)sorted[0].mEnt)->start();
                        p1 = sorted[0].mInteritem->mPoint->getp();
                    }
                    else if(it <= sorted[0].mt)
                    {
                        p0 = ((LinearEntity*)sorted[0].mEnt)->end();
                        p1 = sorted[0].mInteritem->mPoint->getp();
                    }
                }
            }
        }
        else
        {
            p0 = ((LinearEntity*)this->mPickEnt)->start();
            p1 = ((LinearEntity*)this->mPickEnt)->end();
        }
    }
    ~PickEntItem()
    {
        mOrigPickEnt = nullptr;
        //if(mPickEnt != nullptr)
        //    delete mPickEnt;
            //db.geometry.del(mPickEnt);
        mPickEnt = nullptr;
        mTrimmedEnt = nullptr;
        mIntPointItems.clear();
    }
};
typedef std::vector<PickEntItem> PickEntItemList;

void getTrimmedSegments(PickEntItemList &elist, std::vector<Point> &trimPoints)
{
#if 1
    EntityHeader rbHeader(5,0,0,5);
    rbHeader.setlayer(db.header.getlayer());
    rbHeader.setcolour(db.header.getcolour());
    rbHeader.setstyle(db.header.getstyle());
    rbHeader.setweight(db.header.getweight());
    rbHeader.setvisible(*db.header.getvisible());

    for(uint i=0; i<elist.size(); i++)
    {
        Point p0 = trimPoints.at(i*2);
        Point p1 = trimPoints.at(i*2+1);

        qDebug() << "p0 x: " << p0.x << "y: " << p0.y;
        qDebug() << "p1 x: " << p1.x << "y: " << p1.y;

        if(elist.at(i).getOriginalEnt()->type() == line_entity)
        {
            qDebug() << "start x: " << ((LinearEntity*)elist.at(i).getOriginalEnt())->start().x << "y: " << ((LinearEntity*)elist.at(i).getOriginalEnt())->start().y;
            qDebug() << "end   x: " << ((LinearEntity*)elist.at(i).getOriginalEnt())->end().x << "y: " << ((LinearEntity*)elist.at(i).getOriginalEnt())->end().y;

            Line *line = new Line(rbHeader,p0,p1);
            state.excludelist.del(elist.at(i).getTrimEnt());
            elist.at(i).setTrimEnt(nullptr);
            elist.at(i).setTrimEnt(line);
            //line->setcolour(db.header.getcolour());
            //line->setweight(db.header.getweight());
            db.geometry.add(line);
            // debug
            //line->setselected(1);
            //
        }
        else if(elist.at(i).getOriginalEnt()->type() == circle_entity)
        {

            double ts = ((Circle*)elist.at(i).getEnt())->gett(p0);
            double te = ((Circle*)elist.at(i).getEnt())->gett(p1);
            double tm = ts + (te-ts) * 0.5;
            Point pm = ((Circle*)elist.at(i).getEnt())->position(tm);
            if( ts < te )
            {
                Circle *circle = new Circle(1,p0,pm,p1);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(circle);
                //circle->setcolour(rbHeader.getcolour());
                //circle->setweight(rbHeader.getweight());
                db.geometry.add(circle);
            }
            else if( ts > te )
            {
                Circle *circle = new Circle(1,p1,pm,p0);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(circle);
                //circle->setcolour(rbHeader.getcolour());
                //circle->setweight(rbHeader.getweight());
                db.geometry.add(circle);
            }
        }
        else if(elist.at(i).getOriginalEnt()->type() == curve_entity)
        {
            Entity *e = elist.at(i).getEnt();
            double t0 = ((Curve*)e)->gett(p0);
            double t1 = ((Curve*)e)->gett(p1);
            //double disto0 = fabs(t0 - 0.0);
            //double disto1 = fabs(t1 - 1.0);
            if (t0 < t1)
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim start to end
                ((Curve *)ecp)->moveendpoint(1,1,p0);
                ((Curve *)ecp)->moveendpoint(1,2,p1);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(ecp);
                //ecp->setcolour(rbHeader.getcolour());
                //ecp->setweight(rbHeader.getweight());
                db.geometry.add(ecp);
            }
            else
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim end to start
                ((Curve *)ecp)->moveendpoint(1,2,p0);
                ((Curve *)ecp)->moveendpoint(1,1,p1);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(ecp);
                //ecp->setcolour(rbHeader.getcolour());
                //ecp->setweight(rbHeader.getweight());
                db.geometry.add(ecp);
            }
        }
        else if(elist.at(i).getOriginalEnt()->type() == ellipse_entity)
        {
            Entity *e = elist.at(i).getEnt();
            double t0 = ((EllipseE*)e)->gett(p0);
            double t1 = ((EllipseE*)e)->gett(p1);
            double disto0 = fabs(t0 - 0.0);
            double disto1 = fabs(t1 - 1.0);
            if ( t0 < t1)
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                EllipseE *ecp = (EllipseE*)e->clone(ident);
                e->setcopye(0);
                //
                ((EllipseE *)ecp)->moveendpoint(1,1,p0);
                ((EllipseE *)ecp)->moveendpoint(1,2,p1);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(ecp);
                //ecp->setcolour(rbHeader.getcolour());
                //ecp->setweight(rbHeader.getweight());
                db.geometry.add(ecp);
            }
            else if( t0 > t1)
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                EllipseE *ecp = (EllipseE*)e->clone(ident);
                e->setcopye(0);
                //
                ((EllipseE *)ecp)->moveendpoint(1,2,p1);
                ((EllipseE *)ecp)->moveendpoint(1,1,p0);
                elist.at(i).setTrimEnt(nullptr);
                elist.at(i).setTrimEnt(ecp);
                //ecp->setcolour(rbHeader.getcolour());
                //ecp->setweight(rbHeader.getweight());
                db.geometry.add(ecp);
            }
        }
    }
#endif
#if 0
    // find the common points
    EntityHeader rbHeader(5,0,0,5);
    std::vector<PointE*> Points;

    for(uint i=0; i<elist.size(); i++)
    {
        for(uint j=0; j<elist.size(); j++)
        {
            if(i == j)
                continue;

            for(uint k=0; k< elist.at(j).mIntPointItems.size(); k++)
            {
                if(elist.at(i).getEnt() == elist.at(j).mIntPointItems.at(k)->mEnt1 ||
                   elist.at(i).getEnt() == elist.at(j).mIntPointItems.at(k)->mEnt2)
                {
                    if(std::find(Points.begin(),Points.end(),elist.at(j).mIntPointItems.at(k)->mPoint) == Points.end())
                        Points.push_back(elist.at(j).mIntPointItems.at(k)->mPoint);
                }
            }
        }
    }

    for(uint i=0; i<elist.size(); i++)
    {
        PointE *P0=nullptr,*P1=nullptr;

        for(uint L=0; L <2; L++)
        {
            for(uint j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                for(uint k=0; k<Points.size(); k++ )
                {
                    if(elist.at(i).mIntPointItems.at(j)->mPoint == Points.at(k))
                    {
                        if(P0 == nullptr)
                            P0 = elist.at(i).mIntPointItems.at(j)->mPoint;
                        else if(P1 != P0 && P1 == nullptr)
                            P1 = elist.at(i).mIntPointItems.at(j)->mPoint;
                    }
                }
            }
        }
        if(P0 != nullptr && P1 != nullptr)
        {
            Point lp0 = P0->getp();
            Point lp1 = P1->getp();
            Line *line = new Line(rbHeader,lp0,lp1);
            elist.at(i).setTrimEnt(line);
            //line->setcolour(db.header.getcolour());
            //line->setweight(db.header.getweight());
            db.geometry.add(line);
            // debug
            //line->setselected(1);
            //
            qDebug() << "got em";
        }
    }

    // make the trim ents between the points in each entity
    qDebug() << "got em";
#endif
}

// finds loop of intersecting entities
void getIntersectingLoops(PickEntItemList &elist)
{
#if 0
    // make intersection structures for all entities
    EntityList *tlist = db.geometry.getlist();
    Entity *e,*e1;

    for (tlist->start() ; (e = tlist->next()) != NULL ;)
    {
        if(state.excludelist.inlist(e))
            continue;
        if(e->type() == line_entity    ||
           e->type() == circle_entity  ||
           e->type() == ellipse_entity ||
           e->type() == curve_entity )
        {
            Point pp;
            Transform ident;
            ident.identity();
            e->setcopye(0);
            Entity *ecp = e->clone(ident);
            elist.push_back(PickEntItem(ecp,PickPointItem(ecp,pp),e));
            state.excludelist.add(ecp);
            //
            e->setcopye(0);
            ecp = e->clone(ident);
            //ecp->setcolour(5);
            //ecp->setweight(5);
            db.geometry.add(ecp);
            //ecp->setselected(1);
            elist.at(elist.size()-1).setTrimEnt(ecp);
            state.excludelist.add(ecp);
            //e->setselected(0);
        }
    }

    // get the intersection points
    if(elist.size() > 1)
    {
        for (uint l=0; l < elist.size(); l++)
        {
            for (uint j=l; j < elist.size(); j++)
            {
                if(elist.at(l).getOriginalEnt() == elist.at(j).getOriginalEnt())
                    continue;
                Entity *e1;
                e = elist.at(l).getEnt();
                e1 = elist.at(j).getEnt();
                Intersect i(e,e1);
                //qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        double et = ((LinearEntity*)e)->gett(((PointE *)i.intersection(k))->getp());
                        double e1t = ((LinearEntity*)e1)->gett(((PointE *)i.intersection(k))->getp());

                        // no extentions allowed
                        if(et >= 0.0-db.getptoler() && et <= 1.0+db.getptoler() && e1t >= 0.0-db.getptoler() && e1t <= 1.0+db.getptoler())
                        {
                            PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());

                            IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                            elist.at(l).addIntPointItem(ipitem);

                            ipitem = new IntersectPointItem(ip,e1,e);
                            elist.at(j).addIntPointItem(ipitem);

                            //db.geometry.add(ip);
                            //ip->draw(DM_NORMAL);
                        }
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        elist.at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        elist.at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                }
            }
        }
    }

    PickEntItemList eplist;

    for(uint i=0; i < elist.size(); i++)
    {
        if(elist.at(i).mIntPointItems.size() > 2)
        {
            std::vector<sortableInterItem> sorted;
            for(uint j=0; j < elist.at(i).mIntPointItems.size(); j++)
            {
                if(elist.at(i).getEnt() == elist.at(i).mIntPointItems.at(j)->mEnt1)
                {
                    sortableInterItem sitem(elist.at(i).mIntPointItems.at(j),elist.at(i).getEnt(),0.0);
                    sorted.push_back(sitem);
                }
                else if(elist.at(i).getEnt() == elist.at(i).mIntPointItems.at(j)->mEnt2)
                {
                    sortableInterItem sitem(elist.at(i).mIntPointItems.at(j),elist.at(i).getEnt(),0.0);
                    sorted.push_back(sitem);
                }
            }
            std::sort(sorted.begin(),sorted.end(),sortInters);

            for(uint j=0; j < sorted.size(); j++)
            {
                // make a copy of the orignal entity for ech segment
                // make a copy of that entity for the pick ent and the trim ent
                // make a new inter point list for each segment with two intersection points
                // push the new segment onto the eplist

                Entity *e = elist.at(i).getOriginalEnt();
                Point pp,pi;
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Entity *ecp = e->clone(ident);

                PickEntItem pickItem(ecp,PickPointItem(ecp,pp),e);
                state.excludelist.add(ecp);
                //
                e->setcopye(0);
                ecp = e->clone(ident);
                //ecp->setcolour(5);
                //ecp->setweight(5);
                db.geometry.add(ecp);
                //ecp->setselected(1);
                pickItem.setTrimEnt(ecp);
                state.excludelist.add(ecp);
                //e->setselected(0);

                eplist.push_back(pickItem);

                qDebug() << "debug";
            }
            Entity *e = elist.at(i).getOriginalEnt();
            Point pp;
            Transform ident;
            ident.identity();
            e->setcopye(0);
            Entity *ecp = e->clone(ident);
            PickEntItem pickItem(ecp,PickPointItem(ecp,pp),e);
            state.excludelist.add(ecp);
            //
            e->setcopye(0);
            ecp = e->clone(ident);
            //ecp->setcolour(5);
            //ecp->setweight(5);
            db.geometry.add(ecp);
            //ecp->setselected(1);
            pickItem.setTrimEnt(ecp);
            state.excludelist.add(ecp);
            //e->setselected(0);

            eplist.push_back(pickItem);

            qDebug() << "debug";
        }
        else
        {

            PickEntItem pickItem = elist.at(i);
            pickItem.mIntPointItems.clear();

            eplist.push_back(pickItem);
        }
    }

    // get the new intersection points
    if(eplist.size() > 1)
    {
        for (uint l=0; l < eplist.size(); l++)
        {
            for (uint j=l; j < eplist.size(); j++)
            {
                if(elist.at(l).getOriginalEnt() == eplist.at(j).getOriginalEnt())
                    continue;
                Entity *e1;
                e = eplist.at(l).getEnt();
                e1 = eplist.at(j).getEnt();
                Intersect i(e,e1);
                //qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        double et = ((LinearEntity*)e)->gett(((PointE *)i.intersection(k))->getp());
                        double e1t = ((LinearEntity*)e1)->gett(((PointE *)i.intersection(k))->getp());

                        // no extentions allowed
                        if(et >= 0.0-db.getptoler() && et <= 1.0+db.getptoler() && e1t >= 0.0-db.getptoler() && e1t <= 1.0+db.getptoler())
                        {
                            PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());

                            IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                            eplist.at(l).addIntPointItem(ipitem);

                            ipitem = new IntersectPointItem(ip,e1,e);
                            eplist.at(j).addIntPointItem(ipitem);

                            //db.geometry.add(ip);
                            //ip->draw(DM_NORMAL);
                        }
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        eplist.at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        eplist.at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                }
            }
        }
    }

    elist = eplist;

    qDebug() << "debug";

#endif
#if 1
    // make intersection structures for all entities
    EntityList *tlist = db.geometry.getlist();
    Entity *e,*e1;

    for (tlist->start() ; (e = tlist->next()) != NULL ;)
    {
        if(state.excludelist.inlist(e))
            continue;
        if(e->type() == line_entity    ||
           e->type() == circle_entity  ||
           e->type() == ellipse_entity ||
           e->type() == curve_entity )
        {
            Point pp;
            Transform ident;
            ident.identity();
            e->setcopye(0);
            Entity *ecp = e->clone(ident);
            elist.push_back(PickEntItem(ecp,PickPointItem(ecp,pp),e));
            state.excludelist.add(ecp);
            //
            e->setcopye(0);
            ecp = e->clone(ident);
            //ecp->setcolour(5);
            //ecp->setweight(5);
            db.geometry.add(ecp);
            //ecp->setselected(1);
            elist.at(elist.size()-1).setTrimEnt(ecp);
            state.excludelist.add(ecp);
            //e->setselected(0);
        }
    }

    // get the intersection points
    if(elist.size() > 1)
    {
        for (uint l=0; l < elist.size(); l++)
        {
            for (uint j=l; j < elist.size(); j++)
            {
                if(elist.at(l).getOriginalEnt() == elist.at(j).getOriginalEnt())
                    continue;
                Entity *e1;
                e = elist.at(l).getEnt();
                e1 = elist.at(j).getEnt();
                Intersect i(e,e1);
                //qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        double et = ((LinearEntity*)e)->gett(((PointE *)i.intersection(k))->getp());
                        double e1t = ((LinearEntity*)e1)->gett(((PointE *)i.intersection(k))->getp());

                        // no extentions allowed
                        if(et >= 0.0-db.getptoler() && et <= 1.0+db.getptoler() && e1t >= 0.0-db.getptoler() && e1t <= 1.0+db.getptoler())
                        {
                            PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());

                            IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                            elist.at(l).addIntPointItem(ipitem);

                            ipitem = new IntersectPointItem(ip,e1,e);
                            elist.at(j).addIntPointItem(ipitem);

                            //db.geometry.add(ip);
                            //ip->draw(DM_NORMAL);
                        }
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        elist.at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        elist.at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                    /*
                    else
                    {
                        qDebug() << "oyoy??";
                    }
                    */
                    // no connection put up a message?
                }
            }
        }
    }
#endif
}

int getOriginalEntIndex(Entity *e, PickEntItemList &elist)
{
    int index=-1;
    uint i=0;
    for(i=0; i<elist.size(); i++)
    {
        if(elist.at(i).getOriginalEnt() == e)
        {
            return i;
        }
    }
    if(i == elist.size())
        return -1;
}

bool isConnectedSegment(Entity *e, PickEntItemList &elist)
{
    int found=0, index=-1;

    if((index = getOriginalEntIndex(e,elist)) >= 0)
    {
        for(uint i=0; i<elist.size(); i++)
        {
            if(elist.at(i).getOriginalEnt() == e)
             continue;

            for(uint j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                if(elist.at(i).mIntPointItems.at(j)->mEnt1 == elist.at(index).getEnt() ||
                   elist.at(i).mIntPointItems.at(j)->mEnt2 == elist.at(index).getEnt())
                {
                    found++;
                }
            }
        }
    }
    return found >= 2 ? true : false;
}

bool isInterUnique(sortableInterItem a, sortableInterItem b)
{
    if(a.mEnt == b.mEnt && a.mInteritem->mEnt1 == a.mEnt && b.mInteritem->mEnt1 == b.mEnt
       && a.mInteritem->t1 == b.mInteritem->t1)
    {
        return true;
    }
    else if(a.mEnt == b.mEnt && a.mInteritem->mEnt2 == a.mEnt && b.mInteritem->mEnt2 == b.mEnt
            && a.mInteritem->t2 == b.mInteritem->t2)
    {
        return true;
    }
    return false;
}

bool getClosestInters(PickEntItem pitem,double seed, Point &p0, Point &p1)
{
    double tmin = DBL_MAX,tmax = -DBL_MAX;

    std::vector<sortableInterItem> sorted;
    for(uint i=0; i < pitem.mIntPointItems.size(); i++)
    {
        if(pitem.mIntPointItems.at(i)->mEnt1 == pitem.getEnt())
        {
            sortableInterItem sitem(pitem.mIntPointItems.at(i),pitem.getEnt(),seed);
            sorted.push_back(sitem);
        }
        else if(pitem.mIntPointItems.at(i)->mEnt2 == pitem.getEnt())
        {
            sortableInterItem sitem(pitem.mIntPointItems.at(i),pitem.getEnt(),seed);
            sorted.push_back(sitem);
        }
    }

    std::sort(sorted.begin(),sorted.end(),sortInters);

    // remove duplicates
    std::unique(sorted.begin(),sorted.end(),isInterUnique);

    std::vector<sortableInterItem>::iterator bound;
    bound = std::stable_partition(sorted.begin(),sorted.end(),partitionInters);


    if(seed == 0.0)
    {
        p0 = bound->mInteritem->mPoint->getp();
        bound++;
        p1 = bound->mInteritem->mPoint->getp();
        return true;
    }
    else if(seed == 1.0)
    {
        bound--;
        p0 = bound->mInteritem->mPoint->getp();
        bound++;
        p1 = bound->mInteritem->mPoint->getp();
        return true;
    }
    else
    {
        if(bound == sorted.begin())
        {
            // todo: fix up
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            return true;
        }
        else if(bound == sorted.end())
        {
            // todo: fix up
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            return true;
        }
        else
        {
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            return true;
        }
    }
    return false;
}

bool getClosestInters(PickEntItem pitem,double seed, std::vector<Point> &pList)
{
    double tmin = DBL_MAX,tmax = -DBL_MAX;

    std::vector<sortableInterItem> sorted;
    for(uint i=0; i < pitem.mIntPointItems.size(); i++)
    {
        if(pitem.mIntPointItems.at(i)->mEnt1 == pitem.getEnt())
        {
            sortableInterItem sitem(pitem.mIntPointItems.at(i),pitem.getEnt(),seed);
            sorted.push_back(sitem);
        }
        else if(pitem.mIntPointItems.at(i)->mEnt2 == pitem.getEnt())
        {
            sortableInterItem sitem(pitem.mIntPointItems.at(i),pitem.getEnt(),seed);
            sorted.push_back(sitem);
        }
    }

    std::sort(sorted.begin(),sorted.end(),sortInters);

    // remove duplicates
    std::unique(sorted.begin(),sorted.end(),isInterUnique);

    std::vector<sortableInterItem>::iterator bound;
    bound = std::stable_partition(sorted.begin(),sorted.end(),partitionInters);


    Point p0,p1;
    if(seed == 0.0)
    {
        p0 = bound->mInteritem->mPoint->getp();
        bound++;
        p1 = bound->mInteritem->mPoint->getp();
        pList.push_back(p0);
        pList.push_back(p1);
        return true;
    }
    else if(seed == 1.0)
    {
        bound--;
        p0 = bound->mInteritem->mPoint->getp();
        bound++;
        p1 = bound->mInteritem->mPoint->getp();
        pList.push_back(p0);
        pList.push_back(p1);
        return true;
    }
    else
    {
        if(bound == sorted.begin())
        {
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            pList.push_back(p0);
            pList.push_back(p1);
            return true;
        }
        else if(bound == sorted.end())
        {
            bound--;
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            pList.push_back(p0);
            pList.push_back(p1);
            return true;
        }
        else if(bound == sorted.end()-1)
        {
            bound--;
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            pList.push_back(p0);
            pList.push_back(p1);
            return true;
        }
        else
        {
            bound--;
            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            pList.push_back(p0);
            pList.push_back(p1);

            p0 = bound->mInteritem->mPoint->getp();
            bound++;
            p1 = bound->mInteritem->mPoint->getp();
            pList.push_back(p0);
            pList.push_back(p1);
            return true;
        }
    }
    return false;
}

bool getMinMaxInters(PickEntItem pitem,Point &p0, Point &p1)
{
    double tmin = DBL_MAX,tmax = -DBL_MAX;

    for(uint i=0; i < pitem.mIntPointItems.size(); i++)
    {
        if(pitem.mIntPointItems.at(i)->mEnt1 == pitem.getEnt() && pitem.mIntPointItems.at(i)->t1 > tmax)
        {
            tmax = pitem.mIntPointItems.at(i)->t1;
        }
        if(pitem.mIntPointItems.at(i)->mEnt1 == pitem.getEnt() && pitem.mIntPointItems.at(i)->t1 < tmin)
        {
            tmin = pitem.mIntPointItems.at(i)->t1;
        }
        if(pitem.mIntPointItems.at(i)->mEnt2 == pitem.getEnt() && pitem.mIntPointItems.at(i)->t2 > tmax)
        {
            tmax = pitem.mIntPointItems.at(i)->t2;
        }
        if(pitem.mIntPointItems.at(i)->mEnt2 == pitem.getEnt() && pitem.mIntPointItems.at(i)->t2 < tmin)
        {
            tmin = pitem.mIntPointItems.at(i)->t2;
        }
    }
    p0 = ((LinearEntity*)pitem.getEnt())->position(tmin);
    p1 = ((LinearEntity*)pitem.getEnt())->position(tmax);
    return true;
}

int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}

class polynode
{
public:
    Point mPoint;
    PickEntItem mEntItem;
    double mAngle;
public:
    polynode()
    {
        mAngle = 0.0;
    }

    polynode(Point point, PickEntItem item, double angle=0.0)
    {
        mPoint = point;
        mEntItem = item;
        mAngle = angle;
    }

    polynode(const polynode &o)
    {
        mPoint = o.mPoint;
        mEntItem = o.mEntItem;
        mAngle = o.mAngle;
    }

    polynode& operator==(const polynode &o)
    {
        mPoint = o.mPoint;
        mEntItem = o.mEntItem;
        mAngle = o.mAngle;
        return *this;
    }

    ~polynode()
    {
        ;
    }
};
typedef std::vector<polynode> PolynodeList;

// A utility function to return square of distance between
// p1 and p2
int dist(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
           (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clockwise or counterclock wise
}

// A function used by std library function sort() to sort
//  an vector of points with respect to the first point
Point polynode0;
bool comparePnodes(polynode node1, polynode node2)
{
    Point p1 = node1.mPoint;
    Point p2 = node2.mPoint;

    // Find orientation
    int o = orientation(polynode0, p1, p2);
    if (o == 0)
      return (dist(polynode0, p2) >= dist(polynode0, p1))? false : true;

    return (o == 2)? false: true;
}

void getEdgesAroundPoint( Point cp, PickEntItemList &elist, PickEntItemList &joined)
{
    for(uint i=0; i < elist.size(); i++)
    {
        for(uint j=0; j < elist.at(i).mIntPointItems.size(); j++)
        {
            if(elist.at(i).mIntPointItems.at(j)->mEnt1 == elist.at(i).getEnt() &&
               elist.at(i).mIntPointItems.at(j)->mPoint->getp() == cp)
            {
                uint k=0;
                for(k=0; k < joined.size(); k++)
                {
                    if(joined.at(k).getEnt() == elist.at(i).getEnt())
                        break;
                }
                if(k == joined.size())
                    joined.push_back(elist.at(i));
            }
        }
    }
}

void testAngle()
{
    Point start1,end1;
    Point start2,end2;
    Point v1,v2;

    start1 = Point(0,0,0);
    end1 = Point(10,0,0);
    v1 = end1 - start1;

    start2 = Point(0,0,0);
    end2 = Point(10,0,0);
    v2 = end2 - start2;
    // angle = 0.0
    qDebug() << "angle = " << AngleBetween(v1,v2,Point(0,0,1.0));
    qDebug() << "-----";

    start2 = Point(0,0,0);
    end2 = Point(-10,0,0);
    v2 = end2 - start2;
    // angle = 3.14 (180)
    qDebug() << "angle = " << AngleBetween(v1,v2,Point(0,0,1.0));
    qDebug() << "-----";

    start2 = Point(0,0,0);
    end2 = Point(0,10,0);
    v2 = end2 - start2;
    // angle 1.5708 (90)
    qDebug() << "angle = " << AngleBetween(v1,v2,Point(0,0,1.0));
    qDebug() << "-----";

    start2 = Point(0,0,0);
    end2 = Point(0,-10,0);
    v2 = end2 - start2;
    // angle 4.71239 (270)

    qDebug() << "angle = " << AngleBetween(v1,v2,Point(0,0,1.0));
    qDebug() << "-----";
}

bool isInClosedLoop(View3dSurface *surface,int x,int y,Entity *entity, PickEntItemList &elist, PickEntItemList &loop)
{
     // look for entities connected to this one
#if 0
    // do prior to comming here
    // create segments
    // remove any unconnected or single connected segments
    // do after
    // get all segments linked to the first segment selected via the pick point ie. the input entity
    // make a list of data records which have the following data
    // boundary point
    // owning edge
    // polar angle from (from the lowest point(y)) in the list to the point
    // #loop1
    // find the lowest point in the list
    // find the polar angle to each point store it in the data
    // sort the points by increasing angle or closest if same angle
    // make trim edges for the edges between the found points
    // make loops and check if closed
    // if closed make a plane and check if it contains the pick point
    // return successful
    // if unsuccessful remove the points from the list and repeat #loop1

    Point zdir(0.0,0.0,1.0);
    Point op(0.0,0.0,0.0);
    op = surface->modeltoscreen(op);
    int index = getOriginalEntIndex(entity,elist);
    Entity *e = entity;

    if(state.excludelist.inlist(entity))
        return false;

    loop.push_back(elist.at(index));
    // get the start and end points
    Point opStart,opEnd,cp,lpStart,lpEnd;
    Point spPick = Point(x,int(surface->getheight()) - 1 - y,op.z);

    if(elist.at(index).mIntPointItems.size() > 2)
    {
        Point closest;
        double dist = DBL_MAX;
        for(uint j=0; j<elist.at(index).mIntPointItems.size(); j++)
        {
            if(elist.at(index).mIntPointItems.at(j)->mEnt1 == elist.at(index).getEnt() &&
               (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength() < dist)
            {
                Point p = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
                Point sp = surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp());
                dist = (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength();
                closest = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
            }
            else if(elist.at(index).mIntPointItems.at(j)->mEnt2 == elist.at(index).getEnt() &&
                    (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength() < dist)
            {
                Point p = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
                Point sp = surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp());
                dist = (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength();
                closest = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
            }
        }

        getClosestInters(elist.at(index),((LinearEntity*)elist.at(index).getEnt())->gett(closest),opStart,opEnd);
        double tclosest = ((LinearEntity*)elist.at(index).getEnt())->gett(closest);
        double tstart = ((LinearEntity*)elist.at(index).getEnt())->gett(opStart);
        double tend = ((LinearEntity*)elist.at(index).getEnt())->gett(opEnd);

        qDebug() << "tclosest: " << tclosest;
        qDebug() << "tstart: " << tstart;
        qDebug() << "tend: " << tend;
        qDebug() << "closest x: " << closest.x << ",y: " << closest.y;
        qDebug() << "start x: " << opStart.x << ",y: " << opStart.y;
        qDebug() << "end x: " << opEnd.x << ",y: " << opEnd.y;
        qDebug() << "====";

        if(!(closest == opStart) && !(closest == opEnd))
        {
            Point spClosest = surface->modeltoscreen(closest);
            Point spStart = surface->modeltoscreen(opStart);
            Point spEnd = surface->modeltoscreen(opEnd);

            if(fabs(spEnd.x-spStart.x) > fabs(spEnd.y-spStart.y))
            {
                if(spPick.x > spStart.x && spPick.x < spClosest.x ||
                   spPick.x < spStart.x && spPick.x > spClosest.x)
                    opEnd = closest;
                else if(spPick.x > spClosest.x && spPick.x > spEnd.x ||
                        spPick.x < spClosest.x && spPick.x < spEnd.x)
                    opStart = closest;
            }
            else
            {
                if(spPick.y < spStart.y && spPick.y > spClosest.y ||
                   spPick.y > spStart.y && spPick.y < spClosest.y)
                    opEnd = closest;
                else if(spPick.y < spClosest.y && spPick.y > spEnd.y ||
                        spPick.y > spClosest.y && spPick.y < spEnd.y)
                    opStart = closest;
            }
        }
    }
    else
        getMinMaxInters(elist.at(index),opStart,opEnd);

    lpStart = opStart;
    lpEnd = opEnd;
    cp=opEnd;
    //
    Point slpStart = surface->modeltoscreen(lpStart);
    Point slpEnd = surface->modeltoscreen(lpEnd);
    Point sldir = (slpEnd - slpStart).normalize();
    Point slpdir = (spPick - slpEnd).normalize();
    Point spndir = sldir.cross(slpdir);
    //

    // find all the segments connected from the first edge entity
    PickEntItemList egroup;

    // init with the first edge
    egroup.push_back(elist.at(index));

    for(uint ii=0; ii < elist.size(); ii++)
    {
        for(uint i=0; i < elist.size(); i++)
        {
            for(uint j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                for(uint k=0; k<egroup.size(); k++)
                {
                    for(uint m=0; m<egroup.at(k).mIntPointItems.size(); m++)
                    {
                        Point p1 = elist.at(i).mIntPointItems.at(j)->mPoint->getp();
                        Point p2 = egroup.at(k).mIntPointItems.at(m)->mPoint->getp();
                        double t1 =  egroup.at(k).mIntPointItems.at(m)->t1;
                        Point p3 = ((LinearEntity*)egroup.at(k).mIntPointItems.at(m)->mEnt1)->position(t1);
                        double t2 =  egroup.at(k).mIntPointItems.at(m)->t2;
                        if(t2 > t1)
                            p3 = ((LinearEntity*)egroup.at(k).mIntPointItems.at(m)->mEnt1)->position(t2);
                        t1 =  egroup.at(k).mIntPointItems.at(m)->t2;
                        Point p4 = ((LinearEntity*)egroup.at(k).mIntPointItems.at(m)->mEnt2)->position(t1);
                        t2 =  egroup.at(k).mIntPointItems.at(m)->t2;
                        if(t2 > t1)
                            p4 = ((LinearEntity*)egroup.at(k).mIntPointItems.at(m)->mEnt2)->position(t2);

                        qDebug() << "p1 x: " << p1.x << "y: " << p1.y;
                        qDebug() << "p2 x: " << p2.x << "y: " << p2.y;

                        if( p1 == p2 && (p1 == p3 || p1 == p4))
                        {
                            uint n=0;
                            for(n=0; n < egroup.size(); n++)
                            {
                                if(egroup.at(n).getOriginalEnt() == elist.at(i).getOriginalEnt())
                                    break;
                            }
                            if(n == egroup.size())
                            {
                                egroup.push_back(elist.at(i));
                            }
                        }
                    }
                }
            }
        }
    }

    //loop.clear();
    //loop = egroup;
    //return true;

    qDebug() << "step 1";

    // make a list of data records which have the following data
    // boundary point
    // owning edge
    // polar angle from (from the lowest point(y)) in the list to the point
    PolynodeList pnodelist;

    for(uint ii=0; ii < egroup.size(); ii++)
    {
        for(uint i=0; i < egroup.size(); i++)
        {
            for(uint j=0; j<egroup.at(i).mIntPointItems.size(); j++)
            {
                //Point p1 = elist.at(i).mIntPointItems.at(j)->mPoint->getp();
                Point p2 = egroup.at(i).mIntPointItems.at(j)->mPoint->getp();
                double t1 =  egroup.at(i).mIntPointItems.at(j)->t1;
                Point p3 = ((LinearEntity*)egroup.at(i).mIntPointItems.at(j)->mEnt1)->position(t1);
                double t2 =  egroup.at(i).mIntPointItems.at(j)->t2;
                if(t2 > t1)
                    p3 = ((LinearEntity*)egroup.at(i).mIntPointItems.at(j)->mEnt1)->position(t2);
                t1 =  egroup.at(i).mIntPointItems.at(j)->t2;
                Point p4 = ((LinearEntity*)egroup.at(i).mIntPointItems.at(j)->mEnt2)->position(t1);
                t2 =  egroup.at(i).mIntPointItems.at(j)->t2;
                if(t2 > t1)
                    p4 = ((LinearEntity*)egroup.at(i).mIntPointItems.at(j)->mEnt2)->position(t2);

                if(egroup.at(i).mIntPointItems.at(j)->mEnt1 == egroup.at(i).getEnt() && p2 == p3)
                {
                    polynode pnode(egroup.at(i).mIntPointItems.at(j)->mPoint->getp(),egroup.at(i));
                    uint k=0;
                    for(k=0; k < pnodelist.size(); k++)
                    {
                        if(pnodelist.at(k).mPoint == pnode.mPoint
                           /*pnodelist.at(k).mEntItem.getOriginalEnt() == pnode.mEntItem.getOriginalEnt()*/)
                            break;
                    }
                    if(k == pnodelist.size())
                    {
                        pnodelist.push_back(pnode);
                    }
                }
            }
        }
    }

    qDebug() << "step 2";
    // debug
    //loop.clear();
    //for (uint i = 0; i < pnodelist.size(); i++)
    //    loop.push_back(pnodelist.at(i).mEntItem);

    //return true;
    //

    // find the bottom-most point in the list
    double ymin = surface->modeltoscreen(pnodelist.at(0).mPoint).y;
    int min = 0;
    for (int i = 1; i < pnodelist.size(); i++)
    {
      double y = surface->modeltoscreen(pnodelist.at(i).mPoint).y;

      // Pick the bottom-most. In case of tie, chose the
      // left most point
      if ((y < ymin) || (ymin == y &&
          surface->modeltoscreen(pnodelist.at(i).mPoint).x < surface->modeltoscreen(pnodelist.at(min).mPoint).x))
         ymin = surface->modeltoscreen(pnodelist.at(i).mPoint).y, min = i;
    }

    polynode temp = pnodelist.at(min);
    pnodelist.at(min) = pnodelist.at(0);
    pnodelist.at(0) = temp;

    PointE *minpt = new PointE(pnodelist.at(0).mPoint);
    db.geometry.add(minpt,1);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    Point polynode0 = pnodelist.at(0).mPoint;
    std::sort(pnodelist.begin()+1,pnodelist.end() , comparePnodes);

    qDebug() << "step 3";

    loop.clear();
    for (uint i = 0; i < pnodelist.size(); i++)
        loop.push_back(pnodelist.at(i).mEntItem);

    //return true;

    // check all connections
    for(uint i=0; i<loop.size(); i++)
    {
        if(!isConnectedSegment(loop.at(i).getOriginalEnt(),loop))
            return false;
    }

    getTrimmedSegments(loop);

    return loop.size() > 2 ? true : false;

#endif
#if 1
    Point zdir(0.0,0.0,1.0);
    Point op(0.0,0.0,0.0);
    op = surface->modeltoscreen(op);
    int index = getOriginalEntIndex(entity,elist);
    Entity *e = entity;

    if(state.excludelist.inlist(entity))
        return false;

    //testAngle();

    loop.push_back(elist.at(index));
    // get the start and end points
    std::vector<Point> trimPoints;
    Point opStart,opEnd,cp,lpStart,lpEnd;
    Point spPick = Point(x,int(surface->getheight()) - 1 - y,op.z);

    if(elist.at(index).mIntPointItems.size() > 2)
    {
        Point closest;
        double dist = DBL_MAX;
        for(uint j=0; j<elist.at(index).mIntPointItems.size(); j++)
        {
            if(elist.at(index).mIntPointItems.at(j)->mEnt1 == elist.at(index).getEnt() &&
               (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength() < dist)
            {
                Point p = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
                Point sp = surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp());
                dist = (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength();
                closest = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
            }
            else if(elist.at(index).mIntPointItems.at(j)->mEnt2 == elist.at(index).getEnt() &&
                    (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength() < dist)
            {
                Point p = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
                Point sp = surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp());
                dist = (surface->modeltoscreen(elist.at(index).mIntPointItems.at(j)->mPoint->getp()) - spPick).GetLength();
                closest = elist.at(index).mIntPointItems.at(j)->mPoint->getp();
            }
        }

        std::vector<Point> pList;
        getClosestInters(elist.at(index),((LinearEntity*)elist.at(index).getEnt())->gett(closest),pList);
        //getClosestInters(elist.at(index),((LinearEntity*)elist.at(index).getEnt())->gett(closest),opStart,opEnd);

        for (uint m=0; m<pList.size(); m+=2)
        {
            Point topStart = pList.at(m);
            Point topEnd = pList.at(m+1);

            qDebug() << "closest x: " << closest.x << ",y: " << closest.y;
            qDebug() << "start x: " << opStart.x << ",y: " << opStart.y;
            qDebug() << "end x: " << opEnd.x << ",y: " << opEnd.y;
            qDebug() << "====";

            if(closest == topStart || closest == topEnd)
            {
                if(topStart.x > topEnd.x || topStart.y > topEnd.y)
                {
                    Point temp = topEnd;
                    topEnd = topStart;
                    topStart = temp;
                }

                Point spStart = surface->modeltoscreen(topStart);
                Point spEnd = surface->modeltoscreen(topEnd);

               if((spPick.x >= spStart.x && spPick.x <= spEnd.x) ||
                  (spPick.y >= spStart.y && spPick.y <= spEnd.y))
               {
                   opStart = pList.at(m);
                   opEnd = pList.at(m+1);
                   break;
               }
            }
        }
    }
    else
        getMinMaxInters(elist.at(index),opStart,opEnd);

    lpStart = opStart;
    lpEnd = opEnd;
    cp=opEnd;
    //
    Point slpStart = surface->modeltoscreen(lpStart);
    Point slpEnd = surface->modeltoscreen(lpEnd);

    Point sldir = (slpEnd - slpStart).normalize();
    Point slpdir = (spPick - slpStart).normalize();
    Point spndir = sldir.cross(slpdir);

    if(spndir.z > 0.0)
    {
        Point temp = lpStart;
        lpStart = lpEnd;
        lpEnd = temp;
        cp = lpEnd;
        opStart = lpStart;
        opEnd = lpEnd;
        slpStart = surface->modeltoscreen(lpStart);
        slpEnd = surface->modeltoscreen(lpEnd);
    }

#if 0
    if(slpStart.x > slpEnd.x || slpStart.y < slpEnd.y)
    {
        Point temp = lpStart;
        lpStart = lpEnd;
        lpEnd = temp;
        cp = lpEnd;
        opStart = lpStart;
        opEnd = lpEnd;
        slpStart = surface->modeltoscreen(lpStart);
        slpEnd = surface->modeltoscreen(lpEnd);
    }
    else if(slpStart.x < slpEnd.x || slpStart.y > slpEnd.y)
    {
        Point temp = lpStart;
        lpStart = lpEnd;
        lpEnd = temp;
        cp = lpEnd;
        opStart = lpStart;
        opEnd = lpEnd;
        slpStart = surface->modeltoscreen(lpStart);
        slpEnd = surface->modeltoscreen(lpEnd);
    }
#endif
    //

    trimPoints.push_back(opStart);
    trimPoints.push_back(opEnd);

    int ii=0;
    for(ii=0; ii<elist.size()*2; ii++)
    {
        for(int i=0; i<elist.size(); i++)
        {
            if(state.excludelist.inlist(e))
                continue;

            int index = getOriginalEntIndex(e,elist);

            if(i==index)
                continue;

            for(uint j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                if(elist.at(i).mIntPointItems.at(j)->mEnt1 == elist.at(index).getEnt() &&
                   elist.at(i).mIntPointItems.at(j)->mPoint->getp() == cp)
                {
                    // use ent1 for the next entity
                    Entity *te = elist.at(i).getOriginalEnt();
                    //Entity *te = elist.at(i).getEnt();
                    // find the next point
                    Point pStart, pEnd;
                    PickEntItemList joined;
                    int nextEdge=-1;
                    if(elist.at(i).mIntPointItems.size() > 2)
                    {
                        // get all the edges joining at this point
                        joined.clear();
                        getEdgesAroundPoint(cp, elist, joined);
                        // get the edge with the smallest angle between the current edge
                        Point tpStart,tpEnd;
                        double angle = DBL_MAX;
                        nextEdge = -1;
                        for(uint k=0; k < joined.size(); k++)
                        {
                            std::vector<Point> pList;
                            if(joined.at(k).getEnt() == elist.at(index).getEnt() && joined.at(k).mIntPointItems.size() < 3)
                                continue;
                            if(joined.at(k).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pList);
                                //getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pStart,pEnd);
                            else
                            {
                                getMinMaxInters(joined.at(k),tpStart,tpEnd);
                                pList.push_back(tpStart);
                                pList.push_back(tpEnd);
                            }

                            for (uint m=0; m<pList.size(); m+=2)
                            {
                                tpStart = pList.at(m);
                                tpEnd   = pList.at(m+1);

                                if((tpStart == lpStart && tpEnd == lpEnd) || (tpStart == lpEnd && tpEnd == lpStart))
                                    continue;
                                //
                                Point esdir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpStart));
                                Point eedir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpEnd));
                                //
                                slpStart = surface->modeltoscreen(lpStart);
                                slpEnd = surface->modeltoscreen(lpEnd);
                                Point spStart = surface->modeltoscreen(tpStart);
                                Point spEnd = surface->modeltoscreen(tpEnd);
                                //
                                Point cdir  = (slpEnd - slpStart).normalize();
                                if(lpEnd == cp)
                                    cdir = (slpStart - slpEnd).normalize();
                                Point tdir = (spEnd - spStart).normalize();
                                if(tpEnd == cp)
                                    tdir = (spStart - spEnd).normalize();
                                qDebug() << "normal : " << cdir.cross(tdir).z;
                                double tangle = AngleBetween(cdir,tdir,Point(0.0,0.0,1.0));
                                if(tangle < angle)
                                {
                                    angle = tangle;
                                    nextEdge = k;
                                    pStart = tpStart;
                                    pEnd = tpEnd;
                                }
                            }
                        }

                        if(nextEdge >= 0)
                        {
#if 0
                            if(joined.at(nextEdge).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(nextEdge),((LinearEntity*)joined.at(nextEdge).getEnt())->gett(cp),pStart,pEnd);
                            else
                                getMinMaxInters(joined.at(nextEdge),pStart,pEnd);
#endif
                            te = joined.at(nextEdge).getOriginalEnt();
                            //te = joined.at(nextEdge).getEnt();

                            if(pEnd == cp)
                            {
                                Point temp = pStart;
                                lpStart = pEnd;
                                lpEnd = temp;
                            }
                            else
                            {
                                lpStart = pStart;
                                lpEnd = pEnd;
                            }

                            cp = lpEnd;                            
                        }
#if 0
                        getClosestInters(elist.at(i),((LinearEntity*)elist.at(i).getEnt())->gett(cp),pStart,pEnd);
                        //
                        slpStart = surface->modeltoscreen(lpStart);
                        slpEnd = surface->modeltoscreen(lpEnd);
                        Point spStart = surface->modeltoscreen(pStart);
                        Point spEnd = surface->modeltoscreen(pEnd);
                        //
                        sldir = (spEnd - spStart).normalize();
                        slpdir = (spPick - spEnd).normalize();
                        Point sndir = sldir.cross(slpdir);
                        if(signbit(sndir.z) != signbit(spndir.z))
                        {
                            Point temp = pStart;
                            pStart = pEnd;
                            pEnd = temp;
                        }
                        else
                        {
                            lpStart = pStart;
                            lpEnd = pEnd;
                        }

                        cp = lpEnd;

#endif

                        //
                    }
                    else
                    {
                        // get all the edges joining at this point
                        joined.clear();
                        getEdgesAroundPoint(cp, elist, joined);
                        // get the edge with the smallest angle between the current edge
                        Point tpStart,tpEnd;
                        double angle = DBL_MAX;
                        nextEdge = -1;
                        for(uint k=0; k < joined.size(); k++)
                        {
                            std::vector<Point> pList;
                            if(joined.at(k).getEnt() == elist.at(index).getEnt() && joined.at(k).mIntPointItems.size() < 3)
                                continue;
                            if(joined.at(k).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pList);
                                //getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pStart,pEnd);
                            else
                            {
                                getMinMaxInters(joined.at(k),tpStart,tpEnd);
                                pList.push_back(tpStart);
                                pList.push_back(tpEnd);
                            }

                            for (uint m=0; m<pList.size(); m+=2)
                            {
                                tpStart = pList.at(m);
                                tpEnd   = pList.at(m+1);

                                if((tpStart == lpStart && tpEnd == lpEnd) || (tpStart == lpEnd && tpEnd == lpStart))
                                    continue;
                                //
                                Point esdir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpStart));
                                Point eedir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpEnd));
                                //
                                slpStart = surface->modeltoscreen(lpStart);
                                slpEnd = surface->modeltoscreen(lpEnd);
                                Point spStart = surface->modeltoscreen(tpStart);
                                Point spEnd = surface->modeltoscreen(tpEnd);
                                //
                                Point cdir  = (slpEnd - slpStart).normalize();
                                if(lpEnd == cp)
                                    cdir = (slpStart - slpEnd).normalize();
                                Point tdir = (spEnd - spStart).normalize();
                                if(tpEnd == cp)
                                    tdir = (spStart - spEnd).normalize();
                                qDebug() << "normal : " << cdir.cross(tdir).z;
                                double tangle = AngleBetween(cdir,tdir,Point(0.0,0.0,1.0));
                                if(tangle < angle)
                                {
                                    angle = tangle;
                                    nextEdge = k;
                                    pStart = tpStart;
                                    pEnd = tpEnd;
                                }
                            }
                        }

                        if(nextEdge >= 0)
                        {
#if 0
                            if(joined.at(nextEdge).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(nextEdge),((LinearEntity*)joined.at(nextEdge).getEnt())->gett(cp),pStart,pEnd);
                            else
                                getMinMaxInters(joined.at(nextEdge),pStart,pEnd);
#endif

                            te = joined.at(nextEdge).getOriginalEnt();
                            //te = joined.at(nextEdge).getEnt();

                            if(pEnd == cp)
                            {
                                Point temp = pStart;
                                lpStart = pEnd;
                                lpEnd = temp;
                            }
                            else
                            {
                                lpStart = pStart;
                                lpEnd = pEnd;
                            }

                            cp = lpEnd;
                        }
#if 0
                        getMinMaxInters(elist.at(i),pStart,pEnd);
                        //
                        if(pStart == cp)
                        {
                            cp = pEnd;
                            lpStart = pStart;
                            lpEnd = pEnd;
                        }
                        else
                        {
                            cp = pStart;
                            lpStart = pEnd;
                            lpEnd = pStart;
                        }
#endif
                    }
                    uint k=0;
                    for(k=0; k <loop.size(); k++)
                    {
                        if(loop.at(k).getOriginalEnt() == te &&
                           trimPoints.at(k*2) == lpStart &&
                           trimPoints.at(k*2+1) == lpEnd)
                            break;
                    }
                    if(k == loop.size())
                    {
                        e = te;;
                        loop.push_back(joined.at(nextEdge));
                        trimPoints.push_back(lpStart);
                        trimPoints.push_back(lpEnd);
                    }
                }
                else if(elist.at(i).mIntPointItems.at(j)->mEnt2 == elist.at(index).getEnt() &&
                        elist.at(i).mIntPointItems.at(j)->mPoint->getp() == cp)
                {
                    // get all the edges joining at this point
                    Entity *te = elist.at(i).getOriginalEnt();
                    //Entity *te = elist.at(i).getEnt();
                    Point pStart, pEnd;
                    PickEntItemList joined;
                    int nextEdge=-1;
                    // find the next point
                    if(elist.at(i).mIntPointItems.size() > 2)
                    {
                        joined.clear();
                        getEdgesAroundPoint(cp, elist, joined);
                        // get the edge with the smallest angle between the current edge
                        Point tpStart,tpEnd;
                        double angle = DBL_MAX;
                        nextEdge = -1;
                        for(uint k=0; k < joined.size(); k++)
                        {
                            std::vector<Point> pList;
                            if(joined.at(k).getEnt() == elist.at(index).getEnt() && joined.at(k).mIntPointItems.size() < 3)
                                continue;
                            if(joined.at(k).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pList);
                                //getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pStart,pEnd);
                            else
                            {
                                getMinMaxInters(joined.at(k),tpStart,tpEnd);
                                pList.push_back(tpStart);
                                pList.push_back(tpEnd);
                            }

                            for (uint m=0; m<pList.size(); m+=2)
                            {
                                tpStart = pList.at(m);
                                tpEnd   = pList.at(m+1);

                                if((tpStart == lpStart && tpEnd == lpEnd) || (tpStart == lpEnd && tpEnd == lpStart))
                                    continue;
                                //
                                Point esdir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpStart)+0.1);
                                Point eedir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpEnd)-0.1);
                                //
                                slpStart = surface->modeltoscreen(lpStart);
                                slpEnd = surface->modeltoscreen(lpEnd);
                                Point spStart = surface->modeltoscreen(tpStart);
                                Point spEnd = surface->modeltoscreen(tpEnd);
                                //
                                Point cdir  = (slpEnd - slpStart).normalize();
                                if(lpEnd == cp)
                                    cdir = (slpStart - slpEnd ).normalize();
                                Point tdir = (spEnd - spStart).normalize();
                                if(tpEnd == cp)
                                    tdir = (spStart - spEnd).normalize();
                                qDebug() << "normal : " << cdir.cross(tdir).z;
                                double tangle = AngleBetween(cdir,tdir,Point(0.0,0.0,1.0));
                                if(tangle < angle)
                                {
                                    angle = tangle;
                                    nextEdge = k;
                                    pStart = tpStart;
                                    pEnd = tpEnd;
                                }
                            }
                        }

                        if(nextEdge >= 0)
                        {
#if 0
                            if(joined.at(nextEdge).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(nextEdge),((LinearEntity*)joined.at(nextEdge).getEnt())->gett(cp),pStart,pEnd);
                            else
                                getMinMaxInters(joined.at(nextEdge),pStart,pEnd);
#endif
                            te = joined.at(nextEdge).getOriginalEnt();
                            //te = joined.at(nextEdge).getEnt();

                            if(pEnd == cp)
                            {
                                Point temp = pStart;
                                lpStart = pEnd;
                                lpEnd = temp;
                            }
                            else
                            {
                                lpStart = pStart;
                                lpEnd = pEnd;
                            }

                            cp = lpEnd;
                        }
#if 0
                    // use ent2 for the next entity
                    Entity *te = elist.at(i).getOriginalEnt();
                    // find the next point
                    Point pStart, pEnd;
                    if(elist.at(i).mIntPointItems.size() > 2)
                    {
                        getClosestInters(elist.at(i),((LinearEntity*)elist.at(i).getEnt())->gett(cp),pStart,pEnd);
                        //
                        slpStart = surface->modeltoscreen(lpStart);
                        slpEnd = surface->modeltoscreen(lpEnd);
                        Point spStart = surface->modeltoscreen(pStart);
                        Point spEnd = surface->modeltoscreen(pEnd);
                        //
                        sldir = (spEnd - spStart).normalize();
                        slpdir = (spPick - spEnd).normalize();
                        Point sndir = sldir.cross(slpdir);
                        if(signbit(sndir.z) != signbit(spndir.z))
                        {
                            Point temp = pStart;
                            lpStart = pEnd;
                            lpEnd = temp;
                        }
                        else
                        {
                            lpStart = pStart;
                            lpEnd = pEnd;
                        }
                        cp = lpEnd;
                        //
#endif
                    }
                    else
                    {
                        // get all the edges joining at this point
                        joined.clear();
                        getEdgesAroundPoint(cp, elist, joined);
                        // get the edge with the smallest angle between the current edge
                        Point tpStart,tpEnd;
                        double angle = DBL_MAX;
                        nextEdge = -1;
                        for(uint k=0; k < joined.size(); k++)
                        {
                            std::vector<Point> pList;
                            if(joined.at(k).getEnt() == elist.at(index).getEnt() && joined.at(k).mIntPointItems.size() < 3)
                                continue;
                            if(joined.at(k).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pList);
                                //getClosestInters(joined.at(k),((LinearEntity*)joined.at(k).getEnt())->gett(cp),pStart,pEnd);
                            else
                            {
                                getMinMaxInters(joined.at(k),tpStart,tpEnd);
                                pList.push_back(tpStart);
                                pList.push_back(tpEnd);
                            }

                            for (uint m=0; m<pList.size(); m+=2)
                            {
                                tpStart = pList.at(m);
                                tpEnd   = pList.at(m+1);

                                if((tpStart == lpStart && tpEnd == lpEnd) || (tpStart == lpEnd && tpEnd == lpStart))
                                    continue;
                                //
                                Point esdir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpStart));
                                Point eedir = ((LinearEntity*)joined.at(k).getEnt())->direction(((LinearEntity*)joined.at(k).getEnt())->gett(tpEnd));
                                //
                                slpStart = surface->modeltoscreen(lpStart);
                                slpEnd = surface->modeltoscreen(lpEnd);
                                Point spStart = surface->modeltoscreen(tpStart);
                                Point spEnd = surface->modeltoscreen(tpEnd);
                                //
                                Point cdir  = (slpEnd - slpStart).normalize();
                                if(lpEnd == cp)
                                    cdir = (slpStart -slpEnd).normalize();
                                Point tdir = (spEnd - spStart).normalize();
                                if(tpEnd == cp)
                                    tdir = (spStart - spEnd).normalize();
                                qDebug() << "normal : " << cdir.cross(tdir).z;
                                double tangle = AngleBetween(cdir,tdir,Point(0.0,0.0,1.0));
                                if(tangle < angle)
                                {
                                    angle = tangle;
                                    nextEdge = k;
                                    pStart = tpStart;
                                    pEnd = tpEnd;
                                }
                            }
                        }

                        if(nextEdge >= 0)
                        {
#if 0
                            if(joined.at(nextEdge).mIntPointItems.size() > 2)
                                getClosestInters(joined.at(nextEdge),((LinearEntity*)joined.at(nextEdge).getEnt())->gett(cp),pStart,pEnd);
                            else
                                getMinMaxInters(joined.at(nextEdge),pStart,pEnd);
#endif
                            te = joined.at(nextEdge).getOriginalEnt();
                            //te = joined.at(nextEdge).getEnt();

                            if(pEnd == cp)
                            {
                                Point temp = pStart;
                                lpStart = pEnd;
                                lpEnd = temp;
                            }
                            else
                            {
                                lpStart = pStart;
                                lpEnd = pEnd;
                            }

                            cp = lpEnd;
                        }
#if 0
                        getMinMaxInters(elist.at(i),pStart,pEnd);
                        //
                        //
                        if(pStart == cp)
                        {
                            cp = pEnd;
                            lpStart = pStart;
                            lpEnd = pEnd;
                        }
                        else
                        {
                            cp = pStart;
                            lpStart = pEnd;
                            lpEnd = pStart;
                        }
#endif
                    }
                    uint k=0;
                    for(k=0; k <loop.size(); k++)
                    {
                        if(loop.at(k).getOriginalEnt() == te &&
                           trimPoints.at(k*2) == lpStart &&
                           trimPoints.at(k*2+1) == lpEnd)
                            break;
                    }
                    if(k == loop.size())
                    {
                        e = te;
                        loop.push_back(joined.at(nextEdge));
                        trimPoints.push_back(lpStart);
                        trimPoints.push_back(lpEnd);
                    }
                }
            }
            if(cp == opStart)
                break;
        }
        if(cp == opStart)
            break;
    }

    // check all connections
#if 0
    for(uint i=0; i<loop.size(); i++)
    {
        if(!isConnectedSegment(loop.at(i).getOriginalEnt(),loop))
            return false;
    }
#endif

    getTrimmedSegments(loop,trimPoints);

    return loop.size() > 2 ? true : false;

#endif
}

void removeInterFromEdge(PickEntItem &item, PickEntItemList &elist)
{
    for(int i=0; i<elist.size(); i++)
    {
        if(item.getEnt() == elist.at(i).getEnt())
            continue;
        for(int j=0; j<elist.at(i).mIntPointItems.size(); j++)
        {
            if(elist.at(i).mIntPointItems.at(j)->mEnt1 == item.getEnt())
                elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin()+j);
            if(elist.at(i).mIntPointItems.at(j)->mEnt2 == item.getEnt())
                elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin()+j);
        }
    }
}

void purgeUnconnectedNodes(PickEntItemList &elist)
{
    // removes edges with point nodes with no other connections

    for(int i=0; i<elist.size(); i++)
    {
        Point p;
        int count = 0;
        for(int j=0; j<elist.at(i).mIntPointItems.size(); j++)
        {
            if(j==0)
            {
                p = elist.at(i).mIntPointItems.at(j)->mPoint->getp();
                count = 1;
            }
            else
            {
                if(p == elist.at(i).mIntPointItems.at(j)->mPoint->getp())
                    count++;
            }
        }
        if(count == 0 || count == elist.at(i).mIntPointItems.size())
        {
            state.excludelist.add(elist.at(i).getOriginalEnt());
            removeInterFromEdge(elist.at(i),elist);
            elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin(),elist.at(i).mIntPointItems.end());
            db.geometry.del(elist.at(i).getEnt());
            elist.at(i).mPickEnt = nullptr;
            elist.at(i).setTrimEnt(nullptr);
            elist.erase(elist.begin()+i);
            i=-1;
            continue;
        }
    }
}

void purgeUnconnectedEdges(PickEntItemList &elist)
{
    // removes edges with less than two connections

    for(int i=0; i<elist.size(); i++)
    {
        if(elist.at(i).mIntPointItems.size() < 2)
        {
            state.excludelist.add(elist.at(i).getOriginalEnt());
            for(int j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                if(elist.at(i).mIntPointItems.at(j)->mEnt1 == elist.at(i).getEnt())
                {
                    removeInterFromEdge(elist.at(i),elist);
                    elist.at(i).mIntPointItems.at(j)->mEnt1 = nullptr;
                    elist.at(i).mIntPointItems.at(j)->mEnt2 = nullptr;
                }
            }
            elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin(),elist.at(i).mIntPointItems.end());
            db.geometry.del(elist.at(i).getEnt());
            elist.at(i).mPickEnt = nullptr;
            elist.at(i).setTrimEnt(nullptr);
            elist.erase(elist.begin()+i);
            i=-1;
            continue;
        }
    }
}

void getClosestLoop(View3dSurface *surface,int x,int y, PickEntItemList &elist, PickEntItemList &loop)
{
    Entity *entity=0,*e=0;
    Point start,end,uv1,uv2;
    double side;
    bool found=false;

    purgeUnconnectedNodes(elist);
    purgeUnconnectedEdges(elist);
#if 0
    // purge any unconnected or single connected segments
    for(int i=0; i<elist.size(); i++)
    {
        if(elist.at(i).mIntPointItems.size() > 1)
        {
            for(int j=0; j<elist.at(i).mIntPointItems.size(); j++)
            {
                if(elist.at(i).mIntPointItems.at(j)->mEnt1 == nullptr ||
                   elist.at(i).mIntPointItems.at(j)->mEnt2 == nullptr)
                {
                    state.excludelist.add(elist.at(i).getOriginalEnt());
                    for(int k=0; k<elist.size(); k++)
                    {
                        for(int L=0; L<elist.at(k).mIntPointItems.size(); L++)
                        {
                            if(elist.at(k).mIntPointItems.at(L)->mEnt1 == elist.at(i).getEnt())
                                elist.at(k).mIntPointItems.at(L)->mEnt1 = nullptr;
                            if(elist.at(k).mIntPointItems.at(L)->mEnt2 == elist.at(i).getEnt())
                                elist.at(k).mIntPointItems.at(L)->mEnt2 = nullptr;
                        }
                    }
                    elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin(),elist.at(i).mIntPointItems.end());
                    db.geometry.del(elist.at(i).getEnt());
                    elist.at(i).mPickEnt = nullptr;
                    elist.at(i).setTrimEnt(nullptr);
                    elist.erase(elist.begin()+i);
                    i=-1;
                    break;
                }
            }
        }
        else
        {
            state.excludelist.add(elist.at(i).getOriginalEnt());
            for(int k=0; k<elist.size(); k++)
            {
                for(int j=0; j<elist.at(k).mIntPointItems.size(); j++)
                {
                    if(elist.at(k).mIntPointItems.at(j)->mEnt1 == elist.at(i).getEnt())
                    {
                        elist.at(k).mIntPointItems.at(j)->mEnt1 = nullptr;
                        elist.at(k).mIntPointItems.at(j)->mEnt2 = nullptr;
                    }
                    //if(elist.at(k).mIntPointItems.at(j)->mEnt2 == elist.at(i).getEnt())
                }
            }
            elist.at(i).mIntPointItems.erase(elist.at(i).mIntPointItems.begin(),elist.at(i).mIntPointItems.end());
            db.geometry.del(elist.at(i).getEnt());
            elist.at(i).mPickEnt = nullptr;
            elist.at(i).setTrimEnt(nullptr);
            elist.erase(elist.begin()+i);
            i=-1;
            continue;
        }
    }
#endif
#if 1
    while(!found)
    {
        if((entity = db.geometry.pickhor(surface,x,y,&start,&end,&uv1,&uv2,&side)) != 0)
        {
            e = entity;
            if((found = isInClosedLoop(surface,x,y,e,elist,loop)) == false)
            {
                //state.excludelist.add(entity);
                loop.clear();
            }
        }
    }
#endif
    qDebug() << "found? " << found;
}

/**
 * @brief plane_command3
 * @param cstate
 * @param e
 *
 * Entry point for Insert Plane by Interior Point
 *
 * KMJ : Working in Qt version
 *
 */
void plane_command3(int *cstate,HighLevelEvent *e,void **)
{
#if 0
    // new method does not work given up on it
 Point p;
 Entity *entity;
 double xspacing,yspacing,angle,crossangle,transparency,side,red,green,blue,transparencyrange;
 Point start,end,uv1,uv2;
 Point origin,xaxis,yaxis;
 BitMask options(32);
 ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;
 
  if (*cstate == InitialState && ((NewCommandEvent *)e)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(409,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {case InitialState :
      state.excludelist.destroy();
      state.excludelist.start();
       *cstate = 1;
       break;
     case 1 :
       if (e->isa(Coordinateuv))
         {  p = ((CoordinateuvEvent *)e)->coordinate();
            if (((CoordinateuvEvent *)e)->window() != 0)
              {
                 p = ((CoordinateuvEvent *)e)->window()->uvtoscreen(p);

                 qDebug() << "DB entity count before: " << db.geometry.getlist()->length();

                 db.saveundo(UD_STARTBLOCK,NULL);
                 // find loops made of intersecton entities
                 PickEntItemList elist,loop;
                 //elist.push_back(PickEntItemList());
                 getIntersectingLoops(elist);

                 //p = ((CoordinateuvEvent *)e)->window()->uvtoscreen(p);
                 state.selmask.entity.clearandset(linear_mask,end_list);
                 getClosestLoop(((CoordinateuvEvent *)e)->window(),(int)p.x,(int)p.y,elist,loop);

                 //std::reverse(loop.begin(),loop.end());
                 EntityList bounds;
                 for(uint i=0; i<loop.size(); i++)
                     bounds.add(loop.at(i).getTrimEnt());

                 Loops loops(1);
                 loops.create(&bounds);

                     if (! loops.getloops().empty())
                       {
                         /*
                         EntityList tloops,ploop;
                         tloops = loops.getloops();
                         for (tloops.start();  ! tloops.atend() ; )
                         {
                             Entity *e = tloops.next();
                             if (e != NULL)
                                 ploop.add(e);
                             else
                                break;
                         }
                         */

                          Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                          options.set(0);
                          Plane *plane = new Plane(loops.getloops().copy(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                          if (! plane->getdefined() || plane->pick(((CoordinateuvEvent *)e)->window(),(int)p.x,int(((CoordinateuvEvent *)e)->window()->getheight()-p.y-1.0),0) < 0.0)
                            {  cadwindow->MessageBox("The point does not lie inside the plane","Insert plane ",MB_ICONSTOP);
                               delete plane;

                              // clean up
                              state.excludelist.destroy();
                              // clear the intersections
                              for (uint j=0; j < elist.size(); j++)
                                  elist.at(j).mIntPointItems.erase(elist.at(j).mIntPointItems.begin(),elist.at(j).mIntPointItems.end());

                              // clear the other entities
                              for (uint j=0; j < elist.size(); j++)
                              {
                                  db.geometry.del(elist.at(j).getEnt());
                                  elist.at(j).mPickEnt = nullptr;
                                  elist.at(j).setTrimEnt(nullptr);
                              }

                              elist.clear();
                              loop.clear();
                              db.saveundo(UD_ENDBLOCK,NULL);
                            }
                          else
                            {  delete plane;
                               Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                               Plane *plane = new Plane(loops.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                               if (plane->getdefined())
                                 {Entity *e;
                                  EntityList list;
                                  EntityHeader cheader;

                                    db.geometry.add(plane);
                                    //db.geometry.tofront(plane);
                                    db.geometry.toback(plane);
                                    state.excludelist.destroy();
                                    list = plane->getlist();
                                    for (list.start() ; ! list.atend() ; )
                                    {
                                        if ((e = list.next()) != 0)
                                        {
                                            //db.geometry.tofront(e);
                                            e->setcolour(cheader.getcolour());
                                            e->setweight(cheader.getweight());
                                            e->draw(DM_NORMAL);
                                            state.excludelist.add(e);
                                        }
                                    }

                                    qDebug() << "DB entity count after create: " << db.geometry.getlist()->length();

                                    // clean up
                                    // clear the intersections
                                    for (uint j=0; j < elist.size(); j++)
                                        elist.at(j).mIntPointItems.erase(elist.at(j).mIntPointItems.begin(),elist.at(j).mIntPointItems.end());

                                    // clear the other entities
                                    for (uint j=0; j < elist.size(); j++)
                                    {
                                        if(elist.at(j).getEnt() != nullptr)
                                            delete elist.at(j).getEnt();
                                        elist.at(j).mPickEnt = nullptr;
                                        if(!state.excludelist.inlist(elist.at(j).getTrimEnt()))
                                            elist.at(j).setTrimEnt(nullptr);
                                    }
                                    state.excludelist.destroy();
                                    elist.clear();
                                    loop.clear();
                                    qDebug() << "DB entity count after clear: " << db.geometry.getlist()->length();

                                    db.saveundo(UD_ENDBLOCK,NULL);

                                 }
                               else
                                 {  cadwindow->MessageBox(rs3.gets(),rs4.gets(),MB_ICONINFORMATION);
                                    delete plane;

                                   // clean up
                                   state.excludelist.destroy();
                                   // clear the intersections
                                   for (uint j=0; j < elist.size(); j++)
                                       elist.at(j).mIntPointItems.erase(elist.at(j).mIntPointItems.begin(),elist.at(j).mIntPointItems.end());

                                   // clear the other entities
                                   for (uint j=0; j < elist.size(); j++)
                                   {
                                       db.geometry.del(elist.at(j).getEnt());
                                       elist.at(j).mPickEnt = nullptr;
                                       elist.at(j).setTrimEnt(nullptr);
                                   }

                                   elist.clear();
                                   loop.clear();
                                   db.saveundo(UD_ENDBLOCK,NULL);

                                 }
                            }
                       }
                     else
                     {
                       cadwindow->MessageBox("A plane could not be traced.","Insert Plane by interior point",MB_ICONSTOP);

                       qDebug() << "DB entity count after error: " << db.geometry.getlist()->length();

                       // clean up
                       state.excludelist.destroy();
                       // clear the intersections
                       for (uint j=0; j < elist.size(); j++)
                           elist.at(j).mIntPointItems.erase(elist.at(j).mIntPointItems.begin(),elist.at(j).mIntPointItems.end());

                       // clear the other entities
                       for (uint j=0; j < elist.size(); j++)
                       {
                           db.geometry.del(elist.at(j).getEnt());
                           elist.at(j).mPickEnt = nullptr;
                           elist.at(j).setTrimEnt(nullptr);
                       }

                       elist.clear();
                       loop.clear();
                       qDebug() << "DB entity count after clear: " << db.geometry.getlist()->length();
                       db.saveundo(UD_ENDBLOCK,NULL);
                     }
              }
         }
       else if (e->isa(Abort) || e->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the interior of the polygon to fill");
    }
#else
  // original method
  Point p;
  Entity *entity;
  double xspacing,yspacing,angle,crossangle,transparency,side,red,green,blue,transparencyrange;
  Point start,end,uv1,uv2;
  Point origin,xaxis,yaxis;
  BitMask options(32);
  ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
  RCCHAR patternfilename[300];
  RCCOLORREF gradientcolour;

   if (*cstate == InitialState && ((NewCommandEvent *)e)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
     {  ::state.sendevent(new NewCommandEvent(409,0));
        *cstate = ExitState;
        return;
     }

   switch (*cstate)
     {case InitialState :
        *cstate = 1;
        break;
      case 1 :
        if (e->isa(Coordinateuv))
          {  p = ((CoordinateuvEvent *)e)->coordinate();
             if (((CoordinateuvEvent *)e)->window() != 0)
               {  p = ((CoordinateuvEvent *)e)->window()->uvtoscreen(p);
                  state.selmask.entity.clearandset(linear_mask,end_list);
                  if ((entity = db.geometry.pickhor(((CoordinateuvEvent *)e)->window(),(int)p.x,(int)p.y,&start,&end,&uv1,&uv2,&side)) != 0)
                    {Loops loops(1);
                      loops.trace(((CoordinateuvEvent *)e)->window(),(LinearEntity *)entity,start,end,uv1,uv2,side);
                      if (! loops.getloops().empty())
                        {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                           options.set(0);
                           Plane *plane = new Plane(loops.getloops().copy(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                           if (! plane->getdefined() || plane->pick(((CoordinateuvEvent *)e)->window(),(int)p.x,int(((CoordinateuvEvent *)e)->window()->getheight()-p.y-1.0),0) < 0.0)
                             {  cadwindow->MessageBox("The point does not lie inside the plane","Insert plane ",MB_ICONSTOP);
                                delete plane;
                             }
                           else
                             {  delete plane;
                                Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                                Plane *plane = new Plane(loops.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                                if (plane->getdefined())
                                  {Entity *e;
                                   EntityList list;
                                     db.geometry.add(plane);
                                     //db.geometry.tofront(plane);
                                     db.geometry.toback(plane);
                                     list = plane->getlist();
                                     for (list.start() ; ! list.atend() ; )
                                     {
                                         if ((e = list.next()) != 0)
                                         {
                                             //db.geometry.tofront(e);
                                             e->draw(DM_NORMAL);
                                         }
                                     }
                                  }
                                else
                                  {  cadwindow->MessageBox(rs3.gets(),rs4.gets(),MB_ICONINFORMATION);
                                     delete plane;
                                  }
                             }
                        }
                      else
                        cadwindow->MessageBox("A plane could not be traced.","Insert Plane by interior point",MB_ICONSTOP);
                    }
               }
          }
        else if (e->isa(Abort) || e->isa(Exit))
          *cstate = ExitState;
        break;
     }
   if (*cstate == 1)
     {  state.selmask.entity.clearandset(uv_mask,end_list);
        cadwindow->prompt->normalprompt("Locate the interior of the polygon to fill");
     }
#endif
}

#define MAX_POINTS 200

struct RBPlane1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgline, *qgplane;
#endif
   int npoints;
   Point p[200];
};

#ifdef NEW_RUBBERB
void RBPlane1(int dm,void *data,Point *p2,View3dSurface *)
{
 RBPlane1_data *rbdata = (RBPlane1_data *) data;
 Point p3,p4;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point origin,xaxis,yaxis;
 BitMask options(32),visible(MaxViews);
 RCCHAR patternfilename[300];
 EntityList list;
 int i;
 RCCOLORREF gradientcolour;

 if( dm == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  if (rbdata->npoints == 1)
    {
       Line line(rbdata->p[0],*p2);
       //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
  }
  else if (rbdata->npoints > 1)
    {
       visible = *db.header.getvisible();
       db.header.getvisible()->clearall();
       rbdata->p[rbdata->npoints] = *p2;
       for (i = 0 ; i <= rbdata->npoints  ; i++)
         list.add(new Line(rbdata->p[i],rbdata->p[(i+1) % (rbdata->npoints+1)]));
       db.header.setvisible(visible);

       Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
       Plane plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       if (plane.getdefined())
       {
           //plane.qgi = rbdata->qgplane; plane.zValue = rbdata->zValue;
           plane.draw(DM_INVERT);
           //rbdata->qgplane = plane.qgi; rbdata->zValue = plane.zValue;
       }
    }

}
#else
void RBPlane1(int dm,void *data,Point *p2,View3dSurface *)
{
 RBPlane1_data *rbdata = (RBPlane1_data *) data;
 Point p3,p4;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point origin,xaxis,yaxis;
 BitMask options(32),visible(MaxViews);
 RCCHAR patternfilename[300];
 EntityList list;
 int i;
 RCCOLORREF gradientcolour;

  if (rbdata->npoints == 1)
    {
       Line line(rbdata->p[0],*p2);
#ifdef USING_WIDGETS
       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
#else
       line.draw(DM_INVERT);
#endif
    }
  else if (rbdata->npoints > 1)
    {  
       visible = *db.header.getvisible();
       db.header.getvisible()->clearall();
       rbdata->p[rbdata->npoints] = *p2;
       for (i = 0 ; i <= rbdata->npoints  ; i++)
         list.add(new Line(rbdata->p[i],rbdata->p[(i+1) % (rbdata->npoints+1)]));
       db.header.setvisible(visible);
      
       Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
       Plane plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
#ifdef USING_WIDGETS
       if (plane.getdefined())
       {
           plane.qgi = rbdata->qgplane; plane.zValue = rbdata->zValue;
           plane.draw(DM_INVERT);
           rbdata->qgplane = plane.qgi; rbdata->zValue = plane.zValue;
       }
#else
       if (plane.getdefined())
         plane.draw(DM_INVERT);
#endif
    }

}
#endif

/**
 * @brief plane_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Plane by Boundary points command
 *
 * KMJ : Working in Qt version
 * TODO : add undo to the input loop??
 *
 */
void plane_command4(int *cstate,HighLevelEvent *event,void **data)
{
 RBPlane1_data *rbdata = (RBPlane1_data *) *data;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point origin,xaxis,yaxis;
 BitMask options(32);
 EntityList list;
 RCCHAR patternfilename[300];
 int i;
 BitMask visible(MaxViews);
 RCCOLORREF gradientcolour;
 

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(409,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {case InitialState :
       rbdata = new RBPlane1_data;  *data = rbdata;
       rbdata->npoints = 0;
#ifdef USING_WIDGETS
       rbdata->qgline = 0;
       rbdata->qgplane = 0;
       rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
       *cstate = 1;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p[rbdata->npoints++] = ((Coordinate3dEvent *) event)->getp();

            if (rbdata->npoints > 2)
              {  for (i = 0 ; i < rbdata->npoints  ; i++)
                   list.add(new Line(rbdata->p[i],rbdata->p[(i+1) % rbdata->npoints]));
                 Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                 Plane plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 if (! plane.getdefined())
                   rbdata->npoints--;  
                   

              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  if (rbdata->npoints > 0)
              {  if (rbdata->npoints >= 3)
                   {  visible = *db.header.getvisible();
                      db.header.getvisible()->clearall();
                      for (i = 0 ; i < rbdata->npoints ; i++)
                        list.add(new Line(rbdata->p[i],rbdata->p[(i+1) % rbdata->npoints]));
                      db.header.setvisible(visible);
                      Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                      Plane *plane = new Plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                      if (plane->getdefined())
                      {
                           Entity *e;
                           EntityList list;
                           db.geometry.add(plane);
                           //db.geometry.tofront(plane);
                           db.geometry.toback(plane);
                           list = plane->getlist();
                           for (list.start() ; ! list.atend() ; )
                           {
                               if ((e = list.next()) != 0)
                               {
                                  //db.geometry.tofront(e);
                                  e->draw(DM_NORMAL);
                               }
                           }
                      }
                      else
                        delete plane;
                   } 
                 rbdata->npoints = 0;
              }
            else
              *cstate = ExitState;
         } 
       break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgline)
         cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
      delete rbdata->qgline; rbdata->qgline=0;
      if(rbdata->qgplane)
         cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgplane);
      delete rbdata->qgplane; rbdata->qgplane=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBPlane1,rbdata);
       state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
       if (rbdata->npoints == 0)
         cadwindow->prompt->normalprompt("Locate the first plane boundary point");
       else
         cadwindow->prompt->normalprompt("Locate the plane boundary point");
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

/**
 * @brief plane_command5
 * @param cstate
 * @param event
 *
 * Entry point for the set Plane anchor point command
 *
 * KMJ : working in Qt version
 */
void plane_command5(int *cstate,HighLevelEvent *event,void **)
{Point p;
 static EntityList list;
 Entity *e;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(409,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       state.destroyselection(0);
       if (list.length() == 0)
         *cstate = 1;
       else  
         *cstate = 2;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            list.add(((EntitySelectedEvent *) event)->getentity());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;

     case 2 :
       if (event->isa(Coordinate3d))
         {  for (list.start() ; (e = list.next()) != 0 ; )
              if (e->isa(plane_entity))
                ((Plane *)e)->SetAnchorPoint(((Coordinate3dEvent *)event)->getp(),1);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(plane_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the plane to modify");
    }
  else if (*cstate == 2)
    {
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the anchor point of the plane");
    }
  else if (*cstate == ExitState)
    state.setselection(list,1);  //  Reset the selected planes into the current selection set
}

/**
 * @brief plane_command6
 * @param cstate
 * @param event
 *
 * Entry point for the Remove Hole (in a plane) Command
 *
 * KMJ : Working in the Qt version
 *
 */
void plane_command6(int *cstate,HighLevelEvent *event,void **)
{Point p;
 Entity *e1,*e2,*e3;
 EntityList list,alist,plist,olist;
 int error;


  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(409,0));
       *cstate = ExitState;
       return;
    }

  error = 0;

  db.saveundo(UD_STARTBLOCK,NULL);
       
  list = state.getselection().copy();
  while (list.length() > 0)
    {  //  Get the first entity of the selection
       list.start();
       e1 = list.next();
       list.del(e1);

       //  Search associated entities to see if this is part of a plane
       alist = (e1->getalist()).copy();
       for (alist.start() ; (e2 = alist.next()) != 0 ; )
         if (e2->isa(plane_entity))
           {  plist = (((Plane *)e2)->getlist()).copy();
              olist = state.getselection();
              for (olist.start() ; (e3 = olist.next()) != 0 ; )
                plist.del(e3);
              if (plist.length() < ((Plane *)e2)->getlist().length())
                {Loops loops(1);

                   loops.create(&plist);
                   if (! plist.empty() || loops.getloops().empty())
                     error = 1;
                   else
                     {  e2->draw(DM_ERASE);
                        cadwindow->invalidatedisplaylist(e2);
                        plist = (((Plane *)e2)->getlist()).copy();
                        for (olist.start() ; (e3 = olist.next()) != 0 ; )
                          if (plist.inlist(e3))
                            {  e3->dela(e2);
                               plist.del(e3);
                            }
                        db.saveundo(UD_MOVED,e2);
                        ((Plane *)e2)->setlist(plist);
                        e2->draw(DM_NORMAL);
                     }
                }
           }
         alist.destroy();

    }

  if (error == 0)
    {//  Delete the selected entities
       //PostMessage(cadwindow->gethwnd(),WM_COMMAND,205,0);
       delete_command(cstate,0,0);
    }
  
  db.saveundo(UD_ENDBLOCK,NULL);



  *cstate = ExitState;


}

//extern "C" void WINAPI AddVertexTool(void);
typedef int (WINAPI *runcommand)(char *);

/**
 * @brief plane_command6
 * @param cstate
 * @param event
 *
 * Entry point for the Add Vertex command
 *
 */
void plane_command7(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            *cstate = ExitState;
            /*
            // KMJ: alternative method of running commands in modules or plugins
            // if you know the command description
            int cmdid=0;
            if((cmdid = commands.command(_RCT("Add Vertex"))) >=0)
            {
                NewCommandEvent *event = new NewCommandEvent(cmdid,0);
                state.sendevent(event);
                state.process();
            }
            */
            // KMJ: new api methods to run commands in modules or plugins
            // which do not have there own button menues
            // they can be hooked up to other button menus or menu actions if required
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Add Vertex"); // add vertex function
        }
        else
        {
            // put up a message to tell the suer the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools plugin is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

void plane_command8(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Delete Vertex"); // delete plane vertex function

        }
        else
        {
            // put up a message to tell the user the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools plugin is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

void plane_command9(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Move Vertex"); // move plane vertex function
        }
        else
        {
            // put up a message to tell the user the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools plugin is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

void plane_command10(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Copy Plane Boundary"); // Copy Plane Boundary function
        }
        else
        {
            // put up a message to tell the user the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools plugin is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

void plane_command11(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Detach Entities"); // Detach Entities function
        }
        else
        {
            // put up a message to tell the user the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools plugin is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

void plane_command12(int *cstate,HighLevelEvent *event,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libEnhancedTools.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("EnhancedTools.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"White Out"); // White Out function
        }
        else
        {
            // put up a message to tell the user the enhanced tool plugin is not installed
            QMessageBox::warning(0,"Enhanced Tools","The Enhanced Tools module is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}

struct RBPlane13_data
{
    QGraphicsItem *qgi;
    qreal zValue;
    int nloops;

    std::vector<PickEntItemList> elist;
};

void RBPlane13(int dm,void *data,Point *p2,View3dSurface *)
{
    RBPlane13_data *rbdata = (RBPlane13_data *) data;

    if(dm ==  RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    for(uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        PointE point(rbdata->elist.at(rbdata->nloops).at(j).getPickPointE());
        point.draw(DM_INVERT);
    }
#if 0
    for(uint i=0; i < rbdata->elist.size(); i++)
    {
        for(uint j=0; j < rbdata->elist.at(i).size(); j++)
        {
            PointE point(rbdata->elist.at(i).at(j).getPickPointE());
            point.draw(DM_INVERT);
        }
    }
#endif
}

void getTrimmedSegments2(void *data)
{
    RBPlane13_data *rbdata = (RBPlane13_data *) data;
    Entity *e=0;
    EntityHeader rbHeader(5,0,0,5);

    // clear the intersections
    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.erase(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.begin(),rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.end());
    }
    // clear the trim segments
    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        if(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt() != 0)
            db.geometry.del(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt());
        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(nullptr);
    }

    // get the intersection points
    if(rbdata->elist.at(rbdata->nloops).size() > 1)
    {
        for (uint l=0; l < rbdata->elist.at(rbdata->nloops).size(); l++)
        {
            for (uint j=l; j < rbdata->elist.at(rbdata->nloops).size(); j++)
            {
                if(rbdata->elist.at(rbdata->nloops).at(l).getOriginalEnt() == rbdata->elist.at(rbdata->nloops).at(j).getOriginalEnt())
                    continue;
                Entity *e1;
                e = rbdata->elist.at(rbdata->nloops).at(l).getEnt();
                e1 = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                Intersect i(e,e1);
                qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        qDebug() << "int point" << k << ": " << ip->getp().x << "," << ip->getp().y;
                        Point sp = ((LinearEntity*)e)->start();
                        Point ep = ((LinearEntity*)e)->end();
                        qDebug() << "e start point: " << sp.x << "," << sp.y;
                        qDebug() << "e end point  : " << ep.x << "," << ep.y;
                        sp = ((LinearEntity*)e1)->start();
                        ep = ((LinearEntity*)e1)->end();
                        qDebug() << "e1 start point: " << sp.x << "," << sp.y;
                        qDebug() << "e1 end point  : " << ep.x << "," << ep.y;

                        rbdata->elist.at(rbdata->nloops).at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        rbdata->elist.at(rbdata->nloops).at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                    else
                    {
                        qDebug() << "oyoy??";
                    }
                    // no connection put up a message?
                }
            }
        }

    }

    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
        if(e->type() == line_entity)
        {
            Point lp0;
            Point lp1;
            rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
            Line *line = new Line(rbHeader,lp0,lp1);
            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(line);
            db.geometry.add(line);
            // debug
            line->setselected(1);
            //
        }
        else if(e->type() == circle_entity)
        {
            // special case for a complete circle

            if(((Circle*)e)->getsweepa() > 1.99999 * M_PI)
            {
                // add dummy inter points at 0.0 and 1.0
                PointE *vp = new PointE(((Circle*)e)->position(0.0));
                IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=0.0;
                ipitem->t2=0.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                vp = new PointE(((Circle*)e)->position(1.0));
                ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=1.0;
                ipitem->t2=1.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            }

            Point lp0;
            Point lp1 = rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mPickPoint;
            Point lp2;

            if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Circle *ecp = (Circle*)e->clone(ident);
                e->setcopye(0);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                db.geometry.add(ecp);
                // debug
                ecp->setselected(1);
            }
            else
            {
                rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp2);
                Circle *circle = new Circle(1,lp0,lp1,lp2);
                circle->setcolour(rbHeader.getcolour());
                circle->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(circle);
                db.geometry.add(circle);
                // debug
                circle->setselected(1);
            }

        }
        else if(e->type() == curve_entity)
        {
            Point lp0;
            Point lp1;
            rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
            double t0 = ((Curve*)e)->gett(lp0);
            double t1 = ((Curve*)e)->gett(lp1);
            double disto0 = fabs(t0 - 0.0);
            double disto1 = fabs(t1 - 1.0);
            if (t0 < t1)
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim start to end
                ((Curve *)ecp)->moveendpoint(1,1,lp0);
                ((Curve *)ecp)->moveendpoint(1,2,lp1);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                db.geometry.add(ecp);
                // debug
                ecp->setselected(1);
            }
            else
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim end to start
                ((Curve *)ecp)->moveendpoint(1,2,lp0);
                ((Curve *)ecp)->moveendpoint(1,1,lp1);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                db.geometry.add(ecp);
                // debug
                ecp->setselected(1);
            }
        }
        else if(e->type() == ellipse_entity)
        {
            if(((EllipseE*)e)->getsweepa() > 1.99999 * M_PI)
            {
                // add dummy inter points at 0.0 and 1.0
                PointE *vp = new PointE(((EllipseE*)e)->position(0.0));
                IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=0.0;
                ipitem->t2=0.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                vp = new PointE(((EllipseE*)e)->position(1.0));
                ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=1.0;
                ipitem->t2=1.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            }

            if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                EllipseE *ecp = (EllipseE*)e->clone(ident);
                e->setcopye(0);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                db.geometry.add(ecp);
                // debug
                ecp->setselected(1);
            }
            else
            {
                Point lp0;
                Point lp1;
                rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                double t0 = ((EllipseE*)e)->gett(lp0);
                double t1 = ((EllipseE*)e)->gett(lp1);
                double disto0 = fabs(t0 - 0.0);
                double disto1 = fabs(t1 - 1.0);
                if (rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t0 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt < t1)
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    db.geometry.add(ecp);
                    // debug
                    ecp->setselected(1);
                }
                else if(rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t1 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt <= 1.0)
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    db.geometry.add(ecp);
                    // debug
                    ecp->setselected(1);
                }
                else
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    db.geometry.add(ecp);
                    // debug
                    ecp->setselected(1);
                }
            }
        }
    }
}

void getTrimmedSegments(void *data)
{
    RBPlane13_data *rbdata = (RBPlane13_data *) data;
    Entity *e=0;
    EntityHeader rbHeader(5,0,0,5);

    // clear the intersections
    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.erase(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.begin(),rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.end());
    }
    // clear the trim segments
    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        if(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt() != nullptr)
            db.geometry.del(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt());
        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(nullptr);
    }

    // get the intersection points
    if(rbdata->elist.at(rbdata->nloops).size() > 1)
    {
        for (uint l=0; l < rbdata->elist.at(rbdata->nloops).size(); l++)
        {
            for (uint j=l; j < rbdata->elist.at(rbdata->nloops).size(); j++)
            {
                if(rbdata->elist.at(rbdata->nloops).at(l).getOriginalEnt() == rbdata->elist.at(rbdata->nloops).at(j).getOriginalEnt())
                    continue;
                Entity *e1;
                e = rbdata->elist.at(rbdata->nloops).at(l).getEnt();
                e1 = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                Intersect i(e,e1);
                qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        rbdata->elist.at(rbdata->nloops).at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        rbdata->elist.at(rbdata->nloops).at(l).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        //db.geometry.add(ip);
                        //ip->draw(DM_NORMAL);
                    }
                    // no connection put up a message?
                }
            }
        }
    }

    for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
    {
        e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
        if(e->type() == line_entity)
        {
            // add dummy inter points at 0.0 and 1.0
            PointE *vp = new PointE(((Line*)e)->position(0.0));
            IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=0.0;
            ipitem->t2=0.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            vp = new PointE(((Line*)e)->position(1.0));
            ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=1.0;
            ipitem->t2=1.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

            Point lp0;
            Point lp1;
            rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
            Line *line = new Line(rbHeader,lp0,lp1);
            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(line);
            line->setselected(1);
            db.geometry.add(line);
            state.excludelist.add(line);
        }
        else if(e->type() == circle_entity)
        {
            // add dummy inter points at 0.0 and 1.0
            PointE *vp = new PointE(((Circle*)e)->position(0.0));
            IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=0.0;
            ipitem->t2=0.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            vp = new PointE(((Circle*)e)->position(1.0));
            ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=1.0;
            ipitem->t2=1.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

            Point lp0;
            Point lp1 = rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mPickPoint;
            Point lp2;

            rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp2);

            if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Circle *ecp = (Circle*)e->clone(ident);
                e->setcopye(0);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                ecp->setselected(1);
                db.geometry.add(ecp);
                state.excludelist.add(ecp);
            }
            else
            {
                rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp2);
                Circle *circle = new Circle(1,lp0,lp1,lp2);
                circle->setcolour(rbHeader.getcolour());
                circle->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(circle);
                circle->setselected(1);
                db.geometry.add(circle);
                state.excludelist.add(circle);
            }
        }
        else if(e->type() == curve_entity)
        {
            Point lp0;
            Point lp1;

            // add dummy inter points at 0.0 and 1.0
            PointE *vp = new PointE(((Curve*)e)->position(0.0));
            IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=0.0;
            ipitem->t2=0.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            vp = new PointE(((Curve*)e)->position(1.0));
            ipitem = new IntersectPointItem(vp,e,e);
            ipitem->t1=1.0;
            ipitem->t2=1.0;
            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

            rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
            double t0 = ((Curve*)e)->gett(lp0);
            double t1 = ((Curve*)e)->gett(lp1);
            double disto0 = fabs(t0 - 0.0);
            double disto1 = fabs(t1 - 1.0);
            if (t0 < t1)
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim start to end
                ((Curve *)ecp)->moveendpoint(1,1,lp0);
                ((Curve *)ecp)->moveendpoint(1,2,lp1);
                ((Curve *)ecp)->setcolour(rbHeader.getcolour());
                ((Curve *)ecp)->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                ecp->setselected(1);
                db.geometry.add(ecp);
                //ecp->draw(DM_NORMAL);
            }
            else
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Curve *ecp = (Curve*)e->clone(ident);
                e->setcopye(0);
                // trim end to start
                ((Curve *)ecp)->moveendpoint(1,2,lp0);
                ((Curve *)ecp)->moveendpoint(1,1,lp1);
                ((Curve *)ecp)->setcolour(rbHeader.getcolour());
                ((Curve *)ecp)->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                ecp->setselected(1);
                db.geometry.add(ecp);
                //ecp->draw(DM_NORMAL);
            }
        }
        else if(e->type() == ellipse_entity)
        {
            //if(((EllipseE*)e)->getsweepa() > 1.99999 * M_PI)
            {
                // add dummy inter points at 0.0 and 1.0
                PointE *vp = new PointE(((EllipseE*)e)->position(0.0));
                IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=0.0;
                ipitem->t2=0.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                vp = new PointE(((EllipseE*)e)->position(1.0));
                ipitem = new IntersectPointItem(vp,e,e);
                ipitem->t1=1.0;
                ipitem->t2=1.0;
                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
            }

            if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
               rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
            {
                Transform ident;
                ident.identity();
                e->setcopye(0);
                EllipseE *ecp = (EllipseE*)e->clone(ident);
                e->setcopye(0);
                ecp->setcolour(rbHeader.getcolour());
                ecp->setweight(rbHeader.getweight());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                ecp->setselected(1);
                db.geometry.add(ecp);
                state.excludelist.add(ecp);
            }
            else
            {
                Point lp0;
                Point lp1;
                rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                double t0 = ((EllipseE*)e)->gett(lp0);
                double t1 = ((EllipseE*)e)->gett(lp1);
                double disto0 = fabs(t0 - 0.0);
                double disto1 = fabs(t1 - 1.0);
                if (rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t0 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt < t1)
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    ecp->setselected(1);
                    db.geometry.add(ecp);
                    state.excludelist.add(ecp);
                }
                else if(rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t1 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt <= 1.0)
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    ecp->setselected(1);
                    db.geometry.add(ecp);
                    state.excludelist.add(ecp);
                }
                else
                {
                    Transform ident;
                    ident.identity();
                    e->setcopye(0);
                    EllipseE *ecp = (EllipseE*)e->clone(ident);
                    e->setcopye(0);
                    //
                    ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                    ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                    ecp->setcolour(rbHeader.getcolour());
                    ecp->setweight(rbHeader.getweight());
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                    ecp->setselected(1);
                    db.geometry.add(ecp);
                    state.excludelist.add(ecp);
                }
            }
        }
    }
}

bool isInNodeList(const InterPointItemList &nodeList, IntersectPointItem *bnode)
{
    for(uint i=0; i < nodeList.size(); i++)
    {
        if(nodeList.at(i)->mPoint == bnode->mPoint)
            return true;
    }
    return false;
}

bool isInNodeList(const InterPointItemList &nodeList, Point point)
{
    for(uint i=0; i < nodeList.size(); i++)
    {
        if(nodeList.at(i)->mPoint->getp() == point)
            return true;
    }
    return false;
}

bool isInPointList(std::vector<Point> &pointList, Point point)
{
    Point p;
    for (uint i=0; i < pointList.size(); i++)
    {
        p = pointList.at(i);
        if(p == point)
            return true;
    }
    return false;
}

bool isSegmentUnique(PickEntItem item1, PickEntItem item2)
{
    //if(item1.getEnt()->issame(Geometry,*item2.getEnt()))
    if(item1.getTrimEnt()->issame(Geometry,*item2.getTrimEnt()))
        return true;
    return false;
}

int getGapLines(void *data, std::vector<Point> &gapPoints, EntityList &gapLines)
{
    RBPlane13_data *rbdata = (RBPlane13_data *) data;
    std::vector<int> used;

    // find connected segments which have unconnected points
    // find the closest unconnected point to each of these segments

    // make a list of the connected segments which have a point in the gapPoints list
    // match up each segments unconnected point with the closest other segment
    for(uint i=0; i < gapPoints.size(); i++)
        used.push_back(0);


    for(uint i=0; i < gapPoints.size(); i++)
    {
        if(used.at(i) == 1)
            continue;

        Point p0 = gapPoints.at(i);
        for (uint j=0; j < rbdata->elist.size(); j++)
        {
            for (uint k=0; k < rbdata->elist.at(j).size(); k++)
            {
                LinearEntity *e = (LinearEntity*)rbdata->elist.at(j).at(k).getTrimEnt();
                if(e != nullptr)
                {
                    if(p0 == e->start() || p0 == e->end())
                    {
                        used.at(i) = 1;
                        // force the outer loop to end
                        j = rbdata->elist.size();
                        break;
                    }
                }
            }
        }
        // find the closest point to p0
        Point p1;
        uint p1Id=-1;
        double dist = DBL_MAX;
        if(gapPoints.size() > 1)
        {
        for(uint ii=i; ii < gapPoints.size(); ii++)
        {
            if(used.at(ii) == 1)
                continue;

            Point tp = gapPoints.at(ii);
            if((tp - p0).length() < dist)
            {
                p1 = tp;
                p1Id = ii;
                dist = (tp - p0).length();
            }
        }
        for (uint j=0; j < rbdata->elist.size(); j++)
        {
            // find the segment with this point
            for (uint k=0; k < rbdata->elist.at(j).size(); k++)
            {
                LinearEntity *e = (LinearEntity*)rbdata->elist.at(j).at(k).getTrimEnt();
                if(e != nullptr)
                {
                    if(p1 == e->start() || p1 == e->end())
                    {
                        // force the outer loop to end
                        j = rbdata->elist.size();
                        break;
                    }
                }
            }
        }
        // create a joining line
        if(p1Id >= 0)
        {
            Line *line = new Line(p0,p1);
            gapLines.add(line);
            // remove p1 from the list
            used.at(p1Id) = 1;
        }
        }
    }

    return gapLines.length();
}

/**
 * @brief findBoundaryGaps
 * @param data
 * in: rubberband data struct
 * @param bounds
 * in: boundary entity list
 * @param points
 * out: list of points not shared between two boundary entities
 * @return
 * number of unshared points
 */
int findBoundaryGaps(void *data, EntityList &bounds, std::vector<Point> &points)
{
    RBPlane13_data *rbdata = (RBPlane13_data *) data;
    LinearEntity *e=0, *e1=0;
    Point p,p1;
    std::vector<LinearEntity*>  compare;
    std::vector<Point> connectedPoints, unconnectedPoints;

    // look for connected points in the trimmed ents
    for(bounds.start(); (e = (LinearEntity*) bounds.next()) != NULL ; )
        compare.push_back(e);

    for(uint i =0; i < compare.size(); i++)
    {
        for(uint j =0; j < compare.size(); j++)
        {
            qDebug() << "i= " << i << "j= " << j;
            e = compare.at(i);
            e1 = compare.at(j);
            if(e == e1)
                continue;
            if(e->issame(Geometry,*e1))
                continue;
            if(e->type() == circle_entity && e->start() == e->end())
                continue;
            if(e->type() == ellipse_entity && e->start() == e->end())
                continue;
            if(e1->type() == circle_entity && e1->start() == e1->end())
                continue;
            if(e1->type() == ellipse_entity && e1->start() == e1->end())
                continue;

            if(e->start() == e1->start() || e->start() == e1->end())
                if(!isInPointList(connectedPoints,e->start()))
                    connectedPoints.push_back(e->start());
            if(e->end() == e1->end() || e->end() == e1->start())
                if(!isInPointList(connectedPoints,e->end()))
                    connectedPoints.push_back(e->end());
        }
    }

    //qDebug() << "got connected nodes";
    for (uint i=0; i < connectedPoints.size(); i++ )
    {
        Point p = connectedPoints.at(i);
        qDebug() << "point: (" << p.x << "," << p.y << ","<< p.z << ")";
    }

    // find the unconnected points
    for(uint i =0; i < compare.size(); i++)
    {
        e = compare.at(i);
        if(e->type() == circle_entity && e->start() == e->end())
            continue;
        if(e->type() == ellipse_entity && e->start() == e->end())
            continue;

        if(!isInPointList(connectedPoints,e->start()))
            if(!isInPointList(unconnectedPoints,e->start()))
                unconnectedPoints.push_back(e->start());
        if(!isInPointList(connectedPoints,e->end()))
            if(!isInPointList(unconnectedPoints,e->end()))
                unconnectedPoints.push_back(e->end());
    }

    //qDebug() << "got unconnected nodes";
    for (uint i=0; i < unconnectedPoints.size(); i++ )
    {
        Point p = unconnectedPoints.at(i);
        qDebug() << "point: (" << p.x << "," << p.y << ","<< p.z << ")";
    }

    points = unconnectedPoints;

    //qDebug() << "done";

    return unconnectedPoints.size();
}

int doPlanePopup(void *data)
{
    static QMenu *Popup = 0;
    RBPlane13_data *rbdata = (RBPlane13_data *) data;
    double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
    Point origin,xaxis,yaxis;
    BitMask options(32);
    EntityList list;
    RCCHAR patternfilename[300];
    BitMask visible(MaxViews);
    RCCOLORREF gradientcolour;
    Entity *e;
    ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
    EntityHeader rbHeader(5,0,0,5);

    if(rbdata->elist.size() > 0)
    {
        RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();

        // pop up the context menu
        if(Popup == 0)
        {
            Popup = new QMenu((QWidget*)pView);
            QAction *Popup_action_Done = new QAction(Popup);
            Popup_action_Done->setObjectName(QStringLiteral("Popup_action_Done"));
            Popup_action_Done->setText("Done");
            //
            QAction *Popup_action_Continue = new QAction(Popup);
            Popup_action_Continue->setObjectName(QStringLiteral("Popup_action_Continue"));
            Popup_action_Continue->setText("Continue loop");
            //
            QAction *Popup_action_AddToLoop = new QAction(Popup);
            Popup_action_AddToLoop->setObjectName(QStringLiteral("Popup_action_AddToLoop"));
            Popup_action_AddToLoop->setText("Add to loop");
            //
            QAction *Popup_action_Undo = new QAction(Popup);
            Popup_action_Undo->setObjectName(QStringLiteral("Popup_action_Undo"));
            Popup_action_Undo->setText("Undo");
            //
            QAction *Popup_action_Quit = new QAction(Popup);
            Popup_action_Quit->setObjectName(QStringLiteral("Popup_action_Quit"));
            Popup_action_Quit->setText("Quit");
            //
            QAction *Popup_action_NewLoop = new QAction(Popup);
            Popup_action_NewLoop->setObjectName(QStringLiteral("Popup_action_NewLoop"));
            Popup_action_NewLoop->setText("New loop");
            //
            QAction *Popup_action_Delete = new QAction(Popup);
            Popup_action_Delete->setObjectName(QStringLiteral("Popup_action_Delete"));
            Popup_action_Delete->setText("Delete");
            //
            Popup->setObjectName("Plane Insert Context Menu");
            //
            Popup->addAction(Popup_action_Continue);
            Popup->addAction(Popup_action_AddToLoop);
            Popup->addAction(Popup_action_NewLoop);
            Popup->addAction(Popup_action_Delete);
            Popup->addAction(Popup_action_Undo);
            Popup->addAction(Popup_action_Done);
            Popup->addAction(Popup_action_Quit);
        }

        QRect vRect = pView->view()->rect();
        QPoint cPos = pView->mapFromGlobal(QCursor::pos());
        QAction *menuRet = 0;
        if(vRect.contains(cPos))
            menuRet = Popup->exec(QCursor::pos());

        if( menuRet && menuRet->text() == "Undo"  )
        {
            // undo
            Popup->hide();
            if(rbdata->elist.size() > 0)
            {
                int lp = rbdata->nloops;
                if(rbdata->nloops > 0 && rbdata->elist.at(rbdata->nloops).size() == 0)
                {
                    rbdata->elist.pop_back();
                    rbdata->nloops--;
                }
                if(rbdata->elist.at(rbdata->nloops).size() > 0)
                {
                    rbdata->elist.at(rbdata->nloops).back().setTrimEnt(nullptr);
                    rbdata->elist.at(rbdata->nloops).pop_back();
                    getTrimmedSegments2(rbdata);
                }
            }
            //*cstate = 2;
            return 2;
        }
        else if ( menuRet && menuRet->text() == "Done" )
        {
            Popup->hide();
            if(rbdata->elist.size() > 0)
            {
                EntityHeader cheader;
                EntityList bounds;
                bounds.start();
                for(uint i=0; i<rbdata->elist.size(); i++)
                {
                    if(rbdata->elist.at(i).size() > 0)
                    {

                        for (uint j=0; j < rbdata->elist.at(i).size(); j++)
                        {
                            Entity *te = rbdata->elist.at(i).at(j).getTrimEnt();
                            te->setcolour(cheader.getcolour());
                            te->setweight(cheader.getweight());
                            bounds.add(te);
                        }
                    }
                }

                std::vector<Point> gapPoints;
                int ngaps = findBoundaryGaps(rbdata, bounds, gapPoints);
                if(ngaps > 0)
                {
                    // gap manager stuff here add the gap fills to the bounds list
                    EntityList gapLines;
                    getGapLines(rbdata, gapPoints, gapLines);
                    Entity *e;
                    for(gapLines.start(); (e = gapLines.next()) != NULL ;)
                        bounds.add(e);
                }

                Loops loops(1);
                loops.create(&bounds);

                Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                Plane *plane = new Plane(loops.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                if (plane->getdefined())
                {
                     Entity *e;
                     EntityList list;
                     db.geometry.add(plane);
                     db.geometry.toback(plane);
                     list = plane->getlist();
                     for (list.start() ; ! list.atend() ; )
                     {
                         if ((e = list.next()) != nullptr)
                         {
                             e->draw(DM_NORMAL);
                             db.geometry.toback(e);
                         }
                     }
                }
                else
                {
                    //cadwindow->MessageBox(rs3.gets(),rs4.gets(),MB_ICONINFORMATION);
                    cadwindow->MessageBox(_RCT("The plane could not be created"),rs4.gets(),MB_ICONINFORMATION);
                    for(uint i=0; i < rbdata->elist.size(); i++)
                    {
                        for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                            rbdata->elist.at(i).at(j).setTrimEnt(nullptr);
                    }
                    delete plane;
                }
            }
            db.saveundo(UD_ENDBLOCK,NULL);
            //*cstate = ExitState;
            if(Popup != 0)
            {
                Popup->deleteLater();
                Popup = 0;
            }
            return ExitState;
        }
        else if ( menuRet &&  menuRet->text() == "Continue loop" )
        {
            // continue loop
            Popup->hide();
            //*cstate = 2;
            return 2;
        }
        else if ( menuRet &&  menuRet->text() == "Add to loop" )
        {
            // continue loop
            Popup->hide();
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    state.excludelist.add(rbdata->elist.at(i).at(j).mOrigPickEnt);
                    state.excludelist.del(rbdata->elist.at(i).at(j).getTrimEnt());
                }
            }
            //*cstate = 4;
            return 4;
        }
        else if ( menuRet &&  menuRet->text() == "New loop" )
        {
            // new loop
            Popup->hide();
            rbdata->nloops++;
            rbdata->elist.push_back(PickEntItemList());
            //*cstate = 2;
            return 2;
        }
        else if ( menuRet &&  menuRet->text() == "Delete" )
        {
            // delete
            Popup->hide();
            // make the pick ents and copy ents unselectable
            // make the trim edges selectable
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    state.excludelist.add(rbdata->elist.at(i).at(j).mOrigPickEnt);
                    state.excludelist.del(rbdata->elist.at(i).at(j).getTrimEnt());
                }
            }
            //cstate = 3;
            return 3;
        }
        else if ( menuRet && menuRet->text() == "Quit" )
        {
            // quit
            Popup->hide();
            db.saveundo(UD_ENDBLOCK,NULL);
            db.undo();
            //*cstate = ExitState;
            if(Popup != 0)
            {
                Popup->deleteLater();
                Popup = 0;
            }
            return ExitState;
        }
        // continue
        //*cstate = 2;
        return 2;
    }
    db.saveundo(UD_ENDBLOCK,NULL);
    if(Popup != 0)
    {
        Popup->deleteLater();
        Popup = 0;
    }
    return ExitState;
}

/**
 * @brief plane_command13
 * @param cstate
 * @param event
 *
 * Create a plane using trimmed sections of line. circles or curves.
 *
 */
void plane_command13(int *cstate,HighLevelEvent *event,void **data)
{
#if 1
    // method
    // user picks curves in any order
    // generate any intersections between all selected curves
    // chain the curves into an ordered list
    // generate the curve segements
    // create the plane

    RBPlane13_data *rbdata = (RBPlane13_data *) *data;
    double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
    Point origin,xaxis,yaxis;
    BitMask options(32);
    EntityList list;
    RCCHAR patternfilename[300];
    BitMask visible(MaxViews);
    RCCOLORREF gradientcolour;
    Entity *e;
    ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
    EntityHeader rbHeader(5,0,0,5);
    static BitMask snapmask(32);

    switch(*cstate)
    {
    case InitialState:
        rbdata = new RBPlane13_data;  *data = rbdata;
        rbdata->qgi=nullptr;
        rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
        rbdata->nloops=0;
        rbdata->elist.push_back(PickEntItemList());
        state.destroyselection(1);
        state.excludelist.destroy();
        snapmask = db.snapmask;
        db.snapmask.set(SnapMaskOrigin,0);
        db.snapmask.set(SnapMaskIntersection,0);
        db.snapmask.set(SnapMaskEnd,0);
        db.saveundo(UD_STARTBLOCK,NULL);
        *cstate = 1;
        break;
    case 1:
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
            Transform ident;
            ident.identity();
            e->setcopye(0);
            Entity *ecp = e->clone(ident);
            rbdata->elist.at(rbdata->nloops).push_back(PickEntItem(ecp,PickPointItem(ecp,pp),e));
            state.excludelist.add(ecp);
            //
            e->setcopye(0);
            ecp = e->clone(ident);
            ecp->setcolour(5);
            ecp->setweight(5);
            db.geometry.add(ecp);
            ecp->setselected(1);
            rbdata->elist.at(rbdata->nloops).at(rbdata->elist.at(rbdata->nloops).size()-1).setTrimEnt(ecp);
            state.excludelist.add(ecp);
            e->setselected(0);
            *cstate = 2;
            break;
        }
        else if (event->isa(Exit) || event->isa(Abort))
        {
            *cstate = doPlanePopup(rbdata);
        }
        break;
    case 2:
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
            Transform ident;
            ident.identity();
            e->setcopye(0);
            Entity *ecp = e->clone(ident);
            rbdata->elist.at(rbdata->nloops).push_back(PickEntItem(ecp,PickPointItem(ecp,pp),e));
            state.excludelist.add(ecp);
            //
            e->setcopye(0);
            ecp = e->clone(ident);
            ecp->setcolour(5);
            ecp->setweight(5);
            db.geometry.add(ecp);
            ecp->setselected(1);
            rbdata->elist.at(rbdata->nloops).at(rbdata->elist.at(rbdata->nloops).size()-1).setTrimEnt(ecp);
            state.excludelist.add(ecp);
            e->setselected(0);

            db.saveundo(UD_STARTBLOCK,NULL);
            getTrimmedSegments2(rbdata);
            db.saveundo(UD_ENDBLOCK,NULL);
        }
        else if (event->isa(Abort))
        {
            //rbdata->nloops++;
            //rbdata->elist.push_back(PickEntItemList());
            //*cstate = 1;
            *cstate = doPlanePopup(rbdata);
        }
        else if (event->isa(Exit))
        {
            db.saveundo(UD_ENDBLOCK,NULL);
            *cstate = ExitState;
        }
        break;
    case 3: // delete
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    if(e == rbdata->elist.at(i).at(j).getTrimEnt())
                    {
                        state.excludelist.del(rbdata->elist.at(i).at(j).mOrigPickEnt);
                        state.excludelist.del(rbdata->elist.at(i).at(j).getTrimEnt());
                        rbdata->elist.at(i).at(j).setTrimEnt(nullptr);
                        rbdata->elist.at(i).erase(rbdata->elist.at(i).begin()+j);
                        rbdata->nloops = i;
                    }
                }
            }
            getTrimmedSegments2(rbdata);
            state.excludelist.destroy();
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    state.excludelist.add(rbdata->elist.at(i).at(j).mOrigPickEnt);
                    state.excludelist.del(rbdata->elist.at(i).at(j).getTrimEnt());
                }
            }
            *cstate = 3;
        }
        else if (event->isa(Abort) || event->isa(Exit))
        {
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    state.excludelist.del(rbdata->elist.at(i).at(j).mOrigPickEnt);
                    state.excludelist.add(rbdata->elist.at(i).at(j).getTrimEnt());                    
                }
            }
            //*cstate = 2;
            *cstate = doPlanePopup(rbdata);
        }
        break;
    case 4: // add to loop
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
            for(uint i=0; i < rbdata->elist.size(); i++)
            {
                for(uint j=0; j < rbdata->elist.at(i).size(); j++)
                {
                    if(e == rbdata->elist.at(i).at(j).getTrimEnt())
                    {
                        rbdata->elist.at(i).at(j).getTrimEnt()->setcolour(5);
                        rbdata->elist.at(i).at(j).getTrimEnt()->setweight(5);
                        rbdata->nloops = i;
                        for(i=0; i < rbdata->elist.size(); i++)
                        {
                            for(j=0; j < rbdata->elist.at(i).size(); j++)
                            {
                                state.excludelist.del(rbdata->elist.at(i).at(j).mOrigPickEnt);
                                state.excludelist.add(rbdata->elist.at(i).at(j).getTrimEnt());
                                //rbdata->elist.at(i).at(j).getTrimEnt()->setselected(1);
                            }
                        }
                        getTrimmedSegments2(rbdata);
                        *cstate = 2;
                        break;
                    }
                }
            }
        }
        else if (event->isa(Abort) || event->isa(Exit))
            *cstate = doPlanePopup(rbdata);
        break;
    }
//
//  About to enter state actions
//
    if (*cstate == 1 || *cstate == 2)
    {
         rubberband.begin(0);
         rubberband.add(RBPlane13,rbdata);
         state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
         if (rbdata->elist.size() == 0)
           cadwindow->prompt->normalprompt("Locate the first plane boundary entity");
         else
           cadwindow->prompt->normalprompt("Locate the plane boundary entity");
    }
    else if (*cstate == 3)
    {
         rubberband.begin(0);
         rubberband.add(RBPlane13,rbdata);
         state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
         cadwindow->prompt->normalprompt("Locate the boundary entity to delete");
    }
    else if (*cstate == 4)
    {
         rubberband.begin(0);
         rubberband.add(RBPlane13,rbdata);
         state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
         cadwindow->prompt->normalprompt("Locate a boundary entity on the loop to add to");
    }
    else if (*cstate == ExitState)
    {
        if(rbdata != 0)
        {
            // clear the intersections
            for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
                rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.erase(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.begin(),rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.end());

            // clear the trim segments
            for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(nullptr);

            rbdata->elist.clear();
        }
       state.excludelist.destroy();
       delete rbdata;
       db.snapmask = snapmask;

       /*
       if(Popup != 0)
       {
           Popup->deleteLater();
           Popup = 0;
       }
       */
    }
#endif
# if 0
    // method
    // the user picks the target entities at a point within the part of the entity that will make up the boundary of the plane.
    // each subsequet curve will be intersected with the last curve and the segment will be used
    // this is repeated until the the selected segment form a closed boundary or the user stops the process.
    // if the user stops the process with an open boundary they can close it with a straight line or quit the command.

    RBPlane13_data *rbdata = (RBPlane13_data *) *data;
    double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
    Point origin,xaxis,yaxis;
    BitMask options(32);
    EntityList list;
    RCCHAR patternfilename[300];
    BitMask visible(MaxViews);
    RCCOLORREF gradientcolour;
    Entity *e;
    ResourceString rs3(NCPLANEB+3),rs4(NCPLANEB+4);
    EntityHeader rbHeader(5,0,0,5);

    switch(*cstate)
    {
    case InitialState:
        rbdata = new RBPlane13_data;  *data = rbdata;
        rbdata->qgi=nullptr;
        rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
        rbdata->nloops=0;
        rbdata->elist.push_back(PickEntItemList());
        state.destroyselection(1);
        *cstate = 1;
        break;
    case 1:
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            // ToDo: check for repeated curves?
            //if(std::find(rbdata->elist.begin(),rbdata->elist.end(),e) == rbdata->elist.end())
            {
                Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Entity *ecp = e->clone(ident);
                rbdata->elist.at(rbdata->nloops).push_back(PickEntItem(ecp,PickPointItem(ecp,pp)));
                //
                e->setcopye(0);
                ecp = e->clone(ident);
                ecp->setcolour(5);
                ecp->setweight(5);
                db.geometry.add(ecp);
                rbdata->elist.at(rbdata->nloops).at(rbdata->elist.at(rbdata->nloops).size()-1).setTrimEnt(ecp);
            }
            *cstate = 2;
            break;
        }
        else if (event->isa(Exit) || event->isa(Abort))
        {
            if(rbdata->elist.size() > 0)
            {
                EntityHeader cheader;
                EntityList bounds;
                bounds.start();
                for(uint i=0; i<rbdata->elist.size(); i++)
                {
                    if(rbdata->elist.at(i).size() > 0)
                    {
                        for (uint j=0; j < rbdata->elist.at(i).size(); j++)
                        {
                            Entity *te = rbdata->elist.at(i).at(j).getTrimEnt();
                            te->setcolour(cheader.getcolour());
                            te->setweight(cheader.getweight());
                            bounds.add(te);
                        }
                        bounds.add(0);
                    }
                }
                Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                Plane *plane = new Plane(bounds,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                if (plane->getdefined())
                  {Entity *e;
                   EntityList list;
                     db.geometry.add(plane);
                     db.geometry.toback(plane);
                     list = plane->getlist();
                     for (list.start() ; ! list.atend() ; )
                     {
                         if ((e = list.next()) != nullptr)
                         {
                             e->draw(DM_NORMAL);
                         }
                     }
                  }
                else
                  {  cadwindow->MessageBox(rs3.gets(),rs4.gets(),MB_ICONINFORMATION);
                     delete plane;
                  }
            }
            *cstate = ExitState;
        }
        break;
    case 2:
        rubberband.end(0);
        state.destroyselection(1);
        if(event->isa(EntitySelected))
        {
            e = ((EntitySelectedEvent *) event)->getentity();
            // ToDo: check for repeated curves?
            //if(std::find(rbdata->elist.begin(),rbdata->elist.end(),e) == rbdata->elist.end())
            {
                Point pp = e->nearp(((EntitySelectedEvent *) event)->getp());
                Transform ident;
                ident.identity();
                e->setcopye(0);
                Entity *ecp = e->clone(ident);
                rbdata->elist.at(rbdata->nloops).push_back(PickEntItem(ecp,PickPointItem(ecp,pp)));
                //
                e->setcopye(0);
                ecp = e->clone(ident);
                ecp->setcolour(5);
                ecp->setweight(5);
                db.geometry.add(ecp);
                rbdata->elist.at(rbdata->nloops).at(rbdata->elist.at(rbdata->nloops).size()-1).setTrimEnt(ecp);
            }

            if(rbdata->elist.at(rbdata->nloops).size() > 1)
            {
                // get the intersection points
                Entity *e1;
                uint j = rbdata->elist.at(rbdata->nloops).size();
                e = rbdata->elist.at(rbdata->nloops).at(j-2).getEnt();
                e1 = rbdata->elist.at(rbdata->nloops).at(j-1).getEnt();
                Intersect i(e,e1);
                qDebug() << "inters : " << i.nintersections();
                if(i.nintersections() != 0)
                {
                    for(int k=0; k < i.nintersections(); k++)
                    {
                        PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        rbdata->elist.at(rbdata->nloops).at(j-2).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j-1).addIntPointItem(ipitem);

                        db.geometry.add(ip);
                        ip->draw(DM_NORMAL);
                    }
                }
                else
                {
                    PointE *ip = nullptr;
                    // no intersection get the closest end points
                    if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->start());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                        ip = new PointE(((LinearEntity*)e)->end());
                    if(ip != nullptr)
                    {
                        IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                        rbdata->elist.at(rbdata->nloops).at(j-2).addIntPointItem(ipitem);

                        ipitem = new IntersectPointItem(ip,e1,e);
                        rbdata->elist.at(rbdata->nloops).at(j-1).addIntPointItem(ipitem);

                        db.geometry.add(ip);
                        ip->draw(DM_NORMAL);
                    }
                    // no connection put up a message?
                }

                for (uint j=rbdata->elist.at(rbdata->nloops).size()-2; j < rbdata->elist.at(rbdata->nloops).size(); j++)
                {
                    e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                    if(e->type() == line_entity)
                    {
                        // add dummy inter points at 0.0 and 1.0
                        PointE *vp = new PointE(((Circle*)e)->position(0.0));
                        IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=0.0;
                        ipitem->t2=0.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        vp = new PointE(((Circle*)e)->position(1.0));
                        ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=1.0;
                        ipitem->t2=1.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        Point lp0;
                        Point lp1;
                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                        Line *line = new Line(rbHeader,lp0,lp1);
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(line);
                        db.geometry.add(line);
                    }
                    else if(e->type() == circle_entity)
                    {
                        // add dummy inter points at 0.0 and 1.0
                        PointE *vp = new PointE(((Circle*)e)->position(0.0));
                        IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=0.0;
                        ipitem->t2=0.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        vp = new PointE(((Circle*)e)->position(1.0));
                        ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=1.0;
                        ipitem->t2=1.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        Point lp0;
                        Point lp1 = rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mPickPoint;
                        Point lp2;

                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp2);

                        Circle *circle = new Circle(1,lp0,lp1,lp2);
                        circle->setcolour(rbHeader.getcolour());
                        circle->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(circle);
                        db.geometry.add(circle);
                    }
                    else if(e->type() == curve_entity)
                    {
                        Point lp0;
                        Point lp1;

                        // add dummy inter points at 0.0 and 1.0
                        PointE *vp = new PointE(((Curve*)e)->position(0.0));
                        IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=0.0;
                        ipitem->t2=0.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        vp = new PointE(((Curve*)e)->position(1.0));
                        ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=1.0;
                        ipitem->t2=1.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);

                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                        double t0 = ((Curve*)e)->gett(lp0);
                        double t1 = ((Curve*)e)->gett(lp1);
                        double disto0 = fabs(t0 - 0.0);
                        double disto1 = fabs(t1 - 1.0);
                        if (t0 < t1)
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            Curve *ecp = (Curve*)e->clone(ident);
                            // trim start to end
                            ((Curve *)ecp)->moveendpoint(1,1,lp0);
                            ((Curve *)ecp)->moveendpoint(1,2,lp1);
                            ((Curve *)ecp)->setcolour(rbHeader.getcolour());
                            ((Curve *)ecp)->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            //ecp->draw(DM_NORMAL);
                        }
                        else
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            Curve *ecp = (Curve*)e->clone(ident);
                            // trim end to start
                            ((Curve *)ecp)->moveendpoint(1,2,lp0);
                            ((Curve *)ecp)->moveendpoint(1,1,lp1);
                            ((Curve *)ecp)->setcolour(rbHeader.getcolour());
                            ((Curve *)ecp)->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            //ecp->draw(DM_NORMAL);
                        }
                    }
                    else if(e->type() == ellipse_entity)
                    {
                        //if(((EllipseE*)e)->getsweepa() > 1.99999 * M_PI)
                        {
                            // add dummy inter points at 0.0 and 1.0
                            PointE *vp = new PointE(((EllipseE*)e)->position(0.0));
                            IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                            ipitem->t1=0.0;
                            ipitem->t2=0.0;
                            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                            vp = new PointE(((EllipseE*)e)->position(1.0));
                            ipitem = new IntersectPointItem(vp,e,e);
                            ipitem->t1=1.0;
                            ipitem->t2=1.0;
                            rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        }

                        Point lp0;
                        Point lp1;
                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                        double t0 = ((EllipseE*)e)->gett(lp0);
                        double t1 = ((EllipseE*)e)->gett(lp1);
                        double disto0 = fabs(t0 - 0.0);
                        double disto1 = fabs(t1 - 1.0);
                        if (rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t0 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt < t1)
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                            ((EllipseE *)ecp)->setcolour(rbHeader.getcolour());
                            ((EllipseE *)ecp)->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            ecp->draw(DM_NORMAL);
                        }
                        else if(rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t1 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt <= 1.0)
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp1);
                            ((EllipseE *)ecp)->setcolour(rbHeader.getcolour());
                            ((EllipseE *)ecp)->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            ecp->draw(DM_NORMAL);
                        }
                        else
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                            ((EllipseE *)ecp)->setcolour(rbHeader.getcolour());
                            ((EllipseE *)ecp)->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            ecp->draw(DM_NORMAL);
                        }
                    }
                }
            }
        }
        else if (event->isa(Abort))
        {
#if 1
            // get the intersection points
            // for the first and last segment
            Entity *e1;
            uint j = rbdata->elist.at(rbdata->nloops).size();
            if(j > 1)
            {
                e = rbdata->elist.at(rbdata->nloops).at(j-1).getEnt();
                e1 = rbdata->elist.at(rbdata->nloops).at(0).getEnt();
                Intersect i(e,e1);
                qDebug() << "inters : " << i.nintersections();
                for(int k=0; k < i.nintersections(); k++)
                {
                    PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());
                    IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                    rbdata->elist.at(rbdata->nloops).at(j-1).addIntPointItem(ipitem);
                    rbdata->elist.at(rbdata->nloops).at(0).addIntPointItem(ipitem);

                    db.geometry.add(ip);
                    ip->draw(DM_NORMAL);
                }
            }
            // clear the intersections
            for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
            {
                rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.erase(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.begin(),rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.end());
            }
            // clear the trim segments
            for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
            {
                if(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt() != 0)
                    db.geometry.del(rbdata->elist.at(rbdata->nloops).at(j).getTrimEnt());
                rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(nullptr);
            }
            // get the intersection points
            if(rbdata->elist.at(rbdata->nloops).size() > 1)
            {
                for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
                {
                    if(j < rbdata->elist.at(rbdata->nloops).size()-1)
                    {
                        e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                        e1 = rbdata->elist.at(rbdata->nloops).at(j+1).getEnt();
                    }
                    else
                    {
                        e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                        e1 = rbdata->elist.at(rbdata->nloops).at(0).getEnt();
                    }
                    Intersect i(e,e1);
                    qDebug() << "inters : " << i.nintersections();
                    if(i.nintersections() != 0)
                    {
                        for(int k=0; k < i.nintersections(); k++)
                        {
                            PointE *ip = new PointE(((PointE *)i.intersection(k))->getp());
                            IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                            if(j < rbdata->elist.at(rbdata->nloops).size()-1)
                            {
                                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                                rbdata->elist.at(rbdata->nloops).at(j+1).addIntPointItem(ipitem);
                            }
                            else
                            {
                                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                                rbdata->elist.at(rbdata->nloops).at(0).addIntPointItem(ipitem);
                            }
                            db.geometry.add(ip);
                            ip->draw(DM_NORMAL);
                        }
                    }
                    else
                    {
                        PointE *ip = nullptr;
                        // no intersection get the closest end points
                        if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                            ip = new PointE(((LinearEntity*)e)->start());
                        else if((((LinearEntity*)e)->start() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                            ip = new PointE(((LinearEntity*)e)->start());
                        else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->start()).length() < db.getptoler())
                            ip = new PointE(((LinearEntity*)e)->end());
                        else if((((LinearEntity*)e)->end() - ((LinearEntity*)e1)->end()).length() < db.getptoler())
                            ip = new PointE(((LinearEntity*)e)->end());
                        if(ip != nullptr)
                        {
                            IntersectPointItem *ipitem = new IntersectPointItem(ip,e,e1);

                            if(j < rbdata->elist.at(rbdata->nloops).size()-1)
                            {
                                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                                rbdata->elist.at(rbdata->nloops).at(j+1).addIntPointItem(ipitem);
                            }
                            else
                            {
                                rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                                rbdata->elist.at(rbdata->nloops).at(0).addIntPointItem(ipitem);
                            }

                            db.geometry.add(ip);
                            ip->draw(DM_NORMAL);
                        }
                        // no connection put up a message?
                    }
                }
            }
            // create the plane from the selected entities
            for (uint j=0; j < rbdata->elist.at(rbdata->nloops).size(); j++)
            {
                e = rbdata->elist.at(rbdata->nloops).at(j).getEnt();
                if(e->type() == line_entity)
                {
                    Point lp0;
                    Point lp1;
                    rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                    Line *line = new Line(rbHeader,lp0,lp1);
                    rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(line);
                    db.geometry.add(line);
                    //line->draw(DM_NORMAL);
                }
                else if(e->type() == circle_entity)
                {
                    // special case for a complete circle

                    if(((Circle*)e)->getsweepa() > 1.99999 * M_PI)
                    {
                        // add dummy inter points at 0.0 and 1.0
                        PointE *vp = new PointE(((Circle*)e)->position(0.0));
                        IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=0.0;
                        ipitem->t2=0.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        vp = new PointE(((Circle*)e)->position(1.0));
                        ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=1.0;
                        ipitem->t2=1.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                    }

                    Point lp0;
                    Point lp1 = rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mPickPoint;
                    Point lp2;

                    if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
                       rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
                       rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
                    {
                        Transform ident;
                        ident.identity();
                        e->setcopye(0);
                        Circle *ecp = (Circle*)e->clone(ident);
                        ecp->setcolour(rbHeader.getcolour());
                        ecp->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                        db.geometry.add(ecp);
                        //ecp->draw(DM_NORMAL);
                        e->setcopye(0);
                    }
                    else
                    {
                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp2);
                        Circle *circle = new Circle(1,lp0,lp1,lp2);
                        circle->setcolour(rbHeader.getcolour());
                        circle->setweight(rbHeader.getweight());
                        circle->setcolour(rbHeader.getcolour());
                        circle->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(circle);
                        db.geometry.add(circle);
                        //circle->draw(DM_NORMAL);
                    }

                }
                else if(e->type() == curve_entity)
                {
                    Point lp0;
                    Point lp1;
                    rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                    double t0 = ((Curve*)e)->gett(lp0);
                    double t1 = ((Curve*)e)->gett(lp1);
                    double disto0 = fabs(t0 - 0.0);
                    double disto1 = fabs(t1 - 1.0);
                    if (t0 < t1)
                    {
                        Transform ident;
                        ident.identity();
                        e->setcopye(0);
                        Curve *ecp = (Curve*)e->clone(ident);
                        // trim start to end
                        ((Curve *)ecp)->moveendpoint(1,1,lp0);
                        ((Curve *)ecp)->moveendpoint(1,2,lp1);
                        ecp->setcolour(rbHeader.getcolour());
                        ecp->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                        db.geometry.add(ecp);
                        e->setcopye(0);
                    }
                    else
                    {
                        Transform ident;
                        ident.identity();
                        e->setcopye(0);
                        Curve *ecp = (Curve*)e->clone(ident);
                        // trim end to start
                        ((Curve *)ecp)->moveendpoint(1,2,lp0);
                        ((Curve *)ecp)->moveendpoint(1,1,lp1);
                        ecp->setcolour(rbHeader.getcolour());
                        ecp->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                        db.geometry.add(ecp);
                        e->setcopye(0);
                    }
                }
                else if(e->type() == ellipse_entity)
                {
                    if(((EllipseE*)e)->getsweepa() > 1.99999 * M_PI)
                    {
                        // add dummy inter points at 0.0 and 1.0
                        PointE *vp = new PointE(((EllipseE*)e)->position(0.0));
                        IntersectPointItem *ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=0.0;
                        ipitem->t2=0.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                        vp = new PointE(((EllipseE*)e)->position(1.0));
                        ipitem = new IntersectPointItem(vp,e,e);
                        ipitem->t1=1.0;
                        ipitem->t2=1.0;
                        rbdata->elist.at(rbdata->nloops).at(j).addIntPointItem(ipitem);
                    }

                    if(rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.size() == 2 &&
                       rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(0)->t1 == 0.0 &&
                       rbdata->elist.at(rbdata->nloops).at(j).mIntPointItems.at(1)->t1 == 1.0 )
                    {
                        Transform ident;
                        ident.identity();
                        e->setcopye(0);
                        EllipseE *ecp = (EllipseE*)e->clone(ident);
                        ecp->setcolour(rbHeader.getcolour());
                        ecp->setweight(rbHeader.getweight());
                        rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                        db.geometry.add(ecp);
                        //ecp->draw(DM_NORMAL);
                        e->setcopye(0);
                    }
                    else
                    {
                        Point lp0;
                        Point lp1;
                        rbdata->elist.at(rbdata->nloops).at(j).getClosestInters(lp0,lp1);
                        double t0 = ((EllipseE*)e)->gett(lp0);
                        double t1 = ((EllipseE*)e)->gett(lp1);
                        double disto0 = fabs(t0 - 0.0);
                        double disto1 = fabs(t1 - 1.0);
                        if (rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t0 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt < t1)
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                            ecp->setcolour(rbHeader.getcolour());
                            ecp->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            //ecp->draw(DM_NORMAL);
                        }
                        else if(rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt > t1 && rbdata->elist.at(rbdata->nloops).at(j).mPickPointI.mt <= 1.0)
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp1);
                            ecp->setcolour(rbHeader.getcolour());
                            ecp->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            //ecp->draw(DM_NORMAL);
                        }
                        else
                        {
                            Transform ident;
                            ident.identity();
                            e->setcopye(0);
                            EllipseE *ecp = (EllipseE*)e->clone(ident);
                            //
                            ((EllipseE *)ecp)->moveendpoint(1,1,lp0);
                            ((EllipseE *)ecp)->moveendpoint(1,2,lp1);
                            ecp->setcolour(rbHeader.getcolour());
                            ecp->setweight(rbHeader.getweight());
                            rbdata->elist.at(rbdata->nloops).at(j).setTrimEnt(ecp);
                            e->setcopye(0);
                            db.geometry.add(ecp);
                            //ecp->draw(DM_NORMAL);
                        }
                    }
                }
            }
#endif
            rbdata->nloops++;
            rbdata->elist.push_back(PickEntItemList());
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {
            *cstate = ExitState;
        }
        break;
    }
//
//  About to enter state actions
//
    if (*cstate == 1 || *cstate == 2)
      {  rubberband.begin(0);
         rubberband.add(RBPlane13,rbdata);
         state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
         if (rbdata->elist.size() == 0)
           cadwindow->prompt->normalprompt("Locate the first plane boundary entity");
         else
           cadwindow->prompt->normalprompt("Locate the plane boundary entity");
      }
    else if (*cstate == ExitState)
    {
        if(rbdata != 0)
            rbdata->elist.clear();
      delete rbdata;
    }
#endif
}
