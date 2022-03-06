
#include "ncwin.h"

int HideImage::add(Polygon3d *polygon)
{ polygon->clip(surface);
  polygon->modeltoscreen(surface);
  polygon->setextents();
  return list.add(polygon);
}

HideImage::~HideImage()
{Polygon3d *polygon;
  for (list.start() ; (polygon = (Polygon3d *) list.next()) != NULL ; )
    delete polygon;
  list.destroy();
}

static int shadecallback(SurfaceEntity *,void *polygon,int,void *,void *cbdata)
{HideImage *hi = (HideImage *)cbdata;
 int status;
  if (! ((Polygon3d *)polygon)->getdefined()) return 1;
  status = hi->add((Polygon3d *)polygon);
  return status;
}

void HideImage::hide(Entity *e,Point p1,Point p2)
{Segments segs,segs1;
 Polygon3d *polygon;
 double t1,t2,tlast,l,l1,l2;
 Point p3,p4,p5,p6,p7,p8;
 int status;
 Entity *hidden;

 QGraphicsItem *qgline=0;

  if (transform == 0)
    {  p1 = surface->modeltoview(p1);  p2 = surface->modeltoview(p2);
       if (! surface->clip3d(&p1,&p2)) return;
       p1 = surface->viewtoscreen(p1);  p2 = surface->viewtoscreen(p2);
       segs.add(0.0);  segs.add(1.0);
       // Then clip to view stuff
       for (list.start() ; ! segs.empty() && (polygon = (Polygon3d *) list.next()) != NULL ; )
         if (! polygon->checkextents(p1,p2))
           {  status = polygon->hide(surface,p1,p2,&segs1);
              if (status == 0) return;
              if (status == 1) segs &= segs1;
              segs1.destroy();
           }

       tlast = 0.0;
       for (segs.start() ; ! segs.atend() ;)
         {  t1 = segs.next();
            if (segs.atend())
              break;
            t2 = segs.next();
            if (t1 - tlast > 0.00001 && (clear & 2) == 2)
              {  if (lastentity != e || ! lasthidden)
                   getsurface()->setup(e->getlinestylescale(),0.0,DM_NORMAL,e->getcolour(),2,e->getweight(),0);
                 p3 = p1 + (p2 - p1) * tlast;
                 p4 = p1 + (p2 - p1) * t1;

                 surface->moveto(p3.x,p3.y);
                 qgline = surface->lineto((QGraphicsItem*)0,p4.x,p4.y);
                 if(e->qgi && qgline)
                     ((QGraphicsItemGroup*)e->qgi)->addToGroup(qgline);

                 lasthidden = 1;
              }
            if (t2 - t1 > 0.00001)
              {  p3 = p1 + (p2 - p1) * t1;  p4 = p1 + (p2 - p1) * t2;
                 if (lastentity != e || lasthidden)
                   getsurface()->setup(e->getlinestylescale(),0.0,DM_NORMAL,e->getcolour(),e->getstyle(),e->getweight(),0);

                 surface->moveto(p3.x,p3.y);
                 qgline = surface->lineto((QGraphicsItem*)0,p4.x,p4.y);
                 if(e->qgi && qgline)
                     ((QGraphicsItemGroup*)e->qgi)->addToGroup(qgline);

                 lasthidden = 0;
              }
            tlast = t2;
         }

        if (1.0 - tlast > 0.00001 && (clear & 2) == 2)
          {  if (lastentity != e || ! lasthidden)
               getsurface()->setup(e->getlinestylescale(),0.0,DM_NORMAL,e->getcolour(),2,e->getweight(),0);
             p3 = p1 + (p2 - p1) * tlast;
             p4 = p1 + (p2 - p1) * 1.0;

             surface->moveto(p3.x,p3.y);
             qgline = surface->lineto((QGraphicsItem*)0,p4.x,p4.y);
             if(e->qgi && qgline)
                 ((QGraphicsItemGroup*)e->qgi)->addToGroup(qgline);

             lasthidden = 1;
          }
        lastentity = e;
    }
  else
    {  p3 = p1;  p4 = p2;
       p5 = p1 = surface->modeltoview(p1);  p6 = p2 = surface->modeltoview(p2);
       if (! surface->clip3d(&p1,&p2)) return;
       l = (p6 - p5).length();
       l1 = (p1 - p5).length();
       l2 = (p2 - p5).length();
       if (l1 > l2)
         {  t1 = l1;  l1 = l2;  l2 = t1;
         }
       p1 = surface->viewtoscreen(p1);  p2 = surface->viewtoscreen(p2);
       segs.add(0.0);  segs.add(1.0);
       // Then clip to view stuff
       for (list.start() ; ! segs.empty() && (polygon = (Polygon3d *) list.next()) != NULL ; )
         if (! polygon->checkextents(p1,p2))
           {  status = polygon->hide(surface,p1,p2,&segs1);
              if (status == 0) return;
              if (status == 1) segs &= segs1;
              segs1.destroy();
           }
       if (l > 1.0E-20 && l2 - l1 > 1.0E-20)
         {  p7 = p3 + (p4 - p3) * l1 / l;
            p8 = (p4 - p3) * l / (l2 - l1);
         }
       else
         {  p7 = p3;  p8 = p4 - p3;
         }
       for (segs.start() ; ! segs.atend() ;)
         {  t1 = segs.next();  if (segs.atend()) break;
            t2 = segs.next();
            p3 = transform->transform(p7 + p8 * t1);  p3.z = 0.0;
            p4 = transform->transform(p7 + p8 * t2);  p4.z = 0.0;
            hidden = new Line(*e,p3,p4);
            hidden->setstatus(1);
            hidden->setvisible(*visibility);
            db.geometry.add(hidden);
         }
    }
  segs.destroy();
  segs1.destroy();
}

