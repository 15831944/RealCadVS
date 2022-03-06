#ifndef PATCHMOVE_DIALOG_H
#define PATCHMOVE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PatchMove_Dialog;
}

class PatchMove_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PatchMove_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PatchMove_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PatchMove_Dialog();

signals:

public slots:
    // define movement radio buttons
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);

    // Flatten optins radio buttons
    void on__110_clicked(bool checked);
    void on__111_clicked(bool checked);

    // Point selection radio buttons
    void on__113_clicked(bool checked);
    void on__114_clicked(bool checked);

    // Delete selected button
    void on__116_clicked(bool checked);

    // Delete unselected button
    void on__117_clicked(bool checked);

    // Cut out button
    void on__118_clicked(bool checked);

    void on_accept();

private:
    Ui::PatchMove_Dialog *ui;
};

#endif // PATCHMOVE_DIALOG_H
