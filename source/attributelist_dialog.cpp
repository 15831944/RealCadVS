
#include "ncwin.h"
#include "RCDialog.h"
#include "attributelist_dialog.h"
#include "ui_attributelist_dialog.h"

AttributeList_Dialog::AttributeList_Dialog(HANDLE parent, HANDLE rcparent, int f):
	AttributeList_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
AttributeList_Dialog::AttributeList_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::AttributeList_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

AttributeList_Dialog::~AttributeList_Dialog()
{
    delete ui;
}

void AttributeList_Dialog::on__102_clicked(bool checked)
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

void AttributeList_Dialog::on__103_clicked(bool checked)
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

