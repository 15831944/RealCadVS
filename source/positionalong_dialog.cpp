
#include "ncwin.h"

#include "RCDialog.h"
#include "positionalong_dialog.h"
#include "ui_positionalong_dialog.h"

PositionAlong_Dialog::PositionAlong_Dialog(HANDLE parent, HANDLE rcparent, int f):
	PositionAlong_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
PositionAlong_Dialog::PositionAlong_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::PositionAlong_Dialog)
{
  ui->setupUi(this);

  initDialog(this);
  ActivateParent = false;
  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

PositionAlong_Dialog::~PositionAlong_Dialog()
{
    delete ui;
}

void PositionAlong_Dialog::on__200_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 200
    wParam = MAKEWPARAM((WORD)200,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_200;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void PositionAlong_Dialog::on__201_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 201
    wParam = MAKEWPARAM((WORD)201,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_201;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void PositionAlong_Dialog::on__202_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 202
    wParam = MAKEWPARAM((WORD)202,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_202;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void PositionAlong_Dialog::on_accept()
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
