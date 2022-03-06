
#include "ncwin.h"
#include "Ncdialog.h"

#include "RCDialog.h"
#include "definecolour_dialog.h"
#include "ui_definecolour_dialog.h"

class DefineColour_Dialog;

bool MousePressFilter::eventFilter(QObject *obj, QEvent *event)
{
    bool status=false;

    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonDblClick)
    {
        if(obj->objectName()== "_121")
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                QPoint ipos = ((QMouseEvent*)event)->pos();
                QWidget *dialog = app->activeModalWidget();
                if(dialog)
                {
                    if(((DefineColour_Dialog*)dialog)->ui->_122->rect().contains(ipos))
                    {
                        QRgb col = ((DefineColour_Dialog*)dialog)->hlsColourImage->pixel(ipos);
                        ((DefineColour_Dialog*)dialog)->ui->_101->setValue((double)qRed(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_102->setValue((double)qGreen(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_103->setValue((double)qBlue(col)/255.0);

                        if(event->type() == QEvent::MouseButtonDblClick)
                            ((DefineColour_Dialog*)dialog)->on__1_clicked(false);

                        status = true;
                    }
                }
            }
        }
        else if( obj->objectName()== "_122")
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                QPoint ipos = ((QMouseEvent*)event)->pos();
                QWidget *dialog = app->activeModalWidget();
                if(dialog)
                {
                    if(((DefineColour_Dialog*)dialog)->ui->_122->rect().contains(ipos))
                    {
                        QRgb col = ((DefineColour_Dialog*)dialog)->grayScaleImage->pixel(ipos);
                        ((DefineColour_Dialog*)dialog)->ui->_101->setValue((double)qRed(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_102->setValue((double)qGreen(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_103->setValue((double)qBlue(col)/255.0);

                        if(event->type() == QEvent::MouseButtonDblClick)
                            ((DefineColour_Dialog*)dialog)->on__1_clicked(false);

                        status = true;
                    }
                }
            }
        }
        else if( obj->objectName()== "_123")
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                QPoint ipos = ((QMouseEvent*)event)->pos();
                QPoint gpos = ((QMouseEvent*)event)->globalPos();
                QWidget *dialog = app->activeModalWidget();
                if(dialog)
                {
                    if(((DefineColour_Dialog*)dialog)->ui->_123->rect().contains(ipos))
                    {
                        QRect irect = ((DefineColour_Dialog*)dialog)->ui->_123->rect();
                        int tileSizeX = irect.width() / 16;
                        int tileSizeY = irect.height() / 16;
                        int colourIndex = (ipos.y()/tileSizeY*16)+(ipos.x()/tileSizeX);

                        // if we are being used for other colours we can set the index value control
                        if(((DefineColour_Dialog*)dialog)->rcDialog() && ((DefineColour_Dialog*)dialog)->rcDialog()->getcontrol(114))
                            if(((ColourApplyButtonDialogControl*)((DefineColour_Dialog*)dialog)->rcDialog()->getcontrol(114))->mode != 0)
                                ((DefineColour_Dialog*)dialog)->ui->_100->setValue(colourIndex);

                        QToolTip::showText(gpos,QString("%1").arg(colourIndex),((DefineColour_Dialog*)dialog)->ui->_123,irect);

                        QRgb col = ((DefineColour_Dialog*)dialog)->curPaletteImage->pixel(ipos);

                        //qDebug() << "col Red   : " << (double)qRed(col)/255.0;
                        //qDebug() << "col Green : " << (double)qGreen(col)/255.0;
                        //qDebug() << "col Blue  : " << (double)qBlue(col)/255.0;

                        ((DefineColour_Dialog*)dialog)->ui->_101->setValue((double)qRed(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_102->setValue((double)qGreen(col)/255.0);
                        ((DefineColour_Dialog*)dialog)->ui->_103->setValue((double)qBlue(col)/255.0);


                        if(event->type() == QEvent::MouseButtonDblClick)
                            ((DefineColour_Dialog*)dialog)->on__1_clicked(false);

                        status = true;
                    }
                }
            }
        }
        else if( obj->objectName()== "_113")
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                QRgb col = ((QGraphicsView*)obj)->scene()->backgroundBrush().color().rgb();
                ((DefineColour_Dialog*)obj->parent())->ui->_101->setValue((double)qRed(col)/255.0);
                ((DefineColour_Dialog*)obj->parent())->ui->_102->setValue((double)qGreen(col)/255.0);
                ((DefineColour_Dialog*)obj->parent())->ui->_103->setValue((double)qBlue(col)/255.0);

                status = true;
            }
       }

       return status ? status : QObject::eventFilter(obj, event);
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

DefineColour_Dialog::DefineColour_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DefineColour_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
DefineColour_Dialog::DefineColour_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent, parent, f),
    ui(new Ui::DefineColour_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  // put any inits not known to the realCAD Dialog class here

  // add a scene to the current colour sample graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_113)
      ui->_113->setScene(scene);

  //((GPColourDisplayDialogControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);
  if(dynamic_cast<GPColourSampleDialogControl *>(RCdlg->getcontrol(113)))
      ((GPColourSampleDialogControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);
  else if(dynamic_cast<OtherColourSampleDialogControl *>(RCdlg->getcontrol(113)))
      ((OtherColourSampleDialogControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);
  //else if(dynamic_cast<GridColourDialogSampleControl *>(RCdlg->getcontrol(113)))
  //    ((GridColourDialogSampleControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);

  scene = new QGraphicsScene;
  if(ui->_115)
      ui->_115->setScene(scene);

   //((GPColourSampleDialogControl*)RCdlg->getcontrol(115))->sethwnd(ui->_115);
  if(dynamic_cast<GPColourSampleDialogControl *>(RCdlg->getcontrol(115)))
      ((GPColourSampleDialogControl*)RCdlg->getcontrol(115))->sethwnd(ui->_115);
  else if(dynamic_cast<OtherColourSampleDialogControl *>(RCdlg->getcontrol(115)))
      ((OtherColourSampleDialogControl*)RCdlg->getcontrol(115))->sethwnd(ui->_115);
 // else if(dynamic_cast<GridColourDialogSampleControl *>(RCdlg->getcontrol(115)))
  //    ((GridColourDialogSampleControl*)RCdlg->getcontrol(115))->sethwnd(ui->_115);

  scene = new QGraphicsScene;
  if(ui->_116)
      ui->_116->setScene(scene);

  //((GPColourSampleDialogControl*)RCdlg->getcontrol(116))->sethwnd(ui->_116);
  if(dynamic_cast<GPColourSampleDialogControl *>(RCdlg->getcontrol(116)))
      ((GPColourSampleDialogControl*)RCdlg->getcontrol(116))->sethwnd(ui->_116);
  else if(dynamic_cast<OtherColourSampleDialogControl *>(RCdlg->getcontrol(116)))
      ((OtherColourSampleDialogControl*)RCdlg->getcontrol(116))->sethwnd(ui->_116);
  //else if(dynamic_cast<GridColourDialogSampleControl *>(RCdlg->getcontrol(116)))
 //     ((GridColourDialogSampleControl*)RCdlg->getcontrol(116))->sethwnd(ui->_116);


  // init the realCAD Dialog class here
  initDialog(this);

  //if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
  //    ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->setIndex(-1);

  QRect irect = ui->_121->rect();
  hlsColourImage = new QImage(irect.size(),QImage::Format_RGB32);
  irect = ui->_122->rect();
  grayScaleImage = new QImage(irect.size(),QImage::Format_RGB32);
  irect = ui->_123->rect();
  curPaletteImage = new QImage(irect.size(),QImage::Format_RGB32);
  initHlsTab();
  initGrayTab();
  initPaletteTab();
  ui->_120->setCurrentIndex(0);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(accept()));
  //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  //connect((QWidget*)ui->_121,SIGNAL(mouseMoveEvent(QMouseEvent*)),this,SLOT(on_121_mousemove(QMouseEvent*)));

  MousePressFilter *mousePressFilter = new MousePressFilter(this);
  ui->_113->installEventFilter(mousePressFilter);
  ui->_121->installEventFilter(mousePressFilter);
  ui->_122->installEventFilter(mousePressFilter);
  ui->_123->installEventFilter(mousePressFilter);
}

DefineColour_Dialog::~DefineColour_Dialog()
{
    delete hlsColourImage;
    delete grayScaleImage;
    delete curPaletteImage;
    delete ui;
}

// current colour index spin box
void DefineColour_Dialog::on__100_valueChanged(int v)
{
    if((IntegerDialogControl*)RCdlg->getcontrol(100))
    {
        ((IntegerDialogControl*)RCdlg->getcontrol(100))->change(RCdlg,v);
        if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
            ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
        else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
            ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
        //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    }
}

// rgb spin boxes
void DefineColour_Dialog::on__101_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineColour_Dialog::on__102_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineColour_Dialog::on__103_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

// rgb sliders
void DefineColour_Dialog::on__104_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender());
}

void DefineColour_Dialog::on__105_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender() );
}

