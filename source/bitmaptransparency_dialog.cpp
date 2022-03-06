
#include "ncwin.h"
#include "Ncdialog.h"

#include "RCDialog.h"
#include "bitmaptransparency_dialog.h"
#include "ui_bitmaptransparency_dialog.h"

bool BitmapPickFilter::eventFilter(QObject *watched, QEvent *event)
{
    qDebug() << "mouse evnet type : " << event->type();
    if(event->type() == QEvent::MouseMove ||
       event->type() == QEvent::MouseButtonPress ||
       event->type() == QEvent::MouseButtonRelease)
    {
        if(watched->objectName() == "_200")
        {
            QWidget *dialog = app->activeModalWidget();
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                QPointF pos = ((QMouseEvent*)event)->pos();
                // this code is used when we use a graphics view for the picking preview
                // currently we ae using a QLabel for this
#if 1
                //QPointF scenePos = ((BitmapTransparency_Dialog*)dialog)->ui->_200->mapToScene(pos);
                //QPointF itemPos = ((BitmapTransparency_Dialog*)dialog)->ui->_200->itemAt(pos)->mapFromScene(scenePos);
                //QColor pcol = ((BitmapTransparency_Dialog*)dialog)->bitmap.pixelColor((int)itempos.x(),(int)itempos.y());
                QRectF labelRect(((BitmapTransparency_Dialog*)dialog)->ui->_200->rect());
                //QRectF pixmapRect(((BitmapTransparency_Dialog*)dialog)->ui->_200->pixmap()->rect());
                QRectF pixmapRect(((BitmapTransparency_Dialog*)dialog)->bitmap.rect());
                //QPolygonF labelPoly(labelRect);
                //QPolygonF pixmapPoly(pixmapRect);
                QTransform transf;
                transf.scale(pixmapRect.width()/labelRect.width(),pixmapRect.height()/labelRect.height());
                //QTransform::quadToQuad(labelPoly,pixmapPoly,transf);
                QPointF imagePos = transf.map(pos);
                //QImage image(((BitmapTransparency_Dialog*)dialog)->ui->_200->pixmap()->toImage());
                //image.save("pixmaptest.jpg","JPG");
                //QColor pcol = image.pixelColor((int)imagePos.x(),(int)imagePos.y());
                QColor pcol = ((BitmapTransparency_Dialog*)dialog)->bitmap.pixelColor((int)imagePos.x(),(int)imagePos.y());
#else
                QColor pcol = ((BitmapTransparency_Dialog*)dialog)->bitmap.pixelColor((int)pos.x(),(int)pos.y());
#endif
                ((BitmapTransparency_Dialog*)dialog)->ui->_101->setBackgroundBrush(QBrush(pcol));
                Colour rccol(pcol.redF(),pcol.greenF(),pcol.blueF());
                ((ColourSampleControl*)((BitmapTransparency_Dialog*)dialog)->RCdlg->getcontrol(101))->change(rccol);
                if(event->type() == QEvent::MouseButtonPress)
                    ((BitmapTransparency_Dialog*)dialog)->drawSample();
                else if(event->type() == QEvent::MouseMove)
                    ((BitmapTransparency_Dialog*)dialog)->drawSample(2);
            }
            else if(event->type() == QEvent::MouseButtonRelease)
                ((BitmapTransparency_Dialog*)dialog)->drawSample();

            return true;
        }
    }
    return QObject::eventFilter(watched,event);
}

BitmapTransparency_Dialog::BitmapTransparency_Dialog(HANDLE parent, HANDLE rcparent, int f):
	BitmapTransparency_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
