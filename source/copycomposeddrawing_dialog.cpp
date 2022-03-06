
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "copycomposeddrawing_dialog.h"
#include "ui_copycomposeddrawing_dialog.h"

CopyComposedDrawing_Dialog::
CopyComposedDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	CopyComposedDrawing_Dialog((QWidget *)parent, (Dialog *)rcparent,( Qt::WindowFlags) f)
{}
CopyComposedDrawing_Dialog::CopyComposedDrawing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::CopyComposedDrawing_Dialog)
{
  ui->setupUi(this);
  ui->_101->adjustSize();
  ui->_101->adjustListSize();

  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  //connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
}

CopyComposedDrawing_Dialog::~CopyComposedDrawing_Dialog()
{
    delete ui;
}

void CopyComposedDrawing_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;

    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}
