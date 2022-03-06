#ifndef SELECTHATCHPATTERN_DIALOG_H
#define SELECTHATCHPATTERN_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SelectHatchPattern_Dialog;
}

class SelectHatchEventFilter : public QObject
{    Q_OBJECT
public:
    explicit SelectHatchEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class SelectHatchPattern_Dialog : public RCDialog
{
    friend class SelectHatchEventFilter;

    Q_OBJECT
public:
    explicit SelectHatchPattern_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  SelectHatchPattern_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectHatchPattern_Dialog();

    void trigger_118(int action);
signals:

public slots:
    void draw();
    // picture buttons
    void on_picture_clicked(int id);

    // control buttons
    void on__112_clicked(bool checked);// previous
    void on__113_clicked(bool checked);// next
    void on__116_clicked(bool checked);// first
    void on__117_clicked(bool checked);// last
    void on__115_clicked(bool checked);// browse

    void on__118_actionTriggered(int action);// scroll bar

    void on__114_currentIndexChanged(int index);// combo box

private:
    Ui::SelectHatchPattern_Dialog *ui;
};

#endif // SELECTHATCHPATTERN_DIALOG_H
