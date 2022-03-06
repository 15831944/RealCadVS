#ifndef POLYGON_DIALOG_H
#define POLYGON_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Polygon_Dialog;
}

//class Polygon_Dialog : public QDialog, public RCDialog
class Polygon_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Polygon_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Polygon_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Polygon_Dialog();

signals:
    
public slots:
    void on__1100_actionTriggered(int action);
    void on__1101_actionTriggered(int action);
    void on__1105_actionTriggered(int action);
    void on__1106_actionTriggered(int action);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on_accept();

private:
    Ui::Polygon_Dialog *ui;
};

#endif // POLYGON_DIALOG_H
