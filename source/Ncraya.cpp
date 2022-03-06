
#include "ncwin.h"
#include "texture_dialog.h"

#if MENUS != STUDENTMENUS

void SurfaceProperties::setdefaults(void)
{ v.setinteger("sp::texturetype",tmi.texturetype);
  v.setstring("sp::texturemap",tmi.filename);
  v.setreal("sp::tmx1",tmi.x1);
  v.setreal("sp::tmy1",tmi.y1);
  v.setreal("sp::tmx2",tmi.x2);
  v.setreal("sp::tmy2",tmi.y2);
  v.setbm("sp::tmoptions",tmi.options);
  v.setreal("sp::texturesize",tmi.size);
  v.setreal("sp::patternscale",tmi.patternscale);
  v.setreal("sp::colourscale",tmi.colourscale);
  v.setreal("sp::transparentred",tmi.transparentcolour.red);
  v.setreal("sp::transparentgreen",tmi.transparentcolour.green);
  v.setreal("sp::transparentblue",tmi.transparentcolour.blue);

  v.setstring("sp::bumpmap",bmi.filename);
  v.setreal("sp::bmx1",bmi.x1);
  v.setreal("sp::bmy1",bmi.y1);
  v.setreal("sp::bmx2",bmi.x2);
  v.setreal("sp::bmy2",bmi.y2);
  v.setbm("sp::bmoptions",bmi.options);

  v.setinteger("sp::shader",shader);
  v.setbm("sp::options",options);
  v.setreal("sp::transparency",transparency);
  v.setreal("sp::ambient",ambient);
  v.setreal("sp::diffuse",diffuse);
  v.setreal("sp::specular",specular);
  v.setreal("sp::exponent",exponent);
  v.setreal("sp::reflectivity",reflectivity);
  v.setreal("sp::smoothness",smoothness);
  v.setreal("sp::metallic",metallic);
  v.setreal("sp::refractiveindex",refractiveindex);
}

void SurfaceProperties::verify(Entity *e,int set)
{AttributeList atlist;
 RCCHAR *at;
  getdefaults();
  atlist = e->getatlist().copy();
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    {  if (strncmp(at,"SP::A:",6) == 0 && set == 0)
         loada(at);
       else if (strncmp(at,"SP::B:",6) == 0 && set == 0)
         loadb(at);
       else if (strncmp(at,"SP::C:",6) == 0 && set == 0)
         loadc(at);
       else if (strncmp(at,"SP::D:",6) == 0 && set == 1)
         loada(at);
       else if (strncmp(at,"SP::E:",6) == 0 && set == 1)
         loadb(at);
       else if (strncmp(at,"SP::F:",6) == 0 && set == 1)
         loadc(at);
    }
  if (setup(1))
     change(e,set);
}

void SurfaceProperties::change(Entity *e,int set)
{AttributeList atlist;
 RCCHAR *at;
 SurfaceProperties sp;
  sp = *this;
  atlist = e->getatlist().copy();
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    {  if (strncmp(at,"SP::A:",6) == 0 || strncmp(at,"SP::D:",6) == 0)
         {  e->delat(at);  e->setattribute(at);
            db.saveundo(UD_DELETEATT,e);
            delete at;
        }
       else if (strncmp(at,"SP::B:",6) == 0 || strncmp(at,"SP::E:",6) == 0)
         {  e->delat(at);  e->setattribute(at);
            db.saveundo(UD_DELETEATT,e);
            delete at;
         }
       else if (strncmp(at,"SP::C:",6) == 0 || strncmp(at,"SP::F:",6) == 0)
         {  e->delat(at);  e->setattribute(at);
            db.saveundo(UD_DELETEATT,e);
            delete at;
         }
    }
  sp.update(this);
  sp.save(e,set);
}

int SurfaceProperties::copyinfo(Entity *e,int set,RayTraceImage *rti,SurfaceInfo **si)
{AttributeList atlist;
 int found;
 RCCHAR *at;
  if (si != 0)
    *si = 0;
  if (e == 0)
    getdefaults();
  else
    {  found = 0;
       atlist = e->getatlist().copy();
       for (atlist.start() ; (at = atlist.next()) != NULL ; )
         {  if (strncmp(at,"SP::A:",6) == 0 && set == 0)
              {  if (! found) getdefaults();
                 loada(at);
                 found = 1;
              }
            else if (strncmp(at,"SP::B:",6) == 0 && set == 0)
              {  if (! found) getdefaults();
                 loadb(at);
                 found = 1;
              }
            else if (strncmp(at,"SP::C:",6) == 0 && set == 0)
              {  if (! found) getdefaults();
                 loadc(at);
                 found = 1;
              }
            else if (strncmp(at,"SP::D:",6) == 0 && set == 1)
              {  if (! found) getdefaults();
                 loada(at);
                 found = 1;
              }
            else if (strncmp(at,"SP::E:",6) == 0 && set == 1)
              {  if (! found) getdefaults();
                 loadb(at);
                 found = 1;
              }
            else if (strncmp(at,"SP::F:",6) == 0 && set == 1)
              {  if (! found) getdefaults();
                 loadc(at);
                 found = 1;
              }
          }
       if (! found)
         return 0;
    }

  if (rti == 0 && si == 0)
    return 1;  //  Just load the texture information.
 
  if ((*si = new SurfaceInfo) == 0)
    return 1;
  (*si)->white = options.test(0);
  (*si)->shader = shader;
  (*si)->noise = rti != 0 ? & rti->noise : 0;
  (*si)->reflective = options.test(1);
  (*si)->transparency = transparency;
  (*si)->ambient = ambient;
  (*si)->diffuse = diffuse;
  (*si)->specular = specular;
  (*si)->exponent = exponent;
  if (shader == 0)
    (*si)->reflectivity = reflectivity;
  else
    (*si)->reflectivity = 1.0 - transparency - (1.0 - smoothness * smoothness * smoothness)*(1 - transparency);
  (*si)->smoothness = smoothness;
  (*si)->metallic = metallic;
  (*si)->refractiveindex = refractiveindex;
  if (tmi.texturetype != 0 && ((*si)->tmi = new TMapInfo) != 0)
    {
      (*si)->tmi->texturetype = tmi.texturetype;
       if (tmi.texturetype == 1)
         {  if (strlen(tmi.filename) > 0)
              (*si)->tmi->texturemap = rti != 0 ? rti->loadtexturemap(tmi.filename) : 0;
            else
              {  delete (*si)->tmi;
                 (*si)->tmi = 0;
              }
         }
       else
         (*si)->tmi->texturemap = 0;
       if ((*si)->tmi != 0)
         {  (*si)->tmi->x1 = tmi.x1;
            (*si)->tmi->y1 = tmi.y1;
            (*si)->tmi->x2 = tmi.x2;
            (*si)->tmi->y2 = tmi.y2;
            (*si)->tmi->transparentcolour.red = tmi.transparentcolour.red;
            (*si)->tmi->transparentcolour.green = tmi.transparentcolour.green;
            (*si)->tmi->transparentcolour.blue = tmi.transparentcolour.blue;
            (*si)->tmi->swapxy = tmi.options.test(0);
            (*si)->tmi->usetransparentcolour = tmi.options.test(1);
            (*si)->tmi->size = tmi.size;
            (*si)->tmi->patternscale = tmi.patternscale;
            (*si)->tmi->colourscale = tmi.colourscale;
            (*si)->tmi->origin = tmi.origin;
            (*si)->tmi->xaxis = tmi.xaxis;
            (*si)->tmi->yaxis = tmi.yaxis;
            (*si)->tmi->zaxis = tmi.xaxis.cross(tmi.yaxis).normalize();
         }
    }
  else
    (*si)->tmi = 0;
  if (strlen(bmi.filename) > 0 && ((*si)->bmi = new BMapInfo) != 0)
    { (*si)->bmi->bumpmap = rti != 0 ? rti->loadbumpmap(bmi.filename) : 0;
      (*si)->bmi->x1 = bmi.x1;
      (*si)->bmi->y1 = bmi.y1;
      (*si)->bmi->x2 = bmi.x2;
      (*si)->bmi->y2 = bmi.y2;
      (*si)->bmi->swapxy = bmi.options.test(0);
    }
  else
    (*si)->bmi = 0;
  if (shader == 0 && specular == 0.0)
    (*si)->shader = 2;  //  Ambient + diffuse calculation only

  return 1;
}

void SurfaceProperties::loada(RCCHAR *at)
{int o;
 char c;
  sscanf(at,"SP::%c:%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &c,&shader,&o,&transparency,&ambient,&diffuse,&specular,&exponent,
            &reflectivity,&smoothness,&metallic,&refractiveindex);
  options.set(0,(o & 1) == 1);
  options.set(1,(o & 2) == 2);
  options.set(2,(o & 4) == 4);
  options.set(3,(o & 8) == 8);
}

void SurfaceProperties::loadb(RCCHAR *at)
{int o;
 RCCHAR c;

  if (sscanf(at,"SP::%c:%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf%ls",
                 &c,&tmi.texturetype,&o,&tmi.x1,&tmi.y1,&tmi.x2,&tmi.y2,&tmi.size,
                 &tmi.patternscale,&tmi.colourscale,
                 &tmi.origin.x,&tmi.origin.y,&tmi.origin.z,
                 &tmi.xaxis.x,&tmi.xaxis.y,&tmi.xaxis.z,
                 &tmi.yaxis.x,&tmi.yaxis.y,&tmi.yaxis.z,
                 &tmi.transparentcolour.red,&tmi.transparentcolour.green,&tmi.transparentcolour.blue,tmi.filename) != 23)
    {  sscanf(at,"SP::%c:%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf%ls",
                     &c,&tmi.texturetype,&o,&tmi.x1,&tmi.y1,&tmi.x2,&tmi.y2,&tmi.size,
                     &tmi.patternscale,&tmi.colourscale,
                     &tmi.origin.x,&tmi.origin.y,&tmi.origin.z,
                     &tmi.xaxis.x,&tmi.xaxis.y,&tmi.xaxis.z,
                     &tmi.yaxis.x,&tmi.yaxis.y,&tmi.yaxis.z,tmi.filename);
       tmi.transparentcolour.red = tmi.transparentcolour.green = tmi.transparentcolour.blue = 0;
    }

  if (strrchr(at,',') != 0)
    strcpy(tmi.filename,strrchr(at,',')+1);

  tmi.options.set(0,(o & 1) == 1);
  tmi.options.set(1,(o & 2) == 2);
  tmi.options.set(2,(o & 4) == 4);
  tmi.options.set(3,(o & 8) == 8);
  tmi.options.set(4,(o & 16) == 16);
}

void SurfaceProperties::loadc(RCCHAR *at)
{int o;
 RCCHAR c;
  sscanf(at,"SP::%c:%d,%lf,%lf,%lf,%lf,%s",
            &c,&o,&bmi.x1,&bmi.y1,&bmi.x2,&bmi.y2,bmi.filename);

  if (strrchr(at,',') != 0)
    strcpy(bmi.filename,strrchr(at,',')+1);

  bmi.options.set(0,(o & 1) == 1);
  bmi.options.set(1,(o & 2) == 2);
  bmi.options.set(2,(o & 4) == 4);
  bmi.options.set(3,(o & 8) == 8);
}

void SurfaceProperties::update(SurfaceProperties *sp)
{ if (sp->chang.test(0))
    shader = sp->shader;
  if (sp->chang.test(1))
    transparency = sp->transparency;
  if (sp->chang.test(2))
    options.set(0,sp->options.test(0));
  if (sp->chang.test(3))
    options.set(1,sp->options.test(1));
  if (sp->chang.test(5))
    ambient = sp->ambient;
  if (sp->chang.test(6))
    diffuse = sp->diffuse;
  if (sp->chang.test(7))
    specular = sp->specular;
  if (sp->chang.test(8))
    reflectivity = sp->reflectivity;
  if (sp->chang.test(11))
    smoothness = sp->smoothness;
  if (sp->chang.test(12))
    metallic = sp->metallic;
  if (sp->chang.test(13))
    refractiveindex = sp->refractiveindex;
  if (sp->chang.test(14))
    tmi.texturetype = sp->tmi.texturetype;
  if (sp->chang.test(16))
    strcpy(tmi.filename,sp->tmi.filename);
  if (sp->chang.test(17))
    tmi.x1 = sp->tmi.x1;
  if (sp->chang.test(18))
    tmi.y1 = sp->tmi.y1;
  if (sp->chang.test(19))
    tmi.x2 = sp->tmi.x2;
  if (sp->chang.test(20))
    tmi.y2 = sp->tmi.y2;
  if (sp->chang.test(21))
    tmi.options.set(0,sp->tmi.options.test(0));
  if (sp->chang.test(23))
    tmi.size = sp->tmi.size;
  if (sp->chang.test(24))
    tmi.patternscale = sp->tmi.patternscale;
  if (sp->chang.test(25))
    tmi.colourscale = sp->tmi.colourscale;
  if (sp->chang.test(26))
    {  tmi.origin = db.workplanes.getcurrent()->getorigin();
       tmi.xaxis  = db.workplanes.getcurrent()->getxaxis();
       tmi.yaxis  = db.workplanes.getcurrent()->getyaxis();
    }

  if (sp->chang.test(35))
    tmi.options.set(1,sp->tmi.options.test(1));
  if (sp->chang.test(34))
    {  tmi.transparentcolour.red = sp->tmi.transparentcolour.red;
       tmi.transparentcolour.green = sp->tmi.transparentcolour.green;
       tmi.transparentcolour.blue = sp->tmi.transparentcolour.blue;
    }

  if (sp->chang.test(27))
    strcpy(bmi.filename,sp->bmi.filename);
  if (sp->chang.test(28))
    bmi.x1 = sp->bmi.x1;
  if (sp->chang.test(29))
    bmi.y1 = sp->bmi.y1;
  if (sp->chang.test(30))
    bmi.x2 = sp->bmi.x2;
  if (sp->chang.test(31))
    bmi.y2 = sp->bmi.y2;
  if (sp->chang.test(32))
    bmi.options.set(0,bmi.options.test(0));
}

