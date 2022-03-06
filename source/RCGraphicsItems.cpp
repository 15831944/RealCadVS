

#include "ncwin.h"
#include "ncent.h"
#include "ncmatrix.h"
#include "RCView.h"
#include "RCGraphicsItems.h"


/**
 * @brief RCGraphicsPlaneItem::RCGraphicsPlaneItem
 * @param mplane
 * @param parent
 */
RCGraphicsPlaneItem::RCGraphicsPlaneItem(Plane *mplane, const Transform &transf, View3dSurface *msurface, bool clipto, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    // init our stuff here
    rcplane = mplane;
    rcbitmap = 0;
    qpattern = 0;
    rcsurface = msurface;
    transform = transf;
    qedges=0;
    rcdrawmode = DM_NORMAL;

    if(clipto)
        setFlags(QGraphicsItem::ItemClipsToShape | QGraphicsItem::ItemClipsChildrenToShape);
}

RCGraphicsPlaneItem::~RCGraphicsPlaneItem()
{
    // delete our stuff here
    rcplane = 0;
    rcbitmap = 0;
    rcsurface = 0;
    qpattern = 0;
    qedges=0;
}

QRectF RCGraphicsPlaneItem::boundingRect(void) const
{
    if(qpath.elementCount() > 0)
        return qpath.boundingRect();
    else
        return QRectF();
}

QPainterPath RCGraphicsPlaneItem::shape(void) const
{
    return qpath;
}

void RCGraphicsPlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(qpath);
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);

    qDebug() << "paint - paint engine type : " << painter->paintEngine()->type();

    if(painter->paintEngine()->type() != QPaintEngine::OpenGL2)
        return;

    // opengl drawing from here
    if(rcplane == 0) // check for a model plane
        return;

    if(rcsurface == 0)
        rcsurface = cadwindow->getcurrentsurface();

    if(rcsurface == 0)
        return;

    painter->beginNativePainting();

    // OpenGL shading mode
    if(cadwindow->getcurrentsurface() !=0 && cadwindow->getcurrentsurface()->getrepaintstyle() == Shaded)
    //if(rcsurface !=0 && rcsurface->getrepaintstyle() == Shaded)
    {
        View *view;
        Entity *e;
        int ntriangles;
        Point *triangles;
        SurfaceProperties *sp;
        double ambient,diffuse;
        BitMask foptions(32);
        Point eye,pmin,pmax;
        int i,k,j,t,nlights,y;

        ambient = v.getreal("sh::ambient");
        diffuse = v.getreal("sh::diffuse");

        view = rcsurface->getview();

        db.geometry.extents(rcsurface,&pmin,&pmax);

        //qDebug() << "rcsurface->gethdc() : " << (QObject*)rcsurface->gethdc();

        OpenOpenGl(((RCView*)rcsurface->gethdc())->view(),3,0,0,widget->width(),widget->height());

        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

        if (view->getperspective())
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
        else
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

        //SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
        SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
        BeginScene(0/*BS_NOCLEAR*//*| BS_FRONTBUFFERONLY */);
        //SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);
        nlights = 0;
        for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
          if (e->isa(light_entity) && e->isvisible(rcsurface))
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

        if (nlights == 0)
            SetLight(eye.x,eye.y,eye.z);

        if(glIsEnabled(GL_DEPTH_TEST))
            qDebug() << "gl depth test IS enabled";
        else
            qDebug() << "gl depth test is NOT enabled";

        if(rcplane->isvisible(rcsurface))
        {
           if (rcplane->getntriangles() < 0)
             rcplane->triangulate((rcsurface->getumax() - rcsurface->getumin()) / rcsurface->getwidth() * db.getdtoler(),&transform);

           ntriangles = rcplane->getntriangles();
           triangles = rcplane->gettriangles();

           for (k = 0,j = 0 ; k < ntriangles ; k++,j += 9)
             {  if (triangles[j].z == RCDBL_MAX)
                  {  sp = (SurfaceProperties *) (int)triangles[j].x;
                     if (sp != 0 && sp->tmi.texturetype == 1)
                       {  SetMaterial(1.0,1.0,1.0,sp->ambient,sp->diffuse,sp->specular);
                          LoadTexture(sp->tmi.filename,1.0,1.0,1.0,0.004,1.0-triangles[j].y,&t);
                          if (t == -1)
                            {  SetTexture(0);
                               SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                            }
                          else
                            {  SetTexture(t);
                            }
                       }
                     else
                       {  SetTexture(0);
                          SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                       }
                     j += 2;
                  }

                if (DrawTexturedTriangles != 0 && j+1 < ntriangles && triangles[j+9].z != RCDBL_MAX)
                {  DrawTexturedTriangles(ntriangles,triangles);
                   break;
                }
                else
                   DrawTexturedTriangle(triangles + j);
             }
           glFinish();
        }

        //EndScene();
    }
    else // other painting modes
    {
        View *view;
        int i,j,halfway,imin,jmin,imax,jmax,n,r1,g1,b1,r2,g2,b2,gradientfillstyle;
        Point p1,p2,p3,p4,xa,ya,pmin1,pmax1,pmin2,pmax2;
        Transform t1,t2,tview;

        OpenOpenGl(((RCView*)rcsurface->gethdc())->view(),3,0,0,widget->width(),widget->height());

        //view = cadwindow->getcurrentsurface()->getview();
        view = rcsurface->getview();
        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
        SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
        //QtBeginScene();
        BeginScene(BS_NOCLEAR/*BS_FRONTBUFFERONLY | BS_IGNOREDEPTH */);
        SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);

        if (rcplane->getoptions().test(1))
        {
             t1.rotate(Point(0.0,0.0,0.0),rcplane->getxaxis().cross(rcplane->getyaxis()),rcplane->getangle() * acos(-1.0) / 180.0);
             xa = t1.transform(rcplane->getxaxis());
             ya = t1.transform(rcplane->getyaxis());
        }
        else
        {
            xa = rcplane->getxaxis();
            ya = rcplane->getyaxis();
        }

        //Polygon3d polygon3d(rcplane->getorigin(),xa,ya,rcplane->getlist(),0,(cadwindow->getcurrentsurface()->getumax() - cadwindow->getcurrentsurface()->getumin()) / cadwindow->getcurrentsurface()->getwidth());
        Polygon3d polygon3d(rcplane->getorigin(),xa,ya,rcplane->getlist(),0,(rcsurface->getumax() - rcsurface->getumin()) / rcsurface->getwidth());

        polygon3d.setextents();

        if (rcplane->getoptions().test(PlaneGradientFilled))
        {
            if(polygon3d.getdefined())
            {
                pmin1 = polygon3d.getpmin();
                pmax1 = polygon3d.getpmax();

                pmin2.x = pmax2.x = (pmin1 - rcplane->getorigin()).dot(xa);
                pmin2.y = pmax2.y = (pmin1 - rcplane->getorigin()).dot(ya);
                for (i = 0 ; i < 7 ; i++)
                  {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                     p2.x = (p1 - rcplane->getorigin()).dot(xa);
                     p2.y = (p1 - rcplane->getorigin()).dot(ya);
                     if (pmin2.x > p2.x) pmin2.x = p2.x;
                     if (pmin2.y > p2.y) pmin2.y = p2.y;
                     if (pmax2.x < p2.x) pmax2.x = p2.x;
                     if (pmax2.y < p2.y) pmax2.y = p2.y;
                  }
                pmin2.z = pmax2.z = 0.0;

                imin = (int)floor(pmin2.x /  rcplane->getxspacing());
                jmin = (int)floor(pmin2.y /  rcplane->getyspacing());
                imax = (int)ceil(pmax2.x /  rcplane->getxspacing());
                jmax = (int)ceil(pmax2.y /  rcplane->getyspacing());

                if (imin >= imax)
                  imin = imax - 1;
                if (jmin >= jmax)
                  jmin = jmax - 1;

                r1 = db.colourtable[rcplane->getcolour()].red;
                g1 = db.colourtable[rcplane->getcolour()].green;
                b1 = db.colourtable[rcplane->getcolour()].blue;
                //r1 = GetRValue(rcplane->getgradientcolour());
                //g1 = GetGValue(rcplane->getgradientcolour());
                //b1 = GetBValue(rcplane->getgradientcolour());

                r2 = GetRValue(rcplane->getgradientcolour());
                g2 = GetGValue(rcplane->getgradientcolour());
                b2 = GetBValue(rcplane->getgradientcolour());
                //r2 = db.colourtable[rcplane->getcolour()].red;
                //g2 = db.colourtable[rcplane->getcolour()].green;
                //b2 = db.colourtable[rcplane->getcolour()].blue;


                gradientfillstyle = rcplane->getoptions().test(PlaneGradientStyle1) * 8 + rcplane->getoptions().test(PlaneGradientStyle2) * 4 +  rcplane->getoptions().test(PlaneGradientStyle3) * 2 +  rcplane->getoptions().test(PlaneGradientStyle4);

                //draw the stencil mask
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0x00);
                glClearStencil(0);
                glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                glDepthMask(GL_FALSE);

                glStencilFunc(GL_NEVER,1,0xff);
                glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
                glStencilMask(0xff);
                glClear(GL_STENCIL_BUFFER_BIT);

                //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                //polygon3d.transform(&rcsurface->modeltransform);
                polygon3d.transform(&transform);
                DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());

                glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
                glDepthMask(GL_TRUE);
                glStencilFunc(GL_EQUAL,1,0xFF);
                glStencilMask(0x00);


                if(gradientfillstyle == 0)
                {
                    for (i = imin ; i <= imax ; i++)
                    {
                         p1 = rcplane->getorigin() + xa *  i * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                         p2 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                         p3 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                         p4 = rcplane->getorigin() + xa * i * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                         Polygon3d polygon3d(p1,p2,p3,p4);

                         double grc = (r1 + ((r2 - r1)*(i-imin))/(imax - imin)) / 255.0;
                         double ggc = (g1 + ((g2 - g1)*(i-imin))/(imax - imin)) / 255.0;
                         double gbc = (b1 + ((b2 - b1)*(i-imin))/(imax - imin)) / 255.0;

                         SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                         //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                         //polygon3d.transform(&rcsurface->modeltransform);
                         polygon3d.transform(&transform);
                         DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                         Flush();
                    }
                }
                else if (gradientfillstyle == 1)
                  {

                    for (i = imin ; i <= imax ; i++)
                       {  p1 = rcplane->getorigin() + xa *  i * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * i * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);

                          //RGB(r2 + ((r1 - r2)*(i-imin))/(imax - imin),g2 + ((g1 - g2)*(i-imin))/(imax - imin),b2 + ((b1 - b2)*(i-imin))/(imax - imin));
                          double grc = (r2 + ((r1 - r2)*(i-imin))/(imax - imin)) / 255.0;
                          double ggc = (g2 + ((g1 - g2)*(i-imin))/(imax - imin)) / 255.0;
                          double gbc = (b2 + ((b1 - b2)*(i-imin))/(imax - imin)) / 255.0;

                          SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                          //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                          //polygon3d.transform(&rcsurface->modeltransform);
                          polygon3d.transform(& transform);
                          DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                          Flush();
                       }
                  }
                else if (gradientfillstyle == 2)
                  {

                    for (j = jmin ; j <= jmax ; j++)
                       {  p1 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);

                          //RGB(r2 + ((r1 - r2)*(j-jmin))/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin))/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin))/(jmax - jmin))
                          double grc = (r2 + ((r1 - r2)*(j-jmin))/(jmax - jmin)) / 255.0;
                          double ggc = (g2 + ((g1 - g2)*(j-jmin))/(jmax - jmin)) / 255.0;
                          double gbc = (b2 + ((b1 - b2)*(j-jmin))/(jmax - jmin)) / 255.0;

                          SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                          //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                          //polygon3d.transform(&rcsurface->modeltransform);
                          polygon3d.transform(& transform);
                          DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                          Flush();
                    }
                  }
                else if (gradientfillstyle == 3)
                  {  for (j = jmin ; j <= jmax ; j++)
                       {  p1 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);

                          //RGB(r1 + ((r2 - r1)*(j-jmin))/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin))/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin))/(jmax - jmin));
                          double grc = (r1 + ((r2 - r1)*(j-jmin))/(jmax - jmin)) / 255.0;
                          double ggc = (g1 + ((g2 - g1)*(j-jmin))/(jmax - jmin)) / 255.0;
                          double gbc = (b1 + ((b2 - b1)*(j-jmin))/(jmax - jmin)) / 255.0;

                          SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                          //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                          //polygon3d.transform(&rcsurface->modeltransform);
                          polygon3d.transform(& transform);
                          DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                          Flush();
                      }
                  }
                else if (gradientfillstyle == 4)
                  {  for (i = imin ; i <= imax ; i++)
                       {  p1 = rcplane->getorigin() + xa *  i * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * i * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);



                          halfway = (imin + imax) / 2;
                          if (i < halfway)
                          {
                              //RGB(r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin),g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin),b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin));
                              double grc = (r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin)) / 255.0;
                              double ggc = (g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin)) / 255.0;
                              double gbc = (b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin)) / 255.0;

                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin),g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin),b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin)),transparency);
                          }
                          else
                          {
                              //RGB(r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin),g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin),b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin));

                              double grc = (r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin)) / 255.0;
                              double ggc = (g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin)) / 255.0;
                              double gbc = (b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin)) / 255.0;

                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin),g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin),b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin)),transparency);
                          }

                          Flush();
                       }
                  }
                else if (gradientfillstyle == 5)
                  {  for (i = imin ; i <= imax ; i++)
                       {  p1 = rcplane->getorigin()  + xa *  i * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p2 = rcplane->getorigin()  + xa * (i+1) * rcplane->getxspacing() + ya * jmin * rcplane->getyspacing();
                          p3 = rcplane->getorigin()  + xa * (i+1) * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          p4 = rcplane->getorigin()  + xa * i * rcplane->getxspacing() + ya * jmax * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);

                          halfway = (imin + imax) / 2;
                          if (i < halfway)
                          {
                              //RGB(r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin),g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin),b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin));
                              double grc = (r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin)) / 255.0;
                              double ggc = (g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin)) / 255.0;
                              double gbc = (b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin)) / 255.0;
                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin),g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin),b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin)),transparency);
                          }
                          else
                          {
                              //RGB(r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin),g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin),b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin));
                             double grc = (r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                             double ggc = (g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                             double gbc = (b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                             SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                             //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                             //polygon3d.transform(&rcsurface->modeltransform);
                             polygon3d.transform(& transform);
                             DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                             //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin),g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin),b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin)),transparency);
                          }
                       }
                  }
                else if (gradientfillstyle == 6)
                  {  for (j = jmin ; j <= jmax ; j++)
                       {  p1 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);

                          halfway = (jmin + jmax) / 2;
                          if (j < halfway)
                          {
                              //RGB(r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin));
                              double grc = (r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              double ggc = (g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              double gbc = (b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin)),transparency);
                          }
                          else
                          {
                              //RGB(r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin));
                              double grc = (r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              double ggc = (g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              double gbc = (b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin)),transparency);
                          }
                       }
                  }
                else if (gradientfillstyle == 7)
                  {  for (j = jmin ; j <= jmax ; j++)
                       {  p1 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p2 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                          p3 = rcplane->getorigin() + xa * imax * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          p4 = rcplane->getorigin() + xa * imin * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          halfway = (jmin + jmax) / 2;

                          if (j < halfway)
                          {
                              //RGB(r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin));
                              double grc = (r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              double ggc = (g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              double gbc = (b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin)),transparency);
                          }
                          else
                          {
                             // RGB(r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin));
                              double grc = (r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              double ggc = (g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              double gbc = (b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                              SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                              //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                              //polygon3d.transform(&rcsurface->modeltransform);
                              polygon3d.transform(& transform);
                              DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                              //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin)),transparency);
                          }
                      }
                  }
                else if (gradientfillstyle == 8 || gradientfillstyle == 9)
                  {double radius,radius1,radius2;

                     radius = (pmax2 - pmin2).length() / 2.0;
                     radius1 = 0.0;
                     radius2 = rcplane->getxspacing();
                     i = 0;
                     imax = int(radius / rcplane->getxspacing() + 0.5);
                     if (imax < 1) imax = 1;
                     while (radius1 < radius)
                       {  p1 = rcplane->getorigin() + xa * (pmin2.x + pmax2.x) / 2.0 + ya * (pmin2.y + pmax2.y) / 2.0;
                          Circle circle1(p1,xa,ya,radius1,0.0,acos(-1.0)*2.0);
                          Circle circle2(p1,xa,ya,radius2,0.0,acos(-1.0)*2.0);
                          EntityList list;
                          if (radius1 > 0.0)
                          {
                               list.add(&circle1);
                               list.add(0);
                          }
                          list.add(&circle2);
                          Polygon3d polygon3d(p1,xa,ya,list,0,(cadwindow->getcurrentsurface()->getumax() - cadwindow->getcurrentsurface()->getumin()) / cadwindow->getcurrentsurface()->getwidth());
                          if (gradientfillstyle == 8)
                          {
                               //RGB(r2 + ((r1 - r2)*i)/imax,g2 + ((g1 - g2)*i)/imax,b2 + ((b1 - b2)*i/imax));
                               double grc = (r2 + ((r1 - r2)*i)/imax) / 255.0;
                               double ggc = (g2 + ((g1 - g2)*i)/imax) / 255.0;
                               double gbc = (b2 + ((b1 - b2)*i/imax)) / 255.0;
                               SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                               //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                               //polygon3d.transform(&rcsurface->modeltransform);
                               polygon3d.transform(& transform);
                               DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                               //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*i)/imax,g2 + ((g1 - g2)*i)/imax,b2 + ((b1 - b2)*i/imax)),transparency);
                          }
                          else
                          {
                               //RGB(r2 + ((r1 - r2)*(imax-i))/imax,g2 + ((g1 - g2)*(imax-i))/imax,b2 + ((b1 - b2)*(imax-i)/imax))
                               double grc = (r2 + ((r1 - r2)*(imax-i))/imax) / 255.0;
                               double ggc = (g2 + ((g1 - g2)*(imax-i))/imax) / 255.0;
                               double gbc = (b2 + ((b1 - b2)*(imax-i)/imax)) / 255.0;
                               SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());
                               //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                               //polygon3d.transform(&rcsurface->modeltransform);
                               polygon3d.transform(& transform);
                               DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                               //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(imax-i))/imax,g2 + ((g1 - g2)*(imax-i))/imax,b2 + ((b1 - b2)*(imax-i)/imax)),transparency);
                          }
                          radius1 += rcplane->getxspacing();
                          radius2 += rcplane->getxspacing();
                          i++;
                       }
                  }

                glStencilMask(0xFF);
                glClear(GL_STENCIL_BUFFER_BIT);
                glDisable(GL_STENCIL_TEST);

            }
        }
        else if (rcplane->getoptions().test(PlaneBitmapFilled))
        {
            if ( polygon3d.getdefined() &&
                 rcplane->getpatternfilename() &&
                 strlen(rcplane->getpatternfilename()) > 0)
            {
                int texture;
                Point p[9];
                Point p3,p4;

                 //polygon3d.setextents();

                 pmin1 = polygon3d.getpmin();
                 pmax1 = polygon3d.getpmax();

                 pmin2.x = pmax2.x = (pmin1 - rcplane->getorigin()).dot(xa);
                 pmin2.y = pmax2.y = (pmin1 - rcplane->getorigin()).dot(ya);
                 for (i = 0 ; i < 7 ; i++)
                   {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                      p2.x = (p1 - rcplane->getorigin()).dot(xa);
                      p2.y = (p1 - rcplane->getorigin()).dot(ya);
                      if (pmin2.x > p2.x) pmin2.x = p2.x;
                      if (pmin2.y > p2.y) pmin2.y = p2.y;
                      if (pmax2.x < p2.x) pmax2.x = p2.x;
                      if (pmax2.y < p2.y) pmax2.y = p2.y;
                   }

                 imin = (int)floor(pmin2.x /  rcplane->getxspacing());
                 jmin = (int)floor(pmin2.y /  rcplane->getyspacing());
                 imax = (int)ceil(pmax2.x /  rcplane->getxspacing());
                 jmax = (int)ceil(pmax2.y /  rcplane->getyspacing());


                 //draw the stencil mask
                 glEnable(GL_STENCIL_TEST);
                 glStencilMask(0x00);
                 glClearStencil(0);
                 glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                 glDepthMask(GL_FALSE);

                 glStencilFunc(GL_NEVER,1,0xff);
                 glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
                 glStencilMask(0xff);
                 glClear(GL_STENCIL_BUFFER_BIT);

                 //polygon3d.transform(&cadwindow->getcurrentsurface()->modeltransform);
                 //polygon3d.transform(&rcsurface->modeltransform);
                 polygon3d.transform(& transform);
                 DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());

                 glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
                 glDepthMask(GL_TRUE);
                 glStencilFunc(GL_EQUAL,1,0xFF);
                 glStencilMask(0x00);

    #if 0
                 double grc = db.colourtable[rcplane->getcolour()].red / 255.0;
                 double ggc = db.colourtable[rcplane->getcolour()].green / 255.0;
                 double gbc = db.colourtable[rcplane->getcolour()].blue / 255.0;
                 SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,rcplane->gettransparency());

                 int hor = rcbitmap->getoptions().test(2) * 2 + rcbitmap->getoptions().test(1);
                 int ver = rcbitmap->getoptions().test(4) * 2 + rcbitmap->getoptions().test(3);

    #else
                 BitMask bo(32);
                 if (rcplane->gettransparency() != 0.0 || rcplane->gettransparencyrange() != 0.0)
                   bo.set(9,1);
                 if(rcbitmap == 0)
                 {
                     // if the bitmap is null
                     // make a new bitmap otherwise use the existing pointer
                     if(rcplane->bitmape == 0)
                     {
                        rcbitmap = new BitMapE(rcplane->getpatternfilename(),rcplane->getorigin(),
                                           xa,ya,rcplane->getxspacing(),rcplane->getyspacing(),
                                           rcplane->getred(),rcplane->getgreen(),rcplane->getblue(),
                                           rcplane->gettransparencyrange(),1.0-rcplane->gettransparency(),bo);
                        rcplane->bitmape = rcbitmap;
                     }
                     else
                         rcbitmap = rcplane->bitmape;
                 }

                 int hor = rcbitmap->getoptions().test(2) * 2 + rcbitmap->getoptions().test(1);
                 int ver = rcbitmap->getoptions().test(4) * 2 + rcbitmap->getoptions().test(3);

                 if (rcdrawmode == DM_NORMAL)
                 {
                     SetMaterial(1.0,1.0,1.0,1.0,1.0,0.0);
                     LoadMemoryTexture(rcbitmap->getfilename(),(char *)rcbitmap->getpbuffer(),
                                   rcbitmap->getbmi()->bmiHeader.biWidth,rcbitmap->getbmi()->bmiHeader.biHeight,
                                   rcbitmap->getred(),rcbitmap->getgreen(),rcbitmap->getblue(),
                                   rcbitmap->gettransparencyrange(),rcbitmap->getopacity(),&texture);
                     SetTexture(texture);
                 }
                 else if (rcdrawmode == DM_SELECT )
                 {
                     SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
                     LoadMemoryTexture(rcbitmap->getfilename(),(char *)rcbitmap->getpbuffer(),
                                   rcbitmap->getbmi()->bmiHeader.biWidth,rcbitmap->getbmi()->bmiHeader.biHeight,
                                   rcbitmap->getred(),rcbitmap->getgreen(),rcbitmap->getblue(),
                                   rcbitmap->gettransparencyrange(),rcbitmap->getopacity(),&texture);
                     SetTexture(texture);
                 }
                 else if (rcdrawmode == DM_INVERT )
                 {
                     SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
                     LoadMemoryTexture(rcbitmap->getfilename(),(char *)rcbitmap->getpbuffer(),
                                   rcbitmap->getbmi()->bmiHeader.biWidth,rcbitmap->getbmi()->bmiHeader.biHeight,
                                   rcbitmap->getred(),rcbitmap->getgreen(),rcbitmap->getblue(),
                                   rcbitmap->gettransparencyrange(),rcbitmap->getopacity(),&texture);
                     SetTexture(texture);
                 }
                 else
                 {
                     SetMaterial(v.getreal("wn::backgroundcolour.red"),v.getreal("wn::backgroundcolour.green"),v.getreal("wn::backgroundcolour.blue"),1.0,1.0,0.0);
                     SetTexture(0);
                 }
    #endif
                 // draw the bitmap
                 for (i = imin ; i <= imax ; i++)
                   for (j = jmin ; j <= jmax ; j++)
                     {
                        p1 = rcplane->getorigin() + xa *  i * rcplane->getxspacing() + ya * j * rcplane->getyspacing();
                        //p2 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                        rcbitmap->setorigin(p1);
                        if (rcbitmap->getdefined())
                        {
                            //rcbitmap->draw(drawmode,transform,surface);

                            //p1 = cadwindow->getcurrentsurface()->modeltransform.transform(rcbitmap->getorigin());
                            p1 = rcsurface->modeltransform.transform(rcbitmap->getorigin());

                            p1 = p1 - rcbitmap->getxaxis() * (hor*rcbitmap->getwidth()/2.0) - rcbitmap->getyaxis() * ver*rcbitmap->getheight()/2.0;
                            p2 = p1 + rcbitmap->getxaxis() * rcbitmap->getwidth();
                            p3 = p2 + rcbitmap->getyaxis() * rcbitmap->getheight();
                            p4 = p3 - rcbitmap->getxaxis() * rcbitmap->getwidth();


                            //p1 = cadwindow->getcurrentsurface()->modeltransform.transform(p1);
                            //p2 = cadwindow->getcurrentsurface()->modeltransform.transform(p2);
                            //p3 = cadwindow->getcurrentsurface()->modeltransform.transform(p3);
                            //p4 = cadwindow->getcurrentsurface()->modeltransform.transform(p4);

                            p1 = rcsurface->modeltransform.transform(p1);
                            p2 = rcsurface->modeltransform.transform(p2);
                            p3 = rcsurface->modeltransform.transform(p3);
                            p4 = rcsurface->modeltransform.transform(p4);

                            p[0] = p1;
                            p[1] = p2;
                            p[2] = p3;
                            p[3] = rcbitmap->getxaxis().cross(rcbitmap->getyaxis());
                            p[4] = p[3];
                            p[5] = p[3];
                            p[6] = Point(0.001,0.001,0.0);
                            p[7] = Point(0.999,0.001,0.0);
                            p[8] = Point(0.999,0.999,0.0);
                            DrawTexturedTriangle(p);

                            p[0] = p1;
                            p[1] = p3;
                            p[2] = p4;
                            p[3] = rcbitmap->getxaxis().cross(rcbitmap->getyaxis());
                            p[4] = p[3];
                            p[5] = p[3];
                            p[6] = Point(0.001,0.001,0.0);
                            p[7] = Point(0.999,0.999,0.0);
                            p[8] = Point(0.001,0.999,0.0);
                            DrawTexturedTriangle(p);

                            Flush();
                        }
                     }

                 glStencilMask(0xFF);
                 glClear(GL_STENCIL_BUFFER_BIT);
                 glDisable(GL_STENCIL_TEST);
              }
        }
        else if(rcplane->getoptions().test(PlaneFilled))
        {
            double rc = db.colourtable[rcplane->getcolour()].red;
            double gc = db.colourtable[rcplane->getcolour()].green;
            double bc = db.colourtable[rcplane->getcolour()].blue;

            SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,rcplane->gettransparency());
            //polygon3d.transform(&rcsurface->modeltransform);
            polygon3d.transform(&transform);
            //rcsurface->modeltransform.dump("Gl transform");
            DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
        }
    }

    painter->endNativePainting();
}

