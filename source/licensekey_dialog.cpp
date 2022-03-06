
#include "ncwin.h"
#include "RCDialog.h"
#include "licensekey_dialog.h"
#include "ui_licensekey_dialog.h"

LicenseKey_Dialog::LicenseKey_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LicenseKey_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
LicenseKey_Dialog::LicenseKey_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f):
       RCDialog(rcparent,parent,f),
       ui(new Ui::LicenseKey_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

}

LicenseKey_Dialog::~LicenseKey_Dialog()
{
   delete ui;
}

void LicenseKey_Dialog::on__100_clicked()
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

    done(100);
}

void LicenseKey_Dialog::on__101_clicked()
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

    done(101);
}

void LicenseKey_Dialog::on__102_clicked()
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

    done(102);
}
