
#include "ncwin.h"
#include "ncdialog.h"
#include "grid_dialog.h"
#include "definecolour_dialog.h"

Grid::Grid()
{
    initialize();
}

void Grid::clear()
{
    initialize();
}

void Grid::initialize()
{
#ifdef USING_WIDGETS
    qgiGrid = 0; // Qt graphics item 1
    qgiRef = 0; // Qt graphics item 2

    style = short((v.getinteger("gd::options") & 16) == 16);
    custcolour = short((v.getinteger("gd::options") & 32) == 32);
#endif
    RCHMENU hmenu;
    xspacing = v.getreal("gd::xspacing");
    yspacing = v.getreal("gd::yspacing");
    xreference = short(v.getinteger("gd::xreference"));
    yreference = short(v.getinteger("gd::yreference"));
    angle = v.getreal("gd::angle");
    plane = short(((v.getinteger("gd::options") & 4) == 4 ? onviewplane : onworkplane));
    display = short((v.getinteger("gd::options") & 1) == 1);
    referenceonly = short((v.getinteger("gd::options") & 8) == 8);
    snap = short((v.getinteger("gd::options") & 2) == 2);
    cosa = cos(angle / 180.0 * M_PI);
    sina = sin(angle / 180.0 * M_PI);

    if (cadwindow != 0 && (hmenu = GetMenu(cadwindow->gethwnd())) != 0)
    {
#ifdef USING_WIDGETS
        QAction *planeAct = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_work_plane;
        QAction *viewAct  = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_view;
        if(planeAct)
            planeAct->setChecked(!plane);
        if(viewAct)
            viewAct->setChecked(plane);
#else
        CheckMenuItem(GetSubMenu(hmenu,3),0,MF_BYPOSITION | (! plane ? MF_CHECKED : MF_UNCHECKED));
        CheckMenuItem(GetSubMenu(hmenu,3),1,MF_BYPOSITION | (plane ? MF_CHECKED : MF_UNCHECKED));
#endif
    }
}

#ifdef USING_WIDGETS
void drawgrid(int drawmode,void *grid,View3dSurface *window)
{
    // ((Grid *)grid)->draw(drawmode,window);
    if(cadwindow)
        ((QtMainWindow*)cadwindow->gethwnd())->update();
}
#else
void drawgrid(int drawmode,void *grid,View3dSurface *window)
{
    ((Grid *)grid)->draw(drawmode,window);
}
#endif

// this is defined in ncdialog.h
/*
class GridColourSampleControl : public  DisplayDialogControl
{
private:
    GeneralList controllist;
    int lastindex;
    Dialog *parentdialog;
    Colour lastcolour;
public:
    GridColourSampleControl(int id,Dialog *d,Colour c) :  DisplayDialogControl(id )
    {  lastindex = -1;  parentdialog = d;  lastcolour = c; }
    void add(DisplayDialogControl *dc){ controllist.add(dc); }
    void draw(Dialog *dialog,RCHDC hDC);
    void change(Colour c) { lastcolour = c; }
    Colour getColour() { return lastcolour; }
};
*/

void ColourSampleControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if(hWnd == 0)
      return;

    //qDebug() << "OtherColourSampleDialogControl::draw start";

    // this is the new colour
    red   = lastcolour->red;
    green = lastcolour->green;
    blue  = lastcolour->blue;

   QGraphicsView *gv=0;
   QGraphicsScene *scene=0;
   //qDebug() << "OtherColourSampleDialogControl::draw id " << id;

   if(dialog->gethdlg())
       gv = (QGraphicsView*)((RCDialog*)dialog->gethdlg())->getControl(id);
   if(gv)
       scene = gv->scene();

   if(gv && scene)
   {
       //qDebug() << "setting colour sample scene";
       output.setScene(scene);
       output.setWidet(gv);
       scene->setSceneRect(QRectF());
       scene->clear();
      // qDebug() << "colour : red : " << red << " green : " << green << " blue : " << blue;
       if(strlen(specialValueText) != 0)
       {
           scene->setBackgroundBrush(QBrush(QColor(255,255,255)));
           scene->addText(QString(getSpecialValueText())/*"*Varies*"*/);
       }
       else
           scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
   }
}

void GridColourSampleControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 int red,green,blue,fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  if(hWnd == 0) return;

    //qDebug() << "OtherColourSampleDialogControl::draw start";

    // this is the new colour
    red   = lastcolour->red;
    green = lastcolour->green;
    blue  = lastcolour->blue;

   QGraphicsView *gv=0;
   QGraphicsScene *scene=0;
   //qDebug() << "OtherColourSampleDialogControl::draw id " << id;

   if(dialog->gethdlg())
       gv = (QGraphicsView*)((Grid_Dialog*)dialog->gethdlg())->getControl(id);
   if(gv)
       scene = gv->scene();

   if(gv && scene)
   {
       //qDebug() << "setting colour sample scene";
       output.setScene(scene);
       output.setWidet(gv);
       scene->setSceneRect(QRectF());
       scene->clear();
      // qDebug() << "colour : red : " << red << " green : " << green << " blue : " << blue;
       scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
   }
}

