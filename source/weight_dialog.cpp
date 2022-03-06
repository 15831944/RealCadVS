#include "ncwin.h"

#include "RCDialog.h"
#include "weightspinbox.h"
#include "weight_dialog.h"
#include "ui_Weight_Dialog.h"

WeightItemDelegate::WeightItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void WeightItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QColor backColour(::v.getreal("wn::backgroundcolour.red")*255.0,
                     ::v.getreal("wn::backgroundcolour.green")*255.0,
                     ::v.getreal("wn::backgroundcolour.blue")*255.0);

    QColor frameColour(backColour.red()==127?255:255-backColour.red(),
                       backColour.green()==127?255:255-backColour.green(),
                       backColour.blue()==127?255:255-backColour.blue());

    QColor lineColour(db.colourtable[db.header.getcolour()].red,
                     db.colourtable[db.header.getcolour()].green,
                     db.colourtable[db.header.getcolour()].blue);

    int lineOffset = option.fontMetrics.width(index.data().toString())*2;
    int itemWidth = ((QListView*)parent())->viewport()->size().width();

    // draw the background for the line
    painter->fillRect(option.rect.x()+lineOffset,option.rect.y(),itemWidth-1,option.rect.height(),QBrush(backColour));

    // draw the item frame
    painter->setPen(frameColour);
    painter->setPen(QPen(frameColour,0));
    painter->drawRect(option.rect.x(),option.rect.y(),itemWidth-1,option.rect.height());

    // draw the selection
    QStyledItemDelegate::paint(painter, option, index);

    // draw the line on top of the selection
    int ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));
    int penWidth = (ydotspermm < 3 ? 3 : ydotspermm) * db.lineweights.weighttomm(index.row()) + 0.5;
    painter->setPen(QPen(lineColour,penWidth,Qt::SolidLine,Qt::SquareCap));
    QPoint p1 = QPoint(option.rect.x()+lineOffset+10,option.rect.y()+option.rect.height()/2);
    QPoint p2 = QPoint(itemWidth-10,option.rect.y()+option.rect.height()/2);
    painter->drawLine(p1,p2);
}

QSize WeightItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

Weight_Dialog::Weight_Dialog(HANDLE parent, HANDLE rcparent, int f):
	Weight_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
Weight_Dialog::Weight_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
      RCDialog(rcparent,parent,f),
      ui(new Ui::Weight_Dialog)
{
    ui->setupUi(this);
	ActivateParent = false;
    // put any inits not known to the realCAD Dialog class here

    QStringListModel *model = new QStringListModel();
    QStringList list;
    // put the weight values into the list
    for(int i=0; i < db.lineweights.numweights(); i++)
    {
        RCCHAR string[10];
        sprintf(string,"%.2f",db.lineweights.weighttomm(i));
        list << QString(string);
    }
    model->setStringList(list);
    model->sort(0);

    ui->_101->setModel(model);
    WeightItemDelegate *delegate = new WeightItemDelegate(ui->_101);
    ui->_101->setItemDelegate(delegate);

    // get the current value from the existing rc control
    double weight = v.getreal("ss:weight");
    int iweight = db.lineweights.mmtoweight(weight);
    ui->_101->setCurrentIndex(ui->_101->model()->index(iweight,0));
    ui->_100->setValue(weight);

    // init the realCAD Dialog class here
    initDialog(this);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

Weight_Dialog::~Weight_Dialog()
{
    delete ui;
}

void Weight_Dialog::on__101_clicked(const QModelIndex & index)
{
   // change the spinbox value
    ui->_100->setValue(index.data().toDouble());
}

void Weight_Dialog::on__102_clicked(bool checked)
{
    // using the dialog callback function
    // change the value through the realCAD spin box control
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 101
    wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_102;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void Weight_Dialog::on_accept()
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

void Weight_Dialog::on__100_valueChanged(double v)
{
    int w = db.lineweights.mmtoweight(v);
    ui->_101->setCurrentIndex(ui->_101->model()->index(w,0));
}

/*
QWidget* Weight_Dialog::getControl(int id)
{
    return findChild<QWidget *>(QString("_%1").arg(id));
}

QWidget* Weight_Dialog::getControl(QString name)
{
    return findChild<QWidget *>(name);
}
*/
