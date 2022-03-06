
#include "ncwin.h"
#include "Ncdialog.h"

#include "RCDialog.h"
#include "planeverify_dialog.h"
#include "ui_PLANEVERIFY_DIALOG.h"

bool PlaneVerifyEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *dialog = app->activeModalWidget();
    //qDebug() << "event type : " << event->type();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1021")
                {
                    // change the dummy index to 1 to indicate it has changed
                    ((PlaneVerify_Dialog*)dialog)->ui->_1021->setCurrentIndex(1);
                    ((PlaneVerify_Dialog*)dialog)->ui->_1021->hide();
                    ((PlaneVerify_Dialog*)dialog)->ui->_121->show();
                    ((PlaneVerify_Dialog*)dialog)->ui->_121->showPopup();
                    return true;
                }
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_219")
                {
                    program->setinterrupt(true);
                    return true;
                }
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1004")
                {
                    bool status = true;
                    /*
                    if(strlen(((GradientPlaneButtonDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(1004))->getSpecialValueText()) != 0)
                    {
                        ((GradientPlaneButtonDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(1004))->setSpecialValueText(_RCT(""));
                        ((ColourSampleControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(204))->setSpecialValueText(_RCT(""));
                    }
                    */
                    /*
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_122)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_122 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_122->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(122))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_122->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_122->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(122))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_123)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_123 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_123->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(123))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_123->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_123->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(123))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_124)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_124 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_124->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(124))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_124->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_124->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(124))->getDefaultValue());
                    }
                    */
                    return false;
                }
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1005")
                {
                    bool status = true;
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_126)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_126 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_126->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(126))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_126->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(126))->getDefaultValue());
                    }
                    /*
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_127)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_127 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_127->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_127->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_128)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_128 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_128->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_128->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_128->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_129)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_129 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_129->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_129->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_129->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->getDefaultValue());
                    }
                    */
                    return false;
                }
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
            {
                if(obj->objectName()== "_1006")
                {
                    bool status = true;
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_134)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_134 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_134->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_134->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(134))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_125)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_125 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_125->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(125))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_125->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(125))->getDefaultValue());
                    }
                    /*
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_127)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_127 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_127->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_127->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_128)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_128 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_128->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_128->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_128->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->getDefaultValue());
                    }
                    if(((PlaneVerify_Dialog*)dialog)->variesWasSet_129)
                    {
                        ((PlaneVerify_Dialog*)dialog)->variesWasSet_129 = false;
                        ((PlaneVerify_Dialog*)dialog)->ui->_129->setSpecialValueText("");
                        ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->setSpecialValueText(_RCT(""));
                        //((PlaneVerify_Dialog*)dialog)->ui->_129->clear();
                        ((PlaneVerify_Dialog*)dialog)->ui->_129->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->getDefaultValue());
                    }
                    */
                    return false;
                }
            }
        }
        return false;
    }
    else
    if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<PlaneVerify_Dialog *>(dialog) != 0)
        {
            if(obj->objectName()== "_100")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_100)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_100 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_100->setSpecialValueText("");
                    ((IntegerDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(100))->setSpecialValueText(_RCT(""));
                    ((PlaneVerify_Dialog*)dialog)->ui->_100->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_101")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_101)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_101 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_101->setSpecialValueText("");
                    ((IntegerDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(101))->setSpecialValueText(_RCT(""));
                    ((PlaneVerify_Dialog*)dialog)->ui->_101->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_102")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_102)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_102 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_102->setSpecialValueText("");
                    ((IntegerDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(102))->setSpecialValueText(_RCT(""));
                    ((PlaneVerify_Dialog*)dialog)->ui->_102->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_103")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_103)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_103 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_103->setSpecialValueText("");
                    ((WeightAssocRealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(103))->setSpecialValueText(_RCT(""));
                    ((PlaneVerify_Dialog*)dialog)->ui->_103->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_104")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_104)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_104 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_104->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(104))->setSpecialValueText(_RCT(""));
                    ((PlaneVerify_Dialog*)dialog)->ui->_104->setValue(((PlaneVerify_Dialog*)dialog)->ui->_104->defaultValue);
                    ((PlaneVerify_Dialog*)dialog)->ui->_104->clear();
                    return false;
                }
            }
            if(obj->objectName()== "_105")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_105)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_105 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_105->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(105))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_105->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_105->setValue(((PlaneVerify_Dialog*)dialog)->ui->_105->defaultValue);
                    return false;
                }
            }
            if(obj->objectName()== "_106")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_106)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_106 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_106->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(106))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_106->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_106->setValue(((PlaneVerify_Dialog*)dialog)->ui->_106->defaultValue);
                    return false;
                }
            }
            if(obj->objectName()== "_107")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_107)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_107 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_107->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(107))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_107->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_107->setValue(((PlaneVerify_Dialog*)dialog)->ui->_107->defaultValue);
                    return false;
                }
            }
            /*
            if(obj->objectName()== "_122")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_122)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_122 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_122->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(122))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_122->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_122->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(122))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_123")
            {
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_123)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_123 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_123->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(123))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_123->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_123->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(123))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_124")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_124)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_124 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_124->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(124))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_124->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_124->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(124))->getDefaultValue());
                    return false;
                }
            }
            */
            if(obj->objectName()== "_125")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_125)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_125 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_125->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(125))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_125->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_125->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(125))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_126")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_126)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_126 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_126->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(126))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_126->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_126->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(126))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_134")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_134)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_134 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_134->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(134))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_126->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_134->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(134))->getDefaultValue());
                    return false;
                }
            }
            /*
            if(obj->objectName()== "_127")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_127)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_127 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_127->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_127->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_127->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(127))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_128")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_128)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_128 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_128->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_128->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_128->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(128))->getDefaultValue());
                    return false;
                }
            }
            if(obj->objectName()== "_129")
            {
                // only use this once
                if(((PlaneVerify_Dialog*)dialog)->variesWasSet_129)
                {
                    ((PlaneVerify_Dialog*)dialog)->variesWasSet_129 = false;
                    ((PlaneVerify_Dialog*)dialog)->ui->_129->setSpecialValueText("");
                    ((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->setSpecialValueText(_RCT(""));
                    //((PlaneVerify_Dialog*)dialog)->ui->_129->clear();
                    ((PlaneVerify_Dialog*)dialog)->ui->_129->setValue(((RealDialogControl*)((PlaneVerify_Dialog*)dialog)->RCdlg->getcontrol(129))->getDefaultValue());
                    return false;
                }
            }
            */
        }
        return false;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

