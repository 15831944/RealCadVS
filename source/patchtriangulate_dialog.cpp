
#include "ncwin.h"
#include "RCDialog.h"
#include "patchtriangulate_dialog.h"
#include "ui_patchtriangulate_dialog.h"

PatchTriangulate_Dialog::PatchTriangulate_Dialog(HANDLE parent, HANDLE rcparent, int f):
	PatchTriangulate_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
PatchTriangulate_Dialog::PatchTriangulate_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::PatchTriangulate_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

PatchTriangulate_Dialog::~PatchTriangulate_Dialog()
{
    delete ui;
}

void PatchTriangulate_Dialog::on_accept()
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