void SurfaceProperties::save(Entity *e,int set)
{RCCHAR attribute[1024],*at;
 int o;
  set *= 3;
  o =     options.test(0) + 2 * options.test(1) +
      4 * options.test(2) + 2 * options.test(3);
  sprintf(attribute,"SP::%c:%d,%d,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf",
            'A' + set,shader,o,transparency,ambient,diffuse,specular,exponent,
            reflectivity,smoothness,metallic,refractiveindex);
  if ((at = new RCCHAR[strlen(attribute) + 1]) != NULL)
    {  strcpy(at,attribute);
       e->addat(at);
       e->setattribute(at);
       db.saveundo(UD_INSERTATT,e);
    }
  o =     tmi.options.test(0) + 2 * tmi.options.test(1) +
      4 * tmi.options.test(2) + 2 * tmi.options.test(3);
  sprintf(attribute,"SP::%c:%d,%d,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%ls",
            'B' + set,tmi.texturetype,o,tmi.x1,tmi.y1,tmi.x2,tmi.y2,tmi.size,
            tmi.patternscale,tmi.colourscale,
            tmi.origin.x,tmi.origin.y,tmi.origin.z,
            tmi.xaxis.x,tmi.xaxis.y,tmi.xaxis.z,
            tmi.yaxis.x,tmi.yaxis.y,tmi.yaxis.z,
            tmi.transparentcolour.red,tmi.transparentcolour.green,tmi.transparentcolour.blue,tmi.filename);
  if ((at = new RCCHAR[strlen(attribute) + 1]) != NULL)
    {  strcpy(at,attribute);
       e->addat(at);
       e->setattribute(at);
       db.saveundo(UD_INSERTATT,e);
    }
  o =     bmi.options.test(0) + 2 * bmi.options.test(1) +
      4 * bmi.options.test(2) + 2 * bmi.options.test(3);
  sprintf(attribute,"SP::%c:%d,%.4lf,%.4lf,%.4lf,%.4lf,%ls",
                    'C' + set,o,bmi.x1,bmi.y1,bmi.x2,bmi.y2,bmi.filename);
  if ((at = new RCCHAR[strlen(attribute) + 1]) != NULL)
    {  strcpy(at,attribute);
       e->addat(at);
       e->setattribute(at);
       db.saveundo(UD_INSERTATT,e);
    }
}

class RTTexturetypeDialogControl : public ListDialogControl
  {public:
    RTTexturetypeDialogControl(int id,int n,RCCHAR **l,int *i) : ListDialogControl(id,n,l,i) {}
    void load(Dialog *);
    void changefocus(Dialog *,int updatescreen = 1);
  };

void RTTexturetypeDialogControl::load(Dialog *dialog)
{int i;
  for (i = 0 ; i < n ; i++)
    SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)list[i]));
  dialog->SetDlgItemText(id,list[*value]);
  changefocus(dialog);
}

void RTTexturetypeDialogControl::changefocus(Dialog *dialog,int)
{int i,texturetype;
 static int map2d[36] = { 102,103,104,105,106,107,
                          /*202,203,204,205,206,207,*/
                          /*302,303,304,305,306,307,*/
                          /*1103,1104,1105,1106,*/
                          119,113,114,115,112,117
                          /*,1113,1114,1115,1116,*/
                          /*312,313,314,315*/
                        };
 static int solid[12] = {  109,110,111/*,209,210,211,309,310,311,1109,1110,1111*/  };
  dialog->currentvalue(id,&texturetype);
  for (i = 0 ; i < 36 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),map2d[i]),texturetype == 1);
  for (i = 0 ; i < 12 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),solid[i]),texturetype >= 2);
}

