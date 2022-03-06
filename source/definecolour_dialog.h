#ifndef DEFINECOLOUR_DIALOG_H
#define DEFINECOLOUR_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

class MousePressFilter : public QObject
{
    Q_OBJECT
public:
    explicit MousePressFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class DefineColour_Dialog;
}

class DefineColour_Dialog : public RCDialog
{
    Q_OBJECT
public:
    friend class MousePressFilter;

    explicit DefineColour_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DefineColour_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DefineColour_Dialog();

    void on_slider_actionTriggered(int action, QObject *sender);
    void initHlsTab();
    void initGrayTab();
    void initPaletteTab();

signals:
    
public slots:
    void on__1_clicked(bool checked);
    void on__2_clicked(bool checked);
    // current colour spin box
    void on__100_valueChanged(int v);
    // rgb spin boxes
    void on__101_valueChanged(double v);
    void on__102_valueChanged(double v);
    void on__103_valueChanged(double v);
    // rgb sliders
    void on__104_actionTriggered(int action);
    void on__105_actionTriggered(int action);
    void on__106_actionTriggered(int action);
    // hls spin boxes
    void on__107_valueChanged(double v);
    void on__108_valueChanged(double v);
    void on__109_valueChanged(double v);
    // hls sliders
    void on__110_actionTriggered(int action);
    void on__111_actionTriggered(int action);
    void on__112_actionTriggered(int action);
    // set button
    void on__114_clicked(bool checked);   
    // tab changed
    void on__120_currentChanged(int index);

private:
    Ui::DefineColour_Dialog *ui;

    QImage *hlsColourImage;
    QImage *grayScaleImage;
    QImage *curPaletteImage;
};

#endif // DEFINECOLOUR_DIALOG_H
