
#include "ncwin.h"

#include "RCDialog.h"
#include "definestyle_dialog.h"
#include "ui_definestyle_dialog.h"

DefineStyle_Dialog::DefineStyle_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
      RCDialog(rcparent, parent, f),
      ui(new Ui::DefineStyle_Dialog)
{
    ui->setupUi(this);
	ActivateParent = false;
    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_105)
        ui->_105->setScene(scene);

    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->sethwnd(ui->_105);

    // put any inits not know to the realCAD Dialog class here
    // init the realCAD Dialog class here
    initDialog(this);

    //connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    //connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());
}

DefineStyle_Dialog::~DefineStyle_Dialog()
{
    delete ui;
}

void DefineStyle_Dialog::on__100_valueChanged(int v)
{
    //draw_sample();
    //((IntegerDialogControl*)RCdlg->getcontrol(100))->change(RCdlg,v);
    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineStyle_Dialog::on__101_valueChanged(int v)
{
    //draw_sample();
    //((IntegerDialogControl*)RCdlg->getcontrol(101))->change(RCdlg,v);
    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());

}

void DefineStyle_Dialog::on__102_valueChanged(int v)
{
    //draw_sample();
    //((IntegerDialogControl*)RCdlg->getcontrol(102))->change(RCdlg,v);
    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());

}

void DefineStyle_Dialog::on__103_valueChanged(double v)
{
    //draw_sample();
    //((RealDialogControl*)RCdlg->getcontrol(103))->change(RCdlg,v);
    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineStyle_Dialog::on__104_valueChanged(double v)
{
    //draw_sample();
    //((RealDialogControl*)RCdlg->getcontrol(104))->change(RCdlg,v);
    ((StyleDisplayDialogControl*)RCdlg->getcontrol(105))->draw(RCdlg,RCdlg->gethdlg());
}

void DefineStyle_Dialog::on__106_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // 106 clicked
    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void  DefineStyle_Dialog::draw_sample()
{
    int index,nsegments,csegment;
    double dash,space;

    int i,colour,style;
    double weight;
    LineStyle linestyle;
    QGraphicsView *ctl = 0;

    if((ctl=ui->_105) != 0)
    {
        QGraphicsScene *scene = ctl->scene();
        if(scene)
        {
            //scene->addText("Hello, world!");
            scene->setSceneRect(QRectF());
            scene->clear();
            QRectF rect = ctl->rect();

            scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                    ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                    ::v.getreal("wn::backgroundcolour.blue")*255.0)));


            if (RCdlg->currentvalue(101,&nsegments) &&
                RCdlg->currentvalue(102,&csegment) &&
                RCdlg->currentvalue(103,&dash) &&
                RCdlg->currentvalue(104,&space))
              {
                RCdlg->currentvalue(100,&index);
                linestyle.setnsegments(nsegments);
                 if (nsegments > 0)
                   linestyle.setsegment(csegment,dash,space);
                 // setup colour and width
                 QColor penColour(db.colourtable[db.header.getcolour()].red,db.colourtable[db.header.getcolour()].green,db.colourtable[db.header.getcolour()].blue);
                 int ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));
                 qreal penWidth = int((ydotspermm < 3 ? 3 : ydotspermm) * db.lineweights.weighttomm(db.header.getweight()) + 0.5);
                 QPen pen(penColour,penWidth,Qt::SolidLine,Qt::FlatCap);

                 if (nsegments > 0)
                 {
                     linestyle.start(0.5,rect.right() - 20);
                     linestyle.draw(ctl,rect.left() + 10,(rect.top() + rect.bottom()) / 2,
                                     rect.right() - 10,(rect.top() + rect.bottom()) / 2,
                                     pen);

                 }
                 else
                 {
                     scene->addLine(rect.left()+10,rect.height()-(rect.top() + rect.bottom()) / 2,
                                    rect.right()-10,rect.height()-(rect.top() + rect.bottom()) / 2,
                                    pen);
                 }
            }
        }
    }
}

void DefineStyle_Dialog::on__1_clicked(bool checked)
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

    //QDialog::accept();
}

void DefineStyle_Dialog::on__2_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDCANCEL,(WORD)0);
    lParam = (LPARAM)ui->_2;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::reject();
}
