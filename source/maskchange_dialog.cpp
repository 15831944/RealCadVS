
#include "ncwin.h"

#include "RCDialog.h"
#include "maskchange_dialog.h"
#include "ui_MASKCHANGE_DIALOG.h"


bool LayerTableKeyPressFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->objectName() == "_1111")
    {
        // look for a match to the input
        // if it is a number look in the id colum then look in name column
        if(event->type() == QEvent::KeyPress)
        {
            if(((QKeyEvent*)event)->text().length() != 0)
            {

            }
        }

    }
    return false;
}

void SelectabilitySwitcher::paint(QPainter *painter, const QRect &rect,
           const QPalette &palette) const
{
    //qDebug() << "SelectabilitySwitcher::paint";
    painter->save();

    QPointF center = QPointF( rect.left()+ rect.width()/2, rect.top()+ rect.height()/2);
    int rectSize =  rect.height()/2;
    QRect prect(center.x()-rectSize/2,center.y()-rectSize/2,rectSize,rectSize);

    // draw the selection background
         //painter->fillRect( rect,  palette.highlight());

    // draw the selection eye
    if (mChecked)
    {
        QPen oldPen = painter->pen();

        // draw the square
        //QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
        //painter->setPen(pen);
        //painter->drawRect(rect);

        // fill the square
        painter->fillRect(prect,QColor(0,0,0));

        painter->setPen(oldPen);
    }
    else
    {
        QPen oldPen = painter->pen();

        QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
        painter->setPen(pen);
        painter->drawRect(prect);

        painter->setPen(oldPen);
    }

    painter->restore();
}

void VisibilitySwitcher::paint(QPainter *painter, const QRect &rect,
           const QPalette &palette) const
{
    //qDebug() << "VisibilitySwitcher::paint";
    painter->save();

    QPointF center = QPointF( rect.left()+ rect.width()/2, rect.top()+ rect.height()/2);
    double radius =  rect.height()/4;

    // draw the selection background
         //painter->fillRect( rect,  palette.highlight());

    // draw the selection eye
    if (mChecked)
    {
#if 1

        // draw an image
        QImage image(":/images/openeye.png","PNG");
        painter->drawImage(painter->viewport(),image,image.rect());
#else
        QPen oldPen = painter->pen();
        QBrush oldBrush = painter->brush();

        // draw the eye ellipse
        QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
        painter->setPen(pen);
        painter->drawEllipse(center,radius*2,radius);

        // fill the eye circle
        pen = QPen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
        painter->setPen(pen);
        painter->setBrush(QBrush(QColor(0,0,0)));
        painter->drawEllipse(center,radius,radius);

        painter->setPen(oldPen);
        painter->setBrush(oldBrush);
#endif
    }
    else
    {
#if 1
        // draw an image
        QImage image(":/images/closedeye.png","PNG");
        painter->drawImage(painter->viewport(),image,image.rect());
#else
        QPen oldPen = painter->pen();

        QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
        painter->setPen(pen);
        painter->drawEllipse(center,radius*2,radius);
        painter->drawEllipse(center,radius,radius);

        painter->setPen(oldPen);
#endif
    }

    painter->restore();
}

PropertyItemEditor::PropertyItemEditor(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

void PropertyItemEditor::mousePressEvent(QMouseEvent * event )
{
    //mSwitcher->toggle();
    //qDebug() << "selEditor : mousePressEvent : " ;
}


void PropertyItemEditor::mouseReleaseEvent(QMouseEvent * event )
{
    emit editingFinished();
    //qDebug() << "selEditor : mouseReleaseEvent : " ;
}


void PropertyItemEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(dynamic_cast<VisibilitySwitcher*>(mSwitcher) != 0)
        ((VisibilitySwitcher*)mSwitcher)->paint(&painter, rect(), this->palette());
    else if(dynamic_cast<SelectabilitySwitcher*>(mSwitcher) != 0)
        ((SelectabilitySwitcher*)mSwitcher)->paint(&painter, rect(), this->palette());
}

QSize PropertyItemEditor::sizeHint() const
{
    return size();
}

MaskTableItemDelegate::MaskTableItemDelegate(QWidget *parent, QTableView *tableView)
    : QStyledItemDelegate(parent), mTableView(tableView)
{
    ;
}

void MaskTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

    if(index.column() == 0)
    { // this is the Visible column
        QPointF center = QPointF(option.rect.left()+option.rect.width()/2,option.rect.top()+option.rect.height()/2);
        double radius = option.rect.height()/4;

        // draw the selection background
        // the current layer item is drawn in grey
        // don't show selection for current item

        if (option.state & QStyle::State_Selected)
           painter->fillRect(option.rect, option.palette.highlight());

        // draw the selection eye
        if (index.data(Qt::CheckStateRole).toInt() ||
            (this->mTableView != 0 &&
             this->mTableView->objectName() == "_1111" &&
             this->mTableView->currentIndex().row() == index.row()))
        {
#if 1
            QPen oldPen = painter->pen();
            QBrush oldBrush = painter->brush();

            // set the pen and brush
            // the current layer item is drawn in grey
            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            QBrush brush(QColor(0,0,0));
            if(this->mTableView != 0 && this->mTableView->objectName() == "_1111" &&
                this->mTableView->currentIndex().row() == index.row())
            {
                pen = QPen(QBrush(QColor(200,200,200)),2,Qt::SolidLine);
                brush = QBrush(QColor(200,200,200));
                painter->fillRect(option.rect,brush);
                // draw an image
                QImage image(":/images/openeye_dissabled.png","PNG");
                image = image.scaledToHeight(option.rect.height(),Qt::SmoothTransformation);
                QPointF origin = QPointF(option.rect.center().x()-image.rect().width()/2,option.rect.center().y()-image.rect().height()/2);
                painter->drawImage(origin,image,image.rect());
                //qDebug() << "(selection table) selected layer row : " << index.row();
            }
            else
            {
                // draw an image
                QImage image(":/images/openeye.png","PNG");
                image = image.scaledToHeight(option.rect.height(),Qt::SmoothTransformation);
                QPointF origin = QPointF(option.rect.center().x()-image.rect().width()/2,option.rect.center().y()-image.rect().height()/2);
                painter->drawImage(origin,image,image.rect());
            }
#else
            QPen oldPen = painter->pen();
            QBrush oldBrush = painter->brush();

            // set the pen and brush
            // the current layer item is drawn in grey
            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            QBrush brush(QColor(0,0,0));
            if(this->mTableView != 0 && this->mTableView->objectName() == "_1111" &&
                this->mTableView->currentIndex().row() == index.row())
            {
                pen = QPen(QBrush(QColor(200,200,200)),2,Qt::SolidLine);
                brush = QBrush(QColor(200,200,200));
            }

            // draw the eye ellipse
            painter->setPen(pen);
            painter->drawEllipse(center,radius*2,radius);

            // fill the eye circle
            painter->setPen(pen);
            painter->setBrush(brush);
            painter->drawEllipse(center,radius,radius);

            painter->setPen(oldPen);
            painter->setBrush(oldBrush);
#endif
        }
        else
        {
#if 1
            // draw an image
            QImage image(":/images/closedeye.png","PNG");
            image = image.scaledToHeight(option.rect.height(),Qt::SmoothTransformation);
            QPointF origin = QPointF(option.rect.center().x()-image.rect().width()/2,option.rect.center().y()-image.rect().height()/2);
            painter->drawImage(origin,image,image.rect());
#else
            QPen oldPen = painter->pen();
            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);

            painter->setPen(pen);
            painter->drawEllipse(center,radius*2,radius);
            painter->drawEllipse(center,radius,radius);

            painter->setPen(oldPen);
#endif
        }
    }
    else if(index.column() == 1)
    { // this is the Selectable column
        QPointF center = QPointF(option.rect.left()+option.rect.width()/2,option.rect.top()+option.rect.height()/2);
        int rectSize = option.rect.height()/2;
        QRect rect(center.x()-rectSize/2,center.y()-rectSize/2,rectSize,rectSize);

        // draw the selection background
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // draw the selection eye
        if (index.data(Qt::CheckStateRole).toInt())
        {
#if 1
            // draw an image
            QImage image(":/images/boxchecked.png","PNG");
            image = image.scaledToHeight(option.rect.height(),Qt::SmoothTransformation);
            QPointF origin = QPointF(option.rect.center().x()-image.rect().width()/2,option.rect.center().y()-image.rect().height()/2);
            painter->drawImage(origin,image,image.rect());
#else
            QPen oldPen = painter->pen();

            // draw the square
            //QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            //painter->setPen(pen);
            //painter->drawRect(rect);

            // fill the square
            painter->fillRect(rect,QColor(0,0,0));

            painter->setPen(oldPen);
#endif
        }
        else
        {
#if 1
            // draw an image
            QImage image(":/images/boxunchecked.png","PNG");
            image = image.scaledToHeight(option.rect.height(),Qt::SmoothTransformation);
            QPointF origin = QPointF(option.rect.center().x()-image.rect().width()/2,option.rect.center().y()-image.rect().height()/2);
            painter->drawImage(origin,image,image.rect());
#else
             QPen oldPen = painter->pen();

            QPen pen(QBrush(QColor(0,0,0)),2,Qt::SolidLine);
            painter->setPen(pen);
            painter->drawRect(rect);

            painter->setPen(oldPen);
#endif
        }
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize MaskTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
/*
QWidget *MaskTableItemDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{

    if(index.column() == 0  )
    {
        PropertyItemEditor *editor = new PropertyItemEditor(parent);
        VisibilitySwitcher *switcher = new VisibilitySwitcher();
        editor->setSwitcher(switcher);
        connect(editor, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return editor;
    }
    else if(index.column() == 1  )
    {
        PropertyItemEditor *editor = new PropertyItemEditor(parent);
        SelectabilitySwitcher *switcher = new SelectabilitySwitcher();
        editor->setSwitcher(switcher);
        connect(editor, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return editor;
    }
    else
        return QStyledItemDelegate::createEditor(parent, option, index);
}

void MaskTableItemDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{

    if(index.column() == 0  )
    {
        //bool value = index.model()->data(index, Qt::EditRole).toBool();
        int value = index.model()->data(index, Qt::CheckStateRole).toInt();
        PropertyItemEditor *visEditor = qobject_cast<PropertyItemEditor *>(editor);
        visEditor->switcher()->setChecked(value!=0?1:0);
        qDebug() << "visEditor : data in : " << visEditor->switcher()->checked();
        //visEditor->switcher()->toggle();
    }
    else if(index.column() == 1  )
    {
        //bool value = index.model()->data(index, Qt::EditRole).toBool();
        int value = index.model()->data(index, Qt::CheckStateRole).toInt();
        PropertyItemEditor *selEditor = qobject_cast<PropertyItemEditor *>(editor);
        selEditor->switcher()->setChecked(value!=0?1:0);
        qDebug() << "selEditor : data in : " << selEditor->switcher()->checked();
        //selEditor->switcher()->toggle();
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void MaskTableItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{

    if(index.column() == 0 )
    {
        PropertyItemEditor *visEditor = qobject_cast<PropertyItemEditor *>(editor);
        int value = visEditor->switcher()->checked() == Qt::Unchecked ? Qt::Unchecked : Qt::Checked;
        qDebug() << "visEditor : data out : " << visEditor->switcher()->checked();
        //model->setData(index, value, Qt::EditRole);
        model->setData(index, value, Qt::CheckStateRole);
    }
    else if(index.column() == 1)
    {
        PropertyItemEditor *selEditor = qobject_cast<PropertyItemEditor *>(editor);
        int value = selEditor->switcher()->checked() == Qt::Unchecked ? Qt::Unchecked : Qt::Checked;
        qDebug() << "selEditor : data out : " << selEditor->switcher()->checked();
        //model->setData(index, value, Qt::EditRole);
        model->setData(index, value, Qt::CheckStateRole);
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void MaskTableItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    editor->setGeometry(option.rect);
}

void MaskTableItemDelegate::commitAndCloseEditor()
{
    PropertyItemEditor *editor = qobject_cast<PropertyItemEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
*/

// layer table delegate
LayerTableItemDelegate::LayerTableItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void LayerTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{    
    //qDebug() << "(layer table) selected layer row : " << index.row();

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
    { // this is the layer number column
        //QStyledItemDelegate::paint(painter, option, index);
        // draw the selection background
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
    { // this is the layer name column

        painter->save();

        //QStyledItemDelegate::paint(painter, option, index);

         // draw the selection background
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

QSize LayerTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// layer table editbox column delegate
LayerTableEditDelegate::LayerTableEditDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget *LayerTableEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
}

void LayerTableEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();

        QLineEdit *textedit = static_cast<QLineEdit*>(editor);
        textedit->setText(text);
    }
}

void LayerTableEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QLineEdit *textedit = static_cast<QLineEdit*>(editor);
        QString text = textedit->text();

        model->setData(index, text, Qt::EditRole);
    }
}

void LayerTableEditDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        editor->setGeometry(option.rect);
    }
}

void LayerTableEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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
    { // this is the layer number column
        //QStyledItemDelegate::paint(painter, option, index);
        // draw the selection background
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
    { // this is the layer name column
        painter->save();

        //QStyledItemDelegate::paint(painter, option, index);

         // draw the selection background
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

QSize LayerTableEditDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// weight table delegates
WeightTableItemDelegate::WeightTableItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void WeightTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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
    { // this is the Weight Value column
        // draw the selection background
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
    { // this is the weight appearance column
        painter->save();

        QStyledItemDelegate::paint(painter, option, index);

        QColor backColour(::v.getreal("wn::backgroundcolour.red")*255.0,
                          ::v.getreal("wn::backgroundcolour.green")*255.0,
                          ::v.getreal("wn::backgroundcolour.blue")*255.0);

        QColor lineColour(db.colourtable[db.header.getcolour()].red,
                          db.colourtable[db.header.getcolour()].green,
                          db.colourtable[db.header.getcolour()].blue);

        //painter->setRenderHint(QPainter::Antialiasing,true);
        //painter->setViewport(option.rect);
        //painter->setWindow(option.rect);

        // draw the selection background
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // draw the background for the line
        if(option.state & QStyle::State_Selected)
            painter->fillRect(option.rect.left()+2,option.rect.top()+2,option.rect.width()-5,option.rect.height()-4,QBrush(backColour));
        else
            painter->fillRect(option.rect.left(),option.rect.top(),option.rect.width(),option.rect.height(),QBrush(backColour));

        // get the pen width from the mm with
        // draw the line
        //double ydotspermm = 3;
        //double penWidth = (int)(ydotspermm * db.lineweights.weighttomm(index.row()+1)) ;
        // get the pen with in pixels from the fixed pixel widths
        int penWidth = db.lineweights.weighttopx(index.row());
        //qDebug() << "line width : " << db.lineweights.weighttomm(index.row()+1) << " penwidth : " << penWidth;
        QPen qpen(lineColour,penWidth,Qt::SolidLine,Qt::SquareCap);
        // set the pen with in pixels
        qpen.setWidth(penWidth);
        painter->setPen(qpen);
        QPointF p1 = QPointF((double)option.rect.left()+10.0,(double)option.rect.top()+(double)option.rect.height()/2.0);
        QPointF p2 = QPointF((double)option.rect.right()-10.0,(double)option.rect.top()+(double)option.rect.height()/2.0);
        painter->drawLine(p1,p2);

        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize WeightTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// colour table delegates
ColourTableItemDelegate::ColourTableItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void ColourTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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
    { // this is the style Value column
        // draw the selection background
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
    { // this is the style appearance column
        painter->save();

        QStyledItemDelegate::paint(painter, option, index);

        QColor backColour(::v.getreal("wn::backgroundcolour.red")*255.0,
                          ::v.getreal("wn::backgroundcolour.green")*255.0,
                          ::v.getreal("wn::backgroundcolour.blue")*255.0);


        QColor colour(db.colourtable[index.row()].red,
                      db.colourtable[index.row()].green,
                      db.colourtable[index.row()].blue);

        // draw the selection background
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        // draw the colour sample
        if(option.state & QStyle::State_Selected)
            painter->fillRect(option.rect.left()+2,option.rect.top()+2,option.rect.width()-4,option.rect.height()-4,QBrush(colour));
        else
            painter->fillRect(option.rect.left()+2,option.rect.top()+2,option.rect.width()-4,option.rect.height()-4,QBrush(colour));

        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize ColourTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// style table delegates
StyleTableItemDelegate::StyleTableItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void StyleTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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
    { // this is the style Value column
        // draw the selection background
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
    { // this is the style appearance column
        painter->save();

        QStyledItemDelegate::paint(painter, option, index);

        QColor backColour(::v.getreal("wn::backgroundcolour.red")*255.0,
                          ::v.getreal("wn::backgroundcolour.green")*255.0,
                          ::v.getreal("wn::backgroundcolour.blue")*255.0);

        QColor lineColour(db.colourtable[db.header.getcolour()].red,
                          db.colourtable[db.header.getcolour()].green,
                          db.colourtable[db.header.getcolour()].blue);

        // draw the selection background
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect.left(),option.rect.top(),option.rect.width()-1,option.rect.height(), option.palette.highlight());

        // draw the background for the line
        if(option.state & QStyle::State_Selected)
            painter->fillRect(option.rect.left()+2,option.rect.top()+2,option.rect.width()-5,option.rect.height()-4,QBrush(backColour));
        else
            painter->fillRect(option.rect.left(),option.rect.top(),option.rect.width()-1,option.rect.height(),QBrush(backColour));

        // draw the line
        LineStyle *linestyle;
        int ydotspermm = 3;
        // get the pen width from the mm with
        //double penWidth = (int)(ydotspermm * db.lineweights.weighttomm(db.header.getweight()));
        // get the pen with in pixels from the fixed pixel widths
        int penWidth = db.lineweights.weighttopx(db.header.getweight());
        QPen qpen(lineColour,penWidth,Qt::SolidLine,Qt::FlatCap);
        // set the pen with in pixels
        qpen.setWidth(penWidth);
        painter->setPen(qpen);
        if ((linestyle = db.linestyles.indextostyle(index.row())) == NULL)
        {
            painter->drawLine((double)option.rect.left()+10.0,(double)option.rect.top()+(double)(option.rect.height()/2.0),
                              (double)option.rect.right()-10.0,(double)option.rect.top()+(double)(option.rect.height()/2.0));
        }
        else
        {
            linestyle->start(0.5,(double)option.rect.width()-20.0);
            linestyle->draw(painter,(double)option.rect.left()+10.0,(double)option.rect.top()+(double)(option.rect.height()/2.0),
                                    (double)option.rect.right()-10.0,(double)option.rect.top()+(double)(option.rect.height()/2.0));

        }
        painter->restore();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize StyleTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// entity table delegate
EntityTableItemDelegate::EntityTableItemDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    ;
}

void EntityTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize EntityTableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// mask change dialog
MaskChange_Dialog::MaskChange_Dialog(HANDLE parent, HANDLE rcparent, int tab, int f):
	MaskChange_Dialog((QWidget*)parent, (Dialog*)rcparent, tab, (Qt::WindowFlags) f)
{}
MaskChange_Dialog::MaskChange_Dialog(QWidget *parent, Dialog *rcparent, int tab, Qt::WindowFlags f) :
      RCDialog(rcparent,parent,f),
      ui(new Ui::MaskChange_Dialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowSystemMenuHint);
    setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    setWindowFlag(Qt::WindowMinimizeButtonHint,false);
	ActivateParent = false;
    mLayerVisibilityMasks=0;
    mLayerSelectabilityMasks=0;
    mWeightVisibilityMasks=0;
    mWeightSelectabilityMasks=0;
    mColourVisibilityMasks=0;
    mColourSelectabilityMasks=0;
    mStyleVisibilityMasks=0;
    mStyleSelectabilityMasks=0;
    mEntityVisibilityMasks=0;
    mEntitySelectabilityMasks=0;


    // put any inits not known to the realCAD Dialog class here
    // init the tabs
    initLayerTab();
    initWeightTab();
    initColourTab();
    initStyleTab();
    initEntityTab();
    //initTestTab();
    //ui->_100->setTabEnabled(5,false);
    //ui->_100->removeTab(5);


    // init the realCAD Dialog class here
    initDialog(this);

    // connect the double click actions
    connect(ui->_1111, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_accept()));
    connect(ui->_1121, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_accept()));
    connect(ui->_1131, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_accept()));
    connect(ui->_1141, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_accept()));

    //LayerTableKeyPressFilter *layerKeyPressFilter = new LayerTableKeyPressFilter(this);
    //ui->_1111->installEventFilter(layerKeyPressFilter);

    // connect the OK and Cancel buttons
    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
    // connect the weight display checkbox to the style dialog slot
    RCStyleEntryToolBar *tb = (RCStyleEntryToolBar *)cadwindow->styleentry->getQW();
    if(tb != 0)
    {
        QObject::disconnect(ui->_112,SIGNAL(toggled(bool)),tb,SLOT(weightDisplayToggled(bool)));
        QObject::connect(ui->_112,SIGNAL(toggled(bool)),tb,SLOT(weightDisplayToggled(bool)));
    }

    tab = GetPrivateProfileInt("DialogSettings","GenProps::CurrentTab",0,home.getinifilename());
    setCurrentTab(tab);
    on__100_currentChanged(tab);

    // disable other tabs and buttons if the parent is the styleset dialog
    RCCHAR dlgParent[300];
    GetPrivateProfileString("DialogSettings","GenProps::Parent","",dlgParent,300,home.getinifilename());
    qDebug() << "parent : " << QString(dlgParent);

    if(strlen(dlgParent) > 0 && strcmp(QString(dlgParent),QString("Dialog")) == 0)
    {
        for(int i=0; i < 5; i++)
            if(i != tab)
                ui->_100->setTabEnabled(i,false);

        // hide the clone button and icon
        ui->_107->hide();
        //ui->CloneIcon->hide();
        /*
        if(tab == 0)
        {// layer
            ui->_106->setText("Edit Palette");
            ui->_106->show();
            ui->_108_label->show();
            ui->_108->show();
            ui->_109->hide();
            ui->_120->hide();
            ui->_111->hide();
            ui->_112->hide();
        }
        else if(tab == 1)
        {// weight
            ui->_108_label->hide();
            ui->_108->hide();
            ui->_106->hide();
            ui->_109->hide();
            ui->_120->hide();
            ui->_111->hide();
            ui->_112->show();

        }
        else if(tab == 2)
        {// colour
            ui->_108_label->hide();
            ui->_108->hide();
            ui->_106->show();
            ui->_109->show();
            ui->_120->show();
            ui->_111->hide();
            ui->_112->hide();

        }
        else if(tab == 3)
        {// style tab
            ui->_108_label->hide();
            ui->_108->hide();
            ui->_106->show();
            ui->_109->hide();
            ui->_120->hide();
            ui->_111->show();
            ui->_112->hide();

        }
        */
    }


    startWidth = width();
    lastFoundLayer = -1;

    ui->_1111->installEventFilter(this);
    ui->_116->installEventFilter(this);

}

MaskChange_Dialog::~MaskChange_Dialog()
{
    delete ui;
    delete mLayerVisibilityMasks;
    delete mLayerSelectabilityMasks;
    delete mWeightVisibilityMasks;
    delete mWeightSelectabilityMasks;
    delete mColourVisibilityMasks;
    delete mColourSelectabilityMasks;
    delete mStyleVisibilityMasks;
    delete mStyleSelectabilityMasks;
    delete mEntityVisibilityMasks;
    delete mEntitySelectabilityMasks;
}

bool MaskChange_Dialog::eventFilter(QObject *o, QEvent *e)
{
    /*
    if(o == ui->_1111)
    {
        if(e->type() == QEvent::KeyRelease)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
            if(keyEvent->key() == Qt::Key_Up )
            {
                int row = ui->_1111->currentIndex().row();
                ui->_1112->selectionModel()->clear();
                QModelIndex otherIndex = ui->_1112->model()->index(row,0);
                ui->_1112->scrollTo(otherIndex);
                ui->_1112->repaint();
                //return QDialog::eventFilter(o, e);
            }
            else if(keyEvent->key() == Qt::Key_Down )
            {
                int row = ui->_1111->currentIndex().row();
                ui->_1112->selectionModel()->clear();
                QModelIndex otherIndex = ui->_1112->model()->index(row,0);
                ui->_1112->scrollTo(otherIndex);
                ui->_1112->repaint();
                //return QDialog::eventFilter(o, e);
            }
            else
                return QDialog::eventFilter(o, e);
        }
        else
            return QDialog::eventFilter(o, e);
    }
    */
    if(o == ui->_116)
    {
        if(e->type() == QEvent::FocusOut)
        {
            if(((QFocusEvent*)e)->reason() != Qt::TabFocusReason)
                return true;
        }
    }
    return QDialog::eventFilter(o, e);
}

void MaskChange_Dialog::showEvent(QShowEvent * event)
{
#ifndef _MAC
    setGeometry(pos().x(),pos().y(),minimumWidth(),minimumHeight());
#endif
}

void MaskChange_Dialog::loadLayerPalettes()
{
    RCCHAR key[300],value[600],*s,name[300];
    int i;

    ui->_108->clear();
    ui->_108->addItem("All Layers");

    for(i = 1 ; ; i++)
    {
       sprintf(key,"LayerGroupName%d",i);
       GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,600,home.getinifilename());
       //qDebug() << "Layer Groups : " << "(" << QString(key) << ") : " << QString(value);
       if (strlen(value) == 0)
         break;
       s = strtok(value,_RCT("\t"));
       if (s != 0)
       {
           strcpy(name,s);
           s = strtok(0,_RCT("\t"));
           if (s != 0 && _stricmp(s,"deleted") != 0)
              ui->_108->addItem(QString(name));
       }
    }

    // get the last use palette and show it in the dialog
    GetPrivateProfileString(_RCT("LayerGroups"),_RCT("LastLayerGroup"),_RCT(""),value,600,home.getinifilename());
    QString groupName(value);
    int index = ui->_108->findText(groupName,Qt::MatchExactly);
    if(index >= 0)
        ui->_108->setCurrentIndex(index);
    else
        ui->_108->setCurrentIndex(0);
    showLayerGroup(groupName);
}

