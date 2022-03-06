
#include "ncwin.h"

//#if MENUS != STUDENTMENUS

struct RBBitMap1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qlines[4];
#endif
    RCCHAR name[300];
   BitMask options;
   double w,h,tr,tg,tb,trange,op;
   Point xaxis,yaxis,offset;
   RBBitMap1_data() : options(32) {}
};

#ifdef NEW_RUBBERB
void RBBitMap1(int dmode ,void *data,Point *p2,View3dSurface *surface)
{
 RBBitMap1_data *rbdata = (RBBitMap1_data *) data;
 Point p1;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (surface != 0)
  {
      rbdata->xaxis = surface->getxaxis() * rbdata->w;
      rbdata->yaxis = surface->getyaxis() * rbdata->h;

      rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) / 2.0) -
                        rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) / 2.0);
  }

  p1 = *p2 + rbdata->offset;

  Line line1(p1,p1+rbdata->xaxis);
  //line1.qgi = rbdata->qlines[0]; line1.zValue = rbdata->zValue;
  line1.draw(DM_INVERT);
  //rbdata->qlines[0] = line1.qgi; rbdata->zValue = line1.zValue;

  Line line2(p1+rbdata->xaxis,p1+rbdata->xaxis+rbdata->yaxis);
  //line2.qgi = rbdata->qlines[1]; line2.zValue = rbdata->zValue;
  line2.draw(DM_INVERT);
  //rbdata->qlines[1] = line2.qgi;rbdata->zValue = line2.zValue;

  Line line3(p1+rbdata->xaxis+rbdata->yaxis,p1+rbdata->yaxis);
  //line3.qgi = rbdata->qlines[2]; line3.zValue = rbdata->zValue;
  line3.draw(DM_INVERT);
  //rbdata->qlines[2] = line3.qgi;rbdata->zValue = line3.zValue;

  Line line4(p1+rbdata->yaxis,p1);
  //line4.qgi = rbdata->qlines[3];line4.zValue = rbdata->zValue;
  line4.draw(DM_INVERT);
  //rbdata->qlines[3] = line4.qgi;rbdata->zValue = line4.zValue;
}
#else
void RBBitMap1(int,void *data,Point *p2,View3dSurface *surface)
{
 RBBitMap1_data *rbdata = (RBBitMap1_data *) data;
 Point p1;
  if (surface != 0)
    {  if (rbdata->options.test(9))
         {  rbdata->xaxis = surface->getxaxis() * rbdata->w;
            rbdata->yaxis = surface->getyaxis() * rbdata->h;
         }
       else
         {  rbdata->xaxis = surface->getuaxis() * rbdata->w;
            rbdata->yaxis = surface->getvaxis() * rbdata->h;
         }

       rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) / 2.0) -
                         rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) / 2.0);
    }
  p1 = *p2 + rbdata->offset;

  Line line1(p1,p1+rbdata->xaxis);
  line1.qgi = rbdata->qlines[0]; line1.zValue = rbdata->zValue;
  line1.draw(DM_INVERT);
  rbdata->qlines[0] = line1.qgi; rbdata->zValue = line1.zValue;

  Line line2(p1+rbdata->xaxis,p1+rbdata->xaxis+rbdata->yaxis);
  line2.qgi = rbdata->qlines[1]; line2.zValue = rbdata->zValue;
  line2.draw(DM_INVERT);
  rbdata->qlines[1] = line2.qgi;rbdata->zValue = line2.zValue;

  Line line3(p1+rbdata->xaxis+rbdata->yaxis,p1+rbdata->yaxis);
  line3.qgi = rbdata->qlines[2]; line3.zValue = rbdata->zValue;
  line3.draw(DM_INVERT);
  rbdata->qlines[2] = line3.qgi;rbdata->zValue = line3.zValue;

  Line line4(p1+rbdata->yaxis,p1);
  line4.qgi = rbdata->qlines[3];line4.zValue = rbdata->zValue;
  line4.draw(DM_INVERT);
  rbdata->qlines[3] = line4.qgi;rbdata->zValue = line4.zValue;
}
#endif

