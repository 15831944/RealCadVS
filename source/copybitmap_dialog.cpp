
#include "ncwin.h"
#include "RCDialog.h"
#include "copybitmap_dialog.h"
#include "ui_copybitmap_dialog.h"

CopyBitmap_Dialog::CopyBitmap_Dialog(HANDLE parent, HANDLE rcparent, int f):
	CopyBitmap_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
CopyBitmap_Dialog::CopyBitmap_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::CopyBitmap_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

CopyBitmap_Dialog::~CopyBitmap_Dialog()
{
    delete ui;
}

void CopyBitmap_Dialog::on_accept()
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
