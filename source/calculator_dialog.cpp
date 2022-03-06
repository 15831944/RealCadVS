
#include "ncwin.h"

#include "RCDialog.h"
#include "calculator_dialog.h"
#include "ui_calculator_dialog.h"

Calculator_Dialog::Calculator_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Calculator_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Calculator_Dialog::Calculator_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Calculator_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  // connect the calculator keys to the slot
  for (int i = 101 ; i < 140 ; i++)
  {
      QPushButton *key = findChild<QPushButton *>(RCintToName(i));
      if(key)
          connect(key,SIGNAL(pressed()),this,SLOT(on_key_pressed()));
  }

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Calculator_Dialog::~Calculator_Dialog()
{
    delete ui;
}

void Calculator_Dialog::on__100_editingFinished()
{
    // send a kill focus message to the display control
    // to update any expressions
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)100,(WORD)EN_KILLFOCUS);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Calculator_Dialog::on_key_pressed()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click buttons 101 ~ 139
    WORD id = (WORD)RCnameToInt(sender()->objectName());
    wParam = MAKEWPARAM((WORD)id,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Calculator_Dialog::on_accept()
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