#if 0
void GridColourDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output1(hWnd,hDC,&top,1.0);
 View3dOutput output2(hWnd,hDC,&top,1.0);
 View3dOutput output3(hWnd,hDC,&top,1.0);
 double red,green,blue;
 int fred,fgreen,fblue,index;
 double hue,lightness,saturation;
 RECT rect;
 RCHBRUSH brush;
 RCHPEN oldhpen,hpen;
 Colour c;

  //qDebug() << "GPColourDisplayDialogControl::draw : start";

  //if (dialog->getcontrol(100) != 0)
  //{
  //    if (! dialog->currentvalue(100,&index)) return;
  //    if (lastindex != -1 && lastindex != index)
  //      {  ((RealDialogControl *) dialog->getcontrol(101))->
  //                    change(dialog,db.colourtable[index].red/255.0);
  //         ((RealDialogControl *) dialog->getcontrol(102))->
  //                    change(dialog,db.colourtable[index].green/255.0);
  //         ((RealDialogControl *) dialog->getcontrol(103))->
  //                    change(dialog,db.colourtable[index].blue/255.0);
  //      }
  //    lastindex = index;
  //    fred=db.colourtable[lastindex].red;
  //    fgreen=db.colourtable[lastindex].green;
  //    fblue=db.colourtable[lastindex].blue;
  //}


  if (dialog->currentvalue(101,&red) &&
      dialog->currentvalue(102,&green) &&
      dialog->currentvalue(103,&blue) &&
      dialog->currentvalue(107,&hue) &&
      dialog->currentvalue(108,&lightness) &&
      dialog->currentvalue(109,&saturation))
    {
      if (lastcolour == Colour(red,green,blue))
      //if (lastcolour.red == red && lastcolour.green == green && lastcolour.blue == blue)
      {//  HLS has changed
          //qDebug() << "HLS has changed";
            c.sethls(hue,lightness,saturation);
            c.get(&red,&green,&blue);

            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));

       }
       else
       {//  RGB has changed
          //qDebug() << "RGB has changed";
            c.set(red,green,blue);
            c.gethls(&hue,&lightness,&saturation);
            ((ColourScrollBarDialogControl *)dialog->getcontrol(104))->setscrollpos(dialog,(int)(red*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(105))->setscrollpos(dialog,(int)(green*255.0+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(106))->setscrollpos(dialog,(int)(blue*255.0+0.5));

            ((ColourScrollBarDialogControl *)dialog->getcontrol(110))->setscrollpos(dialog,(int)(hue/360.0*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(111))->setscrollpos(dialog,(int)(lightness*255+0.5));
            ((ColourScrollBarDialogControl *)dialog->getcontrol(112))->setscrollpos(dialog,(int)(saturation*255+0.5));
       }
       lastcolour.set(red,green,blue);

       QGraphicsView *gv1;
       QGraphicsScene *scene1;

       if(dialog->gethdlg())
           gv1 = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(113);
       if(gv1)
           scene1 = gv1->scene();

       // draw our sample
       /*
       if(gv1 && scene1)
       {
           //qDebug() << "setting colour sample scene";
           output1.setScene(scene1);
           scene1->setSceneRect(QRectF());
           scene1->clear();
           //qDebug() << "colour : fred : " << fred << " fgreen : " << green << " fblue : " << blue;
           scene1->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
       }
       */
        //qDebug() << "draw the other samples";
       // draw the other samples
       DisplayDialogControl *ddc;
       for (controllist.start() ; (ddc = (DisplayDialogControl *)controllist.next()) != NULL ; )
       {
           ((GridColourDialogSampleControl*)ddc)->change(lastcolour);
           ((GridColourDialogSampleControl*)ddc)->draw(dialog,dialog->gethdlg());
       }
   }
}
#endif
void GridColourDialogSampleControl::change(Colour c)
{
    lastcolour = c;
}
void GridColourDialogSampleControl::draw(Dialog *dialog,RCHDC hDC)
{
    ResourceString rs0(NCVIEWA);
    View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hDC,&top,1.0);
    int red,green,blue,minred,mingreen,minblue,majred,majgreen,majblue,index;
    int fred,fgreen,fblue;
    double hue,lightness,saturation;
    RECT rect;
    RCHBRUSH brush;
    RCHPEN oldhpen,hpen;
    Colour c;

    if(hWnd == 0) return;

    //qDebug() << "GridColourDialogSampleControl::draw start";

    // these is the current colour
    fred   = fixedcolour.red;
    fgreen = fixedcolour.green;
    fblue  = fixedcolour.blue;

    // this is the new colour
    //((OtherColourDisplayDialogControl*)dialog->getcontrol(120))->
    red   = lastcolour.red;
    green = lastcolour.green;
    blue  = lastcolour.blue;

    QGraphicsView *gv;
    QGraphicsScene *scene;
    // qDebug() << "GridColourDialogSampleControl::draw id " << id;
    if(id == 115)
    {
      //qDebug() << "GridColourDialogSampleControl::draw 115";
      if(dialog->gethdlg())
          gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(115);
      if(gv)
          scene = gv->scene();

      if(gv && scene)
      {
          //qDebug() << "setting colour sample scene";
          output.setScene(scene);
          output.setWidet(gv);
          scene->setSceneRect(QRectF());
          scene->clear();
          //qDebug() << "colour : red : " << red << " green : " << green << " blue : " << blue;
          scene->setBackgroundBrush(QBrush(QColor(red,green,blue)));
      }
    }
    else if(id == 113)
    {
      //qDebug() << "GridColourDialogSampleControl::draw 113";
      // this control is showing the current background colour
      if(dialog->gethdlg())
        gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(113);
      if(gv)
          scene = gv->scene();

      if(gv && scene)
      {
          //qDebug() << "setting the fixed colour sample scene";
          output.setScene(scene);
          scene->setSceneRect(QRectF());
          scene->clear();
          //qDebug() << "colour : fred : " << fred << " fgreen : " << fgreen << " fblue : " << fblue;
          scene->setBackgroundBrush(QBrush(QColor(fred,fgreen,fblue)));
      }
    }
    else if(id == 116)
    {
      //qDebug() << "GridColourDialogSampleControl::draw 116";
      if(dialog->gethdlg())
        gv = (QGraphicsView*)((DefineColour_Dialog*)dialog->gethdlg())->getControl(116);
      if(gv)
          scene = gv->scene();
       //qDebug() << "GridColourDialogSampleControl::draw gv" << gv;
       //qDebug() << "GridColourDialogSampleControl::draw scene" << scene;
      if(gv && scene)
      {
          //qDebug() << "GridColourDialogSampleControl::draw setting scene output" ;
          output.setScene(scene);
          scene->setSceneRect(QRectF());
          scene->clear();
          QRect qrect = gv->rect();
          rect.left = qrect.left();
          rect.top = qrect.top();
          rect.right = qrect.right();
          rect.bottom = qrect.bottom();
          //qDebug() << "GridColourDialogSampleControl::draw setbackground" ;
          //output.setbackground(RGB(red,green,blue));

           // draw the background
           int bred,bgreen,bblue;
           bred   = ::v.getreal("wn::backgroundcolour.red")*255.0 ;
           bgreen = ::v.getreal("wn::backgroundcolour.green")*255.0 ;
           bblue  = ::v.getreal("wn::backgroundcolour.blue")*255.0 ;

           scene->setBackgroundBrush(QBrush(QColor(bred,bgreen,bblue)));

           // draw the grid over the background
           // the grid style and colour matches the current grid settings on the current background

           // grid lines
           double minred   = ::v.getreal("gd::minor.red")*255.0;
           double mingreen = ::v.getreal("gd::minor.green")*255.0;
           double minblue  = ::v.getreal("gd::minor.blue")*255.0;
           double majred   = ::v.getreal("gd::major.red")*255.0;
           double majgreen = ::v.getreal("gd::major.green")*255.0;
           double majblue  = ::v.getreal("gd::major.blue")*255.0;

           // grid style is 0 (dots) and ownerid is 1110 change major and minor colour (grid and reference grid dots)
           // grid style is 0 (dots) and ownerid is 1130 change major and minor colour (grid and reference grid dots)
           // grid style is 1 (lines) and ownerid is 1110 change major colour (reference lines)
           // grid style is 1 (lines) and ownerid is 1130 change minor colour (grid lines)
           if(gridstyle == 1)
           {
               if(ownerid == 1110)
               {
                   majred   = red;
                   majgreen = green;
                   majblue  = blue;
               }
               else if(ownerid == 1130)
               {
                   minred   = red;
                   mingreen = green;
                   minblue  = blue;
               }
           }
           else
           {
               majred   = red;
               majgreen = green;
               majblue  = blue;
           }

           // if grid style is 0 draw dots
           // if grid style is 1 draw lines

           if(gridstyle == 0)
           {
               // draw dots
               for(int i=0; i<output.getheight(); i+=4)
               {
                   for(int j=0; j<output.getwidth(); j+=4)
                   {
                       output.curPen.setColor(QColor(majred,majgreen,majblue));
                       if( (j % 5) == 0 && (i % 5 == 0))
                       {
                           output.moveto(j,i-2);
                           output.lineto(j,i+2);
                           output.moveto(j-2,i);
                           output.lineto(j+2,i);
                       }
                       else
                       {
                           output.dot(j,i);
                       }
                   }
               }
           }
           else
           {
               // draw grid lines
               // minor grid lines
               output.curPen.setColor(QColor(minred,mingreen,minblue));
               for(int i=0; i<output.getheight(); i+=4)
               {
                   output.moveto(0,i);
                   output.lineto(output.getwidth()-1,i);
               }
               for(int i=0; i<output.getwidth(); i+=4)
               {
                   output.moveto(i,0);
                   output.lineto(i,output.getheight()-1);
               }
               // major lines
               output.curPen.setColor(QColor(majred,majgreen,majblue));
               for(int i=0; i<output.getheight(); i+=4)
               {
                   if( (i % 5) == 0)
                   {
                       output.moveto(1,i);
                       output.lineto(output.getwidth()-1,i);
                   }
               }
               for(int i=0; i<output.getwidth(); i+=4)
               {
                   if( (i % 5) == 0)
                   {
                       output.moveto(i,1);
                       output.lineto(i,output.getheight()-1);
                   }
               }
           }

           // draw a line in the current colour and line style
           //qDebug() << "GridColourDialogSampleControl::draw getstyle" <<db.header.getstyle();
           LineStyle *linestyle = db.linestyles.indextostyle(db.header.getstyle());
           //qDebug() << "GridColourDialogSampleControl::draw setupt" ;
           output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
           //qDebug() << "GridColourDialogSampleControl::drawsetcap" ;
           output.curPen.setCapStyle(Qt::FlatCap);
           //qDebug() << "GridColourDialogSampleControl::draw pencolour" ;
           output.curPen.setColor(QColor(db.colourtable[db.header.getcolour()].red,
                                         db.colourtable[db.header.getcolour()].green,
                                         db.colourtable[db.header.getcolour()].blue));
           //qDebug() << "GridColourDialogSampleControl::draw linestyle" << linestyle;
           if (linestyle && linestyle->getnsegments() > 0)
           {//qDebug() << "GridColourDialogSampleControl::draw linestyle" ;
                linestyle->start(0.5,rect.right - 20);
                linestyle->draw(&output,10,(rect.bottom - rect.top)/2+4,
                           rect.right - 10,(rect.bottom - rect.top)/2+4);
           }
           else
           {//qDebug() << "GridColourDialogSampleControl::draw moveto" ;
                output.moveto(10,(rect.bottom - rect.top)/2-4);
                output.lineto(rect.right - 10,(rect.bottom - rect.top)/2-4);
           }
           //qDebug() << "GridColourDialogSampleControl::draw done" ;
      }
    }
}

class GridColourButtonControl : public ButtonDialogControl
{
protected:
    int baseid;
    int mode; // 0 = define grid colour; 1 = define reference grid line colour
public:
     GridColourButtonControl(int id, int bid, int m=0) : ButtonDialogControl(id) { baseid = bid; mode=m; }
     int select(Dialog *);
};

int GridColourButtonControl::select(Dialog *parentdialog)
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

    Colour lc = ((GridColourSampleControl*)parentdialog->getcontrol(baseid))->getColour();
    lc.get(&red,&green,&blue);

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    int style = 0;
    ((CheckBoxDialogControl*)parentdialog->getcontrol(108))->currentvalue(parentdialog,&style);

    if(id == 1110)
    {
        if(style == 0)
            dialog.title(app->tr("Define Grid Colour").data());
        else
            dialog.title(app->tr("Define Reference Grid Line Colour").data());
    }
    else if(id == 1130)
        dialog.title(app->tr("Define Grid Colour").data());

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
    GridColourDialogSampleControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c,lc)));
    dialog.add((DisplayDialogControl *)(sddc = new GridColourDialogSampleControl(113,pddc,&dialog,c,lc)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new GridColourDialogSampleControl(115,pddc,&dialog,c,lc)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new GridColourDialogSampleControl(116,pddc,&dialog,c,lc)));
    sddc->setgridstyle(style);
    sddc->setownerid(id);
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
        Colour nc;
        nc.set(red,green,blue);
        ((GridColourSampleControl*)parentdialog->getcontrol(baseid))->change(nc);
        ((GridColourSampleControl*)parentdialog->getcontrol(baseid))->draw(parentdialog,0);
    }
    else
    {
        //Colour lc;
        //lc.set(lastRed,lastGreen,lastBlue);
        ((GridColourSampleControl*)parentdialog->getcontrol(baseid))->change(lc);
        ((GridColourSampleControl*)parentdialog->getcontrol(baseid))->draw(parentdialog,0);
    }

    db.setnumericdisplaytolerance(n);

    return 0;
}

void Grid::setup(void)
{
    //Dialog dialog("Grid_dialog");
    Dialog dialog("Grid_Dialog");
    DialogControl *dc;
    Grid oldgrid;
    oldgrid = *this;
    Colour minorcol,majorcol;

    minorcol.set(v.getreal("gd::minor.red"),v.getreal("gd::minor.green"),v.getreal("gd::minor.blue"));
    majorcol.set(v.getreal("gd::major.red"),v.getreal("gd::major.green"),v.getreal("gd::major.blue"));

    dialog.add(dc = new RealDialogControl(100,&xspacing,ScaleByLength));
    //dialog.add(new ScrollBarDialogControl(1100,dc));
    dialog.add(dc = new RealDialogControl(101,&yspacing,ScaleByLength));
    //dialog.add(new ScrollBarDialogControl(1101,dc));
    dialog.add(dc = new IntegerDialogControl(102,&xreference));
    //dialog.add(new ScrollBarDialogControl(1102,dc));
    dialog.add(dc = new IntegerDialogControl(103,&yreference));
    //dialog.add(new ScrollBarDialogControl(1103,dc));
    dialog.add(dc = new RealDialogControl(104,&angle));
    //dialog.add(new ScrollBarDialogControl(1104,dc));
    dialog.add(new CheckBoxDialogControl(105,&snap));
    dialog.add(new CheckBoxDialogControl(106,&display));
    dialog.add(new CheckBoxDialogControl(107,&referenceonly));
    // KMJ : new functionality
    dialog.add(new CheckBoxDialogControl(108,&style));
    dialog.add(new CheckBoxDialogControl(109,&custcolour));

    dialog.add((DisplayDialogControl *)new GridColourSampleControl(111,&dialog,&majorcol));
    dialog.add((DisplayDialogControl *)new GridColourSampleControl(113,&dialog,&minorcol));

    dialog.add(new GridColourButtonControl(1110,111,1));
    dialog.add(new GridColourButtonControl(1130,113,0));

    if (dialog.process() == TRUE)
    {
        cadwindow->drawcallback(DM_ERASE,&oldgrid,drawgrid);
        cosa = cos(angle / 180.0 * M_PI);
        sina = sin(angle / 180.0 * M_PI);
        cadwindow->drawcallback(DM_NORMAL,this,drawgrid);
        v.setreal("gd::xspacing",xspacing);
        v.setreal("gd::yspacing",yspacing);
        v.setinteger("gd::xreference",xreference);
        v.setinteger("gd::yreference",yreference);
        v.setreal("gd::angle",angle);
        v.setinteger("gd::options",display + snap * 2 + plane * 4 + referenceonly * 8 + style * 16 + custcolour * 32 );

        // KMJ: new grid colours
        double red,green,blue;
        minorcol.get(&red,&green,&blue);
        v.setreal("gd::minor.red",red);
        v.setreal("gd::minor.green",green);
        v.setreal("gd::minor.blue",blue);

        majorcol.get(&red,&green,&blue);
        v.setreal("gd::major.red",(double)red);
        v.setreal("gd::major.green",(double)green);
        v.setreal("gd::major.blue",(double)blue);

        cadwindow->updatetitle();
    }
}

