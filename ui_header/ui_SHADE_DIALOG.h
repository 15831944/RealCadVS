/********************************************************************************
** Form generated from reading UI file 'SHADE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHADE_DIALOG_H
#define UI_SHADE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Shade_Dialog
{
public:
    QLabel *label_9;
    QLabel *label_8;
    QPushButton *_1;
    QPushButton *_2;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_101;

    void setupUi(QDialog *Shade_Dialog)
    {
        if (Shade_Dialog->objectName().isEmpty())
            Shade_Dialog->setObjectName(QString::fromUtf8("Shade_Dialog"));
        Shade_Dialog->resize(261, 125);
        Shade_Dialog->setMinimumSize(QSize(261, 125));
        Shade_Dialog->setMaximumSize(QSize(261, 125));
        QFont font;
        font.setPointSize(10);
        Shade_Dialog->setFont(font);
        label_9 = new QLabel(Shade_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(22, 37, 93, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        label_9->setFont(font1);
        label_8 = new QLabel(Shade_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(22, 13, 95, 20));
        label_8->setFont(font1);
        _1 = new QPushButton(Shade_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(52, 82, 75, 23));
        _1->setFont(font1);
        _2 = new QPushButton(Shade_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(142, 82, 75, 23));
        _2->setFont(font1);
        _100 = new RCDoubleSpinBox(Shade_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(138, 10, 99, 24));
        _100->setFont(font1);
        _100->setMaximum(100.000000000000000);
        _101 = new RCDoubleSpinBox(Shade_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(138, 36, 99, 24));
        _101->setFont(font1);
        _101->setMaximum(100.000000000000000);

        retranslateUi(Shade_Dialog);

        QMetaObject::connectSlotsByName(Shade_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Shade_Dialog)
    {
        Shade_Dialog->setWindowTitle(QCoreApplication::translate("Shade_Dialog", "Shade options", nullptr));
        label_9->setText(QCoreApplication::translate("Shade_Dialog", "Diffuse intensity", nullptr));
        label_8->setText(QCoreApplication::translate("Shade_Dialog", "Ambient intensity", nullptr));
        _1->setText(QCoreApplication::translate("Shade_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Shade_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Shade_Dialog: public Ui_Shade_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHADE_DIALOG_H