/*
 //  defined in ncdialog.h
 //
class TextureDisplayDialogControl : public DisplayDialogControl
  {private:
     char lastfilename[300];
     int exists;
   public:
     TextureDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(lastfilename,"");  exists = 0;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void TextureDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
    RCCHAR bitmapfilename[300];
    FILE *infile;
    int texturetype;

    QWidget *widget = 0;
    if(dialog->gethdlg())
        widget = dialog->getcontrol(118)->getQW();

    if(qobject_cast<QGraphicsView *>(widget) != 0)
    {
        QGraphicsScene *scene=0;
        if (dialog->currentvalue(100,&texturetype) &&
            dialog->currentvalue(102,bitmapfilename,300) && texturetype == 1)
          {
             if (strlen(bitmapfilename) > 0 && strcmp(bitmapfilename,lastfilename) != 0)
             {
                  infile = fopen(bitmapfilename,"r");
                  if (infile != 0)
                  {
                       fclose(infile);
                       exists = 1;
                  }
                  else
                    exists = 0;
                  strcpy(lastfilename,bitmapfilename);
             }
             if (exists)
             {
                 QString qfilepath(bitmapfilename);
                 QPixmap image(qfilepath);
                 if(!image.isNull())
                 {
                     if(((QGraphicsView*)widget)->scene() == 0)
                     {
                         scene = new QGraphicsScene();
                         scene->setSceneRect(image.rect());
                         ((QGraphicsView*)widget)->setScene(scene);
                         ((QGraphicsView*)widget)->setSceneRect(image.rect());
                     }
                     else
                     {
                         scene = ((QGraphicsView*)widget)->scene();
                         scene->clear();
                         scene->setSceneRect(image.rect());
                         qDebug() << "scene Rect"<< scene->sceneRect();
                         ((QGraphicsView*)widget)->setSceneRect(image.rect());
                     }
                     qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
                     qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
                     qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
                     qDebug() << "scene Rect"<< scene->sceneRect();

                     QGraphicsPixmapItem *gitem = ((QGraphicsView*)widget)->scene()->addPixmap(image);
                     scene->setSceneRect(gitem->boundingRect());
                     qDebug() << scene->sceneRect();
                     QRectF srect = scene->sceneRect();
                     QRectF nrect = srect;

                     if(srect.height() >= srect.width())
                     {
                         nrect.setWidth(srect.height());
                         nrect.setHeight(srect.height());
                     }
                     else
                     {
                         nrect.setWidth(srect.width());
                         nrect.setHeight(srect.width());
                     }
                     scene->setSceneRect(nrect);
                     nrect = scene->sceneRect();
                     qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
                     qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
                     qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
                     qDebug() << "scene Rect"<< scene->sceneRect();
                     ((QGraphicsView*)widget)->fitInView(nrect,Qt::KeepAspectRatio);
                     ((QGraphicsView*)widget)->centerOn(nrect.center());
                     ((QGraphicsView*)widget)->show();
                     ((QGraphicsView*)widget)->setProperty("bbox",scene->itemsBoundingRect());
                 }
             }
        }
    }
#if 0
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 int texturetype;
 EntityHeader header(0,0,0,0);
 BitMask options(32);
 FILE *infile;
 RCCHAR bitmapfilename[300];

  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
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
       if (dialog->currentvalue(100,&texturetype) && 
           dialog->currentvalue(102,bitmapfilename,300) && texturetype == 1)
         {  if (strlen(bitmapfilename) > 0 && strcmp(bitmapfilename,lastfilename) != 0)
              {  infile = fopen(bitmapfilename,"r");
                 if (infile != 0)
                   {  fclose(infile);
                      exists = 1;
                   }
                 else
                   exists = 0;
                 strcpy(lastfilename,bitmapfilename);
              }
            if (exists)
              {  Point org(output.getumin(),output.getvmin(),0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
                 BitMapE bitmape(bitmapfilename,org,xaxis,yaxis,output.getumax()-output.getumin(),output.getvmax()-output.getvmin(),1.0,1.0,1.0,0.004,1.0,options);
                 bitmape.draw(DM_NORMAL,NULL,&output);
              }
         }
    }
#endif
}

class RTColourButtonDialogControl : public ButtonDialogControl
{
protected:
    int baseid;
public:
     RTColourButtonDialogControl(int id, int bid) : ButtonDialogControl(id) { baseid = bid; }
     int select(Dialog *);
};

int RTColourButtonDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    //index = db.header.getcolour();
    index = -1;
    ((RealDialogControl*)parentdialog->getcontrol(baseid))->currentvalue(parentdialog,&red);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->currentvalue(parentdialog,&green);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->currentvalue(parentdialog,&blue);

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Transparent Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = red;
    lastGreen = green;
    lastBlue  = blue;

    if(dialog.process() == TRUE)
    {
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,red);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
    }
    else
    {
        // reset the state
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,lastRed) ;
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,lastGreen);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,lastBlue);
    }

    db.setnumericdisplaytolerance(n);

    return 0;
}

class RTMaterialButtonDialogControl : public ButtonDialogControl
  {private:
     TextureMapInfo *tmi;
     BumpMapInfo *bmi;
     BitMask *change;
     int action,stringid;
   public:
     RTMaterialButtonDialogControl(int id,int a,int id1,TextureMapInfo *ti,BumpMapInfo *bi,BitMask *c) : ButtonDialogControl(id)  {  action = a;  stringid = id1;  tmi = ti;  bmi = bi;  change = c;  }
     int select(Dialog *);
  };

int RTMaterialButtonDialogControl::select(Dialog *dialog)
{int i;
  if (action == 0)
    {Dialog dialog("Texture_Dialog");
     DialogControl *dc;
     ResourceString rs16(NCRAY+16);
     ResourceString rs17(NCRAY+17);
     ResourceString rs25(NCRAY+25);
     ResourceString rs33(NCRAY+33);
     ResourceString rs34(NCRAY+34);
     ResourceString rs35(NCRAY+35);
     ResourceString rs36(NCRAY+36);
     ResourceString rs37(NCRAY+37);
     ResourceString rs38(NCRAY+38);
     ResourceString rs39(NCRAY+39);
     RCCHAR *texturelist[9];
       texturelist[0] = rs33.gets();
       texturelist[1] = rs16.gets();
       texturelist[2] = rs17.gets();
       texturelist[3] = rs34.gets();
       texturelist[4] = rs35.gets();
       texturelist[5] = rs36.gets();
       texturelist[6] = rs37.gets();
       texturelist[7] = rs38.gets();
       texturelist[8] = rs39.gets();
       dialog.add(new RTTexturetypeDialogControl(100,9,texturelist,&tmi->texturetype));
       dialog.add(new StringDialogControl(102,tmi->filename,256));
       dialog.add(dc = new RealDialogControl(103,&tmi->x1,0.1));
       //dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(dc = new RealDialogControl(104,&tmi->y1,0.1));
       //dialog.add(new ScrollBarDialogControl(1104,dc));
       dialog.add(dc = new RealDialogControl(105,&tmi->x2,0.1));
       //dialog.add(new ScrollBarDialogControl(1105,dc));
       dialog.add(dc = new RealDialogControl(106,&tmi->y2,0.1));
       //dialog.add(new ScrollBarDialogControl(1106,dc));
       dialog.add(new CheckBoxDialogControl(107,&tmi->options,0));
       dialog.add(dc = new RealDialogControl(109,&tmi->size,ScaleByLength));
       //dialog.add(new ScrollBarDialogControl(1109,dc));
       dialog.add(dc = new RealDialogControl(110,&tmi->patternscale));
       //dialog.add(new ScrollBarDialogControl(1110,dc));
       dialog.add(dc = new RealDialogControl(111,&tmi->colourscale));
       //dialog.add(new ScrollBarDialogControl(1111,dc));
       dialog.add(new RTMaterialButtonDialogControl(112,4,102,tmi,bmi,change));
       dialog.add(new CheckBoxDialogControl(119,&tmi->options,1));
       dialog.add(dc = new RealDialogControl(113,&tmi->transparentcolour.red,0.1,0.0,1.0));
       //dialog.add(new ScrollBarDialogControl(1113,dc));
       dialog.add(dc = new RealDialogControl(114,&tmi->transparentcolour.green,0.1,0.0,1.0));
       //dialog.add(new ScrollBarDialogControl(1114,dc));
       dialog.add(dc = new RealDialogControl(115,&tmi->transparentcolour.blue,0.1,0.0,1.0));
       //dialog.add(new ScrollBarDialogControl(1115,dc));
       dialog.add(new RTColourButtonDialogControl(117,113));
       dialog.add(new TextureDisplayDialogControl(118));


       if (change != 0)
       {
            dialog.title(rs25.gets());
            //for (i = 0 ; i <= 12 ; i++)
            //  dialog.add(new CheckBoxDialogControl(200+i,change,i+14));
            //dialog.add(new CheckBoxDialogControl(213,change,34));
            //dialog.add(new CheckBoxDialogControl(219,change,35));
       }


       dialog.process();
    }
  else if (action == 1)
    {Dialog dialog("Bumpmap_Dialog");
     DialogControl *dc;
     ResourceString rs26(NCRAY+26);
       dialog.add(new StringDialogControl(100,bmi->filename,256));
       dialog.add(dc = new RealDialogControl(101,&bmi->x1,0.1));
       //dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(dc = new RealDialogControl(102,&bmi->y1,0.1));
       //dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(dc = new RealDialogControl(103,&bmi->x2,0.1));
       //dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(dc = new RealDialogControl(104,&bmi->y2,0.1));
       //dialog.add(new ScrollBarDialogControl(1104,dc));
       dialog.add(new CheckBoxDialogControl(105,&bmi->options,0));
       dialog.add(new RTMaterialButtonDialogControl(106,3,100,tmi,bmi,change));


       if (change != 0)
       {
           dialog.title(rs26.gets());
           //for (i = 0 ; i < 6 ; i++)
           //   dialog.add(new CheckBoxDialogControl(200+i,change,i+27));
       }

       dialog.process();
    }
  else if (action == 3 || action == 4)
    {OPENFILENAME ofn;
     RCCHAR szDirName[256],szFile[256],szFileTitle[256],title[256],szFilter[256];
     RCUINT  i;
     ResourceString rs5(NCRAY+5);
     ResourceString rs13(NCRAY+13);
     ResourceString rs14(NCRAY+14);
     ResourceString rs15(NCRAY+15);
      _getcwd(szDirName, sizeof(szDirName));
      szFile[0] = '\0';
      if (action == 3)
        _tcscpy(szFilter,rs14.getws());
      else
        _tcscpy(szFilter,rs5.getws());
      //for (i = 0; szFilter[i] != '\0'; i++)
      //  if (szFilter[i] == '|') szFilter[i] = '\0';
      memset(&ofn, 0, sizeof(OPENFILENAME));
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = dialog->gethdlg();
      ofn.lpstrFilter = szFilter;
      ofn.nFilterIndex = 1;
      ofn.lpstrFile= szFile;
      ofn.nMaxFile = sizeof(szFile);
      ofn.lpstrFileTitle = szFileTitle;
      ofn.nMaxFileTitle = sizeof(szFileTitle);
      ofn.lpstrInitialDir = szDirName;
      if (action == 3)
        ofn.lpstrDefExt = _RCT("BMF");
      else
        ofn.lpstrDefExt = _RCT("BMP");
      ofn.lpstrFileTitle = title;
      if (action == 3)
        ofn.lpstrTitle = rs13.getws();
      else
        ofn.lpstrTitle = rs15.getws();
      ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
      if (GetOpenFileName(&ofn))
        { 
           ((StringDialogControl *)dialog->getcontrol(stringid))->change(dialog,ofn.lpstrFile);
           if (dialog->getcontrol(stringid+100) != 0)
             ((CheckBoxDialogControl *)dialog->getcontrol(stringid+100))->change(dialog,1);

           //InvalidateRect(GetDlgItem(dialog->gethdlg(),118),0,TRUE);
           // texture map selection
           if(action == 4)
               ((Texture_Dialog*)dialog->gethdlg())->drawSample();
        }
    }
  return 0;
}

class RTShaderDialogControl : public ListDialogControl
  {public:
    RTShaderDialogControl(int id,int n,RCCHAR **l,int *i) : ListDialogControl(id,n,l,i) {}
    void load(Dialog *);
    void changefocus(Dialog *,int updatescreen = 1);
  };

void RTShaderDialogControl::load(Dialog *dialog)
{int i;
  for (i = 0 ; i < n ; i++)
    SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)list[i]));
  dialog->SetDlgItemText(id,list[*value]);
  changefocus(dialog);
}

void RTShaderDialogControl::changefocus(Dialog *dialog,int)
{int i,shader;
 static int phong[20] = { 105,106,107,108,109 };
 static int general[12] = {  111,112,113 };

  dialog->currentvalue(id,&shader);
  for (i = 0 ; i < 20 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),phong[i]),shader == 0);
  for (i = 0 ; i < 12 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),general[i]),shader != 0);
}

int SurfaceProperties::setup(int c)
{
 Dialog dialog("SurfaceProperty_Dialog");
 DialogControl *dc;
 ResourceString rs11(NCRAY+11);
 ResourceString rs12(NCRAY+12);
 ResourceString rs24(NCRAY+24);
 RCCHAR *shaderlist[2];
 int i;
  shaderlist[0] = rs11.gets();
  shaderlist[1] = rs12.gets();
  dialog.add(new RTShaderDialogControl(100,2,shaderlist,&shader));
  dialog.add(new RealDialogControl(101,&transparency,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(new CheckBoxDialogControl(102,&options,0));
  dialog.add(new CheckBoxDialogControl(103,&options,1));
  dialog.add(new RealDialogControl(105,&ambient,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(new RealDialogControl(106,&diffuse,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(new RealDialogControl(107,&specular,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(new RealDialogControl(108,&exponent,1.0,1.0,1000.0));
  //dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(new RealDialogControl(109,&reflectivity,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(new RealDialogControl(111,&smoothness,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(new RealDialogControl(112,&metallic,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(new RealDialogControl(113,&refractiveindex,0.1,1E-10,1000.0));
  //dialog.add(new ScrollBarDialogControl(1113,dc));
  dialog.add(new RTMaterialButtonDialogControl(114,0,0,&tmi,&bmi,(c == 1 ? &chang : 0)));
  dialog.add(new RTMaterialButtonDialogControl(115,1,0,&tmi,&bmi,(c == 1 ? &chang : 0)));
  if (c == 1)
    {
      // we force all items to changed
       dialog.title(rs24.gets());
       //chang.clearall();
       chang.setall();
       //for (i = 0 ; i < 14 ; i++)
       //  dialog.add(new CheckBoxDialogControl(200+i,&chang,i));
    }
  return dialog.process() == TRUE;
}

struct RBRaytrace1_data
  {int cstate;
   double height,radius,size;
   Point p1,p2,xaxis,yaxis;
  };

void RBRaytrace1(int dm,void *data,Point *p2,View3dSurface *surface)
{RBRaytrace1_data *rbdata = (RBRaytrace1_data *) data;
 Point xaxis,yaxis,zaxis,p1,p3,p4,p5,v1;
 int i,ilast,defined;
 double radius;

 if (dm == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (surface != 0)
    {  rbdata->xaxis = surface->getuaxis();
       rbdata->yaxis = surface->getvaxis();
       rbdata->size = 3.0 * ((surface->getumax() - surface->getumin()) / surface->getwidthmm());
    }
  if (rbdata->cstate == 1)
    p1 = *p2 + db.workplanes.getcurrent()->getzaxis() * rbdata->height;
  else
    p1 = rbdata->p1;
  for (i = 0 ; i < 8 ; i++)
    {  v1 = rbdata->xaxis * (i < 5 ? 2 - i : i - 6) +
            rbdata->yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rbdata->size;
       Line line(p1 + v1 * 0.6,p1 + v1);
       if (line.getdefined())
         line.draw(DM_INVERT);
    }
  if (rbdata->cstate == 2 || rbdata->cstate == 3)
    {  InfinitePlane ip(db.workplanes.getcurrent()->getzaxis(),db.workplanes.getcurrent()->getorigin());
       if (rbdata->cstate == 2)
         rbdata->p2 = *p2;
       if (fabs(db.workplanes.getcurrent()->getzaxis().dot((rbdata->p1 - rbdata->p2).normalize())) > cos(0.01))
         xaxis = db.workplanes.getcurrent()->getzaxis();
       else
         xaxis = db.workplanes.getcurrent()->getzaxis().cross(rbdata->p1 - rbdata->p2).normalize();
       yaxis = (rbdata->p1 - rbdata->p2).cross(xaxis).normalize();
       if (rbdata->cstate == 3)
         {Line line(rbdata->p1,rbdata->p2);
            p5 = line.nearp(*p2);
            radius = (*p2 - p5).length();
         }
       else
         {  radius = (rbdata->p2 - rbdata->p1).length() / 4.0;
            p5 = rbdata->p2;
         }
       for (i = 0,defined = 0,ilast = -1 ; i < 16 ; i++)
         {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
            Line line(rbdata->p1,p5 + v1 * radius);
            Intersect ints(&line,&ip);
            if (ints.nintersections() > 0)
              {  p4 = ((PointE *) ints.intersection(0))->getp();
                 if ((rbdata->p2 - rbdata->p1).dot(p4 - rbdata->p1) > 0.0)
                   {  if (i == 0 || i == 15)
                        defined++;
                      if (i == 0)
                        p3 = p4;
                      else if (ilast + 1 == i)
                        {  Line line2(p1,p4);
                           if (line2.getdefined())
                             line2.draw(DM_INVERT);
                        }
                      p1 = p4;
                      ilast = i;
                   }
              }
         }
       if (defined == 2)
         {  Line line3(p1,p3);
            if (line3.getdefined())
              line3.draw(DM_INVERT);
         }
       if (rbdata->cstate == 2)
         {Line line4(rbdata->p1,rbdata->p2);
           if (line4.getdefined())
             line4.draw(DM_INVERT);
         }
     }
}

/**
 * @brief raytrace_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Insert a raytrace light command
 * opengl lights can also use these settings
 *
 * the user clicks on the screen and inputs a height
 */
void raytrace_command1(int *cstate,HighLevelEvent *event,void **data)
{RBRaytrace1_data *rbdata = (RBRaytrace1_data *) *data;
 ResourceString rs18(NCRAY+18),rs19(NCRAY+19),rs20(NCRAY+20),rs21(NCRAY+21),rs22(NCRAY+22);
 Point  p3,p4;
 Light *light;
 double height,i,s,fd,fe,ca,co;
 BitMask o(32);
 Dialog dialog(rs18.gets(),rs19.gets(),&height,ScaleByLength);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBRaytrace1_data;  *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           *cstate = 1;
         height = v.getreal("li::height");
         if (dialog.process() == TRUE)
           {  rbdata->height = height;
              v.setreal("li::height",height);
              rbdata->radius = 10.0;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp() + db.workplanes.getcurrent()->getzaxis() * rbdata->height;
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p3 = ((Coordinate3dEvent *) event)->getp();
              Light::getdefaults(&i,&s,&fd,&fe,&ca,&co,&o);
              Line line(rbdata->p1,rbdata->p2);
              p4 = line.nearp(p3);
              ca = atan2((p3 - p4).length(),(rbdata->p1 - p4).length()) * 180.0 / M_PI;
              light = new Light(rbdata->p1,rbdata->p2,i,s,fd,fe,ca,co,o);
              if (light != 0 && light->getdefined())
                db.geometry.add(light);
              *cstate = 1;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2 || *cstate == 3)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 1)
    {  rbdata->cstate = 1;
       rubberband.begin(0);
       rubberband.add(RBRaytrace1,rbdata);
       cadwindow->prompt->normalprompt(rs20.gets());
    }
  else if (*cstate == 2)
    {  rbdata->cstate = 2;
       rubberband.begin(0);
       rubberband.add(RBRaytrace1,rbdata);
       cadwindow->prompt->normalprompt(rs21.gets());
    }
  else if (*cstate == 3)
    {  rbdata->cstate = 3;
       rubberband.begin(0);
       rubberband.add(RBRaytrace1,rbdata);
       cadwindow->prompt->normalprompt(rs22.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}


class ILListDialogControl : public ListDialogControl
  {private:
     RCCHAR city[300];
   public:
     ILListDialogControl(int id,RCCHAR *c) : ListDialogControl(id,0,0,c,300)  {  strcpy(city,c);  };
     void load(Dialog *);
  };

void ILListDialogControl::load(Dialog *dialog)
{
 char line[300],*s;
 RCCHAR rcs[300];
 FILE *infile;

  removeallitems(dialog);

  if ((infile = fopen(home.getpublicfilename("Cities.dat"),"r")) != 0)
  {
      while (fgets(line,300,infile) != 0)
      {
          s = strtok(line,",");
          strcpy(rcs,s);
          additem(dialog,rcs);
      }
      fclose(infile);
  }

  change(dialog,city);

}

class ILDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR lastcity[300];
   public:
     ILDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(lastcity,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

void ILDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
    RCCHAR city[300],filename[300];
    char line[300],*s;

  if (dialog->currentvalue(102,city,300) != 0 && strcmp(city,lastcity) != 0)
  {
      FILE *infile;

       strcpy(filename,home.getpublicfilename("Cities.DAT"));

       if ((infile = fopen(filename,"r")) != 0)
       {
           while (fgets(line,300,infile) != 0)
           {
                 s = strtok(line,",");
                 if (strcmp(s,city) == 0)
                 {
                      s = strtok(0,",");
                      ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,atof(s != 0 ? s : ""));
                      s = strtok(0,",");
                      ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,atof(s != 0 ? s : ""));
                      s = strtok(0,",");
                      ((RealDialogControl *)dialog->getcontrol(105))->change(dialog,atof(s != 0 ? s : ""));
                 }
           }
           fclose(infile);
       }
  }

  strcpy(lastcity,city);

}

