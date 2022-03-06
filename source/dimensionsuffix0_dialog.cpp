
#include "ncwin.h"
#include "RCDialog.h"
#include "dimensionsuffix0_dialog.h"
#include "ui_dimensionsuffix0_dialog.h"

DimensionSuffix0_Dialog::DimensionSuffix0_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DimensionSuffix0_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DimensionSuffix0_Dialog::DimensionSuffix0_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::DimensionSuffix0_Dialog)
{
    ui->setupUi(this);

    ignoreCheckChange=false;
    ignoreTextChange=false;
    selectText=false;

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_103)
        ui->_103->setScene(scene);
    ((SEntryDisplayDialogControl*)RCdlg->getcontrol(103))->setQW(ui->_103);
    ((SEntryDisplayDialogControl*)RCdlg->getcontrol(103))->sethwnd(ui->_103);

    ui->_100->setTristate(change);

    initDialog(this);

    if(change)
    {
        ui->SuffixLabel->setVisible(false);
        ui->SuffixLabel->hide();
        ui->_104->setVisible(true);
        ui->_104->show();
        lastText = ui->_102->toPlainText();
        ui->_102->setPlainText(lastText);
    }
    else
    {
        ui->_104->setVisible(false);
        ui->_104->hide();
        ui->SuffixLabel->setVisible(true);
        ui->SuffixLabel->show();
        lastText = ui->_102->toPlainText();
        ui->_102->setPlainText(lastText);
    }

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

}

DimensionSuffix0_Dialog::~DimensionSuffix0_Dialog()
{
    delete ui;
}

void DimensionSuffix0_Dialog::on__100_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_100->isTristate())
    {
        ui->_100->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(100))->setTriState(false);
    }

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 100
    wParam = MAKEWPARAM((WORD)100,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionSuffix0_Dialog::on__101_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}


void DimensionSuffix0_Dialog::on__102_textChanged()
{
    QString text = ui->_102->toPlainText();

    if(!ui->_102->document()->isEmpty())
    {
        if(!ignoreTextChange)
        {
            if(lastText == text)
                selectText=true;
            else
                selectText=false;
            lastText = text;
            ignoreTextChange=true;
            ui->_102->setPlainText(lastText);
            if(!ui->_104->isChecked())
                on__104_clicked(true);
            ignoreTextChange=false;
            if(selectText)
                ui->_102->selectAll();
            else
                ui->_102->moveCursor(QTextCursor::End);
            ui->_102->setPlaceholderText("");
        }
    }
    else
    {
        if(!ignoreTextChange)
        {
            if(ui->_104->isChecked())
            {
                lastText = text;
                ui->_102->setPlaceholderText("");
            }
            else
                ui->_102->setPlaceholderText(QString(((StringDialogControl*)RCdlg->getcontrol(102))->getSpecialValueText()));
        }
    }

    drawSample();
}

void DimensionSuffix0_Dialog::on__104_clicked(bool checked)
{
    if(checked)
    {
         ui->_102->setPlainText(lastText);
    }
    else
    {
        ui->_102->setPlainText(QString());
    }

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void DimensionSuffix0_Dialog::on_accept()
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



void DimensionSuffix0_Dialog::drawSample()
{
    if((SEntryDisplayDialogControl*)RCdlg->getcontrol(103));
        ((SEntryDisplayDialogControl*)RCdlg->getcontrol(103))->draw(RCdlg,0);

}
