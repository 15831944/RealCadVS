
#include "ncwin.h"
#include "RCDialog.h"
#include "measure_dialog.h"
#include "ui_MEASURE_DIALOG.h"

Measure_Dialog::Measure_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Measure_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Measure_Dialog::Measure_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Measure_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool);

    initDialog(this);
}

Measure_Dialog::~Measure_Dialog()
{
    delete ui;
}

void Measure_Dialog::on__107_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 107
    wParam = MAKEWPARAM((WORD)107,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_107;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Measure_Dialog::on__120_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 120
    wParam = MAKEWPARAM((WORD)120,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_120;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Measure_Dialog::on__121_clicked(bool checked)
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

void Measure_Dialog::closeEvent(QCloseEvent * e)
{
    //AbortEvent *aevent = new AbortEvent;
    ExitEvent *aevent = new ExitEvent;
    state.sendevent(aevent);

    QDialog::reject();
    //RCDialog::closeEvent(e);
}

void Measure_Dialog::on_hide()
{
    RCDialog::hide();
}
