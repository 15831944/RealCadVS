
#include "ncwin.h"

#include "RCDialog.h"
#include "GeneratePoints_Dialog.h"
#include "ui_GeneratePoints_Dialog.h"

GeneratePoints_Dialog::GeneratePoints_Dialog(HANDLE parent, HANDLE rcparent, int f):
	GeneratePoints_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
GeneratePoints_Dialog::GeneratePoints_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
      RCDialog(rcparent, parent, f),
      ui(new Ui::GeneratePoints_Dialog)
{
    ui->setupUi(this);
	ActivateParent = false;
    // put any inits not know to the realCAD Dialog class here
    //ui->_102->setChecked(true);
    //ui->_104->setText("1");
    //ui->_105->setText("1");

    // init the realCAD Dialog class here
    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}


void GeneratePoints_Dialog::on__104_valueChanged(int value)
{

}

void GeneratePoints_Dialog::on__1104_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_104;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_104;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    // using the direct functions
    /*
    if(action == QAbstractSlider::SliderSingleStepAdd)
        ui->_104->setText(QString::number(ui->_104->text().toDouble() - 1.0));
    else if(action == QAbstractSlider::SliderSingleStepSub)
        ui->_104->setText(QString::number(ui->_104->text().toDouble() + 1.0));
    */
}

void GeneratePoints_Dialog::on__1105_actionTriggered(int action)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    if(action == QAbstractSlider::SliderSingleStepAdd)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEDOWN;
        lParam = (LPARAM)ui->_105;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(action == QAbstractSlider::SliderSingleStepSub)
    {
        msg = WM_VSCROLL;
        wParam = SB_LINEUP;
        lParam = (LPARAM)ui->_105;

        dialogcb((HWND)this,msg,wParam,lParam);
    }
    // using the direct functions
    /*
    if(action == QAbstractSlider::SliderSingleStepAdd)
        ui->_105->setText(QString::number(ui->_105->text().toInt() - 1));
    else if(action == QAbstractSlider::SliderSingleStepSub)
        ui->_105->setText(QString::number(ui->_105->text().toInt() + 1));
    */
}

void GeneratePoints_Dialog::on__105_valueChanged(double value)
{

}

// this function is used for the dailogcb function
void GeneratePoints_Dialog::on__102_clicked(bool checked)
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

// this function is used for direct exchange
void GeneratePoints_Dialog::on__102_toggled(bool checked)
{
    // using the direct functions
    /*
    // enable 104 and 1104
    ui->_104->setEnabled(true);
    ui->_1104->setEnabled(true);   
    // disable 105 an 1105
    ui->_105->setEnabled(false);
    ui->_1105->setEnabled(false);
    */
}

// this function is used for the dailogcb function
void GeneratePoints_Dialog::on__103_clicked(bool checked)
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

// this function is used for direct exchange
void GeneratePoints_Dialog::on__103_toggled(bool checked)
{
    // using the direct functions
    /*
    // enable 105 and 1105
    ui->_105->setEnabled(true);
    ui->_1105->setEnabled(true);
    // disable 104 an 1104
    ui->_104->setEnabled(false);
    ui->_1104->setEnabled(false);
    */
}
/*
void GeneratePoints_Dialog::on_accept()
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
*/
void GeneratePoints_Dialog::on__1_clicked(bool checked)
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

    //QDialog::accept();
}

void GeneratePoints_Dialog::on__2_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDCANCEL,(WORD)0);
    lParam = (LPARAM)ui->_2;
    dialogcb((HWND)this,msg,wParam,lParam);

    //QDialog::reject();
}

GeneratePoints_Dialog::~GeneratePoints_Dialog()
{
    delete ui;
}

void GeneratePoints_Dialog::on_accept()
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