void Grid::setdisplay(int d)
{
    Grid oldgrid;
    oldgrid = *this;
    cadwindow->drawcallback(DM_ERASE,&oldgrid,drawgrid);
    display = short(d);
    cadwindow->drawcallback(DM_NORMAL,this,drawgrid);
}

Point Grid::snappoint(View3dSurface *surface,Point p1)
{ if (getplane() == onviewplane || surface->getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
    {  if (surface != NULL)
         {  p1 = surface->modeltouv(p1);
            p1 = snappoint(p1);
            p1 = surface->uvtomodel(p1);
         }
    }
  else
    {  p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
       p1 = snappoint(p1);
       p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
    }
  return p1;
}

Point Grid::snappoint(Point p)
{double sx,sy,u,v;
  if (! snap) return p;
  u =  cosa * p.x + sina * p.y;
  v = -sina * p.x + cosa * p.y;
  if (u < 0.0)
    {  u = -u;  sx = -1.0;
    }
  else
    {  sx = 1.0;
    }
  if (v < 0.0)
    {  v = -v;  sy = -1.0;
    }
  else
    {  sy = 1.0;
    }
  u = floor(u / xspacing + 0.5) * sx * xspacing;
  v = floor(v / yspacing + 0.5) * sy * yspacing;
  p.x =  cosa * u - sina * v;
  p.y =  sina * u + cosa * v;
  return p;
}


void Grid::draw(int drawmode,View3dSurface *window)
{
#ifdef USING_WIDGETS
    long i,j,imin,imax,jmin,jmax;
    int k,sx,sy,x,y;
    double u,v,depth;
    Point p1,p2,o,du,dv;

    QGraphicsItem *qgItem = 0,*qgItem2 = 0, *qgTemp = 0;
    QList<QGraphicsItem *> qgridList,qrefList;

    if(qgiGrid)
        window->getScene()->removeItem(qgiGrid);
    delete qgiGrid;
    qgiGrid = 0;

     if (cadwindow != 0)
       cadwindow->updatetitle();

     if (drawmode != DM_ERASE && ! display) return;
     imin = jmin = imax = jmax = 0;
     depth = ::v.getreal("vw::depth");
     if (plane == onviewplane || window->getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
       {  for (k = 0 ; k < 4 ; k++)
            {  p1.x = (k % 2 == 0 ? window->getumin() : window->getumax());
               p1.y = (k / 2 == 0 ? window->getvmin() : window->getvmax());
               p1.z = depth;
               u =  cosa * p1.x + sina * p1.y;
               v = -sina * p1.x + cosa * p1.y;
               if (window->getsurfacetype() == View2dSurf)
                 {  if (u < 0.0) u = 0.0;
                    if (v < 0.0) v = 0.0;
                    if (u > ((View2dWindow *)window)->getview2d()->getpaperwidth())
                      u = ((View2dWindow *)window)->getview2d()->getpaperwidth();
                    if (v > ((View2dWindow *)window)->getview2d()->getpaperheight())
                      v = ((View2dWindow *)window)->getview2d()->getpaperheight();
                 }
               if (u < 0.0)
                 {  u = -u;  sx = -1;
                 }
               else
                 {  sx = 1;
                 }
               if (v < 0.0)
                 {  v = -v;  sy = -1;
                 }
               else
                 {  sy = 1;
                 }
               if (fabs(u / xspacing) > INT_MAX - 100 || fabs(v / yspacing) > INT_MAX-100)
                 return;
               i = int(u / xspacing);  i *= sx;
               j = int(v / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
            }
          if (drawmode == DM_ERASE)
            window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
          else
            window->setup(1.0,0.0,DM_GRID,0,0,0,0);
          if (window->getview()->getperspective())
            {  for (i = imin ; i <= imax  && ! program->getinterrupt() ; i++)
                 for (j = jmin ; j <= jmax ; j++)
                   {  u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);
                      p1 = window->modeltoscreen(p1);
                      //if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                      if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                      if (! referenceonly)
                      {
                         //qgItem = window->dot(qgTemp,int(p1.x),int(p1.y));
                         //if(qgItem)
                         //    qgridList.append(qgItem);
                         qgItem = window->line(qgTemp,int(p1.x-1),int(p1.y),int(p1.x+1),int(p1.y));
                         if(qgItem)
                             qgridList.append(qgItem);
                         qgItem2 = window->line(qgTemp,int(p1.x),int(p1.y-1),int(p1.x),int(p1.y+1));
                         if(qgItem2)
                             qgridList.append(qgItem2);
                        //window->dot(int(p1.x),int(p1.y));
                      }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {

                           qgItem = window->line(qgTemp,int(p1.x-4),int(p1.y),int(p1.x+4),int(p1.y));
                           if(qgItem)
                               qgridList.append(qgItem);
                           qgItem2 = window->line(qgTemp,int(p1.x),int(p1.y-4),int(p1.x),int(p1.y+4));
                           if(qgItem2)
                               qgridList.append(qgItem2);

                      }
                      p2 = p1;
                   }
            }
          else
            {  u = imin * xspacing;  v = jmin * yspacing;
               o.x =  cosa * u - sina * v;
               o.y =  sina * u + cosa * v;
               o.z = window->getdepth();
               o = window->uvtomodel(o);  o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = depth;
               p1 = window->uvtomodel(p1);  p1 = window->modeltoscreen(p1);
               du = p1 - o;
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = window->getdepth();
               p1 = window->uvtomodel(p1);  p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               //if (du.length() < 2.0 || dv.length() < 2.0) return;
               if (du.length() < 3.0 || dv.length() < 3.0) return;
               for (i = imin,p1 = o ; i <= imax && ! program->getinterrupt()  ; i++,p1 += du)
                 for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                   {
                     if (! referenceonly)
                     {
                         //qgItem = window->dot(qgTemp,int(p2.x),int(p2.y));
                         //if(qgItem)
                         //    qgridList.append(qgItem);
                         qgItem = window->line(qgTemp,int(p2.x-1),int(p2.y),int(p2.x+1),int(p2.y));
                         if(qgItem)
                             qgridList.append(qgItem);
                         qgItem2 = window->line(qgTemp,int(p2.x),int(p2.y-1),int(p2.x),int(p2.y+1));
                         if(qgItem2)
                             qgridList.append(qgItem2);
                        //window->dot(int(p2.x),int(p2.y));
                     }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {

                          qgItem = window->line(qgTemp,int(p2.x-4),int(p2.y),int(p2.x+4),int(p2.y));
                          if(qgItem)
                              qgridList.append(qgItem);
                          qgItem2 = window->line(qgTemp,int(p2.x),int(p2.y-4),int(p2.x),int(p2.y+4));
                          if(qgItem2)
                              qgridList.append(qgItem2);

                      }
                   }
            }
       }
     else
       {  if (fabs(window->getwaxis().dot(db.workplanes.getcurrent()->getzaxis())) < 0.07)  // angle greater than 86 degrees.
            return;
          for (k = 0 ; k < 4 ; k++)
            {  x = (k % 2 == 0 ? 0 : int(window->getwidth())-1);
               y = (k / 2 == 0 ? 0 : int(window->getheight())-1);
               if (! window->coordinate3d(x,y,&p1,0))
                 return;
               p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
               if (p1.x < 0.0)
                 {  p1.x = -p1.x;  sx = -1;
                 }
               else
                 {  sx = 1;
                 }
               if (p1.y < 0.0)
                 {  p1.y = -p1.y;  sy = -1;
                 }
               else
                 {  sy = 1;
                 }
               if (fabs(p1.x / xspacing) > INT_MAX - 100 || fabs(p1.y / yspacing) > INT_MAX-100)
                 return;
               i = int(p1.x / xspacing);  i *= sx;
               j = int(p1.y / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
            }
          if (drawmode == DM_ERASE)
            window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
          else
            window->setup(1.0,0.0,DM_GRID,0,0,0,0);
          if (window->getview()->getperspective())
            {  for (i = imin ; i <= imax && ! program->getinterrupt() ; i++)
                 for (j = jmin ; j <= jmax ; j++)
                   {  u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
                      p1 = window->modeltoscreen(p1);
                      //if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                      if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                      if (! referenceonly)
                      {
                          //qgItem = window->dot(qgTemp,int(p1.x),int(p1.y));
                         // if(qgItem)
                          //    qgridList.append(qgItem);
                          qgItem = window->line(qgTemp,int(p1.x-1),int(p1.y),int(p1.x+1),int(p1.y));
                          if(qgItem)
                              qgridList.append(qgItem);
                          qgItem2 = window->line(qgTemp,int(p1.x),int(p1.y-1),int(p1.x),int(p1.y+1));
                          if(qgItem2)
                              qgridList.append(qgItem2);
                          //window->dot(int(p1.x),int(p1.y));
                      }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {

                          qgItem = window->line(qgTemp,int(p1.x-4),int(p1.y),int(p1.x+4),int(p1.y));
                          if(qgItem)
                              qgridList.append(qgItem);
                          qgItem2 = window->line(qgTemp,int(p1.x),int(p1.y-4),int(p1.x),int(p1.y+4));
                          if(qgItem2)
                              qgridList.append(qgItem2);

                      }
                      p2 = p1;
                   }
            }
          else
            {  u = imin * xspacing;  v = jmin * yspacing;
               o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               o = db.workplanes.getcurrent()->workplanetomodel(o);
               o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
               p1 = window->modeltoscreen(p1);
               du = p1 - o;
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,0.0);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);  p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               //if (du.length() < 2.0 || dv.length() < 2.0) return;
               if (du.length() < 3.0 || dv.length() < 3.0) return;
               for (i = imin,p1 = o ; i <= imax  && ! program->getinterrupt() ; i++,p1 += du)
                 for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                 {
                     if (! referenceonly)
                     {
                         //qgItem = window->dot(qgTemp,int(p2.x),int(p2.y));
                         //if(qgItem)
                         //    qgridList.append(qgItem);
                         qgItem = window->line(qgTemp,int(p2.x-1),int(p2.y),int(p2.x+1),int(p2.y));
                         if(qgItem)
                             qgridList.append(qgItem);
                         qgItem2 = window->line(qgTemp,int(p2.x),int(p2.y-1),int(p2.x),int(p2.y+1));
                         if(qgItem2)
                             qgridList.append(qgItem2);
                         //window->dot(int(p2.x),int(p2.y));
                     }
                     if ((i % xreference) == 0 && (j % yreference) == 0)
                     {

                          qgItem = window->line(qgTemp,int(p2.x-4),int(p2.y),int(p2.x+4),int(p2.y));
                          if(qgItem)
                              qgridList.append(qgItem);
                          qgItem2 = window->line(qgTemp,int(p2.x),int(p2.y-4),int(p2.x),int(p2.y+4));
                          if(qgItem2)
                              qgridList.append(qgItem2);

                     }
                 }
            }
       }
     if(qgridList.size() > 0)
        qgiGrid = window->getScene()->createItemGroup(qgridList);
#else
 long i,j,imin,imax,jmin,jmax;
 int k,sx,sy,x,y;
 double u,v,depth;
 Point p1,p2,o,du,dv;


  if (cadwindow != 0)
    cadwindow->updatetitle();

  if (drawmode != DM_ERASE && ! display) return;
  imin = jmin = imax = jmax = 0;
  depth = ::v.getreal("vw::depth");
  if (plane == onviewplane || window->getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
    {  for (k = 0 ; k < 4 ; k++)
         {  p1.x = (k % 2 == 0 ? window->getumin() : window->getumax());
            p1.y = (k / 2 == 0 ? window->getvmin() : window->getvmax());
            p1.z = depth;
            u =  cosa * p1.x + sina * p1.y;
            v = -sina * p1.x + cosa * p1.y;
            if (window->getsurfacetype() == View2dSurf)
              {  if (u < 0.0) u = 0.0;
                 if (v < 0.0) v = 0.0;
                 if (u > ((View2dWindow *)window)->getview2d()->getpaperwidth())
                   u = ((View2dWindow *)window)->getview2d()->getpaperwidth();
                 if (v > ((View2dWindow *)window)->getview2d()->getpaperheight())
                   v = ((View2dWindow *)window)->getview2d()->getpaperheight();
              }
            if (u < 0.0)
              {  u = -u;  sx = -1;
              }
            else
              {  sx = 1;
              }
            if (v < 0.0)
              {  v = -v;  sy = -1;
              }
            else
              {  sy = 1;
              }
            if (fabs(u / xspacing) > INT_MAX - 100 || fabs(v / yspacing) > INT_MAX-100)
              return;
            i = int(u / xspacing);  i *= sx;
            j = int(v / yspacing);  j *= sy;
            if (k == 0 || i < imin) imin = i;
            if (k == 0 || i > imax) imax = i;
            if (k == 0 || j < jmin) jmin = j;
            if (k == 0 || j > jmax) jmax = j;
         }
       if (drawmode == DM_ERASE)
         window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
       else
         window->setup(1.0,0.0,DM_GRID,0,0,0,0);
       if (window->getview()->getperspective())
         {  for (i = imin ; i <= imax  && ! program->getinterrupt() ; i++)
              for (j = jmin ; j <= jmax ; j++)
                {  u = i * xspacing;  v = j * yspacing;
                   p1.x =  cosa * u - sina * v;
                   p1.y =  sina * u + cosa * v;
                   p1.z = depth;
                   p1 = window->uvtomodel(p1);
                   p1 = window->modeltoscreen(p1);
                   if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                   if (! referenceonly)
                     window->dot(int(p1.x),int(p1.y));
                   if ((i % xreference) == 0 && (j % yreference) == 0)
                     {  //window->line(int(p1.x-2),int(p1.y),int(p1.x+3),int(p1.y));
                        //window->line(int(p1.x),int(p1.y-2),int(p1.x),int(p1.y+3));
                     }
                   p2 = p1;
                }
         }
       else
         {  u = imin * xspacing;  v = jmin * yspacing;
            o.x =  cosa * u - sina * v;
            o.y =  sina * u + cosa * v;
            o.z = window->getdepth();
            o = window->uvtomodel(o);  o = window->modeltoscreen(o);
            u = (imin+1) * xspacing;  v = jmin * yspacing;
            p1.x =  cosa * u - sina * v;
            p1.y =  sina * u + cosa * v;
            p1.z = depth;
            p1 = window->uvtomodel(p1);  p1 = window->modeltoscreen(p1);
            du = p1 - o;
            u = imin * xspacing;  v = (jmin+1) * yspacing;
            p1.x =  cosa * u - sina * v;
            p1.y =  sina * u + cosa * v;
            p1.z = window->getdepth();
            p1 = window->uvtomodel(p1);  p1 = window->modeltoscreen(p1);
            dv = p1 - o;
            if (du.length() < 2.0 || dv.length() < 2.0) return;
            for (i = imin,p1 = o ; i <= imax && ! program->getinterrupt()  ; i++,p1 += du)
              for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                {  if (! referenceonly)
                     window->dot(int(p2.x),int(p2.y));
                   if ((i % xreference) == 0 && (j % yreference) == 0)
                     {  //window->line(int(p2.x-2),int(p2.y),int(p2.x+3),int(p2.y));
                        //window->line(int(p2.x),int(p2.y-2),int(p2.x),int(p2.y+3));
                     }
                }
         }
    }
  else
    {  if (fabs(window->getwaxis().dot(db.workplanes.getcurrent()->getzaxis())) < 0.07)  // angle greater than 86 degrees.
         return;
       for (k = 0 ; k < 4 ; k++)
         {  x = (k % 2 == 0 ? 0 : int(window->getwidth())-1);
            y = (k / 2 == 0 ? 0 : int(window->getheight())-1);
            if (! window->coordinate3d(x,y,&p1,0))
              return;
            p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
            if (p1.x < 0.0)
              {  p1.x = -p1.x;  sx = -1;
              }
            else
              {  sx = 1;
              }
            if (p1.y < 0.0)
              {  p1.y = -p1.y;  sy = -1;
              }
            else
              {  sy = 1;
              }
            if (fabs(p1.x / xspacing) > INT_MAX - 100 || fabs(p1.y / yspacing) > INT_MAX-100)
              return;
            i = int(p1.x / xspacing);  i *= sx;
            j = int(p1.y / yspacing);  j *= sy;
            if (k == 0 || i < imin) imin = i;
            if (k == 0 || i > imax) imax = i;
            if (k == 0 || j < jmin) jmin = j;
            if (k == 0 || j > jmax) jmax = j;
         }
       if (drawmode == DM_ERASE)
         window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
       else
         window->setup(1.0,0.0,DM_GRID,0,0,0,0);
       if (window->getview()->getperspective())
         {  for (i = imin ; i <= imax && ! program->getinterrupt() ; i++)
              for (j = jmin ; j <= jmax ; j++)
                {  u = i * xspacing;  v = j * yspacing;
                   p1.x =  cosa * u - sina * v;
                   p1.y =  sina * u + cosa * v;
                   p1.z = depth;
                   p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
                   p1 = window->modeltoscreen(p1);
                   if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                   if (! referenceonly)
                     window->dot(int(p1.x),int(p1.y));
                   if ((i % xreference) == 0 && (j % yreference) == 0)
                     {  //window->line(int(p1.x-2),int(p1.y),int(p1.x+3),int(p1.y));
                        //window->line(int(p1.x),int(p1.y-2),int(p1.x),int(p1.y+3));
                     }
                   p2 = p1;
                }
         }
       else
         {  u = imin * xspacing;  v = jmin * yspacing;
            o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
            o = db.workplanes.getcurrent()->workplanetomodel(o);  
            o = window->modeltoscreen(o);
            u = (imin+1) * xspacing;  v = jmin * yspacing;
            p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
            p1 = db.workplanes.getcurrent()->workplanetomodel(p1);  
            p1 = window->modeltoscreen(p1);
            du = p1 - o;
            u = imin * xspacing;  v = (jmin+1) * yspacing;
            p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,0.0);
            p1 = db.workplanes.getcurrent()->workplanetomodel(p1);  p1 = window->modeltoscreen(p1);
            dv = p1 - o;
            if (du.length() < 2.0 || dv.length() < 2.0) return;
            for (i = imin,p1 = o ; i <= imax  && ! program->getinterrupt() ; i++,p1 += du)
              for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                {  if (! referenceonly)
                     window->dot(int(p2.x),int(p2.y));
                   if ((i % xreference) == 0 && (j % yreference) == 0)
                     {  //window->line(int(p2.x-2),int(p2.y),int(p2.x+3),int(p2.y));
                       //window->line(int(p2.x),int(p2.y-2),int(p2.x),int(p2.y+3));
                     }
                }
         }
    }
#endif
}

#ifdef USING_WIDGETS
void Grid::drawGL(int drawmode,View3dSurface *window)
{
    long i,j,imin,imax,jmin,jmax;
    int k,sx,sy,x,y;
    double u,v,depth;
    Point p1s,p2s,p1,p2,o,du,dv,dus,dvs;
    Point p3,p4,p5,p6,duv,dvv;

    int gridstyle=getStyle();

    RCCOLORREF pcol;
    GLfloat fr,fg,fb;

     //if (cadwindow != 0)
     //  cadwindow->updatetitle();

     //if (drawmode != DM_ERASE && ! display)
     //    return;

     //if (drawmode == DM_ERASE)
     //  window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
     //else
       window->setupGL(1.0,0.0,DM_GRID,0,0,0,0);


     pcol = window->getcolour();
     //fr=GetRValue(pcol)/255.0;
     //fg=GetGValue(pcol)/255.0;
     //fb=GetBValue(pcol)/255.0;
     //SetNoMaterial(fr,fg,fb);
     SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
     GLfloat lwidth = window->curPen.widthF();
     glLineWidth(lwidth<1.0?1.0:lwidth);

     imin = jmin = imax = jmax = 0;
     depth = ::v.getreal("vw::depth");
     if (plane == onviewplane || window->getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
       {
         //qDebug() << "Grid::draw : part 1";
         for (k = 0 ; k < 4 ; k++)
            {  p1.x = (k % 2 == 0 ? window->getumin() : window->getumax());
               p1.y = (k / 2 == 0 ? window->getvmin() : window->getvmax());
               p1.z = depth;
               u =  cosa * p1.x + sina * p1.y;
               v = -sina * p1.x + cosa * p1.y;
               if (window->getsurfacetype() == View2dSurf)
                 {  if (u < 0.0) u = 0.0;
                    if (v < 0.0) v = 0.0;
                    if (u > ((View2dWindow *)window)->getview2d()->getpaperwidth())
                      u = ((View2dWindow *)window)->getview2d()->getpaperwidth();
                    if (v > ((View2dWindow *)window)->getview2d()->getpaperheight())
                      v = ((View2dWindow *)window)->getview2d()->getpaperheight();
                 }
               if (u < 0.0)
                 {  u = -u;  sx = -1;
                 }
               else
                 {  sx = 1;
                 }
               if (v < 0.0)
                 {  v = -v;  sy = -1;
                 }
               else
                 {  sy = 1;
                 }
               if (fabs(u / xspacing) > INT_MAX - 100 || fabs(v / yspacing) > INT_MAX-100)
                 return;
               i = int(u / xspacing);  i *= sx;
               j = int(v / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
            }

          if (window->getview()->getperspective())
            {

              //qDebug() << "Grid::draw : part 1 : with pers";
              u = imin * xspacing;  v = jmin * yspacing;
              o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
              o = window->uvtomodel(o);
              //o = window->modeltoscreen(o);
              p2.setxyz(cosa * (imin+1 * xspacing) - sina * jmin * yspacing,sina * (imin+1 * xspacing) + cosa * jmin * yspacing,depth);
              p2 = window->uvtomodel(p2);
              //p2 = window->modeltoscreen(p2);
              du = p2 - o;
              p2.setxyz(cosa * imin * xspacing - sina * (jmin+1) * yspacing,sina * imin * xspacing + cosa * (jmin+1) * yspacing,depth);
              p2 = window->uvtomodel(p2);
              //p2 = window->modeltoscreen(p2);
              dv = p2 - o;

              //double delta = min(xspacing,yspacing) * 0.5;
              Point pright = window->getview()->getright();
              Point pup = window->getview()->getup();
              double sc = window->getpixelscale() * 6.0;

              duv = du; duv.normalize();
              dvv = dv; dvv.normalize();


              if( gridstyle == 0)
              { // points
                  pcol = window->getGridMajorColour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));


                  for (i = imin ; i <= imax  ; i++)
                  {
                     for (j = jmin ; j <= jmax ; j++)
                       {
                          u = i * xspacing;  v = j * yspacing;
                          p1.x =  cosa * u - sina * v;
                          p1.y =  sina * u + cosa * v;
                          p1.z = depth;
                          p1 = window->uvtomodel(p1);
                          //p1 = window->modeltoscreen(p1);
                          p1s = window->modeltoscreen(p1);
                          if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 3.0) return;
                          //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                          if (! referenceonly)
                          {
                              glPointSize(1.0);
                              glBegin(GL_POINTS);
#if 1
                              glVertex3f(p1.x,p1.y,p1.z);
#else
                              glVertex3d(p1.x,p1.y,p1.z);
#endif
                              glEnd();
                                //painter->drawPoint(int(p1.x),window->getheight() - 1.0 - int(p1.y));
                                //painter->drawPoint(int(p1.x),int(p1.y));
                                //painter->drawPoint(int(p1.x),int(p1.y));
                                //window->dot(int(p1.x),int(p1.y));
                          }
                          if ((i % xreference) == 0 && (j % yreference) == 0)
                          {
                              //window->cross2dGL(p1,pright,pup,sc,sc);

                              glPointSize(3.0);
                              glBegin(GL_POINTS);
                              glVertex3f(p2.x,p2.y,p2.z);
                              glEnd();

                              //p3 = p1 - duv * delta;
                              //p4 = p1 + duv * delta;
                              //p5 = p1 - dvv * delta;
                              //p6 = p1 + dvv * delta;

                              //glBegin(GL_LINES);
                              //glVertex3f(p3.x,p3.y,p3.z);
                              //glVertex3f(p4.x,p4.y,p4.z);
                              //glVertex3f(p5.x,p5.y,p5.z);
                              //glVertex3f(p6.x,p6.y,p6.z);
                              // glEnd();
                              //painter->drawLine(int(p1.x-2),window->getheight() - 1.0 - int(p1.y),int(p1.x+2),window->getheight() - 1.0 - int(p1.y));
                              //painter->drawLine(int(p1.x),window->getheight() - 1.0 - int(p1.y-2),int(p1.x),window->getheight() - 1.0 - int(p1.y+2));
                              //painter->drawLine(int(p1.x-2),int(p1.y),int(p1.x+2),int(p1.y));
                              //painter->drawLine(int(p1.x),int(p1.y-2),int(p1.x),int(p1.y+2));
                          }
                          //p2 = p1;
                          p2s = p1s;
                       }
                     }
              }
              else
              {
                  // lines
                  // minor lines
                  // minor grid colour
                  pcol = window->getGridMinorColour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                  j=jmin;
                  for (i = imin,p1 = o ; i <= imax  ; i++,p1 += du)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);

                      //p1 = window->modeltoscreen(p1);
                      p1s = window->modeltoscreen(p1);
                      if (i > imin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;

                      p2 = p1 - dvv * yspacing;
                      p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                      if (! referenceonly && (i % xreference) != 0 )
                      {

                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
                  i=imin;
                  for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);

                      //p1 = window->modeltoscreen(p1);
                      p1s = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;

                      p2 = p1 - duv * xspacing;
                      p3 = p1 + duv * xspacing * (imax-imin+1);
                      if (! referenceonly && (j % yreference) != 0)
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
                  // major lines
                  // major grid colour
                  pcol = window->getGridMajorColour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                  j=jmin;
                  for (i = imin,p1 = o ; i <= imax  ; i++,p1 += du)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);

                      //p1 = window->modeltoscreen(p1);
                      p1s = window->modeltoscreen(p1);
                      if (i > imin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;

                      p2 = p1 - dvv * yspacing;
                      p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                      if ( (i % xreference) == 0 )
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
                  i=imin;
                  for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);

                      //p1 = window->modeltoscreen(p1);
                      p1s = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;

                      p2 = p1 - duv * xspacing;
                      p3 = p1 + duv * xspacing * (imax-imin+1);
                      if ( (j % yreference) == 0 )
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
              }
            }
          else
            { //qDebug() << "Grid::draw : part 1 : no pers";

              u = imin * xspacing;  v = jmin * yspacing;
               o.x =  cosa * u - sina * v;
               o.y =  sina * u + cosa * v;
               o.z = window->getdepth();
               o = window->uvtomodel(o);
               //o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = window->getdepth();
               p1 = window->uvtomodel(p1);
               //p1 = window->modeltoscreen(p1);
               du = p1 - o;
               dus = window->modeltoscreen(p1) - window->modeltoscreen(o);
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = window->getdepth();
               p1 = window->uvtomodel(p1);
               //p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               dvs = window->modeltoscreen(p1) - window->modeltoscreen(o);
               if (dus.length() < 2.0 || dvs.length() < 2.0) return;
                //if (du.length() < 3.0 || dv.length() < 3.0) return;
               Point oneunit(1.0,1.0,0.0);
               Point origin(0.0,0.0,0.0);
               origin = window->screentomodel(origin);
               oneunit = window->screentomodel(oneunit);

               //double delta = min(xspacing,yspacing) * 0.5;
               //Point pright = window->getview()->getright();
               //Point pup = window->getview()->getup();
               //double sc = window->getpixelscale() * 6.0;

               duv = du; duv.normalize();
               dvv = dv; dvv.normalize();

               if( gridstyle == 0)
               { // points
                   pcol = window->getGridMajorColour();
                   //fr=GetRValue(pcol)/255.0;
                   //fg=GetGValue(pcol)/255.0;
                   //fb=GetBValue(pcol)/255.0;
                   //glColor3f(fr,fg,fb);
                   glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                   for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                   {
                     for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                     {
                          if (! referenceonly)
                          {
                              glPointSize(1.0);
                              glBegin(GL_POINTS);
                              glVertex3f(p2.x,p2.y,p2.z);
                              glEnd();
                             //painter->drawPoint(int(p2.x),window->getheight() - 1.0 - int(p2.y));
                             //window->dot(int(p2.x),int(p2.y));
                          }
                          if ((i % xreference) == 0 && (j % yreference) == 0)
                          {
                              //window->cross2dGL(p2,pright,pup,sc,sc);

                              glPointSize(3.0);
                              glBegin(GL_POINTS);
                              glVertex3f(p2.x,p2.y,p2.z);
                              glEnd();

                              //p3 = p2 - duv * delta;
                              //p4 = p2 + duv * delta;
                              //p5 = p2 - dvv * delta;
                              //p6 = p2 + dvv * delta;

                              //glBegin(GL_LINES);
                              //glVertex3f(p3.x,p3.y,p3.z);
                              //glVertex3f(p4.x,p4.y,p4.z);
                              //glVertex3f(p5.x,p5.y,p5.z);
                              //glVertex3f(p6.x,p6.y,p6.z);
                              //glEnd();
                              //painter->drawLine(int(p2.x-2),window->getheight() - 1.0 - int(p2.y),int(p2.x+2),window->getheight() - 1.0 - int(p2.y));
                              //painter->drawLine(int(p2.x),window->getheight() - 1.0 - int(p2.y-2),int(p2.x),window->getheight() - 1.0 - int(p2.y+2));
                          }
                     }
                   }
               }
               else
               {
                   // lines
                       // minor lines
                       // minor grid colour
                       pcol = window->getGridMinorColour();
                       //fr=GetRValue(pcol)/255.0;
                       //fg=GetGValue(pcol)/255.0;
                       //fb=GetBValue(pcol)/255.0;
                       //glColor3f(fr,fg,fb);
                       glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                       for (i = imin,p1 = o ; i <= imax  ; i++,p1 += du)
                       {
                           p2 = p1 - dvv * yspacing;
                           p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                           if (! referenceonly && (i % xreference) != 0 )
                           {

                               glBegin(GL_LINES);
                               glVertex3f(p2.x,p2.y,p2.z);
                               glVertex3f(p3.x,p3.y,p3.z);
                               glEnd();
                           }
                       }
                       for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                       {
                           p2 = p1 - duv * xspacing;
                           p3 = p1 + duv * xspacing * (imax-imin+1);
                           if (! referenceonly && (j % yreference) != 0)
                           {
                               glBegin(GL_LINES);
                               glVertex3f(p2.x,p2.y,p2.z);
                               glVertex3f(p3.x,p3.y,p3.z);
                               glEnd();
                           }
                       }
                       // major lines
                       // major grid colour
                       pcol = window->getGridMajorColour();
                       //fr=GetRValue(pcol)/255.0;
                       //fg=GetGValue(pcol)/255.0;
                       //fb=GetBValue(pcol)/255.0;
                       //glColor3f(fr,fg,fb);
                       glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                       for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                       {
                           p2 = p1 - dvv * yspacing;
                           p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                           if ( (i % xreference) == 0 )
                           {
                               glBegin(GL_LINES);
                               glVertex3f(p2.x,p2.y,p2.z);
                               glVertex3f(p3.x,p3.y,p3.z);
                               glEnd();
                           }
                       }
                       for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                       {
                           p2 = p1 - duv * xspacing;
                           p3 = p1 + duv * xspacing * (imax-imin+1);
                           if ( (j % yreference) == 0 )
                           {
                               glBegin(GL_LINES);
                               glVertex3f(p2.x,p2.y,p2.z);
                               glVertex3f(p3.x,p3.y,p3.z);
                               glEnd();
                           }
                       }
               }
            }
       }
     else
       {
         if (fabs(window->getwaxis().dot(db.workplanes.getcurrent()->getzaxis())) < 0.07)  // angle greater than 86 degrees.
            return;
         //qDebug() << "Grid::draw : part 2";

          for (k = 0 ; k < 4 ; k++)
          {
              // todo fix the size of the rotated grid
              x = (k % 2 == 0 ? 0 : int(window->getwidth())-1);
              y = (k / 2 == 0 ? 0 : int(window->getheight())-1);

              if (! window->coordinate3d(x,y,&p1,0))
                return;
              p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);

              if (p1.x < 0.0)
                {  p1.x = -p1.x;  sx = -1;
                }
              else
                {  sx = 1;
                }
              if (p1.y < 0.0)
                {  p1.y = -p1.y;  sy = -1;
                }
              else
                {  sy = 1;
                }
               if (fabs(p1.x / xspacing) > INT_MAX - 100 || fabs(p1.y / yspacing) > INT_MAX-100)
                 return;
               i = int(p1.x / xspacing);  i *= sx;
               j = int(p1.y / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
          }

          //if (drawmode == DM_ERASE)
          //  window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
          //else
            window->setup(1.0,0.0,DM_GRID,0,0,0,0);

          GLfloat lwidth = window->curPen.widthF();
          glLineWidth(lwidth<1.0?1.0:lwidth);

           if (window->getview()->getperspective())
            {
              //qDebug() << "Grid::draw : part 2 : with pers";
              u = imin * xspacing;  v = jmin * yspacing;
              o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
              o = db.workplanes.getcurrent()->workplanetomodel(o);
              p2.setxyz(cosa * (imin+1 * xspacing) - sina * jmin * yspacing,sina * (imin+1 * xspacing) + cosa * jmin * yspacing,depth);
              p2 = db.workplanes.getcurrent()->workplanetomodel(p2);
              du = p2 - o;
              p2.setxyz(cosa * imin * xspacing - sina * (jmin+1) * yspacing,sina * imin * xspacing + cosa * (jmin+1) * yspacing,depth);
              p2 = db.workplanes.getcurrent()->workplanetomodel(p2);
              dv = p2 - o;

              //Point oneunit(1.0,1.0,0.0);
              //Point origin(0.0,0.0,0.0);
              //origin = window->screentomodel(origin);
              //oneunit = window->screentomodel(oneunit);
              //double delta = (oneunit-origin).length()*2.0;
              //double delta = min(xspacing,yspacing) * 0.5;
              Point pright = window->getview()->getright();
              Point pup = window->getview()->getup();
              double delta = window->getpixelscale() * 4.0;

              duv = du; duv.normalize();
              dvv = dv; dvv.normalize();

              if(gridstyle == 0)
              { // points
                  pcol = window->getGridMajorColour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                  for (i = imin ; i <= imax ; i++)
                  {
                     for (j = jmin ; j <= jmax ; j++)
                       {
                          u = i * xspacing;  v = j * yspacing;
                          p1.x =  cosa * u - sina * v;
                          p1.y =  sina * u + cosa * v;
                          p1.z = depth;
                          p1 = db.workplanes.getcurrent()->workplanetomodel(p1);

                          p1s = window->modeltoscreen(p1);
                          //p1 = window->modeltoscreen(p1);
                          if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0)
                              return;
                          //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                          //double delta = min(xspacing,yspacing) * 0.5;
                          if (! referenceonly)
                          {
                              glPointSize(1.0);
                              glBegin(GL_POINTS);
                              glVertex3f(p1.x,p1.y,p1.z);
                              glEnd();
                              //QPointF qpf = ((QGraphicsView*)window->getWidget())->mapFromScene(p1.x,p1.y);
                              //painter->drawPoint(int(p1.x),window->getheight() - 1.0 - int(p1.y));
                              //window->dot(int(p1.x),int(p1.y));
                          }
                          if ((i % xreference) == 0 && (j % yreference) == 0)
                          {
                              //window->cross2dGL(p1,pright,pup,delta+delta,delta+delta);

                              glPointSize(3.0);
                              glBegin(GL_POINTS);
                              glVertex3f(p1.x,p1.y,p1.z);
                              glEnd();

                              //p3 = p1 - duv * delta;
                              //p4 = p1 + duv * delta;
                              //p5 = p1 - dvv * delta;
                              //p6 = p1 + dvv * delta;

                              //glBegin(GL_LINES);
                              //glVertex3f(p3.x,p3.y,p3.z);
                              //glVertex3f(p4.x,p4.y,p4.z);
                              //glVertex3f(p5.x,p5.y,p5.z);
                              //glVertex3f(p6.x,p6.y,p6.z);
                              //glEnd();
                              //painter->drawLine(int(p1.x-2),window->getheight() - 1.0 - int(p1.y),int(p1.x+2),window->getheight() - 1.0 - int(p1.y));
                              //painter->drawLine(int(p1.x),window->getheight() - 1.0 - int(p1.y-2),int(p1.x),window->getheight() - 1.0 - int(p1.y+2));
                          }
                          //p2 = p1;
                          p2s = p1s;
                       }
                   }
              }
              else
              { // lines
                  // minor lines
                  // minor grid colour
                  pcol = window->getGridMinorColour();
                  //RCCOLORREF pcol = window->getcolour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                  j = jmin;
                  for (i = imin,p1 = o ; i <= imax   ; i++,p1 += du)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);

                      p1s = window->modeltoscreen(p1);
                      //p1 = window->modeltoscreen(p1);
                      if (i > imin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0)
                          return;

                      p2 = p1 - dvv * yspacing * (jmax-jmin+1);
                      p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                      if (! referenceonly && (i % xreference) != 0 )
                      {

                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
                  i = imin;
                  for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);

                      p1s = window->modeltoscreen(p1);
                      //p1 = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0)
                          return;

                      p2 = p1 - duv * xspacing * (imax-imin+1);
                      p3 = p1 + duv * xspacing * (imax-imin+1);
                      if (! referenceonly && (j % yreference) != 0)
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                  }
                  // major lines
                  // major grid colour
                  pcol = window->getGridMajorColour();
                  //fr=GetRValue(pcol)/255.0;
                  //fg=GetGValue(pcol)/255.0;
                  //fb=GetBValue(pcol)/255.0;
                  //glColor3f(fr,fg,fb);
                  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));

                  j=jmin;
                  for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);

                      p1s = window->modeltoscreen(p1);
                      //p1 = window->modeltoscreen(p1);
                      if (i > imin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0)
                          return;

                      p2 = p1 - dvv * yspacing * (jmax-jmin+1);
                      p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                      if ( (i % xreference) == 0 )
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                      //else
                      //    qDebug() << "line not drawn";
                  }
                  i = imin;
                  for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                  {
                      u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);

                      p1s = window->modeltoscreen(p1);
                      //p1 = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2s.x - p1s.x) < 2.0 && fabs(p2s.y - p1s.y) < 2.0)
                          return;

                      p2 = p1 - duv * xspacing * (imax-imin+1);
                      p3 = p1 + duv * xspacing * (imax-imin+1);
                      if ( (j % yreference) == 0 )
                      {
                          glBegin(GL_LINES);
                          glVertex3f(p2.x,p2.y,p2.z);
                          glVertex3f(p3.x,p3.y,p3.z);
                          glEnd();
                      }
                      //else
                      //    qDebug() << "line not drawn";
                  }
              }
            }
          else
            {  //qDebug() << "Grid::draw : part 2 : no pers";

               u = imin * xspacing;  v = jmin * yspacing;
               o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               o = db.workplanes.getcurrent()->workplanetomodel(o);
               //o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
               //p1 = window->modeltoscreen(p1);
               du = p1 - o;
               dus = window->modeltoscreen(p1) - window->modeltoscreen(o);
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
               //p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               dvs = window->modeltoscreen(p1) - window->modeltoscreen(o);
               if (dus.length() < 3.0 || dvs.length() < 3.0)
                   return;
               //if (du.length() < 3.0 || dv.length() < 3.0) return;
               //Point oneunit(1.0,1.0,0.0);
               //Point origin(0.0,0.0,0.0);
               //origin = window->screentomodel(origin);
               //oneunit = window->screentomodel(oneunit);
               //double delta = (oneunit-origin).length()*2.0;
               //double delta = min(xspacing,yspacing) * 0.5;
               Point pright = window->getview()->getright();
               Point pup = window->getview()->getup();
               double delta = window->getpixelscale() * 4.0;

               duv = du; duv.normalize();
               dvv = dv; dvv.normalize();

               if(gridstyle == 0)
               { // points
                   //pcol = window->getcolour();
                   pcol = window->getGridMajorColour();
                   //fr=GetRValue(pcol)/255.0;
                   //fg=GetGValue(pcol)/255.0;
                   //fb=GetBValue(pcol)/255.0;
                   //glColor3f(fr,fg,fb);
                   glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                   //painter->setPen(window->curPen);

                   for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                   {
                     for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                     {
                         if (! referenceonly)
                         {
                             glPointSize(1.0);
                             glBegin(GL_POINTS);
                             glVertex3f(p2.x,p2.y,p2.z);
                             glEnd();
                             //painter->drawPoint(int(p2.x),window->getheight() - 1.0 - int(p2.y));
                             //window->dot(int(p2.x),int(p2.y));
                         }
                         if ((i % xreference) == 0 && (j % yreference) == 0)
                         {
                             //window->cross2dGL(p2,pright,pup,delta+delta,delta+delta);

                             glPointSize(3.0);
                             glBegin(GL_POINTS);
                             glVertex3f(p2.x,p2.y,p2.z);
                             glEnd();

                             //p3 = p2 - duv * delta;
                             //p4 = p2 + duv * delta;
                             //p5 = p2 - dvv * delta;
                             //p6 = p2 + dvv * delta;

                             //glBegin(GL_LINES);
                             //glVertex3f(p3.x,p3.y,p3.z);
                             //glVertex3f(p4.x,p4.y,p4.z);
                             //glVertex3f(p5.x,p5.y,p5.z);
                             //glVertex3f(p6.x,p6.y,p6.z);
                             //glEnd();
                             //painter->drawLine(int(p2.x-2),window->getheight() - 1.0 - int(p2.y),int(p2.x+2),window->getheight() - 1.0 - int(p2.y));
                             //painter->drawLine(int(p2.x),window->getheight() - 1.0 - int(p2.y-2),int(p2.x),window->getheight() - 1.0 - int(p2.y+2));
                         }
                      }
                   }
               }
               else
               { // lines
                   // minor lines
                   // minor grid colour
                   pcol = window->getGridMinorColour();
                   //fr=GetRValue(pcol)/255.0;
                   //fg=GetGValue(pcol)/255.0;
                   //fb=GetBValue(pcol)/255.0;
                   //glColor3f(fr,fg,fb);
                   glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                   for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                   {
                       p2 = p1 - dvv * yspacing;
                       p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                       if (! referenceonly && (i % xreference) != 0 )
                       {

                           glBegin(GL_LINES);
                           glVertex3f(p2.x,p2.y,p2.z);
                           glVertex3f(p3.x,p3.y,p3.z);
                           glEnd();
                       }
                   }
                   for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                   {
                       p2 = p1 - duv * xspacing;
                       p3 = p1 + duv * xspacing * (imax-imin+1);
                       if (! referenceonly && (j % yreference) != 0)
                       {
                           glBegin(GL_LINES);
                           glVertex3f(p2.x,p2.y,p2.z);
                           glVertex3f(p3.x,p3.y,p3.z);
                           glEnd();
                       }
                   }
                   // major lines
                   // major grid colour
                   pcol = window->getGridMajorColour();
                   //RCCOLORREF pcol = window->getcolour();
                   //fr=GetRValue(pcol)/255.0;
                   //fg=GetGValue(pcol)/255.0;
                   //fb=GetBValue(pcol)/255.0;
                   //glColor3f(fr,fg,fb);
                   glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                   for (i = imin,p1 = o ; i <= imax ; i++,p1 += du)
                   {
                       p2 = p1 - dvv * yspacing;
                       p3 = p1 + dvv * yspacing * (jmax-jmin+1);
                       if ( (i % xreference) == 0 )
                       {
                           glBegin(GL_LINES);
                           glVertex3d(p2.x,p2.y,p2.z);
                           glVertex3d(p3.x,p3.y,p3.z);
                           glEnd();
                       }
                   }
                   for (j = jmin,p1 = o ; j <= jmax ; j++,p1 += dv)
                   {
                       p2 = p1 - duv * xspacing;
                       p3 = p1 + duv * xspacing * (imax-imin+1);
                       if ( (j % yreference) == 0 )
                       {
                           glBegin(GL_LINES);
                           glVertex3f(p2.x,p2.y,p2.z);
                           glVertex3f(p3.x,p3.y,p3.z);
                           glEnd();
                       }
                   }
               }
            }
       }
}