void DefineColour_Dialog::on__106_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender() );
}

// hls spin boxes
void DefineColour_Dialog::on__107_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineColour_Dialog::on__108_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineColour_Dialog::on__109_valueChanged(double v)
{
    if(dynamic_cast<GPColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((GPColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    else if(dynamic_cast<OtherColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
        ((OtherColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
    //else if(dynamic_cast<GridColourDisplayDialogControl *>(RCdlg->getcontrol(120)))
    //    ((GridColourDisplayDialogControl*)RCdlg->getcontrol(120))->draw(RCdlg,RCdlg->gethdlg());
}

// hls sliders
void DefineColour_Dialog::on__110_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender() );
}

void DefineColour_Dialog::on__111_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender() );
}

void DefineColour_Dialog::on__112_actionTriggered(int action)
{
    on_slider_actionTriggered(action, sender() );
}

void DefineColour_Dialog::on_slider_actionTriggered(int action, QObject *sender)
{
    //qDebug() << "on_slider_actionTriggered : " << action;

    int scrollcode,start,end,colour;
    int increment = 0,pos = 0;

    start = ((QSlider*)sender)->minimum();
    end = ((QSlider*)sender)->maximum();

    colour = int(double(((QSlider*)sender)->value() - start) / double(end - start) * 255.0 + 0.5);
    pos = ((QSlider*)sender)->sliderPosition();

    switch (action)
    {
    case QAbstractSlider::SliderNoAction:
    default: // 0
        //scrollcode = SB_ENDSCROLL;
        //increment = 0;
        return;
    case QAbstractSlider::SliderSingleStepAdd: // 1
        //scrollcode = SB_LINEUP;
        colour +=  ((QSlider*)sender)->singleStep();
        break;
    case QAbstractSlider::SliderSingleStepSub: // 2
        //scrollcode = SB_LINEDOWN;
        colour -=  ((QSlider*)sender)->singleStep();
        break;
    case QAbstractSlider::SliderPageStepAdd: // 3
        //scrollcode = SB_PAGEUP;
        colour +=  1;//((QSlider*)sender)->pageStep();
        break;
    case QAbstractSlider::SliderPageStepSub: // 4
        //scrollcode = SB_PAGEDOWN;
        colour -=  1;//((QSlider*)sender)->pageStep();
        break;
    case QAbstractSlider::SliderToMinimum: // 5
        //scrollcode = SB_BOTTOM;
        colour = start;
        break;
    case QAbstractSlider::SliderToMaximum: // 6
        //scrollcode = SB_TOP;
        colour = end;
        break;
   case QAbstractSlider::SliderMove: // 7
        //scrollcode = 10;
        colour = pos;
        break;
    }

    if (colour > 255) colour = 255;
    if (colour < 0) colour = 0;
    pos = start + int((end - start) * (double(colour)) / 255.0+0.5);
    ((QSlider*)sender)->setSliderPosition(pos);
    //SetScrollPos(GetDlgItem(dialog->gethdlg(),id),SB_CTL,pos,TRUE);

    int id = RCnameToInt((sender->objectName()));
    if (id == 104)
        ui->_101->setValue((double)colour/255.0);
    else if (id == 105)
        ui->_102->setValue((double)colour/255.0);
    else if (id == 106)
        ui->_103->setValue((double)colour/255.0);
    else if (id == 110)
        ui->_107->setValue((double)colour/255.0 * 360.0);
    else if (id == 111)
        ui->_108->setValue((double)colour/255.0);
    else if (id == 112)
        ui->_109->setValue((double)colour/255.0);
}

void DefineColour_Dialog::on__114_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void DefineColour_Dialog::on__120_currentChanged(int index)
{
}

void DefineColour_Dialog::initHlsTab()
{
    QRect irect = ui->_121->rect();
    int ix,iy;
    QRgb value;
    Colour c;
    double h,l,s,hi,si;

    h = 0.0;
    hi = 360.0 / (double)irect.width();
    l = 0.5;
    s = 1.0;
    si = 1.0 / (double)irect.height();

    uint *px=0;
    int scanIndex=0;
    for(iy=irect.top(); iy<=irect.bottom(); iy++)
    {
        h = 0.0;
        px = (uint*)hlsColourImage->scanLine(scanIndex);
        for(ix=irect.left(); ix<=irect.right(); ix++)
        {
            c.sethls(h,l,s);
            QColor qcol(c.red, c.green, c.blue);
            *px++ = qcol.rgba();
            h += hi;
        }
        s -= si;
        scanIndex++;
    }

    ui->_121->setPixmap(QPixmap::fromImage(*hlsColourImage));
}

void DefineColour_Dialog::initGrayTab()
{
    QRect irect = ui->_122->rect();
    int ix,iy;
    uint *px=0;
    int scanIndex=0;
    double grey=255.0;
    double gi = 255.0 / (double)irect.width();

    // draw a black and white sample
    for(iy=irect.top(); iy<=irect.bottom()/3; iy++)
    {
        px = (uint*)grayScaleImage->scanLine(scanIndex);
        for(ix=irect.left(); ix<=irect.right(); ix++)
        {
            if(ix<irect.width()/2)
                grey = 0;
            else
                grey = 255;
            QColor qcol((int)grey+0.5,(int)grey+0.5, (int)grey+0.5);
            *px++ = qcol.rgba();
        }
        scanIndex++;
    }

    // draw 256 shades of grey
    for(iy=irect.bottom()/3+1; iy<=irect.bottom()/3*2; iy++)
    {
        grey=255.0;
        px = (uint*)grayScaleImage->scanLine(scanIndex);
        for(ix=irect.left(); ix<=irect.right(); ix++)
        {
            QColor qcol((int)grey+0.5,(int)grey+0.5, (int)grey+0.5);
            *px++ = qcol.rgba();
            grey -= gi;
        }
        scanIndex++;
    }

    // draw 11 shades of grey
    for(iy=irect.bottom()/3*2+1; iy<=irect.bottom(); iy++)
    {
        px = (uint*)grayScaleImage->scanLine(scanIndex);
        for(ix=irect.left(); ix<=irect.right(); ix++)
        {
            grey = (ix/33) * (255.0/10.0);
             QColor qcol((int)grey,(int)grey, (int)grey);
            *px++ = qcol.rgba();
        }
        scanIndex++;
    }

    ui->_122->setPixmap(QPixmap::fromImage(*grayScaleImage));
}

void DefineColour_Dialog::initPaletteTab()
{
    QRect irect = ui->_123->rect();
    int ix,iy;
    uint *px=0;
    int scanIndex=0;
    int colourIndex=0;
    int tiles = 16;
    int tileSizeX = irect.width() / 16;
    int tileSizeY = irect.height() / 16;

    for(iy=irect.top(); iy<=irect.bottom(); iy++)
    {
        px = (uint*)curPaletteImage->scanLine(scanIndex);
        for(ix=irect.left(); ix<=irect.right(); ix++)
        {
            colourIndex = (iy/tileSizeY*16)+(ix/tileSizeX);
            Colour c = db.colourtable[colourIndex];
            int col = *px;
            QColor qcol(c.red, c.green, c.blue);
            *px++ = qcol.rgba();
        }
        scanIndex++;
    }

    ui->_123->setPixmap(QPixmap::fromImage(*curPaletteImage));
}

void DefineColour_Dialog::on__1_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    //QDialog::accept();
}

void DefineColour_Dialog::on__2_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDCANCEL,(WORD)0);
    lParam = (LPARAM)ui->_2;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::reject();
}
