/********************************************************************************
** Form generated from reading UI file 'CURVE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVE_DIALOG_H
#define UI_CURVE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Curve_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QCheckBox *_101;
    QSpinBox *_100;

    void setupUi(QDialog *Curve_Dialog)
    {
        if (Curve_Dialog->objectName().isEmpty())
            Curve_Dialog->setObjectName(QString::fromUtf8("Curve_Dialog"));
        Curve_Dialog->resize(307, 122);
        QFont font;
        font.setPointSize(10);
        Curve_Dialog->setFont(font);
        _2 = new QPushButton(Curve_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(220, 90, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Curve_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(130, 90, 75, 23));
        label_4 = new QLabel(Curve_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 131, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QCheckBox(Curve_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(126, 54, 81, 20));
        _101->setLayoutDirection(Qt::RightToLeft);
        _100 = new QSpinBox(Curve_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(152, 20, 143, 22));

        retranslateUi(Curve_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Curve_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Curve_Dialog)
    {
        Curve_Dialog->setWindowTitle(QCoreApplication::translate("Curve_Dialog", "Curve options", nullptr));
        _2->setText(QCoreApplication::translate("Curve_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Curve_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("Curve_Dialog", "Curve degree", nullptr));
        _101->setText(QCoreApplication::translate("Curve_Dialog", "Closed", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Curve_Dialog: public Ui_Curve_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVE_DIALOG_H