PlaneVerify_Dialog::PlaneVerify_Dialog(HANDLE parent, HANDLE rcparent, int f):
	PlaneVerify_Dialog((QWidget*) parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
PlaneVerify_Dialog::PlaneVerify_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::PlaneVerify_Dialog)
{
  ui->setupUi(this);

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
  ui->_107->setValue(ui->_107->maximum());

  /*
  variesWasSet_122 = false;
  ui->_122->setValue(ui->_122->maximum());
  variesWasSet_123 = false;
  ui->_123->setValue(ui->_123->maximum());
  variesWasSet_124 = false;
  ui->_124->setValue(ui->_124->maximum());
  */
  variesWasSet_125 = false;
  ui->_125->setValue(ui->_125->maximum());
  variesWasSet_126 = false;
  ui->_126->setValue(ui->_126->maximum());

  /*
  variesWasSet_127 = false;
  ui->_127->setValue(ui->_127->maximum());
  variesWasSet_128 = false;
  ui->_128->setValue(ui->_128->maximum());
  variesWasSet_129 = false;
  ui->_129->setValue(ui->_129->maximum());
  */  
  variesWasSet_134 = false;
  ui->_134->setValue(ui->_134->maximum());

  // get the change settings flags
  changecount = v.getinteger("op::changecount");
  change = (bool)v.getinteger("op::change");
  changeBits = v.getbm("op::changemask");

  // make the dummy gradation style combo the same size as the real one and hide it
  ui->_1021->setGeometry(ui->_121->geometry());
  ui->_1021->hide();

  // set the preview pointers to null so it only gets draw once during init
  if(ui->_219)
      ui->_219->setScene(0);
  ((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))->setQW(0);
  ((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))->sethwnd(0);

  if(ui->_220)
      ui->_220->setScene(0);

  if(ui->_205)
      ui->_205->setScene(0);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->setQW(0);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->sethwnd(0);

  // gradient colour sample control
  if(ui->_204)
      ui->_204->setScene(0);
  ((ColourSampleControl*)RCdlg->getcontrol(204))->setQW(0);
  ((ColourSampleControl*)RCdlg->getcontrol(204))->sethwnd(0);

  initDialog(this);

  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_219)
      ui->_219->setScene(scene);
  ui->_219->setSceneRect(ui->_219->rect());
  ((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))->setQW(ui->_219);
  ((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))->sethwnd(ui->_219);

  // bitmap/pattern sample preview
  scene = new QGraphicsScene;
  if(ui->_220)
      ui->_220->setScene(scene);
  ui->_220->setSceneRect(ui->_220->rect());

  // bitmap transparent colour sample control
  scene = new QGraphicsScene;
  if(ui->_205)
      ui->_205->setScene(scene);
  ui->_205->setSceneRect(ui->_205->rect());
  ((ColourSampleControl*)RCdlg->getcontrol(205))->setQW(ui->_205);
  ((ColourSampleControl*)RCdlg->getcontrol(205))->sethwnd(ui->_205);

  // gradient colour sample control
  scene = new QGraphicsScene;
  if(ui->_204)
      ui->_204->setScene(scene);
  ui->_204->setSceneRect(ui->_204->rect());
  ((ColourSampleControl*)RCdlg->getcontrol(204))->setQW(ui->_204);
  ((ColourSampleControl*)RCdlg->getcontrol(204))->sethwnd(ui->_204);

  ui->_120->setTristate(false);
  if(change && changecount > 0)
      ui->_120->setTristate(!changeBits->test(5));

  // init the transparency slider
  ui->_303->setValue(ui->_134->value());

  if(change)
  {
      // replace the gradation style combox box with the *Varies* version
      if(changecount > 1 && !changeBits->test(7))
      {
          ui->_121->hide();
          ui->_1021->show();
      }
  }

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

  // hide the sample dimension label and text label
  // if the fill style is no fill, hatching or solid fill
  if(ui->_114->isChecked() || ui->_115->isChecked() || ui->_117->isChecked())
  {
      ui->_133->hide();
      ui->_1333->hide();
  }
  else
  {
      ui->_133->show();
      ui->_1333->show();
  }

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  // connect the decimal transparency control to the percentage control
  //connect(ui->_125,  SIGNAL(valueChanged(double)),  this,  SLOT(decimalTranspChanged(double)));
  //connect(ui->_134,  SIGNAL(valueChanged(double)),  this,  SLOT(percentTranspChanged(double)));


  PlaneVerifyEventFilter *Filter = new PlaneVerifyEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_104->installEventFilter(Filter);
  ui->_105->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);
  ui->_107->installEventFilter(Filter);
  ui->_121->installEventFilter(Filter);
  ui->_1021->installEventFilter(Filter);
  //ui->_122->installEventFilter(Filter);
  //ui->_123->installEventFilter(Filter);
  //ui->_124->installEventFilter(Filter);
  ui->_125->installEventFilter(Filter);
  ui->_126->installEventFilter(Filter);
  //ui->_127->installEventFilter(Filter);
  //ui->_128->installEventFilter(Filter);
  //ui->_129->installEventFilter(Filter);
  ui->_134->installEventFilter(Filter);

  ui->_1004->installEventFilter(Filter);
  ui->_1005->installEventFilter(Filter);

  rcplane = 0;
  sliderIsReleased=true;

  // if the plane settings are not equal unlink them
  // else use the session default
  ui->_1008->setChecked(v.getinteger("pl::spacinglinked") == 1 ? true : false);
  if(fabs(ui->_104->value() - ui->_105->value()) > 0.00001)
      ui->_1008->setChecked(false);

  drawSample();
  //drawSample(1);
  //drawSample(2);
}

