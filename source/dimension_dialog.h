#ifndef DIMENSION_DIALOG_H
#define DIMENSION_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Dimension_Dialog;
}

class DimensionVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit DimensionVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Dimension_Dialog : public RCDialog
{
    friend class DimensionVerifyEventFilter;

    Q_OBJECT
public:
    explicit Dimension_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Dimension_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Dimension_Dialog();


protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_200;
    bool variesWasSet_201;
    bool variesWasSet_202;
    bool variesWasSet_205;
    bool variesWasSet_212;
    bool variesWasSet_213;
    bool variesWasSet_218;
    bool variesWasSet_222;

    int changecount;
    bool change;
    BitMask * changeBits;

signals:

public slots:
    void drawSample();
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(double v);

    void on__200_valueChanged(double v);
    void on__201_valueChanged(double v);
    void on__202_valueChanged(int v);

    void on__205_valueChanged(double v);
    void on__212_valueChanged(double v);
    void on__213_valueChanged(double v);

    void on__218_valueChanged(double v);
    void on__222_valueChanged(double v);

    void on__203_clicked(bool checked);
    void on__204_clicked(bool checked);

    void on__206_clicked(bool checked);
    void on__207_clicked(bool checked);
    void on__208_clicked(bool checked);
    void on__209_clicked(bool checked);
    void on__210_clicked(bool checked);
    void on__211_currentIndexChanged(int index);
    void on__215_clicked(bool checked);

    void on__216_clicked(bool checked);
    void on__217_clicked(bool checked);
    void on__219_clicked(bool checked);
    void on__220_clicked(bool checked);
    void on__224_clicked(bool checked);
    void on__225_clicked(bool checked);
    void on__226_clicked(bool checked);

    void on__221_clicked(bool checked); // clone
    void on__223_clicked(bool checked); // clone all

    void on__228_clicked(bool checked); // modify length
    void on__229_clicked(bool checked); // modify equation
    void on__230_clicked(bool checked); // modify value

    void on__231_clicked(bool checked); // add point button
    void on__232_clicked(bool checked); // delete point button
    void on__233_clicked(bool checked); // move point button

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);

    void on_accept();

private:
    Ui::Dimension_Dialog *ui;
};

#endif // DIMENSION_DIALOG_H
