
#include "ncwin.h"
#include "RCDialog.h"
#include "bumpmap_dialog.h"
#include "ui_BUMPMAP_DIALOG.h"

Bumpmap_Dialog::Bumpmap_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Bumpmap_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Bumpmap_Dialog::Bumpmap_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Bumpmap_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Bumpmap_Dialog::~Bumpmap_Dialog()
{
    delete ui;
}

void Bumpmap_Dialog::on__105_clicked()
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

void Bumpmap_Dialog::on_accept()
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

void Bumpmap_Dialog::on__106_clicked()
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
