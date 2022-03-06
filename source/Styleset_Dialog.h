#ifndef STYLESET_DIALOG_H
#define STYLESET_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Styleset_Dialog;
}

//class Styleset_Dialog : public QDialog, public RCDialog
class Styleset_Dialog : public RCDialog
{
    Q_OBJECT
public:
	explicit Styleset_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f = 0);
	Q_INVOKABLE Styleset_Dialog(HANDLE parent, HANDLE rcparent , int f);
    ~Styleset_Dialog();

    void draw_sample();
    void updateAll();

signals:
    
public slots:
    void on__100_currentTextChanged(const QString & text);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(int v);
    void on__104_valueChanged(double v);
    void on__105_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on__110_clicked(bool checked);
    void on__111_clicked(bool checked);
    void on__112_clicked(bool checked);
    void on_accept();

private:
    Ui::Styleset_Dialog *ui;
};


#endif // STYLESET_DIALOG_H
