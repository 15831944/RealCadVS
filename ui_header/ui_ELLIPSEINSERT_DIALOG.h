/********************************************************************************
** Form generated from reading UI file 'ELLIPSEINSERT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ELLIPSEINSERT_DIALOG_H
#define UI_ELLIPSEINSERT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_EllipseInsert_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_3;
    QLabel *label_4;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_101;

    void setupUi(QDialog *EllipseInsert_Dialog)
    {
        if (EllipseInsert_Dialog->objectName().isEmpty())
            EllipseInsert_Dialog->setObjectName(QString::fromUtf8("EllipseInsert_Dialog"));
        EllipseInsert_Dialog->resize(260, 103);
        QFont font;
        font.setPointSize(10);
        EllipseInsert_Dialog->setFont(font);
        _2 = new QPushButton(EllipseInsert_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(170, 70, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(EllipseInsert_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(80, 70, 75, 23));
        label_3 = new QLabel(EllipseInsert_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(24, 10, 107, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        label_3->setFont(font1);
        label_4 = new QLabel(EllipseInsert_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(24, 40, 105, 21));
        label_4->setFont(font1);
        _100 = new RCDoubleSpinBox(EllipseInsert_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(132, 10, 113, 22));
        _100->setMinimum(-100000000.000000000000000);
        _100->setMaximum(10000.000000000000000);
        _101 = new RCDoubleSpinBox(EllipseInsert_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(132, 38, 113, 22));
        _101->setMinimum(-10000.000000000000000);
        _101->setMaximum(10000.000000000000000);

        retranslateUi(EllipseInsert_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(EllipseInsert_Dialog);
    } // setupUi

    void retranslateUi(QDialog *EllipseInsert_Dialog)
    {
        EllipseInsert_Dialog->setWindowTitle(QCoreApplication::translate("EllipseInsert_Dialog", "Insert ellipse options", nullptr));
        _2->setText(QCoreApplication::translate("EllipseInsert_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("EllipseInsert_Dialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("EllipseInsert_Dialog", "Major axis length", nullptr));
        label_4->setText(QCoreApplication::translate("EllipseInsert_Dialog", "Minor axis length", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EllipseInsert_Dialog: public Ui_EllipseInsert_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ELLIPSEINSERT_DIALOG_H
