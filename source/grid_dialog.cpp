
#include "ncwin.h"

#include "RCDialog.h"
#include "grid_dialog.h"
#include "ui_grid_dialog.h"

bool GridDialogMousePressFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress /*||
        event->type() == QEvent::MouseButtonDblClick*/)
    {
        if(obj->objectName()== "_111" && ((QWidget*)obj)->isEnabled())
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
                ((Grid_Dialog*)obj->parent())->on__1110_clicked(false);
            return true;
        }
        else if( obj->objectName()== "_113" && ((QWidget*)obj)->isEnabled())
        {
            if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
                ((Grid_Dialog*)obj->parent())->on__1130_clicked(false);
            return true;
        }
        else
            return QObject::eventFilter(obj, event);
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

Grid_Dialog::Grid_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Grid_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Grid_Dialog::Grid_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent, parent, f),
    ui(new Ui::Grid_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  // add a scene to the graphics view
  QGraphicsScene *scene = new QGraphicsScene;
  if(ui->_111)
      ui->_111->setScene(scene);
  ((GridColourSampleControl*)RCdlg->getcontrol(111))->setQW(ui->_111);
  ((GridColourSampleControl*)RCdlg->getcontrol(111))->sethwnd(ui->_111);


  scene = new QGraphicsScene;
  if(ui->_113)
      ui->_113->setScene(scene);
  ((GridColourSampleControl*)RCdlg->getcontrol(113))->setQW(ui->_113);
  ((GridColourSampleControl*)RCdlg->getcontrol(113))->sethwnd(ui->_113);

  // put any inits not know to the realCAD Dialog class here
  // init the realCAD Dialog class here
  initDialog(this);

  // enable the colour swatches depending on the setting
  style = ((v.getinteger("gd::options") & 16) == 16);
  custcolour = ((v.getinteger("gd::options") & 32) == 32);

  on_styleCol_changed();

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

  GridDialogMousePressFilter *mousePressFilter = new GridDialogMousePressFilter(this);
  ui->_111->installEventFilter(mousePressFilter);
  ui->_113->installEventFilter(mousePressFilter);

}

Grid_Dialog::~Grid_Dialog()
{
    delete ui;
}

void Grid_Dialog::on_accept( )
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

void Grid_Dialog::on__105_clicked(bool checked)
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
}

void Grid_Dialog::on__106_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 106
    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Grid_Dialog::on__107_clicked(bool checked)
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
}

void Grid_Dialog::on__108_clicked(bool checked)
{
    style = checked;
    on_styleCol_changed();

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

void Grid_Dialog::on__109_clicked(bool checked)
{
    custcolour = checked;
    on_styleCol_changed();

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

void Grid_Dialog::on__1110_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1110 minor line colour button
    wParam = MAKEWPARAM((WORD)1110,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1110;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Grid_Dialog::on__1130_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1130 major grid line colour
    wParam = MAKEWPARAM((WORD)1130,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1130;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Grid_Dialog::on_styleCol_changed()
{
    if(style == 0)
    {
        ui->_110->setText("Grid colour");
        ui->_110->setEnabled(custcolour);
        ui->_111->setEnabled(custcolour);
        ui->_1110->setEnabled(custcolour);
        ui->_112->setEnabled(false);
        ui->_113->setEnabled(false);
        ui->_1130->setEnabled(false);
        ui->_112->hide();
        ui->_113->hide();
        ui->_1130->hide();
    }
    else
    {
        ui->_110->setText("Reference grid line colour");
        ui->_110->setEnabled(custcolour);
        ui->_111->setEnabled(custcolour);
        ui->_1110->setEnabled(custcolour);
        ui->_112->show();
        ui->_113->show();
        ui->_1130->show();
        ui->_112->setEnabled(custcolour);
        ui->_113->setEnabled(custcolour);
        ui->_1130->setEnabled(custcolour);
    }
}