void MaskChange_Dialog::initLayerTab()
{
    int i;
    QStandardItemModel *model1 = new QStandardItemModel(MAX_LAYERS, 3, this);
    model1->setHeaderData(0, Qt::Horizontal, tr("Current"));
    model1->setHeaderData(1, Qt::Horizontal, tr("No. "));
    model1->setHeaderData(2, Qt::Horizontal, tr("Name"));
    // the number and name columns are duplicated for sorting purposes
    QStandardItemModel *model2 = new QStandardItemModel(MAX_LAYERS, 4, this);
    model2->setHeaderData(0, Qt::Horizontal, tr("Visible"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Selectable"));
    model2->setHeaderData(2, Qt::Horizontal, tr("No. "));
    model2->setHeaderData(3, Qt::Horizontal, tr("Name"));

    mLayerVisibilityMasks = new BitMask(MAX_LAYERS);
    mLayerSelectabilityMasks = new BitMask(MAX_LAYERS);
    //BitMask layers(MAX_LAYERS);
    //BitMask grouplayers(MAX_LAYERS);

    // put the values into the table
    for( i=0; i < MAX_LAYERS; i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the Current item
        item = new QStandardItem();
        item->setData(i); // layer number
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //if(i == db.header.getlayer())
        if(i == v.getinteger("ss::layer"))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model1->setItem(i, 0, item);

        // set the style number item
        //sprintf(string,"%4d",i);
        QString sitem = QString("%1").arg(i,4);
        item = new QStandardItem(sitem);
        item->setData(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 1, item);
        item = new QStandardItem(QString(string));
        item->setData(i);
        model2->setItem(i, 2, item);

        // set the Name item
        item = new QStandardItem(QString(db.layernames.getlayername(i)));
        item->setData(i);
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 2, item);
        item = new QStandardItem(QString(db.layernames.getlayername(i)));
        item->setData(i);
        model2->setItem(i, 3, item);

        // set the Visible item
        item = new QStandardItem();
        item->setData(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        mLayerVisibilityMasks->set(i,db.dismask.layer.test(i));
        if(mLayerVisibilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 0, item);

        // set the selectable item
        item = new QStandardItem();
        item->setData(i);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        //qDebug() << "setting selmask.weight : (" << i << ") : " << state.selmask.layer.test(i);
        mLayerSelectabilityMasks->set(i,state.selmask.layer.test(i));
        //qDebug() << "getting mLayerSelectabilityMasks : (" << i << ") : " << mLayerSelectabilityMasks->test(i);
        if(mLayerSelectabilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 1, item);

        //int rowHeight = 16;
        //if(RCGetHDPIYscale() > 1)
        //    rowHeight = 8;

        //ui->_1111->setRowHeight(i,rowHeight);
        //ui->_1112->setRowHeight(i,rowHeight);

    }

    ui->_1111->setMouseTracking(true);
    ui->_1111->setModel(model1);
    LayerTableItemDelegate *delegateC1 = new LayerTableItemDelegate(ui->_1111);
    //ui->_1111->setItemDelegate(delegate1);
    ui->_1111->setItemDelegateForColumn(0,delegateC1);
    LayerTableItemDelegate *delegateC2 = new LayerTableItemDelegate(ui->_1111);
    ui->_1111->setItemDelegateForColumn(1,delegateC2);
    LayerTableEditDelegate *editDelegate = new LayerTableEditDelegate(ui->_1111);
    ui->_1111->setItemDelegateForColumn(2,editDelegate);
    ui->_1111->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->_1111->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->_1111->setEditTriggers(/*QAbstractItemView::AllEditTriggers*/QAbstractItemView::SelectedClicked);
    //ui->_1111->selectRow(db.header.getlayer());
    ui->_1111->selectRow(v.getinteger("ss::layer"));

    ui->_1111->scrollTo(ui->_1111->model()->index(db.header.getlayer(),0));
    ui->_1111->resizeRowsToContents();
    ui->_1111->resizeColumnsToContents();
    //ui->_1111->setColumnWidth(0,50);
    //ui->_1111->setColumnWidth(1,40);
    ui->_1111->setColumnWidth(2,255);
    ui->_1111->horizontalHeader()->setStretchLastSection(true);

    ui->_1111->setSortingEnabled(true);
    ui->_1111->sortByColumn(1,Qt::AscendingOrder);
    layerSortOrder = Qt::AscendingOrder;
    ui->_1111->horizontalHeader()->setSortIndicator(1,Qt::AscendingOrder);
    //ui->_1111->horizontalHeader()->setSortIndicatorShown(true);
    ui->_1111->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //
    ui->_1112->setModel(model2);
    //MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1112);
    MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1112,ui->_1111);
    ui->_1112->setItemDelegate(delegate2);
    ui->_1112->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    ui->_1112->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->_1112->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->_1112->scrollTo(ui->_1111->model()->index(db.header.getlayer(),0));
    ui->_1112->hideColumn(2);
    ui->_1112->hideColumn(3);
    ui->_1112->resizeRowsToContents();
    ui->_1112->setColumnWidth(0,70);
    ui->_1112->setColumnWidth(1,70);
    //ui->_1112->setSortingEnabled(true);
    ui->_1112->sortByColumn(2,Qt::AscendingOrder);
    ui->_1112->horizontalHeader()->setStretchLastSection(true);
    ui->_1112->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->_1112->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1112_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1112->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1112_currentChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->_1112->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1112_headerPressed(int)));

    connect(ui->_1111->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1111_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1111->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1111_currentChanged(const QModelIndex & , const QModelIndex & )));

    // connect the scroll bars to sync the table view scrolling
    QObject::connect(ui->_1112->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1111->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->_1111->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1112->verticalScrollBar(), SLOT(setValue(int)));

    connect(ui->_1111->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1111_headerPressed(int)));


    // show the change button
    //ui->_106->setText("Edit Palette");
    // show the clone button
    //ui->_106->show();
    // show the clone icon
    //ui->CloneIcon->show();
    // show the combobox and label
    //ui->_108_label->show();
    //ui->_108->show();

    loadLayerPalettes();
    ui->_1111->horizontalHeader()->setSortIndicatorShown(false);

    // unset the change flag
    v.setinteger("ss::layerSet",0);

}

