#include "ncwin.h"
#include "RCDialog.h"
#include "aboutrealcad_dialog.h"
#include "ui_aboutrealcad_dialog.h"

AboutRealCAD_Dialog::AboutRealCAD_Dialog(HANDLE parent, HANDLE rcparent, int f):
	AboutRealCAD_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
AboutRealCAD_Dialog::AboutRealCAD_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::AboutRealCAD_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));

}

AboutRealCAD_Dialog::~AboutRealCAD_Dialog()
{
    delete ui;
}

void AboutRealCAD_Dialog::on__104_clicked()
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

void AboutRealCAD_Dialog::on__106_clicked()
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

void AboutRealCAD_Dialog::on_accept()
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
