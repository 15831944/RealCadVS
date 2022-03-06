
#include "ncwin.h"

#include "RCDialog.h"
#include "rccombolist.h"
#include "deleteworkplane_dialog.h"
#include "ui_deleteworkplane_dialog.h"

DeleteWorkplane_Dialog::DeleteWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DeleteWorkplane_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
DeleteWorkplane_Dialog::DeleteWorkplane_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DeleteWorkplane_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

DeleteWorkplane_Dialog::~DeleteWorkplane_Dialog()
{
    delete ui;
}

void DeleteWorkplane_Dialog::on_accept()
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
