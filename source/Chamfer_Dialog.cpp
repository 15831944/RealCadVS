
#include "ncwin.h"
#include "RCDialog.h"
#include "chamfer_dialog.h"
#include "ui_chamfer_dialog.h"

Chamfer_Dialog::Chamfer_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Chamfer_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Chamfer_Dialog::Chamfer_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QDialog(parent,f),
    RCDialog(rcparent,parent,f),
    ui(new Ui::Chamfer_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Chamfer_Dialog::~Chamfer_Dialog()
{
    delete ui;
}

void Chamfer_Dialog::on__1102_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_102;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_102;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Chamfer_Dialog::on__1103_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_103;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_103;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Chamfer_Dialog::on__1104_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_104;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_104;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Chamfer_Dialog::on__100_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 100
    wParam = MAKEWPARAM((WORD)100,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Chamfer_Dialog::on__101_clicked(bool checked)
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

void Chamfer_Dialog::on_accept()
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