void MaskChange_Dialog::initWeightTab()
{
    // KMJ : NOTE :
    // we are not using row 0 because it currently contains a width of 0.0
    // we should get rid of this but don't yet fully understand the side effects
    // to keep things simple we are currently hiding row zero of this table
    QStandardItemModel *model1 = new QStandardItemModel(db.lineweights.numweights(), 3, this);
    model1->setHeaderData(0, Qt::Horizontal, tr("Current"));
    model1->setHeaderData(1, Qt::Horizontal, tr("Width"));
    model1->setHeaderData(2, Qt::Horizontal, tr("Appearance"));
    QStandardItemModel *model2 = new QStandardItemModel(db.lineweights.numweights(), 2, this);
    model2->setHeaderData(0, Qt::Horizontal, tr("Visible"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Selectable"));

    mWeightVisibilityMasks = new BitMask(256);
    mWeightSelectabilityMasks = new BitMask(256);

    printf("state.selmask.entity in weight change : "); state.selmask.entity.debug();

    // put the values into the table
    for(int i=0; i < db.lineweights.numweights(); i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the Current item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //if(i == db.header.getweight())
        if(i == db.lineweights.mmtoweight(v.getreal("ss::weight")))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model1->setItem(i, 0, item);

        // set the Weight item
        sprintf(string,"%.2f",db.lineweights.weighttomm(i));
        item = new QStandardItem(QString(string));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 1, item);

        // set the Appearance item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 2, item);

        // set the Visible item
        //BitMask disweights(256);
        item = new QStandardItem();
        item->setFlags(/*Qt::ItemIsEditable |*/ Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        //item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        mWeightVisibilityMasks->set(i,db.dismask.weight.test(i));
        if(mWeightVisibilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 0, item);

        // set the selectable item
        //BitMask selweights(256);
        item = new QStandardItem();
        item->setFlags(/*Qt::ItemIsEditable |*/ Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        //item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        //qDebug() << "setting selmask.weight : (" << i << ") : " << state.selmask.weight.test(i);
        mWeightSelectabilityMasks->set(i,state.selmask.weight.test(i));
        //qDebug() << "getting mWeightSelectabilityMasks : (" << i << ") : " << mWeightSelectabilityMasks->test(i);
        if(mWeightSelectabilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 1, item);

        //int rowHeight = 16;
        //if(RCGetHDPIYscale() > 1)
        //    rowHeight = 8;

        //ui->_1121->setRowHeight(i,rowHeight);
        //ui->_1122->setRowHeight(i,rowHeight);
    }

    ui->_1121->setMouseTracking(true);
    ui->_1121->setModel(model1);
    WeightTableItemDelegate *delegate1 = new WeightTableItemDelegate(ui->_1121);
    ui->_1121->setItemDelegate(delegate1);
    ui->_1121->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->_1121->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->_1121->hideRow(0);
    //ui->_1121->selectRow(db.header.getweight()==0?1:db.header.getweight());
    ui->_1121->selectRow(db.lineweights.mmtoweight(v.getreal("ss::weight")));
    ui->_1121->scrollTo(ui->_1121->model()->index(db.header.getweight()==0?1:db.header.getweight(),0));
    ui->_1121->resizeRowsToContents();
    ui->_1121->resizeColumnsToContents();
    ui->_1121->setColumnWidth(2,210);
    ui->_1121->horizontalHeader()->setStretchLastSection(true);
    ui->_1121->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->_1121->sortByColumn(1,Qt::AscendingOrder);

    //
    ui->_1122->setModel(model2);
    MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1122);
    ui->_1122->setItemDelegate(delegate2);
    ui->_1122->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    ui->_1122->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->_1122->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->_1122->hideRow(0);
    //ui->_1122->selectRow(db.header.getweight());
    ui->_1122->scrollTo(ui->_1122->model()->index(db.header.getweight()==0?1:db.header.getweight(),0));
    ui->_1122->resizeRowsToContents();
    ui->_1122->setColumnWidth(0,70);
    ui->_1122->setColumnWidth(1,70);
    ui->_1122->horizontalHeader()->setStretchLastSection(true);
    ui->_1122->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->_1122->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1122_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1122->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1122_currentChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->_1122->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1122_headerPressed(int)));

    // connect the scroll bars to sync the table view scrolling
    QObject::connect(ui->_1122->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1121->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->_1121->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1122->verticalScrollBar(), SLOT(setValue(int)));

    // hide the change button
    //ui->_106->hide();
    // show the clone button
    //ui->_107->show();
    // show the clone icon
    //ui->CloneIcon->show();
    // hide the combobox
    //ui->_108_label->hide();
    //ui->_108->hide();
    // show the weight display check box
    ui->_112->setChecked((db.getlineweightdisplay() == 1 ? true : false));
    //ui->_112->show();

    // unset the change flag
    v.setinteger("ss::weightSet",0);
}


void MaskChange_Dialog::initColourTab()
{
    QItemSelectionModel *selModel1 = ui->_1131->selectionModel();
    if(selModel1)
        selModel1->deleteLater();
    QAbstractItemModel *oldModel1 = ui->_1131->model();
    if(oldModel1 != 0)
        oldModel1->deleteLater();
    QItemSelectionModel *selModel2 = ui->_1132->selectionModel();
    if(selModel2)
        selModel2->deleteLater();
    QAbstractItemModel *oldModel2 = ui->_1132->model();
    if(oldModel2 != 0)
        oldModel2->deleteLater();

    QStandardItemModel *model1 = new QStandardItemModel(256, 3, this);
    model1->setHeaderData(0, Qt::Horizontal, tr("Current"));
    model1->setHeaderData(1, Qt::Horizontal, tr("No."));
    model1->setHeaderData(2, Qt::Horizontal, tr("Appearance"));
    QStandardItemModel *model2 = new QStandardItemModel(256, 2, this);
    model2->setHeaderData(0, Qt::Horizontal, tr("Visible"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Selectable"));
    model2->setHeaderData(2, Qt::Horizontal, tr("No."));
    int rowHeight = 16;

    if(mColourVisibilityMasks != 0)
        delete mColourVisibilityMasks;
    mColourVisibilityMasks = new BitMask(256);
    if(mColourSelectabilityMasks !=0)
        delete mColourSelectabilityMasks;
    mColourSelectabilityMasks = new BitMask(256);

    // put the values into the table
    for(int i=0; i < 256; i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the Current item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //if(i == db.header.getcolour())
        if(i == v.getinteger("ss::colour"))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model1->setItem(i, 0, item);

        // set the style number item
        sprintf(string,"%d",i);
        QString str = QString("%1").arg(i,3);
        item = new QStandardItem(str);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 1, item);
        item->setData(i);
        model2->setItem(i,2 , item);

        // set the Appearance item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 2, item);

        // set the Visible item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        mColourVisibilityMasks->set(i,db.dismask.colour.test(i));
        if(mColourVisibilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 0, item);

        // set the selectable item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        //qDebug() << "setting selmask.weight : (" << i << ") : " << state.selmask.colour.test(i);
        mColourSelectabilityMasks->set(i,state.selmask.colour.test(i));
        //qDebug() << "getting mWeightSelectabilityMasks : (" << i << ") : " << mColourSelectabilityMasks->test(i);
        if(mColourSelectabilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 1, item);

        //ui->_1131->setRowHeight(i,rowHeight);
        //ui->_1132->setRowHeight(i,rowHeight);
    }

    ui->_1131->setMouseTracking(true);
    ui->_1131->setModel(model1);
    ColourTableItemDelegate *delegate1 = new ColourTableItemDelegate(ui->_1131);
    ui->_1131->setItemDelegate(delegate1);
    ui->_1131->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->_1131->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->_1131->selectRow(db.header.getcolour());
    ui->_1131->selectRow(v.getinteger("ss::colour"));


    ui->_1131->scrollTo(ui->_1131->model()->index(db.header.getcolour(),0));
    ui->_1131->resizeRowsToContents();
    ui->_1131->resizeColumnsToContents();   
    ui->_1131->setColumnWidth(2,210);
    ui->_1131->horizontalHeader()->setStretchLastSection(true);

    ui->_1131->sortByColumn(1,Qt::AscendingOrder);
    ui->_1131->horizontalHeader()->setSortIndicator(1,Qt::AscendingOrder);
    ui->_1131->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   // ui->_1131->horizontalHeader()->setFixedHeight(rowHeight);
    //ui->_1131->setSortingEnabled(true);

    //
    ui->_1132->setModel(model2);
    MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1132);
    ui->_1132->setItemDelegate(delegate2);
    ui->_1132->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    ui->_1132->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->_1132->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->_1132->scrollTo(ui->_1131->model()->index(db.header.getcolour(),0));
    ui->_1132->hideColumn(2);
    ui->_1132->resizeRowsToContents();
    ui->_1132->setColumnWidth(0,70);
    ui->_1132->setColumnWidth(1,70);
    //ui->_1132->setSortingEnabled(true);

    ui->_1132->sortByColumn(2,Qt::AscendingOrder);
    ui->_1132->horizontalHeader()->setStretchLastSection(true);
    ui->_1132->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->_1132->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1132_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1132->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1132_currentChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->_1132->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1132_headerPressed(int)));

    // connect the scroll bars to sync the table view scrolling
    QObject::connect(ui->_1132->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1131->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->_1131->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1132->verticalScrollBar(), SLOT(setValue(int)));

    connect(ui->_1131->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1131_headerPressed(int)));

    // unset the change flag
    v.setinteger("ss::colourSet",0);
}

void MaskChange_Dialog::initStyleTab()
{
    QStandardItemModel *model1 = new QStandardItemModel(256, 3, this);
    model1->setHeaderData(0, Qt::Horizontal, tr("Current"));
    model1->setHeaderData(1, Qt::Horizontal, tr("No."));
    model1->setHeaderData(2, Qt::Horizontal, tr("Appearance"));
    QStandardItemModel *model2 = new QStandardItemModel(256, 2, this);
    model2->setHeaderData(0, Qt::Horizontal, tr("Visible"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Selectable"));

    mStyleVisibilityMasks = new BitMask(256);
    mStyleSelectabilityMasks = new BitMask(256);

    // put the values into the table
    for(int i=0; i < 256; i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the Current item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //if(i == db.header.getstyle())
        if(i == v.getinteger("ss::style"))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model1->setItem(i, 0, item);

        // set the style number item
        sprintf(string,"%3d",i);
        item = new QStandardItem(QString(string));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 1, item);

        // set the Appearance item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 2, item);

        // set the Visible item
        //BitMask disweights(256);
        item = new QStandardItem();
        item->setFlags(/*Qt::ItemIsEditable |*/ Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        //item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        mStyleVisibilityMasks->set(i,db.dismask.style.test(i));
        if(mStyleVisibilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 0, item);

        // set the selectable item
        //BitMask selweights(256);
        item = new QStandardItem();
        item->setFlags(/*Qt::ItemIsEditable |*/ Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsUserCheckable*/);
        //item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        //qDebug() << "setting selmask.weight : (" << i << ") : " << state.selmask.style.test(i);
        mStyleSelectabilityMasks->set(i,state.selmask.style.test(i));
        //qDebug() << "getting mWeightSelectabilityMasks : (" << i << ") : " << mStyleSelectabilityMasks->test(i);
        if(mStyleSelectabilityMasks->test(i))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 1, item);

        //int rowHeight = 16;
        //if(RCGetHDPIYscale() > 1)
        //    rowHeight = 8;

        //ui->_1141->setRowHeight(i,rowHeight);
        //ui->_1142->setRowHeight(i,rowHeight);
    }

    ui->_1141->setMouseTracking(true);
    ui->_1141->setModel(model1);
    StyleTableItemDelegate *delegate1 = new StyleTableItemDelegate(ui->_1141);
    ui->_1141->setItemDelegate(delegate1);
    ui->_1141->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->_1141->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->_1141->selectRow(db.header.getstyle());
    ui->_1141->selectRow(v.getinteger("ss::style"));
    ui->_1141->scrollTo(ui->_1141->model()->index(db.header.getstyle(),0));
    ui->_1141->resizeRowsToContents();
    ui->_1141->resizeColumnsToContents();
    ui->_1141->setColumnWidth(2,210);
    ui->_1141->horizontalHeader()->setStretchLastSection(true);
    ui->_1141->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->_1141->sortByColumn(1,Qt::AscendingOrder);
    //
    ui->_1142->setModel(model2);
    MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1142);
    ui->_1142->setItemDelegate(delegate2);
    ui->_1142->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    ui->_1142->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->_1142->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->_1142->scrollTo(ui->_1141->model()->index(db.header.getstyle(),0));
    ui->_1142->resizeRowsToContents();
    ui->_1142->setColumnWidth(0,70);
    ui->_1142->setColumnWidth(1,70);
    ui->_1142->horizontalHeader()->setStretchLastSection(true);
    ui->_1142->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->_1142->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1142_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1142->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1142_currentChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->_1142->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1142_headerPressed(int)));

    // connect the scroll bars to sync the table view scrolling
    QObject::connect(ui->_1142->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1141->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->_1141->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1142->verticalScrollBar(), SLOT(setValue(int)));

    // show the change button
    //ui->_106->show();
    // show the clone button
    //ui->_107->show();
    // show the clone icon
    //ui->CloneIcon->show();
    // hide the combobox
    //ui->_108_label->hide();
    //ui->_108->hide();
    // hide the weight display check box
    //ui->_112->hide();

    // unset the change flag
    v.setinteger("ss::styleSet",0);
}

