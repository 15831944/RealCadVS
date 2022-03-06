
#include "ncwin.h"

#include "RCDialog.h"
#include "textverify_dialog.h"
#include "ui_textverify_dialog.h"

bool TextVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<TextVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1005")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((TextVerify_Dialog*)dialog)->ui->_1005->setCurrentIndex(1);
                    ((TextVerify_Dialog*)dialog)->ui->_1005->hide();
                    ((TextVerify_Dialog*)dialog)->ui->_105->show();
                    ((TextVerify_Dialog*)dialog)->ui->_105->showPopup();
                    ((TextVerify_Dialog*)dialog)->ui->_105->setMaxVisibleItems(20);
                    return true;
                }
                if(obj->objectName()== "_1009")
                {
                    ((TextVerify_Dialog*)dialog)->ui->_1009->hide();
                    ((TextVerify_Dialog*)dialog)->ui->_1009->setCurrentIndex(1);
                    ((TextVerify_Dialog*)dialog)->ui->_109->show();
                    ((TextVerify_Dialog*)dialog)->ui->_109->showPopup();
                    return true;
                }
                if(obj->objectName()== "_1010")
                {
                    ((TextVerify_Dialog*)dialog)->ui->_1010->hide();
                    ((TextVerify_Dialog*)dialog)->ui->_1010->setCurrentIndex(1);
                    ((TextVerify_Dialog*)dialog)->ui->_110->show();
                    ((TextVerify_Dialog*)dialog)->ui->_110->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<TextVerify_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_100)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_100 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_101)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_101 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_102)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_102 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_103)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_103 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_106)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_106 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_106->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_107")
            {
                if(((TextVerify_Dialog*)dialog)->variesWasSet_107)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_107 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_107->setSpecialValueText("");
                    ((RealDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_107->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_108")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_108)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_108 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_108->setSpecialValueText("");
                    ((RealDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(108))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_108->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_111")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_111)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_111 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_111->setSpecialValueText("");
                    ((RealDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(111))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_111->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_121")
            {
                // only use this once
                if(((TextVerify_Dialog*)dialog)->variesWasSet_121)
                {
                    ((TextVerify_Dialog*)dialog)->variesWasSet_121 = false;
                    ((TextVerify_Dialog*)dialog)->ui->_121->setSpecialValueText("");
                    ((IntegerDialogControl*)((TextVerify_Dialog*)dialog)->RCdlg->getcontrol(121))->setSpecialValueText(_RCT(""));
                    ((TextVerify_Dialog*)dialog)->ui->_121->clear();
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


TextVerify_Dialog::TextVerify_Dialog(HANDLE parent, HANDLE rcparent, int f):
	TextVerify_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
TextVerify_Dialog::TextVerify_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::TextVerify_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  variesWasSet_100 = false;
  ui->_100->setValue(ui->_100->maximum());
  variesWasSet_101 = false;
  ui->_101->setValue(ui->_101->maximum());
  variesWasSet_102 = false;
  ui->_102->setValue(ui->_102->maximum());
  variesWasSet_103 = false;
  ui->_103->setValue(ui->_103->maximum());
  variesWasSet_106 = false;
  ui->_106->setValue(ui->_106->maximum());
  variesWasSet_107 = false;
  ui->_107->setValue(ui->_107->maximum());
  variesWasSet_108 = false;
  ui->_108->setValue(ui->_108->maximum());
  variesWasSet_111 = false;
  ui->_111->setValue(ui->_111->maximum());
  variesWasSet_121 = false;
  ui->_121->setValue(ui->_121->maximum());

  // get the change settings flags
  changecount = v.getinteger("op::changecount");
  change = (bool)v.getinteger("op::change");
  changeBits = v.getbm("op::changemask");

  // make the dummy font combo the same size as the real one and hide it
  ui->_1005->setGeometry(ui->_105->geometry());
  ui->_1005->hide();

  // make the dummy hjust combo the same size as the real one and hide it
  ui->_1009->setGeometry(ui->_109->geometry());
  ui->_1009->hide();

  // make the dummy vjust combo the same size as the real one and hide it
  ui->_1010->setGeometry(ui->_110->geometry());
  ui->_1010->hide();

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_124)
      ui->_124->setScene(scene);
  ((TextDisplayDialogControl*)RCdlg->getcontrol(124))->setQW(ui->_124);
  ((TextDisplayDialogControl*)RCdlg->getcontrol(124))->sethwnd(ui->_124);

  // get the scene for the halo colour sample
  scene = new QGraphicsScene;
  if(ui->_125)
      ui->_125->setScene(scene);
  ((TextDisplayDialogControl*)RCdlg->getcontrol(125))->setQW(ui->_125);
  ((TextDisplayDialogControl*)RCdlg->getcontrol(125))->sethwnd(ui->_125);

  ui->_112->setTristate(change);// bold
  ui->_113->setTristate(change);// underline
  ui->_114->setTristate(change);// italic
  ui->_115->setTristate(change);// boxed
  ui->_116->setTristate(change);// outline
  ui->_120->setTristate(change);// scaled
  ui->_122->setTristate(change);// halo
  ui->_123->setTristate(change);// text hieght in points

  // init the realCAD Dialog class here
  initDialog(this);

  if(change)
  {
      // replace the font combo with the *Varies* version
      if(changecount > 1 && !changeBits->test(4))
      {
          ui->_105->hide();
          ui->_1005->show();
      }
      // replace the hjust combo box with the *Varies* version
      if(changecount > 1 && !changeBits->test(11))
      {
          ui->_109->hide();
          ui->_1009->show();
      }
      // replace the vjust combo box with the *Varies* version
      if(changecount > 1 && !changeBits->test(12))
      {
          ui->_110->hide();
          ui->_1010->show();
      }
      if(changecount > 1 && !changeBits->test(22))
          variesWasSet_121 = true;
      //if(ui->_104->count() == 4)
      //    ui->_104->setCurrentIndex(0);
      //if(ui->_105->count() == 4)
      //    ui->_105->setCurrentIndex(0);
  }

  // if in text verify mode enable the text box
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
      // enable the text box
      ui->_104->setDisabled(false);
      ui->_104->selectAll();
      ui->_104->setFocus();
      // enable the prompt text if this is a label
      if(v.getinteger("tx::label"))
      {
          // enable the label prompt text box
          ui->_119->setDisabled(false);
          ui->label_119->setDisabled(false);
      }
      else
      {
          // disable the label prompt text box
          ui->_119->setDisabled(true);
          ui->label_119->setDisabled(true);
      }
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
      // disable the text box
      ui->_104->setDisabled(true);
      ui->_119->setDisabled(true);
      ui->label_119->setDisabled(true);
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
      // disable the text box
      ui->_104->setDisabled(true);
      ui->_119->setDisabled(true);
      ui->label_119->setDisabled(true);
  }

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  TextVerifyEventFilter *Filter = new TextVerifyEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);
  ui->_107->installEventFilter(Filter);
  ui->_108->installEventFilter(Filter);
  ui->_111->installEventFilter(Filter);
  ui->_121->installEventFilter(Filter);
  ui->_1009->installEventFilter(Filter);
  ui->_1010->installEventFilter(Filter);
  ui->_1005->installEventFilter(Filter);

  // adjust the font for HDPI scaling
  if(RCGetHDPIYscale() > 1)
  {
      QFont font = ui->_105->font();
      font.setPixelSize(11);
      ui->_105->setFont(font);
  }

  drawSample();
}

TextVerify_Dialog::~TextVerify_Dialog()
{
  delete ui;
}

// default colour
void TextVerify_Dialog::on__100_valueChanged(int v)
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

    drawSample();
}

// default layer
void TextVerify_Dialog::on__101_valueChanged(int v)
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

    drawSample();
}

// default style
void TextVerify_Dialog::on__102_valueChanged(int v)
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

    drawSample();
}