void Grid::draw(int drawmode,View3dSurface *window, QPainter *painter)
{
    long i,j,imin,imax,jmin,jmax;
    int k,sx,sy,x,y;
    double u,v,depth;
    Point p1,p2,o,du,dv;

    if(painter == 0) return;

     if (cadwindow != 0)
       cadwindow->updatetitle();

     if (drawmode != DM_ERASE && ! display)
         return;
     imin = jmin = imax = jmax = 0;
     depth = ::v.getreal("vw::depth");
     if (plane == onviewplane || window->getsurfacetype() == View2dSurf || db.workplanes.getcurrent() == NULL)
       {
         //qDebug() << "Grid::draw : part 1";
         for (k = 0 ; k < 4 ; k++)
            {  p1.x = (k % 2 == 0 ? window->getumin() : window->getumax());
               p1.y = (k / 2 == 0 ? window->getvmin() : window->getvmax());
               p1.z = depth;
               u =  cosa * p1.x + sina * p1.y;
               v = -sina * p1.x + cosa * p1.y;
               if (window->getsurfacetype() == View2dSurf)
                 {  if (u < 0.0) u = 0.0;
                    if (v < 0.0) v = 0.0;
                    if (u > ((View2dWindow *)window)->getview2d()->getpaperwidth())
                      u = ((View2dWindow *)window)->getview2d()->getpaperwidth();
                    if (v > ((View2dWindow *)window)->getview2d()->getpaperheight())
                      v = ((View2dWindow *)window)->getview2d()->getpaperheight();
                 }
               if (u < 0.0)
                 {  u = -u;  sx = -1;
                 }
               else
                 {  sx = 1;
                 }
               if (v < 0.0)
                 {  v = -v;  sy = -1;
                 }
               else
                 {  sy = 1;
                 }
               if (fabs(u / xspacing) > INT_MAX - 100 || fabs(v / yspacing) > INT_MAX-100)
                 return;
               i = int(u / xspacing);  i *= sx;
               j = int(v / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
            }
          if (drawmode == DM_ERASE)
            window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
          else
            window->setup(1.0,0.0,DM_GRID,0,0,0,0);


          painter->setPen(window->curPen);

          if (window->getview()->getperspective())
            {
              for (i = imin ; i <= imax  && ! program->getinterrupt() ; i++)
              {
                 for (j = jmin ; j <= jmax ; j++)
                   {  u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = window->uvtomodel(p1);
                      p1 = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                      if (! referenceonly)
                      {
                            painter->drawPoint(int(p1.x),window->getheight() - 1.0 - int(p1.y));
                            //painter->drawPoint(int(p1.x),int(p1.y));
                            //painter->drawPoint(int(p1.x),int(p1.y));
                            //window->dot(int(p1.x),int(p1.y));
                      }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {
                          painter->drawLine(int(p1.x-2),window->getheight() - 1.0 - int(p1.y),int(p1.x+2),window->getheight() - 1.0 - int(p1.y));
                          painter->drawLine(int(p1.x),window->getheight() - 1.0 - int(p1.y-2),int(p1.x),window->getheight() - 1.0 - int(p1.y+2));
                          //painter->drawLine(int(p1.x-2),int(p1.y),int(p1.x+2),int(p1.y));
                          //painter->drawLine(int(p1.x),int(p1.y-2),int(p1.x),int(p1.y+2));
                      }
                      p2 = p1;
                   }
                 }
            }
          else
            {  u = imin * xspacing;  v = jmin * yspacing;
               o.x =  cosa * u - sina * v;
               o.y =  sina * u + cosa * v;
               o.z = window->getdepth();
               o = window->uvtomodel(o);
               o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = depth;
               p1 = window->uvtomodel(p1);
               p1 = window->modeltoscreen(p1);
               du = p1 - o;
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.x =  cosa * u - sina * v;
               p1.y =  sina * u + cosa * v;
               p1.z = window->getdepth();
               p1 = window->uvtomodel(p1);
               p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               if (du.length() < 2.0 || dv.length() < 2.0) return;
               //if (du.length() < 3.0 || dv.length() < 3.0) return;
               for (i = imin,p1 = o ; i <= imax && ! program->getinterrupt()  ; i++,p1 += du)
               {
                 for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                 {
                      if (! referenceonly)
                      {
                         painter->drawPoint(int(p2.x),window->getheight() - 1.0 - int(p2.y));
                         //painter->drawPoint(int(p2.x),int(p2.y));
                         //painter->drawPoint(int(p2.x),int(p2.y));
                         //window->dot(int(p2.x),int(p2.y));
                      }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {
                          painter->drawLine(int(p2.x-2),window->getheight() - 1.0 - int(p2.y),int(p2.x+2),window->getheight() - 1.0 - int(p2.y));
                          painter->drawLine(int(p2.x),window->getheight() - 1.0 - int(p2.y-2),int(p2.x),window->getheight() - 1.0 - int(p2.y+2));
                          //painter->drawLine(int(p2.x-2),int(p2.y),int(p2.x+2),int(p2.y));
                          //painter->drawLine(int(p2.x),int(p2.y-2),int(p2.x),int(p2.y+2));
                      }
                 }
               }
            }
       }
     else
       {
         if (fabs(window->getwaxis().dot(db.workplanes.getcurrent()->getzaxis())) < 0.07)  // angle greater than 86 degrees.
            return;
         //qDebug() << "Grid::draw : part 2";

          for (k = 0 ; k < 4 ; k++)
          {
               x = (k % 2 == 0 ? 0 : int(window->getwidth())-1);
               y = (k / 2 == 0 ? 0 : int(window->getheight())-1);
               if (! window->coordinate3d(x,y,&p1,0))
                 return;
               p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
               if (p1.x < 0.0)
                 {  p1.x = -p1.x;  sx = -1;
                 }
               else
                 {  sx = 1;
                 }
               if (p1.y < 0.0)
                 {  p1.y = -p1.y;  sy = -1;
                 }
               else
                 {  sy = 1;
                 }
               if (fabs(p1.x / xspacing) > INT_MAX - 100 || fabs(p1.y / yspacing) > INT_MAX-100)
                 return;
               i = int(p1.x / xspacing);  i *= sx;
               j = int(p1.y / yspacing);  j *= sy;
               if (k == 0 || i < imin) imin = i;
               if (k == 0 || i > imax) imax = i;
               if (k == 0 || j < jmin) jmin = j;
               if (k == 0 || j > jmax) jmax = j;
          }
          //if ((drawmode & DM_ERASE) == DM_ERASE)
          if (drawmode == DM_ERASE)
            window->setup(1.0,0.0,DM_ERASE,0,0,0,0);
          else
            window->setup(1.0,0.0,DM_GRID,0,0,0,0);

          painter->setPen(window->curPen);
          //painter->setPen(QPen(QBrush(QColor(0,0,0)),1));
          //window->curPen.setCapStyle(Qt::SquareCap);
          //window->curPen.setCapStyle(Qt::FlatCap);

          if (window->getview()->getperspective())
            {
               for (i = imin ; i <= imax && ! program->getinterrupt() ; i++)
               {
                 for (j = jmin ; j <= jmax ; j++)
                   {  u = i * xspacing;  v = j * yspacing;
                      p1.x =  cosa * u - sina * v;
                      p1.y =  sina * u + cosa * v;
                      p1.z = depth;
                      p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
                      p1 = window->modeltoscreen(p1);
                      if (j > jmin && fabs(p2.x - p1.x) < 2.0 && fabs(p2.y - p1.y) < 2.0) return;
                      //if (j > jmin && fabs(p2.x - p1.x) < 3.0 && fabs(p2.y - p1.y) < 3.0) return;
                      if (! referenceonly)
                      {
                          //QPointF qpf = ((QGraphicsView*)window->getWidget())->mapFromScene(p1.x,p1.y);
                          //painter->drawPoint(qpf);
                          //painter->drawPoint(int(p1.x),int(p1.y));
                          painter->drawPoint(int(p1.x),window->getheight() - 1.0 - int(p1.y));
                          //window->dot(int(p1.x),int(p1.y));
                      }
                      if ((i % xreference) == 0 && (j % yreference) == 0)
                      {
                          painter->drawLine(int(p1.x-2),window->getheight() - 1.0 - int(p1.y),int(p1.x+2),window->getheight() - 1.0 - int(p1.y));
                          painter->drawLine(int(p1.x),window->getheight() - 1.0 - int(p1.y-2),int(p1.x),window->getheight() - 1.0 - int(p1.y+2));
                          //painter->drawLine(int(p1.x-2),int(p1.y),int(p1.x+2),int(p1.y));
                          //painter->drawLine(int(p1.x),int(p1.y-2),int(p1.x),int(p1.y+2));
                      }
                      p2 = p1;
                   }
               }
            }
          else
            {  //qDebug() << "Grid::draw : part 2 : no pers";

               u = imin * xspacing;  v = jmin * yspacing;
               o.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               o = db.workplanes.getcurrent()->workplanetomodel(o);
               o = window->modeltoscreen(o);
               u = (imin+1) * xspacing;  v = jmin * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,depth);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
               p1 = window->modeltoscreen(p1);
               du = p1 - o;
               u = imin * xspacing;  v = (jmin+1) * yspacing;
               p1.setxyz(cosa * u - sina * v,sina * u + cosa * v,0.0);
               p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
               p1 = window->modeltoscreen(p1);
               dv = p1 - o;
               if (du.length() < 2.0 || dv.length() < 2.0) return;
               //if (du.length() < 3.0 || dv.length() < 3.0) return;
               for (i = imin,p1 = o ; i <= imax  && ! program->getinterrupt() ; i++,p1 += du)
               {
                 for (j = jmin,p2 = p1 ; j <= jmax ; j++,p2 += dv)
                 {
                     if (! referenceonly)
                     {
                         //glBegin(GL_POINTS);
                         //glVertex2f(p1.x,p1.y);
                         //glEnd();
                         painter->drawPoint(int(p2.x),window->getheight() - 1.0 - int(p2.y));
                         //painter->drawPoint(int(p2.x),int(p2.y));
                         //window->dot(int(p2.x),int(p2.y));
                     }
                     if ((i % xreference) == 0 && (j % yreference) == 0)
                     {
                         //glBegin(GL_LINES);
                         //glVertex2f(p1.x-2,p1.y);
                         //glVertex2f(p1.x+2,p1.y);
                         //glVertex2f(p1.x,p1.y-2);
                         //glVertex2f(p1.x,p1.y+2);
                         //glEnd();

                         painter->drawLine(int(p2.x-2),window->getheight() - 1.0 - int(p2.y),int(p2.x+2),window->getheight() - 1.0 - int(p2.y));
                         painter->drawLine(int(p2.x),window->getheight() - 1.0 - int(p2.y-2),int(p2.x),window->getheight() - 1.0 - int(p2.y+2));
                         //painter->drawLine(int(p2.x-2),int(p2.y),int(p2.x+2),int(p2.y));
                         //painter->drawLine(int(p2.x),int(p2.y-2),int(p2.x),int(p2.y+2));
                     }
                  }
               }
            }
       }
}
#endif

