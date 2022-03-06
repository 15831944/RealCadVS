/********************************************************************************
** Form generated from reading UI file 'SELECTAPPLICATION_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTAPPLICATION_DIALOG_H
#define UI_SELECTAPPLICATION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectApplication_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *SelectApplication_Dialog)
    {
        if (SelectApplication_Dialog->objectName().isEmpty())
            SelectApplication_Dialog->setObjectName(QString::fromUtf8("SelectApplication_Dialog"));
        SelectApplication_Dialog->resize(289, 218);
        SelectApplication_Dialog->setMinimumSize(QSize(289, 218));
        SelectApplication_Dialog->setMaximumSize(QSize(289, 218));
        QFont font;
        font.setPointSize(10);
        SelectApplication_Dialog->setFont(font);
        label = new QLabel(SelectApplication_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(24, 10, 251, 20));
        label->setFont(font);
        _2 = new QPushButton(SelectApplication_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(194, 182, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(SelectApplication_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(104, 182, 75, 23));
        _100 = new RCComboList(SelectApplication_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(16, 34, 257, 144));

        retranslateUi(SelectApplication_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SelectApplication_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectApplication_Dialog)
    {
        SelectApplication_Dialog->setWindowTitle(QCoreApplication::translate("SelectApplication_Dialog", "Select the application to load", nullptr));
        label->setText(QCoreApplication::translate("SelectApplication_Dialog", "Select the application to load", nullptr));
        _2->setText(QCoreApplication::translate("SelectApplication_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SelectApplication_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectApplication_Dialog: public Ui_SelectApplication_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTAPPLICATION_DIALOG_H
