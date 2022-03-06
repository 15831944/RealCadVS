
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "zoomzone_dialog.h"
#include "ui_zoomzone_dialog.h"

ZoomZone_Dialog::ZoomZone_Dialog(HANDLE parent, HANDLE rcparent, int f):
	ZoomZone_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
ZoomZone_Dialog::ZoomZone_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::ZoomZone_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

ZoomZone_Dialog::~ZoomZone_Dialog()
{
    delete ui;
}

void ZoomZone_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}
