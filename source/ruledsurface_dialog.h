#ifndef RULEDSURFACE_DIALOG_H
#define RULEDSURFACE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class RuledSurface_Dialog;
}

class RuledSurfVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit RuledSurfVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class RuledSurface_Dialog : public RCDialog
{
    friend class RuledSurfVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_104;
    bool variesWasSet_105;
    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit RuledSurface_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE RuledSurface_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~RuledSurface_Dialog();

    void drawSample();
signals:

public slots:
    void on__100_valueChanged(int v); // colour
    void on__101_valueChanged(int v); // style
    void on__102_valueChanged(int v); // layer
    void on__103_valueChanged(double v); // weight

    void on__104_valueChanged(int v); // X mesh
    void on__105_valueChanged(int v); // y mesh

    void on__106_currentIndexChanged(int index); // draw mode combo box

    void on__108_clicked(bool checked); // surface properties
    void on__109_clicked(bool checked); // triagulate
    void on__110_clicked(bool checked); // clone
    void on__111_clicked(bool checked); // clone all

    void on__1000_clicked(bool checked); // colour select button
    void on__1001_clicked(bool checked); // layer select button
    void on__1002_clicked(bool checked); // style select button
    void on__1003_clicked(bool checked); // weight select button
    void on_accept();

private:
    Ui::RuledSurface_Dialog *ui;
};

#endif // RULEDSURFACEDIALOG_H