void bitmap_command0(int *cstate,HighLevelEvent *event,void **data)
{
    cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Image"));
   *cstate = ExitState;
}

void bitmap_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBBitMap1_data *rbdata = (RBBitMap1_data *) *data;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 long w,h;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(428,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBBitMap1_data;  *data = rbdata;
#ifdef USING_WIDGETS
        for(int i=0; i<4; i++)
            rbdata->qlines[i]=0;
        rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         if (rbdata != 0 && BitMapE::select(rbdata->name))
           {  BitMapE::getdefaults(&rbdata->w,&rbdata->h,&rbdata->tr,&rbdata->tg,&rbdata->tb,&rbdata->trange,&rbdata->op,&rbdata->options);
              if (rbdata->options.test(0) && BitMapE::getshape(rbdata->name,&w,&h))
                rbdata->w = rbdata->h * double(w) / double(h);
              *cstate = 1;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  BitMapE *bitmap = new BitMapE(rbdata->name,((Coordinate3dEvent *)event)->getp(),rbdata->xaxis.normalize(),rbdata->yaxis.normalize(),rbdata->w,rbdata->h,rbdata->tr,rbdata->tg,rbdata->tb,rbdata->trange,rbdata->op,rbdata->options);
              if (bitmap != 0 && bitmap->getdefined())
                {
                  bitmap->zValue=rbdata->zValue;
                  if (! cadwindow->getcurrentsurface()->requiresopengl() && rbdata->options.test(9))
                     {
                        db.geometry.add(bitmap);
                        cadwindow->paintall();   //  Create the OpenGL Surfaces
                     }
                   else
                     db.geometry.add(bitmap);
                }
              else
                delete bitmap;
              if (EndScene != 0)
                EndScene();  //  Flush the graphics buffer
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
        for(int i=0; i<4; i++)
        {
            if(rbdata->qlines[i]) cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qlines[i]);
            delete rbdata->qlines[i];rbdata->qlines[i]=0;
        }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBBitMap1,rbdata);
       cadwindow->prompt->normalprompt(NCBITMAP + 9);
    }
  else
    delete rbdata;
}

struct RBBitMap2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qlines[4];
#endif
   RCCHAR name[300];
   BitMask options;
   double w,h,bw,bh,tr,tg,tb,trange,op,ratio;
   Point p1,xaxis,yaxis,offset;
   int state;
   RBBitMap2_data() : options(32) {}
};

