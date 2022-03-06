
#include "ncwin.h"
#include "RCDialog.h"
#include "deletedrawing_dialog.h"
#include "ui_deletedrawing_dialog.h"

DeleteDrawing_Dialog::DeleteDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DeleteDrawing_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DeleteDrawing_Dialog::DeleteDrawing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DeleteDrawing_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  //connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on__101_clicked(bool)));
}

DeleteDrawing_Dialog::~DeleteDrawing_Dialog()
{
    delete ui;
}

void DeleteDrawing_Dialog::on__101_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void DeleteDrawing_Dialog::on__100_listItemDoubleClicked(QListWidgetItem * item)
{
    on__101_clicked(false);
}
