
#include "ncwin.h"
#include "RCDialog.h"
#include "group_dialog.h"
#include "ui_group_dialog.h"

Group_Dialog::Group_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Group_Dialog((QWidget*) parent, (Dialog*) rcparent, (Qt::WindowFlags) f)
{}
Group_Dialog::Group_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Group_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Group_Dialog::~Group_Dialog()
{
    delete ui;
}

void Group_Dialog::on_accept()
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
