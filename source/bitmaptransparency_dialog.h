#ifndef BITMAPTRANSPARENCYDIALOG_H
#define BITMAPTRANSPARENCYDIALOG_H

#include "ncwin.h"
#include "RCDialog.h"
#include <QDialog>

class BitmapPickFilter : public QObject
{
    Q_OBJECT
public:
    explicit BitmapPickFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class BitmapTransparency_Dialog;
}

class BitmapTransparency_Dialog : public RCDialog
{
    friend class BitmapPickFilter;

    Q_OBJECT

public:
    QString bitmapPath;
    QImage bitmap;

public:
    explicit BitmapTransparency_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE BitmapTransparency_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~BitmapTransparency_Dialog();

    void drawSample(int flags=3);

protected:
    bool sliderIsReleased;
    //bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *);

private slots:
    void on__1_clicked();

    void on__2_clicked();

    void on__102_valueChanged(double arg1);

    void on__103_valueChanged(int value);

    void on__103_sliderPressed();

    void on__103_sliderReleased();

    void on__1011_clicked();

    void on__1012_clicked();

    void on__1013_clicked();

    void on__104_clicked();

    void on__105_clicked();

private:
    Ui::BitmapTransparency_Dialog *ui;
};

#endif // BITMAPTRANSPARENCYDIALOG_H
