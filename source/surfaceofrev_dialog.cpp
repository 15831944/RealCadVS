
#include "ncwin.h"
#include "RCDialog.h"
#include "surfaceofrev_dialog.h"
#include "ui_surfaceofrev_dialog.h"

SurfaceOfRev_Dialog::SurfaceOfRev_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SurfaceOfRev_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SurfaceOfRev_Dialog::SurfaceOfRev_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SurfaceOfRev_Dialog)
{
  ui->setupUi(this);

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_109)
      ui->_109->setScene(scene);
  ui->_109->setSceneRect(QRectF());
  ((SurfaceOfRevDisplayDialogControl*)RCdlg->getcontrol(109))->setQW(ui->_109);
  ((SurfaceOfRevDisplayDialogControl*)RCdlg->getcontrol(109))->sethwnd(ui->_109);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  drawSample();
}

SurfaceOfRev_Dialog::~SurfaceOfRev_Dialog()
{
    delete ui;
}

// colour
void SurfaceOfRev_Dialog::on__100_valueChanged(int v)
{
     drawSample();
}

// style
void SurfaceOfRev_Dialog::on__101_valueChanged(int v)
{
     drawSample();
}

// layer
void SurfaceOfRev_Dialog::on__102_valueChanged(int v)
{
    drawSample();
}

// weight
void SurfaceOfRev_Dialog::on__103_valueChanged(double v)
{
    drawSample();
}

// angle
void SurfaceOfRev_Dialog::on__104_valueChanged(double v)
{
     drawSample();
}

// delta z
void SurfaceOfRev_Dialog::on__105_valueChanged(double v)
{
     drawSample();
}

// X mesh
void SurfaceOfRev_Dialog::on__106_valueChanged(int v)
{
    drawSample();
}

// Y mesh
void SurfaceOfRev_Dialog::on__107_valueChanged(int v)
{
     drawSample();
}

// draw mode combo box
void SurfaceOfRev_Dialog::on__108_currentIndexChanged(int index)
{
     drawSample();
}

// surface properties
void SurfaceOfRev_Dialog::on__110_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 110
    wParam = MAKEWPARAM((WORD)110,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_110;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// triangulate
void SurfaceOfRev_Dialog::on__111_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 111
    wParam = MAKEWPARAM((WORD)111,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_111;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// clone
void SurfaceOfRev_Dialog::on__112_clicked(bool checked)
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

// clone all
void SurfaceOfRev_Dialog::on__113_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 113
    wParam = MAKEWPARAM((WORD)113,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_113;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// colour select button
void SurfaceOfRev_Dialog::on__1000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1000
    wParam = MAKEWPARAM((WORD)1000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1000;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// layer select button
void SurfaceOfRev_Dialog::on__1001_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1001
    wParam = MAKEWPARAM((WORD)1001,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1001;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// style select button
void SurfaceOfRev_Dialog::on__1002_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1002
    wParam = MAKEWPARAM((WORD)1002,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1002;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// weight select button
void SurfaceOfRev_Dialog::on__1003_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1003
    wParam = MAKEWPARAM((WORD)1003,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1003;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SurfaceOfRev_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}

void SurfaceOfRev_Dialog::drawSample()
{
    if((SurfaceOfRevDisplayDialogControl*)RCdlg->getcontrol(109));
        ((SurfaceOfRevDisplayDialogControl*)RCdlg->getcontrol(109))->draw(RCdlg,0);
}
