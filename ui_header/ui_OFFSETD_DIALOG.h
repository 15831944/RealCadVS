/********************************************************************************
** Form generated from reading UI file 'OFFSETD_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OFFSETD_DIALOG_H
#define UI_OFFSETD_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Offsetd_Dialog
{
public:
    QLabel *label_13;
    QCheckBox *_101;
    QPushButton *_2;
    QPushButton *_1;
    RCDoubleSpinBox *_100;

    void setupUi(QDialog *Offsetd_Dialog)
    {
        if (Offsetd_Dialog->objectName().isEmpty())
            Offsetd_Dialog->setObjectName(QString::fromUtf8("Offsetd_Dialog"));
        Offsetd_Dialog->resize(269, 120);
        Offsetd_Dialog->setMinimumSize(QSize(269, 120));
        Offsetd_Dialog->setMaximumSize(QSize(269, 120));
        QFont font;
        font.setPointSize(10);
        Offsetd_Dialog->setFont(font);
        label_13 = new QLabel(Offsetd_Dialog);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(30, 13, 141, 20));
        label_13->setFont(font);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QCheckBox(Offsetd_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(34, 41, 191, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _101->setFont(font1);
        _101->setLayoutDirection(Qt::RightToLeft);
        _101->setCheckable(true);
        _101->setChecked(false);
        _101->setTristate(false);
        _2 = new QPushButton(Offsetd_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(148, 73, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Offsetd_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(58, 73, 75, 23));
        _1->setFont(font1);
        _100 = new RCDoubleSpinBox(Offsetd_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(128, 12, 111, 24));
        _100->setFont(font1);
        _100->setMinimum(-100000000.000000000000000);
        _100->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Offsetd_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Offsetd_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Offsetd_Dialog)
    {
        Offsetd_Dialog->setWindowTitle(QCoreApplication::translate("Offsetd_Dialog", "Offset profile by given distance", nullptr));
        label_13->setText(QCoreApplication::translate("Offsetd_Dialog", "Offset distance", nullptr));
        _101->setText(QCoreApplication::translate("Offsetd_Dialog", "Offset using round corners", nullptr));
        _2->setText(QCoreApplication::translate("Offsetd_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Offsetd_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Offsetd_Dialog: public Ui_Offsetd_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OFFSETD_DIALOG_H