double ut(double h,double m,double z)
{ return (h-z+m/60.0);
}

double jd(double y,double m,double d,double u)
{ return (367*y)-floor((7.0/4.0)*(floor((m+9.0)/12.0)+y))+floor(275.0*m/9.0)+d-730531.5+(u/24);
}

double frm(double n)
{ return floor(n * 100.0 + 0.5) / 100.0;
}

double Azimuth(double lg,double la,double ye,double mo,double da,double ho,double mi,double zo)
{double uu,jj,T,k,Lo,DL,eps,RA,GMST,LMST,H,L,M,eqt,azm,delta;
  uu = ut(ho,mi,zo);
  jj = jd(ye,mo,da,uu);
  T = jj / 36525.0;
  k = acos(-1.0) / 180.0;
  M = 357.5291 + 35999.0503*T - 0.0001559*T*T - 0.00000045*T*T*T;
  M = fmod(M,360.0);
  Lo = 280.46645 + 36000.76983*T + 0.0003032*T*T;
  Lo = fmod(Lo,360.0);
  DL = (1.9146-0.004817*T-0.000014*T*T)*sin(k*M) + (0.019993-0.000101*T)*sin(k*2*M) + 0.00029*sin(k*3*M);
  L = Lo + DL;
  eps = 23.43999-0.013*T;
  delta = (1/k)*asin(sin(L*k)*sin(eps*k));
  RA = (1/k)*atan2(cos(eps*k)*sin(L*k),cos(L*k));
  RA = fmod(RA + 360.0,360.0);
  GMST = 280.46061837 + 360.98564736629*jj + 0.000387933*T*T - T*T*T/38710000.0;
  GMST = fmod(GMST+360.0,360.0);
  LMST = GMST + lg;
  H = LMST - RA;
  eqt = (Lo-RA) * 4.0;
  azm = (1/k)*atan2(-sin(H*k),cos(la*k)*tan(delta*k)-sin(la*k)*cos(H*k));
  azm = fmod(azm+360.0,360.0);
  return azm;
}

double Altitude(double lg,double la,double ye,double mo,double da,double ho,double mi,double zo)
{double uu,jj,T,k,M,Lo,DL,eps,delta,RA,GMST,LMST,H,L,eqt,alt;

  uu = ut(ho,mi,zo);
  jj = jd(ye,mo,da,uu);
  T = jj / 36525.0;
  k = acos(-1.0)/180.0;
  M = 357.5291+35999.0503*T - 0.0001559*T*T - 0.00000045*T*T*T;
  M = fmod(M,360.0);
  Lo = 280.46645 + 36000.76983*T + 0.0003032*T*T;
  Lo = fmod(Lo,360.0);
  DL = (1.9146-0.004817*T-0.000014*T*T)*sin(k*M)+(0.019993-0.000101*T)*sin(k*2*M)+0.00029*sin(k*3*M);
  L = Lo+DL;
  eps = 23.43999-0.013*T;
  delta = (1/k)*asin(sin(L*k)*sin(eps*k));
  RA = (1/k)*atan2(cos(eps*k)*sin(L*k),cos(L*k));
  RA = fmod((RA+360),360.0);
  GMST = 280.46061837+360.98564736629*jj+0.000387933*T*T-T*T*T/38710000;
  GMST = fmod(GMST+360.0,360.0);
  LMST = GMST+lg;
  H = LMST - RA;
  eqt = (Lo-RA)*4;
  alt = (1/k)*asin(sin(la*k)*sin(delta*k)+cos(la*k)*cos(delta*k)*cos(H*k));

  return alt;
}


// KMJ:
// modified for v8 to be the same as the LANDWorksCAD sun position command
// inserting a NorthDirection or NorthPoint figure file is now implemented
//
void insertlightatsunposition_command(int *cstate,HighLevelEvent *event,void **data)
{Dialog dialog("SunPosition_Dialog");
 RCCHAR sdate[300],stime[300],city[300],text[300];
 double timezone;
 int result;
 double lg,la,ye,mo,da,ho,mi,alt,azm,x;
 // KMJ: not using month names now
 //static RCCHAR *smonthnames[] = {  _RCT("Jan"), _RCT("Feb"), _RCT("Mar"), _RCT("Apr"), _RCT("May"), _RCT("Jun") ,_RCT("Jul") ,_RCT("Aug"), _RCT("Sep"), _RCT("Oct"), _RCT("Nov"), _RCT("Dec")  };
 Point p1,p2,rotation,scale;
 BitMask options(32);
 Byte state1,state2;
 static Point pickp1;
 static RCCHAR filename[300]={0};
 static bool fileExists = false;
 int keepExisting = 0;
 EntityList dblist;
 Entity *e;

 switch(*cstate)
 {
     case InitialState :
     {
      strcpy(sdate,"");
      strcpy(stime,"");
      strcpy(city,v.getstring("li::city"));
      lg = v.getreal("li::longitude");
      la = v.getreal("li::latitude");
      timezone = v.getreal("li::timezone");

      // KMJ: Note: data and time are QDateEdit and QTimeEdit widgets
      dialog.add(new StringDialogControl(100,sdate,300));
      dialog.add(new StringDialogControl(101,stime,300));
      dialog.add(new ILListDialogControl(102,city));
      dialog.add(new RealDialogControl(103,&lg));
      dialog.add(new RealDialogControl(104,&la));
      dialog.add(new RealDialogControl(105,&timezone));
      dialog.add(new ButtonDialogControl(106));
      dialog.add(new ILDisplayDialogControl(107));
      dialog.add(new CheckBoxDialogControl(107,&keepExisting));

      if ((result = dialog.process()) == IDOK)
      {
           v.setstring("li::city",city);
           v.setreal("li::longitude",lg);
           v.setreal("li::latitude",la);
           v.setreal("li::timezone",timezone);

           // internal date format : yyyy/mm/dd
           // might not be the same as the display format
           ye = 2000 + atoi(sdate);
           //ye = 2000 + atoi(sdate+2);
           // KMJ: this is old code not using month names with Qt
           // see the next code line
           /*
           sdate[6] = 0;
           for (mo = 1 ; mo <= 12 ; mo++)
             if (_stricmp(sdate+3,smonthnames[int(mo)-1]) == 0)
               break;
           */
           mo = atoi(sdate+5);
           da = atoi(sdate+8);

           // internal 24 hour time format HH:mm:ss
           // might not be the same as the display format
           //if (strlen(stime) != 12)
           if (strlen(stime) != 8)
             {  strcpy(text,"0");  strcat(text,stime);  strcpy(stime,text);
             }
           ho = atoi(stime);
           mi = atoi(stime+3);
           // KMJ:: this is old code, now using 24 hour notation
           /*
           if (ho == 12 && stime[9] == 'A')
             ho = 0;
           if (ho < 12 && stime[9] == 'P')
             ho += 12;
           */

           x = (fabs(lg) - fmod(fabs(lg),1.0)) + fmod(fabs(lg),1.0) / 0.60;
           lg = lg < 0.0 ? -x : x;

           x = (fabs(la) - fmod(fabs(la),1.0)) + fmod(fabs(la),1.0) / 0.60;
           la = la < 0.0 ? -x : x;

           alt = Altitude(lg,la,ye,mo,da,ho,mi,timezone);
           azm = Azimuth(lg,la,ye,mo,da,ho,mi,timezone);

           // KMJ: v8 enhancement to keep or delete existing sun lights
           // default is to delete existing
           // delete any exsting sun lights in the model if the option is set
           if(!keepExisting)
           {
               dblist = *db.geometry.getlist();
               for (dblist.start() ; (e = dblist.next()) != 0 ; )
               {
                   if (e->isa(light_entity))
                   {
                       AttributeList atlist;
                       RCCHAR *at;
                       atlist = e->getatlist();
                       for (atlist.start() ; (at = atlist.next()) != NULL ; )
                       {
                           if(strncmp(at,"RealCAD::SunPosition::",strlen("RealCAD::SunPosition::")) == 0)
                           {
                               EntityList list;
                               int cstate2,d;
                               list.add(e);
                               state.setselection(list,0);
                               delete_command(&cstate2,0,0);
                           }
                       }
                   }
               }
           }

           // KMJ: v8 enhancement to look for a north direction figure
           // if one is found adjust the angle in relation to it
           double angle = 0.0;
           dblist = *db.geometry.getlist();
           for (dblist.start() ; (e = dblist.next()) != 0 ; )
           {
               if (e->isa(figure_entity))
               {
                   QString qfilename(((Figure*)e)->getfilename());
                   if(qfilename.contains("northdirection.cad",Qt::CaseInsensitive) ||
                      qfilename.contains("northpoint.cad",Qt::CaseInsensitive))
                   {
                       angle = ((Figure*)e)->getrotation().z;
                   }
               }
           }

           //  Insert the new light using the north direction angle
           //p1.x = 10000 * cos(acos(-1.0) / 2.0 - (azm - 0.0) * acos(-1.0)/180.0);
           //p1.y = 10000 * sin(acos(-1.0) / 2.0 - (azm - 0.0) * acos(-1.0)/180.0);
           p1.x = 10000 * cos(acos(-1.0) / 2.0 - (azm - angle) * acos(-1.0)/180.0);
           p1.y = 10000 * sin(acos(-1.0) / 2.0 - (azm - angle) * acos(-1.0)/180.0);
           p1.z = 10000 * tan(alt*acos(-1.0)/180.0);
           p2 = p1;  p2.normalize();
           p2 = p1 - p2;
           options.set(0,1);
           options.set(1,1);
           Light *pLight = new Light(p1,p2,1.0,200.0,10000.0,1.0,45.0,1.0,options);
           if(pLight != 0)
           {
               db.geometry.add(pLight);
               // add the sun attributes to the light
               RCCHAR attribute[1024],*patt;
               sprintf(attribute,"RealCAD::SunPosition::Date:%ls|Time:%ls|Long:%.2lf|Lat:%.2lf|TZone:%.2lf",sdate,stime,lg,la,timezone);
               if((patt = new RCCHAR[strlen(attribute)+1]) != 0 )
               {
                   strcpy(patt,attribute);
                   pLight->addat(patt);
                   pLight->setattribute(patt);
                   db.saveundo(UD_INSERTATT,pLight);
               }
           }
           else
               cadwindow->MessageBox("Memory Error: Could not create a Light Entity","Insert Sun Light",MB_OK);

           *cstate = ExitState;
      }
      else if (result == 106)
      {
          // KMJ: v8 enhancement Insert a north indicator
          // look for the standard figure files
          QFile testFile;
          strcpy(filename,"");
          fileExists = false;
          if((fileExists = testFile.exists((QString(home.getpublicfilename("NorthDirection.cad"))))))
              strcpy(filename,home.getpublicfilename("NorthDirection.cad"));
          else if((fileExists = testFile.exists((QString(home.getpublicfilename("NorthDirection.CAD"))))))
              strcpy(filename,home.getpublicfilename("NorthDirection.CAD"));
          else if((fileExists = testFile.exists((QString(home.getpublicfilename("NorthPoint.cad"))))))
              strcpy(filename,home.getpublicfilename("NorthPoint.cad"));
          else if((fileExists = testFile.exists((QString(home.getpublicfilename("NorthPoint.CAD"))))))
              strcpy(filename,home.getpublicfilename("NorthPoint.CAD"));

          if(!fileExists)
          {
              cadwindow->MessageBox("NorthDirection.cad or NorthPoint.cad cannot be found.\nThe Y Axis will be used for North direction","North Direction",MB_OK);
              *cstate = ExitState;
          }
          else
          {
            rubberband.end(0);
            *cstate = 1;
          }
      }
      else
          *cstate = ExitState;
     }
     break;
     case 1:
      rubberband.end(0);
      if (event->isa(Coordinate3d))
      {
          pickp1 = ((Coordinate3dEvent *) event)->getp();
          cadwindow->rubberbanduv(pickp1);
          cadwindow->cursoraction(1);
          *cstate = 2;
      }
      else if (event->isa(Abort) || event->isa(Exit))
        *cstate = ExitState;
     break;
     case 2 :
     rubberband.end(0);
     cadwindow->cursoraction(0);
     if (event->isa(Coordinate3d))
     {
         p2 = ((Coordinate3dEvent *) event)->getp();
         if(fileExists && strlen(filename) > 0)
         {
             // remove any existing north figures
             EntityList dblist;
             Entity *e;
             dblist = *db.geometry.getlist();
             for (dblist.start() ; (e = dblist.next()) != 0 ; )
             {
                 if (e->isa(figure_entity))
                 {
                     QString qfilename(((Figure*)e)->getfilename());
                     if(qfilename.contains("northdirection.cad",Qt::CaseInsensitive) ||
                        qfilename.contains("northpoint.cad",Qt::CaseInsensitive))
                     {
                         EntityList list;
                         int cstate2,d;
                         list.add(e);
                         state.setselection(list,0);
                         delete_command(&cstate2,0,0);
                     }
                 }
             }
             // insert the new north figure
             options.clearall();
             Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
             db.geometry.add(new Figure(filename,_RCT(""),pickp1,(p2 - pickp1).cross(::Point(0.0,0.0,1.0)).normalize(),(p2 - pickp1).normalize(),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,state1,state2,0,options));
         }
         *cstate = ExitState;
     }
     else if (event->isa(Abort) || event->isa(Exit))
       *cstate = ExitState;
     break;
 }
 //
 //  About to enter state actions
 //
 if (*cstate == 1)
 {
      rubberband.begin(0);
      //rubberband.add(RBZoom2,rbdata);
      state.selmask.entity.clearandset(xyz_mask,end_list);
      cadwindow->prompt->normalprompt("Locate the origin of the north figure");
 }
 else if (*cstate == 2)
 {
      rubberband.begin(0);
      state.selmask.entity.clearandset(xyz_mask,end_list);
      cadwindow->prompt->normalprompt("Locate the direction of the north figure");
 }
 else if (*cstate == ExitState)
 {
     rubberband.end(0);
     cadwindow->cursoraction(0);
     program->setcursorshape(CursorRestore);
 }
}