/**
 * @brief RCGraphicsBitmapItem::RCGraphicsBitmapItem
 * @param mbitmap
 * @param parent
 */
RCGraphicsBitmapItem::RCGraphicsBitmapItem(BitMapE *mbitmap, View3dSurface *msurface, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    rcbitmap = mbitmap;
    rcsurface = msurface;
    rcdrawmode = DM_NORMAL;
}

RCGraphicsBitmapItem::~RCGraphicsBitmapItem()
{
    rcbitmap = 0;
}

QRectF RCGraphicsBitmapItem::boundingRect(void) const
{
    if(qpath.elementCount() > 0)
        return qpath.boundingRect();
    else
        return QRectF();
}

QPainterPath RCGraphicsBitmapItem::shape(void) const
{
    return qpath;
}

void RCGraphicsBitmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPen oldPen = painter->pen();
    painter->setPen(Qt::NoPen);
    painter->drawPath(qpath);
    painter->setPen(oldPen);

    if(painter->paintEngine()->type() != QPaintEngine::OpenGL2)
       return;

    // opengl drawing from here
    if(rcbitmap == 0) // check for a model bitmap
        return;

    painter->beginNativePainting();

    Entity *e;
    int texture,nlights;
    Point p[9];
    Point p1,p2,p3,p4;
    Point eye,pmin,pmax;
    double ambient,diffuse;

    if( rcbitmap->getdefined() &&
        rcbitmap->getfilename() != 0 &&
        strlen(rcbitmap->getfilename()) > 0 &&
        rcbitmap->getpbuffer() != 0)
    {
        View *view;
        view = cadwindow->getcurrentsurface()->getview();
        db.geometry.extents(cadwindow->getcurrentwindow(),&pmin,&pmax);

        OpenOpenGl(((RCView*)cadwindow->getcurrentwindow()->gethdc())->view(),3,0,0,widget->width(),widget->height());

        if(cadwindow->getcurrentsurface() !=0 && cadwindow->getcurrentsurface()->getrepaintstyle() == Shaded)
        {
            ambient = v.getreal("sh::ambient");
            diffuse = v.getreal("sh::diffuse");

            if (view->getperspective())
              eye = view->geteye();
            else
              eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

            if (view->getperspective())
              SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
            else
              SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

            //SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(cadwindow->getcurrentwindow()->getumin(),cadwindow->getcurrentwindow()->getvmin(),cadwindow->getcurrentwindow()->getumax(),cadwindow->getcurrentwindow()->getvmax());
            SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
            BeginScene(0/*BS_NOCLEAR*/ /*| BS_FRONTBUFFERONLY */);
            //SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);
            nlights = 0;
            for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
              if (e->isa(light_entity) && e->isvisible(rcsurface))
                {double red,green,blue;
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

            if (nlights == 0)
                SetLight(eye.x,eye.y,eye.z);
        }
        else
        {
            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(cadwindow->getcurrentsurface()->getumin(),cadwindow->getcurrentsurface()->getvmin(),cadwindow->getcurrentsurface()->getumax(),cadwindow->getcurrentsurface()->getvmax());
            SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
            BeginScene(/*BS_FRONTBUFFERONLY |*/ BS_IGNOREDEPTH | BS_NOCLEAR/* //* rcbitmap->getoptions().test(31)*/);
            SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
        }
         int hor = rcbitmap->getoptions().test(2) * 2 + rcbitmap->getoptions().test(1);
         int ver = rcbitmap->getoptions().test(4) * 2 + rcbitmap->getoptions().test(3);

         if (rcdrawmode == DM_NORMAL)
         {
             SetMaterial(1.0,1.0,1.0,1.0,1.0,0.0);
             LoadMemoryTexture(rcbitmap->getfilename(),(char *)rcbitmap->getpbuffer(),
                               rcbitmap->getbmi()->bmiHeader.biWidth,rcbitmap->getbmi()->bmiHeader.biHeight,
                               rcbitmap->getred(),rcbitmap->getgreen(),rcbitmap->getblue(),
                               rcbitmap->gettransparencyrange(),rcbitmap->getopacity(),&texture);
             SetTexture(texture);
         }
         else if (rcdrawmode == DM_SELECT)
         {
            SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
            LoadMemoryTexture(rcbitmap->getfilename(),(char *)rcbitmap->getpbuffer(),
                              rcbitmap->getbmi()->bmiHeader.biWidth,rcbitmap->getbmi()->bmiHeader.biHeight,
                              rcbitmap->getred(),rcbitmap->getgreen(),rcbitmap->getblue(),
                              rcbitmap->gettransparencyrange(),rcbitmap->getopacity(),&texture);
            SetTexture(texture);
         }
         else
         {
             SetMaterial(v.getreal("wn::backgroundcolour.red"),v.getreal("wn::backgroundcolour.green"),v.getreal("wn::backgroundcolour.blue"),1.0,1.0,0.0);
             SetTexture(0);
         }

            // draw the bitmap

            Transform t;
            cadwindow->getcurrentsurface()->gettransform(&t);
            //if (t == 0)
            p1 = rcbitmap->getorigin();
            //else
            //  p1 = t.transform(rcbitmap->getorigin());

            //p1 = cadwindow->getcurrentsurface()->modeltransform.transform(rcbitmap->getorigin());
            p1 = cadwindow->getcurrentsurface()->modeltransform.transform(p1);

            p1 = p1 - rcbitmap->getxaxis() * (hor*rcbitmap->getwidth()/2.0) - rcbitmap->getyaxis() * ver*rcbitmap->getheight()/2.0;
            p2 = p1 + rcbitmap->getxaxis() * rcbitmap->getwidth();
            p3 = p2 + rcbitmap->getyaxis() * rcbitmap->getheight();
            p4 = p3 - rcbitmap->getxaxis() * rcbitmap->getwidth();

            p1 = cadwindow->getcurrentsurface()->modeltransform.transform(p1);
            p2 = cadwindow->getcurrentsurface()->modeltransform.transform(p2);
            p3 = cadwindow->getcurrentsurface()->modeltransform.transform(p3);
            p4 = cadwindow->getcurrentsurface()->modeltransform.transform(p4);

            p[0] = p1;
            p[1] = p2;
            p[2] = p3;
            p[3] = rcbitmap->getxaxis().cross(rcbitmap->getyaxis());
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.001,0.0);
            p[8] = Point(0.999,0.999,0.0);
            DrawTexturedTriangle(p);

            p[0] = p1;
            p[1] = p3;
            p[2] = p4;
            p[3] = rcbitmap->getxaxis().cross(rcbitmap->getyaxis());
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.999,0.0);
            p[8] = Point(0.001,0.999,0.0);
            DrawTexturedTriangle(p);

            Flush();
    }

    painter->endNativePainting();
}

