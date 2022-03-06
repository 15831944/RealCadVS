
#include "ncwin.h"

#include "RCDialog.h"
#include "layergroup_dialog.h"
#include "ui_layergroup_dialog.h"

// palette item delegate
LayerPaletteItemDelegate::LayerPaletteItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void LayerPaletteItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(index.column() == 0)
    { // this is the Current column
        QPointF center = QPointF(option.rect.left()+option.rect.width()/2,option.rect.top()+option.rect.height()/2);
        double radius = option.rect.height()/4;

        // draw the selection background
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // draw the selection dot
        if (option.state & QStyle::State_Selected)
        {
            painter->save();

            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            painter->setPen(pen);
            painter->setBrush(QBrush(QColor(0,0,0)));
            painter->drawEllipse(center,radius,radius);

            painter->restore();
        }
        else
        {
            painter->save();

            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            painter->setPen(pen);
            painter->drawEllipse(center,radius,radius);

            painter->restore();
        }
    }
    else if(index.column() == 1)
    {
        painter->save();

        QPen textPen;
        if (option.state & QStyle::State_Selected)
        {
            textPen.setColor(QColor(255,255,255));
            painter->fillRect(option.rect, option.palette.highlight());
        }
        else
            textPen.setColor(QColor(0,0,0));

        painter->setPen(textPen);
        int textYOffset = option.fontMetrics.height()/2;
        painter->drawText(option.rect.left(),option.rect.bottom()-textYOffset,index.data().toString());

        painter->restore();


    }
    else if(index.column() == 2)
    {
        painter->save();

        QPen textPen;
        if (option.state & QStyle::State_Selected)
        {
            textPen.setColor(QColor(255,255,255));
            painter->fillRect(option.rect, option.palette.highlight());
        }
        else
            textPen.setColor(QColor(0,0,0));

        painter->setPen(textPen);
        int textYOffset = option.fontMetrics.height()/2;
        painter->drawText(option.rect.left(),option.rect.bottom()-textYOffset,index.data().toString());

        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize LayerPaletteItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

LayerGroup_Dialog::LayerGroup_Dialog(HANDLE parent, HANDLE rcparent, int f):
	LayerGroup_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}

LayerGroup_Dialog::LayerGroup_Dialog(QWidget *parent, Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::LayerGroup_Dialog)
{
    ui->setupUi(this);
	ActivateParent = false;
    populatePaletteList();

    populateTable();

    setCurrentPalette(ui->_100->currentText());

    // connect the OK and Cancel buttons
    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
    connect(ui->_100,  SIGNAL(currentIndexChanged(const QString &)),
            this,      SLOT(checkPaletteHasName(const QString &)));
    connect(ui->_100,  SIGNAL(editTextChanged(const QString &)),
            this,      SLOT(checkPaletteHasName(const QString &)));
}

LayerGroup_Dialog::~LayerGroup_Dialog()
{
    delete ui;
}

void LayerGroup_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LayerGroup_Dialog::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "LayerGroup_Dialog::resizeEvent" ;

    // move the buttons
    int deltaY = e->size().height() - e->oldSize().height();
    //qDebug() << "LayerGroup_Dialog::resizeEvent : deltaY : " << deltaY ;

    if(isVisible())
    {
        ui->_1->move(ui->_1->pos().x(),ui->_1->pos().y()+deltaY);
        ui->_2->move(ui->_2->pos().x(),ui->_2->pos().y()+deltaY);
        ui->_300->move(ui->_300->pos().x(),ui->_300->pos().y()+deltaY);
        ui->_400->move(ui->_400->pos().x(),ui->_400->pos().y()+deltaY);

        ui->_200->resize(ui->_200->width(),ui->_200->height()+deltaY);
    }
}

void LayerGroup_Dialog::on__100_activated(const QString & paletteName)
{
    showPalette(paletteName);
    if(paletteName == "All Layers")
        ui->_400->setDisabled(true);
    else
        ui->_400->setDisabled(false);
}

