
#include "ncwin.h"
#include "ncdialog.h"
#include "RCDialog.h"
#include "information_dialog.h"
#include "ui_information_dialog.h"

Information_Dialog::Information_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Information_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Information_Dialog::Information_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Information_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  // put the cursor in the first description cell
  //ui->_100->setCurrentCell(0,1,QItemSelectionModel::Select);
  //ui->_100->item(0,1)->setSelected(true);
  ui->_100->editItem(ui->_100->item(0,1));

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  //ui->_100->installEventFilter(this);
}

Information_Dialog::~Information_Dialog()
{
  delete ui;
}

bool Information_Dialog::eventFilter(QObject *obj, QEvent *e)
{

    if(e->type() == QEvent::KeyPress)
    {
        if(((QKeyEvent*)e)->key() == Qt::Key_Tab)
        {
            int nextRow = ui->_100->currentRow()+1;
            if(nextRow < 0)
                nextRow = ui->_100->rowCount()-1;
            else if(nextRow > ui->_100->rowCount())
                nextRow = 0;

            ui->_100->setCurrentCell(nextRow,1);
            ui->_100->editItem(ui->_100->item(nextRow,1));
            return true;

        }
        else if(((QKeyEvent*)e)->key() == Qt::Key_Backtab)
        {
            int nextRow = ui->_100->currentRow()-1;
            if(nextRow < 0)
                nextRow = ui->_100->rowCount()-1;
            else if(nextRow > ui->_100->rowCount())
                nextRow = 0;

            ui->_100->setCurrentCell(nextRow,1);
            ui->_100->editItem(ui->_100->item(nextRow,1));
            return true;
        }
    }

    return QDialog::eventFilter(obj,e);
}

void Information_Dialog::on_accept()
{
    EIListDialogControl *table = (EIListDialogControl*)rcDialog()->getcontrol(100);
    RCCHAR *plabel = table->labels;
    RCCHAR *pvalue = table->values;
    for(int i=0; i<ui->_100->rowCount(); i++)
    {
        QString label  = ui->_100->item(i,0)->text();
        QString value = ui->_100->item(i,1)->text();
        strcpy(plabel,label.data());
        plabel += 100;
        strcpy(pvalue,value.data());
        pvalue += 300;
    }
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
