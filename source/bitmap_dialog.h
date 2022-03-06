#ifndef BITMAP_DIALOG_H
#define BITMAP_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Bitmap_Dialog;
}

class BitMapEVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit BitMapEVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Bitmap_Dialog : public RCDialog
{
    friend class BitMapEVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_106;
    bool variesWasSet_107;
    bool variesWasSet_112;
    bool variesWasSet_113;
    bool variesWasSet_114;
    //bool variesWasSet_116;
    bool variesWasSet_117;
    bool variesWasSet_126;
    int changecount;
    bool change;
    BitMask * changeBits;
    bool sliderIsReleased;

public:
    explicit Bitmap_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Bitmap_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Bitmap_Dialog();

    void drawSample();

signals:

public slots:
    void on__100_valueChanged(int v); // def colour
    void on__101_valueChanged(int v); // def layer
    void on__102_valueChanged(int v); // def style
    void on__103_valueChanged(double v); // def weight
    void on__104_clicked(bool checked); // original shape
    void on__105_clicked(bool checked); // link to file
    void on__106_valueChanged(double v); // width
    void on__107_valueChanged(double v); // height
    void on__108_currentIndexChanged(int index); // horizontal justification
    void on__109_currentIndexChanged(int index); // vertical justification
    void on__110_currentIndexChanged(int index); // frame style
    void on__111_clicked(bool checked); // transparent flag
    //void on__112_valueChanged(double v); // tr
    //void on__113_valueChanged(double v); // tg
    //void on__114_valueChanged(double v); // tb
    //void on__1005_clicked(bool checked); // trans colour select
    //void on__116_valueChanged(double v); // trange
    void on__117_valueChanged(double v); // opacity

    void on__119_clicked(bool checked);// clone button
    void on__120_clicked(bool checked);// clone all button

    void on__1000_clicked(bool checked);// colour selector
    void on__1001_clicked(bool checked);// layer selector
    void on__1002_clicked(bool checked);// style selector
    void on__1003_clicked(bool checked);// weight selector

    void on_accept();

private slots:
    void on__126_valueChanged(double arg1);

    void on__1006_clicked();

    void on__1007_clicked();

    void on__303_sliderPressed();

    void on__303_sliderReleased();

    void on__303_valueChanged(int value);

    void on__1011_clicked();

    void on__1012_clicked();

    void on__1013_clicked();

private:
    Ui::Bitmap_Dialog *ui;
};

#endif // BITMAP_DIALOG_H
