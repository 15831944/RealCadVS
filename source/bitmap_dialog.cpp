
#include "ncwin.h"
#include "RCDialog.h"
#include "bitmap_dialog.h"
#include "ui_BITMAP_DIALOG.h"

bool BitMapEVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<Bitmap_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1008")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((Bitmap_Dialog*)dialog)->ui->_1008->setCurrentIndex(1);
                    ((Bitmap_Dialog*)dialog)->ui->_1008->hide();
                    ((Bitmap_Dialog*)dialog)->ui->_108->show();
                    ((Bitmap_Dialog*)dialog)->ui->_108->showPopup();
                    return true;
                }
                if(obj->objectName()== "_1009")
                {
                    ((Bitmap_Dialog*)dialog)->ui->_1009->hide();
                    ((Bitmap_Dialog*)dialog)->ui->_1009->setCurrentIndex(1);
                    ((Bitmap_Dialog*)dialog)->ui->_109->show();
                    ((Bitmap_Dialog*)dialog)->ui->_109->showPopup();
                    return true;
                }
                if(obj->objectName()== "_1010")
                {
                    ((Bitmap_Dialog*)dialog)->ui->_1010->hide();
                    ((Bitmap_Dialog*)dialog)->ui->_1010->setCurrentIndex(1);
                    ((Bitmap_Dialog*)dialog)->ui->_110->show();
                    ((Bitmap_Dialog*)dialog)->ui->_110->showPopup();
                    return true;
                }
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<Bitmap_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1006")
                {
                    bool status = true;
                    if(((Bitmap_Dialog*)dialog)->variesWasSet_126)
                    {
                        ((Bitmap_Dialog*)dialog)->variesWasSet_126 = false;
                        ((Bitmap_Dialog*)dialog)->ui->_126->setSpecialValueText("");
                        ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(126))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((Bitmap_Dialog*)dialog)->ui->_126->setValue(((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(126))->getDefaultValue());
                    }
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Bitmap_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_100)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_100 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_101)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_101 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_102)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_102 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_103)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_103 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_106)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_106 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_106->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_107")
            {
                if(((Bitmap_Dialog*)dialog)->variesWasSet_107)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_107 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_107->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_107->clear();
                    return false;
                }
            }
            /*
            if(obj->objectName()== "_112")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_112)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_112 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_112->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(112))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_112->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_113")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_113)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_113 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_113->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(113))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_113->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_114")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_114)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_114 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_114->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(114))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_114->clear();
                    return false;
                }
            }
            */
            /*
            if(obj->objectName()== "_116")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_116)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_116 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_116->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(116))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_116->clear();
                    return false;
                }
            }
            */
            if(obj->objectName()== "_117")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_117)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_117 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_117->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(117))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_117->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_126")
            {
                // only use this once
                if(((Bitmap_Dialog*)dialog)->variesWasSet_126)
                {
                    ((Bitmap_Dialog*)dialog)->variesWasSet_126 = false;
                    ((Bitmap_Dialog*)dialog)->ui->_126->setSpecialValueText("");
                    ((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(126))->setSpecialValueText(_RCT(""));
                    ((Bitmap_Dialog*)dialog)->ui->_126->setValue(((RealDialogControl*)((Bitmap_Dialog*)dialog)->RCdlg->getcontrol(126))->getDefaultValue());
                    //((Bitmap_Dialog*)dialog)->ui->_126->clear();
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


Bitmap_Dialog::Bitmap_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Bitmap_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Bitmap_Dialog::Bitmap_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Bitmap_Dialog)
{
  ui->setupUi(this);

  // set the link to false so the values are not forced
  ui->_1014->setChecked(false);
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
  //variesWasSet_112 = false;
  //ui->_112->setValue(ui->_112->maximum());
  //variesWasSet_113 = false;
  //ui->_113->setValue(ui->_113->maximum());
  //variesWasSet_114 = false;
  //ui->_114->setValue(ui->_114->maximum());
  //variesWasSet_116 = false;
  //ui->_116->setValue(ui->_116->maximum());
  variesWasSet_117 = false;
  ui->_117->setValue(ui->_117->maximum());
  variesWasSet_126 = false;
  ui->_126->setValue(ui->_126->maximum());

  // get the change settings flags
  changecount = v.getinteger("op::changecount");
  change = (bool)v.getinteger("op::change");
  changeBits = v.getbm("op::changemask");

  // make the dummy hjust combo the same size as the real one and hide it
  ui->_1008->setGeometry(ui->_108->geometry());
  ui->_1008->hide();

  // make the dummy vjust combo the same size as the real one and hide it
  ui->_1009->setGeometry(ui->_109->geometry());
  ui->_1009->hide();

  // make the dummy style combo the same size as the real one and hide it
  ui->_1010->setGeometry(ui->_110->geometry());
  ui->_1010->hide();

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_118)
      ui->_118->setScene(scene);
  ui->_118->setSceneRect(ui->_118->rect());
  ((BitMapEDisplayDialogControl*)RCdlg->getcontrol(118))->setQW(ui->_118);
  ((BitMapEDisplayDialogControl*)RCdlg->getcontrol(118))->sethwnd(ui->_118);

  if(ui->_205)
      ui->_205->setScene(0);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->setQW(0);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->sethwnd(0);

  ui->_104->setTristate(change);// origial shape
  ui->_105->setTristate(change);// link to file
  ui->_111->setTristate(change);// transparent flag

  // init the realCAD Dialog class here
  initDialog(this);

  // bitmap transparent colour sample control
  scene = new QGraphicsScene;
  if(ui->_205)
      ui->_205->setScene(scene);
  ui->_205->setSceneRect(ui->_205->rect());
  ((ColourSampleControl*)RCdlg->getcontrol(205))->setQW(ui->_205);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->sethwnd(ui->_205);

  // init the transparency slider
  ui->_303->setValue(ui->_117->value());


  if(change)
  {
      // replace the hjust combo box with the *Varies* version
      if(changecount > 1 && !changeBits->test(8))
      {
          ui->_108->hide();
          ui->_1008->show();
      }
      // replace the vjust combo box with the *Varies* version
      if(changecount > 1 && !changeBits->test(9))
      {
          ui->_109->hide();
          ui->_1009->show();
      }
      // replace the style combo box with the *Varies* version
      if(changecount > 1 && !changeBits->test(10))
      {
          ui->_110->hide();
          ui->_1010->show();
      }
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
      // enable the filename label
      ui->label_200->setEnabled(true);
      // disable the filename
      ui->_200->setEnabled(false);
      // enable the origin label
      ui->label_201->setEnabled(true);
      // disable the origin text
      ui->_201->setEnabled(false);
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
      // disable the filename label
      ui->label_200->setEnabled(false);
      // disable the filename
      ui->_200->setEnabled(false);
      // disable the origin label
      ui->label_201->setEnabled(false);
      // disable the origin text
      ui->_201->setEnabled(false);

      ui->previewback_label->setEnabled(false);
      ui->_1011->setEnabled(false);
      ui->_1012->setEnabled(false);
      ui->_1013->setEnabled(false);
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
      // disable the filename label
      ui->label_200->setEnabled(false);
      // disable the filename
      ui->_200->setEnabled(false);
      // disable the origin label
      ui->label_201->setEnabled(false);
      // disable the origin text
      ui->_201->setEnabled(false);
  }

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  BitMapEVerifyEventFilter *Filter = new BitMapEVerifyEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);
  ui->_107->installEventFilter(Filter);
  ui->_108->installEventFilter(Filter);
  ui->_109->installEventFilter(Filter);
  ui->_110->installEventFilter(Filter);
  //ui->_112->installEventFilter(Filter);
  //ui->_113->installEventFilter(Filter);
  //ui->_114->installEventFilter(Filter);
  //ui->_116->installEventFilter(Filter);
  ui->_117->installEventFilter(Filter);
  ui->_126->installEventFilter(Filter);
  ui->_1006->installEventFilter(Filter);
  ui->_1008->installEventFilter(Filter);
  ui->_1009->installEventFilter(Filter);
  ui->_1010->installEventFilter(Filter);

  sliderIsReleased=true;

  ui->previewback_label->setEnabled(ui->_111->isChecked());
  ui->_1011->setEnabled(ui->_111->isChecked());
  ui->_1012->setEnabled(ui->_111->isChecked());
  ui->_1013->setEnabled(ui->_111->isChecked());

  // if the plane settings are not equal unlink them
  // else use the session default
  ui->_1014->setChecked(v.getinteger("bi::sizelinked") == 1 ? true : false);
  qDebug() << "ui->_106->value()" << ui->_106->value();
  qDebug() << "ui->_107->value()" << ui->_107->value();
  qDebug() << "fabs(ui->_106->value() - ui->_107->value())" << fabs(ui->_106->value() - ui->_107->value());
  if(fabs(ui->_106->value() - ui->_107->value()) > 0.00001)
      ui->_1014->setChecked(false);

  drawSample();
}

