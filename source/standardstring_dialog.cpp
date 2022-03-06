#include "ncwin.h"

#include "RCDialog.h"
#include "standardstring_dialog.h"
#include "ui_standardstring_dialog.h"

StandardString_Dialog::StandardString_Dialog(HANDLE parent, HANDLE rcparent, int f):
	StandardString_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
StandardString_Dialog::StandardString_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent, parent, f),
    ui(new Ui::StandardString_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  // put any inits not know to the realCAD Dialog class here
  // init the realCAD Dialog class here
  initDialog(this);

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

StandardString_Dialog::~StandardString_Dialog()
{
    delete ui;
}

void StandardString_Dialog::on_accept( )
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
