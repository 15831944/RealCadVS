
#include "ncwin.h"
#include "ncdialog.h"
#include "RCDialog.h"
#include "dimension_dialog.h"
#include "ui_dimension_dialog.h"

bool DimensionVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<Dimension_Dialog *>(dialog) != 0)
            {

                if(obj->objectName()== "_2011")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((Dimension_Dialog*)dialog)->ui->_2011->setCurrentIndex(1);
                    ((Dimension_Dialog*)dialog)->ui->_2011->hide();
                    ((Dimension_Dialog*)dialog)->ui->_211->show();
                    ((Dimension_Dialog*)dialog)->ui->_211->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Dimension_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_100)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_100 = false;
                    ((Dimension_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_101)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_101 = false;
                    ((Dimension_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_102)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_102 = false;
                    ((Dimension_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_103)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_103 = false;
                    ((Dimension_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_200")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_200)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_200 = false;
                    ((Dimension_Dialog*)dialog)->ui->_200->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(200))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_200->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_201")
            {
                // only use this once
                if(((Dimension_Dialog*)dialog)->variesWasSet_201)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_201 = false;
                    ((Dimension_Dialog*)dialog)->ui->_201->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(201))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_201->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_202")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_202)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_202 = false;
                    ((Dimension_Dialog*)dialog)->ui->_202->setSpecialValueText("");
                    ((IntegerDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(202))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_202->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_205")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_205)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_205 = false;
                    ((Dimension_Dialog*)dialog)->ui->_205->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(205))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_205->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_212")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_212)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_212 = false;
                    ((Dimension_Dialog*)dialog)->ui->_212->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(212))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_212->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_213")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_213)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_213 = false;
                    ((Dimension_Dialog*)dialog)->ui->_213->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(213))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_213->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_218")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_218)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_218 = false;
                    ((Dimension_Dialog*)dialog)->ui->_218->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(218))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_218->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_222")
            {
                if(((Dimension_Dialog*)dialog)->variesWasSet_222)
                {
                    ((Dimension_Dialog*)dialog)->variesWasSet_222 = false;
                    ((Dimension_Dialog*)dialog)->ui->_222->setSpecialValueText("");
                    ((RealDialogControl*)((Dimension_Dialog*)dialog)->RCdlg->getcontrol(222))->setSpecialValueText(_RCT(""));
                    ((Dimension_Dialog*)dialog)->ui->_222->clear();
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

Dimension_Dialog::Dimension_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Dimension_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Dimension_Dialog::Dimension_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    //QWidget(parent)
    RCDialog(rcparent,parent,f),
    ui(new Ui::Dimension_Dialog)
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
    variesWasSet_200 = false;
    ui->_200->setValue(ui->_200->maximum());
    variesWasSet_201 = false;
    ui->_201->setValue(ui->_201->maximum());
    variesWasSet_202 = false;
    ui->_202->setValue(ui->_202->maximum());
    variesWasSet_205 = false;
    ui->_205->setValue(ui->_205->maximum());
    variesWasSet_212 = false;
    ui->_212->setValue(ui->_212->maximum());
    variesWasSet_213 = false;
    ui->_213->setValue(ui->_213->maximum());
    variesWasSet_218 = false;
    ui->_218->setValue(ui->_218->maximum());
    variesWasSet_222 = false;
    ui->_222->setValue(ui->_222->maximum());

    // get the change settings flags
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    // make the dummy tolerance combo the same size as the real one and hide it
    ui->_2011->setGeometry(ui->_2011->geometry());
    ui->_211->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_214)
        ui->_214->setScene(scene);

    ((DimensionDisplayDialogControl*)RCdlg->getcontrol(214))->setQW(ui->_214);
    ((DimensionDisplayDialogControl*)RCdlg->getcontrol(214))->sethwnd(ui->_214);

    ui->_203->setTristate(change);//
    ui->_204->setTristate(change);//
    ui->_216->setTristate(change);//
    ui->_217->setTristate(change);//
    ui->_219->setTristate(change);//
    ui->_220->setTristate(change);//
    ui->_224->setTristate(change);//
    ui->_225->setTristate(change);//
    ui->_226->setTristate(change);//

    initDialog(this);

    if(change)
    {
        // replace the tolerance combo box with the *Varies* version
        if(changecount > 1 && !changeBits->test(11))
        {
            ui->_211->hide();
            ui->_2011->setVisible(true);
            ui->_2011->show();
        }
        else
        {
            ui->_211->show();
            ui->_2011->hide();
        }
    }
    else
    {
        ui->_2011->hide();
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

    DimensionVerifyEventFilter *Filter = new DimensionVerifyEventFilter(this);
    ui->_100->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);
    ui->_102->installEventFilter(Filter);
    ui->_103->installEventFilter(Filter);
    ui->_200->installEventFilter(Filter);
    ui->_201->installEventFilter(Filter);
    ui->_202->installEventFilter(Filter);
    ui->_205->installEventFilter(Filter);
    ui->_212->installEventFilter(Filter);
    ui->_213->installEventFilter(Filter);
    ui->_218->installEventFilter(Filter);
    ui->_222->installEventFilter(Filter);
    ui->_2011->installEventFilter(Filter);

    QTimer::singleShot(200, this, SLOT(drawSample()));
}

Dimension_Dialog::~Dimension_Dialog()
{
    delete ui;
}

void Dimension_Dialog::on__100_valueChanged(int v)
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

void Dimension_Dialog::on__101_valueChanged(int v)
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

void Dimension_Dialog::on__102_valueChanged(int v)
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

void Dimension_Dialog::on__103_valueChanged(double v)
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

void Dimension_Dialog::on__222_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(222))->getDefaultValue();
    if(variesWasSet_222 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(222))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(222))->setSpecialValueText(_RCT(""));
        ui->_222->setSpecialValueText("");
        variesWasSet_222 = false;
    }
    else
    {
        if(!ui->_222->specialValueText().isEmpty() && ui->_222->minimum() == v )
            variesWasSet_222 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__200_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(200))->getDefaultValue();
    if(variesWasSet_200 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(200))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(200))->setSpecialValueText(_RCT(""));
        ui->_200->setSpecialValueText("");
        variesWasSet_200 = false;
    }
    else
    {
        if(!ui->_200->specialValueText().isEmpty() && ui->_200->minimum() == v )
            variesWasSet_200 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__201_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(201))->getDefaultValue();
    if(variesWasSet_201 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(201))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(201))->setSpecialValueText(_RCT(""));
        ui->_201->setSpecialValueText("");
        variesWasSet_201 = false;
    }
    else
    {
        if(!ui->_201->specialValueText().isEmpty() && ui->_201->minimum() == v )
            variesWasSet_201 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__202_valueChanged(int v)
{
    int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(202))->getDefaultValue();
    if(variesWasSet_202 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(202))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(202))->setSpecialValueText(_RCT(""));
        ui->_202->setSpecialValueText("");
        variesWasSet_202 = false;
    }
    else
    {
        if(!ui->_202->specialValueText().isEmpty() && ui->_202->minimum() == v )
            variesWasSet_202 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__203_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_203->isTristate())
    {
        ui->_203->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(203))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 203
    wParam = MAKEWPARAM((WORD)203,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_203;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__204_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_204->isTristate())
    {
        ui->_204->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(204))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 204
    wParam = MAKEWPARAM((WORD)204,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_204;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__205_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(205))->getDefaultValue();
    if(variesWasSet_205 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(205))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(205))->setSpecialValueText(_RCT(""));
        ui->_205->setSpecialValueText("");
        variesWasSet_205 = false;
    }
    else
    {
        if(!ui->_205->specialValueText().isEmpty() && ui->_205->minimum() == v )
            variesWasSet_205 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__206_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 206
    wParam = MAKEWPARAM((WORD)206,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_206;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__207_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 207
    wParam = MAKEWPARAM((WORD)207,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_207;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}
void Dimension_Dialog::on__208_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 208
    wParam = MAKEWPARAM((WORD)208,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_208;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}
void Dimension_Dialog::on__209_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 209
    wParam = MAKEWPARAM((WORD)209,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_209;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__211_currentIndexChanged(int index)
{
    drawSample();
}

void Dimension_Dialog::on__210_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 210
    wParam = MAKEWPARAM((WORD)210,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_210;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__212_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(212))->getDefaultValue();
    if(variesWasSet_212 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(212))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(212))->setSpecialValueText(_RCT(""));
        ui->_212->setSpecialValueText("");
        variesWasSet_212 = false;
    }
    else
    {
        if(!ui->_212->specialValueText().isEmpty() && ui->_212->minimum() == v )
            variesWasSet_212 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__213_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(213))->getDefaultValue();
    if(variesWasSet_213 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(213))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(213))->setSpecialValueText(_RCT(""));
        ui->_213->setSpecialValueText("");
        variesWasSet_213 = false;
    }
    else
    {
        if(!ui->_213->specialValueText().isEmpty() && ui->_213->minimum() == v )
            variesWasSet_213 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__218_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(218))->getDefaultValue();
    if(variesWasSet_218 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(218))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(218))->setSpecialValueText(_RCT(""));
        ui->_218->setSpecialValueText("");
        variesWasSet_218 = false;
    }
    else
    {
        if(!ui->_218->specialValueText().isEmpty() && ui->_218->minimum() == v )
            variesWasSet_218 = true;
    }

    drawSample();
}

