/********************************************************************************
** Form generated from reading UI file 'DELETEZONE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEZONE_DIALOG_H
#define UI_DELETEZONE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_DeleteZone_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *DeleteZone_Dialog)
    {
        if (DeleteZone_Dialog->objectName().isEmpty())
            DeleteZone_Dialog->setObjectName(QString::fromUtf8("DeleteZone_Dialog"));
        DeleteZone_Dialog->resize(247, 235);
        QFont font;
        font.setPointSize(10);
        DeleteZone_Dialog->setFont(font);
        label = new QLabel(DeleteZone_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 241, 20));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DeleteZone_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(150, 202, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(DeleteZone_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(68, 202, 75, 23));
        _100 = new RCComboList(DeleteZone_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 31, 209, 169));
        _100->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

        retranslateUi(DeleteZone_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DeleteZone_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DeleteZone_Dialog)
    {
        DeleteZone_Dialog->setWindowTitle(QCoreApplication::translate("DeleteZone_Dialog", "Delete zone", nullptr));
        label->setText(QCoreApplication::translate("DeleteZone_Dialog", "Select the zone to delete:", nullptr));
        _2->setText(QCoreApplication::translate("DeleteZone_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DeleteZone_Dialog", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteZone_Dialog: public Ui_DeleteZone_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEZONE_DIALOG_H
