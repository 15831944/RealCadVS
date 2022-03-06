
#include "ncwin.h"
#include "RCDialog.h"
#include "trim_dialog.h"
#include "ui_trim_dialog.h"


Trim_Dialog::Trim_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Trim_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Trim_Dialog::Trim_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Trim_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Trim_Dialog::~Trim_Dialog()
{
    delete ui;
}

void Trim_Dialog::on_accept()
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

