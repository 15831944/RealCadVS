
#include "ncwin.h"
#include "RCDialog.h"
#include "figure_dialog.h"
#include "ui_FIGURE_DIALOG.h"

extern void FigureExtents(RCCHAR *,View3dSurface *, Point *,Point *);

bool FigureVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();

    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Figure_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_100)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_100 = false;
                    ((Figure_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_101)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_101 = false;
                    ((Figure_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_102)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_102 = false;
                    ((Figure_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_103)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_103 = false;
                    ((Figure_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_104")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_104)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_104 = false;
                    ((Figure_Dialog*)dialog)->ui->_104->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(104))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(104))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_104->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_105")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_105)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_105 = false;
                    ((Figure_Dialog*)dialog)->ui->_105->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(105))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(105))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_105->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_106)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_106 = false;
                    ((Figure_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_106->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_107")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_107)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_107 = false;
                    ((Figure_Dialog*)dialog)->ui->_107->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(107))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_107->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_108")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_108)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_108 = false;
                    ((Figure_Dialog*)dialog)->ui->_108->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(108))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(108))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_108->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_109")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_109)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_109 = false;
                    ((Figure_Dialog*)dialog)->ui->_109->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(109))->setSpecialValueText(_RCT(""));
                    double defval = ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(106))->getDefaultValue();
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(109))->change(((Figure_Dialog*)dialog)->RCdlg,defval);
                    ((Figure_Dialog*)dialog)->ui->_109->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_116")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_116)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_116 = false;
                    ((Figure_Dialog*)dialog)->ui->_116->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(116))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_116->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_117")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_117)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_117 = false;
                    ((Figure_Dialog*)dialog)->ui->_117->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(117))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_117->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_118")
            {
                // only use this once
                if(((Figure_Dialog*)dialog)->variesWasSet_118)
                {
                    ((Figure_Dialog*)dialog)->variesWasSet_118 = false;
                    ((Figure_Dialog*)dialog)->ui->_118->setSpecialValueText("");
                    ((RealDialogControl*)((Figure_Dialog*)dialog)->RCdlg->getcontrol(118))->setSpecialValueText(_RCT(""));
                    ((Figure_Dialog*)dialog)->ui->_118->clear();
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

Figure_Dialog::Figure_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Figure_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Figure_Dialog::Figure_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Figure_Dialog)
{
  initialised = false;

  ui->setupUi(this);

  setAttribute(Qt::WA_AlwaysShowToolTips,true);

  // set the link to false so the values are not forced
  ui->_1008->setChecked(false);

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
  ui->_105->setValue(ui->_105->maximum());
  variesWasSet_106 = false;
  ui->_106->setValue(ui->_106->maximum());
  variesWasSet_107 = false;
  ui->_106->setValue(ui->_107->maximum());
  variesWasSet_108 = false;
  ui->_106->setValue(ui->_108->maximum());
  variesWasSet_109 = false;
  ui->_106->setValue(ui->_109->maximum());
  variesWasSet_116 = false;
  ui->_116->setValue(ui->_116->maximum());
  variesWasSet_117 = false;
  ui->_117->setValue(ui->_117->maximum());
  variesWasSet_118 = false;
  ui->_118->setValue(ui->_118->maximum());

  // get the change settings flags
  changecount = v.getinteger("op::changecount");
  change = (bool)v.getinteger("op::change");
  changeBits = v.getbm("op::changemask");

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_123)
      ui->_123->setScene(scene);
  ui->_123->setSceneRect(QRectF());
  ((FigureDisplayDialogControl*)RCdlg->getcontrol(123))->setQW(ui->_123);
  ((FigureDisplayDialogControl*)RCdlg->getcontrol(123))->sethwnd(ui->_123);

  initDialog(this);

  // force the size/scale check boxes to update
  on__131_clicked(true);

  // disable the origin text
  ui->_110->setEnabled(false);
  // disable the filename text
  ui->_119->setEnabled(false);
  if(ui->_119->text().length() > 0)
    ui->_119->setToolTip(ui->_119->text());
  // disable the alt name text
  ui->_120->setEnabled(false);

  // enable or disable the header controls
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

  FigureVerifyEventFilter *Filter = new FigureVerifyEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_104->installEventFilter(Filter);
  ui->_105->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);
  ui->_116->installEventFilter(Filter);
  ui->_117->installEventFilter(Filter);
  ui->_118->installEventFilter(Filter);

  library = ui->_119->text();
  if(!library.isEmpty())
  {
      Point pmin,pmax;
      FigureExtents(library.data(),0,&pmin,&pmax);
      librarySize = pmax - pmin;
  }
  else
      librarySize = Point(1000.0,1000.0,0.0);

  initialised = true;

  drawSample();
}

