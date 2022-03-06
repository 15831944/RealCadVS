
#include "ncwin.h"

#include "RCDialog.h"
#include "weightspinbox.h"
#include "Styleset_Dialog.h"
#include "ui_Styleset_Dialog.h"

class SSDisplayDialogControl;

Styleset_Dialog::Styleset_Dialog(HANDLE parent, HANDLE rcparent, int f)
	:Styleset_Dialog((QWidget*)parent,(Dialog*)rcparent,(Qt::WindowFlags)f)
{
}

Styleset_Dialog::Styleset_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
     // QDialog(parent,f),
      RCDialog(rcparent,parent,f),
      ui(new Ui::Styleset_Dialog)
{

    ui->setupUi(this);
	ActivateParent = false;
    // add a scene to the graphics view
    QGraphicsScene *scene = new QGraphicsScene;
    if(ui->_106)
        ui->_106->setScene(scene);

    ((SSDisplayDialogControl*)RCdlg->getcontrol(106))->setQW(ui->_106);
    // init the realCAD Dialog class here

    initDialog(this);

    // any extra inits go here

    // we must reset the contents to reflect the current style bar settings
    // which the normal init does not do
    int i,colour,style,layer;
    double weight;

    ui->_100->clear();
    ((SSListDialogControl *)RCdlg->getcontrol(100))->removeallitems(RCdlg);
    ((SSListDialogControl *)RCdlg->getcontrol(100))->load(RCdlg);
    ui->_100->addItem("");

    colour = db.header.getcolour();
    //qDebug() <<  "colour " << colour;
    layer = db.header.getlayer();
    //qDebug() <<  "layer " << layer;
    style = db.header.getstyle();
    //qDebug() <<  "style " << style;
    weight = db.lineweights.weighttomm(db.header.getweight());
    //qDebug() <<  "weight " << weight;

    ((IntegerDialogControl *)RCdlg->getcontrol(101))->change(RCdlg,colour);
    ((IntegerDialogControl *)RCdlg->getcontrol(102))->change(RCdlg,layer);
    ((IntegerDialogControl *)RCdlg->getcontrol(103))->change(RCdlg,style);
    ((WeightAssocRealDialogControl *)RCdlg->getcontrol(104))->change(RCdlg,weight);

    RCCHAR label[300],name[300],key[300],value[300],*s;
    //  Try to match the current settings with a style.
    for (i = 1 ; ; i++)
      {  sprintf(key,"StyleSet%d",i);
         strcpy(value,"");
         strcpy(name,"");
         GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
         //qDebug() <<  "style set : key " << QString(key) << " value : " << QString(value);
         if (strlen(value) == 0)
           break;
         s = strtok(value,_RCT(","));
         if( s != 0 )
             strcpy(name,s);

         //qDebug() <<  "name " << QString(name);

         s = strtok(0,_RCT(","));
         if (s != 0 && db.header.getcolour() != atoi(s))
           continue;  //  Doesn't match
         //qDebug() <<  "colour matched : " << QString(s);
         s = strtok(0,_RCT(","));
         if (s != 0 && db.header.getlayer() != atoi(s))
           continue;
         //qDebug() <<  "layer matched : " << QString(s);
         s = strtok(0,_RCT(","));
         if (s != 0 && db.header.getstyle() != atoi(s))
           continue;
         //qDebug() <<  "style matched : " << QString(s);
         s = strtok(0,_RCT(","));
		 if (atof(s) == 0.0) break;
         if (s != 0 && fabs(db.lineweights.weighttomm(db.header.getweight())-atof(s)) > 0.001)
           continue;
         //qDebug() << "weight matched : " << QString(s);
         break;
      }

    //qDebug() << " style name : " << QString(name);
    ui->_100->setEditText(QString(name));

    draw_sample();

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Styleset_Dialog::~Styleset_Dialog()
{
    delete ui;
}

void Styleset_Dialog::on__100_currentTextChanged(const QString & text)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    wParam = MAKEWPARAM((WORD)100,(WORD)CBN_SELCHANGE);
    lParam = (LPARAM)ui->_100;
    dialogcb((HWND)this,msg,wParam,lParam);

    updateAll();
}

void Styleset_Dialog::on__101_valueChanged(int v)
{
    //draw_sample();
    ((IntegerDialogControl*)RCdlg->getcontrol(101))->change(RCdlg,v);
    RCdlg->updatecustomcontrols();
    //((SSDisplayDialogControl*)RCdlg->getcontrol(106))->draw(RCdlg,RCdlg->gethdlg());
}

void Styleset_Dialog::on__102_valueChanged(int v)
{
    //draw_sample();
    ((IntegerDialogControl*)RCdlg->getcontrol(102))->change(RCdlg,v);
    RCdlg->updatecustomcontrols();
    //((SSDisplayDialogControl*)RCdlg->getcontrol(106))->draw(RCdlg,RCdlg->gethdlg());
}

void Styleset_Dialog::on__103_valueChanged(int v)
{
    //draw_sample();
    ((IntegerDialogControl*)RCdlg->getcontrol(103))->change(RCdlg,v);
    RCdlg->updatecustomcontrols();
    //((SSDisplayDialogControl*)RCdlg->getcontrol(106))->draw(RCdlg,RCdlg->gethdlg());
}

