
#include "ncwin.h"
#include "RCDialog.h"
#include "networksoftwarelicense_dialog.h"
#include "ui_networksoftwarelicense_dialog.h"

NetworkSoftwareLicense_Dialog::NetworkSoftwareLicense_Dialog(HANDLE parent, HANDLE rcparent, int f):
	NetworkSoftwareLicense_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
NetworkSoftwareLicense_Dialog::NetworkSoftwareLicense_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::NetworkSoftwareLicense_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

}

NetworkSoftwareLicense_Dialog::~NetworkSoftwareLicense_Dialog()
{
    delete ui;
}

void NetworkSoftwareLicense_Dialog::on__100_clicked()
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

void NetworkSoftwareLicense_Dialog::on__101_clicked()
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

void NetworkSoftwareLicense_Dialog::on__102_clicked()
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