Figure_Dialog::~Figure_Dialog()
{
    delete ui;
}

// colour
void Figure_Dialog::on__100_valueChanged(int v)
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
void Figure_Dialog::on__101_valueChanged(int v)
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
void Figure_Dialog::on__102_valueChanged(int v)
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
void Figure_Dialog::on__103_valueChanged(double v)
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

// original layer
void Figure_Dialog::on__111_clicked(bool checked)
{
    // if this in tristate mode make it two state
    /*
    if(ui->_111->isTristate())
    {
        ui->_111->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(111))->setTriState(false);
    }
    */
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

// original colour
void Figure_Dialog::on__112_clicked(bool checked)
{
    // if this in tristate mode make it two state
    /*
    if(ui->_112->isTristate())
    {
        ui->_112->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(112))->setTriState(false);
    }
    */
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 112
    wParam = MAKEWPARAM((WORD)112,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_112;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// original style
void Figure_Dialog::on__113_clicked(bool checked)
{
    // if this in tristate mode make it two state
    /*
    if(ui->_113->isTristate())
    {
        ui->_113->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(113))->setTriState(false);
    }
    */
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 113
    wParam = MAKEWPARAM((WORD)113,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_113;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// original weight
void Figure_Dialog::on__114_clicked(bool checked)
{
    // if this in tristate mode make it two state
    /*
    if(ui->_114->isTristate())
    {
        ui->_114->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(114))->setTriState(false);
    }
    */
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 114
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// orient to view
void Figure_Dialog::on__115_clicked(bool checked)
{
    // if this in tristate mode make it two state
    /*
    if(ui->_115->isTristate())
    {
        ui->_115->setTristate(false);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(115))->setTriState(false);
    }
    */
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 115
    wParam = MAKEWPARAM((WORD)115,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_115;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// def path
void Figure_Dialog::on__1022_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 122
    wParam = MAKEWPARAM((WORD)1022,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1022;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// auto attach/detach
void Figure_Dialog::on__124_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 124
    wParam = MAKEWPARAM((WORD)124,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_124;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// attach all figures
void Figure_Dialog::on__125_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 125
    wParam = MAKEWPARAM((WORD)125,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_125;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// detach all figures
void Figure_Dialog::on__126_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 126
    wParam = MAKEWPARAM((WORD)126,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_126;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// missing
void Figure_Dialog::on__127_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 127
    wParam = MAKEWPARAM((WORD)127,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_127;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// modify parametric figure
void Figure_Dialog::on__128_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 128
    wParam = MAKEWPARAM((WORD)128,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_128;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone
void Figure_Dialog::on__129_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 129
    wParam = MAKEWPARAM((WORD)129,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_129;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone all
void Figure_Dialog::on__130_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 130
    wParam = MAKEWPARAM((WORD)130,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_130;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// colour select button
void Figure_Dialog::on__1000_clicked(bool checked)
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
void Figure_Dialog::on__1001_clicked(bool checked)
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
void Figure_Dialog::on__1002_clicked(bool checked)
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
void Figure_Dialog::on__1003_clicked(bool checked)
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

void Figure_Dialog::on_accept()
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

void Figure_Dialog::drawSample()
{
    if((FigureDisplayDialogControl*)RCdlg->getcontrol(123));
        ((FigureDisplayDialogControl*)RCdlg->getcontrol(123))->draw(RCdlg,0);
}

// size x
void Figure_Dialog::on__116_valueChanged(double v)
{

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

    qDebug() << "116 value :  " << ui->_116->value();
    if(initialised && change && changecount == 1 && !ui->_132->isChecked())
        ui->_104->setValue(v / librarySize.x);

    if(ui->_1008->isChecked())
        ui->_117->setValue(v);

    drawSample();
}

// size y
void Figure_Dialog::on__117_valueChanged(double v)
{
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
        if(!ui->_117->specialValueText().isEmpty() && ui->_117->minimum() == v )
            variesWasSet_117 = true;
    }

    if(initialised && change && changecount == 1 && !ui->_132->isChecked())
        ui->_105->setValue(v / librarySize.y);

    if(ui->_1008->isChecked())
        ui->_116->setValue(v);

    drawSample();
}

// size z
void Figure_Dialog::on__118_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(118))->getDefaultValue();
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

    if(initialised && change && changecount == 1 && !ui->_132->isChecked())
        ui->_106->setValue(v / librarySize.z);

    drawSample();
}

void Figure_Dialog::on__104_valueChanged(double v)
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

    qDebug() << "104 value :  " << ui->_104->value();
    if(initialised && change && changecount == 1 && !ui->_131->isChecked())
       ui->_116->setValue(librarySize.x * v);

    if(ui->_1008->isChecked())
        ui->_105->setValue(v);

    drawSample();
}

void Figure_Dialog::on__105_valueChanged(double v)
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

    if(initialised && change && changecount == 1 && !ui->_131->isChecked())
        ui->_117->setValue(librarySize.y * v);

    if(ui->_1008->isChecked())
        ui->_104->setValue(v);

    drawSample();
}

void Figure_Dialog::on__106_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(106))->getDefaultValue();
    if(variesWasSet_106 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
        ui->_105->setSpecialValueText("");
        variesWasSet_106 = false;
    }
    else
    {
        if(!ui->_106->specialValueText().isEmpty() && ui->_106->minimum() == v )
            variesWasSet_106 = true;
    }

    if(initialised && change && changecount == 1 && !ui->_131->isChecked())
        ui->_118->setValue(librarySize.z * v);

    drawSample();
}

void Figure_Dialog::on__107_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(107))->getDefaultValue();
    if(variesWasSet_107 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(107))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
        ui->_105->setSpecialValueText("");
        variesWasSet_107 = false;
    }
    else
    {
        if(!ui->_107->specialValueText().isEmpty() && ui->_107->minimum() == v )
            variesWasSet_107 = true;
    }

    if(initialised && !ui->_131->isChecked())
        ui->_118->setValue(librarySize.z * v);

    drawSample();
}

void Figure_Dialog::on__108_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(108))->getDefaultValue();
    if(variesWasSet_108 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(108))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(108))->setSpecialValueText(_RCT(""));
        ui->_105->setSpecialValueText("");
        variesWasSet_108 = false;
    }
    else
    {
        if(!ui->_108->specialValueText().isEmpty() && ui->_108->minimum() == v )
            variesWasSet_108 = true;
    }

    if(initialised && !ui->_131->isChecked())
        ui->_118->setValue(librarySize.z * v);

    drawSample();
}

void Figure_Dialog::on__109_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(109))->getDefaultValue();
    if(variesWasSet_109 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(109))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(109))->setSpecialValueText(_RCT(""));
        ui->_105->setSpecialValueText("");
        variesWasSet_109 = false;
    }
    else
    {
        if(!ui->_109->specialValueText().isEmpty() && ui->_109->minimum() == v )
            variesWasSet_109 = true;
    }

    if(initialised && !ui->_131->isChecked())
        ui->_118->setValue(librarySize.z * v);

    drawSample();
}

