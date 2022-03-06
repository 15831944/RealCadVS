/********************************************************************************
** Form generated from reading UI file 'LINE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINE_DIALOG_H
#define UI_LINE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Line_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_9;
    RCDoubleSpinBox *_100;

    void setupUi(QDialog *Line_Dialog)
    {
        if (Line_Dialog->objectName().isEmpty())
            Line_Dialog->setObjectName(QString::fromUtf8("Line_Dialog"));
        Line_Dialog->resize(289, 72);
        QFont font;
        font.setPointSize(10);
        Line_Dialog->setFont(font);
        _2 = new QPushButton(Line_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(194, 40, 75, 26));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Line_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(112, 40, 75, 28));
        label_9 = new QLabel(Line_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 139, 21));
        _100 = new RCDoubleSpinBox(Line_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(168, 9, 100, 24));
        _100->setAccelerated(true);
        _100->setMinimum(-360.000000000000000);
        _100->setMaximum(360.000000000000000);

        retranslateUi(Line_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Line_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Line_Dialog)
    {
        Line_Dialog->setWindowTitle(QCoreApplication::translate("Line_Dialog", "Line options", nullptr));
        _2->setText(QCoreApplication::translate("Line_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Line_Dialog", "OK", nullptr));
        label_9->setText(QCoreApplication::translate("Line_Dialog", "Auto snap angle", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Line_Dialog: public Ui_Line_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINE_DIALOG_H
