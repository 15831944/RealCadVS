/********************************************************************************
** Form generated from reading UI file 'ZOOMZONE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZOOMZONE_DIALOG_H
#define UI_ZOOMZONE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_ZoomZone_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *ZoomZone_Dialog)
    {
        if (ZoomZone_Dialog->objectName().isEmpty())
            ZoomZone_Dialog->setObjectName(QString::fromUtf8("ZoomZone_Dialog"));
        ZoomZone_Dialog->resize(239, 241);
        ZoomZone_Dialog->setMinimumSize(QSize(239, 241));
        ZoomZone_Dialog->setMaximumSize(QSize(239, 241));
        label = new QLabel(ZoomZone_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 8, 145, 21));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        _2 = new QPushButton(ZoomZone_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(152, 206, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(ZoomZone_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(68, 206, 75, 23));
        _1->setFont(font);
        _100 = new RCComboList(ZoomZone_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(10, 30, 221, 169));
        _100->setMinimumSize(QSize(221, 169));
        _100->setMaximumSize(QSize(221, 169));

        retranslateUi(ZoomZone_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(ZoomZone_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ZoomZone_Dialog)
    {
        ZoomZone_Dialog->setWindowTitle(QCoreApplication::translate("ZoomZone_Dialog", "Zoom to zone", nullptr));
        label->setText(QCoreApplication::translate("ZoomZone_Dialog", "Select the zone to zoom to ", nullptr));
        _2->setText(QCoreApplication::translate("ZoomZone_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("ZoomZone_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ZoomZone_Dialog: public Ui_ZoomZone_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZOOMZONE_DIALOG_H
