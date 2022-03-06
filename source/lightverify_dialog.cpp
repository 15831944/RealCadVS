
#include "ncwin.h"
#include "RCDialog.h"
#include "lightverify_dialog.h"
#include "ui_LIGHTVERIFY_DIALOG.h"

bool LightVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<LightVerify_Dialog *>(dialog) != 0)
            {
                /*
                if(obj->objectName()== "_1005")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((LightVerify_Dialog*)dialog)->ui->_1005->setCurrentIndex(1);
                    ((LightVerify_Dialog*)dialog)->ui->_1005->hide();
                    ((LightVerify_Dialog*)dialog)->ui->_105->show();
                    ((LightVerify_Dialog*)dialog)->ui->_105->showPopup();
                    ((LightVerify_Dialog*)dialog)->ui->_105->setMaxVisibleItems(20);
                    return true;
                }
                */
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<LightVerify_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((LightVerify_Dialog*)dialog)->variesWasSet_100)
                {
                    ((LightVerify_Dialog*)dialog)->variesWasSet_100 = false;
                    ((LightVerify_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((LightVerify_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((LightVerify_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((LightVerify_Dialog*)dialog)->variesWasSet_101)
                {
                    ((LightVerify_Dialog*)dialog)->variesWasSet_101 = false;
                    ((LightVerify_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((LightVerify_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((LightVerify_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((LightVerify_Dialog*)dialog)->variesWasSet_102)
                {
                    ((LightVerify_Dialog*)dialog)->variesWasSet_102 = false;
                    ((LightVerify_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((LightVerify_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((LightVerify_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((LightVerify_Dialog*)dialog)->variesWasSet_103)
                {
                    ((LightVerify_Dialog*)dialog)->variesWasSet_103 = false;
                    ((LightVerify_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((LightVerify_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((LightVerify_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_104")
            {
                // intensity
                // only use this once
                if(((LightVerify_Dialog*)dialog)->variesWasSet_104)
                {
                    ((LightVerify_Dialog*)dialog)->variesWasSet_104 = false;
                    ((LightVerify_Dialog*)dialog)->ui->_104->setSpecialValueText("");
                    ((RealDialogControl*)((LightVerify_Dialog*)dialog)->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
                    ((LightVerify_Dialog*)dialog)->ui->_104->clear();
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

LightVerify_Dialog::LightVerify_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LightVerify_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
LightVerify_Dialog::LightVerify_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::LightVerify_Dialog)
{
    ui->setupUi(this);

    variesWasSet_100 = false;
    ui->_100->setValue(ui->_100->maximum());
    variesWasSet_101 = false;
    ui->_101->setValue(ui->_101->maximum());
    variesWasSet_102 = false;
    ui->_102->setValue(ui->_102->maximum());
    variesWasSet_103 = false;
    ui->_103->setValue(ui->_103->maximum());
    variesWasSet_104 = false;
    ui->_104->setValue(ui->_104->maximum());
    variesWasSet_107 = false;
    ui->_107->setValue(ui->_107->maximum());
    variesWasSet_109 = false;
    ui->_109->setValue(ui->_109->maximum());
    variesWasSet_110 = false;
    ui->_110->setValue(ui->_110->maximum());
    variesWasSet_112 = false;
    ui->_112->setValue(ui->_112->maximum());
    variesWasSet_113 = false;
    ui->_113->setValue(ui->_113->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    ui->_105->setTristate(change);// distant light
    ui->_106->setTristate(change);// white light
    ui->_108->setTristate(change);// enable fade
    ui->_111->setTristate(change);// enable spot

    ui->_2000->setEnabled(false);
    ui->Date->setText("");
    ui->Time->setText("");
    ui->TimeZone->setText("");
    ui->Latitude->setText("");
    ui->Longitude->setText("");

    initDialog(this);

    // if in verify mode enable the text box
    // and the default controls
    if(change && changecount == 1)
    {
        ui->_100->setDisabled(false);
        ui->label_100->setDisabled(false);
        ui->_1000->setDisabled(false);
        ui->_101->setDisabled(false);
        ui->label_101->setDisabled(false);
        ui->_1001->setDisabled(false);
        ui->_102->setDisabled(false);
        ui->label_102->setDisabled(false);
        ui->_1002->setDisabled(false);
        ui->_103->setDisabled(false);
        ui->label_103->setDisabled(false);
        ui->_1003->setDisabled(false);
    }
    else if(change && changecount > 1)
    {
        ui->_100->setDisabled(false);
        ui->label_100->setDisabled(false);
        ui->_1000->setDisabled(false);
        ui->_101->setDisabled(false);
        ui->label_101->setDisabled(false);
        ui->_1001->setDisabled(false);
        ui->_102->setDisabled(false);
        ui->label_102->setDisabled(false);
        ui->_1002->setDisabled(false);
        ui->_103->setDisabled(false);
        ui->label_103->setDisabled(false);
        ui->_1003->setDisabled(false);
    }
    else
    {
        ui->_100->setDisabled(true);
        ui->label_100->setDisabled(true);
        ui->_1000->setDisabled(true);
        ui->_101->setDisabled(true);
        ui->label_101->setDisabled(true);
        ui->_1001->setDisabled(true);
        ui->_102->setDisabled(true);
        ui->label_102->setDisabled(true);
        ui->_1002->setDisabled(true);
        ui->_103->setDisabled(true);
        ui->label_103->setDisabled(true);
        ui->_1003->setDisabled(true);
    }

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    LightVerifyEventFilter *Filter = new LightVerifyEventFilter(this);
    ui->_100->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_103->installEventFilter(Filter);
    ui->_104->installEventFilter(Filter);
    ui->_107->installEventFilter(Filter);
    ui->_109->installEventFilter(Filter);
    ui->_110->installEventFilter(Filter);
    ui->_112->installEventFilter(Filter);
    ui->_113->installEventFilter(Filter);
}

LightVerify_Dialog::~LightVerify_Dialog()
{
    delete ui;
}

bool LightVerify_Dialog::eventFilter(QObject *obj, QEvent *event)
{
    return QDialog::eventFilter(obj,event);
}

// default colour
void LightVerify_Dialog::on__100_valueChanged(int v)
{
    //((IntegerDialogControl*)RCdlg->getcontrol(100))->change(RCdlg,v);
    int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(100))->getDefaultValue();
    if(variesWasSet_100 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(100))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
        ui->_100->setSpecialValueText("");
        variesWasSet_100 = false;
    }
    else
    {
        if(!ui->_100->specialValueText().isEmpty() && ui->_100->minimum() == v )
            variesWasSet_100 = true;
    }

}

// default layer
void LightVerify_Dialog::on__101_valueChanged(int v)
{
    //((IntegerDialogControl*)RCdlg->getcontrol(101))->change(RCdlg,v);
    int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(101))->getDefaultValue();
    if(variesWasSet_101 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(101))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
        ui->_101->setSpecialValueText("");
        variesWasSet_101 = false;
    }
    else
    {
        if(!ui->_101->specialValueText().isEmpty() && ui->_101->minimum() == v )
            variesWasSet_101 = true;
    }

}

// default style
void LightVerify_Dialog::on__102_valueChanged(int v)
{
    //((IntegerDialogControl*)RCdlg->getcontrol(102))->change(RCdlg,v);
    int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(102))->getDefaultValue();
    if(variesWasSet_102 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(102))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
        ui->_102->setSpecialValueText("");
        variesWasSet_102 = false;
    }
    else
    {
        if(!ui->_102->specialValueText().isEmpty() && ui->_102->minimum() == v )
            variesWasSet_102 = true;
    }

}

// default wieght
void LightVerify_Dialog::on__103_valueChanged(double v)
{
    //((WeightAssocRealDialogControl*)RCdlg->getcontrol(103))->change(RCdlg,v);
    int defVal = ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(103))->getDefaultValue();
    if(variesWasSet_103 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(103))->change(this->RCdlg,defVal);
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
        ui->_103->setSpecialValueText("");
        variesWasSet_103 = false;
    }
    else
    {
        if(!ui->_103->specialValueText().isEmpty() && ui->_103->minimum() == v )
            variesWasSet_103 = true;
    }

}

void LightVerify_Dialog::on__104_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(104))->getDefaultValue();
    if(variesWasSet_104 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(104))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
        ui->_104->setSpecialValueText("");
        variesWasSet_104 = false;
    }
    else
    {
        if(!ui->_104->specialValueText().isEmpty() && ui->_104->minimum() == v )
            variesWasSet_104 = true;
    }
}

void LightVerify_Dialog::on__107_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(107))->getDefaultValue();
    if(variesWasSet_107 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(107))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
        ui->_107->setSpecialValueText("");
        variesWasSet_107 = false;
    }
    else
    {
        if(!ui->_107->specialValueText().isEmpty() && ui->_107->minimum() == v )
            variesWasSet_107 = true;
    }

}

void LightVerify_Dialog::on__109_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(109))->getDefaultValue();
    if(variesWasSet_109 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(109))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(109))->setSpecialValueText(_RCT(""));
        ui->_109->setSpecialValueText("");
        variesWasSet_109 = false;
    }
    else
    {
        if(!ui->_109->specialValueText().isEmpty() && ui->_109->minimum() == v )
            variesWasSet_109 = true;
    }

}

void LightVerify_Dialog::on__110_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(110))->getDefaultValue();
    if(variesWasSet_110 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(110))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(110))->setSpecialValueText(_RCT(""));
        ui->_110->setSpecialValueText("");
        variesWasSet_110 = false;
    }
    else
    {
        if(!ui->_110->specialValueText().isEmpty() && ui->_110->minimum() == v )
            variesWasSet_110 = true;
    }

}

void LightVerify_Dialog::on__112_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(112))->getDefaultValue();
    if(variesWasSet_112 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(112))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(112))->setSpecialValueText(_RCT(""));
        ui->_112->setSpecialValueText("");
        variesWasSet_112 = false;
    }
    else
    {
        if(!ui->_112->specialValueText().isEmpty() && ui->_112->minimum() == v )
            variesWasSet_112 = true;
    }

}

void LightVerify_Dialog::on__113_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(113))->getDefaultValue();
    if(variesWasSet_113 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(113))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(113))->setSpecialValueText(_RCT(""));
        ui->_113->setSpecialValueText("");
        variesWasSet_113 = false;
    }
    else
    {
        if(!ui->_113->specialValueText().isEmpty() && ui->_113->minimum() == v )
            variesWasSet_113 = true;
    }

}

