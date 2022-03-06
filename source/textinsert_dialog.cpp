
#include "ncwin.h"
#include "RCDialog.h"
#include "textinsert_dialog.h"
#include "ui_textinsert_dialog.h"

TextInsert_Dialog::TextInsert_Dialog(HANDLE parent, HANDLE rcparent, int f):
	TextInsert_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
TextInsert_Dialog::TextInsert_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::TextInsert_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  if(!v.getinteger("op::textangle"))
  {
      ui->_102->setDisabled(true);
      ui->label_102->setDisabled(true);
  }

  ui->_100->selectAll();

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

TextInsert_Dialog::~TextInsert_Dialog()
{
  delete ui;
}

void TextInsert_Dialog::on__103_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 103
    wParam = MAKEWPARAM((WORD)103,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_103;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void TextInsert_Dialog::on_accept()
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
