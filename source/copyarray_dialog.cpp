
#include "ncwin.h"
#include "RCDialog.h"
#include "copyarray_dialog.h"
#include "ui_copyarray_dialog.h"

CopyArray_Dialog::CopyArray_Dialog(HANDLE parent, HANDLE rcparent, int f):
	CopyArray_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
CopyArray_Dialog::CopyArray_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::CopyArray_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

CopyArray_Dialog::~CopyArray_Dialog()
{
    delete ui;
}

void CopyArray_Dialog::on__100_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 100
    wParam = MAKEWPARAM((WORD)100,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void CopyArray_Dialog::on__101_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void CopyArray_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}
