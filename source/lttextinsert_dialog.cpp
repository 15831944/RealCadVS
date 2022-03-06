
#include "ncwin.h"

#include "RCDialog.h"
#include "LTTextInsert_Dialog.h"
#include "ui_LTTextInsert_Dialog.h"

LTTextInsert_Dialog::LTTextInsert_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LTTextInsert_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
LTTextInsert_Dialog::LTTextInsert_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::LTTextInsert_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  ui->_100->selectAll();

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

LTTextInsert_Dialog::~LTTextInsert_Dialog()
{
  delete ui;
}

void LTTextInsert_Dialog::on_accept()
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
