
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "selectdrawing_dialog.h"
#include "ui_selectdrawing_dialog.h"

SelectDrawing_Dialog::SelectDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectDrawing_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SelectDrawing_Dialog::SelectDrawing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectDrawing_Dialog)
{
  ui->setupUi(this);
  ui->_101->adjustListSize();

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
  connect(ui->_101,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));

}

SelectDrawing_Dialog::~SelectDrawing_Dialog()
{
    delete ui;
}

void SelectDrawing_Dialog::on__100_currentRowChanged(int currentRow)
{
    RCdlg->getcontrol(100)->changefocus(RCdlg);

}

void SelectDrawing_Dialog::on__100_itemPressed(QListWidgetItem * item)
{
    RCdlg->getcontrol(100)->changefocus(RCdlg);
}

void SelectDrawing_Dialog::on__100_itemDoubleClicked(QListWidgetItem * item)
{
    on_accept();
}

void SelectDrawing_Dialog::on__101_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
        v.setstring("op::lastzonename",ui->_101->item(currentRow)->text().data());

}

void SelectDrawing_Dialog::on_accept()
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