void MaskChange_Dialog::initEntityTab()
{
    /*
     * entity types
    point_entity = 10,
    line_entity = 11,
    circle_entity = 12,
    ellipse_entity = 23,
    curve_entity = 20,
    plane_entity = 14,
    ruledsurface_entity = 16,
    surfaceofrev_entity = 17,
    patch_entity = 28,
    text_entity = 15,
    figure_entity = 19,
    group_entity = 21,

    lineardimension_entity = 18,
    angulardimension_entity = 22,
    circulardimension_entity = 25,
    distancebearingdimension_entity = 24,

    light_entity = 26,
    bitmap_entity = 29,
    wall_entity = 27,
    */

    int i;
    entityNames.clear();
    entityNames << "Point" << "Lines" << "Circles" << "Ellipses" << "Curves"
            << "Planes" << "Ruled Surfaces" << "Revolved Surfaces" << "Patch Surfaces"
            << "Text" << "Components" << "Groups" << "Dimensions" << "Lights" << "Bitmaps";// << "Walls" ;

    entityCodes.clear();
    entityCodes << point_entity << line_entity << circle_entity << ellipse_entity << curve_entity
                << plane_entity << ruledsurface_entity << surfaceofrev_entity << patch_entity
                << text_entity << figure_entity << group_entity << dimension_mask << light_entity << bitmap_entity << wall_entity;

    QStandardItemModel *model1 = new QStandardItemModel(entityNames.size(), 1, this);
    model1->setHeaderData(0, Qt::Horizontal, tr("Entity Type"));
    QStandardItemModel *model2 = new QStandardItemModel(entityNames.size(), 2, this);
    model2->setHeaderData(0, Qt::Horizontal, tr("Visible"));
    model2->setHeaderData(1, Qt::Horizontal, tr("Selectable"));

    mEntityVisibilityMasks = new BitMask(32);
    mEntitySelectabilityMasks = new BitMask(32);

    // put the values into the table
    for(i=0; i < entityNames.size(); i++)
    {
        RCCHAR string[10];
        QStandardItem *item=0;

        // set the name item
        item = new QStandardItem(entityNames.at(i));
        item->setData(entityCodes.at(i));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        model1->setItem(i, 0, item);

        // set the Visible item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        mEntityVisibilityMasks->set(entityCodes.at(i),db.dismask.entity.test(entityCodes.at(i)));
        if(mEntityVisibilityMasks->test(entityCodes.at(i)))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 0, item);

        // set the selectable item
        item = new QStandardItem();
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        //mEntitySelectabilityMasks->set(entityCodes.at(i),state.tempentitymask.test(entityCodes.at(i)));
        mEntitySelectabilityMasks->set(entityCodes.at(i),state.selmask.entity.test(entityCodes.at(i)));
        if(mEntitySelectabilityMasks->test(entityCodes.at(i)))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        model2->setItem(i, 1, item);

        //int rowHeight = 16;
        //if(RCGetHDPIYscale() > 1)
        //    rowHeight = 8;

        //ui->_1151->setRowHeight(i,rowHeight);
        //ui->_1152->setRowHeight(i,rowHeight);
    }

    ui->_1151->setMouseTracking(true);
    ui->_1151->setModel(model1);
    EntityTableItemDelegate *delegate1 = new EntityTableItemDelegate(ui->_1151);
    ui->_1151->setItemDelegate(delegate1);
    ui->_1151->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->_1151->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->_1151->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->_1151->scrollToTop();
    ui->_1151->resizeRowsToContents();
    ui->_1151->resizeColumnsToContents();
    ui->_1151->setColumnWidth(2,210);
    ui->_1151->horizontalHeader()->setStretchLastSection(true);
    //
    ui->_1152->setModel(model2);
    MaskTableItemDelegate *delegate2 = new MaskTableItemDelegate(ui->_1152);
    ui->_1152->setItemDelegate(delegate2);
    ui->_1152->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    ui->_1152->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->_1152->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->_1152->scrollToTop();
    ui->_1152->resizeRowsToContents();
    ui->_1152->setColumnWidth(0,70);
    ui->_1152->setColumnWidth(1,70);
    ui->_1152->horizontalHeader()->setStretchLastSection(true);
    ui->_1152->horizontalHeader()->setSectionsMovable(false);
    ui->_1152->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->_1152->selectionModel(),
                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on__1152_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->_1152->selectionModel(),
                SIGNAL(currentChanged(const QModelIndex & , const QModelIndex & )),
                this, SLOT(on__1152_currentChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->_1152->horizontalHeader(),
                SIGNAL(sectionPressed(int)),
                this, SLOT(on__1152_headerPressed(int)));

    // connect the scroll bars to sync the table view scrolling
    QObject::connect(ui->_1152->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1151->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->_1151->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->_1152->verticalScrollBar(), SLOT(setValue(int)));

    // hide the change button
    //ui->_106->hide();
    // hide the clone button
    //ui->_107->hide();
    // hide the clone icon
    //ui->CloneIcon->hide();
    // hide the combobox and label
    //ui->_108_label->hide();
    //ui->_108->hide();
    // hide the weight display check box
    //ui->_112->hide();

}

// layer tab methods
void MaskChange_Dialog::on__1111_entered(const QModelIndex & index)
{
    ui->_1112->selectionModel()->clear();
}

void MaskChange_Dialog::on__1111_clicked(const QModelIndex & index)
{
    v.setinteger("ss::layer",index.data(Qt::UserRole+1).toInt());
    v.setinteger("ss::layerSet",1);
    qDebug() << "index data    : " << index.data(Qt::UserRole+1).toInt();
    qDebug() << "index data    : " << index.data(Qt::DisplayRole).toString();
    qDebug() << "clicked layer : " << v.getinteger("ss::layer");
    qDebug() << "clicked row : " << index.row();
    ui->_1112->selectionModel()->clear();
    QModelIndex otherIndex = ui->_1112->model()->index(index.row(),0);
    ui->_1112->model()->setData(otherIndex,Qt::Checked,Qt::CheckStateRole);
    mLayerVisibilityMasks->set(otherIndex.data(Qt::UserRole+1).toInt());
    ui->_1112->scrollTo(otherIndex);
    ui->_1112->repaint();
}

void MaskChange_Dialog::on__1111_headerPressed(int lindex)
{
    // KMJ: TODO: fix sorting for layer table
    if(lindex == 0)
    {
        ui->_1111->scrollTo(ui->_1111->currentIndex(),QAbstractItemView::PositionAtCenter);
        // scroll the other table to the same row
        QModelIndex index = ui->_1112->model()->index(ui->_1111->currentIndex().row(),0);
        ui->_1112->scrollTo(index,QAbstractItemView::PositionAtCenter);
    }
    else if(lindex == 1)
    {
        //if(layerSortOrder == Qt::AscendingOrder)
        if(ui->_1111->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        {
            ui->_1111->scrollToBottom();
            ui->_1112->scrollToBottom();
        }
        else
        {
            ui->_1111->scrollToTop();
            ui->_1112->scrollToTop();
        }
    }
    /*
    else if(lindex == 2)
    {
        if(ui->_1111->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        {            
            ui->_1111->horizontalHeader()->setSortIndicator(lindex,Qt::DescendingOrder);
            ui->_1111->sortByColumn(lindex,Qt::AscendingOrder);
            // sort the other table in the same way
            ui->_1112->sortByColumn(3,Qt::AscendingOrder);

            // find the first non empty row and scroll it to the top of the viewport
            QModelIndex index;
            for(int i=0; i<MAX_LAYERS; i++)
            {
                index = ui->_1111->model()->index(i,2);
                if(index.data().toString().length()!=0)
                    break;
            }
            ui->_1111->scrollTo(index, QAbstractItemView::PositionAtTop);
            // scroll the other table to the same row
            index = ui->_1112->model()->index(index.row(),0);
            ui->_1112->scrollTo(index, QAbstractItemView::PositionAtTop);
         }
        else
        {
            ui->_1111->horizontalHeader()->setSortIndicator(lindex,Qt::AscendingOrder);
            ui->_1111->sortByColumn(lindex,Qt::DescendingOrder);
            // sort the other table in the same way
            ui->_1112->sortByColumn(3,Qt::DescendingOrder);

            // find the first non empty row and scroll it to the top of the viewport
            QModelIndex index;
            for(int i=0; i<MAX_LAYERS; i++)
            {
                index = ui->_1111->model()->index(i,2);
                //qDebug() << "index (" << i << ") data : " << index.data().toString();
                if(index.data().toString().length() != 0)
                    break;
            }
            ui->_1111->scrollTo(index, QAbstractItemView::PositionAtTop);
            // scroll the other table to the same row
            index = ui->_1112->model()->index(index.row(),0);
            ui->_1112->scrollTo(index,QAbstractItemView::PositionAtTop);
        }
    }
    */
}

void MaskChange_Dialog::on__1111_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;
    qDebug() << "on__1111_selectionChanged";
}

void MaskChange_Dialog::on__1111_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    qDebug() << "on__1111_currentChanged : current row " << current.row() << " previous row " << previous.row();
    int row = current.row();
    QModelIndex index = current.model()->index(row,2);
    if(v.getinteger("ss::layer") >= 0)
    {
        v.setinteger("ss::layer",index.data(Qt::UserRole+1).toInt());
        v.setinteger("ss::layerSet",1);
    }
    ui->_1112->selectionModel()->clear();
    QModelIndex otherIndex = ui->_1112->model()->index(row,0);
    ui->_1112->model()->setData(otherIndex,Qt::Checked,Qt::CheckStateRole);
    mLayerVisibilityMasks->toggle(otherIndex.data(Qt::UserRole+1).toInt());
    ui->_1112->scrollTo(otherIndex);
    ui->_1112->repaint();
}