/**
 * @brief RCGraphicsPatchItem::RCGraphicsPatchItem
 * @param mpatch
 * @param msurface
 * @param parent
 */
RCGraphicsPatchItem::RCGraphicsPatchItem(Patch *mpatch, View3dSurface *msurface, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    rcpatch = mpatch;
    rcsurface = msurface;
    rcdrawmode = DM_NORMAL;
}

RCGraphicsPatchItem::~RCGraphicsPatchItem()
{
    rcpatch = 0;
    rcsurface = 0;
}

QRectF RCGraphicsPatchItem::boundingRect(void) const
{
    if(qpath.elementCount() > 0)
        return qpath.boundingRect();
    else
        return QRectF();
}

QPainterPath RCGraphicsPatchItem::shape(void) const
{
    return qpath;
}

void RCGraphicsPatchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QPen oldPen = painter->pen();
    //painter->setPen(Qt::NoPen);
    //painter->drawPath(qpath);
    //painter->setPen(oldPen);

    qDebug() << "paint engine type : " << painter->paintEngine()->type();

    if(painter->paintEngine()->type() != QPaintEngine::OpenGL2)
        return;

    // opengl drawing from here
    if(rcpatch == 0) // check for a model patch
        return;

    if(rcsurface == 0)
        return;

    painter->beginNativePainting();

    Entity *e;
    int ntriangles;
    Point *triangles;
    SurfaceProperties *sp;
    double ambient,diffuse;
    View *view;
    BitMask foptions(32);
    Point eye,pmin,pmax;
    int i,k,j,t,nlights,y;

    ambient = v.getreal("sh::ambient");
    diffuse = v.getreal("sh::diffuse");

    RCCheckGlError();

    foptions.set(2);
    view = rcsurface->getview();
    db.geometry.extents(rcsurface,&pmin,&pmax);

    OpenOpenGl(((RCView*)rcsurface->gethdc())->view(),3,0,0,widget->width(),widget->height());

        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

        if (view->getperspective())
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
        else
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

        //SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
        SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
        BeginScene(0/*BS_NOCLEAR*/ /*| BS_FRONTBUFFERONLY */);
        //SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);
        nlights = 0;
        for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
          if (e->isa(light_entity) && e->isvisible(rcsurface))
            {double red,green,blue;
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

        if (nlights == 0)
            SetLight(eye.x,eye.y,eye.z);

        if(glIsEnabled(GL_DEPTH_TEST))
            qDebug() << "gl depth test IS enabled";
        else
            qDebug() << "gl depth test is NOT enabled";


        if(rcpatch->isvisible(rcsurface))
        {
           if (rcpatch->getntriangles() < 0)
             rcpatch->triangulate((rcsurface->getumax() - rcsurface->getumin()) / rcsurface->getwidth() * db.getdtoler());

           ntriangles = rcpatch->getntriangles();
           triangles = rcpatch->gettriangles();

           for (k = 0,j = 0 ; k < ntriangles ; k++,j += 9)
             {  if (triangles[j].z == RCDBL_MAX)
                  {  sp = (SurfaceProperties *) (int)triangles[j].x;
                     if (sp != 0 && sp->tmi.texturetype == 1)
                       {  SetMaterial(1.0,1.0,1.0,sp->ambient,sp->diffuse,sp->specular);
                          LoadTexture(sp->tmi.filename,1.0,1.0,1.0,0.004,1.0-triangles[j].y,&t);
                          if (t == -1)
                            {  SetTexture(0);
                               SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                            }
                          else
                            {  SetTexture(t);
                            }
                       }
                     else
                       {  SetTexture(0);
                          SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                       }
                     j += 2;
                  }

                if (DrawTexturedTriangles != 0 && j+1 < ntriangles && triangles[j+9].z != RCDBL_MAX)
                {  DrawTexturedTriangles(ntriangles,triangles);
                   break;
                }
                else
                   DrawTexturedTriangle(triangles + j);
             }
           glFinish();
        }

    //EndScene();

    painter->endNativePainting();
}


