/********************************************************************************
** Form generated from reading UI file 'SELECTVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTVIEW_DIALOG_H
#define UI_SELECTVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectView_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;
    RCComboList *_101;

    void setupUi(QDialog *SelectView_Dialog)
    {
        if (SelectView_Dialog->objectName().isEmpty())
            SelectView_Dialog->setObjectName(QString::fromUtf8("SelectView_Dialog"));
        SelectView_Dialog->resize(497, 260);
        SelectView_Dialog->setMinimumSize(QSize(497, 260));
        SelectView_Dialog->setMaximumSize(QSize(497, 260));
        QFont font;
        font.setPointSize(10);
        SelectView_Dialog->setFont(font);
        label = new QLabel(SelectView_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 209, 20));
        label->setFont(font);
        label_2 = new QLabel(SelectView_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 10, 223, 20));
        label_2->setFont(font);
        _2 = new QPushButton(SelectView_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(396, 206, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(SelectView_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(306, 206, 75, 23));
        _100 = new RCComboList(SelectView_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 32, 209, 169));
        _100->setMinimumSize(QSize(209, 169));
        _100->setMaximumSize(QSize(209, 169));
        _100->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _101 = new RCComboList(SelectView_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(250, 32, 229, 169));
        _101->setMinimumSize(QSize(229, 169));
        _101->setMaximumSize(QSize(229, 169));

        retranslateUi(SelectView_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SelectView_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectView_Dialog)
    {
        SelectView_Dialog->setWindowTitle(QCoreApplication::translate("SelectView_Dialog", "Select the 3D view and zone to display", nullptr));
        label->setText(QCoreApplication::translate("SelectView_Dialog", "Select the 3D view to display", nullptr));
        label_2->setText(QCoreApplication::translate("SelectView_Dialog", "Select the zone to display", nullptr));
        _2->setText(QCoreApplication::translate("SelectView_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SelectView_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectView_Dialog: public Ui_SelectView_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTVIEW_DIALOG_H
