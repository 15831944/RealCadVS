
#include "ncwin.h"
#include "RCDialog.h"
#include "measureangle_dialog.h"
#include "ui_measureangle_dialog.h"

MeasureAngle_Dialog::MeasureAngle_Dialog(HANDLE parent, HANDLE rcparent, int f):
	MeasureAngle_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
MeasureAngle_Dialog::MeasureAngle_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::MeasureAngle_Dialog)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Tool);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

MeasureAngle_Dialog::~MeasureAngle_Dialog()
{
    delete ui;
}
