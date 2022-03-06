
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "deleteview_dialog.h"
#include "ui_deleteview_dialog.h"

DeleteView_Dialog::DeleteView_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DeleteView_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
DeleteView_Dialog::DeleteView_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DeleteView_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustSize();
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on__101_clicked()));
}

DeleteView_Dialog::~DeleteView_Dialog()
{
    delete ui;
}

void DeleteView_Dialog::on__101_clicked(bool checked)
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

    // QDialog::accept();
}

void DeleteView_Dialog::on_accept()
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

    QDialog::accept();
}