// size
void Figure_Dialog::on__131_clicked(bool checked)
{
    // using the dialog callback function
    /*
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 131
    wParam = MAKEWPARAM((WORD)131,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_131;
    dialogcb((HWND)this,msg,wParam,lParam);
    */

    ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(131))->change(this->RCdlg,ui->_131->checkState());

    qDebug() << "state :" << ui->_131->checkState();

    switch(ui->_131->checkState())
    {
    case Qt::Unchecked :
        ui->_132->setCheckState(Qt::Checked);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(132))->change(this->RCdlg,Qt::Checked);
        ui->_116->setEnabled(false);
        ui->_117->setEnabled(false);
        ui->_118->setEnabled(false);

        ui->_104->setEnabled(true);
        ui->_105->setEnabled(true);
        ui->_106->setEnabled(true);
        break;
    case Qt::Checked :
        ui->_132->setCheckState(Qt::Unchecked);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(132))->change(this->RCdlg,Qt::Unchecked);
        ui->_116->setEnabled(true);
        ui->_117->setEnabled(true);
        ui->_118->setEnabled(true);

        ui->_104->setEnabled(false);
        ui->_105->setEnabled(false);
        ui->_106->setEnabled(false);
        break;
    case Qt::PartiallyChecked :
        if(ui->_132->isTristate())
        {
            ui->_132->setCheckState(Qt::PartiallyChecked);
            ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(132))->change(this->RCdlg,Qt::PartiallyChecked);
        }
        else
        {
            ui->_132->setCheckState(Qt::Checked);
            ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(132))->change(this->RCdlg,Qt::Checked);
        }
        ui->_116->setEnabled(false);
        ui->_117->setEnabled(false);
        ui->_118->setEnabled(false);

        ui->_104->setEnabled(false);
        ui->_105->setEnabled(false);
        ui->_106->setEnabled(false);
        break;
    };

    drawSample();
}

