#include "ncwin.h"
#include "RCDialog.h"
#include "patch_dialog.h"
#include "ui_patch_dialog.h"

Patch_Dialog::Patch_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Patch_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Patch_Dialog::Patch_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Patch_Dialog)
{
  ui->setupUi(this);

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_109)
      ui->_109->setScene(scene);
  ui->_109->setSceneRect(QRectF());
  ((PatchDisplayDialogControl*)RCdlg->getcontrol(109))->setQW(ui->_109);
  ((PatchDisplayDialogControl*)RCdlg->getcontrol(109))->sethwnd(ui->_109);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  drawSample();
}

Patch_Dialog::~Patch_Dialog()
{
    delete ui;
}

// colour
void Patch_Dialog::on__100_valueChanged(int v)
{
    drawSample();
}

// style
void Patch_Dialog::on__101_valueChanged(int v)
{
    drawSample();
}

// layer
void Patch_Dialog::on__102_valueChanged(int v)
{
    drawSample();
}

// weight
void Patch_Dialog::on__103_valueChanged(double v)
{
    drawSample();
}

// x degree
void Patch_Dialog::on__104_valueChanged(double v)
{
    drawSample();
}

// y degree
void Patch_Dialog::on__105_valueChanged(double v)
{
    drawSample();
}

// X mesh
void Patch_Dialog::on__106_valueChanged(int v)
{
    drawSample();
}

// Y mesh
void Patch_Dialog::on__107_valueChanged(int v)
{
    drawSample();
}

// edge mode combo box
void Patch_Dialog::on__108_currentIndexChanged(int index)
{
    drawSample();
}

// surface properties
void Patch_Dialog::on__110_clicked(bool checked)
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

// compare
void Patch_Dialog::on__111_clicked(bool checked)
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

// triagulate
void Patch_Dialog::on__112_clicked(bool checked)
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

// clone all (106->113)
void Patch_Dialog::on__113_clicked(bool checked)
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

// clone (105->114)
void Patch_Dialog::on__114_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 114
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// colour select button
void Patch_Dialog::on__1000_clicked(bool checked)
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
void Patch_Dialog::on__1001_clicked(bool checked)
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
void Patch_Dialog::on__1002_clicked(bool checked)
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
void Patch_Dialog::on__1003_clicked(bool checked)
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

void Patch_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}

void Patch_Dialog::drawSample()
{
    if((PatchDisplayDialogControl*)RCdlg->getcontrol(109));
        ((PatchDisplayDialogControl*)RCdlg->getcontrol(109))->draw(RCdlg,0);
}