int getLightChangeValues(BitMask *change,EntityList &list,EntityHeader *hd,double *in, double *s, double *fd, double *fe, double *ca, double *c,BitMask *o)
{
    Entity *e;
    BitMask options(32),toptions(32);
    Light *light;

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(light_entity))
        {
            light = (Light*)e;

            if(i==0)
            {
                hd->setcolour(light->getcolour());
                hd->setlayer(light->getlayer());
                hd->setstyle(light->getstyle());
                hd->setweight(light->getweight());

                *in  = light->getintensity();
                *s  = light->getsize();
                *fd = light->getfadedistance();
                *fe = light->getfadeexponent();
                *ca = light->getconeangle();
                *c  = light->getconcentration();
                options = light->getoptions();
                *o = light->getoptions();

                change->clearall();

                change->set(0);// def color
                change->set(1);// def layer
                change->set(2);// def style
                change->set(3);// def line weight

                change->set(4);// intensity
                change->set(5);// distant light (option 0)
                change->set(6);// white light (option 1)
                change->set(7);// size
                change->set(8);// enable fade (option 2)
                change->set(9);// fade distance
                change->set(10);// fade exponent
                change->set(11);// enable spot (option 3)
                change->set(12);// cone angle
                change->set(13);// concentration
            }
            else
            {
                toptions = light->getoptions();

                if(hd->getcolour() != light->getcolour())
                    change->clear(0);
                if(hd->getlayer() != light->getlayer())
                    change->clear(1);
                if(hd->getstyle() != light->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),light->getweight()))
                    change->clear(3);

                if(fabs(*in - light->getintensity()) > 10.0e-5)
                    change->clear(4);// intensity
                if(options.test(0) != toptions.test(0))
                    change->clear(5);// distant light (option 0)
                if(options.test(1) != toptions.test(1))
                    change->clear(6);// white light (option 1)
                if(fabs(*s - light->getsize()) > 10.0e-5)
                    change->clear(7);// size
                if(options.test(2) != toptions.test(2))
                    change->clear(8);// enable fade (option 2)
                if(fabs(*fd - light->getfadedistance()) > 10.0e-5)
                    change->clear(9);// fade distance
                if(fabs(*fe - light->getfadeexponent()) > 10.0e-5)
                    change->clear(10);// fade exponent
                if(options.test(3) != toptions.test(3))
                    change->clear(11);// enable spot (option 3)
                if(fabs(*ca - light->getconeangle()) > 10.0e-5)
                    change->clear(12);// cone angle
                if(fabs(*c - light->getconcentration()) > 10.0e-5)
                    change->clear(13); // concentration
            }
            i++;
        }
    }

    return i;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief raytrace_command2
 * @param cstate
 *
 * Light setup/verify dialog command
 *
 */
void raytrace_command2(int *cstate,HighLevelEvent *,void **)
{
 double i,s,fd,fe,ca,c;
 BitMask change(32),options(32);
 EntityList list;
 EntityHeader header;
 Entity *e;
 ResourceString rs7(NCRAY+7),rs8(NCRAY+8);
 int length,status=FALSE,found;

  *cstate = ExitState;
  list = state.getselection().copy();
  if (list.empty())
    {
       Light::getdefaults(&i,&s,&fd,&fe,&ca,&c,&options);
       if (Light::setup(NULL,&i,&s,&fd,&fe,&ca,&c,&options))
         Light::setdefaults(i,s,fd,fe,ca,c,options);
    }
  else
    {
       for (list.start() ; (e = list.next()) != NULL ; )
         if (! e->isa(light_entity))
           {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONINFORMATION);
              return;
           }
       Light::getdefaults(&i,&s,&fd,&fe,&ca,&c,&options);
       // get the change values for the selected ents and pass it to the dialog
       change.clearall();
       int found = getLightChangeValues(&change,list,&header,&i,&s,&fd,&fe,&ca,&c,&options);
       v.setinteger("op::changecount",found);
       if(found == 1)
       {
           // check to see if the selected light is a sunlight
           for (list.start() ; (e = list.next()) != NULL ; )
           {
                if (e->isa(light_entity))
                    break;
           }
           if(e->isa(light_entity))
           {
               // get the sunlight attributes
               RCCHAR *attribute;
               sunlightInfo sli;
               AttributeList atlist;
               atlist = e->getatlist();
               for (atlist.start() ; (attribute = atlist.next()) != 0 ;)
               {
                    // KMJ: added code for landworks sun position
                    if(strncmp(attribute,_RCT("RealCAD::SunPosition::"),strlen(_RCT("RealCAD::SunPosition::"))) == 0 ||
                       strncmp(attribute,_RCT("Landworks::SunPosition::"),strlen(_RCT("Landworks::SunPosition::"))) == 0)
                    {
                        QString sattribute(attribute);
                        int len = sattribute.lastIndexOf("::")+2;
                        sattribute = sattribute.remove(0,len);
                        //sattribute = sattribute.remove("RealCAD::SunPosition::");
                        QStringList sunprops = sattribute.split("|");
                        sli.Date = sunprops.at(0); sli.Date = sli.Date.remove("Date:");
                        sli.Time = sunprops.at(1); sli.Time = sli.Time.remove("Time:");
                        sli.Longitude = sunprops.at(2); sli.Longitude = sli.Longitude.remove("Long:");
                        sli.Latitude = sunprops.at(3); sli.Latitude = sli.Latitude.remove("Lat:");
                        sli.TimeZone = sunprops.at(4); sli.TimeZone = sli.TimeZone.remove("TZone:");
                        break;
                    }
               }
               if(attribute != 0 && (strncmp(attribute,_RCT("RealCAD::SunPosition::"),strlen(_RCT("RealCAD::SunPosition::"))) == 0 ||
                  strncmp(attribute,_RCT("Landworks::SunPosition::"),strlen(_RCT("Landworks::SunPosition::"))) == 0 ))
                   status = Light::setup(&change,&i,&s,&fd,&fe,&ca,&c,&options,&header,&sli);
               else
                   status = Light::setup(&change,&i,&s,&fd,&fe,&ca,&c,&options,&header);
           }
       }
       else if(found > 1)
           status = Light::setup(&change,&i,&s,&fd,&fe,&ca,&c,&options,&header);
       else
           status = Light::setup(NULL,&i,&s,&fd,&fe,&ca,&c,&options,&header);

       if (status == TRUE)
       {
            //list = state.getselection();
            length = list.length();
            RemoveLockedEntities(&list,0);
            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              ((Light *)e)->change(change,header,i,s,fd,fe,ca,c,options);
            db.saveundo(UD_ENDBLOCK,NULL);
            if (list.length() != length)
              cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

       }
    }
  *cstate = ExitState;
}

/**
 * @brief raytrace_command3
 * @param cstate
 *
 * Add/Change material parameters setup dialog command
 *
 */
