
#include "ncwin.h"
#include "RCDialog.h"
#include "leader_dialog.h"
#include "ui_leader_dialog.h"

bool LeaderVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<Leader_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_2033")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((Leader_Dialog*)dialog)->ui->_2033->setCurrentIndex(1);
                    ((Leader_Dialog*)dialog)->ui->_2033->hide();
                    ((Leader_Dialog*)dialog)->ui->_1033->show();
                    ((Leader_Dialog*)dialog)->ui->_1033->setMaxVisibleItems(20);
                    ((Leader_Dialog*)dialog)->ui->_1033->showPopup();
                    return true;
                }
                if(obj->objectName()== "_204")
                {
                    ((Leader_Dialog*)dialog)->ui->_204->hide();
                    ((Leader_Dialog*)dialog)->ui->_204->setCurrentIndex(1);
                    ((Leader_Dialog*)dialog)->ui->_104->show();
                    ((Leader_Dialog*)dialog)->ui->_104->showPopup();
                    return true;
                }
                if(obj->objectName()== "_208")
                {
                    ((Leader_Dialog*)dialog)->ui->_208->hide();
                    ((Leader_Dialog*)dialog)->ui->_208->setCurrentIndex(1);
                    ((Leader_Dialog*)dialog)->ui->_108->show();
                    ((Leader_Dialog*)dialog)->ui->_108->showPopup();
                    return true;
                }
                if(obj->objectName()== "_217")
                {
                    ((Leader_Dialog*)dialog)->ui->_217->hide();
                    ((Leader_Dialog*)dialog)->ui->_217->setCurrentIndex(1);
                    ((Leader_Dialog*)dialog)->ui->_117->show();
                    ((Leader_Dialog*)dialog)->ui->_117->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Leader_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_100)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_100 = false;
                    ((Leader_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_101)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_101 = false;
                    ((Leader_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_102)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_102 = false;
                    ((Leader_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_103)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_103 = false;
                    ((Leader_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_105")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_105)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_105 = false;
                    ((Leader_Dialog*)dialog)->ui->_105->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_105->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_106)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_106 = false;
                    ((Leader_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_106->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_107")
            {
                if(((Leader_Dialog*)dialog)->variesWasSet_107)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_107 = false;
                    ((Leader_Dialog*)dialog)->ui->_107->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_107->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_109")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_109)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_109 = false;
                    ((Leader_Dialog*)dialog)->ui->_109->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(109))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_109->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_118")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_118)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_118 = false;
                    ((Leader_Dialog*)dialog)->ui->_118->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(118))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_118->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_120")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_120)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_120 = false;
                    ((Leader_Dialog*)dialog)->ui->_120->setSpecialValueText("");
                    ((RealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(120))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_120->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_124")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_124)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_124 = false;
                    ((Leader_Dialog*)dialog)->ui->_124->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(124))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_124->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_125")
            {
                // only use this once
                if(((Leader_Dialog*)dialog)->variesWasSet_125)
                {
                    ((Leader_Dialog*)dialog)->variesWasSet_125 = false;
                    ((Leader_Dialog*)dialog)->ui->_125->setSpecialValueText("");
                    ((IntegerDialogControl*)((Leader_Dialog*)dialog)->RCdlg->getcontrol(125))->setSpecialValueText(_RCT(""));
                    ((Leader_Dialog*)dialog)->ui->_125->clear();
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

Leader_Dialog::Leader_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Leader_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Leader_Dialog::Leader_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
  //QWidget(parent)
  RCDialog(rcparent,parent,f),
  ui(new Ui::Leader_Dialog)
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
    variesWasSet_105 = false;
    ui->_105->setValue(ui->_105->maximum());
    variesWasSet_106 = false;
    ui->_106->setValue(ui->_106->maximum());
    variesWasSet_109 = false;
    ui->_106->setValue(ui->_109->maximum());
    variesWasSet_107 = false;
    ui->_107->setValue(ui->_107->maximum());
    variesWasSet_118 = false;
    ui->_118->setValue(ui->_118->maximum());
    variesWasSet_120 = false;
    ui->_120->setValue(ui->_120->maximum());
    variesWasSet_124 = false;
    ui->_124->setValue(ui->_124->maximum());
    variesWasSet_125 = false;
    ui->_125->setValue(ui->_125->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // make the dummy font combo the same size as the real one and hide it
    ui->_2033->setGeometry(ui->_1033->geometry());
    ui->_2033->hide();

    // make the dummy hjust combo the same size as the real one and hide it
    ui->_204->setGeometry(ui->_104->geometry());
    ui->_204->hide();

    // make the dummy vjust combo the same size as the real one and hide it
    ui->_208->setGeometry(ui->_108->geometry());
    ui->_208->hide();

    // make the dummy term style combo the same size as the real one and hide it
    ui->_217->setGeometry(ui->_117->geometry());
    ui->_217->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_113)
        ui->_113->setScene(scene);
    ((LeaderDisplayDialogControl*)RCdlg->getcontrol(113))->setQW(ui->_113);
    ((LeaderDisplayDialogControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);

    ui->_110->setTristate(change);// bold
    ui->_111->setTristate(change);// underline
    ui->_114->setTristate(change);// italic
    ui->_115->setTristate(change);// boxed
    ui->_112->setTristate(change);// outline
    ui->_116->setTristate(change);// halo
    ui->_119->setTristate(change);// use term weight
    ui->_121->setTristate(change);// scaled

    // adjust the font for HDPI scaling
    if(RCGetHDPIYscale() > 1)
    {
        QFont font = ui->_1033->font();
        font.setPixelSize(11);
        ui->_1033->setFont(font);
    }

    initDialog(this);


    if(change)
    {
        // replace the font combo with the *Varies* version
        if(changecount > 1 && !changeBits->test(40))
        {
            ui->_1033->hide();
            ui->_2033->setVisible(true);
            ui->_2033->show();
        }
        else
        {
            ui->_1033->show();
            ui->_2033->hide();
        }
        // replace the hjust combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(44))
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
        // replace the vjust combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(45))
        {
            ui->_108->hide();
            ui->_208->setVisible(true);
            ui->_208->show();
        }
        else
        {
            ui->_108->show();
            ui->_208->hide();
        }
        // replace the term style combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(15))
        {
            ui->_117->hide();
            ui->_217->setVisible(true);
            ui->_217->show();
        }
        else
        {
            ui->_117->show();
            ui->_217->hide();
        }
    }
    else
    {
        ui->_2033->hide();
        ui->_204->hide();
        ui->_208->hide();
        ui->_217->hide();
    }

    if(change && changecount >= 1)
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

    LeaderVerifyEventFilter *Filter = new LeaderVerifyEventFilter(this);
    ui->_2033->installEventFilter(Filter);
    ui->_100->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_103->installEventFilter(Filter);
    ui->_105->installEventFilter(Filter);
    ui->_106->installEventFilter(Filter);
    ui->_107->installEventFilter(Filter);
    ui->_109->installEventFilter(Filter);
    ui->_118->installEventFilter(Filter);
    ui->_120->installEventFilter(Filter);
    ui->_124->installEventFilter(Filter);
    ui->_125->installEventFilter(Filter);
    ui->_217->installEventFilter(Filter);
    ui->_204->installEventFilter(Filter);
    ui->_208->installEventFilter(Filter);

    QTimer::singleShot(200, this, SLOT(drawSample()));
}

Leader_Dialog::~Leader_Dialog()
{
    delete ui;
}

void Leader_Dialog::on__100_valueChanged(int v)
{
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

    drawSample();
}

void Leader_Dialog::on__101_valueChanged(int v)
{
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

    drawSample();
}

void Leader_Dialog::on__102_valueChanged(int v)
{
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

    drawSample();
}

void Leader_Dialog::on__103_valueChanged(double v)
{
    double defVal = ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(103))->getDefaultValue();
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

    drawSample();
}

void Leader_Dialog::on__105_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(105))->getDefaultValue();
    if(variesWasSet_105 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(105))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
        ui->_105->setSpecialValueText("");
        variesWasSet_105 = false;
    }
    else
    {
        if(!ui->_105->specialValueText().isEmpty() && ui->_105->minimum() == v )
            variesWasSet_105 = true;
    }

    drawSample();
}

void Leader_Dialog::on__104_currentIndexChanged(int index)
{
    drawSample();
}

void Leader_Dialog::on__108_currentIndexChanged(int index)
{
    drawSample();
}

void Leader_Dialog::on__106_valueChanged(double v)
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

void Leader_Dialog::on__109_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(109))->getDefaultValue();
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

    drawSample();
}

void Leader_Dialog::on__107_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(107))->getDefaultValue();
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

    drawSample();
}