void MaskChange_Dialog::on__1112_clicked(const QModelIndex & index)
{
    //qDebug() << "on__1112_clicked";
}

void MaskChange_Dialog::on__1112_headerPressed(int lindex)
{
    static bool pressed[2]= {true, true};
    if(pressed[lindex])
    {
        pressed[lindex]=false;
        ui->_1112->selectionModel()->clear();
        for(int i=0; i<MAX_LAYERS; i++)
        {
            // the current layer is never unchecked
            if(!ui->_1112->isRowHidden(i) && i != ui->_1111->currentIndex().row())
            {
                ui->_1112->model()->setData(ui->_1112->model()->index(i,lindex), Qt::Unchecked, Qt::CheckStateRole);
                if(lindex == 0)
                {
                    mLayerVisibilityMasks->clear(i);
                }
                else if(lindex == 1)
                {
                    mLayerSelectabilityMasks->clear(i);
                }
            }
        }
    }
    else if(!pressed[lindex])
    {
        pressed[lindex]=true;
        ui->_1112->selectionModel()->clear();
        for(int i=0; i<MAX_LAYERS; i++)
        //for(int i=0; i < ui->_1112->model()->rowCount(); i++)
        {
            if(!ui->_1112->isRowHidden(i))
            {
                ui->_1112->model()->setData(ui->_1112->model()->index(i,lindex), Qt::Checked, Qt::CheckStateRole);
                if(lindex == 0)
                {
                    mLayerVisibilityMasks->set(i);
                }
                else if(lindex == 1)
                {
                    mLayerSelectabilityMasks->set(i);
                }
            }
        }
    }
}

void MaskChange_Dialog::on__1112_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    qDebug() << "on__1112_currentChanged : current row " << current.row() << " previous row " << previous.row();
}

void MaskChange_Dialog::on__1112_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;

    QModelIndexList items = selected.indexes();
    foreach (index, items)
    {
        if(!ui->_1112->isRowHidden(index.row()))
        {
            int value = index.data(Qt::CheckStateRole).toInt();
            if(value != 0)
                value = Qt::Unchecked;
            else
                value = Qt::Checked;

            if(index.column() == 0 && ui->_1111->currentIndex().row() == index.row())
            {
                mLayerVisibilityMasks->set(index.data(Qt::UserRole+1).toInt());
                value = Qt::Unchecked;
            }

            ui->_1112->model()->setData(index, value, Qt::CheckStateRole);

            //qDebug() << "selected index      : " << index.row();
            //qDebug() << "selected no.        : " << index.data(Qt::UserRole);
            //qDebug() << "selected no.        : " << index.data(Qt::UserRole+1);
            //qDebug() << "selected userRole   : " << ui->_1112->model()->index(index.row(),0).data(Qt::UserRole+1);
            //qDebug() << "selected userRole+1 : " << ui->_1112->model()->index(index.row(),1).data(Qt::UserRole+1);
            //qDebug() << "selected userRole+2 : " << ui->_1112->model()->index(index.row(),2).data(Qt::UserRole+1);
            //qDebug() << "selected userRole+3 : " << ui->_1112->model()->index(index.row(),3).data(Qt::UserRole+1);
            //qDebug() << "selected state      : " << value;

            if(index.column() == 0 && ui->_1111->currentIndex().row() != index.row())
                mLayerVisibilityMasks->toggle(index.data(Qt::UserRole+1).toInt());
            else if(index.column() == 1)
                mLayerSelectabilityMasks->toggle(index.data(Qt::UserRole+1).toInt());
        }
    }
}

// weight tab methods
void MaskChange_Dialog::on__1121_entered(const QModelIndex & index)
{
    ui->_1122->selectionModel()->clear();
}

void MaskChange_Dialog::on__1121_clicked(const QModelIndex & index)
{
    v.setreal("ss::weight",db.lineweights.weighttomm(index.row()));
    v.setinteger("ss::weightSet",1);
    ui->_1122->selectionModel()->clear();
}

void MaskChange_Dialog::on__1122_clicked(const QModelIndex & index)
{
    //qDebug() << "on__1122_clicked";
}

void MaskChange_Dialog::on__1122_headerPressed(int lindex)
{
    static bool pressed[2]= {true, true};
    if(pressed[lindex])
    {
        pressed[lindex]=false;
        ui->_1122->selectionModel()->clear();
        for(int i=1; i<db.lineweights.numweights(); i++)
             ui->_1122->model()->setData(ui->_1122->model()->index(i,lindex), Qt::Unchecked, Qt::CheckStateRole);

        if(lindex == 0)
            mWeightVisibilityMasks->clearall();
        else if(lindex == 1)
            mWeightSelectabilityMasks->clearall();
    }
    else if(!pressed[lindex])
    {
        pressed[lindex]=true;
        ui->_1122->selectionModel()->clear();
        for(int i=1; i<db.lineweights.numweights(); i++)
            ui->_1122->model()->setData(ui->_1122->model()->index(i,lindex), Qt::Checked, Qt::CheckStateRole);

        if(lindex == 0)
        {
            mWeightVisibilityMasks->setall();
            mWeightVisibilityMasks->set(0,0);
        }
        else if(lindex == 1)
        {
            mWeightSelectabilityMasks->setall();
            mWeightSelectabilityMasks->set(0,0);
        }
    }
}

void MaskChange_Dialog::on__1122_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "on__1122_currentChanged : current row " << current.row() << " previous row " << previous.row();
}

void MaskChange_Dialog::on__1122_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;

    QModelIndexList items = selected.indexes();
    foreach (index, items)
    {
        int value = index.data(Qt::CheckStateRole).toInt();
        if(value != 0)
            value = Qt::Unchecked;
        else
            value = Qt::Checked;
        ui->_1122->model()->setData(index, value, Qt::CheckStateRole);

        if(index.column() == 0)
            mWeightVisibilityMasks->toggle(index.row());
        else if(index.column() == 1)
            mWeightSelectabilityMasks->toggle(index.row());
    }
}

// colour tab methods
void MaskChange_Dialog::on__1131_entered(const QModelIndex & index)
{
    ui->_1132->selectionModel()->clear();
}

void MaskChange_Dialog::on__1131_clicked(const QModelIndex & index)
{
    v.setinteger("ss::colour",index.row());
    v.setinteger("ss::colourSet",1);
    ui->_1132->selectionModel()->clear();
}

void MaskChange_Dialog::on__1131_headerPressed(int lindex)
{
    if(lindex == 0)
    {
        ui->_1131->scrollTo(ui->_1131->currentIndex(),QAbstractItemView::PositionAtCenter);
        // scroll the other table to the same row
        QModelIndex index = ui->_1132->model()->index(ui->_1111->currentIndex().row(),0);
        ui->_1132->scrollTo(index,QAbstractItemView::PositionAtCenter);
    }
    else if(lindex == 1)
    {
        if(ui->_1131->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        {
            ui->_1131->scrollToBottom();
            ui->_1132->scrollToBottom();
        }
        else
        {
            ui->_1131->scrollToTop();
            ui->_1132->scrollToTop();
        }
    }
}

void MaskChange_Dialog::on__1132_clicked(const QModelIndex & index)
{
    //qDebug() << "on__1132_clicked";
}

void MaskChange_Dialog::on__1132_headerPressed(int lindex)
{
    static bool pressed[2]= {true, true};
    if(pressed[lindex])
    {
        pressed[lindex]=false;
        ui->_1132->selectionModel()->clear();
        for(int i=0; i<256; i++)
            ui->_1132->model()->setData(ui->_1132->model()->index(i,lindex), Qt::Unchecked, Qt::CheckStateRole);

        if(lindex == 0)
            mColourVisibilityMasks->clearall();
        else if(lindex == 1)
            mColourSelectabilityMasks->clearall();
    }
    else if(!pressed[lindex])
    {
        pressed[lindex]=true;
        ui->_1132->selectionModel()->clear();
        for(int i=0; i<256; i++)
            ui->_1132->model()->setData(ui->_1132->model()->index(i,lindex), Qt::Checked, Qt::CheckStateRole);

        if(lindex == 0)
            mColourVisibilityMasks->setall();
        else if(lindex == 1)
            mColourSelectabilityMasks->setall();
    }
}

void MaskChange_Dialog::on__1132_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "on__1132_currentChanged : current row " << current.row() << " previous row " << previous.row();
}

void MaskChange_Dialog::on__1132_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;

    QModelIndexList items = selected.indexes();
    foreach (index, items)
    {
        int value = index.data(Qt::CheckStateRole).toInt();
        if(value != 0)
            value = Qt::Unchecked;
        else
            value = Qt::Checked;
        ui->_1132->model()->setData(index, value, Qt::CheckStateRole);

        if(index.column() == 0)
            mColourVisibilityMasks->toggle(index.row());
        else if(index.column() == 1)
            mColourSelectabilityMasks->toggle(index.row());
     }
}

// style tab methods
void MaskChange_Dialog::on__1141_entered(const QModelIndex & index)
{
    ui->_1142->selectionModel()->clear();
}

void MaskChange_Dialog::on__1141_clicked(const QModelIndex & index)
{
    v.setinteger("ss::style",index.row());
    v.setinteger("ss::styleSet",1);
    ui->_1142->selectionModel()->clear();
}

void MaskChange_Dialog::on__1142_clicked(const QModelIndex & index)
{
    //qDebug() << "on__1142_clicked";
}

void MaskChange_Dialog::on__1142_headerPressed(int lindex)
{
    static bool pressed[2]= {true, true};
    if(pressed[lindex])
    {
        pressed[lindex]=false;
        ui->_1142->selectionModel()->clear();
        for(int i=0; i<256; i++)
            ui->_1142->model()->setData(ui->_1142->model()->index(i,lindex), Qt::Unchecked, Qt::CheckStateRole);

        if(lindex == 0)
            mStyleVisibilityMasks->clearall();
        else if(lindex == 1)
            mStyleSelectabilityMasks->clearall();
    }
    else if(!pressed[lindex])
    {
        pressed[lindex]=true;
        ui->_1142->selectionModel()->clear();
        for(int i=0; i<256; i++)
            ui->_1142->model()->setData(ui->_1142->model()->index(i,lindex), Qt::Checked, Qt::CheckStateRole);

        if(lindex == 0)
            mStyleVisibilityMasks->setall();
        else if(lindex == 1)
            mStyleSelectabilityMasks->setall();
    }
}

void MaskChange_Dialog::on__1142_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "on__1142_currentChanged : current row " << current.row() << " previous row " << previous.row();
}

void MaskChange_Dialog::on__1142_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;

    QModelIndexList items = selected.indexes();
    foreach (index, items)
    {
        int value = index.data(Qt::CheckStateRole).toInt();
        if(value != 0)
            value = Qt::Unchecked;
        else
            value = Qt::Checked;
        ui->_1142->model()->setData(index, value, Qt::CheckStateRole);

        if(index.column() == 0)
            mStyleVisibilityMasks->toggle(index.row());
        else if(index.column() == 1)
            mStyleSelectabilityMasks->toggle(index.row());
    }
}

// entity tab methods
void MaskChange_Dialog::on__1151_entered(const QModelIndex & index)
{
    ui->_1152->selectionModel()->clear();
}

