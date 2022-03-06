
#include "ncwin.h"

#include <QListWidgetItem>
#include "RCDialog.h"
#include "rccombolist.h"
#include "selectworkplane_dialog.h"
#include "ui_selectworkplane_dialog.h"

SelectWorkplane_Dialog::SelectWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectWorkplane_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
SelectWorkplane_Dialog::SelectWorkplane_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectWorkplane_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
  //connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(listDoubleClicked(QListWidgetItem*)));
}

SelectWorkplane_Dialog::~SelectWorkplane_Dialog()
{
    delete ui;
}

void SelectWorkplane_Dialog::on__101_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SelectWorkplane_Dialog::on__102_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SelectWorkplane_Dialog::on__103_clicked(bool checked)
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

void SelectWorkplane_Dialog::on__104_clicked(bool checked)
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
}

void SelectWorkplane_Dialog::on_accept()
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

void SelectWorkplane_Dialog::listDoubleClicked(QListWidgetItem *)
{
    //qDebug() << "doubleClicked";
    on_accept();
}
