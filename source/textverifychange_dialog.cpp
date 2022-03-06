
#include "ncwin.h"

#include "RCDialog.h"
#include "TextVerifyChange_Dialog.h"
#include "ui_TextVerifyChange_Dialog.h"

bool TextVerifyMousePressFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            int d1=0,d2=0,d3=0;
            QPoint ipos = ((QMouseEvent*)event)->pos();
            QWidget *dialog = app->activeModalWidget();

            if(obj->objectName()== "_100")
            { // colour
                if(dialog)
                {
                    ((TextVerifyChange_Dialog*)dialog)->ui->_100->rect().contains(ipos);
                    if(((TextVerifyChange_Dialog*)dialog)->rcDialog() && ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(100))
                        ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(100)->lbuttondblclkevent(((TextVerifyChange_Dialog*)dialog)->rcDialog(),d1,d2,d3);
                }
            }
            else if(obj->objectName()== "_101")
            { // layer
                if(dialog)
                {
                    ((TextVerifyChange_Dialog*)dialog)->ui->_101->rect().contains(ipos);
                    if(((TextVerifyChange_Dialog*)dialog)->rcDialog() && ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(101))
                        ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(101)->lbuttondblclkevent(((TextVerifyChange_Dialog*)dialog)->rcDialog(),d1,d2,d3);
                }
            }
            else if(obj->objectName()== "_102")
            { // style
                if(dialog)
                {
                    ((TextVerifyChange_Dialog*)dialog)->ui->_102->rect().contains(ipos);
                    if(((TextVerifyChange_Dialog*)dialog)->rcDialog() && ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(102))
                        ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(102)->lbuttondblclkevent(((TextVerifyChange_Dialog*)dialog)->rcDialog(),d1,d2,d3);
                }
            }
            else if(obj->objectName()== "_103")
            { // weight
                if(dialog)
                {
                    ((TextVerifyChange_Dialog*)dialog)->ui->_103->rect().contains(ipos);
                    if(((TextVerifyChange_Dialog*)dialog)->rcDialog() && ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(103))
                        ((TextVerifyChange_Dialog*)dialog)->rcDialog()->getcontrol(103)->lbuttondblclkevent(((TextVerifyChange_Dialog*)dialog)->rcDialog(),d1,d2,d3);
                }
            }
        }
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}


TextVerifyChange_Dialog::TextVerifyChange_Dialog(HANDLE parent, HANDLE rcparent, int f):
	TextVerifyChange_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
TextVerifyChange_Dialog::TextVerifyChange_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::TextVerifyChange_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  if(v.getinteger("tx::label"))
  {
      // enable the label prompt text box
      ui->_119->setDisabled(false);
      //ui->_119->setReadOnly(false);
  }
  else
  {
      // disable the label prompt text box
      ui->_119->setDisabled(true);
      //ui->_119->setReadOnly(true);
  }

  initDialog(this);

  ui->_101->selectAll();

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  //TextVerifyMousePressFilter *mousePressFilter = new TextVerifyMousePressFilter(this);
  //ui->_100->installEventFilter(mousePressFilter);
  //ui->_101->installEventFilter(mousePressFilter);
  //ui->_102->installEventFilter(mousePressFilter);
  //ui->_103->installEventFilter(mousePressFilter);
}

TextVerifyChange_Dialog::~TextVerifyChange_Dialog()
{
  delete ui;
}

void TextVerifyChange_Dialog::on__100_valueChanged(int v)
{
    ((IntegerDialogControl*)RCdlg->getcontrol(100))->change(RCdlg,v);

}

void TextVerifyChange_Dialog::on__101_valueChanged(int v)
{
    ((IntegerDialogControl*)RCdlg->getcontrol(101))->change(RCdlg,v);

}

void TextVerifyChange_Dialog::on__102_valueChanged(int v)
{
    ((IntegerDialogControl*)RCdlg->getcontrol(102))->change(RCdlg,v);

}

void TextVerifyChange_Dialog::on__103_valueChanged(int v)
{
    ((IntegerDialogControl*)RCdlg->getcontrol(103))->change(RCdlg,v);

}

void TextVerifyChange_Dialog::on__1000_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__1001_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__1002_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__1003_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__1004_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__112_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__113_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__114_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__115_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__116_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__120_clicked(bool checked)
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

void TextVerifyChange_Dialog::on__122_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on__123_clicked(bool checked)
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
}

void TextVerifyChange_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}
