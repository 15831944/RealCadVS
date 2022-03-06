
#include "ncwin.h"
#include "RCDialog.h"
#include "attributedelete_dialog.h"
#include "ui_ATTRIBUTEDELETE_DIALOG.h"

AttributeDelete_Dialog::AttributeDelete_Dialog(HANDLE parent, HANDLE rcparent, int f):
	AttributeDelete_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
AttributeDelete_Dialog::AttributeDelete_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::AttributeDelete_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

AttributeDelete_Dialog::~AttributeDelete_Dialog()
{
    delete ui;
}

void AttributeDelete_Dialog::on__102_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 102
    wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);

    // QDialog::accept();
}

void AttributeDelete_Dialog::on__103_clicked(bool checked)
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

    // QDialog::accept();
}

void AttributeDelete_Dialog::on__104_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

    // QDialog::accept();
}
