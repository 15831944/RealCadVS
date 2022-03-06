
#include "ncwin.h"

#include "RCDialog.h"
#include "ellipseinsert_dialog.h"
#include "ui_ellipseinsert_dialog.h"

EllipseInsert_Dialog::EllipseInsert_Dialog(HANDLE parent, HANDLE rcparent, int f):
	EllipseInsert_Dialog((QWidget*)parent, (Dialog*) rcparent, (Qt::WindowFlags) f)
{}
EllipseInsert_Dialog::EllipseInsert_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::EllipseInsert_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

EllipseInsert_Dialog::~EllipseInsert_Dialog()
{
  delete ui;
}

void EllipseInsert_Dialog::on_accept()
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
