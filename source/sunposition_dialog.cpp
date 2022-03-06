
#include "ncwin.h"
#include "RCDialog.h"
#include "sunposition_dialog.h"
#include "ui_SUNPOSITION_DIALOG.h"

SunPosition_Dialog::SunPosition_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SunPosition_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SunPosition_Dialog::SunPosition_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
        RCDialog(rcparent,parent,f),
        ui(new Ui::SunPosition_Dialog)
{

    ui->setupUi(this);

     initDialog(this);

     ui->_107->setVisible(false);

     ui->_100->setDate(QDate::currentDate());
     ui->_101->setTime(QTime::currentTime());

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

SunPosition_Dialog::~SunPosition_Dialog()
{
    delete ui;
}

void SunPosition_Dialog::on__102_currentIndexChanged(int index)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)102,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SunPosition_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}

void SunPosition_Dialog::on__100_dateChanged(const QDate &date)
{
    QString dateStr = date.toString("yyyy/MM/dd");
    ((StringDialogControl*)RCdlg->getcontrol(100))->change(RCdlg,dateStr.data());
}

void SunPosition_Dialog::on__101_timeChanged(const QTime &time)
{
    QString timeStr = time.toString("HH:mm:ss");
    ((StringDialogControl*)RCdlg->getcontrol(101))->change(RCdlg,timeStr.data());
}


void SunPosition_Dialog::on__106_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SunPosition_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);
}
