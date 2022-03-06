
#include "ncwin.h"
#include "RCDialog.h"
#include "licenseagreement_dialog.h"
#include "ui_licenseagreement_dialog.h"

LicenseAgreement_Dialog::LicenseAgreement_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LicenseAgreement_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
LicenseAgreement_Dialog::LicenseAgreement_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::LicenseAgreement_Dialog)
{
    ui->setupUi(this);

    initDialog(this);
}

LicenseAgreement_Dialog::~LicenseAgreement_Dialog()
{
   delete ui;
}

void LicenseAgreement_Dialog::on__101_clicked()
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

void LicenseAgreement_Dialog::on__102_clicked()
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
