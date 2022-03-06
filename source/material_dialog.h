#ifndef MATERIAL_DIALOG_H
#define MATERIAL_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

class SelectMaterialDlgventFilter : public QObject
{    Q_OBJECT
public:
    explicit SelectMaterialDlgventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class Material_Dialog;
}

class Material_Dialog : public RCDialog
{
    friend class SelectMaterialDlgventFilter;

    Q_OBJECT
public:
    explicit Material_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Material_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Material_Dialog();

signals:

public slots:
    void accept();
    void draw();
    // picture buttons
    void on_picture_clicked(int id);
    void on__101_clicked();
    void on__124_clicked();
    void on__123_currentIndexChanged(int index);// combo box
    void on__122_actionTriggered(int action);// scroll bar
    void on__102_valueChanged(double v); // xoffset
    void on__103_valueChanged(double v); // yoffset
    void on__104_valueChanged(double v); // width
    void on__105_valueChanged(double v); // height
    void on__106_valueChanged(double v); // angle
    void on__108_valueChanged(double v); // transparency
    void on__107_clicked(bool checked); // flip the image

private:
    Ui::Material_Dialog *ui;
};

#endif // MATERIAL_DIALOG_H