#ifdef USING_WIDGETS
void HideImage::hideGL(Entity *e,Point p1,Point p2)
{Segments segs,segs1;
 Polygon3d *polygon;
 double t1,t2,tlast,l,l1,l2;
 Point p3,p4,p5,p6,p7,p8;
 int status;
 Entity *hidden;

 // kmj
 Point modelPt1 = p1;
 Point modelPt2 = p2;
 Point bfc1 = p1;
 Point bfc2 = p2;
 double act1 = 0.0;
 double act2 = 0.0;
 double tscale = 1.0;

  if (transform == 0)
    {  p1 = surface->modeltoview(p1);  p2 = surface->modeltoview(p2);
       //bfc1 = surface->viewtoscreen(p1);
       //bfc2 = surface->viewtoscreen(p2);
       if (! surface->clip3d(&p1,&p2)) return;
       p1 = surface->viewtoscreen(p1);  p2 = surface->viewtoscreen(p2);
       segs.add(0.0);  segs.add(1.0);
       // Then clip to view stuff
       for (list.start() ; ! segs.empty() && (polygon = (Polygon3d *) list.next()) != NULL ; )
         if (! polygon->checkextents(p1,p2))
           {  status = polygon->hide(surface,p1,p2,&segs1);
              if (status == 0) return;
              if (status == 1) segs &= segs1;
              segs1.destroy();
           }

       tlast = 0.0;        
       for (segs.start() ; ! segs.atend() ;)
         {  t1 = segs.next();  
            if (segs.atend()) 
              break;
            t2 = segs.next();
            if (t1 - tlast > 0.00001 && (clear & 2) == 2)
              {
                 int dmode = DM_NORMAL;//e->getselected() ? DM_SELECT : DM_NORMAL;
                 if (lastentity != e || ! lasthidden)
                   surface->setupGL(e->getlinestylescale(),0.0,dmode,e->getcolour(),2,e->getweight(),0);

                 RCCOLORREF pcol;
                 pcol = surface->getcolour();
                 SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                 GLfloat lwidth = surface->curPen.widthF();

                 p3 = p1 + (p2 - p1) * tlast;
                 p4 = p1 + (p2 - p1) * t1;

                 if(surface->getview()->getperspective())
                 {
                     Point pm1 = modelPt1 + (modelPt2 - modelPt1) * tlast;
                     Point pm2 = modelPt1 + (modelPt2 - modelPt1) * t1;
                     surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
                 }
                 else
                 {
                     Point pm1 = surface->screentomodel(p3);
                     Point pm2 = surface->screentomodel(p4);
                     surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
                 }
                 //surface->movetoGL(p3.x,p3.y);
                 //surface->linetoGL(p4.x,p4.y);

                 lasthidden = 1;
              }
            if (t2 - t1 > 0.00001)
              {

                 int dmode = DM_NORMAL;// e->getselected() ? DM_SELECT : DM_NORMAL;
                 if (lastentity != e || lasthidden)
                   surface->setupGL(e->getlinestylescale(),0.0,dmode,e->getcolour(),e->getstyle(),e->getweight(),0);

                 RCCOLORREF pcol;
                 pcol = surface->getcolour();
                 SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                 GLfloat lwidth = surface->curPen.widthF();

                 glLineWidth(surface->curGLWeight);

                 p3 = p1 + (p2 - p1) * t1;
                 p4 = p1 + (p2 - p1) * t2;


                 if(surface->getview()->getperspective())
                 {
                    Point pm1 = modelPt1 + (modelPt2 - modelPt1) * t1;
                    Point pm2 = modelPt1 + (modelPt2 - modelPt1) * t2;
                    surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
                 }
                 else
                 {
                    Point pm1 = getsurface()->screentomodel(p3);
                    Point pm2 = getsurface()->screentomodel(p4);
                    surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
                 }

                 //surface->movetoGL(p3.x,p3.y);
                 //surface->linetoGL(p4.x,p4.y);
                 //surface->line2dGL(p3.x,p3.y,p4.x,p4.y);

                 lasthidden = 0;
              } 
            tlast = t2;   
         }

        if (1.0 - tlast > 0.00001 && (clear & 2) == 2)
          {
            int dmode = DM_NORMAL;//e->getselected() ? DM_SELECT : DM_NORMAL;
            if (lastentity != e || ! lasthidden)
               surface->setupGL(e->getlinestylescale(),0.0,dmode,e->getcolour(),2,e->getweight(),0);

             RCCOLORREF pcol;
             pcol = surface->getcolour();
             SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
             GLfloat lwidth = surface->curPen.widthF();

             p3 = p1 + (p2 - p1) * tlast;
             p4 = p1 + (p2 - p1) * 1.0;


             if(surface->getview()->getperspective())
             {
                 Point pm1 = modelPt1 + (modelPt2 - modelPt1) * tlast;
                 Point pm2 = modelPt1 + (modelPt2 - modelPt1) * 1.0;
                 surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
             }
             else
             {
                 Point pm1 = getsurface()->screentomodel(p3);
                 Point pm2 = getsurface()->screentomodel(p4);
                 surface->lineGL(pm1.x,pm1.y,pm1.z,pm2.x,pm2.y,pm2.z);
             }

             //surface->movetoGL(p3.x,p3.y);
             //surface->linetoGL(p4.x,p4.y);

             lasthidden = 1;
          }
        lastentity = e;
    }
  else
    {  p3 = p1;  p4 = p2;
       p5 = p1 = surface->modeltoview(p1);  p6 = p2 = surface->modeltoview(p2);
       if (! surface->clip3d(&p1,&p2)) return;
       l = (p6 - p5).length();
       l1 = (p1 - p5).length();
       l2 = (p2 - p5).length();
       if (l1 > l2)
         {  t1 = l1;  l1 = l2;  l2 = t1;
         }
       p1 = surface->viewtoscreen(p1);  p2 = surface->viewtoscreen(p2);
       segs.add(0.0);  segs.add(1.0);
       // Then clip to view stuff
       for (list.start() ; ! segs.empty() && (polygon = (Polygon3d *) list.next()) != NULL ; )
         if (! polygon->checkextents(p1,p2))
           {
              status = polygon->hide(surface,p1,p2,&segs1);
              if (status == 0) return;
              if (status == 1) segs &= segs1;
              segs1.destroy();
           }
       if (l > 1.0E-20 && l2 - l1 > 1.0E-20)
         {  p7 = p3 + (p4 - p3) * l1 / l;
            p8 = (p4 - p3) * l / (l2 - l1);
         }
       else
         {  p7 = p3;  p8 = p4 - p3;
         }
       for (segs.start() ; ! segs.atend() ;)
         {  t1 = segs.next();  if (segs.atend()) break;
            t2 = segs.next();
            p3 = transform->transform(p7 + p8 * t1);  p3.z = 0.0;
            p4 = transform->transform(p7 + p8 * t2);  p4.z = 0.0;
            hidden = new Line(*e,p3,p4);
            hidden->setstatus(1);
            hidden->setvisible(*visibility);
            db.geometry.add(hidden);
         }
    }
  segs.destroy();
  segs1.destroy();
}
#endif