// scale
void Figure_Dialog::on__132_clicked(bool checked)
{
    // using the dialog callback function
    /*
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 132
    wParam = MAKEWPARAM((WORD)132,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_132;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
    ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(132))->change(this->RCdlg,ui->_132->checkState());

    qDebug() << "state :" << ui->_132->checkState();

    switch(ui->_132->checkState())
    {
    case Qt::Unchecked :
        ui->_131->setCheckState(Qt::Checked);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(131))->change(this->RCdlg,Qt::Checked);
        ui->_116->setEnabled(true);
        ui->_117->setEnabled(true);
        ui->_118->setEnabled(true);

        ui->_104->setEnabled(false);
        ui->_105->setEnabled(false);
        ui->_106->setEnabled(false);
        break;
    case Qt::Checked :
        ui->_131->setCheckState(Qt::Unchecked);
        ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(131))->change(this->RCdlg,Qt::Unchecked);
        ui->_116->setEnabled(false);
        ui->_117->setEnabled(false);
        ui->_118->setEnabled(false);

        ui->_104->setEnabled(true);
        ui->_105->setEnabled(true);
        ui->_106->setEnabled(true);
        break;
    case Qt::PartiallyChecked :
        if(ui->_131->isTristate())
        {
            ui->_131->setCheckState(Qt::PartiallyChecked);
            ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(131))->change(this->RCdlg,Qt::PartiallyChecked);
        }
        else
        {
            ui->_131->setCheckState(Qt::Checked);
            ((TriStateCheckBoxDialogControl*)this->RCdlg->getcontrol(131))->change(this->RCdlg,Qt::Checked);
        }
        ui->_116->setEnabled(false);
        ui->_117->setEnabled(false);
        ui->_118->setEnabled(false);

        ui->_104->setEnabled(false);
        ui->_105->setEnabled(false);
        ui->_106->setEnabled(false);
        break;
    };

    drawSample();
}

void Figure_Dialog::on__116_editingFinished()
{
    if(ui->_1008->isChecked())
        ui->_117->setValue(ui->_116->value());
}

void Figure_Dialog::on__117_editingFinished()
{
    if(ui->_1008->isChecked())
        ui->_116->setValue(ui->_117->value());
}

void Figure_Dialog::on__104_editingFinished()
{
    if(ui->_1008->isChecked())
        ui->_105->setValue(ui->_104->value());
}

void Figure_Dialog::on__105_editingFinished()
{
    if(ui->_1008->isChecked())
        ui->_104->setValue(ui->_105->value());
}


