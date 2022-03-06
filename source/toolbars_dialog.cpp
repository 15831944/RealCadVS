#include "ncwin.h"
#include "RCDialog.h"
#include "toolbars_dialog.h"
#include "ui_toolbars_dialog.h"

ToolBars_Dialog::ToolBars_Dialog(HANDLE parent, HANDLE rcparent, int f):
	ToolBars_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
ToolBars_Dialog::ToolBars_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::ToolBars_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    ui->_100->clearSelection();
    ui->_101->clearSelection();

    ui->_100->clearFocus();
    ui->_101->clearFocus();

    ui->_102->setEnabled(false);
    ui->_103->setEnabled(false);

    //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

ToolBars_Dialog::~ToolBars_Dialog()
{
    delete ui;
}

void ToolBars_Dialog::on__102_clicked()
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

void ToolBars_Dialog::on__103_clicked()
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

void ToolBars_Dialog::on__104_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK/104
    wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}

void ToolBars_Dialog::on__105_clicked()
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

    done(105);
}

void ToolBars_Dialog::on__100_itemDoubleClicked(QListWidgetItem * item)
{
    on__102_clicked();
}

void ToolBars_Dialog::on__100_itemClicked(QListWidgetItem * item)
{
    ui->_101->clearSelection();
    ui->_102->setEnabled(true);
    ui->_103->setEnabled(false);

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)100,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void ToolBars_Dialog::on__101_itemDoubleClicked(QListWidgetItem * item)
{
    on__103_clicked();
}

void ToolBars_Dialog::on__101_itemClicked(QListWidgetItem * item)
{
    ui->_100->clearSelection();
    ui->_102->setEnabled(false);
    ui->_103->setEnabled(true);

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)101,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}
