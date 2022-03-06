
#include "ncwin.h"

#include "RCDialog.h"
#include "rccombolist.h"
#include "createworkplane_dialog.h"
#include "ui_createworkplane_dialog.h"

CreateWorkplane_Dialog::CreateWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f):
	CreateWorkplane_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
CreateWorkplane_Dialog::CreateWorkplane_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::CreateWorkplane_Dialog)
{
  ui->setupUi(this);
  ui->_101->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_101,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

CreateWorkplane_Dialog::~CreateWorkplane_Dialog()
{
    delete ui;
}

void CreateWorkplane_Dialog::on_accept()
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
