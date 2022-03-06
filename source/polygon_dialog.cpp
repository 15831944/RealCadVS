
#include "ncwin.h"

#include "RCDialog.h"
#include "polygon_dialog.h"
#include "ui_polygon_dialog.h"

Polygon_Dialog::Polygon_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Polygon_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Polygon_Dialog::Polygon_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    //QDialog(parent,f),
    RCDialog(rcparent,parent,f),
    ui(new Ui::Polygon_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Polygon_Dialog::~Polygon_Dialog()
{
    delete ui;
}

void Polygon_Dialog::on__1100_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_100;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_100;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Polygon_Dialog::on__1101_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_101;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_101;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Polygon_Dialog::on__1105_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_105;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_105;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Polygon_Dialog::on__1106_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_106;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_106;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void Polygon_Dialog::on__102_clicked(bool checked)
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

void Polygon_Dialog::on__103_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)103,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_103;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Polygon_Dialog::on__104_clicked(bool checked)
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

void Polygon_Dialog::on__107_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)107,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_107;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Polygon_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Polygon_Dialog::on_accept()
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