void Grid::setplane(int p)
{
    if (display && plane != p)
     {  cadwindow->drawcallback(DM_ERASE,this,drawgrid);
        plane = short(p);
        cadwindow->drawcallback(DM_NORMAL,this,drawgrid);
     }
   else
     plane = short(p);
}

void grid_command1(int *cstate,HighLevelEvent *,void **)
{
  db.grid.setup();
  *cstate = ExitState;
}

void grid_command2(int *cstate,HighLevelEvent *,void **)
{RCHMENU hmenu;
 int i;
 RCCHAR label[300];
 ResourceString rs0(NCGRID);
  db.grid.setplane(onworkplane);
  v.setinteger("gd::options",(v.getinteger("gd::options")&3));

  if ((hmenu = GetMenu(cadwindow->gethwnd())) != NULL)
    {
#ifdef USING_WIDGETS
      QAction *planeAct = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_work_plane;
      QAction *viewAct  = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_view;
      if(planeAct)
          planeAct->setChecked(true);
      if(viewAct)
          viewAct->setChecked(false);
#else
      for (i = 0 ; ; i++)
         {  if (GetMenuString(hmenu,i,label,127,MF_BYPOSITION))
              {  if (strcmp(label,rs0.gets()) == 0)
                   {  CheckMenuItem(GetSubMenu(hmenu,i),0,MF_BYPOSITION | MF_CHECKED);
                      CheckMenuItem(GetSubMenu(hmenu,i),1,MF_BYPOSITION | MF_UNCHECKED);
                   }
              }
            else
              break;
         }
#endif
    }

  *cstate = ExitState;
}