Bitmap_Dialog::~Bitmap_Dialog()
{
    QGraphicsScene *scene = 0;
    if(ui->_205)
    {
        scene = ui->_205->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_205->setScene(0);
        }
    }
    delete ui;
}

void Bitmap_Dialog::on__100_valueChanged(int v)
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

void Bitmap_Dialog::on__101_valueChanged(int v)
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

void Bitmap_Dialog::on__102_valueChanged(int v)
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

void Bitmap_Dialog::on__103_valueChanged(double v)
{
    //((WeightAssocRealDialogControl*)RCdlg->getcontrol(103))->change(RCdlg,v);
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

void Bitmap_Dialog::on__104_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 104
    wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_104;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();

}

void Bitmap_Dialog::on__105_clicked(bool checked)
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

    drawSample();

}

// width
void Bitmap_Dialog::on__106_valueChanged(double v)
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

    if(ui->_1014->isChecked())
        ui->_107->setValue(v);

    drawSample();

}

// height
void Bitmap_Dialog::on__107_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(107))->change(RCdlg,v);
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

    if(ui->_1014->isChecked())
        ui->_106->setValue(v);

    drawSample();

}

// horizontal justification
void Bitmap_Dialog::on__108_currentIndexChanged(int index)
{
    drawSample();
}

