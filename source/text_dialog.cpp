
#include "ncwin.h"
#include "ncdialog.h""

#include "RCDialog.h"
#include "text_dialog.h"
#include "ui_text_dialog.h"

bool TextDialogEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(obj->objectName()== "_1000")
            {
                if(dialog)
                {
                    if(qobject_cast<Text_Dialog *>(dialog) != 0)
                    {
                        ((Text_Dialog*)dialog)->ui->_1000->setCurrentIndex(1);
                        ((Text_Dialog*)dialog)->ui->_1000->hide();
                        ((Text_Dialog*)dialog)->ui->_100->show();
                        ((Text_Dialog*)dialog)->ui->_100->showPopup();
                        ((Text_Dialog*)dialog)->ui->_1011->show();
                        return true;
                    }
                }
            }
        }
        return false;
    }
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Text_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_101")
            {
                ((Text_Dialog*)dialog)->variesWasSet_101 = false;
                ((Text_Dialog*)dialog)->ui->_1001->show();
                return false;
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


Text_Dialog::Text_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Text_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Text_Dialog::Text_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Text_Dialog)
{
    ui->setupUi(this);
	ActivateParent = false;
    ResourceString r1(4110),rs2(4107);
    defrs = QString(r1.gets());
    varrs = QString(rs2.gets());

    variesWasSet_101 = false;
    ui->_1001->hide();
    variesWasSet_102 = false;
    ui->_1002->hide();
    variesWasSet_103 = false;
    ui->_1003->hide();
    variesWasSet_106 = false;
    ui->_1006->hide();
    variesWasSet_116 = false;
    ui->_1004->hide();
    ui->_1005->hide();
    ui->_1116->hide();
    ui->_1011->hide();

    // make the dummy font combo the same size as the real one and hide it
    ui->_1000->setGeometry(ui->_100->geometry());
    ui->_1000->hide();

    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_113)
        ui->_113->setScene(scene);


    ((TextDisplayDialogControl*)RCdlg->getcontrol(113))->setQW(ui->_113);
    ((TextDisplayDialogControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);

    // show/hide the confirm check boxes depending on the change setting
    changecount = v.getinteger("op::changecount");
    change = (bool)v.getinteger("op::change");
    changeBits = v.getbm("op::changemask");

    ui->_107->setTristate(change);// bold
    ui->_108->setTristate(change);// underline
    ui->_109->setTristate(change);// italic
    ui->_110->setTristate(change);// boxed
    ui->_111->setTristate(change);// outline
    ui->_112->setTristate(change);// scaled
    ui->_115->setTristate(change);// halo
    ui->_117->setTristate(change);// text hieght in points

    // init the realCAD Dialog class here
    initDialog(this);

    if(change)
    {
        // replace the font combo with the *Varies* version
        if(changecount > 1 && !changeBits->test(4))
        {
            ui->_100->hide();
            ui->_1000->show();
        }
        if(changecount > 1 && !changeBits->test(22))
            variesWasSet_116 = true;
        //if(ui->_104->count() == 4)
        //    ui->_104->setCurrentIndex(0);
        //if(ui->_105->count() == 4)
        //    ui->_105->setCurrentIndex(0);
    }

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    TextDialogEventFilter *Filter = new TextDialogEventFilter(this);
    ui->_1000->installEventFilter(Filter);
    ui->_101->installEventFilter(Filter);

    drawSample();

}

Text_Dialog::~Text_Dialog()
{
    delete ui;
}

// text height
void Text_Dialog::on__101_valueChanged(double val)
{
    //bool change = (bool)v.getinteger("op::change");
    //if(change)
    //{
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(101))->getDefaultValue();
        //QString toolTip = QString("Default Value %1").arg(defVal,0,'f',db.getnumericdisplaytolerance());
        if(variesWasSet_101 == true)
        {
            ((RealDialogControl*)this->RCdlg->getcontrol(101))->change(this->RCdlg,defVal);

            //ui->_101->setToolTip("");
            //ui->_1001->setToolTip(toolTip);
            ui->_1001->show();
            variesWasSet_101 = false;
        }
        else
        {
            if(!ui->_101->specialValueText().isEmpty() && ui->_101->minimum() == val )
            {
                //ui->_101->setToolTip(toolTip);
                ui->_1001->hide();
                variesWasSet_101 = true;
            }
        }
    //}
    drawSample();
}