void grid_command3(int *cstate,HighLevelEvent *,void **)
{
    RCHMENU hmenu;
    int i;
    RCCHAR label[300];
    ResourceString rs0(NCGRID);
    db.grid.setplane(onviewplane);
    v.setinteger("gd::options",(v.getinteger("gd::options")&3)+4);

    if ((hmenu = GetMenu(cadwindow->gethwnd())) != NULL)
    {
#ifdef USING_WIDGETS
        QAction *planeAct = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_work_plane;
        QAction *viewAct  = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_view;
        if(planeAct)
            planeAct->setChecked(false);
        if(viewAct)
            viewAct->setChecked(true);
#else
        for (i = 0 ; ; i++)
        {
            if (GetMenuString(hmenu,i,label,127,MF_BYPOSITION))
            {
                if (strcmp(label,rs0.gets()) == 0)
                {
                      CheckMenuItem(GetSubMenu(hmenu,i),0,MF_BYPOSITION | MF_UNCHECKED);
                      CheckMenuItem(GetSubMenu(hmenu,i),1,MF_BYPOSITION | MF_CHECKED);
                }
            }
            else
              break;
        }
#endif
    }

    *cstate = ExitState;
}

/**
 * @brief grid_command4
 * @param cstate
 * Alt+G key
 * set grid display on or off?
 */
