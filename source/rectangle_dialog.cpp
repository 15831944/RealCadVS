
#include "ncwin.h"

#include "RCDialog.h"
#include "rectangle_dialog.h"
#include "ui_rectangle_dialog.h"

Rectangle_Dialog::Rectangle_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Rectangle_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{
}
Rectangle_Dialog::Rectangle_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Rectangle_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  //ui->_103->installEventFilter(this);
}

Rectangle_Dialog::~Rectangle_Dialog()
{
    delete ui;
}

/*
 * // use this to change focus of a control etc.
bool Rectangle_Dialog::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::FocusOut && ((QFocusEvent*)event)->lostFocus())
    {
        if(obj->objectName() == "_103")
        {
            ((RealDialogControl*)rcDialog()->getcontrol(103))->changefocus(rcDialog());
        }
    }
    return false;
}
*/

void Rectangle_Dialog::on__1103_actionTriggered(int action)
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

void Rectangle_Dialog::on__1104_actionTriggered(int action)
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

void Rectangle_Dialog::on__100_clicked(bool checked)
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

void Rectangle_Dialog::on__101_clicked(bool checked)
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

void Rectangle_Dialog::on__102_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 102
    wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Rectangle_Dialog::on__105_clicked(bool checked)
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

void Rectangle_Dialog::on__106_clicked(bool checked)
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

void Rectangle_Dialog::on_accept()
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
