
#include "ncwin.h"
#include "RCDialog.h"
#include "selecthatchpattern_dialog.h"
#include "ui_selecthatchpattern_dialog.h"

bool SelectHatchEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    qDebug() << "clicked object : " << obj->objectName();
    qDebug() << "event type     : " << event->type();
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<SelectHatchPattern_Dialog *>(dialog) != 0)
            {
                int id = RCnameToInt(obj->objectName());
                if(id >= 100 && id <= 112)
                    ((SelectHatchPattern_Dialog*)dialog)->on_picture_clicked(id);
            }
        }
        return false;
    }
    else if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<SelectHatchPattern_Dialog *>(dialog) != 0)
        {
            if(((QKeyEvent*)event)->key() == Qt::Key_Home)// to the top of the page page scrolls down visible line == min
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderToMinimum);
            else if(((QKeyEvent*)event)->key() == Qt::Key_End)// to the bottom of the page page scrolls up visible == max
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderToMaximum);
            else if(((QKeyEvent*)event)->key() == Qt::Key_Up)// scroll bar up page scrolls down visible line -=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderSingleStepSub);
            else if(((QKeyEvent*)event)->key() == Qt::Key_Down)// scroll bar down page scrolls up visible line +=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            else if(((QKeyEvent*)event)->key() == Qt::Key_PageUp)// scroll bar up page scrolls up visible line -=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderPageStepSub);
            else if(((QKeyEvent*)event)->key() == Qt::Key_PageDown)// scroll bar down page scrolls up visible line +=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderPageStepAdd);
        }
        return false;
    }
    else if (event->type() == QEvent::Wheel)
    {
        if(dialog && qobject_cast<SelectHatchPattern_Dialog *>(dialog) != 0)
        {
            if(((QWheelEvent*)event)->angleDelta().y() < 0)// rotated backwards towards the user; page scrolls up visible line +=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            else if(((QWheelEvent*)event)->angleDelta().y() > 0)// rotated forwards away from the user; page scrolls down visible line -=
                ((SelectHatchPattern_Dialog*)dialog)->ui->_118->triggerAction(QAbstractSlider::SliderSingleStepSub);
        }
        return false;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

SelectHatchPattern_Dialog::SelectHatchPattern_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SelectHatchPattern_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SelectHatchPattern_Dialog::SelectHatchPattern_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SelectHatchPattern_Dialog)
{
  ui->setupUi(this);

  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_100)
      ui->_100->setScene(scene);
  ui->_100->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(100))->setQW(ui->_100);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(100))->sethwnd(ui->_100);
  scene = new QGraphicsScene;
  if(ui->_101)
      ui->_101->setScene(scene);
  ui->_101->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(101))->setQW(ui->_101);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(101))->sethwnd(ui->_101);
  scene = new QGraphicsScene;
  if(ui->_102)
      ui->_102->setScene(scene);
  ui->_102->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(102))->setQW(ui->_102);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(102))->sethwnd(ui->_102);
  scene = new QGraphicsScene;
  if(ui->_103)
      ui->_103->setScene(scene);
  ui->_103->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(103))->setQW(ui->_103);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(103))->sethwnd(ui->_103);
  scene = new QGraphicsScene;
  if(ui->_104)
      ui->_104->setScene(scene);
  ui->_104->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(104))->setQW(ui->_104);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(104))->sethwnd(ui->_104);
  scene = new QGraphicsScene;
  if(ui->_105)
      ui->_105->setScene(scene);
  ui->_105->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(105))->setQW(ui->_105);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(105))->sethwnd(ui->_105);
  scene = new QGraphicsScene;
  if(ui->_106)
      ui->_106->setScene(scene);
  ui->_106->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(106))->setQW(ui->_106);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(106))->sethwnd(ui->_106);
  scene = new QGraphicsScene;
  if(ui->_107)
      ui->_107->setScene(scene);
  ui->_107->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(107))->setQW(ui->_107);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(107))->sethwnd(ui->_107);
  scene = new QGraphicsScene;
  if(ui->_108)
      ui->_108->setScene(scene);
  ui->_108->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(108))->setQW(ui->_108);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(108))->sethwnd(ui->_108);
  scene = new QGraphicsScene;
  if(ui->_109)
      ui->_109->setScene(scene);
  ui->_109->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(109))->setQW(ui->_109);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(109))->sethwnd(ui->_109);
  scene = new QGraphicsScene;
  if(ui->_110)
      ui->_110->setScene(scene);
  ui->_110->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(110))->setQW(ui->_110);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(110))->sethwnd(ui->_110);
  scene = new QGraphicsScene;
  if(ui->_111)
      ui->_111->setScene(scene);
  ui->_111->setSceneRect(QRectF());
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(111))->setQW(ui->_111);
  ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(111))->sethwnd(ui->_111);

  ui->_300->setVisible(false);
  ui->_301->setVisible(false);
  ui->_302->setVisible(false);
  ui->_303->setVisible(false);
  ui->_304->setVisible(false);
  ui->_305->setVisible(false);
  ui->_306->setVisible(false);
  ui->_307->setVisible(false);
  ui->_308->setVisible(false);
  ui->_309->setVisible(false);
  ui->_310->setVisible(false);
  ui->_311->setVisible(false);

  initDialog(this);

  //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  SelectHatchEventFilter *Filter = new SelectHatchEventFilter(this);
  ui->_100->installEventFilter(Filter);
  ui->_101->installEventFilter(Filter);
  ui->_102->installEventFilter(Filter);
  ui->_103->installEventFilter(Filter);
  ui->_104->installEventFilter(Filter);
  ui->_105->installEventFilter(Filter);
  ui->_106->installEventFilter(Filter);
  ui->_107->installEventFilter(Filter);
  ui->_108->installEventFilter(Filter);
  ui->_109->installEventFilter(Filter);
  ui->_110->installEventFilter(Filter);
  ui->_111->installEventFilter(Filter);

  draw();
}


