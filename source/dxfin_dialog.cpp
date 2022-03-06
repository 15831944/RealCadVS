
#include "ncwin.h"
#include "RCDialog.h"
#include "dxfin_dialog.h"
#include "ui_dxfin_dialog.h"

DXFIn_Dialog::DXFIn_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DXFIn_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DXFIn_Dialog::DXFIn_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QDialog(parent,f),
    RCDialog(rcparent,parent,f),
    ui(new Ui::DXFIn_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

DXFIn_Dialog::~DXFIn_Dialog()
{
    delete ui;
}

void DXFIn_Dialog::on_accept()
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
