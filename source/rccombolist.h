#ifndef RCCOMBOLIST_H
#define RCCOMBOLIST_H

#include <QtWidgets/QWidget>
#include <QAbstractItemView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include "ui_rccombolist.h"

class QListWidget;
class QListWidgetItem;

namespace Ui {
class RCComboList;
}

class RCComboList : public QListWidget // ÅÁQWidget
{
    Q_OBJECT
    
    int currentIndex;
    bool editable;

public:
    explicit RCComboList(QWidget *parent = 0);
    ~RCComboList();
    
    QString	currentText() const;
    void adjustListSize();
    int count();
    int findText(const QString & text, Qt::MatchFlags flags = static_cast<Qt::MatchFlags> ( Qt::MatchExactly | Qt::MatchCaseSensitive )) const;
    void addItem(const QString & label);
    void removeItem(int index);
    void setEditTriggers(QAbstractItemView::EditTrigger triggers);
    void setSortingEnabled(bool);
    QList<QListWidgetItem *> selectedItems();
    QListWidgetItem* item(int index);

    void setHorizontalScrollBar(QScrollBar *scrollBar);
    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);
    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);
    void setVerticalScrollMode(QAbstractItemView::ScrollMode mode);
    void setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior);
    void setSelectionMode(QAbstractItemView::SelectionMode mode);
    void setUniformItemSizes(bool enable);
    void setAutoScrollMargin(int margin);

protected:
    void changeEvent(QEvent *e);
    //void mouseDoubleClickEvent(QMouseEvent * event);

public slots:
    void clear();
    void clearEditText();
    void setEditText(const QString & text);
    void setCurrentText(const QString & text);
    void setCurrentIndex(int index);

signals:
    void activated(int index);
    void activated(const QString & text);
    void currentRowChanged(int currentRow);
    void currentIndexChanged(int index);
    void currentIndexChanged(const QString & text);
    void currentTextChanged(const QString & text);
    void editTextChanged(const QString & text);
    void highlighted(int index);
    void highlighted(const QString & text);
    void listItemDoubleClicked(QListWidgetItem * item);
    void itemClicked(QListWidgetItem * item);
public:
    Ui::RCComboList *ui;
private slots:

    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // RCCOMBOLIST_H
