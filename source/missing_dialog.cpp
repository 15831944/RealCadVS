
#include "ncwin.h"
#include "RCDialog.h"
#include "missing_dialog.h"
#include "ui_missing_dialog.h"

Missing_Dialog::Missing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Missing_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Missing_Dialog::Missing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Missing_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

Missing_Dialog::~Missing_Dialog()
{
    delete ui;
}

// delete
void Missing_Dialog::on__101_clicked(bool checked)
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

// show as text file
void Missing_Dialog::on__102_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 102
    wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);
}