void raytrace_command3(int *cstate,HighLevelEvent *,void **)
{SurfaceProperties sp;
 EntityList list;
 Entity *e;
 ResourceString rs23(NCRAY+23),rs24(NCRAY+24);
 BitMask mask(32);
  *cstate = ExitState;
  list = state.getselection();
  if (list.empty())
    {  sp.getdefaults();
       if (sp.setup(0))
         sp.setdefaults();
    }
  else
    {  mask.clearandset(surface_mask,end_list);
       for (list.start() ; (e = list.next()) != NULL ; )
         if (! e->match(mask))
           {  cadwindow->MessageBox(rs23.gets(),rs24.gets(),MB_ICONINFORMATION);
              return;
           }
       sp.getdefaults();
       if (sp.setup(1))
         {  list = state.getselection();
            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              sp.change(e,0);
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
  *cstate = ExitState;
}

class RTFilenameButtonDialogControl : public ButtonDialogControl
  {private:
    int stringid;
   public:
     RTFilenameButtonDialogControl(int id,int id1) : ButtonDialogControl(id) {  stringid = id1;  }
     int select(Dialog *);
  };

int RTFilenameButtonDialogControl::select(Dialog *dialog)
{int imageformat;
 OPENFILENAME ofn;
 RCCHAR szDirName[256],szFile[256], szFileTitle[256],szFilter[256],title[256];
 UINT  i;
 ResourceString rs3(NCRAY+3);
 ResourceString rs4(NCRAY+4);
 ResourceString rs5(NCRAY+5);
  imageformat = 0;
  dialog->currentvalue(100,&imageformat);
  //_getcwd(szDirName, sizeof(szDirName));
  strcpy(szDirName,home.getpublicfilename(""));

  if (imageformat == 0 || imageformat == 1)
    _tcscpy(szFilter,rs5.getws());
  else
    _tcscpy(szFilter,rs3.getws());
  //for (i = 0; szFilter[i] != '\0'; i++)
  //  if (szFilter[i] == '|') szFilter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  if (imageformat == 0 || imageformat == 1)
    {  ofn.lpstrDefExt = _RCT("BMP");
       _tcscpy(szFile,_RCT("*.BMP"));
    }
  else if (imageformat == 2)
    {  ofn.lpstrDefExt = _RCT("BMF");
       _tcscpy(szFile,_RCT("*.BMF"));
    }
  ofn.lpstrFileTitle = title;
  ofn.lpstrTitle =  rs4.getws();

  if (GetSaveFileName(&ofn))
   ((StringDialogControl *)dialog->getcontrol(stringid))->change(dialog,ofn.lpstrFile);

  return 0;
}

class RTBackimageButtonDialogControl : public ButtonDialogControl
  {private:
     int stringid;
   public:
     RTBackimageButtonDialogControl(int id,int id1) : ButtonDialogControl(id) {  stringid = id1;  }
     int select(Dialog *);
  };

// a subclass for selecting fog colour
class RTFogColourSelectButtonDialogControl : public SelectColourButtonDialogControl
{
public:
  RTFogColourSelectButtonDialogControl(int i,int bid)
      : SelectColourButtonDialogControl(i,bid)
  {
      strcpy(title,_RCT("Select Fog Colour"));
  }
};

// a subclass for selecting fog colour
class RTBackColourSelectButtonDialogControl : public SelectColourButtonDialogControl
{
public:
  RTBackColourSelectButtonDialogControl(int i,int bid)
      : SelectColourButtonDialogControl(i,bid)
  {
      strcpy(title,_RCT("Select Background Colour"));
  }
};

// a subclass for selecting fog colour
class RTAmbientColourSelectButtonDialogControl : public SelectColourButtonDialogControl
{
public:
  RTAmbientColourSelectButtonDialogControl(int i,int bid)
      : SelectColourButtonDialogControl(i,bid)
  {
      strcpy(title,_RCT("Select Ambient Colour"));
  }
};

int RTBackimageButtonDialogControl::select(Dialog *dialog)
{OPENFILENAME ofn;
 RCCHAR szDirName[256],szFile[256],szFileTitle[256],title[256],szFilter[256];
 UINT  i;
 ResourceString rs5(NCRAY+5);
 ResourceString rs6(NCRAY+6);
  _getcwd(szDirName, sizeof(szDirName));
  szFile[0] = '\0';
  _tcscpy(szFilter,rs5.getws());
  //for (i = 0; szFilter[i] != '\0'; i++)
  //  if (szFilter[i] == '|') szFilter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.lpstrDefExt = _RCT("BMP");
  ofn.lpstrFileTitle = title;
  ofn.lpstrTitle =  rs6.getws();
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
#if ! defined(_WIN32_WCE)
  if (GetOpenFileName(&ofn))
    ((StringDialogControl *)dialog->getcontrol(stringid))->change(dialog,ofn.lpstrFile);
//WCEFIX
#endif
  return 0;
}

int RayTraceImage::setup(void)
{Dialog dialog("Raytrace_Dialog");
 DialogControl *dc;
 ResourceString rs0(NCRAY);
 ResourceString rs1(NCRAY+1);
 ResourceString rs2(NCRAY+2);
 RCCHAR *imagelist[3];
 int width,height;
  imagelist[0] = rs0.gets();
  imagelist[1] = rs1.gets();
  imagelist[2] = rs2.gets();
  width = int(surface->getwidth());
  height = int(surface->getheight());
  imageformat = v.getinteger("rt::imageformat");
  strcpy(filename,v.getstring("rt::filename"));
  strcpy(backgroundimage,v.getstring("rt::backimage"));
  backgroundcolour.red = v.getreal("rt::backgroundcolour.red");
  backgroundcolour.green = v.getreal("rt::backgroundcolour.green");
  backgroundcolour.blue = v.getreal("rt::backgroundcolour.blue");
  ambientcolour.red = v.getreal("rt::ambientcolour.red");
  ambientcolour.green = v.getreal("rt::ambientcolour.green");
  ambientcolour.blue = v.getreal("rt::ambientcolour.blue");
  maximumrays = v.getinteger("rt::maximumrays");
  supersamplingrays = v.getinteger("rt::supersamplingrays");
  resolution = v.getinteger("rt::resolution");
  options = *v.getbm("rt::options");
  fogcolour.red = v.getreal("rt::fogcolour.red");
  fogcolour.green = v.getreal("rt::fogcolour.green");
  fogcolour.blue = v.getreal("rt::fogcolour.blue");
  fogdistance = v.getreal("rt::fogdistance");
  ambientstrength = v.getreal("rt::ambientstrength");
  dialog.add(new ListDialogControl(100,3,imagelist,&imageformat));
  dialog.add(new StringDialogControl(101,filename,256));
  dialog.add(new RTFilenameButtonDialogControl(116,101));
  dialog.add(new StringDialogControl(102,backgroundimage,256));
  dialog.add(new RTBackimageButtonDialogControl(117,102));
  dialog.add(dc = new RealDialogControl(103,&backgroundcolour.red,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&backgroundcolour.green,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&backgroundcolour.blue,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(new RTBackColourSelectButtonDialogControl(1000,103));
  dialog.add(dc = new RealDialogControl(106,&ambientcolour.red,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&ambientcolour.green,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(dc = new RealDialogControl(108,&ambientcolour.blue,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(new RTAmbientColourSelectButtonDialogControl(1001,106));
  dialog.add(dc = new RealDialogControl(109,&ambientstrength,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(dc = new IntegerDialogControl(110,&maximumrays,0,1000));
  //dialog.add(new ScrollBarDialogControl(1110,dc));
  dialog.add(new CheckBoxDialogControl(111,&options,0));
  dialog.add(dc = new RealDialogControl(112,&fogcolour.red,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(dc = new RealDialogControl(113,&fogcolour.green,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1113,dc));
  dialog.add(dc = new RealDialogControl(114,&fogcolour.blue,0.1,0.0,1.0));
  //dialog.add(new ScrollBarDialogControl(1114,dc));
  dialog.add(new RTFogColourSelectButtonDialogControl(1002,112));
  dialog.add(dc = new RealDialogControl(115,&fogdistance,1.0,0.0,1.0E30));
  //dialog.add(new ScrollBarDialogControl(1115,dc));
  dialog.add(dc = new IntegerDialogControl(121,&width,2,20000));
  //dialog.add(new ScrollBarDialogControl(1121,dc));
  dialog.add(dc = new IntegerDialogControl(122,&height,2,20000));
  //dialog.add(new ScrollBarDialogControl(1122,dc));
  dialog.add(dc = new IntegerDialogControl(123,&supersamplingrays,0,15));
  //dialog.add(new ScrollBarDialogControl(1123,dc));
  dialog.add(dc = new IntegerDialogControl(124,&resolution,1,100));
  //dialog.add(new ScrollBarDialogControl(1124,dc));
  if (dialog.process() == TRUE)
    {  v.setinteger("rt::imageformat",imageformat);
       v.setstring("rt::filename",filename);
       v.setstring("rt::backimage",backgroundimage);
       v.setreal("rt::backgroundcolour.red",backgroundcolour.red);
       v.setreal("rt::backgroundcolour.green",backgroundcolour.green);
       v.setreal("rt::backgroundcolour.blue",backgroundcolour.blue);
       v.setreal("rt::ambientcolour.red",ambientcolour.red);
       v.setreal("rt::ambientcolour.green",ambientcolour.green);
       v.setreal("rt::ambientcolour.blue",ambientcolour.blue);
       v.setreal("rt::ambientstrength",ambientstrength);
       v.setinteger("rt::maximumrays",maximumrays);
       v.setinteger("rt::supersamplingrays",supersamplingrays);
       v.setinteger("rt::resolution",resolution);
       v.setbm("rt::options",options);
       v.setreal("rt::fogcolour.red",fogcolour.red);
       v.setreal("rt::fogcolour.green",fogcolour.green);
       v.setreal("rt::fogcolour.blue",fogcolour.blue);
       v.setreal("rt::fogdistance",fogdistance);
       options.set(31);
       //surface->sendsizeevent(width,height);
       return 1;
    }
  else
    return 0;
}

RayTracePolygon::~RayTracePolygon()
{
}

Face::Face(Point o,Point xa,Point ya,EntityList list,int nsegs,double error)
{LinearEntity *e,*enext;
 int i,j,k,n,maxsegs,reverse;
 Point endp,*points;
 double d1,d2,d3,d4,u,v;
  defined = 0;
  origin = o;  xaxis = xa;  yaxis = ya;  zaxis = xa.cross(ya);
  for (list.start(),nloops = 1 ; ! list.atend() ; )
    {  e = (LinearEntity *)list.next();
       if (e == NULL) nloops++;
    }
  if ((nvert = new int[nloops]) == NULL) return;
  if ((uvs = new UV *[nloops]) == NULL)
    {  delete [] nvert;
       return;
    }
  maxsegs = 0;
  for (list.start(),i = 0,nvert[0] = 0 ; ! list.atend() ; )
    {  e = (LinearEntity *)list.next();
       if (e == NULL)
         nvert[++i] = 0;
       else
         {  if (nsegs == 0)
              n = e->nsegments(error);
            else
              n = nsegs;
            if (n > maxsegs)
              maxsegs = n;
            nvert[i] += n;
         }
    }
  if ((points = new Point[maxsegs]) == 0)
    return;
  for (i = 0 ; i < nloops ; i++)
    if (nvert[i] > 0 && (uvs[i] = new UV[nvert[i]]) == NULL)
      {  for (j = 0 ; j < i ; j++)
           delete [] uvs[j];
         delete [] uvs;  delete [] nvert;  delete [] points;
         return;
      }
    else if (nvert[i] == 0)
      uvs[i] = 0;

  umin = vmin =  1.0E38;
  umax = vmax = -1.0E38;
  for (list.start(),i = j = 0 ; ! list.atend() ; )
    {  reverse = 0;
       if ((e = (LinearEntity *)list.next()) != NULL)
         {  if (j == 0)
              {  if ((enext = (LinearEntity *)list.peek()) != NULL)
                   {  d1 = (e->start() - enext->start()).length();
                      d2 = (e->start() - enext->end()).length();
                      d3 = (e->end() - enext->start()).length();
                      d4 = (e->end() - enext->end()).length();
                      if (d1 > d2) d1 = d2;
                      if (d3 > d4) d3 = d4;
                      if (d3 < d1)
                        endp = e->end();
                      else
                        {  endp = e->start();   reverse = LE_REVERSE;
                        }
                   }
                 else
                   endp = e->end();
              }
            else
              {  d1 = (endp - e->start()).length();
                 d2 = (endp - e->end()).length();
                 if (d2 > d1)
                   endp = e->end();
                 else
                   {  endp = e->start();  reverse = LE_REVERSE;
                   }
              }
         }
       if (e == NULL)
         {  i++; j = 0;
         }
       else
         {  if (nsegs == 0)
              n = e->nsegments(error);
            else
              n = nsegs;
            e->segment(n,LE_INCLUDEEND | reverse,points);
            for (k = 0 ; k < n ; k++)
              {  u = xaxis.dot(points[k] - origin);
                 v = yaxis.dot(points[k] - origin);
                 if (u < umin) umin = u;
                 if (u > umax) umax = u;
                 if (v < vmin) vmin = v;
                 if (v > vmax) vmax = v;
                 uvs[i][j+k].u = u;
                 uvs[i][j+k].v = v;
              }
            j += n;
         }
    }
  d = -zaxis.dot(origin);
  defined = 1;
  delete [] points;
  mapbmu.x = mapu.x = 1.0 / (umax - umin);
  mapbmu.y = mapu.y = 0.0;
  mapbmu.z = mapu.z = -umin / (umax - umin);
  mapbmv.x = mapv.x = 0.0;
  mapbmv.y = mapv.y = 1.0 / (vmax - vmin);
  mapbmv.z = mapv.z = -vmin / (vmax - vmin);
}

Face::~Face()
{int i;
  if (defined)
    {  for (i = 0 ; i < nloops ; i++)
         delete [] uvs[i];
       delete [] uvs;  delete [] nvert;
    }
}

void Face::transform(Transform *t)
{Point p,o,xa,ya,za;
 double umin,vmin,umax,vmax,u,v;
 int i,j;
  o = t->transform(origin);
  xa = (t->transform(origin + xaxis) - o).normalize();
  ya = (t->transform(origin + yaxis) - o).normalize();
  za = xa.cross(ya);
  umin = vmin =  1.0E38;
  umax = vmax = -1.0E38;
  for (i = 0 ; i < nloops ; i++)
    {  for (j = 0 ; j < nvert[i] ; j++)
         {  p = t->transform(origin + xaxis * uvs[i][j].u / xaxis.dot(xaxis) + yaxis * uvs[i][j].v / yaxis.dot(yaxis));
            u = xa.dot(p - o);
            v = ya.dot(p - o);
            if (u < umin) umin = u;
            if (u > umax) umax = u;
            if (v < vmin) vmin = v;
            if (v > vmax) vmax = v;
            uvs[i][j].u = u;
            uvs[i][j].v = v;
         }
    }
  defined = 1;
  mapbmu.x = mapu.x = 1.0 / (umax - umin);
  mapbmu.y = mapu.y = 0.0;
  mapbmu.z = mapu.z = -umin / (umax - umin);
  mapbmv.x = mapv.x = 0.0;
  mapbmv.y = mapv.y = 1.0 / (vmax - vmin);
  mapbmv.z = mapv.z = -vmin / (vmax - vmin);
  origin = o;
  xaxis = xa;
  yaxis = ya;
  zaxis = za;
  d = -za.dot(o);
}

void Face::copy(VoxelPolygon *vp)
{Point p;
 int i,j,nv;
  vp->nloops = nloops;
  nv = 0;
  for (i = 0 ; i < nloops ; i++)
    {  vp->nvert[i] = nvert[i];
       vp->verts[i] = vp->vertices + nv;
       nv += nvert[i];
       for (j = 0 ; j < nvert[i] ; j++)
         {  p = vp->verts[i][j] = origin + xaxis * uvs[i][j].u / xaxis.dot(xaxis) + yaxis * uvs[i][j].v / yaxis.dot(yaxis);
            if (i == 0 && j == 0)
              vp->pmax = vp->pmin = p;
            else
              {  if (p.x < vp->pmin.x) vp->pmin.x = p.x;
                 if (p.x > vp->pmax.x) vp->pmax.x = p.x;
                 if (p.y < vp->pmin.y) vp->pmin.y = p.y;
                 if (p.y > vp->pmax.y) vp->pmax.y = p.y;
                 if (p.z < vp->pmin.z) vp->pmin.z = p.z;
                 if (p.z > vp->pmax.z) vp->pmax.z = p.z;
              }
         }
    }
}

void Face::getinfo(int *nl,int *nv,Point *pmin,Point *pmax)
{Point p;
 int i,j;
  *nl = nloops;
  *nv = 0;
  for (i = 0 ; i < nloops ; i++)
    {  *nv += nvert[i];
       for (j = 0 ; j < nvert[i] ; j++)
         {  p = origin + xaxis * uvs[i][j].u / xaxis.dot(xaxis) + yaxis * uvs[i][j].v / yaxis.dot(yaxis);
            if (p.x < pmin->x) pmin->x = p.x;
            if (p.x > pmax->x) pmax->x = p.x;
            if (p.y < pmin->y) pmin->y = p.y;
            if (p.y > pmax->y) pmax->y = p.y;
            if (p.z < pmin->z) pmin->z = p.z;
            if (p.z > pmax->z) pmax->z = p.z;
         }
    }
}

int Face::intersect(Point o,Point dn,double *t,UV *uv)
{int i,j,k,ntoleft;
 double u1,v1,u2,v2,u3,v3,u,v,vd,vo;
 Point p;
  vd = zaxis.dot(dn);
  if (fabs(vd) < 0.001)
    return 0;
  vo = -(zaxis.dot(o) + d);
  *t = vo / vd;
  if (*t < 1.0E-5)
    return 0;
  p = o + dn * *t;
  u = xaxis.dot(p - origin);
  v = yaxis.dot(p - origin);
  if (u < umin || u > umax || v < vmin || v > vmax)
    return 0;
  ntoleft = 0;
  for (i = 0 ; i < nloops ; i++)
   if (nvert[i] >= 3) 
    {  u1 = uvs[i][nvert[i]-1].u;  v1 = uvs[i][nvert[i]-1].v;
       for (j = 0 ; j < nvert[i] ; j++)
         {  u2 = uvs[i][j].u;
            v2 = uvs[i][j].v;
            if (v1 <= v && v2 >= v || v1 >= v && v2 <= v)
              {  if (v1 == v)
                   { // Do nothing if v1 == v
                   }
                 else
                   {  for (k = j + 1,v3 = v2 ; v3 == v ; k++)
                        v3 = uvs[i][k % nvert[i]].v;
                      if (v1 < v && v3 > v || v1 > v && v3 < v)
                        {  u3 = u1 + (u2 - u1) * (v - v1) / (v2 - v1);
                           ntoleft += u3 < u;
                        }
                   }
              }
            u1 = u2;  v1 = v2;
         }
    }
  if ((ntoleft % 2) == 0)
    return 0;
  else
    {  uv->u = u;
       uv->v = v;
       return 1;
    }
}

void Face::getcolour(Point dn,const UV &uv,RealColour *objectcolour,
                     Point *interp,Point *intern,SurfaceInfo **si)
{double u,v;
 Point p,normal;
  *interp = origin + xaxis * uv.u / xaxis.dot(xaxis) + yaxis * uv.v / yaxis.dot(yaxis);
  if (surfaceinfo->bmi != 0 && surfaceinfo->bmi->bumpmap != 0)
    {  u = mapbmu.x * uv.u + mapbmu.y * uv.v + mapbmu.z;
       v = mapbmv.x * uv.u + mapbmv.y * uv.v + mapbmv.z;
       if (surfaceinfo->bmi->swapxy != 0)
         surfaceinfo->bmi->bumpmap->getnormal(v,u,&normal);
       else
         surfaceinfo->bmi->bumpmap->getnormal(u,v,&normal);
       *intern = (xaxis * normal.x + yaxis * normal.y + zaxis * normal.z).normalize();
    }
  else
    *intern = zaxis;
  if (intern->dot(dn) > 0.0)
    *intern = -*intern;
  if (surfaceinfo->tmi != 0 && surfaceinfo->tmi->texturetype >= 2)
    {  solidtexture(*interp,intern,objectcolour);
       if (intern->dot(dn) > 0.0)
         *intern = -*intern;
    }
  else if (surfaceinfo->tmi != 0 && surfaceinfo->tmi->texturetype == 1 && surfaceinfo->tmi->texturemap != 0)
    {  u = mapu.x * uv.u + mapu.y * uv.v + mapu.z;
       v = mapv.x * uv.u + mapv.y * uv.v + mapv.z;
       if (surfaceinfo->tmi->swapxy != 0)
         surfaceinfo->tmi->texturemap->getcolour(v,u,objectcolour);
       else
         surfaceinfo->tmi->texturemap->getcolour(u,v,objectcolour);
       if (surfaceinfo->tmi->usetransparentcolour && 
               fabs(objectcolour->red-surfaceinfo->tmi->transparentcolour.red) < 0.01 &&
               fabs(objectcolour->green-surfaceinfo->tmi->transparentcolour.green) < 0.01 &&
               fabs(objectcolour->blue-surfaceinfo->tmi->transparentcolour.blue) < 0.01)
         {  objectcolour->red = objectcolour->green = objectcolour->blue = -0.01;
         }
    }
  else
    {  objectcolour->red = colour.red;
       objectcolour->green = colour.green;
       objectcolour->blue = colour.blue;
    }
  *si = surfaceinfo;
}

Triangle::Triangle(int f,Point p1,Point p2,Point p3,Point n1,Point n2,Point n3,UV uv1,UV uv2,UV uv3)
{double dist;
  flipped = f;
  defined = 0;
  origin = p1;
  xaxis = p2 - p1;
  dist = xaxis.dot(xaxis);
  if (dist < 1.0E-10) return;
  xaxis /= dist;
  yaxis = p3 - p1;
  zaxis = xaxis.cross(yaxis).normalize();
  yaxis = zaxis.cross(xaxis).normalize();
  dist = yaxis.dot(p3 - origin);
  if (dist < 1.0E-10) return;
  yaxis /= dist;
  x1 = xaxis.dot(p3 - origin);
  x2 = 1.0 - x1;
  d = -zaxis.dot(origin);
  mapbmu.x = mapu.x = uv2.u - uv1.u;
  mapbmu.y = mapu.y = uv3.u - uv1.u - mapu.x * x1;
  mapbmu.z = mapu.z = uv1.u;
  mapbmv.x = mapv.x = uv2.v - uv1.v;
  mapbmv.y = mapv.y = uv3.v - uv1.v - mapv.x * x1;
  mapbmv.z = mapv.z = uv1.v;
  mapnx.x = n2.x - n1.x;
  mapnx.y = n3.x - n1.x - mapnx.x * x1;
  mapnx.z = n1.x;
  mapny.x = n2.y - n1.y;
  mapny.y = n3.y - n1.y - mapny.x * x1;
  mapny.z = n1.y;
  mapnz.x = n2.z - n1.z;
  mapnz.y = n3.z - n1.z - mapnz.x * x1;
  mapnz.z = n1.z;
  defined = 1;
}

void Triangle::transform(Transform *t)
{Point p1,p2,p3,p4,p5,p6,n1,n2,n3;
 double dist;
  defined = 0;
  p4 = origin;
  p5 = origin + xaxis / xaxis.dot(xaxis);
  p6 = origin + xaxis * x1 / xaxis.dot(xaxis) + yaxis / yaxis.dot(yaxis);
  p1 = t->transform(p4);
  p2 = t->transform(p5);
  p3 = t->transform(p6);
  n1.x = mapnx.z;
  n1.y = mapny.z;
  n1.z = mapnz.z;
  n2.x = mapnx.x + mapnx.z;
  n2.y = mapny.x + mapny.z;
  n2.z = mapnz.x + mapnz.z;
  n3.x = mapnx.x * x1 + mapnx.y + mapnx.z;
  n3.y = mapny.x * x1 + mapny.y + mapny.z;
  n3.z = mapnz.x * x1 + mapnz.y + mapnz.z;
  n1 = (t->transform(n1+p4) - p1).normalize();
  n2 = (t->transform(n2+p5) - p2).normalize();
  n3 = (t->transform(n3+p6) - p3).normalize();

  origin = p1;
  xaxis = p2 - p1;
  dist = xaxis.dot(xaxis);
  if (dist < 1.0E-10) return;
  xaxis /= dist;
  yaxis = p3 - p1;
  zaxis = xaxis.cross(yaxis).normalize();
  yaxis = zaxis.cross(xaxis).normalize();
  dist = yaxis.dot(p3 - origin);
  if (dist < 1.0E-10) return;
  yaxis /= dist;
  x1 = xaxis.dot(p3 - origin);
  x2 = 1.0 - x1;
  d = -zaxis.dot(origin);
  mapnx.x = n2.x - n1.x;
  mapnx.y = n3.x - n1.x - mapnx.x * x1;
  mapnx.z = n1.x;
  mapny.x = n2.y - n1.y;
  mapny.y = n3.y - n1.y - mapny.x * x1;
  mapny.z = n1.y;
  mapnz.x = n2.z - n1.z;
  mapnz.y = n3.z - n1.z - mapnz.x * x1;
  mapnz.z = n1.z;
  defined = 1;
}

void Triangle::copy(VoxelPolygon *vp)
{int i;
 Point p;
  vp->nloops = 1;
  vp->nvert[0] = 3;
  vp->verts[0] = vp->vertices;
  vp->verts[0][0] = origin;
  vp->verts[0][1] = origin + xaxis / xaxis.dot(xaxis);
  vp->verts[0][2] = origin + xaxis * x1 / xaxis.dot(xaxis) + yaxis / yaxis.dot(yaxis);
  vp->pmax = vp->pmin = origin;
  for (i = 1 ; i < 3 ; i++)
    {  p = vp->verts[0][i];
       if (p.x < vp->pmin.x) vp->pmin.x = p.x;
       if (p.x > vp->pmax.x) vp->pmax.x = p.x;
       if (p.y < vp->pmin.y) vp->pmin.y = p.y;
       if (p.y > vp->pmax.y) vp->pmax.y = p.y;
       if (p.z < vp->pmin.z) vp->pmin.z = p.z;
       if (p.z > vp->pmax.z) vp->pmax.z = p.z;
    }
}

void Triangle::getinfo(int *nloops,int *nverts,Point *pmin,Point *pmax)
{Point p;
 int i;
  *nloops = 1;
  *nverts = 3;
  for (i = 0 ; i < 3 ; i++)
    {  switch (i)
         {  case 0 :
              p = origin;
              break;
            case 1 :
              p = origin + xaxis / xaxis.dot(xaxis);
              break;
            case 2 :
              p = origin + xaxis * x1 / xaxis.dot(xaxis) + yaxis / yaxis.dot(yaxis);
              break;
         }
       if (p.x < pmin->x) pmin->x = p.x;
       if (p.x > pmax->x) pmax->x = p.x;
       if (p.y < pmin->y) pmin->y = p.y;
       if (p.y > pmax->y) pmax->y = p.y;
       if (p.z < pmin->z) pmin->z = p.z;
       if (p.z > pmax->z) pmax->z = p.z;
    }
}

int Triangle::intersect(Point o,Point dn,double *t,UV *uv)
{double x,y,vd,vo;
 Point p;
  vd = zaxis.dot(dn);
  if (fabs(vd) < 0.001)
    return 0;
  vo = -(zaxis.dot(o) + d);
  *t = vo / vd;
  if (*t < 1.0E-5)
    return 0;
  p = o + dn * *t;
  x = xaxis.dot(p - origin);
  y = yaxis.dot(p - origin);
  if (x - x1 * y >= 0.0 &&
      y >= 0 &&
      x + x2 * y - 1.0 < 0.0)
    {  uv->u = x;
       uv->v = y;
       return 1;
    }
  else
    return 0;
}

void Triangle::getcolour(Point dn,const UV &uv,RealColour *objectcolour,
                         Point *interp,Point *intern,SurfaceInfo **si)
{double u,v;
 Point p,normal,xa,ya;
  *interp = origin + xaxis * uv.u / xaxis.dot(xaxis) + yaxis * uv.v / yaxis.dot(yaxis);
  intern->x = mapnx.x * uv.u + mapnx.y * uv.v + mapnx.z;
  intern->y = mapny.x * uv.u + mapny.y * uv.v + mapny.z;
  intern->z = mapnz.x * uv.u + mapnz.y * uv.v + mapnz.z;
  intern->normalize();
  if (surfaceinfo->bmi != 0 && surfaceinfo->bmi->bumpmap != 0)
    {  u = mapbmu.x * uv.u + mapbmu.y * uv.v + mapbmu.z;
       v = mapbmv.x * uv.u + mapbmv.y * uv.v + mapbmv.z;
       if (surfaceinfo->bmi->swapxy != 0)
         surfaceinfo->bmi->bumpmap->getnormal(v,u,&normal);
       else
         surfaceinfo->bmi->bumpmap->getnormal(u,v,&normal);
       xa = yaxis.cross(*intern).normalize();
       ya = intern->cross(xa).normalize();
       if (flipped)
         *intern = (-xa * normal.x - ya * normal.y + *intern * normal.z).normalize();
       else
         *intern = ( xa * normal.x + ya * normal.y + *intern * normal.z).normalize();
    }
  if (intern->dot(dn) > 0.0)
    *intern = -*intern;
  if (surfaceinfo->tmi != 0 && surfaceinfo->tmi->texturetype >= 2)
    {  solidtexture(*interp,intern,objectcolour);
       if (intern->dot(dn) > 0.0)
         *intern = -*intern;
    }
  else if (surfaceinfo->tmi != 0 && surfaceinfo->tmi->texturetype == 1 && surfaceinfo->tmi->texturemap != 0)
    {  u = mapu.x * uv.u + mapu.y * uv.v + mapu.z;
       v = mapv.x * uv.u + mapv.y * uv.v + mapv.z;
       if (surfaceinfo->tmi->swapxy != 0)
         surfaceinfo->tmi->texturemap->getcolour(v,u,objectcolour);
       else
         surfaceinfo->tmi->texturemap->getcolour(u,v,objectcolour);
       if (surfaceinfo->tmi->usetransparentcolour && 
               fabs(objectcolour->red-surfaceinfo->tmi->transparentcolour.red) < 0.01 &&
               fabs(objectcolour->green-surfaceinfo->tmi->transparentcolour.green) < 0.01 &&
               fabs(objectcolour->blue-surfaceinfo->tmi->transparentcolour.blue) < 0.01)
         {  objectcolour->red = objectcolour->green = objectcolour->blue = -0.01;
         }
    }
  else
    {  objectcolour->red = colour.red;
       objectcolour->green = colour.green;
       objectcolour->blue = colour.blue;
    }
  *si = surfaceinfo;
}

void RayTracePolygon::setsurfaceinfo(SurfaceEntity *se,SurfaceInfo *si,SurfaceInfo *defaultsi)
{ if (si == 0)
    si = defaultsi;
  surfaceinfo = si;
  if (si != 0 && si->white)
    colour.red = colour.green = colour.blue = 1.0;
  else
    {  colour.red = db.colourtable[se->getcolour()].red / 255.0;
       colour.green = db.colourtable[se->getcolour()].green / 255.0;
       colour.blue = db.colourtable[se->getcolour()].blue / 255.0;
    }
  if (surfaceinfo->tmi != 0 && surfaceinfo->tmi->texturemap != 0)
    {  if (surfaceinfo->tmi->x2 != surfaceinfo->tmi->x1 || surfaceinfo->tmi->y2 != surfaceinfo->tmi->y1)
         {  mapu.x = (surfaceinfo->tmi->x2 - surfaceinfo->tmi->x1) * mapu.x;
            mapu.y = (surfaceinfo->tmi->x2 - surfaceinfo->tmi->x1) * mapu.y;
            mapu.z = surfaceinfo->tmi->x1 + (surfaceinfo->tmi->x2 - surfaceinfo->tmi->x1) * mapu.z;
            mapv.x = (surfaceinfo->tmi->y2 - surfaceinfo->tmi->y1) * mapv.x;
            mapv.y = (surfaceinfo->tmi->y2 - surfaceinfo->tmi->y1) * mapv.y;
            mapv.z = surfaceinfo->tmi->y1 + (surfaceinfo->tmi->y2 - surfaceinfo->tmi->y1) * mapv.z;
         }
       else
         {  mapu.x = surfaceinfo->tmi->xaxis.x;
            mapu.y = surfaceinfo->tmi->xaxis.y;
            mapu.z = surfaceinfo->tmi->xaxis.z;
            mapv.x = surfaceinfo->tmi->yaxis.x;
            mapv.y = surfaceinfo->tmi->yaxis.y;
            mapv.z = surfaceinfo->tmi->yaxis.z;
         }
    }
  if (surfaceinfo->bmi != 0 && surfaceinfo->bmi->bumpmap != 0)
    {  mapbmu.x = (surfaceinfo->bmi->x2 - surfaceinfo->bmi->x1) * mapbmu.x;
       mapbmu.y = (surfaceinfo->bmi->x2 - surfaceinfo->bmi->x1) * mapbmu.y;
       mapbmu.z = surfaceinfo->bmi->x1 + (surfaceinfo->bmi->x2 - surfaceinfo->bmi->x1) * mapbmu.z;
       mapbmv.x = (surfaceinfo->bmi->y2 - surfaceinfo->bmi->y1) * mapbmv.x;
       mapbmv.y = (surfaceinfo->bmi->y2 - surfaceinfo->bmi->y1) * mapbmv.y;
       mapbmv.z = surfaceinfo->bmi->y1 + (surfaceinfo->bmi->y2 - surfaceinfo->bmi->y1) * mapbmv.z;
    }
}

void RayTracePolygon::solidtexture(Point p,Point *n,RealColour *objectcolour)
{double r;
 Point p1;
  if (surfaceinfo->tmi->texturetype == 2)
    {  p1 = surfaceinfo->tmi->origin - p;
       r = fmod((p1 - surfaceinfo->tmi->zaxis * (surfaceinfo->tmi->zaxis.dot(p1))).length() / surfaceinfo->tmi->size + surfaceinfo->noise->getnoise(0,p / surfaceinfo->tmi->size) / 8.0 * surfaceinfo->tmi->patternscale,1.0);
       if (r < 0.1)
         objectcolour->red = objectcolour->green = objectcolour->blue = 0.0;
       else if (r < 0.9)
         {  r = (r - 0.1) / 0.8;
            objectcolour->red = colour.red * r;
            objectcolour->green = colour.green * r;
            objectcolour->blue = colour.blue * r;
         }
       else
         {  objectcolour->red = colour.red;
            objectcolour->green = colour.green;
            objectcolour->blue = colour.blue;
         }
    }
  else if (surfaceinfo->tmi->texturetype == 3)
    {  r = surfaceinfo->noise->getnoise(0,p / surfaceinfo->tmi->size);
       if (r < 0.14)
         {  objectcolour->red = 1.0;
            objectcolour->green = objectcolour->blue = 0.0;
         }
       else if (r < 0.28)
         {  objectcolour->red = objectcolour->green = 1.0;
            objectcolour->blue = 0.0;
         }
       else if (r < 0.42)
         {  objectcolour->red = 1.0;
            objectcolour->green = 0.5;
            objectcolour->blue = 0.0;
         }
       else if (r < 0.57)
         {  objectcolour->red = objectcolour->blue = 0.0;
            objectcolour->green = 1.0;
         }
       else if (r < 0.71)
         {  objectcolour->red = 0.0;
            objectcolour->green = objectcolour->blue = 1.0;
         }
       else if (r < 0.85)
         {  objectcolour->red = objectcolour->blue = 1.0;
            objectcolour->green = 0.0;
         }
       else
         {  objectcolour->red = objectcolour->green = 0.0;
            objectcolour->blue = 1.0;
         }
    }
  else if (surfaceinfo->tmi->texturetype == 4)
    {  r = surfaceinfo->noise->getnoise(0,p / surfaceinfo->tmi->size) * 2.25 * surfaceinfo->tmi->patternscale;
       if (r > 1.0) r = 1.0;
       objectcolour->red = colour.red * r;
       objectcolour->green = colour.green * r;
       objectcolour->blue = colour.blue * r;
    }
  else if (surfaceinfo->tmi->texturetype == 5)
    {Point dn;
       dn = surfaceinfo->noise->getdnoise(0,p / surfaceinfo->tmi->size) / 10.0 * surfaceinfo->tmi->patternscale;
       *n = (*n + dn).normalize();
       objectcolour->red = colour.red;
       objectcolour->green = colour.green;
       objectcolour->blue = colour.blue;
    }
  else if (surfaceinfo->tmi->texturetype == 6)
    {Point dn;
       dn = surfaceinfo->noise->getdnoise(0,p / surfaceinfo->tmi->size) *
               surfaceinfo->noise->getnoise(0,p / surfaceinfo->tmi->size) / 10.0 * surfaceinfo->tmi->patternscale;
       *n = (*n + dn).normalize();
       objectcolour->red = colour.red;
       objectcolour->green = colour.green;
       objectcolour->blue = colour.blue;
    }
  else if (surfaceinfo->tmi->texturetype == 7)
    {Point dn;
       dn = (surfaceinfo->noise->getdnoise(0,p / surfaceinfo->tmi->size) +
             surfaceinfo->noise->getdnoise(1,p / surfaceinfo->tmi->size * 2.0) / 2.0 +
             surfaceinfo->noise->getdnoise(2,p / surfaceinfo->tmi->size * 4.0) / 4.0) / 10.0 * surfaceinfo->tmi->patternscale;
       *n = (*n + dn).normalize();
       objectcolour->red = colour.red;
       objectcolour->green = colour.green;
       objectcolour->blue = colour.blue;
    }
  else if (surfaceinfo->tmi->texturetype == 8)
    {double x;
       x = fmod(fabs(p.x) / surfaceinfo->tmi->size + surfaceinfo->noise->getturbulence(p / surfaceinfo->tmi->size) * 0.3 * surfaceinfo->tmi->patternscale,1.0);
       if (x < 0.05 || x > 0.95)
         objectcolour->red = objectcolour->green = objectcolour->blue = 0.0;
       else if (x < 0.35)
         {  x = (x - 0.05) / 0.3;
            objectcolour->red = colour.red + (1.0 - colour.red) * x;
            objectcolour->green = colour.green + (1.0 - colour.green) * x;
            objectcolour->blue = colour.blue + (1.0 - colour.blue) * x;
         }
       else if (x < 0.65)
         {  objectcolour->red = 1.0;
            objectcolour->green = 1.0;
            objectcolour->blue = 1.0;
         }
       else
         {  x = 1.0 - (x - 0.65) / 0.3;
            objectcolour->red = colour.red + (1.0 - colour.red) * x;
            objectcolour->green = colour.green + (1.0 - colour.green) * x;
            objectcolour->blue = colour.blue + (1.0 - colour.blue) * x;
         }
    }
}

int RayTraceImage::add(RayTracePolygon *rtp)
{ return polygons.add(rtp);
}

int RayTraceImage::add(SurfaceInfo *si)
{ return surfaceinfos.add(si);
}

VoxelPolygon::VoxelPolygon(int maxloops,int maxverts)
{ nvert = new int[maxloops];
  verts = new Point *[maxloops];
  vertices = new Point[maxverts];
  if (nvert == 0 || verts == 0 || vertices == 0)
    {  delete [] nvert;
       delete [] verts;
       delete [] vertices;
       nvert = 0;  verts = 0;  vertices = 0;
    }
}

VoxelPolygon::~VoxelPolygon()
{ delete [] nvert;
  delete [] verts;
  delete [] vertices;
}

VoxelPolygon& VoxelPolygon::operator=(const VoxelPolygon &vp)
{int i,j,n;
  nloops = vp.nloops;
  n = 0;
  for (i = 0 ; i < nloops ; i++)
    {  verts[i] = vertices + n;
       nvert[i] = vp.nvert[i];
       n += vp.nvert[i];
       for (j = 0 ; j < nvert[i] ; j++)
         verts[i][j] = vp.verts[i][j];
    }
  return *this;
}

void VoxelPolygon::clip(int axis,double x,VoxelPolygon *poly1,VoxelPolygon *poly2)
{int i,j,k,left;
 double u1,u2,u3;
 Point p,p3,p4,p5,dir1,*vert1,*vert2;
  poly1->nloops = 0;  poly2->nloops = 0;
  vert1 = poly1->vertices;  vert2 = poly2->vertices;
  for (i = 0 ; i < nloops ; i++)
    {  if (nvert[i] == 0) continue;
       poly1->nvert[poly1->nloops] = 0;
       poly2->nvert[poly2->nloops] = 0;
       poly1->verts[poly1->nloops] = vert1;
       poly2->verts[poly2->nloops] = vert2;
       p3 = verts[i][nvert[i]-1];
       switch (axis)
         {  case 0 :  u1 = p3.x - x;  break;
            case 1 :  u1 = p3.y - x;  break;
            case 2 :  u1 = p3.z - x;  break;
         }
       for (j = 0 ; j < nvert[i] ; j++)
         {  p4 = verts[i][j];
            switch (axis)
              {  case 0 :  u2 = p4.x - x;  break;
                 case 1 :  u2 = p4.y - x;  break;
                 case 2 :  u2 = p4.z - x;  break;
              }
            if (u1 <= 0.0 && u2 >= 0.0 || u1 >= 0.0 && u2 <= 0.0)
              {  if (u1 == 0.0)
                   { // Do nothing if u1 == 0.0
                   }
                 else
                   {  for (k = j + 1,u3 = u2 ; u3 == 0.0 ; k++)
                        switch (axis)
                          {  case 0 :  u3 = verts[i][k % nvert[i]].x - x;  break;
                             case 1 :  u3 = verts[i][k % nvert[i]].y - x;  break;
                             case 2 :  u3 = verts[i][k % nvert[i]].z - x;  break;
                          }
                      if (u1 < 0.0 && u3 > 0.0 || u1 > 0.0 && u3 < 0.0)
                        {  *vert1++ = *vert2++ = p3 - (p4 - p3) * (u1 / (u2 - u1));
                           poly1->nvert[poly1->nloops]++;
                           poly2->nvert[poly2->nloops]++;
                        }
                   }
              }
            p3 = p4;  u1 = u2;
            if (u1 > 0.0)
              left = 1;
            else if (u1 < 0.0)
              left = 0;
            else
              {  for (k = j + 1,u3 = u1 ; u3 == 0.0 && k < j + 1 + nvert[i] ; k++)
                   switch (axis)
                     {  case 0 :  u3 = verts[i][k % nvert[i]].x - x;  break;
                        case 1 :  u3 = verts[i][k % nvert[i]].y - x;  break;
                        case 2 :  u3 = verts[i][k % nvert[i]].z - x;  break;
                     }
                 left = u3 > 0.0;
              }
            if (left)
              {  *vert2++ = p3;
                 poly2->nvert[poly2->nloops]++;
              }
            else
              {  *vert1++ = p3;
                 poly1->nvert[poly1->nloops]++;
              }
         }
       if (poly1->nvert[poly1->nloops] > 0)
         poly1->nloops++;
       if (poly2->nvert[poly2->nloops] > 0)
         poly2->nloops++;
    }
}

#endif