// vertical justification
void Bitmap_Dialog::on__109_currentIndexChanged(int index)
{
    drawSample();
}

// frame style
void Bitmap_Dialog::on__110_currentIndexChanged(int index)
{
    drawSample();
}

void Bitmap_Dialog::on__111_clicked(bool checked)
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

    if(change && changecount == 1)
    {
        ui->previewback_label->setEnabled(checked);
        ui->_1011->setEnabled(checked);
        ui->_1012->setEnabled(checked);
        ui->_1013->setEnabled(checked);
    }
    drawSample();
}

 // tr
/*
void Bitmap_Dialog::on__112_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(112))->change(RCdlg,v);
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

    drawSample();
}

// tg
void Bitmap_Dialog::on__113_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(113))->change(RCdlg,v);
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

    drawSample();
}

// tb
void Bitmap_Dialog::on__114_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(114))->change(RCdlg,v);
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(114))->getDefaultValue();
    if(variesWasSet_114 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(114))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(114))->setSpecialValueText(_RCT(""));
        ui->_114->setSpecialValueText("");
        variesWasSet_114 = false;
    }
    else
    {
        if(!ui->_114->specialValueText().isEmpty() && ui->_114->minimum() == v )
            variesWasSet_114 = true;
    }

    drawSample();
}
*/
/*
void Bitmap_Dialog::on__1005_clicked(bool checked)
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
*/
// trange
/*
void Bitmap_Dialog::on__116_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(116))->change(RCdlg,v);
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(116))->getDefaultValue();
    if(variesWasSet_116 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(116))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(116))->setSpecialValueText(_RCT(""));
        ui->_116->setSpecialValueText("");
        variesWasSet_116 = false;
    }
    else
    {
        if(!ui->_116->specialValueText().isEmpty() && ui->_116->minimum() == v )
            variesWasSet_116 = true;
    }

    drawSample();
}
*/
// opacity
void Bitmap_Dialog::on__117_valueChanged(double v)
{
    //((RealDialogControl*)RCdlg->getcontrol(117))->change(RCdlg,v);
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(117))->getDefaultValue();
    if(variesWasSet_117 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(117))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(117))->setSpecialValueText(_RCT(""));
        ui->_117->setSpecialValueText("");
        variesWasSet_117 = false;
    }
    else
    {
        if(!ui->_117->specialValueText().isEmpty() && ui->_117->maximum() == v )
            variesWasSet_117 = true;
    }

    //ui->_134->setValue(v*100.0);
    ui->_303->setValue(v);
    if(sliderIsReleased)
        drawSample();
}

