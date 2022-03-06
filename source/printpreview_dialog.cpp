
#include "ncwin.h"

#include "RCDialog.h"
#include "printpreview_dialog.h"
#include "ui_PrintPreview_Dialog.h"

PrintPreview_Dialog::PrintPreview_Dialog(HANDLE parent, HANDLE rcparent, int f):
	PrintPreview_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
PrintPreview_Dialog::PrintPreview_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QDialog(parent,f),
    RCDialog(rcparent,parent,f),
    ui(new Ui::PrintPreview_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

//#if MAJOR_VERSION < 8
//  ui->_103->setSpecialValueText("Fit to Paper");
//#endif

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

PrintPreview_Dialog::~PrintPreview_Dialog()
{
  delete ui;
}

void PrintPreview_Dialog::on_accept()
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

void PrintPreview_Dialog::on__100_activated(int index)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)100,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);

    //msg = WM_COMMAND;

    //wParam = MAKEWPARAM((WORD)100,(WORD)CBN_SELCHANGE);
    //lParam = (LPARAM)ui->_100;
    //dialogcb((HWND)this,msg,wParam,lParam);
}

#if MAJOR_VERSION > 7
void PrintPreview_Dialog::on__104_activated(int index)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)104,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

}
#endif