#ifdef NEW_RUBBERB
void RBBitMap2(int dmode,void *data,Point *p2,View3dSurface *surface)
{RBBitMap2_data *rbdata = (RBBitMap2_data *) data;
 Point p1;
 double w,h;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (surface != 0)
    {  if (rbdata->state == 1)
         {  rbdata->p1 = *p2;

            if (rbdata->options.test(0))
              rbdata->bw = fabs(rbdata->bh) * rbdata->ratio * (rbdata->bw < 0.0 ? -1.0 : 1.0);

            //if (rbdata->options.test(9))
            //{
            //     rbdata->xaxis = surface->getxaxis() * rbdata->bw;
            //     rbdata->yaxis = surface->getyaxis() * rbdata->bh;
            //}
            //else
            //{
                 rbdata->xaxis = surface->getuaxis() * rbdata->bw;
                 rbdata->yaxis = surface->getvaxis() * rbdata->bh;
            //}

            rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) / 2.0) -
                              rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) / 2.0);
         }
       else
         {
            //if (rbdata->options.test(9))
            //{
            //     rbdata->xaxis = surface->getxaxis();
            //     rbdata->yaxis = surface->getyaxis();
            //}
            //else
            //{
                 rbdata->xaxis = surface->getuaxis();
                 rbdata->yaxis = surface->getvaxis();
            //}

            rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) * rbdata->w / 2.0) -
                              rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) * rbdata->h / 2.0);
            w = ((*p2 - rbdata->p1).dot(rbdata->xaxis));
            h = ((*p2 - rbdata->p1).dot(rbdata->yaxis));
            if (rbdata->options.test(0))
              w = fabs(h) * rbdata->ratio * (w < 0.0 ? -1.0 : 1.0);
            rbdata->xaxis *= w;
            rbdata->yaxis *= h;
         }
    }

  Line line1(rbdata->p1,rbdata->p1+rbdata->xaxis);
  //line1.qgi = rbdata->qlines[0]; line1.zValue = rbdata->zValue;
    line1.draw(DM_INVERT);
  //rbdata->qlines[0] = line1.qgi; rbdata->zValue = line1.zValue;

  Line line2(rbdata->p1+rbdata->xaxis,rbdata->p1+rbdata->xaxis+rbdata->yaxis);
  //line2.qgi = rbdata->qlines[1];line2.zValue = rbdata->zValue;
  line2.draw(DM_INVERT);
  //rbdata->qlines[1] = line2.qgi;rbdata->zValue = line2.zValue;

  Line line3(rbdata->p1+rbdata->xaxis+rbdata->yaxis,rbdata->p1+rbdata->yaxis);
  //line3.qgi = rbdata->qlines[2];line3.zValue = rbdata->zValue;
  line3.draw(DM_INVERT);
  //rbdata->qlines[2] = line3.qgi;rbdata->zValue = line3.zValue;

  Line line4(rbdata->p1+rbdata->yaxis,rbdata->p1);
  //line4.qgi = rbdata->qlines[3];line4.zValue = rbdata->zValue;
  line4.draw(DM_INVERT);
  //rbdata->qlines[3] = line4.qgi;rbdata->zValue = line4.zValue;
}
#else
void RBBitMap2(int,void *data,Point *p2,View3dSurface *surface)
{RBBitMap2_data *rbdata = (RBBitMap2_data *) data;
 Point p1;
 double w,h;
  if (surface != 0)
    {  if (rbdata->state == 1)
         {  rbdata->p1 = *p2;

            if (rbdata->options.test(0))
              rbdata->bw = fabs(rbdata->bh) * rbdata->ratio * (rbdata->bw < 0.0 ? -1.0 : 1.0);

            if (rbdata->options.test(9))
              {  rbdata->xaxis = surface->getxaxis() * rbdata->bw;
                 rbdata->yaxis = surface->getyaxis() * rbdata->bh;
              }
            else
              {  rbdata->xaxis = surface->getuaxis() * rbdata->bw;
                 rbdata->yaxis = surface->getvaxis() * rbdata->bh;
              }


            rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) / 2.0) -
                              rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) / 2.0);
         }
       else
         {  if (rbdata->options.test(9))
              {  rbdata->xaxis = surface->getxaxis();
                 rbdata->yaxis = surface->getyaxis();
              }
            else
              {  rbdata->xaxis = surface->getuaxis();
                 rbdata->yaxis = surface->getvaxis();
              }

            rbdata->offset = -rbdata->xaxis * ((rbdata->options.test(2)*2+rbdata->options.test(1)) * rbdata->w / 2.0) -
                              rbdata->yaxis * ((rbdata->options.test(4)*2+rbdata->options.test(3)) * rbdata->h / 2.0);
            w = ((*p2 - rbdata->p1).dot(rbdata->xaxis));
            h = ((*p2 - rbdata->p1).dot(rbdata->yaxis));
            if (rbdata->options.test(0))
              w = fabs(h) * rbdata->ratio * (w < 0.0 ? -1.0 : 1.0);
            rbdata->xaxis *= w;
            rbdata->yaxis *= h;
         }
    }

  Line line1(rbdata->p1,rbdata->p1+rbdata->xaxis);
  line1.qgi = rbdata->qlines[0]; line1.zValue = rbdata->zValue;
    line1.draw(DM_INVERT);
  rbdata->qlines[0] = line1.qgi; rbdata->zValue = line1.zValue;

  Line line2(rbdata->p1+rbdata->xaxis,rbdata->p1+rbdata->xaxis+rbdata->yaxis);
  line2.qgi = rbdata->qlines[1];line2.zValue = rbdata->zValue;
  line2.draw(DM_INVERT);
  rbdata->qlines[1] = line2.qgi;rbdata->zValue = line2.zValue;

  Line line3(rbdata->p1+rbdata->xaxis+rbdata->yaxis,rbdata->p1+rbdata->yaxis);
  line3.qgi = rbdata->qlines[2];line3.zValue = rbdata->zValue;
  line3.draw(DM_INVERT);
  rbdata->qlines[2] = line3.qgi;rbdata->zValue = line3.zValue;

  Line line4(rbdata->p1+rbdata->yaxis,rbdata->p1);
  line4.qgi = rbdata->qlines[3];line4.zValue = rbdata->zValue;
  line4.draw(DM_INVERT);
  rbdata->qlines[3] = line4.qgi;rbdata->zValue = line4.zValue;
}
#endif

