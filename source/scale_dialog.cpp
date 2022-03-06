
#include "ncwin.h"
#include "RCDialog.h"
#include "scale_dialog.h"
#include "ui_scale_dialog.h"

Scale_Dialog::Scale_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Scale_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Scale_Dialog::Scale_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Scale_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Scale_Dialog::~Scale_Dialog()
{
    delete ui;
}

void Scale_Dialog::on__100_clicked(bool checked)
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

void Scale_Dialog::on__101_clicked(bool checked)
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

void Scale_Dialog::on__107_clicked(bool checked)
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

void Scale_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 108
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Scale_Dialog::on__109_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 109
    wParam = MAKEWPARAM((WORD)109,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_109;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Scale_Dialog::on_accept()
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
