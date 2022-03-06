
#include "ncwin.h"
#include "RCDialog.h"
#include "shade_dialog.h"
#include "ui_shade_dialog.h"

Shade_Dialog::Shade_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Shade_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Shade_Dialog::Shade_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Shade_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Shade_Dialog::~Shade_Dialog()
{
    delete ui;
}

void Shade_Dialog::on_accept()
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
