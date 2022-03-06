/********************************************************************************
** Form generated from reading UI file 'RESETWORKPLANE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESETWORKPLANE_DIALOG_H
#define UI_RESETWORKPLANE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_ResetWorkplane_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *ResetWorkplane_Dialog)
    {
        if (ResetWorkplane_Dialog->objectName().isEmpty())
            ResetWorkplane_Dialog->setObjectName(QString::fromUtf8("ResetWorkplane_Dialog"));
        ResetWorkplane_Dialog->resize(250, 255);
        ResetWorkplane_Dialog->setMinimumSize(QSize(250, 255));
        ResetWorkplane_Dialog->setMaximumSize(QSize(250, 255));
        QFont font;
        font.setPointSize(10);
        ResetWorkplane_Dialog->setFont(font);
        label = new QLabel(ResetWorkplane_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(18, 8, 223, 20));
        label->setFont(font);
        _2 = new QPushButton(ResetWorkplane_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(150, 204, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(ResetWorkplane_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(68, 204, 75, 23));
        _100 = new RCComboList(ResetWorkplane_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 28, 209, 169));
        _100->setMinimumSize(QSize(209, 169));
        _100->setMaximumSize(QSize(209, 169));

        retranslateUi(ResetWorkplane_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(ResetWorkplane_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ResetWorkplane_Dialog)
    {
        ResetWorkplane_Dialog->setWindowTitle(QCoreApplication::translate("ResetWorkplane_Dialog", "Reset to standard workplane", nullptr));
        label->setText(QCoreApplication::translate("ResetWorkplane_Dialog", "Select the standard workplane to use", nullptr));
        _2->setText(QCoreApplication::translate("ResetWorkplane_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("ResetWorkplane_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResetWorkplane_Dialog: public Ui_ResetWorkplane_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESETWORKPLANE_DIALOG_H
