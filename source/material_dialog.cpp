#include "ncwin.h"
#include "RCDialog.h"
#include "material_dialog.h"
#include "ui_material_dialog.h"

bool SelectMaterialDlgventFilter::eventFilter(QObject *obj, QEvent *event)
{
    qDebug() << "clicked object : " << obj->objectName();
    qDebug() << "event type     : " << event->type();
    QWidget *dialog = app->activeModalWidget();
    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(dialog && qobject_cast<Material_Dialog *>(dialog) != 0)
            {
                int id = RCnameToInt(obj->objectName());
                if(id >= 109 && id <= 121)
                    ((Material_Dialog*)dialog)->on_picture_clicked(id);
            }
        }
        return false;
    }
    else if (event->type() == QEvent::KeyPress)
    {
        if(dialog && qobject_cast<Material_Dialog *>(dialog) != 0)
        {
            if(((QKeyEvent*)event)->key() == Qt::Key_Home)// to the top of the page page scrolls down visible line == min
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderToMinimum);
            else if(((QKeyEvent*)event)->key() == Qt::Key_End)// to the bottom of the page page scrolls up visible == max
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderToMaximum);
            else if(((QKeyEvent*)event)->key() == Qt::Key_Up)// scroll bar up page scrolls down visible line -=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderSingleStepSub);
            else if(((QKeyEvent*)event)->key() == Qt::Key_Down)// scroll bar down page scrolls up visible line +=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            else if(((QKeyEvent*)event)->key() == Qt::Key_PageUp)// scroll bar up page scrolls up visible line -=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderPageStepSub);
            else if(((QKeyEvent*)event)->key() == Qt::Key_PageDown)// scroll bar down page scrolls up visible line +=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderPageStepAdd);
        }
        return false;
    }
    else if (event->type() == QEvent::Wheel)
    {
        if(dialog && qobject_cast<Material_Dialog *>(dialog) != 0)
        {
            if(((QWheelEvent*)event)->angleDelta().y() < 0)// rotated backwards towards the user; page scrolls up visible line +=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            else if(((QWheelEvent*)event)->angleDelta().y() > 0)// rotated forwards away from the user; page scrolls down visible line -=
                ((Material_Dialog*)dialog)->ui->_122->triggerAction(QAbstractSlider::SliderSingleStepSub);
        }
        return false;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

Material_Dialog::Material_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Material_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
Material_Dialog::Material_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::Material_Dialog)
{
    ui->setupUi(this);


    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_109)
        ui->_109->setScene(scene);
    ui->_109->setSceneRect(QRectF());
    ((MaterialTextureDisplayDialogControl*)RCdlg->getdisplaycontrol(109))->setQW(ui->_109);
    ((MaterialTextureDisplayDialogControl*)RCdlg->getdisplaycontrol(109))->sethwnd(ui->_109);

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
    scene = new QGraphicsScene;
    if(ui->_112)
        ui->_112->setScene(scene);
    ui->_112->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(112))->setQW(ui->_112);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(112))->sethwnd(ui->_112);
    scene = new QGraphicsScene;
    if(ui->_113)
        ui->_113->setScene(scene);
    ui->_113->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(113))->setQW(ui->_113);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(113))->sethwnd(ui->_113);
    scene = new QGraphicsScene;
    if(ui->_114)
        ui->_114->setScene(scene);
    ui->_114->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(114))->setQW(ui->_114);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(114))->sethwnd(ui->_114);
    scene = new QGraphicsScene;
    if(ui->_115)
        ui->_115->setScene(scene);
    ui->_115->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(115))->setQW(ui->_115);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(115))->sethwnd(ui->_115);
    scene = new QGraphicsScene;
    if(ui->_116)
        ui->_116->setScene(scene);
    ui->_116->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(116))->setQW(ui->_116);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(116))->sethwnd(ui->_116);
    scene = new QGraphicsScene;
    if(ui->_117)
        ui->_117->setScene(scene);
    ui->_117->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(117))->setQW(ui->_117);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(117))->sethwnd(ui->_117);
    scene = new QGraphicsScene;
    if(ui->_118)
        ui->_118->setScene(scene);
    ui->_118->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(118))->setQW(ui->_118);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(118))->sethwnd(ui->_118);
    scene = new QGraphicsScene;
    if(ui->_119)
        ui->_119->setScene(scene);
    ui->_119->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(119))->setQW(ui->_119);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(119))->sethwnd(ui->_119);
    scene = new QGraphicsScene;
    if(ui->_120)
        ui->_120->setScene(scene);
    ui->_120->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(120))->setQW(ui->_120);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(120))->sethwnd(ui->_120);
    scene = new QGraphicsScene;
    if(ui->_121)
        ui->_121->setScene(scene);
    ui->_121->setSceneRect(QRectF());
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(121))->setQW(ui->_121);
    ((HPDisplayDialogControl*)RCdlg->getdisplaycontrol(121))->sethwnd(ui->_121);

    ui->_310->setVisible(false);
    ui->_311->setVisible(false);
    ui->_312->setVisible(false);
    ui->_313->setVisible(false);
    ui->_314->setVisible(false);
    ui->_315->setVisible(false);
    ui->_316->setVisible(false);
    ui->_317->setVisible(false);
    ui->_318->setVisible(false);
    ui->_319->setVisible(false);
    ui->_320->setVisible(false);
    ui->_321->setVisible(false);

    // init the realCAD Dialog class here
    initDialog(this);

    // hide the angle spin box and label
    ui->_307->hide();
    ui->_106->hide();
    // hide the flip check box
    ui->_107->hide();

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    SelectMaterialDlgventFilter *Filter = new SelectMaterialDlgventFilter(this);
    ui->_110->installEventFilter(Filter);
    ui->_111->installEventFilter(Filter);
    ui->_112->installEventFilter(Filter);
    ui->_113->installEventFilter(Filter);
    ui->_114->installEventFilter(Filter);
    ui->_115->installEventFilter(Filter);
    ui->_116->installEventFilter(Filter);
    ui->_117->installEventFilter(Filter);
    ui->_118->installEventFilter(Filter);
    ui->_119->installEventFilter(Filter);
    ui->_120->installEventFilter(Filter);
    ui->_121->installEventFilter(Filter);
    ui->_122->installEventFilter(Filter);

    draw();
}

