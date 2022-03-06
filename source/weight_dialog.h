#ifndef WEIGHT_DIALOG_H
#define WEIGHT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Weight_Dialog;
}

class WeightItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit WeightItemDelegate(QWidget *parent = 0);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const ;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const ;
};

class Weight_Dialog : public RCDialog
{
    Q_OBJECT

    //friend long dialogcb(HWND,UINT,WPARAM,LPARAM);
public:
    explicit Weight_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f = 0);
	Q_INVOKABLE Weight_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Weight_Dialog();

    /*
    QWidget* getControl(int id);
    QWidget* getControl(QString name);
    */
signals:   

public slots:
    void on__100_valueChanged(double v);
    void on__101_clicked(const QModelIndex & index);
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::Weight_Dialog *ui;
};

#endif // WEIGHT_DIALOG_H