PlaneVerify_Dialog::~PlaneVerify_Dialog()
{
    QGraphicsScene *scene = 0;
    if(ui->_204)
    {
        scene = ui->_204->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_204->setScene(0);
        }
    }
    if(ui->_205)
    {
        scene = ui->_205->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_205->setScene(0);
        }
    }
    if(ui->_219)
    {
        scene = ui->_219->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_219->setScene(0);
        }
    }
    if(ui->_220)
    {
        scene = ui->_220->scene();
        if(scene != 0)
        {
            scene->deleteLater();
            ui->_220->setScene(0);
        }
    }

    delete ui;
}

// colour
void PlaneVerify_Dialog::on__100_valueChanged(int v)
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
void PlaneVerify_Dialog::on__101_valueChanged(int v)
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
void PlaneVerify_Dialog::on__102_valueChanged(int v)
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

// weight
void PlaneVerify_Dialog::on__103_valueChanged(double v)
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
        // special case for weight
        //if(!ui->_103->specialValueText().isEmpty() && v == 0.1)
        if(!ui->_103->specialValueText().isEmpty() && ui->_103->minimum() == v )
            variesWasSet_103 = true;
    }

    drawSample();
}

// x spacing
void PlaneVerify_Dialog::on__104_valueChanged(double v)
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

    if(ui->_1008->isChecked())
        ui->_105->setValue(v);

    drawSample();
}

