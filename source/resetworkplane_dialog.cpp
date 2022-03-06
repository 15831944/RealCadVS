
#include "ncwin.h"

#include "RCDialog.h"
#include "rccombolist.h"
#include "resetworkplane_dialog.h"
#include "ui_resetworkplane_dialog.h"

ResetWorkplane_Dialog::ResetWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f):
	ResetWorkplane_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
ResetWorkplane_Dialog::ResetWorkplane_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::ResetWorkplane_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustSize();
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

ResetWorkplane_Dialog::~ResetWorkplane_Dialog()
{
    delete ui;
}

void ResetWorkplane_Dialog::on_accept()
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

