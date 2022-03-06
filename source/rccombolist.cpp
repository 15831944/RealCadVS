#include "rccombolist.h"
#include "ui_rccombolist.h"


RCComboList::RCComboList(QWidget *parent) :
    //QWidget(parent),
    QListWidget(parent),
    ui(new Ui::RCComboList)
{
    ui->setupUi(this);

    currentIndex = -1;
    editable = false;

    adjustSize();

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->listWidget->setAutoScroll(true);
    //ui->listWidget->setAutoScrollMargin(16);
    ui->listWidget->adjustSize();

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setCurrentIndex(int)));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SIGNAL(listItemDoubleClicked(QListWidgetItem*)));
}

RCComboList::~RCComboList()
{
    delete ui;
}

void RCComboList::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RCComboList::clear()
{
    clearEditText();
    ui->listWidget->clear();
}

void RCComboList::clearEditText()
{
    ui->lineEdit->setText("");
}

void RCComboList::setEditText(const QString & text)
{
    ui->lineEdit->setText(text);
}

void RCComboList::setCurrentText(const QString & text)
{
    ui->lineEdit->setText(text);
    currentIndex = findText(text);
    ui->listWidget->setCurrentRow(currentIndex);
}

QString RCComboList::currentText() const
{
    if(ui->listWidget->currentItem())
        return ui->listWidget->currentItem()->text();
    else
        return QString("");
}

void RCComboList::setCurrentIndex(int index)
{
    QListWidgetItem *item = ui->listWidget->item(index);
    if(item)
    {
        QString text = item->text();
        ui->lineEdit->setText(text);
        ui->listWidget->setCurrentRow(index);
        ui->listWidget->setCurrentItem(item);
        currentIndex = index;
    }
    emit currentRowChanged(index);
}

int RCComboList::count()
{
    return ui->listWidget->count();
}

int RCComboList::findText(const QString & text, Qt::MatchFlags flags) const
{
    QList<QListWidgetItem *> list = ui->listWidget->findItems(text, flags);
    if(list.size() > 0)
        return ui->listWidget->row(list.at(0));
}

 void RCComboList::addItem(const QString & label)
 {
     ui->listWidget->addItem(label);
     ui->listWidget->sortItems();
 }

void RCComboList::removeItem(int index)
{
    QListWidgetItem *item = ui->listWidget->item(index);
    if(item)
    {
        item = ui->listWidget->takeItem(index);
        if(item)
            delete item;
    }
}

void RCComboList::adjustListSize()
{
    ui->listWidget->setMinimumWidth( width());
    ui->listWidget->setMaximumWidth( width());
    ui->listWidget->setMinimumHeight(height()-ui->lineEdit->height());
    ui->listWidget->setMaximumHeight(height()-ui->lineEdit->height());
}

void RCComboList::setEditTriggers(QAbstractItemView::EditTrigger triggers)
{
    ui->listWidget->setEditTriggers(triggers);
}

void RCComboList::setSortingEnabled(bool flag)
{
    ui->listWidget->setSortingEnabled(flag);
}

QList<QListWidgetItem *> RCComboList::selectedItems()
{
    return ui->listWidget->selectedItems();
}

QListWidgetItem* RCComboList::item(int index)
{
    return ui->listWidget->item(index);
}

void RCComboList::setHorizontalScrollBar(QScrollBar *scrollBar)
{
    ui->listWidget->setHorizontalScrollBar(scrollBar);
}

void RCComboList::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    ui->listWidget->setHorizontalScrollBarPolicy(policy);
}

void RCComboList::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    ui->listWidget->setVerticalScrollBarPolicy(policy);
}

void RCComboList::setVerticalScrollMode(QAbstractItemView::ScrollMode mode)
{
    ui->listWidget->setVerticalScrollMode(mode);
}

void RCComboList::setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
{
    ui->listWidget->setSelectionBehavior(behavior);
}

void RCComboList::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    ui->listWidget->setSelectionMode(mode);
}

void RCComboList::setUniformItemSizes(bool enable)
{
    ui->listWidget->setUniformItemSizes(enable);
}

void RCComboList::setAutoScrollMargin(int margin)
{
    ui->listWidget->setAutoScrollMargin(margin);
}


void RCComboList::on_listWidget_itemClicked(QListWidgetItem *item)
{
    emit(itemClicked(item));
}