void HideImage::gettransform(Transform *t) const
{ if (transform == 0) return;
  t->mat(0,0) = transform->mat(0,0);  t->mat(0,1) = transform->mat(0,1);  t->mat(0,2) = transform->mat(0,2);  t->mat(0,3) = transform->mat(0,3);
  t->mat(1,0) = transform->mat(1,0);  t->mat(1,1) = transform->mat(1,1);  t->mat(1,2) = transform->mat(1,2);  t->mat(1,3) = transform->mat(1,3);
  t->mat(2,0) = transform->mat(2,0);  t->mat(2,1) = transform->mat(2,1);  t->mat(2,2) = transform->mat(2,2);  t->mat(2,3) = transform->mat(2,3);
  t->mat(3,0) = 0.0;  t->mat(3,1) = 0.0;  t->mat(3,2) = 0.0;  t->mat(3,3) = 1.0;
}

void HideImage::applytransform(const Transform &t)
{Transform tview,t1;
  if (transform == 0) return;
  gettransform(&tview);
  t1 = t;
  t1.apply(tview);
  settransform(t1);
}

void HideImage::settransform(Transform &t)
{ if (transform == 0) return;
  transform->mat(0,0) = t.mat(0,0);  transform->mat(0,1) = t.mat(0,1);  transform->mat(0,2) = t.mat(0,2);  transform->mat(0,3) = t.mat(0,3);
  transform->mat(1,0) = t.mat(1,0);  transform->mat(1,1) = t.mat(1,1);  transform->mat(1,2) = t.mat(1,2);  transform->mat(1,3) = t.mat(1,3);
  transform->mat(2,0) = t.mat(2,0);  transform->mat(2,1) = t.mat(2,1);  transform->mat(2,2) = t.mat(2,2);  transform->mat(2,3) = t.mat(2,3);
}