void Leader_Dialog::on__117_currentIndexChanged(int index)
{
    drawSample();
}

void Leader_Dialog::on__118_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(118))->getDefaultValue();
    if(variesWasSet_118 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(118))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(118))->setSpecialValueText(_RCT(""));
        ui->_118->setSpecialValueText("");
        variesWasSet_118 = false;
    }
    else
    {
        if(!ui->_118->specialValueText().isEmpty() && ui->_118->minimum() == v )
            variesWasSet_118 = true;
    }

    drawSample();
}

void Leader_Dialog::on__120_valueChanged(double v)
{
    int defVal = ((RealDialogControl*)this->RCdlg->getcontrol(120))->getDefaultValue();
    if(variesWasSet_120 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(120))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(120))->setSpecialValueText(_RCT(""));
        ui->_120->setSpecialValueText("");
        variesWasSet_120 = false;
    }
    else
    {
        if(!ui->_120->specialValueText().isEmpty() && ui->_120->minimum() == v )
            variesWasSet_120 = true;
    }

    drawSample();
}

void Leader_Dialog::on__124_valueChanged(double v)
{
    int defVal = ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(124))->getDefaultValue();
    if(variesWasSet_124 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(124))->change(this->RCdlg,defVal);
        ((WeightAssocRealDialogControl*)this->RCdlg->getcontrol(124))->setSpecialValueText(_RCT(""));
        ui->_124->setSpecialValueText("");
        variesWasSet_124 = false;
    }
    else
    {
        if(!ui->_124->specialValueText().isEmpty() && ui->_124->minimum() == v )
            variesWasSet_124 = true;
    }

    drawSample();
}