// y spacing
void PlaneVerify_Dialog::on__105_valueChanged(double v)
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

    if(ui->_1008->isChecked())
        ui->_104->setValue(v);

    drawSample();
}


// angle
void PlaneVerify_Dialog::on__106_valueChanged(double v)
{
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

// cross angle
void PlaneVerify_Dialog::on__107_valueChanged(double v)
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

    drawSample();
}

void PlaneVerify_Dialog::on__1022_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1022
    wParam = MAKEWPARAM((WORD)1022,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1022;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void PlaneVerify_Dialog::on__110_clicked(bool checked)
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

void PlaneVerify_Dialog::on__111_clicked(bool checked)
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

// align hatch command
void PlaneVerify_Dialog::on__112_clicked(bool checked)
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

}

void PlaneVerify_Dialog::on__113_clicked(bool checked)
{
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

void PlaneVerify_Dialog::on__114_clicked(bool checked)
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

void PlaneVerify_Dialog::on__115_clicked(bool checked)
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

void PlaneVerify_Dialog::on__116_clicked(bool checked)
{
    // clear the filename edit box
    if(RCdlg->getcontrol(108))
    {
        RCCHAR fn[300];
        ((StringDialogControl*)RCdlg->getcontrol(108))->currentvalue(RCdlg,fn,300);
        if(fn && strlen(fn) > 0)
            ((StringDialogControl*)RCdlg->getcontrol(108))->change(RCdlg, (char*)"");
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

void PlaneVerify_Dialog::on__117_clicked(bool checked)
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

void PlaneVerify_Dialog::on__118_clicked(bool checked)
{
    // clear the filename edit box
    if(RCdlg->getcontrol(108))
    {
        RCCHAR fn[300];
        ((StringDialogControl*)RCdlg->getcontrol(108))->currentvalue(RCdlg,fn,300);
        if(fn && strlen(fn) > 0)
            ((StringDialogControl*)RCdlg->getcontrol(108))->change(RCdlg, (char*)"");
    }
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

void PlaneVerify_Dialog::on__119_clicked(bool checked)
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

    drawSample();
}

void PlaneVerify_Dialog::on__120_clicked(bool checked)
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

    drawSample();
}

void PlaneVerify_Dialog::on__121_currentIndexChanged(int index)
{
    drawSample();

}

// gradation red
/*
void PlaneVerify_Dialog::on__122_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(122))->getDefaultValue();
    if(variesWasSet_122 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(122))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(122))->setSpecialValueText(_RCT(""));
        ui->_122->setSpecialValueText("");
        variesWasSet_122 = false;
    }
    else
    {
        if(!ui->_122->specialValueText().isEmpty() && ui->_122->minimum() == v )
            variesWasSet_122 = true;
    }
    drawSample();

}
*/

// gradation green
/*
void PlaneVerify_Dialog::on__123_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(123))->getDefaultValue();
    if(variesWasSet_123 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(123))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(123))->setSpecialValueText(_RCT(""));
        ui->_123->setSpecialValueText("");
        variesWasSet_123 = false;
    }
    else
    {
        if(!ui->_123->specialValueText().isEmpty() && ui->_123->minimum() == v )
            variesWasSet_123 = true;
    }
    drawSample();

}
*/
// gradation blue
/*
void PlaneVerify_Dialog::on__124_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(124))->getDefaultValue();
    if(variesWasSet_124 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(124))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(124))->setSpecialValueText(_RCT(""));
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
*/
// transparency
void PlaneVerify_Dialog::on__125_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(125))->getDefaultValue();
    if(variesWasSet_125 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(125))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(125))->setSpecialValueText(_RCT(""));
        ui->_125->setSpecialValueText("");
        variesWasSet_125 = false;
    }
    else
    {
        if(!ui->_125->specialValueText().isEmpty() && ui->_125->minimum() == v )
            variesWasSet_125 = true;
    }

    ui->_134->setValue(v*100.0);
    ui->_303->setValue(v*100.0);
    if(sliderIsReleased)
        drawSample();
}

// transparency range
void PlaneVerify_Dialog::on__126_valueChanged(double v)
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

