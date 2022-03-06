
#include "ncwin.h"
#include "RCDialog.h"
#include "oneviewchange_dialog.h"
#include "ui_oneviewchange_dialog.h"

OneViewChange_Dialog::OneViewChange_Dialog(HANDLE parent, HANDLE rcparent, int f):
	OneViewChange_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
OneViewChange_Dialog::OneViewChange_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::OneViewChange_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  if(!ui->_107->isChecked())
  {
      if(ui->_111->isChecked())
          on__111_clicked(false);
      ui->_111->setEnabled(false);
  }
  else
      ui->_111->setEnabled(true);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

OneViewChange_Dialog::~OneViewChange_Dialog()
{
    delete ui;
}

// hidden line when plotting
void OneViewChange_Dialog::on__107_clicked(bool checked)
{
    // uncheck and disable dashed if set to unchecked
    // enable if set to checked
    if(checked)
    {
        ui->_111->setEnabled(true);
    }
    else
    {
        if(ui->_111->isChecked())
            on__111_clicked(false);
        ui->_111->setEnabled(false);
    }
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

// layer table button
void OneViewChange_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 108
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// shade view when plotting
void OneViewChange_Dialog::on__109_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 109
    wParam = MAKEWPARAM((WORD)109,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_109;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// dashed hidden lines when plotting
void OneViewChange_Dialog::on__111_clicked(bool checked)
{
    // set hide when plotting on when this is clicked
    if(checked)
    {
        if(!ui->_107->isChecked())
            on__107_clicked(true);
    }
    else
    {
        // this should never happen but just incase
        if(!ui->_107->isChecked())
            ui->_111->setEnabled(false);
    }

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

void OneViewChange_Dialog::on__114_clicked(bool checked)
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

void OneViewChange_Dialog::on_accept()
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
