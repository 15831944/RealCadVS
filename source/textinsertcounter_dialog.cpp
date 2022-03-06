
#include "ncwin.h"

#include "RCDialog.h"
#include "textinsertcounter_dialog.h"
#include "ui_textinsertcounter_dialog.h"

TextInsertCounter_Dialog::TextInsertCounter_Dialog(HANDLE parent, HANDLE rcparent, int f):
	TextInsertCounter_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
TextInsertCounter_Dialog::TextInsertCounter_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::TextInsertCounter_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

TextInsertCounter_Dialog::~TextInsertCounter_Dialog()
{
  delete ui;
}

void TextInsertCounter_Dialog::on_accept()
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
