
#include "ncwin.h"
#include "RCDialog.h"
#include "measurearea_dialog.h"
#include "ui_measurearea_dialog.h"

MeasureArea_Dialog::MeasureArea_Dialog(HANDLE parent, HANDLE rcparent, int f):
	MeasureArea_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
MeasureArea_Dialog::MeasureArea_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::MeasureArea_Dialog)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Tool);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
  //connect(ui->_100,SIGNAL(listItemDoubleClicked(QListWidgetItem*)),this,SLOT(on__101_clicked()));
}

MeasureArea_Dialog::~MeasureArea_Dialog()
{
    delete ui;
}