void SelectHatchPattern_Dialog::on__114_currentIndexChanged(int index)
{
    draw();
}

SelectHatchPattern_Dialog::~SelectHatchPattern_Dialog()
{
    delete ui;
}

// picture buttons
void SelectHatchPattern_Dialog::on_picture_clicked(int id)
{
        // using the dialog callback function
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;

        msg = WM_COMMAND;

        // click 100
        wParam = MAKEWPARAM((WORD)id,(WORD)BN_CLICKED);
        //lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
}


// control buttons
// previous
void SelectHatchPattern_Dialog::on__112_clicked(bool checked)
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

        //draw();
}

// next
void SelectHatchPattern_Dialog::on__113_clicked(bool checked)
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

        //draw();
}

// first
void SelectHatchPattern_Dialog::on__116_clicked(bool checked)
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

        //draw();
}

// last
void SelectHatchPattern_Dialog::on__117_clicked(bool checked)
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

        //draw();
}

// browse
void SelectHatchPattern_Dialog::on__115_clicked(bool checked)
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

        //draw();
}

// scroll bar
void SelectHatchPattern_Dialog::on__118_actionTriggered(int action)
{
    int scrollcode;
    int pos = ((QScrollBar*)ui->_118)->sliderPosition();
    switch (action)
    {
    case QAbstractSlider::SliderNoAction:
    default: // 0
        scrollcode = SB_ENDSCROLL;
        break;
    case QAbstractSlider::SliderSingleStepAdd: // 1
        scrollcode = SB_LINEDOWN;// page scroll up
        break;
    case QAbstractSlider::SliderSingleStepSub: // 2
        scrollcode = SB_LINEUP;// page scrolls down
        break;
    case QAbstractSlider::SliderPageStepAdd: // 3
        scrollcode = SB_PAGEDOWN;// page scrolls down
        break;
    case QAbstractSlider::SliderPageStepSub: // 4
        scrollcode = SB_PAGEUP;// page scrolls up
        break;
    case QAbstractSlider::SliderToMinimum: // 5
        scrollcode = SB_TOP;
        break;
    case QAbstractSlider::SliderToMaximum: // 6
        scrollcode = SB_BOTTOM;
        break;
   case QAbstractSlider::SliderMove: // 7
        scrollcode = 10;
        break;
    }

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_VSCROLL;

    // click 118
    wParam = MAKEWPARAM((WORD)scrollcode,(WORD)pos);
    lParam = (LPARAM)ui->_118;
    dialogcb((HWND)this,msg,wParam,lParam);
}

 void SelectHatchPattern_Dialog::trigger_118(int action)
 {
     ui->_118->actionTriggered(action);
 }

void SelectHatchPattern_Dialog::draw()
{
    //((SelectHatchPattern_Dialog*)RCdlg->gethdlg())->draw();

    for(int i=100; i<112; i++)
    {
        DisplayDialogControl *ddc;
        GeneralList list = RCdlg->getdisplaylist();
        for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
        {
            if(i == ddc->getid())
                ddc->draw(RCdlg,0);
        }

    }

}