void bitmap_command2(int *cstate,HighLevelEvent *event,void **data)
{RBBitMap2_data *rbdata = (RBBitMap2_data *) *data;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 long w,h;
 double width,height;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(428,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBBitMap2_data;
         if (rbdata != 0 && BitMapE::select(rbdata->name))
           {  BitMapE::getdefaults(&rbdata->bw,&rbdata->bh,&rbdata->tr,&rbdata->tg,&rbdata->tb,&rbdata->trange,&rbdata->op,&rbdata->options);
              rbdata->w = rbdata->bw;  rbdata->h = rbdata->bh;
              rbdata->options.set(1,0);  rbdata->options.set(2,0);  rbdata->options.set(3,0);  rbdata->options.set(4,0);
              if (rbdata->options.test(0) && BitMapE::getshape(rbdata->name,&w,&h))
                rbdata->ratio = double(w) / double(h);
              *cstate = 1;
           }
         else
           *cstate = ExitState;
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  xaxis = rbdata->xaxis.normalize();
              yaxis = rbdata->yaxis.normalize();
              width = (((Coordinate3dEvent *)event)->getp() - rbdata->p1).dot(xaxis);
              height = (((Coordinate3dEvent *)event)->getp() - rbdata->p1).dot(yaxis);
              if (rbdata->options.test(0))
                width = fabs(height) * rbdata->ratio * (width < 0.0 ? -1.0 : 1.0); 
              if (width < 0.0)
                rbdata->p1 = rbdata->p1 + xaxis * width;
              if (height < 0.0)
                rbdata->p1 = rbdata->p1 + yaxis * height;
              BitMapE *bitmap = new BitMapE(rbdata->name,rbdata->p1,rbdata->xaxis.normalize(),rbdata->yaxis.normalize(),fabs(width),fabs(height),rbdata->tr,rbdata->tg,rbdata->tb,rbdata->trange,rbdata->op,rbdata->options);
              if (bitmap != 0 && bitmap->getdefined())
                {
                  bitmap->zValue=rbdata->zValue;
                  if (! cadwindow->getcurrentsurface()->requiresopengl() && rbdata->options.test(9))
                     {  db.geometry.add(bitmap);
                        cadwindow->paintall();   //  Create the OpenGL Surfaces
                     }
                   else
                     db.geometry.add(bitmap);
                }
              else
                delete bitmap;
              if (EndScene != 0)
                EndScene();  //  Flush the graphics buffer
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCBITMAP + 9 + *cstate);
       rubberband.begin(0);
       rubberband.add(RBBitMap2,rbdata);
    }
  else
    delete rbdata;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief getBitmapChangeValues
 * @param change
 * @param list
 * @param hd
 * @param f
 * @param w
 * @param h
 * @param vs
 * @param a
 * @param hc
 * @param t
 * @param l
 * @param o
 *
 * looks for BitMapE entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getBitmapChangeValues(BitMask *change,EntityList &list,EntityHeader *hd,double *w,double *h,double *tr,double *tg,double *tb,double *trange,double *op,BitMask *o,RCCHAR **fn,Point *org)
{
    Entity *e;
    //Point origin,torigin;
    //double height,width,transr,transg,transb,transrange,opacity;
    //double theight,twidth,ttransr,ttransg,ttransb,ttransrange,topacity;
    BitMask options(32),toptions(32);
    //EntityHeader header,theader;
    BitMapE *bitmape;

    // values to be compared

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(bitmap_entity))
        {
            bitmape = (BitMapE*)e;

            if(i==0)
            {
                hd->setcolour(bitmape->getcolour());
                hd->setlayer(bitmape->getlayer());
                hd->setstyle(bitmape->getstyle());
                hd->setweight(bitmape->getweight());

                *w = bitmape->getwidth();
                *h = bitmape->getheight();
                *tr = bitmape->getred();
                *tg = bitmape->getgreen();
                *tb = bitmape->getblue();
                *trange = bitmape->gettransparencyrange();
                *op = bitmape->getopacity();
                *fn = bitmape->getfilename();
                *org = bitmape->getorigin();
                options = bitmape->getoptions();
                *o = bitmape->getoptions();

                change->clearall();

                change->set(0);// def color
                change->set(1);// def layer
                change->set(2);// def style
                change->set(3);// def line weight

                change->set(4);// original shape
                change->set(5);// link to file
                change->set(6);// width
                change->set(7);// height
                change->set(8);// horiz just
                change->set(9);// vert just
                change->set(10);// frame style
                change->set(11);// transparent flag
                change->set(12);// tr
                change->set(13);// tg
                change->set(14);// tb
                change->set(15);// filename
                change->set(16);// trange
                change->set(17);// opacity
                //change->set(18);// ???
                // KMJ added
                change->clear(19); // origin change (future use??)

            }
            else
            {
                toptions = bitmape->getoptions();

                if(hd->getcolour() != bitmape->getcolour())
                    change->clear(0);
                if(hd->getlayer() != bitmape->getlayer())
                    change->clear(1);
                if(hd->getstyle() != bitmape->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),bitmape->getweight()))
                    change->clear(3);

                if(options.test(0) != toptions.test(0))
                    change->clear(4);// original shape
                if(options.test(BitmapELinkFile) != toptions.test(BitmapELinkFile))
                    change->clear(5);// link to file
                if(fabs(*w - bitmape->getwidth()) > 10.0e-5)
                    change->clear(6);// width
                if(fabs(*h - bitmape->getheight()) > 10.0e-5)
                    change->clear(7);// height
                // horiz just
                if((options.test(1) != toptions.test(1)) || (options.test(2) != toptions.test(2)))
                    change->clear(8);
                // vert just
                if((options.test(3) != toptions.test(3)) || (options.test(4) != toptions.test(4)))
                    change->clear(9);
                // frame style
                if((options.test(5) != toptions.test(5)) || (options.test(6) != toptions.test(6))
                   || (options.test(7) != toptions.test(7)))
                    change->clear(10);
                if(options.test(BitmapETransparent) != toptions.test(BitmapETransparent))
                    change->clear(11); // transparent flag
                if(fabs(*tr - bitmape->getred()) > 10.0e-5)
                    change->clear(12);// trans red
                if(fabs(*tg - bitmape->getgreen()) > 10.0e-5)
                    change->clear(13);// trans green
                if(fabs(*tb - bitmape->getblue()) > 10.0e-5)
                    change->clear(14);// trans blue
                if(strcmp(*fn,bitmape->getfilename()) != 0)
                    change->clear(15);
                if(fabs(*trange - bitmape->gettransparencyrange()) > 10.0e-5)
                    change->clear(16);// trans range
                if(fabs(*op - bitmape->getopacity()) > 10.0e-5)
                    change->clear(17);// opacity

            }
            i++;
        }
    }
    return i;
}