// text width
void Text_Dialog::on__102_valueChanged(double v)
{
    //bool variesWasSet_102 = false;
    if(variesWasSet_102 == true)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(102))->getDefaultValue();
        ((RealDialogControl*)this->RCdlg->getcontrol(102))->change(this->RCdlg,defVal);
        ui->_1002->show();
        variesWasSet_102 = false;
    }
    else
    {
        if(!ui->_102->specialValueText().isEmpty() && ui->_102->minimum() == v )
        {
            variesWasSet_102 = true;
            ui->_1002->hide();
        }
        //((RealDialogControl*)this->RCdlg->getcontrol(102))->change(this->RCdlg,v);
    }
    drawSample();
}

// text angle
void Text_Dialog::on__103_valueChanged(double v)
{
    //bool variesWasSet_103 = false;
    if(variesWasSet_103 == true)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(103))->getDefaultValue();
        ((RealDialogControl*)this->RCdlg->getcontrol(103))->change(this->RCdlg,defVal);
        ui->_1003->show();
        variesWasSet_103=false;
    }
    else
    {
        if(!ui->_103->specialValueText().isEmpty() && ui->_103->minimum() == v )
        {
            variesWasSet_103 = true;
            ui->_1003->hide();
        }
        //((RealDialogControl*)this->RCdlg->getcontrol(103))->change(this->RCdlg,v);
    }
    drawSample();
}

// horizontal justification
void Text_Dialog::on__104_currentIndexChanged(int index)
{
    if(ui->_104->count() == 4)
    {
        if(index != 0)
            ui->_1004->show();
        else
            ui->_1004->hide();
    }
    drawSample();
}

// vertical justification
void Text_Dialog::on__105_currentIndexChanged(int index)
{
    if(ui->_105->count() == 4)
    {
        if(index != 0)
            ui->_1005->show();
        else
            ui->_1005->hide();
    }
    drawSample();
}

// vertical spacing
void Text_Dialog::on__106_valueChanged(double v)
{
    //bool variesWasSet_106 = false;
    if(variesWasSet_106)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(106))->getDefaultValue();
        ((RealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,defVal);
        ui->_1006->show();
        variesWasSet_106=false;
    }
    else
    {
        if(!ui->_106->specialValueText().isEmpty() && ui->_106->minimum() == v )
        {
            variesWasSet_106 = true;
            ui->_1006->hide();
        }
        //((RealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,v);
    }
    drawSample();

}

void Text_Dialog::on__107_clicked(bool checked)
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

void Text_Dialog::on__108_clicked(bool checked)
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
    drawSample();
}

void Text_Dialog::on__109_clicked(bool checked)
{
    // using the dialog callback function
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

void Text_Dialog::on__110_clicked(bool checked)
{
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

void Text_Dialog::on__111_clicked(bool checked)
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

void Text_Dialog::on__112_clicked(bool checked)
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

// clone button
void Text_Dialog::on__114_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 115
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
    drawSample();
}

void Text_Dialog::on__115_clicked(bool checked)
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

// halo colour
void Text_Dialog::on__116_valueChanged(int val)
{
    if(change)
    {
        if(changecount > 1 && !changeBits->test(22))
        {
            int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(116))->getDefaultValue();
            //QString toolTip = QString(defrs).arg(defVal);
            if(variesWasSet_116 == true) // first time or undo
            {
                //ui->_116->setToolTip(toolTip);
                ui->_1116->hide();
                variesWasSet_116=false;
                ((IntegerDialogControl*)this->RCdlg->getcontrol(116))->change(this->RCdlg,defVal);
            }
            else
            {
                //ui->_116->setToolTip("");
                //ui->_1116->setToolTip(toolTip);
                ui->_1116->show();
                ui->_116->setSpecialValueText("");
                ((IntegerDialogControl*)this->RCdlg->getcontrol(116))->change(this->RCdlg,val);
            }
        }
    }
    drawSample();
}