// transparency red
/*
void PlaneVerify_Dialog::on__127_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(127))->getDefaultValue();
    if(variesWasSet_127 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(127))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(127))->setSpecialValueText(_RCT(""));
        ui->_127->setSpecialValueText("");
        variesWasSet_127 = false;
    }
    else
    {
        if(!ui->_127->specialValueText().isEmpty() && ui->_127->minimum() == v )
            variesWasSet_127 = true;
    }
    drawSample();
}
*/
// transparency green
/*
void PlaneVerify_Dialog::on__128_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(128))->getDefaultValue();
    if(variesWasSet_128 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(128))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(128))->setSpecialValueText(_RCT(""));
        ui->_128->setSpecialValueText("");
        variesWasSet_128 = false;
    }
    else
    {
        if(!ui->_128->specialValueText().isEmpty() && ui->_128->minimum() == v )
            variesWasSet_128 = true;
    }
    drawSample();
}
*/
// transparency blue
/*
void PlaneVerify_Dialog::on__129_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(129))->getDefaultValue();
    if(variesWasSet_129 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(129))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(129))->setSpecialValueText(_RCT(""));
        ui->_129->setSpecialValueText("");
        variesWasSet_129 = false;
    }
    else
    {
        if(!ui->_129->specialValueText().isEmpty() && ui->_129->minimum() == v )
            variesWasSet_129 = true;
    }
    drawSample();
}
*/
void PlaneVerify_Dialog::on__130_valueChanged(double v)
{
    ((RealDialogControl*)this->RCdlg->getcontrol(130))->change(this->RCdlg,v);
    /*
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(134))->getDefaultValue();
    if(variesWasSet_134 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(134))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(134))->setSpecialValueText(_RCT(""));
        ui->_134->setSpecialValueText("");
        variesWasSet_134 = false;
    }
    else
    {
        if(!ui->_134->specialValueText().isEmpty() && ui->_134->minimum() == v )
            variesWasSet_134 = true;
    }
    */
    drawSample();
}

void PlaneVerify_Dialog::on__134_valueChanged(double v)
{
    double defVal = ((RealDialogControl*)this->RCdlg->getcontrol(134))->getDefaultValue();
    if(variesWasSet_134 == true)
    {
        // the *Varies* text goes away if the value is changed
        ((RealDialogControl*)this->RCdlg->getcontrol(134))->change(this->RCdlg,defVal);
        ((RealDialogControl*)this->RCdlg->getcontrol(134))->setSpecialValueText(_RCT(""));
        ui->_134->setSpecialValueText("");
        variesWasSet_134 = false;
    }
    else
    {
        if(!ui->_134->specialValueText().isEmpty() && ui->_134->minimum() == v )
            variesWasSet_134 = true;
    }

    ui->_125->setValue(v/100.0);
    ui->_303->setValue(v);
}

void PlaneVerify_Dialog::on__134_editingFinished()
{
}

void PlaneVerify_Dialog::transparencyChanged(QObject *sender, double v)
{
    if(sender->objectName() != "_125")
        ui->_125->setValue(v);
    double percentageValue = 100.0 * v;
    if(sender->objectName() != "_134")
        ui->_134->setValue(percentageValue);
    if(sender->objectName() != "_303")
        ui->_303->setValue((int)percentageValue);
    drawSample(0);
}

void PlaneVerify_Dialog::decimalTranspChanged( double v)
{
    // convert the value to a percentage and update the other transparency control (#134)
    //transparencyChanged(v);
    //double percentageValue = 100.0 * v;
    //ui->_134->setValue(percentageValue);
}

void PlaneVerify_Dialog::percentTranspChanged(double v)
{
    // convert the value to decimal and update the other transparency control (#125)
    double decimalValue = v / 100.0;
    //transparencyChanged(decimalValue);
    //ui->_125->setValue(decimalValue);
    //ui->_303->setValue(v);
}

// clone button
void PlaneVerify_Dialog::on__131_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 131
    wParam = MAKEWPARAM((WORD)131,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_131;
    dialogcb((HWND)this,msg,wParam,lParam);

}

// clone all button
void PlaneVerify_Dialog::on__132_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 132
    wParam = MAKEWPARAM((WORD)132,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_132;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void PlaneVerify_Dialog::on__1000_clicked(bool checked)
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

    //drawSample();
}

void PlaneVerify_Dialog::on__1001_clicked(bool checked)
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

    //DrawSample();
}

void PlaneVerify_Dialog::on__1002_clicked(bool checked)
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

void PlaneVerify_Dialog::on__1003_clicked(bool checked)
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

void PlaneVerify_Dialog::on__1004_clicked(bool checked)
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

    drawSample(0);
    drawSample(2);
}

