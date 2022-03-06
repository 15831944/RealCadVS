
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "selectview_dialog.h"
#include "ui_selectview_dialog.h"

SelectView_Dialog::SelectView_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectView_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
SelectView_Dialog::SelectView_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectView_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ui->_101->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
  connect(ui->_101,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

SelectView_Dialog::~SelectView_Dialog()
{
    delete ui;
}

void SelectView_Dialog::on__100_currentRowChanged(int currentRow)
{
    // using the dialog callback function
    RCdlg->getcontrol(100)->changefocus(RCdlg);
    /*
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)101,(WORD)CBN_SELCHANGE);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SelectView_Dialog::on__101_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
        v.setstring("op::lastzonename",ui->_101->item(currentRow)->text().data());
}

void SelectView_Dialog::on_accept()
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
