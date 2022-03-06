
#include "ncwin.h"

#include "RCDialog.h"
#include "dimensiontext_dialog.h"
#include "ui_dimensiontext_dialog.h"

DimensionText_Dialog::DimensionText_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DimensionText_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
DimensionText_Dialog::DimensionText_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DimensionText_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

DimensionText_Dialog::~DimensionText_Dialog()
{
  delete ui;
}


void DimensionText_Dialog::on_accept()
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
