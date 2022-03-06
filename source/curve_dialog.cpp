
#include "ncwin.h"
#include "RCDialog.h"
#include "curve_dialog.h"
#include "ui_CURVE_DIALOG.h"

Curve_Dialog::Curve_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Curve_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Curve_Dialog::Curve_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Curve_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Curve_Dialog::~Curve_Dialog()
{
    delete ui;
}

void Curve_Dialog::on__101_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Curve_Dialog::on_accept()
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