void LightVerify_Dialog::on__105_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 105
    wParam = MAKEWPARAM((WORD)105,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_105;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LightVerify_Dialog::on__106_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 106
    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LightVerify_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 108
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LightVerify_Dialog::on__111_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 111
    wParam = MAKEWPARAM((WORD)111,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_111;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// colour selector button
void LightVerify_Dialog::on__1000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1000
    wParam = MAKEWPARAM((WORD)1000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1000;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// layer selector button
void LightVerify_Dialog::on__1001_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1001
    wParam = MAKEWPARAM((WORD)1001,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1001;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// style selector button
void LightVerify_Dialog::on__1002_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1002
    wParam = MAKEWPARAM((WORD)1002,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1002;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// wieght selector button
void LightVerify_Dialog::on__1003_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1003
    wParam = MAKEWPARAM((WORD)1003,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1003;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void LightVerify_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}

void LightVerify_Dialog::draw()
{
    if((LightVerifyDisplayControl*)RCdlg->getcontrol(2000));
    {
        //((LightVerifyDisplayControl*)RCdlg->getcontrol(2000))->draw(RCdlg,0);
        if(suninfo != 0)
        {
            ui->_2000->setEnabled(true);
            ui->Date->setText(suninfo->Date);
            ui->Time->setText(suninfo->Time);
            ui->TimeZone->setText(suninfo->TimeZone);
            ui->Latitude->setText(suninfo->Latitude);
            ui->Longitude->setText(suninfo->Longitude);
        }
        else
        {
            ui->_2000->setEnabled(false);
            ui->Date->setText("");
            ui->Time->setText("");
            ui->TimeZone->setText("");
            ui->Latitude->setText("");
            ui->Longitude->setText("");
        }
    }
}