void Leader_Dialog::on__125_valueChanged(int v)
{
    int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(125))->getDefaultValue();
    if(variesWasSet_125 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(125))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(125))->setSpecialValueText(_RCT(""));
        ui->_125->setSpecialValueText("");
        variesWasSet_125 = false;
    }
    else
    {
        if(!ui->_125->specialValueText().isEmpty() && ui->_125->minimum() == v )
            variesWasSet_125 = true;
    }

    drawSample();
}

void Leader_Dialog::on__1033_currentFontChanged(const QFont & font)
{
    QString defFont(((FontListDialogControl*)this->RCdlg->getcontrol(1033))->getDefaultFont());
    ((FontListDialogControl*)this->RCdlg->getcontrol(1033))->font = font.family();

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)1033,(WORD)LBN_SELCHANGE);
    lParam = (LPARAM)ui->_1033;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// bold
void Leader_Dialog::on__110_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_110->isTristate())
    {
        ui->_110->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(110))->setTriState(false);
    }
    // using the dialog callback function
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

// underline
void Leader_Dialog::on__111_clicked(bool checked)
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

    drawSample();
}

// italic
void Leader_Dialog::on__114_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 114
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// boxed
void Leader_Dialog::on__115_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 115
    wParam = MAKEWPARAM((WORD)115,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_115;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// outline
void Leader_Dialog::on__112_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 112
    wParam = MAKEWPARAM((WORD)112,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_112;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// halo
void Leader_Dialog::on__116_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 116
    wParam = MAKEWPARAM((WORD)116,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_116;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// scale text
void Leader_Dialog::on__121_clicked(bool checked)
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

// specify terminator weight
void Leader_Dialog::on__119_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_119->isTristate())
    {
        ui->_119->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(119))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 119
    wParam = MAKEWPARAM((WORD)119,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_119;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// clone button
void Leader_Dialog::on__122_clicked(bool checked)
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

    drawSample();
}

// clone all button
void Leader_Dialog::on__123_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 123
    wParam = MAKEWPARAM((WORD)123,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_123;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Leader_Dialog::on__1000_clicked(bool checked)
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

    drawSample();

}

void Leader_Dialog::on__1001_clicked(bool checked)
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

    drawSample();
}

void Leader_Dialog::on__1002_clicked(bool checked)
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

    drawSample();
}

void Leader_Dialog::on__1003_clicked(bool checked)
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

    drawSample();
}

void Leader_Dialog::on__1004_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1004
    wParam = MAKEWPARAM((WORD)1004,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1004;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Leader_Dialog::on__1005_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1005
    wParam = MAKEWPARAM((WORD)1005,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1005;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Leader_Dialog::on_accept()
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

void Leader_Dialog::drawSample()
{
    if(isVisible())
    {
        if((LeaderDisplayDialogControl*)RCdlg->getcontrol(113));
            ((LeaderDisplayDialogControl*)RCdlg->getcontrol(113))->draw(RCdlg,0);
    }
}
