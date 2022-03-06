
#include "ncwin.h"
#include "RCDialog.h"
#include "patchcorners_dialog.h"
#include "ui_patchcorners_dialog.h"

PatchCorners_Dialog::PatchCorners_Dialog(HANDLE parent, HANDLE rcparent, int f):
	PatchCorners_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
PatchCorners_Dialog::PatchCorners_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::PatchCorners_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

PatchCorners_Dialog::~PatchCorners_Dialog()
{
    delete ui;
}

void PatchCorners_Dialog::on_accept()
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