Material_Dialog::~Material_Dialog()
{
    delete ui;
}

void Material_Dialog::accept()
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
        QDialog::accept();
    }
}

void Material_Dialog::draw()
{
    for(int i=109; i<122; i++)
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

// picture buttons
void Material_Dialog::on_picture_clicked(int id)
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

void Material_Dialog::on__101_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_101;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Material_Dialog::on__124_clicked()
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

void Material_Dialog::on__123_currentIndexChanged(int index)
{
    draw();
}

void Material_Dialog::on__122_actionTriggered(int action)
{
    int scrollcode;
    int pos = ((QScrollBar*)ui->_122)->sliderPosition();
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

    // click 122
    wParam = MAKEWPARAM((WORD)scrollcode,(WORD)pos);
    lParam = (LPARAM)ui->_122;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Material_Dialog::on__102_valueChanged(double v) // xoffset
{
    ((RealDialogControl*)this->RCdlg->getcontrol(102))->change(this->RCdlg,v);
}

void Material_Dialog::on__103_valueChanged(double v) // yoffset
{
    ((RealDialogControl*)this->RCdlg->getcontrol(103))->change(this->RCdlg,v);
}

void Material_Dialog::on__104_valueChanged(double v) // width
{
    ((RealDialogControl*)this->RCdlg->getcontrol(104))->change(this->RCdlg,v);
}

void Material_Dialog::on__105_valueChanged(double v) // height
{
    ((RealDialogControl*)this->RCdlg->getcontrol(105))->change(this->RCdlg,v);
}

void Material_Dialog::on__106_valueChanged(double v) // angle
{
    ((RealDialogControl*)this->RCdlg->getcontrol(106))->change(this->RCdlg,v);
}

void Material_Dialog::on__108_valueChanged(double v) // transparency
{
    ((RealDialogControl*)this->RCdlg->getcontrol(108))->change(this->RCdlg,v);
}

void Material_Dialog::on__107_clicked(bool checked) // flip the image
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