void grid_command4(int *cstate,HighLevelEvent *,void **)
{
    //qDebug() << "grid_command4";
    //qDebug() << "db.grid.getdisplay() : before " << db.grid.getdisplay();
    db.grid.setdisplay(! db.grid.getdisplay());
    if (cadwindow != 0)
    {
        cadwindow->updatetitle();
        cadwindow->paintall();
    }
    //qDebug() << "db.grid.getdisplay() : after " << db.grid.getdisplay();
    *cstate = ExitState;
}

/**
 * @brief grid_command5
 * @param cstate
 * Alt+S key
 * set grid snap on or off
 */
void grid_command5(int *cstate,HighLevelEvent *,void **)
{
    db.grid.setsnap(! db.grid.getsnap());
    if (cadwindow != 0)
        cadwindow->updatetitle();
    *cstate = ExitState;
}


int Grid::save(CadFile *outfile)
{
    short type,temp;
    type = grid_record;
    if (! outfile->write(&type)) return 0;
    if (! outfile->write(&xspacing)) return 0;
    if (! outfile->write(&yspacing)) return 0;
    if (! outfile->write(&xreference)) return 0;
    if (! outfile->write(&yreference)) return 0;
    if (! outfile->write(&angle)) return 0;
    if (! outfile->write(&display)) return 0;
    if (! outfile->write(&snap)) return 0;
    temp = short(plane + referenceonly * 2 + style * 4 + custcolour * 8);
    if (! outfile->write(&temp)) return 0;
    return 1;
}