void Text_Dialog::on__117_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 117
    wParam = MAKEWPARAM((WORD)117,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_117;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// clone all button
void Text_Dialog::on__118_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 117
    wParam = MAKEWPARAM((WORD)118,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_118;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// colour change dialog
void Text_Dialog::on__2000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 117
    wParam = MAKEWPARAM((WORD)2000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_2000;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}


void Text_Dialog::on__1000_activated(const QString & text)
{
    //qDebug() << "current text : " << text;
    //RCCHAR defFont[300];
    //strcpy(defFont,((FontListDialogControl*)this->RCdlg->getcontrol(100))->getDefaultFont());
    //ui->_100->setCurrentText(QString(&defFont[0]));
    //ui->_1000->hide();
    //ui->_100->show();
}

void Text_Dialog::on__1000_currentTextChanged(const QString & text)
{
    //qDebug() << "current text : " << text;
}

void Text_Dialog::on__1000_currentIndexChanged(int index)
{
    //qDebug() << "current index : " << index;
    if(index == 1)
    {
        // using the dialog callback function
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;

        msg = WM_COMMAND;

        wParam = MAKEWPARAM((WORD)1000,(WORD)LBN_SELCHANGE);
        lParam = (LPARAM)ui->_1000;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(index == 2)
    {
        RCCHAR defFont[300];
        strcpy(defFont,((FontListDialogControl*)this->RCdlg->getcontrol(100))->getDefaultFont());
        ui->_100->setCurrentText(QString(&defFont[0]));
        ui->_1000->hide();
        ui->_100->show();
    }
}

void Text_Dialog::on__100_activated(const QString & text)
{
    QString defFont(((FontListDialogControl*)this->RCdlg->getcontrol(100))->getDefaultFont());
    //qDebug() << "current text : " << ui->_100->currentText();
    ((FontListDialogControl*)this->RCdlg->getcontrol(100))->font = defFont;
    //qDebug() << "current font : " << ((FontListDialogControl*)this->RCdlg->getcontrol(100))->font;
    if(text == defFont)
    {
        // hide the undo button
        ui->_1011->hide();
        // hide the real font combo
        ui->_100->hide();
        // show the dummy font combo
        ui->_1000->show();
        ui->_1000->setCurrentIndex(0);
        drawSample();
    }
}

void Text_Dialog::on__100_currentFontChanged(const QFont & font)
{
    QString defFont(((FontListDialogControl*)this->RCdlg->getcontrol(100))->getDefaultFont());
    //qDebug() << "current text : " << ui->_100->currentText();
    ((FontListDialogControl*)this->RCdlg->getcontrol(100))->font = font.family();
    //qDebug() << "current font : " << ((FontListDialogControl*)this->RCdlg->getcontrol(100))->font;
    if(font.family() == defFont)
    {
        // hide the undo button
        ui->_1011->hide();
        // hide the real font combo
        ui->_100->hide();
        // show the dummy font combo
        ui->_1000->show();
        ui->_1000->setCurrentIndex(0);
    }
    else
    {

        // using the dialog callback function
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;

        msg = WM_COMMAND;

        wParam = MAKEWPARAM((WORD)100,(WORD)LBN_SELCHANGE);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
    drawSample();
}

/*
void Text_Dialog::on__100_currentIndexChanged(int index)
{
    if(ui->_100->itemText(index) == "*Varies*")
        ui->_100->removeItem(index);
}
*/

// font combo undo button
void Text_Dialog::on__1011_clicked(bool checked)
{
    // hide this button
    ui->_1011->hide();
    // hide the real font combo box
    ui->_100->hide();
    // show the dummy font combo box
    ui->_1000->setCurrentIndex(0);
    ui->_1000->show();
    // redraw the preview control
    ((TextDisplayDialogControl*)this->RCdlg->getcontrol(113))->draw(this->RCdlg,0);
}

// text height undo button
void Text_Dialog::on__1001_clicked(bool checked)
{
    ui->_101->setValue(ui->_101->minimum());
}

void Text_Dialog::on__1002_clicked(bool checked)
{
    // text width undo button
    ui->_102->setValue(ui->_102->minimum());
}

void Text_Dialog::on__1003_clicked(bool checked)
{
    // text angle undo button
    ui->_103->setValue(ui->_103->minimum());
}

// horizontal justification undo button
void Text_Dialog::on__1004_clicked(bool checked)
{
    if(ui->_104->count() == 4)
    {
        ui->_104->setCurrentIndex(0);
        ui->_1004->hide();
    }
}

// vertical justification undo button
void Text_Dialog::on__1005_clicked(bool checked)
{
    if(ui->_105->count() == 4)
    {
        ui->_105->setCurrentIndex(0);
        ui->_1005->hide();
    }
}

void Text_Dialog::on__1006_clicked(bool checked)
{
    // vertical spacing undo button
    ui->_106->setValue(ui->_106->minimum());
}

// halo colour undo button
void Text_Dialog::on__1116_clicked(bool checked)
{
    ui->_1116->hide();
    ui->_116->setSpecialValueText(varrs);
    variesWasSet_116=true;
    ui->_116->setValue(ui->_116->minimum());
    variesWasSet_116=false;
}

void Text_Dialog::on_accept()
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

void Text_Dialog::drawSample()
{
    if((TextDisplayDialogControl*)RCdlg->getcontrol(113));
        ((TextDisplayDialogControl*)RCdlg->getcontrol(113))->draw(RCdlg,0);
}
