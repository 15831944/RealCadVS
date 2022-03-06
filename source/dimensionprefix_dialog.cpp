
#include "ncwin.h"
#include "RCDialog.h"
#include "dimensionprefix_dialog.h"
#include "ui_dimensionprefix_dialog.h"

bool DimPrefixTextEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<DimensionPrefix_Dialog *>(dialog) != 0)
            {

                if(obj->objectName()== "_1000")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1000->setCurrentIndex(1);
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1000->hide();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_100->show();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_100->setMaxVisibleItems(20);
                    ((DimensionPrefix_Dialog*)dialog)->ui->_100->showPopup();
                    return true;
                }
                if(obj->objectName()== "_1004")
                {
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1004->hide();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1004->setCurrentIndex(1);
                    ((DimensionPrefix_Dialog*)dialog)->ui->_104->show();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_104->showPopup();
                    return true;
                }
                if(obj->objectName()== "_1005")
                {
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1005->hide();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_1005->setCurrentIndex(1);
                    ((DimensionPrefix_Dialog*)dialog)->ui->_105->show();
                    ((DimensionPrefix_Dialog*)dialog)->ui->_105->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<DimensionPrefix_Dialog *>(dialog) != 0)
        {

            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((DimensionPrefix_Dialog*)dialog)->variesWasSet_101)
                {
                    ((DimensionPrefix_Dialog*)dialog)->variesWasSet_101 = false;
                    ((DimensionPrefix_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionPrefix_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((DimensionPrefix_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((DimensionPrefix_Dialog*)dialog)->variesWasSet_102)
                {
                    ((DimensionPrefix_Dialog*)dialog)->variesWasSet_102 = false;
                    ((DimensionPrefix_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionPrefix_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((DimensionPrefix_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((DimensionPrefix_Dialog*)dialog)->variesWasSet_103)
                {
                    ((DimensionPrefix_Dialog*)dialog)->variesWasSet_103 = false;
                    ((DimensionPrefix_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionPrefix_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((DimensionPrefix_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((DimensionPrefix_Dialog*)dialog)->variesWasSet_106)
                {
                    ((DimensionPrefix_Dialog*)dialog)->variesWasSet_106 = false;
                    ((DimensionPrefix_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((DimensionPrefix_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    ((DimensionPrefix_Dialog*)dialog)->ui->_106->clear();
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

DimensionPrefix_Dialog::DimensionPrefix_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DimensionPrefix_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DimensionPrefix_Dialog::DimensionPrefix_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::DimensionPrefix_Dialog)
{
    ui->setupUi(this);

    variesWasSet_101 = false;
    ui->_101->setValue(ui->_101->maximum());
    variesWasSet_102 = false;
    ui->_102->setValue(ui->_102->maximum());
    variesWasSet_103 = false;
    ui->_103->setValue(ui->_103->maximum());
    variesWasSet_106 = false;
    ui->_106->setValue(ui->_106->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // make the dummy font combo the same size as the real one and hide it
    ui->_1000->setGeometry(ui->_100->geometry());
    ui->_1000->hide();

    // make the dummy hjust combo the same size as the real one and hide it
    ui->_1004->setGeometry(ui->_104->geometry());
    ui->_1004->hide();

    // make the dummy vjust combo the same size as the real one and hide it
    ui->_1005->setGeometry(ui->_105->geometry());
    ui->_1005->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_111)
        ui->_111->setScene(scene);

    if(dynamic_cast<TTextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        ((TTextDisplayDialogControl*)RCdlg->getcontrol(111))->setQW(ui->_111);
        ((TTextDisplayDialogControl*)RCdlg->getcontrol(111))->sethwnd(ui->_111);
    }
    else
    if(dynamic_cast<PTextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        ((PTextDisplayDialogControl*)RCdlg->getcontrol(111))->setQW(ui->_111);
        ((PTextDisplayDialogControl*)RCdlg->getcontrol(111))->sethwnd(ui->_111);
    }
    else
    if(dynamic_cast<STextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        ((STextDisplayDialogControl*)RCdlg->getcontrol(111))->setQW(ui->_111);
        ((STextDisplayDialogControl*)RCdlg->getcontrol(111))->sethwnd(ui->_111);
    }

    ui->_107->setTristate(change);// bold
    ui->_108->setTristate(change);// underline
    ui->_109->setTristate(change);// italic
    ui->_110->setTristate(change);// boxed

    // adjust the font for HDPI scaling
    if(RCGetHDPIYscale() > 1)
    {
        QFont font = ui->_100->font();
        font.setPixelSize(11);
        ui->_100->setFont(font);
    }

    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    if(change)
    {
        // replace the font combo with the *Varies* version
        if(changecount > 1 && !changeBits->test(80))
        {
            ui->_100->hide();
            ui->_1000->setVisible(true);
            ui->_1000->show();
        }
        else
        {
            ui->_100->show();
            ui->_1000->hide();
        }
        // replace the hjust combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(84))
        {
            ui->_104->hide();
            ui->_1004->setVisible(true);
            ui->_1004->show();
        }
        else
        {
            ui->_104->show();
            ui->_1004->hide();
        }
        // replace the vjust combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(85))
        {
            ui->_105->hide();
            ui->_1005->setVisible(true);
            ui->_1005->show();
        }
        else
        {
            ui->_105->show();
            ui->_1005->hide();
        }
    }
    else
    {
        ui->_1000->hide();
        ui->_1004->hide();
        ui->_1005->hide();
    }

    DimPrefixTextEventFilter *Filter = new DimPrefixTextEventFilter(this);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_103->installEventFilter(Filter);
    ui->_103->installEventFilter(Filter);
    ui->_1000->installEventFilter(Filter);
    ui->_1004->installEventFilter(Filter);
    ui->_1005->installEventFilter(Filter);

}


DimensionPrefix_Dialog::~DimensionPrefix_Dialog()
{
    delete ui;
}

void DimensionPrefix_Dialog::on__100_currentFontChanged(const QFont & font)
{
    QString defFont(((FontListDialogControl*)this->RCdlg->getcontrol(100))->getDefaultFont());
    ((FontListDialogControl*)this->RCdlg->getcontrol(100))->font = font.family();

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)100,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionPrefix_Dialog::on__101_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(101))->getDefaultValue();
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

void DimensionPrefix_Dialog::on__102_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(102))->getDefaultValue();
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

void DimensionPrefix_Dialog::on__103_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(103))->getDefaultValue();
    if(variesWasSet_103 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(103))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
        ui->_103->setSpecialValueText("");
        variesWasSet_103 = false;
    }
    else
    {
        if(!ui->_103->specialValueText().isEmpty() && ui->_103->minimum() == v )
            variesWasSet_103 = true;
    }

    drawSample();
}

void DimensionPrefix_Dialog::on__104_currentIndexChanged(int index)
{
    drawSample();
}

void DimensionPrefix_Dialog::on__105_currentIndexChanged(int index)
{
     drawSample();
}

void DimensionPrefix_Dialog::on__106_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(106))->getDefaultValue();
    if(variesWasSet_106 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
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

void DimensionPrefix_Dialog::on__107_clicked(bool checked)
{
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

void DimensionPrefix_Dialog::on__108_clicked(bool checked)
{
    if(ui->_108->isTristate())
    {
        ui->_108->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(108))->setTriState(false);
    }
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 108
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionPrefix_Dialog::on__109_clicked(bool checked)
{
    if(ui->_109->isTristate())
    {
        ui->_109->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(109))->setTriState(false);
    }
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 109
    wParam = MAKEWPARAM((WORD)109,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_109;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionPrefix_Dialog::on__110_clicked(bool checked)
{
    if(ui->_110->isTristate())
    {
        ui->_110->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(110))->setTriState(false);
    }
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 110
    wParam = MAKEWPARAM((WORD)110,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_110;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void DimensionPrefix_Dialog::on_accept()
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

void DimensionPrefix_Dialog::drawSample()
{
    if(dynamic_cast<TTextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        if((TTextDisplayDialogControl*)RCdlg->getcontrol(111));
            ((TTextDisplayDialogControl*)RCdlg->getcontrol(111))->draw(RCdlg,0);
    }
    else
    if(dynamic_cast<PTextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        if((PTextDisplayDialogControl*)RCdlg->getcontrol(111));
            ((PTextDisplayDialogControl*)RCdlg->getcontrol(111))->draw(RCdlg,0);
    }
    else
    if(dynamic_cast<STextDisplayDialogControl *>(RCdlg->getcontrol(111)) != 0)
    {
        if((STextDisplayDialogControl*)RCdlg->getcontrol(111));
            ((STextDisplayDialogControl*)RCdlg->getcontrol(111))->draw(RCdlg,0);
    }
}