int Grid::load(CadFile *infile,Database *database)
{
    RCHMENU hmenu;
    short temp;
    if (! infile->read(&database->grid.xspacing)) return 0;
    if (! infile->read(&database->grid.yspacing)) return 0;
    if (! infile->read(&database->grid.xreference)) return 0;
    if (! infile->read(&database->grid.yreference)) return 0;
    if (! infile->read(&database->grid.angle)) return 0;
    if (! infile->read(&database->grid.display)) return 0;
    if (! infile->read(&database->grid.snap)) return 0;
    if (! infile->read(&temp)) return 0;
    database->grid.plane = short(temp & 1);
    database->grid.referenceonly = short((temp & 2) >> 1);
    database->grid.style = short((temp & 4) >> 2);
    database->grid.custcolour = short((temp & 8) >> 3);
    if (cadwindow != 0 && (hmenu = GetMenu(cadwindow->gethwnd())) != 0)
    {
#ifdef USING_WIDGETS
        QAction *planeAct = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_work_plane;
        QAction *viewAct  = ((QtMainWindow*)cadwindow->gethwnd())->action_Options_Cursor_on_view;
        if(planeAct)
            planeAct->setChecked(!database->grid.plane);
        if(viewAct)
            viewAct->setChecked(database->grid.plane);
#else
        CheckMenuItem(GetSubMenu(hmenu,3),0,MF_BYPOSITION | (! database->grid.plane ? MF_CHECKED : MF_UNCHECKED));
        CheckMenuItem(GetSubMenu(hmenu,3),1,MF_BYPOSITION | (database->grid.plane ? MF_CHECKED : MF_UNCHECKED));
#endif
    }
    return 1;
}