void LayerGroup_Dialog::populatePaletteList()
{
    RCCHAR key[300],value[600],*s,name[300];
    int i;

    ui->_100->addItem("");

    for(i = 1 ; ; i++)
    {
       sprintf(key,"LayerGroupName%d",i);
       GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
       //qDebug() << "Layer Groups : " << "(" << QString(key) << ") : " << QString(value);
       if (strlen(value) == 0)
         break;
       s = strtok(value,_RCT("\t"));
       if (s != 0)
       {
           strcpy(name,s);
           s = strtok(0,_RCT("\t"));
           if (s != 0 && _stricmp(s,"deleted") != 0)
              ui->_100->addItem(QString(name));
       }
    }
}

void LayerGroup_Dialog::setCurrentPalette(const QString &paletteName)
{
    ui->_100->setCurrentText(paletteName);
    showPalette(paletteName);
}

void LayerGroup_Dialog::showPalette(const QString &paletteName)
{
    RCCHAR key[300],value[600],*s,name[300];
    BitMask  paletteLayers(MAX_LAYERS);
    int i;

    // clear all selections and layer masks
    ui->_200->clearSelection();
    paletteLayers.clearall();

    // get the palette members and pre select them
    for (i = 1 ; ; i++)
    {
         sprintf(key,"LayerGroupName%d",i);
         GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,600,home.getinifilename());
         if (strlen(value) == 0)
           break;
         s = strtok(value,_RCT("\t"));
         if (s != 0 && _stricmp(s,paletteName.data()) == 0)
         {
              s = strtok(0,_RCT("\t"));
              if (s != 0 && _stricmp(s,"Deleted") != 0)
              {
                  paletteLayers.loadascii(QString(s).toLatin1().data());
              }
              break;
         }
    }

    // select the palette members
    int first=-1;
    for(i=0; i < MAX_LAYERS; i++)
    {
        if(paletteLayers.test(i))
        {
            if(first < 0)
                first = i;
            ui->_200->selectionModel()->select(QItemSelection(ui->_200->model()->index(i,0),
                                                              ui->_200->model()->index(i,2))
                                               ,QItemSelectionModel::Select);
        }
    }

    ui->_200->scrollTo(ui->_200->model()->index(first,0),QAbstractItemView::PositionAtTop);
    ui->_200->update();

    checkPaletteHasName(ui->_100->currentText());
}

void LayerGroup_Dialog::checkPaletteHasName(const QString &text)
{
    if(text.isEmpty())
    {
        ui->_300->setEnabled(false);
        ui->_1->setEnabled(false);
    }
    else if(text == "All Layers")
    {
        QMessageBox::warning(this,"Layer Group","\"All Layers\" is a reserved name.\nPlease use a different name for your layer palette.");
        ui->_300->setEnabled(false);
        ui->_400->setEnabled(false);
        ui->_1->setEnabled(false);
    }
    else
    {
        ui->_300->setEnabled(true);
        ui->_1->setEnabled(true);
    }
}

