
#include "ncwin.h"

#include "RCDialog.h"
#include "lineparallel_dialog.h"
#include "ui_lineparallel_dialog.h"

LineParallel_Dialog::LineParallel_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LineParallel_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
LineParallel_Dialog::LineParallel_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QDialog(parent,f),
    RCDialog(rcparent,parent,f),
    ui(new Ui::LineParallel_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

LineParallel_Dialog::~LineParallel_Dialog()
{
  delete ui;
}

void LineParallel_Dialog::on__1103_actionTriggered(int action)
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

void LineParallel_Dialog::on__1104_actionTriggered(int action)
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

void LineParallel_Dialog::on__100_clicked(bool checked)
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

void LineParallel_Dialog::on__101_clicked(bool checked)
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

void LineParallel_Dialog::on__102_clicked(bool checked)
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

void LineParallel_Dialog::on__105_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 105
    wParam = MAKEWPARAM((WORD)105,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_105;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void LineParallel_Dialog::on__106_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 106
    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void LineParallel_Dialog::on__107_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 107
    wParam = MAKEWPARAM((WORD)107,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_107;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void LineParallel_Dialog::on_accept()
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

