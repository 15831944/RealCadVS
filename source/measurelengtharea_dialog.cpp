
#include "ncwin.h"
#include "RCDialog.h"
#include "measurelengtharea_dialog.h"
#include "ui_measurelengtharea_dialog.h"

MeasureLengthArea_Dialog::MeasureLengthArea_Dialog(HANDLE parent, HANDLE rcparent, int f):
	MeasureLengthArea_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
MeasureLengthArea_Dialog::MeasureLengthArea_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::MeasureLengthArea_Dialog)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Tool);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}


MeasureLengthArea_Dialog::~MeasureLengthArea_Dialog()
{
    delete ui;
}

void MeasureLengthArea_Dialog::on__110_clicked(bool checked)
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

void MeasureLengthArea_Dialog::closeEvent(QCloseEvent * e)
{
    RCDialog::closeEvent(e);
}

void MeasureLengthArea_Dialog::on_hide()
{
    RCDialog::hide();
}

/*
void MeasureLengthArea_Dialog::on_accept()
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
*/