void LayerGroup_Dialog::populateTable()
{
    // define the headers
    int i;
    QStandardItemModel *model = new QStandardItemModel(MAX_LAYERS, 3, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Select"));
    model->setHeaderData(1, Qt::Horizontal, tr("No. "));
    model->setHeaderData(2, Qt::Horizontal, tr("Name"));

    // put the values into the table
    for( i=0; i < MAX_LAYERS; i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the Select item
        item = new QStandardItem();
        item->setData(i); // layer number
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model->setItem(i, 0, item);

        // set the style number item
        sprintf(string,"%4d",i);
        item = new QStandardItem(QString(string));
        item->setData(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model->setItem(i, 1, item);

        // set the Name item
        item = new QStandardItem(QString(db.layernames.getlayername(i)));
        item->setData(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model->setItem(i, 2, item);

        int rowHeight = 16;
        if(RCGetHDPIYscale() > 1)
            rowHeight = 8;

        ui->_200->setRowHeight(i,rowHeight);
    }

    ui->_200->setModel(model);

    LayerPaletteItemDelegate * delegate = new LayerPaletteItemDelegate();
    ui->_200->setItemDelegate(delegate);

    ui->_200->resizeRowsToContents();
    ui->_200->resizeColumnsToContents();
    ui->_200->setColumnWidth(0,50);
    ui->_200->setColumnWidth(1,40);
    ui->_200->horizontalHeader()->setStretchLastSection(true);
    ui->_200->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void LayerGroup_Dialog::on__300_clicked(bool checked)
{
    QString name = ui->_100->currentText();
    updatePalette(name);
    ui->_100->setCurrentIndex(ui->_100->findText(name));
    showPalette(name);
}

/**
 * @brief LayerGroup_Dialog::updatePalette
 * @param paletteName
 *
 * adds the given palette name to the ini file
 */
void LayerGroup_Dialog::updatePalette(const QString &paletteName)
{
    int i;
    RCCHAR key[300],value[600],*s,layergroupname[300];
    char text[600];
    BitMask layers(MAX_LAYERS);

    if (paletteName.length() > 0)
    {
         layers.clearall();
         strcpy(layergroupname,paletteName);

         // make the layer mask
         QModelIndex index;
         QModelIndexList selected = ui->_200->selectionModel()->selectedRows();
         foreach(index ,selected)
             layers.set(index.row());

         for (i = 1 ; ; i++)
         {
              sprintf(key,"LayerGroupName%d",i);
              GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,600,home.getinifilename());
              if (strlen(value) == 0)
                break;
              s = strtok(value,_RCT("\t"));
              if (s != 0 && _stricmp(s,layergroupname) == 0)
              {
                   s = strtok(0,_RCT("\t"));
                   if (_stricmp(s,"Deleted") == 0)
                     ui->_100->addItem(QString(layergroupname));
                   sprintf(value,"%ls\t%s",layergroupname,layers.saveascii(text));
                   WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
                   break;
              }
         }

         if (strlen(value) == 0)
         {
              sprintf(value,"%ls\t%s",layergroupname,layers.saveascii(text));
              WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
              ui->_100->addItem(QString(layergroupname));
         }
    }
}

/**
 * @brief LayerGroup_Dialog::on__400_clicked
 * @param checked
 *
 * deletes a palette name from the ini file
 */
void LayerGroup_Dialog::on__400_clicked(bool checked)
{
    deletePalette(ui->_100->currentText());
    showPalette(ui->_100->currentText());
}

/**
 * @brief LayerGroup_Dialog::deletePalette
 * @param paletteName
 *
 * deletes the given palette name from the ini file
 */
void LayerGroup_Dialog::deletePalette(const QString &paletteName)
{
    int i;
    RCCHAR key[300],value[600],*s,layergroupname[300];
    BitMask layers(MAX_LAYERS);
    layers.clearall();
    strcpy(layergroupname,paletteName);

    // don't allow the "All Layers" palette to be deleted
    if (paletteName.length() > 0 && paletteName != "All Layers")
    {
        if (strlen(layergroupname) > 0)
        {
             for (i = 1 ; ; i++)
             {
                  sprintf(key,"LayerGroupName%d",i);
                  GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,300,home.getinifilename());
                  if (strlen(value) == 0)
                    break;
                  s = strtok(value,_RCT("\t"));
                  if (s != 0 && _stricmp(s,layergroupname) == 0)
                  {
                       sprintf(value,"%ls\tDeleted",layergroupname);
                       WritePrivateProfileString(_RCT("LayerGroups"),key,value,home.getinifilename());
                       ui->_100->removeItem(ui->_100->findText(QString(layergroupname)));
                       break;
                  }
             }
        }
    }
}

void LayerGroup_Dialog::on_accept()
{
    QString name = ui->_100->currentText();
    updatePalette(name);
    ui->_100->setCurrentIndex(ui->_100->findText(name)>=0?ui->_100->findText(name):0);
    done(ui->_100->currentIndex());
}
