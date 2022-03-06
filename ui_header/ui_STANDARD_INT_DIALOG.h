/********************************************************************************
** Form generated from reading UI file 'STANDARD_INT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STANDARD_INT_DIALOG_H
#define UI_STANDARD_INT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Standard_Int_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *_100;
    RCIntegerSpinBox *_101;

    void setupUi(QDialog *Standard_Int_Dialog)
    {
        if (Standard_Int_Dialog->objectName().isEmpty())
            Standard_Int_Dialog->setObjectName(QString::fromUtf8("Standard_Int_Dialog"));
        Standard_Int_Dialog->resize(400, 90);
        Standard_Int_Dialog->setMinimumSize(QSize(400, 90));
        Standard_Int_Dialog->setMaximumSize(QSize(400, 80));
        QFont font;
        font.setPointSize(10);
        Standard_Int_Dialog->setFont(font);
        _2 = new QPushButton(Standard_Int_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(304, 40, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Standard_Int_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(214, 40, 75, 23));
        _1->setFont(font);
        _100 = new QLabel(Standard_Int_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(28, 10, 179, 20));
        _100->setFont(font);
        _100->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new RCIntegerSpinBox(Standard_Int_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(214, 8, 165, 22));
        _101->setMaximum(1000000000);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Standard_Int_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Standard_Int_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Standard_Int_Dialog)
    {
        Standard_Int_Dialog->setWindowTitle(QCoreApplication::translate("Standard_Int_Dialog", "TITLE", nullptr));
        _2->setText(QCoreApplication::translate("Standard_Int_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Standard_Int_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("Standard_Int_Dialog", "Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Standard_Int_Dialog: public Ui_Standard_Int_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STANDARD_INT_DIALOG_H