void PlaneVerify_Dialog::on__1005_clicked(bool checked)
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

void PlaneVerify_Dialog::on__1007_clicked(bool checked)
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

    drawSample(0);
    drawSample(1);
}

void PlaneVerify_Dialog::on_accept()
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
        // check the state of the style radio buttons before closing
        // pass the value back to the caller before closing
        int fillstyle = 0;
        if(ui->_114->isChecked())
            fillstyle = 1;
        else if(ui->_115->isChecked())
            fillstyle = 2;
        else if(ui->_116->isChecked())
            fillstyle = 3;
        else if(ui->_117->isChecked())
            fillstyle = 4;
        else if(ui->_118->isChecked())
            fillstyle = 5;
        else if(ui->_119->isChecked())
            fillstyle = 6;
        if(fillstyle == 0)
            v.setinteger("op::lastplanefillstyle",31);
        else
            v.setinteger("op::lastplanefillstyle",fillstyle-1);

        // save the linked setting the dialog is being used to set defaults
        if(changecount == 0)
            v.setinteger("pl::spacinglinked",ui->_1008->isChecked() ? 1 : 0);

        QDialog::accept();
    }
}

void PlaneVerify_Dialog::on__303_valueChanged(int value)
{
    double percentageValue = (double)value;
    double decimalValue = percentageValue / 100.0;
    ui->_125->setValue(decimalValue);
    ui->_134->setValue(percentageValue);
    //transparencyChanged(ui->_303,decimalValue);
    //ui->_134->setValue(percentageValue);
    //percentTranspChanged(percentageValue);
    //sliderIsReleased=false;
    //drawSample();
}

// reset the preview scale
void PlaneVerify_Dialog::on__304_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 304
    wParam = MAKEWPARAM((WORD)304,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_304;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void PlaneVerify_Dialog::on__305_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 305
    wParam = MAKEWPARAM((WORD)305,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_305;
    dialogcb((HWND)this,msg,wParam,lParam);

    v.setinteger("pl::showpreview",(checked ? 1 : 0));

    drawSample(8);
}

void PlaneVerify_Dialog::drawSample(int index)
{
    // hide or show the sample dimension label and text label
    // if the fill style is no fill, hatching or solid fill
    if(ui->_114->isChecked() || ui->_115->isChecked() || ui->_117->isChecked())
    {
        ui->_133->hide();
        ui->_1333->hide();
    }
    else
    {
        ui->_133->show();
        ui->_1333->show();
    }

    if((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))
        ((PlaneDisplayDialogControl*)RCdlg->getcontrol(219))->draw(RCdlg,0);

    if((PlaneDisplayDialogControl*)RCdlg->getcontrol(220))
        ((PlaneDisplayDialogControl*)RCdlg->getcontrol(220))->draw(RCdlg,0);

    if((ColourSampleControl*)RCdlg->getcontrol(205))
        ((ColourSampleControl*)RCdlg->getcontrol(205))->draw(RCdlg,0);

    if((ColourSampleControl*)RCdlg->getcontrol(204))
        ((ColourSampleControl*)RCdlg->getcontrol(204))->draw(RCdlg,0);
}

void PlaneVerify_Dialog::on__303_sliderReleased()
{
    sliderIsReleased=true;
    drawSample(0);
}

void PlaneVerify_Dialog::on__303_sliderPressed()
{
    sliderIsReleased=false;
}

void PlaneVerify_Dialog::on__1008_clicked(bool checked)
{
    // kmj: possible future use in the config file?
    //v.setinteger("pl::spacinglinked", (checked == true ? 1 : 0));
}

void PlaneVerify_Dialog::on__104_editingFinished()
{
    if(ui->_1008->isChecked() && ui->_105->value() != ui->_104->value())
            ui->_105->setValue(ui->_104->value());
}

void PlaneVerify_Dialog::on__105_editingFinished()
{
    if(ui->_1008->isChecked() && ui->_104->value() != ui->_105->value())
            ui->_104->setValue(ui->_105->value());

}

void PlaneVerify_Dialog::on__1009_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1009
    wParam = MAKEWPARAM((WORD)1009,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1009;
    dialogcb((HWND)this,msg,wParam,lParam);

    drawSample();
}

void PlaneVerify_Dialog::on__1010_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1010
    wParam = MAKEWPARAM((WORD)1010,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1010;
    dialogcb((HWND)this,msg,wParam,lParam);

     drawSample();
}

void PlaneVerify_Dialog::on__1011_clicked(bool checked)
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
