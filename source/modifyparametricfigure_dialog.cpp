
#include "ncwin.h"
#include "RCDialog.h"
#include "modifyparametricfigure_dialog.h"
#include "ui_MODIFYPARAMETRICFIGURE_DIALOG.h"

ModifyParametricFigure_Dialog::ModifyParametricFigure_Dialog(HANDLE parent, HANDLE rcparent, int f):
	ModifyParametricFigure_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
ModifyParametricFigure_Dialog::ModifyParametricFigure_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::ModifyParametricFigure_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

ModifyParametricFigure_Dialog::~ModifyParametricFigure_Dialog()
{
    delete ui;
}

void ModifyParametricFigure_Dialog::on__1_clicked()
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
    //    QDialog::accept();

}
