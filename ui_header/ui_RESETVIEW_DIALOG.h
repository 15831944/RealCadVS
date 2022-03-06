/********************************************************************************
** Form generated from reading UI file 'RESETVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESETVIEW_DIALOG_H
#define UI_RESETVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_ResetView_Dialog
{
public:
    QLabel *label_1;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *ResetView_Dialog)
    {
        if (ResetView_Dialog->objectName().isEmpty())
            ResetView_Dialog->setObjectName(QString::fromUtf8("ResetView_Dialog"));
        ResetView_Dialog->resize(247, 255);
        ResetView_Dialog->setMinimumSize(QSize(247, 255));
        ResetView_Dialog->setMaximumSize(QSize(247, 255));
        QFont font;
        font.setPointSize(10);
        ResetView_Dialog->setFont(font);
        label_1 = new QLabel(ResetView_Dialog);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(24, 10, 251, 20));
        label_1->setFont(font);
        _2 = new QPushButton(ResetView_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(150, 204, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(ResetView_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(68, 204, 75, 23));
        _100 = new RCComboList(ResetView_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(18, 32, 209, 169));
        _100->setMinimumSize(QSize(209, 169));
        _100->setMaximumSize(QSize(209, 169));
        _100->setUniformItemSizes(true);

        retranslateUi(ResetView_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(ResetView_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ResetView_Dialog)
    {
        ResetView_Dialog->setWindowTitle(QCoreApplication::translate("ResetView_Dialog", "Reset to standard view", nullptr));
        label_1->setText(QCoreApplication::translate("ResetView_Dialog", "Select the standard view to use", nullptr));
        _2->setText(QCoreApplication::translate("ResetView_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("ResetView_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResetView_Dialog: public Ui_ResetView_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESETVIEW_DIALOG_H
