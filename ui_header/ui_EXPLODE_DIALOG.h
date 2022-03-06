/********************************************************************************
** Form generated from reading UI file 'EXPLODE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPLODE_DIALOG_H
#define UI_EXPLODE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Explode_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QCheckBox *_101;
    QCheckBox *_100;

    void setupUi(QDialog *Explode_Dialog)
    {
        if (Explode_Dialog->objectName().isEmpty())
            Explode_Dialog->setObjectName(QString::fromUtf8("Explode_Dialog"));
        Explode_Dialog->resize(231, 93);
        QFont font;
        font.setPointSize(10);
        Explode_Dialog->setFont(font);
        _2 = new QPushButton(Explode_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(120, 62, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Explode_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(36, 62, 75, 23));
        _101 = new QCheckBox(Explode_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(0, 32, 207, 21));
        _101->setLayoutDirection(Qt::RightToLeft);
        _101->setCheckable(true);
        _101->setChecked(false);
        _101->setTristate(false);
        _100 = new QCheckBox(Explode_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(50, 10, 157, 21));
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);

        retranslateUi(Explode_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Explode_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Explode_Dialog)
    {
        Explode_Dialog->setWindowTitle(QCoreApplication::translate("Explode_Dialog", "Explode entity", nullptr));
        _2->setText(QCoreApplication::translate("Explode_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Explode_Dialog", "OK", nullptr));
        _101->setText(QCoreApplication::translate("Explode_Dialog", "Group exploded components", nullptr));
        _100->setText(QCoreApplication::translate("Explode_Dialog", "Delete existing entity", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Explode_Dialog: public Ui_Explode_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPLODE_DIALOG_H
