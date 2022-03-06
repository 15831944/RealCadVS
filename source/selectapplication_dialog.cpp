
#include "ncwin.h"
#include "RCDialog.h"
#include "rccombolist.h"
#include "selectapplication_dialog.h"
#include "ui_selectapplication_dialog.h"

SelectApplication_Dialog::SelectApplication_Dialog::SelectApplication_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectApplication_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SelectApplication_Dialog::SelectApplication_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectApplication_Dialog)
{
    ui->setupUi(this);
    ui->_100->adjustListSize();

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
    connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on_accept()));

}

SelectApplication_Dialog::~SelectApplication_Dialog()
{
    delete ui;
}

void SelectApplication_Dialog::on_accept()
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
