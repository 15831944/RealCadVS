
#include "ncwin.h"
#include "RCDialog.h"
#include "drawingverify_dialog.h"
#include "ui_drawingverify_dialog.h"

DrawingVerify_Dialog::DrawingVerify_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DrawingVerify_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DrawingVerify_Dialog::DrawingVerify_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DrawingVerify_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

DrawingVerify_Dialog::~DrawingVerify_Dialog()
{
    delete ui;
}


void DrawingVerify_Dialog::on__104_clicked(bool checked)
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
}

void DrawingVerify_Dialog::on_accept()
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