// default weight
void TextVerify_Dialog::on__103_valueChanged(double v)
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

    drawSample();
}

// text font combo box
void TextVerify_Dialog::on__105_currentFontChanged(const QFont & font)
{
    QString defFont(((FontListDialogControl*)this->RCdlg->getcontrol(105))->getDefaultFont());
    ((FontListDialogControl*)this->RCdlg->getcontrol(105))->font = font.family();

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

// text hieght value
void TextVerify_Dialog::on__106_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(106))->change(RCdlg,v);
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(106))->getDefaultValue();
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

// text width value
void TextVerify_Dialog::on__107_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(107))->change(RCdlg,v);
    if(variesWasSet_107 == true)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(107))->getDefaultValue();
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

// text angle value
void TextVerify_Dialog::on__108_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(108))->change(RCdlg,v);
    if(variesWasSet_108 == true)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(108))->getDefaultValue();
        ((RealDialogControl*)this->RCdlg->getcontrol(108))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(108))->setSpecialValueText(_RCT(""));
        ui->_108->setSpecialValueText("");
        variesWasSet_108=false;
    }
    else
    {
        if(!ui->_108->specialValueText().isEmpty() && ui->_108->minimum() == v )
            variesWasSet_108 = true;
    }

    drawSample();
}

// vertical spacing value
void TextVerify_Dialog::on__111_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(111))->change(RCdlg,v);
    if(variesWasSet_111)
    {
        double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(111))->getDefaultValue();
        ((RealDialogControl*)this->RCdlg->getcontrol(111))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(111))->setSpecialValueText(_RCT(""));
        ui->_111->setSpecialValueText("");
        variesWasSet_111=false;
    }
    else
    {
        if(!ui->_111->specialValueText().isEmpty() && ui->_111->minimum() == v )
            variesWasSet_111 = true;
    }

    drawSample();
}

