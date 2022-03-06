
#include "ncwin.h"
#include "ncdialog.h"
#include "RCDialog.h"
#include "dimensionterminator_dialog.h"
#include "ui_dimensionterminator_dialog.h"

bool DimTerminatorVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<DimensionTerminator_Dialog *>(dialog) != 0)
            {

                if(obj->objectName()== "_200")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((DimensionTerminator_Dialog*)dialog)->ui->_200->setCurrentIndex(1);
                    ((DimensionTerminator_Dialog*)dialog)->ui->_200->hide();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_100->show();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_100->showPopup();
                    return true;
                }
                if(obj->objectName()== "_203")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((DimensionTerminator_Dialog*)dialog)->ui->_203->setCurrentIndex(1);
                    ((DimensionTerminator_Dialog*)dialog)->ui->_203->hide();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_103->show();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_103->showPopup();
                    return true;
                }
                if(obj->objectName()== "_204")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((DimensionTerminator_Dialog*)dialog)->ui->_204->setCurrentIndex(1);
                    ((DimensionTerminator_Dialog*)dialog)->ui->_204->hide();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_104->show();
                    ((DimensionTerminator_Dialog*)dialog)->ui->_104->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<DimensionTerminator_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((DimensionTerminator_Dialog*)dialog)->variesWasSet_101)
                {
                    ((DimensionTerminator_Dialog*)dialog)->variesWasSet_101 = false;
                    ((DimensionTerminator_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionTerminator_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((DimensionTerminator_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((DimensionTerminator_Dialog*)dialog)->variesWasSet_102)
                {
                    ((DimensionTerminator_Dialog*)dialog)->variesWasSet_102 = false;
                    ((DimensionTerminator_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionTerminator_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((DimensionTerminator_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((DimensionTerminator_Dialog*)dialog)->variesWasSet_106)
                {
                    ((DimensionTerminator_Dialog*)dialog)->variesWasSet_106 = false;
                    ((DimensionTerminator_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((DimensionTerminator_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    ((DimensionTerminator_Dialog*)dialog)->ui->_106->clear();
                    return false;
                }
            }
        }
        return false;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

DimensionTerminator_Dialog::DimensionTerminator_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DimensionTerminator_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DimensionTerminator_Dialog::DimensionTerminator_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::DimensionTerminator_Dialog)
{
    ui->setupUi(this);

    variesWasSet_101 = false;
    ui->_101->setValue(ui->_101->maximum());
    variesWasSet_102 = false;
    ui->_102->setValue(ui->_102->maximum());
    variesWasSet_106 = false;
    ui->_106->setValue(ui->_106->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // make the dummy style combo the same size as the real one and hide it
    ui->_200->setGeometry(ui->_100->geometry());
    ui->_200->hide();

    // make the dummy position combo the same size as the real one and hide it
    ui->_203->setGeometry(ui->_103->geometry());
    ui->_203->hide();

    // make the dummy visible combo the same size as the real one and hide it
    ui->_204->setGeometry(ui->_104->geometry());
    ui->_204->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_105)
        ui->_105->setScene(scene);
    ((NTerminatorDisplayDialogControl*)RCdlg->getcontrol(105))->setQW(ui->_105);
    ((NTerminatorDisplayDialogControl*)RCdlg->getcontrol(105))->sethwnd(ui->_105);


    ui->_107->setTristate(change);//

    initDialog(this);

    if(change)
    {
        // replace the tolerance combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(15))
        {
            ui->_100->hide();
            ui->_200->setVisible(true);
            ui->_200->show();
        }
        else
        {
            ui->_100->show();
            ui->_200->hide();
        }
        // replace the position combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(78))
        {
            ui->_103->hide();
            ui->_203->setVisible(true);
            ui->_203->show();
        }
        else
        {
            ui->_103->show();
            ui->_203->hide();
        }
        // replace the visible combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(79))
        {
            ui->_104->hide();
            ui->_204->setVisible(true);
            ui->_204->show();
        }
        else
        {
            ui->_104->show();
            ui->_204->hide();
        }
    }
    else
    {
        ui->_200->hide();
        ui->_203->hide();
        ui->_204->hide();
    }

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    DimTerminatorVerifyEventFilter *Filter = new DimTerminatorVerifyEventFilter(this);
    ui->_200->installEventFilter(Filter);
    ui->_203->installEventFilter(Filter);
    ui->_204->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_106->installEventFilter(Filter);

    QTimer::singleShot(200, this, SLOT(drawSample()));
}

void DimensionTerminator_Dialog::on__100_currentIndexChanged(int index)
{
    drawSample();
}

void DimensionTerminator_Dialog::on__101_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(101))->getDefaultValue();
    if(variesWasSet_101 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(101))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
        ui->_101->setSpecialValueText("");
        variesWasSet_101 = false;
    }
    else
    {
        if(!ui->_101->specialValueText().isEmpty() && ui->_101->minimum() == v )
            variesWasSet_101 = true;
    }

    drawSample();
}

void DimensionTerminator_Dialog::on__102_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(102))->getDefaultValue();
    if(variesWasSet_102 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(102))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
        ui->_102->setSpecialValueText("");
        variesWasSet_102 = false;
    }
    else
    {
        if(!ui->_102->specialValueText().isEmpty() && ui->_102->minimum() == v )
            variesWasSet_102 = true;
    }

    drawSample();
}

void DimensionTerminator_Dialog::on__103_currentIndexChanged(int index)
{
    drawSample();
}

void DimensionTerminator_Dialog::on__104_currentIndexChanged(int index)
{
    drawSample();
}

void DimensionTerminator_Dialog::on__106_valueChanged(double v)
{
    double defVal = ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(106))->getDefaultValue();
    if(variesWasSet_106 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,defVal);
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
        ui->_106->setSpecialValueText("");
        variesWasSet_106 = false;
    }
    else
    {
        if(!ui->_106->specialValueText().isEmpty() && ui->_106->minimum() == v )
            variesWasSet_106 = true;
    }

    drawSample();
}

void DimensionTerminator_Dialog::on__107_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_107->isTristate())
    {
        ui->_107->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(107))->setTriState(false);
    }
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

void DimensionTerminator_Dialog::on__1006_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1006
    wParam = MAKEWPARAM((WORD)1006,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1006;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

DimensionTerminator_Dialog::~DimensionTerminator_Dialog()
{
    delete ui;
}

void DimensionTerminator_Dialog::on_accept()
{

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}

void DimensionTerminator_Dialog::drawSample()
{
    if((NTerminatorDisplayDialogControl*)RCdlg->getcontrol(105));
        ((NTerminatorDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,0);
}
