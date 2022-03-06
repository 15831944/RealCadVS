#ifndef PLANEVERIFY_DIALOG_H
#define PLANEVERIFY_DIALOG_H


#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PlaneVerify_Dialog;
}

class PlaneVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit PlaneVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class PlaneVerify_Dialog : public RCDialog
{
    friend class PlaneVerifyEventFilter;

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

    bool variesWasSet_122;
    bool variesWasSet_123;
    bool variesWasSet_124;
    bool variesWasSet_125;
    bool variesWasSet_126;
    bool variesWasSet_127;
    bool variesWasSet_128;
    bool variesWasSet_129;
    bool variesWasSet_134;

    int changecount;
    bool change;
    BitMask * changeBits;
    bool sliderIsReleased;

public:
    explicit PlaneVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  PlaneVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PlaneVerify_Dialog();

    Plane *rcplane;
    void drawSample(int index=8);

signals:

public slots:
    void on__100_valueChanged(int v); // colour
    void on__101_valueChanged(int v); // style
    void on__102_valueChanged(int v); // layer
    void on__103_valueChanged(double v); // weight

    void on__104_valueChanged(double v); // x spacing
    void on__105_valueChanged(double v); // y spacing
    void on__106_valueChanged(double v); // angle
    void on__107_valueChanged(double v); // cross angle

    void on__1022_clicked(bool checked); // pattern file browse button
    void on__110_clicked(bool checked); // pattern select button
    void on__111_clicked(bool checked); // change surface properties button
    void on__112_clicked(bool checked); // align button
    void on__113_clicked(bool checked); // set anchor button
    void on__114_clicked(bool checked); // no fill radio button
    void on__115_clicked(bool checked); // hatch fill radio button
    void on__116_clicked(bool checked); // pattern fill radio button
    void on__117_clicked(bool checked); // solid fill radio button
    void on__118_clicked(bool checked); // bitmap fill radio button
    void on__119_clicked(bool checked); // gradient fill radio button
    void on__120_clicked(bool checked); // display edges check box

    void on__121_currentIndexChanged(int index); // gradation style combo box

    //void on__122_valueChanged(double v); // gradation red
    //void on__123_valueChanged(double v); // gradation green
    //void on__124_valueChanged(double v); // gradation blue
    void on__125_valueChanged(double v); // transparency decimal
    void on__126_valueChanged(double v); // transparency range
    //void on__127_valueChanged(double v); // transparency red
    //void on__128_valueChanged(double v); // transparency green
    //void on__129_valueChanged(double v); // transparency blue
    void on__130_valueChanged(double v); // preview height
    void on__134_valueChanged(double v); // transparency percentage

    void on__131_clicked(bool checked); // clone button
    void on__132_clicked(bool checked); // clone all button

    void on__1000_clicked(bool checked); // colour select button
    void on__1001_clicked(bool checked); // layer select button
    void on__1002_clicked(bool checked); // style select button
    void on__1003_clicked(bool checked); // weight select button
    void on__1004_clicked(bool checked); // gradient fill colour select button
    void on__1005_clicked(bool checked); // transparent colour select button
    void on__1007_clicked(bool checked); // transparent colour select button

    void transparencyChanged(QObject *sender, double v);
    void decimalTranspChanged(double v);
    void percentTranspChanged(double v);

    void on_accept();

    void on__134_editingFinished(); // transparency percentage
    void on__303_valueChanged(int value); // preview scale
    void on__304_clicked(); // preview scale reset
    void on__305_clicked(bool checked); // preview display on/off

    void on__303_sliderReleased();

    void on__303_sliderPressed();

    void on__1008_clicked(bool checked);

    void on__104_editingFinished();

    void on__105_editingFinished();

private slots:
    void on__1009_clicked(bool checked);

    void on__1010_clicked(bool checked);

    void on__1011_clicked(bool checked);

private:
    Ui::PlaneVerify_Dialog *ui;
};

#endif // PLANEVERIFY_DIALOG_H
