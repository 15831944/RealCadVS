
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "deletezone_dialog.h"
#include "ui_deletezone_dialog.h"

DeleteZone_Dialog::DeleteZone_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DeleteZone_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
DeleteZone_Dialog::DeleteZone_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DeleteZone_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

DeleteZone_Dialog::~DeleteZone_Dialog()
{
    delete ui;
}

void DeleteZone_Dialog::on_accept()
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
