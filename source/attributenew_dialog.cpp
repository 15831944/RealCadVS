
#include "ncwin.h"
#include "RCDialog.h"
#include "attributenew_dialog.h"
#include "ui_attributenew_dialog.h"

AttributeNew_Dialog::AttributeNew_Dialog(HANDLE parent, HANDLE rcparent, int f):
	AttributeNew_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
AttributeNew_Dialog::AttributeNew_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::AttributeNew_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

AttributeNew_Dialog::~AttributeNew_Dialog()
{
    delete ui;
}

void AttributeNew_Dialog::on_accept()
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
