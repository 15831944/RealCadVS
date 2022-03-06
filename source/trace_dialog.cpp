
#include "ncwin.h"

#include "RCView.h"
#include "RCDialog.h"
#include "trace_dialog.h"
#include "ui_trace_dialog.h"


Trace_Dialog::Trace_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Trace_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Trace_Dialog::Trace_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
#if 1
    QDialog(parent,f),
#else
    // if using realcad stuff
    RCDialog(rcparent,parent,f),
#endif
    ui(new Ui::Trace_Dialog)
{
  ui->setupUi(this);
#if 0
  // if using realcad dialog stuff
  initDialog(this);
#endif
  
  ui->_6->setFocus();

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  ui->_6->installEventFilter(this);
  ui->_7->installEventFilter(this);

  //QPoint winpos(((QtMainWindow*)cadwindow->gethwnd())->pos());
  QPoint winpos(((QtMainWindow*)cadwindow->gethwnd())->centralWidget()->pos());
  move(winpos.x(),winpos.y());
}

Trace_Dialog::~Trace_Dialog()
{
    delete ui;
}

bool Trace_Dialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Y)
        {
            on__6_clicked(false);
            return true;
        }
        else if(keyEvent->key() == Qt::Key_N)
        {
            on__7_clicked(false);
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void Trace_Dialog::on__6_clicked(bool checked)
{
#if 1
    // using the Qt method
    emit gotYes();
    done(IDYES);
#else
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 6
    wParam = MAKEWPARAM((WORD)6,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_6;
    dialogcb((HWND)this,msg,wParam,lParam);
#endif
}

void Trace_Dialog::on__7_clicked(bool checked)
{
#if 1
    // using the Qt method
    emit gotNo();
    done(IDNO);
#else
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 7
    wParam = MAKEWPARAM((WORD)7,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_7;
    dialogcb((HWND)this,msg,wParam,lParam);
#endif
}

