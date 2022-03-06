
#include "ncwin.h"
#include "RCDialog.h"
#include "insertfiguretext_dialog.h"
#include "ui_insertfiguretext_dialog.h"

InsertFigureText_Dialog::InsertFigureText_Dialog(HANDLE parent, HANDLE rcparent, int f):
	InsertFigureText_Dialog((QWidget *)parent, (Dialog *)rcparent,( Qt::WindowFlags) f)
{}
InsertFigureText_Dialog::InsertFigureText_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::InsertFigureText_Dialog)
{
  ui->setupUi(this);

  initDialog(this);

  ui->_101->selectAll();

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

InsertFigureText_Dialog::~InsertFigureText_Dialog()
{
  delete ui;
}

void InsertFigureText_Dialog::on_accept()
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