void MaskChange_Dialog::on__1151_clicked(const QModelIndex & index)
{
    ui->_1152->selectionModel()->clear();
}

void MaskChange_Dialog::on__1152_clicked(const QModelIndex & index)
{
    //qDebug() << "on__1152_clicked";
}

void MaskChange_Dialog::on__1152_headerPressed(int lindex)
{
    static bool pressed[2]= {true, true};
    if(pressed[lindex])
    {
        pressed[lindex]=false;
        ui->_1152->selectionModel()->clear();
        for(int i=0; i<ui->_1152->model()->rowCount(); i++)
            ui->_1152->model()->setData(ui->_1152->model()->index(i,lindex), Qt::Unchecked, Qt::CheckStateRole);

        if(lindex == 0)
            mEntityVisibilityMasks->clearall();
        else if(lindex == 1)
            mEntitySelectabilityMasks->clearall();
    }
    else if(!pressed[lindex])
    {
        pressed[lindex]=true;
        ui->_1152->selectionModel()->clear();
        for(int i=0; i<ui->_1152->model()->rowCount(); i++)
            ui->_1152->model()->setData(ui->_1152->model()->index(i,lindex), Qt::Checked, Qt::CheckStateRole);

        if(lindex == 0)
            mEntityVisibilityMasks->setall();
        else if(lindex == 1)
            mEntitySelectabilityMasks->setall();
    }
}

void MaskChange_Dialog::on__1152_currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //qDebug() << "on__1152_currentChanged : current row " << current.row() << " previous row " << previous.row();
}

void MaskChange_Dialog::on__1152_selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndex index;


    QModelIndexList items = selected.indexes();
    foreach (index, items)
    {
        ui->_1151->selectRow(index.row());

        int value = index.data(Qt::CheckStateRole).toInt();
        if(value != 0)
            value = Qt::Unchecked;
        else
            value = Qt::Checked;
        ui->_1152->model()->setData(index, value, Qt::CheckStateRole);

        if(index.column() == 0)
            mEntityVisibilityMasks->toggle(entityCodes.at(index.row()));
        else if(index.column() == 1)
            mEntitySelectabilityMasks->toggle(entityCodes.at(index.row()));
    }
}


/*
void MaskChange_Dialog::on__106_clicked(bool checked)
{
    if(ui->_100->currentIndex() == 0)
    {
        // layers
        for(int i=0; i < ui->_1111->model()->rowCount(); i++)
        {
            int pos = ui->_1111->model()->index(i,0).data(Qt::UserRole+1).toInt();
            RCCHAR layername[300];
            strcpy(layername,ui->_1111->model()->index(i,2).data().toString());
            db.layernames.setlayername(pos,layername);
        }

        LayerGroup_Dialog dlg(this,RCdlg);

        dlg.setCurrentPalette(ui->_108->currentText());

        int ret = dlg.exec();
        loadLayerPalettes();
        if(ret >= 0)
        {
            ui->_108->setCurrentIndex(ret);
            on__108_activated(ui->_108->currentText());
        }
    }
    else if(ui->_100->currentIndex() == 2 ||
            ui->_100->currentIndex() == 3)
    {
        // colour change and style change
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;

        msg = WM_COMMAND;

        // click 106
        wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_106;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
}
*/
void MaskChange_Dialog::on__107_clicked(bool checked)
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

// save palette button
void MaskChange_Dialog::on__120_clicked(bool checked)
{
    // get the user to select a file name
    QString ftype("rcp");
    app->processEvents();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Colour Palette File"),
                                                    QString(home.getpublicfilename("")),
                                                    tr("Palette Files (*.rcp)"),&ftype
                                                    /*,QFileDialog::DontUseNativeDialog*/);
    app->processEvents();

    if(fileName.length() > 0)
    {
        // save the current colour table to an ini format file
        char key[50],svalue[256];
        long lvalue;
        if( ! fileName.endsWith(".rcp") )
            fileName.append(".rcp");
        for(int i=0; i<256; i++)
        {
            sprintf(key,"cl::%d",i);
            lvalue = (long(db.colourtable[i].red) << 16) | (long(db.colourtable[i].green) << 8) | db.colourtable[i].blue;
            sprintf(svalue,"%ld",lvalue);
            WritePrivateProfileString("ColourPalette",key,svalue,fileName.data());
        }
    }
}

// load palette button
void MaskChange_Dialog::on__109_clicked(bool checked)
{
    // get the user to select a file name
    QString ftype("");
    app->processEvents();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Colour Palette File"),
                                                    QString(home.getpublicfilename("")),
                                                    tr("Palette Files (*.rcp)"),&ftype
                                                    /*,QFileDialog::DontUseNativeDialog*/);

    app->processEvents();

    if(fileName.length() > 0)
    {
        // read the current table from and ini format file
        char key[50],svalue[256];
        long lvalue;
        for(int i=0; i<256; i++)
        {
            sprintf(key,"cl::%d",i);
            GetPrivateProfileString("ColourPalette",key,"",svalue,256,fileName.data());
            if (strlen(svalue) != 0)
              sscanf(svalue,"%ld",&lvalue);
            else
                lvalue = 0;
            db.colourtable[i].set(int((lvalue >>16)&255),int((lvalue>>8)&255),int(lvalue&255));
        }
        initColourTab();
        ui->_100->update();
        app->processEvents();
    }
}

// define pattern style button
void MaskChange_Dialog::on__111_clicked(bool checked)
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

void MaskChange_Dialog::on__112_clicked(bool checked)
{
    db.setlineweightdisplay((int)checked);
    v.setinteger("db::lineweightdisplay",db.getlineweightdisplay());
}

void MaskChange_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
    bool dismaskChanged=false;
    RCCHAR value[600];

    for(int i=0; i<ui->_1151->model()->rowCount(); i++)
    {
        if(db.dismask.entity.test(i) != mEntityVisibilityMasks->test(i))
            dismaskChanged = true;
        //qDebug() << "disp entity [" << entityCodes.at(i) << "] = " << mEntityVisibilityMasks->test(entityCodes.at(i));

        db.dismask.entity.set(entityCodes.at(i),mEntityVisibilityMasks->test(entityCodes.at(i)));
        qDebug() << "disp entity [" << entityCodes.at(i) << "] = " << db.dismask.entity.test(entityCodes.at(i));

        state.tempentitymask.set(entityCodes.at(i),mEntitySelectabilityMasks->test(entityCodes.at(i)));
        qDebug() << "temp entity [" << entityCodes.at(i) << "] = " << state.tempentitymask.test(entityCodes.at(i));

        state.selmask.entity.set(entityCodes.at(i),mEntitySelectabilityMasks->test(entityCodes.at(i)));
        qDebug() << "selmask entity [" << entityCodes.at(i) << "] = " << state.selmask.entity.test(entityCodes.at(i));

        //qDebug() << "set bit[" << entityCodes.at(i) << "] = " << mEntitySelectabilityMasks->test(entityCodes.at(i));
        //qDebug() << "bit[" << entityCodes.at(i) << "] = " << state.selmask.entity.test(entityCodes.at(i));
    }

    //for(int i=0; i<32; i++)
    //{
    //    qDebug() << "bit[" << i << "] = " << state.selmask.entity.test(i);
    //}

    // update the masks
    ui->_1111->sortByColumn(1,Qt::AscendingOrder);
    ui->_1112->sortByColumn(2,Qt::AscendingOrder);
    for(int i=0; i < ui->_1111->model()->rowCount(); i++)
    {
        //int pos = ui->_1111->model()->index(i,0).data(Qt::UserRole+1).toInt();
        if(db.dismask.layer.test(i) != mLayerVisibilityMasks->test(i))
            dismaskChanged = true;
        db.dismask.layer.set(i,mLayerVisibilityMasks->test(i));
        //qDebug() << "dismask layer :" << db.dismask.layer.test(i);

        state.selmask.layer.set(i,mLayerSelectabilityMasks->test(i));
        //qDebug() << "selmask layer :" << state.selmask.layer.test(i);

        RCCHAR layername[300];
        strcpy(layername,ui->_1111->model()->index(i,2).data().toString());
        db.layernames.setlayername(i,layername);
    }

    for(int i=1; i<db.lineweights.numweights(); i++)
    {
        if(db.dismask.weight.test(i) != mWeightVisibilityMasks->test(i))
            dismaskChanged = true;
        db.dismask.weight.set(i,mWeightVisibilityMasks->test(i));
        state.selmask.weight.set(i,mWeightSelectabilityMasks->test(i));
    }

    for(int i=0; i<256; i++)
    {
        if(db.dismask.colour.test(i) != mColourVisibilityMasks->test(i))
            dismaskChanged = true;
        db.dismask.colour.set(i,mColourVisibilityMasks->test(i));
        state.selmask.colour.set(i,mColourSelectabilityMasks->test(i));

    }

    for(int i=0; i<256; i++)
    {
        if(db.dismask.style.test(i) != mStyleVisibilityMasks->test(i))
            dismaskChanged = true;
        db.dismask.style.set(i,mStyleVisibilityMasks->test(i));
        state.selmask.style.set(i,mStyleSelectabilityMasks->test(i));
    }

    /*
        qDebug() << "tempentitymask after before : ";
        for(i=0; i < entityNames.size(); i++)
        {
            if(state.tempentitymask.test(entityCodes.at(i)))
                qDebug() << "tempentitymask [" << i << "] = 1";
            else
                qDebug() << "tempentitymask  [" << i << "] = 0";
        }
        qDebug() << "db.dismask.layer after : end";
    */

    // update the last layer group used
    sprintf(value,"%ls",ui->_108->currentText().data());
    WritePrivateProfileString(_RCT("LayerGroups"),_RCT("LastLayerGroup"),value,home.getinifilename());

    // update the currrent tab
    QString ctab = QString("%1").arg(ui->_100->currentIndex());
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),ctab.data(),home.getinifilename());

    // click OK

    // send the end IDOK command
    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    if(dismaskChanged)
        cadwindow->paintall();

    QDialog::accept();
}

