
#include "ncwin.h"
#include "RCDialog.h"
#include "ruledsurface_dialog.h"
#include "ui_ruledsurface_dialog.h"

bool RuledSurfVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<RuledSurface_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1006")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((RuledSurface_Dialog*)dialog)->ui->_1006->setCurrentIndex(1);
                    ((RuledSurface_Dialog*)dialog)->ui->_1006->hide();
                    ((RuledSurface_Dialog*)dialog)->ui->_106->show();
                    ((RuledSurface_Dialog*)dialog)->ui->_106->showPopup();
                    return true;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<RuledSurface_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_100)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_100 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_101)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_101 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_102)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_102 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_103)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_103 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_104")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_104)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_104 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_104->setSpecialValueText("");
                    ((IntegerDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_104->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_105")
            {
                // only use this once
                if(((RuledSurface_Dialog*)dialog)->variesWasSet_105)
                {
                    ((RuledSurface_Dialog*)dialog)->variesWasSet_105 = false;
                    ((RuledSurface_Dialog*)dialog)->ui->_105->setSpecialValueText("");
                    ((IntegerDialogControl*)((RuledSurface_Dialog*)dialog)->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
                    ((RuledSurface_Dialog*)dialog)->ui->_105->clear();
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

RuledSurface_Dialog::RuledSurface_Dialog(HANDLE parent, HANDLE rcparent, int f):
	RuledSurface_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
RuledSurface_Dialog::RuledSurface_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::RuledSurface_Dialog)
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
  variesWasSet_105 = false;
  //ui->_105->setValue(ui->_105->maximum());

  // get the change settings flags
  changecount = v.getinteger("op::changecount");
  change = (bool)v.getinteger("op::change");
  changeBits = v.getbm("op::changemask");

  // make the dummy gradation style combo the same size as the real one and hide it
  ui->_1006->setGeometry(ui->_1006->geometry());
  ui->_1006->hide();

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_107)
      ui->_107->setScene(scene);
  ui->_107->setSceneRect(QRectF());

  ((RuledSurfaceDisplayDialogControl*)RCdlg->getcontrol(107))->setQW(ui->_107);
  ((RuledSurfaceDisplayDialogControl*)RCdlg->getcontrol(107))->sethwnd(ui->_107);

  initDialog(this);

  // enable or disable the controls
  if(change)
  {
      // replace the gradation style combox box with the *Varies* version
      if(changecount > 1 && !changeBits->test(6))
      {
          ui->_106->hide();
          ui->_1006->show();
      }
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
  }

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  RuledSurfVerifyEventFilter *Filter = new RuledSurfVerifyEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_104->installEventFilter(Filter);
  ui->_105->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);

  drawSample();
}

RuledSurface_Dialog::~RuledSurface_Dialog()
{
    delete ui;
}

// colour
void  RuledSurface_Dialog::on__100_valueChanged(int v)
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

// style
void  RuledSurface_Dialog::on__101_valueChanged(int v)
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

// layer
void  RuledSurface_Dialog::on__102_valueChanged(int v)
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

// weight
void  RuledSurface_Dialog::on__103_valueChanged(double v)
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

// X mesh
void  RuledSurface_Dialog::on__104_valueChanged(int v)
{
    double defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(104))->getDefaultValue();
    if(variesWasSet_104 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(104))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
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

// y mesh
void  RuledSurface_Dialog::on__105_valueChanged(int v)
{
    double defVal = ((IntegerDialogControl*)this->RCdlg->getcontrol(105))->getDefaultValue();
    if(variesWasSet_105 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((IntegerDialogControl*)this->RCdlg->getcontrol(105))->change(this->RCdlg,defVal);
        ((IntegerDialogControl*)this->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
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

// draw mode combo box
void  RuledSurface_Dialog::on__106_currentIndexChanged(int index)
{
    drawSample();
}

// surface properties
void RuledSurface_Dialog::on__108_clicked(bool checked)
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

// triangulate button
void RuledSurface_Dialog::on__109_clicked(bool checked)
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
}

// clone
void RuledSurface_Dialog::on__110_clicked(bool checked)
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
}

// clone all
void RuledSurface_Dialog::on__111_clicked(bool checked)
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

// colour select button
void RuledSurface_Dialog::on__1000_clicked(bool checked)
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

// layer select button
void RuledSurface_Dialog::on__1001_clicked(bool checked)
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

// style select button
void RuledSurface_Dialog::on__1002_clicked(bool checked)
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

// weight select button
void RuledSurface_Dialog::on__1003_clicked(bool checked)
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

void RuledSurface_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1
    wParam = MAKEWPARAM((WORD)1,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}

void RuledSurface_Dialog::drawSample()
{
    if((PlaneDisplayDialogControl*)RCdlg->getcontrol(107));
        ((PlaneDisplayDialogControl*)RCdlg->getcontrol(107))->draw(RCdlg,0);
}
