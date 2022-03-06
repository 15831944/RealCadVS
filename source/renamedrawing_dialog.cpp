
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "renamedrawing_dialog.h"
#include "ui_renamedrawing_dialog.h"

RenameDrawing_Dialog::RenameDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	RenameDrawing_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
RenameDrawing_Dialog::RenameDrawing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::RenameDrawing_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

RenameDrawing_Dialog::~RenameDrawing_Dialog()
{
    delete ui;
}

void RenameDrawing_Dialog::on_accept()
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