RCGraphicsLineItem::RCGraphicsLineItem(Line *mline, View3dSurface *msurface, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    rcline = mline;
    rcsurface = msurface;
    rcdrawmode = DM_NORMAL;
}

RCGraphicsLineItem::~RCGraphicsLineItem()
{
    rcline = 0;
    rcsurface = 0;
}

QRectF RCGraphicsLineItem::boundingRect(void) const
{
#if 1
    return QRectF();
#else
    Point sp1,sp2;

    if(rcline == 0)
        return QRectF();

    //rcsurface = ((RCView*)widget->parent()->parent())->getView3dWindow();

    if(rcsurface == 0)
        return QRectF();

    sp1 = rcsurface->modeltoscreen(rcline->start());
    sp2 = rcsurface->modeltoscreen(rcline->end());
    double minx = min(sp1.x,sp2.x);
    double miny = min(sp1.y,sp2.y);
    double maxx = max(sp1.x,sp2.x);
    double maxy = max(sp1.y,sp2.y);
    return QRectF(0,0,maxx-minx,maxy-miny);
#endif
}

void RCGraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QPen oldPen = painter->pen();
    //painter->setPen(Qt::NoPen);
    //painter->drawPath(qpath);
    //painter->setPen(oldPen);

    //qDebug() << "paint widget  : " << widget;
    //qDebug() << "parent widget : " << widget->parent();
    //qDebug() << "parent parent widget : " << widget->parent()->parent();
    //qDebug() << "paint engine type : " << painter->paintEngine()->type();

    //if(painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    //    return;

    RCView *rcview = (RCView*)widget->parent()->parent();

    rcsurface = rcview->getView3dWindow();

    if(rcline == 0)
        return;

    if(rcsurface == 0)
        return;

    if (rcsurface->getsurfacetype() != View3dSurf)
        return;

