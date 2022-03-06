

#include "ncwin.h"

#include "RCDialog.h"
#include "figuretext_dialog.h"
#include "ui_figuretext_dialog.h"


FigureText_Dialog::FigureText_Dialog(HANDLE parent, HANDLE rcparent, int f):
	FigureText_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
FigureText_Dialog::FigureText_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::FigureText_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

FigureText_Dialog::~FigureText_Dialog()
{
    delete ui;
}

void FigureText_Dialog::on_accept()
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