void Dimension_Dialog::on__216_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_216->isTristate())
    {
        ui->_216->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(216))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 216
    wParam = MAKEWPARAM((WORD)216,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_216;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__217_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_217->isTristate())
    {
        ui->_217->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(217))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 217
    wParam = MAKEWPARAM((WORD)217,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_217;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__219_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_219->isTristate())
    {
        ui->_219->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(219))->setTriState(false);
    }    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 219
    wParam = MAKEWPARAM((WORD)219,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_219;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__220_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_220->isTristate())
    {
        ui->_220->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(220))->setTriState(false);
    }    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 220
    wParam = MAKEWPARAM((WORD)220,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_220;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__224_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_224->isTristate())
    {
        ui->_224->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(224))->setTriState(false);
    }    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 224
    wParam = MAKEWPARAM((WORD)224,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_224;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__225_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_225->isTristate())
    {
        ui->_225->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(225))->setTriState(false);
    }    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 225
    wParam = MAKEWPARAM((WORD)225,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_225;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__226_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_226->isTristate())
    {
        ui->_226->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(226))->setTriState(false);
    }    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 226
    wParam = MAKEWPARAM((WORD)226,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_226;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Dimension_Dialog::on__215_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 215
    wParam = MAKEWPARAM((WORD)215,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_215;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone
void Dimension_Dialog::on__221_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 221
    wParam = MAKEWPARAM((WORD)221,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_221;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone all
void Dimension_Dialog::on__223_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 223
    wParam = MAKEWPARAM((WORD)223,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_223;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// modify length
void Dimension_Dialog::on__228_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 228
    wParam = MAKEWPARAM((WORD)228,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_228;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// modify equation
void Dimension_Dialog::on__229_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 229
    wParam = MAKEWPARAM((WORD)229,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_229;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// modify length
void Dimension_Dialog::on__230_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 230
    wParam = MAKEWPARAM((WORD)230,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_230;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// add point button
void Dimension_Dialog::on__231_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 231
    wParam = MAKEWPARAM((WORD)231,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_231;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// delete point button
void Dimension_Dialog::on__232_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 232
    wParam = MAKEWPARAM((WORD)232,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_232;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// move point button
void Dimension_Dialog::on__233_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 233
    wParam = MAKEWPARAM((WORD)233,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_233;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Dimension_Dialog::on__1000_clicked(bool checked)
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

void Dimension_Dialog::on__1001_clicked(bool checked)
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

void Dimension_Dialog::on__1002_clicked(bool checked)
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

void Dimension_Dialog::on__1003_clicked(bool checked)
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

void Dimension_Dialog::on_accept()
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

void Dimension_Dialog::drawSample()
{
    if(isVisible())
    {
        if((DimensionDisplayDialogControl*)RCdlg->getcontrol(214));
            ((DimensionDisplayDialogControl*)RCdlg->getcontrol(214))->draw(RCdlg,0);
    }
}
