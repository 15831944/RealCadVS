
#include "ncwin.h"
#include "RCDialog.h"
#include "composeddrawing_dialog.h"
#include "ui_composeddrawing_dialog.h"

ComposedDrawing_Dialog::ComposedDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f):
	ComposedDrawing_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
ComposedDrawing_Dialog::ComposedDrawing_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::ComposedDrawing_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  qDebug() << "" << ui->_104->text();
  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

ComposedDrawing_Dialog::~ComposedDrawing_Dialog()
{
    delete ui;
}

void ComposedDrawing_Dialog::on__101_currentIndexChanged(int index)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    //wParam = CBN_SELCHANGE;
    wParam = MAKEWPARAM((WORD)101,(WORD)CBN_SELCHANGE);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void ComposedDrawing_Dialog::on__105_clicked(bool checked)
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

void ComposedDrawing_Dialog::on_accept()
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
