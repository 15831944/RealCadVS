
#include "ncwin.h"
#include "RCDialog.h"
#include "dimensioncterminator_dialog.h"
#include "ui_dimensioncterminator_dialog.h"

bool DimCTerminatorVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<DimensionCTerminator_Dialog *>(dialog) != 0)
            {

                if(obj->objectName()== "_200")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_200->setCurrentIndex(1);
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_200->hide();
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_100->show();
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_100->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<DimensionCTerminator_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((DimensionCTerminator_Dialog*)dialog)->variesWasSet_101)
                {
                    ((DimensionCTerminator_Dialog*)dialog)->variesWasSet_101 = false;
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionCTerminator_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((DimensionCTerminator_Dialog*)dialog)->variesWasSet_102)
                {
                    ((DimensionCTerminator_Dialog*)dialog)->variesWasSet_102 = false;
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionCTerminator_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_104")
            {
                // only use this once
                if(((DimensionCTerminator_Dialog*)dialog)->variesWasSet_104)
                {
                    ((DimensionCTerminator_Dialog*)dialog)->variesWasSet_104 = false;
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_104->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((DimensionCTerminator_Dialog*)dialog)->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
                    ((DimensionCTerminator_Dialog*)dialog)->ui->_104->clear();
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

DimensionCTerminator_Dialog::DimensionCTerminator_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DimensionCTerminator_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DimensionCTerminator_Dialog::DimensionCTerminator_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::DimensionCTerminator_Dialog)
{
    ui->setupUi(this);

    variesWasSet_101 = false;
    ui->_101->setValue(ui->_101->maximum());
    variesWasSet_102 = false;
    ui->_102->setValue(ui->_102->maximum());
    variesWasSet_104 = false;
    ui->_104->setValue(ui->_104->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // make the dummy style combo the same size as the real one and hide it
    ui->_200->setGeometry(ui->_100->geometry());
    ui->_200->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_103)
        ui->_103->setScene(scene);
    ((CTerminatorDisplayDialogControl*)RCdlg->getcontrol(103))->setQW(ui->_103);
    ((CTerminatorDisplayDialogControl*)RCdlg->getcontrol(103))->sethwnd(ui->_103);

    ui->_105->setTristate(change);//

    initDialog(this);

    if(change)
    {
        // replace the tolerance combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(20))
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
    }
    else
    {
        ui->_200->hide();
    }

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    DimCTerminatorVerifyEventFilter *Filter = new DimCTerminatorVerifyEventFilter(this);
    ui->_200->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_104->installEventFilter(Filter);

    //QTimer::singleShot(200, this, SLOT(drawSample()));

}

DimensionCTerminator_Dialog::~DimensionCTerminator_Dialog()
{
    delete ui;
}

void DimensionCTerminator_Dialog::on__100_currentIndexChanged(int index)
{
    drawSample();
}

void DimensionCTerminator_Dialog::on__101_valueChanged(double v)
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

void DimensionCTerminator_Dialog::on__102_valueChanged(double v)
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

void DimensionCTerminator_Dialog::on__104_valueChanged(double v)
{
    double defVal = ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(104))->getDefaultValue();
    if(variesWasSet_104 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(104))->change(this->RCdlg,defVal);
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
        ui->_104->setSpecialValueText("");
        variesWasSet_104 = false;
    }
    else
    {
        if(!ui->_104->specialValueText().isEmpty() && ui->_104->minimum() == v )
            variesWasSet_104 = true;
    }

    drawSample();
}

void DimensionCTerminator_Dialog::on__105_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_105->isTristate())
    {
        ui->_105->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(105))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 105
    wParam = MAKEWPARAM((WORD)105,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_105;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionCTerminator_Dialog::on__1007_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1007
    wParam = MAKEWPARAM((WORD)1007,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1007;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionCTerminator_Dialog::on_accept()
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

void DimensionCTerminator_Dialog::drawSample()
{
    if((CTerminatorDisplayDialogControl*)RCdlg->getcontrol(103));
        ((CTerminatorDisplayDialogControl*)RCdlg->getcontrol(103))->draw(RCdlg,0);
}
