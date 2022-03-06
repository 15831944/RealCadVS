
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "selectdrawingview_dialog.h"
#include "ui_selectdrawingview_dialog.h"

SelectDrawingView_Dialog::SelectDrawingView_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectDrawingView_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SelectDrawingView_Dialog::SelectDrawingView_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectDrawingView_Dialog)
{
  ui->setupUi(this);
  ui->_100->adjustListSize();
  ui->_101->adjustListSize();

  initDialog(this);

  // set the defaults
  if(ui->_102->isChecked())
  {
      ui->_103->setChecked(false);
      ui->_103->setEnabled(true);
  }
  else
      ui->_103->setEnabled(false);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  //connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
  //connect(ui->_101,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));
  //connect(ui->_101,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(on__101_itemClicked(QListWidgetItem*)));

}

SelectDrawingView_Dialog::~SelectDrawingView_Dialog()
{
    delete ui;
}

// hide when plotting
void SelectDrawingView_Dialog::on__102_clicked(bool checked)
{
    // uncheck and disable dashed if set to unchecked
    // enable if set to checked
    if(checked)
    {
        ui->_103->setEnabled(true);
    }
    if(!checked)
    {
        ui->_103->setChecked(false);
        ui->_103->setEnabled(false);
    }

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

// dashed when hiding
void SelectDrawingView_Dialog::on__103_clicked(bool checked)
{
    // set hide when plotting on when this is clicked
    if(checked)
        ui->_102->setChecked(checked);
    else
    {
        if(!ui->_102->isChecked())
            ui->_103->setEnabled(false);
        else
            ui->_103->setEnabled(true);
    }
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

// shade when plotting
void SelectDrawingView_Dialog::on__105_clicked(bool checked)
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

void SelectDrawingView_Dialog::on_accept()
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

void SelectDrawingView_Dialog::on__100_currentRowChanged(int currentRow)
{
    RCdlg->getcontrol(100)->changefocus(RCdlg);
    if(ui->_101->count() < 1)
    {
        if(RCdlg->getcontrol(101))
        {
            int bid = ((SelectViewZoneListDialogControl*)RCdlg->getcontrol(101))->buddyId;
            getControl(bid)->setEnabled(true);
        }
     }
}


void SelectDrawingView_Dialog::on__101_currentRowChanged(int currentRow)
{
    if(currentRow >= 0)
        v.setstring("op::lastzonename",ui->_101->item(currentRow)->text().data());
}


void SelectDrawingView_Dialog::on__101_itemClicked(QListWidgetItem * item)
{
    qDebug() << "on__101_clicked";
    if(ui->_101->count() > 0)
    {
        if(RCdlg->getcontrol(101))
        {
            int bid = ((SelectViewZoneListDialogControl*)RCdlg->getcontrol(101))->buddyId;
            getControl(bid)->setEnabled(false);
        }
    }
    else
    {
        if(RCdlg->getcontrol(101))
        {
            int bid = ((SelectViewZoneListDialogControl*)RCdlg->getcontrol(101))->buddyId;
            getControl(bid)->setEnabled(true);
        }
    }

}

bool SelectDrawingView_Dialog::eventFilter(QObject *watched, QEvent *event)
{
    return QObject::eventFilter(watched, event);
}