// hjust combo box
void TextVerify_Dialog::on__109_currentIndexChanged(int index)
{
    drawSample();
}

void TextVerify_Dialog::on__110_currentIndexChanged(int index)
{
    drawSample();
}

// halo colour value
void TextVerify_Dialog::on__121_valueChanged(int v)
{
    //((IntegerDialogControl*)RCdlg->getcontrol(121))->change(RCdlg,v);
    if(variesWasSet_121)
    {
        int defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(121))->getDefaultValue();
        ((IntegerDialogControl*)this->RCdlg->getcontrol(121))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(121))->setSpecialValueText(_RCT(""));
        ui->_121->setSpecialValueText("");
        variesWasSet_121=false;
    }
    else
    {
        if(!ui->_121->specialValueText().isEmpty() && ui->_121->minimum() == v )
            variesWasSet_121 = true;
    }

    if((ColourSampleControl*)RCdlg->getcontrol(125));
    {
        double r,g,b;
        db.colourtable[v].get(&r,&g,&b);
        Colour c(r,g,b);
        ((ColourSampleControl*)RCdlg->getcontrol(125))->change(c);
    }

    drawSample();
}

// colour selector button
void TextVerify_Dialog::on__1000_clicked(bool checked)
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

// layer selector button
void TextVerify_Dialog::on__1001_clicked(bool checked)
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

// style selector button
void TextVerify_Dialog::on__1002_clicked(bool checked)
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

// weight selector button
void TextVerify_Dialog::on__1003_clicked(bool checked)
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

// halo colour selector button
void TextVerify_Dialog::on__1004_clicked(bool checked)
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

// bold
void TextVerify_Dialog::on__112_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_112->isTristate())
    {
        ui->_112->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(112))->setTriState(false);
    }
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

// underline
void TextVerify_Dialog::on__113_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_113->isTristate())
    {
        ui->_113->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(113))->setTriState(false);
    }
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 113
    wParam = MAKEWPARAM((WORD)113,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_113;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

// italic
void TextVerify_Dialog::on__114_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_114->isTristate())
    {
        ui->_114->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(114))->setTriState(false);
    }
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
void TextVerify_Dialog::on__115_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_115->isTristate())
    {
        ui->_115->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(115))->setTriState(false);
    }    // using the dialog callback function
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
void TextVerify_Dialog::on__116_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_116->isTristate())
    {
        ui->_116->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(116))->setTriState(false);
    }
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

// scaled
void TextVerify_Dialog::on__120_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_120->isTristate())
    {
        ui->_120->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(120))->setTriState(false);
    }
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

// halo
void TextVerify_Dialog::on__122_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_122->isTristate())
    {
        ui->_122->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(122))->setTriState(false);
    }
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

// height in points
void TextVerify_Dialog::on__123_clicked(bool checked)
{
    // if this in tristate mode make it two state
    if(ui->_123->isTristate())
    {
        ui->_123->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(123))->setTriState(false);
    }
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

// clone button
void TextVerify_Dialog::on__117_clicked(bool checked)
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
void TextVerify_Dialog::on__118_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 118
    wParam = MAKEWPARAM((WORD)118,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_118;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();

}

void TextVerify_Dialog::on_accept()
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

void TextVerify_Dialog::drawSample()
{
    if((TextDisplayDialogControl*)RCdlg->getcontrol(124));
        ((TextDisplayDialogControl*)RCdlg->getcontrol(124))->draw(RCdlg,0);

    if((ColourSampleControl*)RCdlg->getcontrol(125));
        ((ColourSampleControl*)RCdlg->getcontrol(125))->draw(RCdlg,0);

}
