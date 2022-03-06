#ifndef LAYERGROUP_DIALOG_H
#define LAYERGROUP_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

// style table delegate
class LayerPaletteItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LayerPaletteItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;

private slots:
};

namespace Ui {
class LayerGroup_Dialog;
}

class LayerGroup_Dialog : public RCDialog
{
    Q_OBJECT
    
public:
    explicit LayerGroup_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f = 0);
	Q_INVOKABLE LayerGroup_Dialog(HANDLE parent, HANDLE rcparent,  int f);
    ~LayerGroup_Dialog();
    
    void populatePaletteList();
    void populateTable();
    void showPalette(const QString &paletteName);
    void setCurrentPalette(const QString &paletteName);
    void updatePalette(const QString &paletteName);
    void deletePalette(const QString &paletteName);

public slots:
    void on__100_activated(const QString & paletteName);
    void checkPaletteHasName(const QString & text);
    void on__300_clicked(bool checked);
    void on__400_clicked(bool checked);
    void on_accept();

protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    Ui::LayerGroup_Dialog *ui;
};

#endif // LAYERGROUP_DIALOG_H
