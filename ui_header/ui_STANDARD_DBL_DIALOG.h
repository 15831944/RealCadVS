/********************************************************************************
** Form generated from reading UI file 'STANDARD_DBL_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STANDARD_DBL_DIALOG_H
#define UI_STANDARD_DBL_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Standard_Dbl_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *_100;
    RCDoubleSpinBox *_101;

    void setupUi(QDialog *Standard_Dbl_Dialog)
    {
        if (Standard_Dbl_Dialog->objectName().isEmpty())
            Standard_Dbl_Dialog->setObjectName(QString::fromUtf8("Standard_Dbl_Dialog"));
        Standard_Dbl_Dialog->resize(400, 90);
        Standard_Dbl_Dialog->setMinimumSize(QSize(400, 90));
        Standard_Dbl_Dialog->setMaximumSize(QSize(400, 90));
        QFont font;
        font.setPointSize(10);
        Standard_Dbl_Dialog->setFont(font);
        _2 = new QPushButton(Standard_Dbl_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(304, 40, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Standard_Dbl_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(214, 40, 75, 23));
        _1->setFont(font);
        _100 = new QLabel(Standard_Dbl_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 10, 201, 20));
        _100->setFont(font);
        _101 = new RCDoubleSpinBox(Standard_Dbl_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(218, 8, 162, 22));
        _101->setMinimum(-1000000000.000000000000000);
        _101->setMaximum(1000000000.000000000000000);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Standard_Dbl_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Standard_Dbl_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Standard_Dbl_Dialog)
    {
        Standard_Dbl_Dialog->setWindowTitle(QCoreApplication::translate("Standard_Dbl_Dialog", "TITLE", nullptr));
        _2->setText(QCoreApplication::translate("Standard_Dbl_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Standard_Dbl_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("Standard_Dbl_Dialog", "Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Standard_Dbl_Dialog: public Ui_Standard_Dbl_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STANDARD_DBL_DIALOG_H
