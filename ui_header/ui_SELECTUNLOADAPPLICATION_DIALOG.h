/********************************************************************************
** Form generated from reading UI file 'SELECTUNLOADAPPLICATION_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTUNLOADAPPLICATION_DIALOG_H
#define UI_SELECTUNLOADAPPLICATION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectUnloadApplication_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *SelectUnloadApplication_Dialog)
    {
        if (SelectUnloadApplication_Dialog->objectName().isEmpty())
            SelectUnloadApplication_Dialog->setObjectName(QString::fromUtf8("SelectUnloadApplication_Dialog"));
        SelectUnloadApplication_Dialog->resize(289, 218);
        SelectUnloadApplication_Dialog->setMinimumSize(QSize(289, 218));
        SelectUnloadApplication_Dialog->setMaximumSize(QSize(289, 218));
        QFont font;
        font.setPointSize(10);
        SelectUnloadApplication_Dialog->setFont(font);
        label = new QLabel(SelectUnloadApplication_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(24, 10, 251, 20));
        label->setFont(font);
        _2 = new QPushButton(SelectUnloadApplication_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(194, 182, 75, 23));
        _1 = new QPushButton(SelectUnloadApplication_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(104, 182, 75, 23));
        _100 = new RCComboList(SelectUnloadApplication_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(16, 34, 257, 144));

        retranslateUi(SelectUnloadApplication_Dialog);

        QMetaObject::connectSlotsByName(SelectUnloadApplication_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectUnloadApplication_Dialog)
    {
        SelectUnloadApplication_Dialog->setWindowTitle(QCoreApplication::translate("SelectUnloadApplication_Dialog", "Select the application to unload", nullptr));
        label->setText(QCoreApplication::translate("SelectUnloadApplication_Dialog", "Select the application to unload", nullptr));
        _2->setText(QCoreApplication::translate("SelectUnloadApplication_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SelectUnloadApplication_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectUnloadApplication_Dialog: public Ui_SelectUnloadApplication_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTUNLOADAPPLICATION_DIALOG_H