#if 0
    Point sp1,sp2;
    sp1 = rcsurface->modeltoscreen(rcline->start());
    sp2 = rcsurface->modeltoscreen(rcline->end());
    sp1.y = rcsurface->getheight() - 1 - sp1.y;
    sp2.y = rcsurface->getheight() - 1 - sp2.y;

    painter->save();
    rcsurface->setup(rcline->getlinestylescale(),rcline->length(),rcdrawmode,rcline->getcolour(),rcline->getstyle(),rcline->getweight(),rcline->getpatternlinestyle());
    painter->setPen(rcsurface->curPen);
    painter->drawLine(QPointF(sp1.x,sp1.y),QPointF(sp2.x,sp2.y));
    painter->restore();
#endif

#if 0
    rcline->draw(rcdrawmode,0,rcsurface,painter);
#endif

    //rcline->drawGL(rcdrawmode,0,rcsurface);

#if 1
    painter->beginNativePainting();

    View *view;
    Entity *e;
    Point p1,p2;
    Point eye,pmin,pmax;

    view = rcsurface->getview();

    OpenOpenGl(rcview->view(),3,0,0,rcview->view()->width(),rcview->view()->height());

    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
    BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

    //glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    //glClear( GL_DEPTH_BUFFER_BIT );
    //glFinish();

    rcline->drawGL(rcdrawmode,0,rcsurface);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_DEPTH_TEST);
    //glFinish();

    EndScene();

    painter->endNativePainting();
