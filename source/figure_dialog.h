#ifndef FIGURE_DIALOG_H
#define FIGURE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Figure_Dialog;
}

class FigureVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit FigureVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Figure_Dialog : public RCDialog
{
    friend class FigureVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_104;
    bool variesWasSet_105;
    bool variesWasSet_106;
    bool variesWasSet_107;
    bool variesWasSet_108;
    bool variesWasSet_109;
    bool variesWasSet_116;
    bool variesWasSet_117;
    bool variesWasSet_118;
    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit Figure_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Figure_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Figure_Dialog();

    QString library;
    Point librarySize;
    bool initialised;

signals:

public slots:
    void drawSample();
    void on__100_valueChanged(int v); // colour
    void on__101_valueChanged(int v); // style
    void on__102_valueChanged(int v); // layer
    void on__103_valueChanged(double v); // weight

    void on__111_clicked(bool checked); // original layer
    void on__112_clicked(bool checked); // original colour
    void on__113_clicked(bool checked); // original style
    void on__114_clicked(bool checked); // original weight
    void on__115_clicked(bool checked); // orient to view
    void on__124_clicked(bool checked); // auto attach/detach

    void on__1022_clicked(bool checked); // def path
    void on__125_clicked(bool checked); // attach all figures
    void on__126_clicked(bool checked); // detach all figures
    void on__127_clicked(bool checked); // missing
    void on__128_clicked(bool checked); // modify parametric figure
    void on__129_clicked(bool checked); // clone
    void on__130_clicked(bool checked); // clone all

    void on__1000_clicked(bool checked);// colour select button
    void on__1001_clicked(bool checked);// layer select button
    void on__1002_clicked(bool checked);// style select button
    void on__1003_clicked(bool checked);// weight select button
    void on_accept();

private slots:
    void on__116_valueChanged(double arg1);

    void on__117_valueChanged(double arg1);

    void on__118_valueChanged(double arg1);

    void on__104_valueChanged(double arg1);

    void on__105_valueChanged(double arg1);

    void on__106_valueChanged(double arg1);

    void on__131_clicked(bool checked);

    void on__132_clicked(bool checked);

    void on__116_editingFinished();

    void on__117_editingFinished();

    void on__104_editingFinished();

    void on__105_editingFinished();

    void on__107_valueChanged(double arg1);

    void on__108_valueChanged(double arg1);

    void on__109_valueChanged(double arg1);

private:
    Ui::Figure_Dialog *ui;
};

#endif // FIGURE_DIALOG_H
