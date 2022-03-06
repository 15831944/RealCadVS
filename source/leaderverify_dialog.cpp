
#include "ncwin.h"
#include "RCDialog.h"
#include "leaderverify_dialog.h"
#include "ui_leaderverify_dialog.h"

LeaderVerify_Dialog::LeaderVerify_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LeaderVerify_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
LeaderVerify_Dialog::LeaderVerify_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::LeaderVerify_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

LeaderVerify_Dialog::~LeaderVerify_Dialog()
{
    delete ui;
}

// bold
void LeaderVerify_Dialog::on__110_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 110
    wParam = MAKEWPARAM((WORD)110,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_110;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// underline
void LeaderVerify_Dialog::on__111_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 111
    wParam = MAKEWPARAM((WORD)111,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_111;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// italic
void LeaderVerify_Dialog::on__113_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 113
    wParam = MAKEWPARAM((WORD)113,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_113;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// boxed
void LeaderVerify_Dialog::on__114_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 114
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// outline
void LeaderVerify_Dialog::on__112_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 112
    wParam = MAKEWPARAM((WORD)112,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_112;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// halo
void LeaderVerify_Dialog::on__115_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 115
    wParam = MAKEWPARAM((WORD)115,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_115;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// scale text
void LeaderVerify_Dialog::on__121_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 121
    wParam = MAKEWPARAM((WORD)121,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_121;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// specify terminator weight
void LeaderVerify_Dialog::on__119_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 119
    wParam = MAKEWPARAM((WORD)119,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_119;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// change test button
void LeaderVerify_Dialog::on__2000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 2000
    wParam = MAKEWPARAM((WORD)2000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_2000;
    dialogcb((HWND)this,msg,wParam,lParam);
}
void LeaderVerify_Dialog::on__1000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1000
    wParam = MAKEWPARAM((WORD)1000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1000;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on__1001_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1001
    wParam = MAKEWPARAM((WORD)1001,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1001;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on__1002_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1002
    wParam = MAKEWPARAM((WORD)1002,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1002;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on__1003_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1003
    wParam = MAKEWPARAM((WORD)1003,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1003;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on__1004_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1004
    wParam = MAKEWPARAM((WORD)1004,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1004;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on__1005_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1005
    wParam = MAKEWPARAM((WORD)1005,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1005;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LeaderVerify_Dialog::on_accept()
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

