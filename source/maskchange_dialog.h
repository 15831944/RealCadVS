#ifndef MASKCHANGE_DIALOG_H
#define MASKCHANGE_DIALOG_H

#include "ncwin.h"

#include <QObject>
#include <QWidget>
#include "RCDialog.h"
#include "layergroup_dialog.h"

class QWidget;

class PropertySwitcher
{
protected:
    bool mChecked;
public:
    explicit PropertySwitcher(bool checked=true) {
        mChecked = checked;
    }
    void setChecked(bool checked) {
        mChecked = checked;
    }
    bool checked() {
        return mChecked;
    }
    void toggle() {
        mChecked = !mChecked;
    }

    virtual void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette) const {;}
    virtual QSize sizeHint() const { return QSize();}
};
Q_DECLARE_METATYPE(PropertySwitcher*)

class VisibilitySwitcher : public PropertySwitcher
{
public:
    explicit VisibilitySwitcher(bool checked=true)
        : PropertySwitcher(checked) {;}
    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette) const;
    //QSize sizeHint() const;
};
Q_DECLARE_METATYPE(VisibilitySwitcher*)

class SelectabilitySwitcher : public PropertySwitcher
{
public:
    explicit SelectabilitySwitcher(bool checked=true)
        : PropertySwitcher(checked) {;}
    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette) const;
    //QSize sizeHint() const;
};
Q_DECLARE_METATYPE(SelectabilitySwitcher*)

class PropertyItemEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyItemEditor(QWidget *parent = 0);

    QSize sizeHint() const;
    void setSwitcher( PropertySwitcher *switcher) {
        mSwitcher = switcher;
    }
    PropertySwitcher* switcher() {
        return mSwitcher;
    }

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    PropertySwitcher *mSwitcher;

};

class MaskTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QTableView *mTableView;
public:
    explicit MaskTableItemDelegate(QWidget *parent = 0, QTableView *tableView = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    /*
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex & index ) const;
private slots:
    void commitAndCloseEditor();
    */

};


// layer table delegates
class LayerTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LayerTableItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

// layer table delegates
class LayerTableEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LayerTableEditDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;

//protected:
//    bool eventFilter(QObject * editor, QEvent * event);
};

// weight table delegates
class WeightTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit WeightTableItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

// colour table delegate
class ColourTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ColourTableItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

// style table delegate
class StyleTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StyleTableItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

// entity table delegate
class EntityTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit EntityTableItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

class LayerTableKeyPressFilter : public QObject
{
    Q_OBJECT
public:
    explicit LayerTableKeyPressFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class MaskChange_Dialog;
}

class MaskChange_Dialog : public RCDialog
{
    Q_OBJECT

    int mLastRow;
    int startWidth;
    QPoint firstPos;
    int lastFoundLayer;
    int lastFoundColour;
    Qt::SortOrder layerSortOrder;

public:
    explicit MaskChange_Dialog(QWidget *parent = 0, Dialog *rcparent=0, int tab=0, Qt::WindowFlags f = 0);
	Q_INVOKABLE MaskChange_Dialog(HANDLE parent, HANDLE rcparent,int tab, int f);
    ~MaskChange_Dialog();

    void initLayerTab();
    void initWeightTab();
    void initColourTab();
    void initStyleTab();
    void initEntityTab();
    int lastRow() { return mLastRow; }
    void showLayerGroup(const QString &groupName);
    void loadLayerPalettes();
    void setCurrentTab(int index);
    void selectRow(int row, int tab=-1);
    void findLayerStartsWith(QString searchString);
    void findColourStartsWith(QString searchString);
    void scrollLayerSelectorTo(int row){;}

protected:
    bool eventFilter(QObject *o, QEvent *e);
    //void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent * event);

signals:

public slots:
    // layer tab functions
    void on__1111_entered(const QModelIndex & index);
    void on__1111_clicked(const QModelIndex & index);
    void on__1111_headerPressed(int lindex);
    void on__1111_selectionChanged(QItemSelection, QItemSelection);
    void on__1111_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1112_clicked(const QModelIndex & index);
    void on__1112_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1112_selectionChanged(QItemSelection, QItemSelection);
    void on__1112_headerPressed(int lindex);
    // weight tab functions
    void on__1121_entered(const QModelIndex & index);
    void on__1121_clicked(const QModelIndex & index);
    void on__1122_clicked(const QModelIndex & index);
    void on__1122_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1122_selectionChanged(QItemSelection, QItemSelection);
    void on__1122_headerPressed(int lindex);
    // colour tab functions
    void on__1131_entered(const QModelIndex & index);
    void on__1131_clicked(const QModelIndex & index);
    void on__1131_headerPressed(int lindex);
    void on__1132_clicked(const QModelIndex & index);
    void on__1132_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1132_selectionChanged(QItemSelection, QItemSelection);
    void on__1132_headerPressed(int lindex);
    // style tab functions
    void on__1141_entered(const QModelIndex & index);
    void on__1141_clicked(const QModelIndex & index);
    void on__1142_clicked(const QModelIndex & index);
    void on__1142_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1142_selectionChanged(QItemSelection, QItemSelection);
    void on__1142_headerPressed(int lindex);
    // Entity tab functions
    void on__1151_entered(const QModelIndex & index);
    void on__1151_clicked(const QModelIndex & index);
    void on__1152_clicked(const QModelIndex & index);
    void on__1152_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void on__1152_selectionChanged(QItemSelection, QItemSelection);
    void on__1152_headerPressed(int lindex);
    // common buttons and tabs
    void on__100_currentChanged(int index);
    //void on__106_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on__108_activated(const QString & groupName);
    // save palette button
    void on__109_clicked(bool checked);
    // load palette button
    void on__120_clicked(bool checked);
    // Define pattern style button
    void on__111_clicked(bool checked);
    void on__112_clicked(bool checked);
    void on_accept();
    void on__115_clicked();
    void on__116_editingFinished();
    void on__114_clicked();
    void on__106_clicked();
    void on__113_clicked();
    void on__116_textEdited(const QString &arg1);

private slots:
    void on__117_editingFinished();

    void on__117_textEdited(const QString &arg1);

    void on__118_clicked();

    void on__119_clicked();

private:
    Ui::MaskChange_Dialog *ui;

    QStringList entityNames;
    QList<int> entityCodes;

    BitMask *mLayerVisibilityMasks;
    BitMask *mLayerSelectabilityMasks;

    BitMask *mWeightVisibilityMasks;
    BitMask *mWeightSelectabilityMasks;

    BitMask *mColourVisibilityMasks;
    BitMask *mColourSelectabilityMasks;

    BitMask *mStyleVisibilityMasks;
    BitMask *mStyleSelectabilityMasks;

    BitMask *mEntityVisibilityMasks;
    BitMask *mEntitySelectabilityMasks;

};

#endif // MASKCHANGE_DIALOG_H
