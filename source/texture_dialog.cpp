
#include "ncwin.h"
#include "RCDialog.h"
#include "texture_dialog.h"
#include "ui_TEXTURE_DIALOG.h"

Texture_Dialog::Texture_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Texture_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Texture_Dialog::Texture_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Texture_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    // give the graphics view pointer to the dialog control
    if(rcparent != 0)
        rcparent->getcontrol(118)->setQW(ui->_118);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

void Texture_Dialog::showEvent(QShowEvent *event)
{
    drawSample();
}

void Texture_Dialog::resizeEvent(QResizeEvent *event)
{
    drawSample();
}

Texture_Dialog::~Texture_Dialog()
{
    delete ui;
}

void Texture_Dialog::on_accept()
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

    QDialog::accept();
}

void Texture_Dialog::on__100_currentIndexChanged(int index)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 100
    //wParam = CBN_SELCHANGE;
    wParam = MAKEWPARAM((WORD)100,(WORD)CBN_SELCHANGE);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Texture_Dialog::on__112_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 112
    wParam = MAKEWPARAM((WORD)112,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_112;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Texture_Dialog::on__117_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 117
    wParam = MAKEWPARAM((WORD)117,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_117;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Texture_Dialog::on__107_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 107
    wParam = MAKEWPARAM((WORD)107,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_107;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Texture_Dialog::on__119_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 119
    wParam = MAKEWPARAM((WORD)119,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_119;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Texture_Dialog::drawSample()
{
    if((TextureDisplayDialogControl*)RCdlg->getcontrol(118));
        ((TextureDisplayDialogControl*)RCdlg->getcontrol(118))->draw(RCdlg,0);
}

// alternative function for drawing the texture sample
void Texture_Dialog::PreviewImage(QWidget *widget, RCCHAR *filepath )
{
    if(qobject_cast<QGraphicsView *>(widget) != 0)
    {
        QGraphicsScene *scene=0;
        QString qfilepath(filepath);
        QPixmap image(qfilepath);
        if(!image.isNull())
        {
            if(((QGraphicsView*)widget)->scene() == 0)
            {
                scene = new QGraphicsScene();
                scene->setSceneRect(image.rect());
                ((QGraphicsView*)widget)->setScene(scene);
                //((QGraphicsView*)widget)->setSceneRect(QRectF());
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }
            else
            {
                scene = ((QGraphicsView*)widget)->scene();
                scene->clear();
                //scene->setSceneRect(QRectF());
                scene->setSceneRect(image.rect());
                qDebug() << "scene Rect"<< scene->sceneRect();
                //((QGraphicsView*)widget)->setSceneRect(QRectF());
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }
            qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
            qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
            qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
            qDebug() << "scene Rect"<< scene->sceneRect();

            QGraphicsPixmapItem *gitem = ((QGraphicsView*)widget)->scene()->addPixmap(image);
            scene->setSceneRect(gitem->boundingRect());
            qDebug() << scene->sceneRect();
            //QRectF srect = scene->temsBoundingRect();
            QRectF srect = scene->sceneRect();
            //QRectF srect = image.rect();
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
            //scene->setSceneRect(nrect);
            //nrect.translate(srect.center()-nrect.center());
            //QGraphicsRectItem *ritem = scene->addRect(nrect.left(),nrect.top(),nrect.width(),nrect.height(),QPen(QColor(0,0,0)),Qt::NoBrush);
            //nrect = scene->itemsBoundingRect();
            //scene->setSceneRect(ritem->boundingRect());
            scene->setSceneRect(nrect);
            nrect = scene->sceneRect();
            //gitem->moveBy(nrect.center().x()-srect.center().x(),nrect.center().y()-srect.center().y());
            // fit it to the window and center it
            //((QGraphicsView*)widget)->setSceneRect(QRect());
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