BitmapTransparency_Dialog::BitmapTransparency_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent, parent, f),
    ui(new Ui::BitmapTransparency_Dialog)
{
    ui->setupUi(this);

    // changes

    if(ui->_100)
        ui->_100->setScene(0);
    ((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->setQW(0);
    ((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->sethwnd(0);

    if(ui->_101)
        ui->_101->setScene(0);
    ((ColourSampleControl*)RCdlg->getcontrol(101))->setQW(0);
    ((ColourSampleControl*)RCdlg->getcontrol(101))->sethwnd(0);

    //((RealDialogControl*)RCdlg->getcontrol(102))->setQW(ui->_102);
    ui->_102->setValue(ui->_102->maximum());

    initDialog(this);

    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_100)
        ui->_100->setScene(scene);
    ui->_100->setSceneRect(ui->_100->rect());
    ((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->setQW(ui->_100);
    ((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->sethwnd(ui->_100);

    scene = new QGraphicsScene;
    if(ui->_101)
        ui->_101->setScene(scene);
    ui->_101->setSceneRect(ui->_101->rect());
    ((ColourSampleControl*)RCdlg->getcontrol(101))->setQW(ui->_101);
    ((ColourSampleControl*)RCdlg->getcontrol(101))->sethwnd(ui->_101);

    // hide the picker shape checkbox for now
    ui->_105->hide();

    // make the bitmap
    RCCHAR bitmapPath[600];
    strcpy(bitmapPath,((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->bitmapPath);
    QString filename(bitmapPath);
    QFileInfo fileinfo(filename);
    QString suffix = fileinfo.suffix().toUpper();
    bitmap = QImage(filename);
    bitmap = bitmap.scaled(ui->_200->height(),ui->_200->height());
    QPixmap pixmap = QPixmap::fromImage(bitmap);
    //qDebug() << "pick sample pixmap : " << pixmap;
    ui->_200->setPixmap(pixmap);

    BitmapPickFilter *eventFilter = new BitmapPickFilter(this);
    ui->_200->installEventFilter(eventFilter);
    //
    sliderIsReleased=true;
    drawSample();
}
/*
bool BitmapTransparency_Dialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseMove ||
       event->type() == QEvent::MouseButtonPress)
    {
        if(watched->objectName() == "_200")
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
            {
                //qDebug() << "mouse is down";
                QPoint pos = ((QMouseEvent*)event)->pos();
                QPointF scenePos = ui->_200->mapToScene(pos);
                QPointF itemPos = ui->_200->itemAt(pos)->mapFromScene(scenePos);
                QColor pcol = bitmap.pixelColor((int)itemPos.x(),(int)itemPos.y());
                ui->_101->setBackgroundBrush(QBrush(pcol));
                Colour rccol(pcol.redF(),pcol.greenF(),pcol.blueF());
                ((ColourSampleControl*)RCdlg->getcontrol(101))->change(rccol);
                drawSample();
                return true;
            }
        }
    }
    return QObject::eventFilter(watched,event);
}
*/
BitmapTransparency_Dialog::~BitmapTransparency_Dialog()
{
    QGraphicsScene *scene = 0;
    if(ui->_100)
    {
        scene = ui->_100->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_100->setScene(0);
        }
    }
    if(ui->_101)
    {
        scene = ui->_101->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_101->setScene(0);
        }
    }
    delete ui;
}

void BitmapTransparency_Dialog::on__1_clicked()
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

void BitmapTransparency_Dialog::on__2_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click CANCEL
    wParam = MAKEWPARAM((WORD)IDCANCEL,(WORD)0);
    lParam = (LPARAM)ui->_2;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::reject();
}

void BitmapTransparency_Dialog::on__102_valueChanged(double v)
{
    ((RealDialogControl*)this->RCdlg->getcontrol(102))->change(RCdlg,v);
    ui->_103->setValue(v*100.0);
    if(sliderIsReleased)
        drawSample(1);
}

void BitmapTransparency_Dialog::on__103_valueChanged(int value)
{
    ui->_102->setValue(value/100.0);
}

void BitmapTransparency_Dialog::on__103_sliderPressed()
{
      sliderIsReleased = false;
}

void BitmapTransparency_Dialog::on__103_sliderReleased()
{
      sliderIsReleased = true;
      drawSample(1);
}

void BitmapTransparency_Dialog::drawSample(int flags)
{
    if(flags & 1)
    {
        if(((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100)))
            ((BitMapETransparencyRangeDisplayControl*)RCdlg->getcontrol(100))->draw(RCdlg,0);
    }
    if(flags & 2)
    {
        if(((ColourSampleControl*)RCdlg->getcontrol(101)))
            ((ColourSampleControl*)RCdlg->getcontrol(101))->draw(RCdlg,0);
    }
}

void BitmapTransparency_Dialog::resizeEvent(QResizeEvent *ev)
{
    drawSample(1);
    QDialog::resizeEvent(ev);
}

void BitmapTransparency_Dialog::on__1011_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1011
    wParam = MAKEWPARAM((WORD)1011,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1011;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample(1);
}

void BitmapTransparency_Dialog::on__1012_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1012
    wParam = MAKEWPARAM((WORD)1012,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1012;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample(1);
}

void BitmapTransparency_Dialog::on__1013_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1013
    wParam = MAKEWPARAM((WORD)1013,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1013;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample(1);
}

void BitmapTransparency_Dialog::on__104_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample(1);
}

void BitmapTransparency_Dialog::on__105_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 105
    wParam = MAKEWPARAM((WORD)105,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_105;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample(1);
}