// clone button
void Bitmap_Dialog::on__119_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 119
    wParam = MAKEWPARAM((WORD)119,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_119;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone all button
void Bitmap_Dialog::on__120_clicked(bool checked)
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

}

// colour selector
void Bitmap_Dialog::on__1000_clicked(bool checked)
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

// layer selector
void Bitmap_Dialog::on__1001_clicked(bool checked)
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

// style selector
void Bitmap_Dialog::on__1002_clicked(bool checked)
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

// weight selector
void Bitmap_Dialog::on__1003_clicked(bool checked)
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

void Bitmap_Dialog::on_accept()
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
    {
        // save the linked setting the dialog is being used to set defaults
        if(changecount == 0)
            v.setinteger("bi::sizelinked",ui->_1014->isChecked() ? 1 : 0);

        QDialog::accept();
    }
}

void Bitmap_Dialog::drawSample()
{
    if((BitMapEDisplayDialogControl*)RCdlg->getcontrol(118));
        ((BitMapEDisplayDialogControl*)RCdlg->getcontrol(118))->draw(RCdlg,0);

    if((ColourSampleControl*)RCdlg->getcontrol(205))
        ((ColourSampleControl*)RCdlg->getcontrol(205))->draw(RCdlg,0);
}

void Bitmap_Dialog::on__126_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(126))->getDefaultValue();
    if(variesWasSet_126 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(126))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(126))->setSpecialValueText(_RCT(""));
        ui->_126->setSpecialValueText("");
        variesWasSet_126 = false;
    }
    else
    {
        if(!ui->_126->specialValueText().isEmpty() && ui->_126->minimum() == v )
            variesWasSet_126 = true;
    }
    drawSample();
}

void Bitmap_Dialog::on__1006_clicked()
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

void Bitmap_Dialog::on__1007_clicked()
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

void Bitmap_Dialog::on__303_sliderPressed()
{
    sliderIsReleased=true;
    drawSample();
}

void Bitmap_Dialog::on__303_sliderReleased()
{
    sliderIsReleased=false;
}

void Bitmap_Dialog::on__303_valueChanged(int value)
{
    double percentageValue = (double)value;
    double decimalValue = percentageValue / 100.0;
    ui->_117->setValue(percentageValue);
    //ui->_134->setValue(percentageValue);

}

void Bitmap_Dialog::on__1011_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1011
    wParam = MAKEWPARAM((WORD)1011,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1011;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Bitmap_Dialog::on__1012_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1012
    wParam = MAKEWPARAM((WORD)1012,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1012;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void Bitmap_Dialog::on__1013_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1013
    wParam = MAKEWPARAM((WORD)1013,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1013;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}