void bitmap_command3(int *cstate,HighLevelEvent *event,void **)
{
 static EntityList list;
 double width,height,tr,tg,tb,trange,op;
 BitMask options(32),change(32);
 Entity *e;
 EntityHeader header;
 int status,length;
 BitMapE *bitmap;
 RCCHAR *fn,fname[300];
 Point origin;

  v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
  {
       list = state.getselection().copy();
       if (list.empty())
       {
            BitMapE::getdefaults(&width,&height,&tr,&tg,&tb,&trange,&op,&options);
            status = BitMapE::setup(0,&width,&height,&tr,&tg,&tb,&trange,&op,&options);
            if (status == TRUE)
            {
                BitMapE::setdefaults(width,height,tr,tg,tb,trange,op,options);
                 *cstate = ExitState;
            }
            else if (status == 119)
              *cstate = 1; // clone button
            else if (status == 120)
              *cstate = 3; // clone all button
            else
              *cstate = ExitState;
       }
       else
       {
            BitMapE::getdefaults(&width,&height,&tr,&tg,&tb,&trange,&op,&options);
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found = getBitmapChangeValues(&change,list,&header,&width,&height,&tr,&tg,&tb,&trange,&op,&options,&fn,&origin);
            RCSTRCPY(fname,fn);
            v.setinteger("op::changecount",found);
            if(found == 1)
                status = BitMapE::setup(&change,&width,&height,&tr,&tg,&tb,&trange,&op,&options,fname,&origin,&header);
            else if(found > 1)
                status = BitMapE::setup(&change,&width,&height,&tr,&tg,&tb,&trange,&op,&options,fname,&origin,&header);
            else
                status = BitMapE::setup(0,&width,&height,&tr,&tg,&tb,&trange,&op,&options);
            if (status == TRUE)
            {
                length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(bitmap_entity))
                     ((BitMapE *)e)->change(change,header,0,width,height,tr,tg,tb,trange,op,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();
                 *cstate = ExitState;
            }
            else if (status == 119)
              *cstate = 2;
            else if (status == 120)
              *cstate = 4;
            else 
              *cstate = ExitState;
       }
  }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
  {
      if (event->isa(EntitySelected))
      {
          state.destroyselection(1);
            bitmap = (BitMapE *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
            {
                BitMapE::setdefaults(bitmap->getwidth(),bitmap->getheight(),bitmap->getred(),bitmap->getgreen(),bitmap->getblue(),bitmap->gettransparencyrange(),bitmap->getopacity(),bitmap->getoptions());
                 if (*cstate == 3)
                 {
                     db.header.change(bitmap->getcolour(),bitmap->getlayer(),bitmap->getstyle(),bitmap->getweight());
                      v.setinteger("df::colour",bitmap->getcolour());
                      v.setinteger("df::layer",bitmap->getlayer());
                      v.setinteger("df::style",bitmap->getstyle());
                      v.setinteger("df::weight",bitmap->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                 }
            }
            else
            {
                if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 4 ; i <= 17 ; i++) change.set(i,1);
                 length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(bitmap_entity))
                     ((BitMapE *)e)->change(change,*bitmap,bitmap->getfilename(),bitmap->getwidth(),bitmap->getheight(),bitmap->getred(),bitmap->getgreen(),bitmap->getblue(),bitmap->gettransparencyrange(),bitmap->getopacity(),bitmap->getoptions());
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


  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
  {
       state.selmask.entity.clearandset(bitmap_entity,end_list);
       cadwindow->prompt->normalprompt("Select the bitmap entity to clone from");
  }

}

//#endif
