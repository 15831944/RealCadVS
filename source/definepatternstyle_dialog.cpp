
#include "ncwin.h"
#include "ncdialog.h"
#include "RCDialog.h"
#include "definepatternstyle_dialog.h"
#include "ui_definepatternstyle_dialog.h"

DefinePatternStyle_Dialog::DefinePatternStyle_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DefinePatternStyle_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DefinePatternStyle_Dialog::DefinePatternStyle_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
  //QWidget(parent)
  RCDialog(rcparent,parent,f),
  ui(new Ui::DefinePatternStyle_Dialog)
{
    ui->setupUi(this);

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_109)
        ui->_109->setScene(scene);
    ((TextDisplayDialogControl*)RCdlg->getcontrol(109))->setQW(ui->_109);
    ((TextDisplayDialogControl*)RCdlg->getcontrol(109))->sethwnd(ui->_109);

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

DefinePatternStyle_Dialog::~DefinePatternStyle_Dialog()
{
    delete ui;
}

void DefinePatternStyle_Dialog::on__101_valueChanged(double v)
{
    drawSample();
}

void DefinePatternStyle_Dialog::on__100_valueChanged(double v)
{
    drawSample();
}

void DefinePatternStyle_Dialog::on__107_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 107
    wParam = MAKEWPARAM((WORD)107,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_107;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DefinePatternStyle_Dialog::on__110_currentTextChanged(const QString & text)
{
    drawSample();
}

void DefinePatternStyle_Dialog::on__110_currentIndexChanged(int index)
{
    drawSample();
}

void DefinePatternStyle_Dialog::on__112_currentIndexChanged(int index)
{
    drawSample();
}

void DefinePatternStyle_Dialog::on__120_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 120
    wParam = MAKEWPARAM((WORD)120,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_120;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DefinePatternStyle_Dialog::on__121_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 121
    wParam = MAKEWPARAM((WORD)121,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_121;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DefinePatternStyle_Dialog::on__122_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 122
    wParam = MAKEWPARAM((WORD)122,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_122;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::done(122);
}

void DefinePatternStyle_Dialog::on_accept()
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

void DefinePatternStyle_Dialog::drawSample()
{

    if(isVisible())
    {
        if((PatternStyleDisplayDialogControl*)RCdlg->getcontrol(109));
            ((PatternStyleDisplayDialogControl*)RCdlg->getcontrol(109))->draw(RCdlg,0);
    }

}