#endif
}


RCGraphicsCircleItem::RCGraphicsCircleItem(Circle *mcircle, View3dSurface *msurface, QGraphicsItem * parent)
{
    rccircle = mcircle;
    rcsurface = msurface;
    rcdrawmode = DM_NORMAL;

}

RCGraphicsCircleItem::~RCGraphicsCircleItem()
{
    rccircle = 0;
    rcsurface = 0;
}

QRectF RCGraphicsCircleItem::boundingRect(void) const
{
    return QRectF();
}

void RCGraphicsCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QPen oldPen = painter->pen();
    //painter->setPen(Qt::NoPen);
    //painter->drawPath(qpath);
    //painter->setPen(oldPen);

    //if(painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    //    return;

    RCView *rcview = (RCView*)widget->parent()->parent();

    rcsurface = rcview->getView3dWindow();

    if(rccircle == 0)
        return;

    if(rcsurface == 0)
        return;

    if (rcsurface->getsurfacetype() != View3dSurf)
        return;

#if 0
    Point sp1,sp2;
    sp1 = rcsurface->modeltoscreen(rcline->start());
    sp2 = rcsurface->modeltoscreen(rcline->end());
    sp1.y = rcsurface->getheight() - 1 - sp1.y;
    sp2.y = rcsurface->getheight() - 1 - sp2.y;

    painter->save();
    rcsurface->setup(rcline->getlinestylescale(),rcline->length(),rcdrawmode,rcline->getcolour(),rcline->getstyle(),rcline->getweight(),rcline->getpatternlinestyle());
    painter->setPen(rcsurface->curPen);
    painter->drawLine(QPointF(sp1.x,sp1.y),QPointF(sp2.x,sp2.y));
    painter->restore();
#endif

#if 0
    rcline->draw(rcdrawmode,0,rcsurface,painter);
#endif

#if 1
    painter->beginNativePainting();

    View *view;
    Entity *e;
    Point p1,p2;
    Point eye,pmin,pmax;

    view = rcsurface->getview();

    OpenOpenGl(rcview->view(),3,0,0,rcview->view()->width(),rcview->view()->height());

    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
    BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

    //glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    //glClear( GL_DEPTH_BUFFER_BIT );
    //glFinish();

    rccircle->drawGL(rcdrawmode,0,rcsurface);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_DEPTH_TEST);
    //glFinish();

    EndScene();

    painter->endNativePainting();
#endif
}