void Styleset_Dialog::on__104_valueChanged(double v)
{
    //draw_sample();
    ((WeightAssocRealDialogControl*)RCdlg->getcontrol(104))->change(RCdlg,v);
    RCdlg->updatecustomcontrols();
    //((SSDisplayDialogControl*)RCdlg->getcontrol(106))->draw(RCdlg,RCdlg->gethdlg());
}

void Styleset_Dialog::on__105_clicked(bool checked)
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

void Styleset_Dialog::on__107_clicked(bool checked)
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

void Styleset_Dialog::on__108_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 107
    wParam = MAKEWPARAM((WORD)108,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_108;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Styleset_Dialog::on__109_clicked(bool checked)
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

void Styleset_Dialog::on__110_clicked(bool checked)
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

void Styleset_Dialog::on__111_clicked(bool checked)
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

void Styleset_Dialog::on__112_clicked(bool checked)
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

void Styleset_Dialog::on_accept()
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

void  Styleset_Dialog::draw_sample()
{
    int i,colour,style;
    double weight;
    LineStyle *linestyle;
    QGraphicsView *ctl = 0;

    if((ctl=ui->_106) != 0)
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


            if (RCdlg->currentvalue(101,&colour) &&
                RCdlg->currentvalue(103,&style) &&
                RCdlg->currentvalue(104,&weight))
              {
                //qDebug() << "SSDisplayDialogControl::draw drawing the sample";
                //qDebug() << "SSDisplayDialogControl::draw : colour : " <<  colour;
                //qDebug() << "SSDisplayDialogControl::draw : style  : " <<  style;
                //qDebug() << "SSDisplayDialogControl::draw : weight : " <<  weight;

                 // setup colour, style and width
                 QColor penColour(db.colourtable[colour].red,db.colourtable[colour].green,db.colourtable[colour].blue);
                 int ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));
                 qreal penWidth = int((ydotspermm < 3 ? 3 : ydotspermm) * weight + 0.5);
                 QPen pen(penColour,penWidth,Qt::SolidLine,Qt::FlatCap);

                 if ((linestyle = db.linestyles.indextostyle(style)) == NULL)
                 {
                      scene->addLine(rect.left()+10,rect.height()-(rect.top() + rect.bottom()) / 2,
                                     rect.right()-10,rect.height()-(rect.top() + rect.bottom()) / 2,
                                     pen);
                 }
                 else
                 {
                     linestyle->start(0.5,rect.right() - 20);
                     linestyle->draw(ctl,rect.left() + 10,(rect.top() + rect.bottom()) / 2,
                                     rect.right() - 10,(rect.top() + rect.bottom()) / 2,
                                     pen);
                 }
              }
        }
    }
}

/**
 * @brief Styleset_Dialog::updateAll
 *
 * updates the current values according to the current styleset name and redraws the sample
 *
 */
void  Styleset_Dialog::updateAll()
{
    RCCHAR name[300],currentName[300],key[300],value[300],*s;
    int i,colour,layer,style;
    double weight;

    colour = db.header.getcolour();
    layer = db.header.getlayer();
    style = db.header.getstyle();
    weight = db.lineweights.weighttomm(db.header.getweight());

    // get b the current style name
    strcpy(currentName,ui->_100->currentText().data());
    //qDebug() << "currentName : " << QString(currentName);

    // get the setting for the current style
    for (i = 1 ; ; i++)
      {
         sprintf(key,"StyleSet%d",i);
         strcpy(name,"");
         strcpy(value,"");
         GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());

         //qDebug() <<  "style set : key " << QString(key) << " value : " << QString(value);
         if (strlen(value) == 0)
           break;
         s = strtok(value,_RCT(","));
         //qDebug() <<  "name : " << QString(s);
         if( s != 0 )
             strcpy(name,s);

         //qDebug() <<  "name " << QString(name);
         //if(strcmp(name,currentName) == 0)
         //    break;
    //}

    if(strcmp(name,currentName) == 0)
    {
         //qDebug() << "s : " << QString(s);
         s = strtok(0,_RCT(","));
         //qDebug() << "colour : " << QString(s);
         if (s != 0 ) // set the colour
         {
             colour = atoi(s);
             //qDebug() << "colour : " << colour;
         }
         //qDebug() << "s : " << QString(s);
         s = strtok(0,_RCT(","));
         //qDebug() << "layer : " << QString(s);
         if (s != 0 ) // set the layer
         {
             layer = atoi(s);
             //qDebug() << "layer : " << layer;
         }
         //qDebug() << "s : " << QString(s);
         s = strtok(0,_RCT(","));
         //qDebug() << "style : " << QString(s);
         if (s != 0 ) // set the style
         {
             style = atoi(s);
             //qDebug() << "style : " << style;
         }
         //qDebug() << "s : " << QString(s);
         s = strtok(0,_RCT(","));
         //qDebug() << "weight : " << QString(s);
         if (s != 0 ) // set the weight
         {
             weight = atof(s);
             //qDebug() << "weight : " << weight;
         }
         break;
    }
    }

    ui->_101->setValue(colour);
    ui->_102->setValue(layer);
    ui->_103->setValue(style);
    ui->_104->setValue(weight);

}