void MaskChange_Dialog::on__100_currentChanged(int index)
{
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    //qDebug() << "on__100_currentChanged : " << index;

    if(index == 0)
    { // layer tab
        //ui->_106->setText("Edit Palette");
        //ui->_106->show();
        ui->_107->show();
        //ui->CloneIcon->show();
        //ui->_108_label->show();
        //ui->_108->show();
        //ui->_109->hide();
        //ui->_120->hide();
        //ui->_111->hide();
        //ui->_112->hide();

        msg = WM_COMMAND;

        // click button 101 = tab 0
        wParam = MAKEWPARAM((WORD)101,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);

        QModelIndex index1 = ui->_1111->currentIndex();
        QModelIndex index2 = ui->_1112->currentIndex();
        ui->_1111->scrollTo(index1,QAbstractItemView::PositionAtCenter);
        ui->_1112->scrollTo(index2,QAbstractItemView::PositionAtCenter);

    }
    else if(index == 1)
    { // weight tab
        //ui->_106->hide();
        ui->_107->show();
        //ui->CloneIcon->show();
        //ui->_108_label->hide();
        //ui->_108->hide();
        //ui->_109->hide();
        //ui->_120->hide();
        //ui->_111->hide();
        ui->_112->setChecked((db.getlineweightdisplay()==1 ? true : false));
        ui->_112->show();

        msg = WM_COMMAND;

        // click button 102 = tab 1
        wParam = MAKEWPARAM((WORD)102,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(index == 2)
    { // colour tab
        //ui->_106->setText("Change");
        //ui->_106->setEnabled(true);
        //ui->_106->show();
        ui->_107->show();
        //ui->CloneIcon->show();
        //ui->_108_label->hide();
        //ui->_108->hide();
        //ui->_109->show();
        //ui->_120->show();
        //ui->_111->hide();
        //ui->_112->hide();

        msg = WM_COMMAND;

        // click button 103 = tab 2
        wParam = MAKEWPARAM((WORD)103,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(index == 3)
    { // style tab
        //ui->_106->setText("Change");
        //ui->_106->show();
        ui->_107->show();
        //ui->CloneIcon->show();
        //ui->_108_label->hide();
        //ui->_108->hide();
        //ui->_109->hide();
        //ui->_120->hide();
        //ui->_111->show();
        //ui->_112->hide();

        msg = WM_COMMAND;

        // click button 104 = tab 3
        wParam = MAKEWPARAM((WORD)104,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
    else if(index == 4)
    { // entity tab
        //ui->_106->hide();
        ui->_107->hide();
        //ui->CloneIcon->hide();
        //ui->_108_label->hide();
        //ui->_108->hide();
        //ui->_109->hide();
        //ui->_120->hide();
        //ui->_111->hide();
        //ui->_112->hide();

        msg = WM_COMMAND;

        // click button 105 = tab 4
        wParam = MAKEWPARAM((WORD)105,(WORD)BN_CLICKED);
        lParam = (LPARAM)ui->_100;
        dialogcb((HWND)this,msg,wParam,lParam);
    }
}

void MaskChange_Dialog::on__108_activated(const QString & groupName)
{
    showLayerGroup(groupName);
}

void MaskChange_Dialog::showLayerGroup(const QString &groupName)
{
    //qDebug() << "Entering : showLayerGroup";

    // show the member of the given group in the table
    // hide the non members

    // read the group data
    // set up the layers to be shown
    // this is either all layers or layers in the selected named group

    int i;
    RCCHAR key[300],value[600],*s,name[300];
    BitMask  showLayers(MAX_LAYERS);
    showLayers.setall();
    if(groupName.length() > 0 && groupName != "All Layers")
    {
        showLayers.clearall();
        for (i = 1 ; ; i++)
        {
             sprintf(key,"LayerGroupName%d",i);
             GetPrivateProfileString(_RCT("LayerGroups"),key,_RCT(""),value,600,home.getinifilename());
             if (strlen(value) == 0)
               break;
             s = strtok(value,_RCT("\t"));
             if (s != 0 && _stricmp(s,groupName.data()) == 0)
             {
                  s = strtok(0,_RCT("\t"));
                  if (s != 0 && _stricmp(s,"Deleted") != 0)
                  {
                      showLayers.loadascii(QString(s).toLatin1().data());
                  }
                  break;
             }
        }

        // hide the non members and show the members
        // and reset sort both tables to be layer number ascending
        ui->_1111->sortByColumn(1,Qt::AscendingOrder);
        ui->_1112->sortByColumn(2,Qt::AscendingOrder);
        int count=0;
        for( i=0; i < MAX_LAYERS; i++)
        {
            if(!showLayers.test(ui->_1111->model()->index(i,0).data(Qt::UserRole+1).toInt()))
            {
                ui->_1111->hideRow(i);
                ui->_1112->hideRow(i);
            }
            else
            {
                ui->_1111->showRow(i);
                ui->_1112->showRow(i);
            }
        }

        // record the last used group name
        //sprintf(value,"%ls",groupName.data());
        //WritePrivateProfileString(_RCT("LayerGroups"),_RCT("LastLayerGroup"),value,home.getinifilename());

    }
    else
    {
        // show all layers and
        // reset sort boths tables to be layer number ascending
        ui->_1111->sortByColumn(1,Qt::AscendingOrder);
        ui->_1112->sortByColumn(2,Qt::AscendingOrder);
        for( i=0; i < MAX_LAYERS; i++)
        {
            ui->_1111->showRow(i);
            ui->_1112->showRow(i);
        }

        // record the last used group name as empty
        WritePrivateProfileString(_RCT("LayerGroups"),_RCT("LastLayerGroup"),_RCT(""),home.getinifilename());

    }
}

void MaskChange_Dialog::setCurrentTab(int index)
{
    //qDebug() << "setCurrentTab : " << index;
    ui->_100->setCurrentIndex(index);
}

void MaskChange_Dialog::selectRow(int row, int tab)
{
    if(tab >= 0 && tab < 5)
        ui->_100->setCurrentIndex(tab);

    switch(ui->_100->currentIndex())
    {
    case 0: // layer tab
        ui->_1111->selectRow(row);
        break;
    case 1: // weight tab
        ui->_1121->selectRow(row);
        break;
    case 2: // colour tab
        ui->_1131->selectRow(row);
        break;
    case 3: // style tab
        ui->_1141->selectRow(row);
        break;
    case 4: // entity tab
        ui->_1151->selectRow(row);
        break;
    }
}
/*
void MaskChange_Dialog::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "MaskChange_Dialog::resizeEvent" ;
    QDialog::resizeEvent(e);
}
*/
void MaskChange_Dialog::on__114_clicked()
{
    // colour change and style change
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 114
    wParam = MAKEWPARAM((WORD)114,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_114;
    dialogcb((HWND)this,msg,wParam,lParam);
}

// layer number or name search function
void MaskChange_Dialog::findLayerStartsWith(QString searchString)
{
    // find a matching number and scroll it to the top of the viewport
    QModelIndex index;
    if(lastFoundLayer >= MAX_LAYERS)
        lastFoundLayer = -1;
    for(int i=0; i<MAX_LAYERS; i++)
    {
        index = ui->_1111->model()->index(i,1);
        QString str = index.data().toString();
        if(index.data().toString().trimmed().startsWith(searchString,Qt::CaseInsensitive))
        {
            ui->_1111->scrollTo(index, QAbstractItemView::PositionAtTop);
            // scroll the other table to the same row
            index = ui->_1112->model()->index(index.row(),0);
            ui->_1112->scrollTo(index, QAbstractItemView::PositionAtTop);
            lastFoundLayer = i;
            return;
        }
        bool ok=false;
        int num = searchString.toInt(&ok);
        if(ok)
        {
            if(lastFoundLayer > 0 && i == MAX_LAYERS-1)
            {
                lastFoundLayer=0;
                i=-1;
            }
        }
    }

    // restart the search
    if(lastFoundLayer >= MAX_LAYERS)
        lastFoundLayer = -1;
    // find the with a match and scroll it to the top of the viewport
    for(int i=lastFoundLayer+1; i<MAX_LAYERS; i++)
    {
        index = ui->_1111->model()->index(i,2);
        if(index.data().toString().contains(searchString,Qt::CaseInsensitive))
        {
            ui->_1111->scrollTo(index, QAbstractItemView::PositionAtTop);
            // scroll the other table to the same row
            index = ui->_1112->model()->index(index.row(),0);
            ui->_1112->scrollTo(index, QAbstractItemView::PositionAtTop);
            lastFoundLayer = i;
            return;
        }
        if(lastFoundLayer > 0 && i == MAX_LAYERS-1)
        {
            lastFoundLayer=0;
            i=0;
        }
    }

    lastFoundLayer = -1;
}

// colour number
void MaskChange_Dialog::findColourStartsWith(QString searchString)
{
    // find a matching number and scroll it to the top of the viewport
    QModelIndex index;
    if(lastFoundColour >= 256)
        lastFoundColour = -1;
    for(int i=0; i<256; i++)
    {
        index = ui->_1131->model()->index(i,1);
        QString str = index.data().toString();
        if(index.data().toString().trimmed().startsWith(searchString,Qt::CaseInsensitive))
        {
            ui->_1131->scrollTo(index, QAbstractItemView::PositionAtTop);
            // scroll the other table to the same row
            index = ui->_1132->model()->index(index.row(),0);
            ui->_1132->scrollTo(index, QAbstractItemView::PositionAtTop);
            lastFoundColour = i;
            return;
        }
        bool ok=false;
        int num = searchString.toInt(&ok);
        if(ok)
        {
            if(lastFoundColour > 0 && i == 255)
            {
                lastFoundColour = 0;
                i=-1;
            }
        }
    }

    lastFoundColour = -1;
}

// layer search button
void MaskChange_Dialog::on__115_clicked()
{
    // find a layer number or name that maches the text box
    QString searchStr = ui->_116->text();
    if( !searchStr.isEmpty() )
        findLayerStartsWith(searchStr);
}

// layer search edit box
void MaskChange_Dialog::on__116_editingFinished()
{
    // find a layer number or name that maches the text box
    QString searchStr = ui->_116->text();
    if( !searchStr.isEmpty() )
        findLayerStartsWith(searchStr);
}

void MaskChange_Dialog::on__116_textEdited(const QString &arg1)
{
    if( !arg1.isEmpty() )
        findLayerStartsWith(arg1);
}

void MaskChange_Dialog::on__106_clicked()
{
    // colour change and style change
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 106
    wParam = MAKEWPARAM((WORD)106,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_106;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void MaskChange_Dialog::on__113_clicked()
{
    // layers
    for(int i=0; i < ui->_1111->model()->rowCount(); i++)
    {
        int pos = ui->_1111->model()->index(i,0).data(Qt::UserRole+1).toInt();
        RCCHAR layername[300];
        strcpy(layername,ui->_1111->model()->index(i,2).data().toString());
        db.layernames.setlayername(pos,layername);
    }

    LayerGroup_Dialog dlg(this,RCdlg);

    // do not put the "All Layers" in the combobox
    if(ui->_108->currentText() != "All Layers")
        dlg.setCurrentPalette(ui->_108->currentText());
    else
        dlg.setCurrentPalette("");

    int ret = dlg.exec();
    loadLayerPalettes();
    if(ret >= 0)
    {
        ui->_108->setCurrentIndex(ret);
        on__108_activated(ui->_108->currentText());
    }
}


void MaskChange_Dialog::on__117_editingFinished()
{
    QString searchStr = ui->_117->text();
    if( !searchStr.isEmpty() )
        findColourStartsWith(searchStr);
}

void MaskChange_Dialog::on__117_textEdited(const QString &arg1)
{
    if( !arg1.isEmpty() )
        findColourStartsWith(arg1);
}

void MaskChange_Dialog::on__118_clicked()
{
    QString searchStr = ui->_117->text();
    if( !searchStr.isEmpty() )
        findColourStartsWith(searchStr);
}

void MaskChange_Dialog::on__119_clicked()
{
    db.initColourTable();
    db.saveColourTable(home.getinifilename());
    // reset the values colour entries to
    RCCHAR key[256];
    for (int i = 0 ; i < 256 ; i++)
    {
        sprintf(key,"cl::%d",i);
        RCCOLORREF c; db.colourtable[i].get(&c);
        v.setlong(key,(long)c);
    }
    initColourTab();
    ui->_100->update();
}