#ifdef USING_WIDGETS
void HideImage::hideGL(void)
{RECT rect;
 Entity *e;
 double error;
 int nhor,nver,stopped;
 BitMask options(32);

 /*
 if ((clear & 1) == 1)
 {
      db.clearScene(DM_ERASE,surface);
 }
 */

 program->setbusy(1);

 for (db.geometry.start(),stopped = 0 ; (e = db.geometry.next()) != NULL ; )
   if (e->issurface() && e->isvisible(surface))
     {SurfaceEntity *se = (SurfaceEntity *) e;
        error = (surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler();
        se->nfacets(error,&nhor,&nver);
        se->facet(options,error,nhor,nver,surface,shadecallback,this);
        //if ((stopped = program->getinterrupt()) != 0) break;
     }
 if (! stopped)
   db.geometry.hideGL(this);
 program->setbusy(0);
}
#endif

void HideImage::hide(void)
{RECT rect;
 Entity *e;
 double error;
 int nhor,nver,stopped;
 BitMask options(32);


 if ((clear & 1) == 1)
 {
      db.clearScene(DM_ERASE,surface);
 }

 //program->setbusy(1);
 for (db.geometry.start(),stopped = 0 ; (e = db.geometry.next()) != NULL ; )
   if (e->issurface() && e->isvisible(surface))
     {SurfaceEntity *se = (SurfaceEntity *) e;
        error = (surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler();
        se->nfacets(error,&nhor,&nver);
        se->facet(options,error,nhor,nver,surface,shadecallback,this);
        if ((stopped = program->getinterrupt()) != 0) break;
     }
 if (! stopped) 
   db.geometry.hide(this);
 //program->setbusy(0);
}

