/********************************************************************************
** Form generated from reading UI file 'ZONE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZONE_DIALOG_H
#define UI_ZONE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_Zone_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *Zone_Dialog)
    {
        if (Zone_Dialog->objectName().isEmpty())
            Zone_Dialog->setObjectName(QString::fromUtf8("Zone_Dialog"));
        Zone_Dialog->resize(241, 237);
        Zone_Dialog->setMinimumSize(QSize(241, 237));
        Zone_Dialog->setMaximumSize(QSize(241, 237));
        label = new QLabel(Zone_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 221, 21));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        _2 = new QPushButton(Zone_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(152, 204, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Zone_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(62, 204, 75, 23));
        _1->setFont(font);
        _100 = new RCComboList(Zone_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(16, 32, 209, 169));
        _100->setMinimumSize(QSize(209, 169));
        _100->setMaximumSize(QSize(209, 169));
        _100->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

        retranslateUi(Zone_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Zone_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Zone_Dialog)
    {
        Zone_Dialog->setWindowTitle(QCoreApplication::translate("Zone_Dialog", "Display view zone", nullptr));
        label->setText(QCoreApplication::translate("Zone_Dialog", "Select the zone to display", nullptr));
        _2->setText(QCoreApplication::translate("Zone_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Zone_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Zone_Dialog: public Ui_Zone_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZONE_DIALOG_H
