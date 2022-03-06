/********************************************************************************
** Form generated from reading UI file 'DELETEWORKPLANE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEWORKPLANE_DIALOG_H
#define UI_DELETEWORKPLANE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_DeleteWorkplane_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    RCComboList *_100;

    void setupUi(QDialog *DeleteWorkplane_Dialog)
    {
        if (DeleteWorkplane_Dialog->objectName().isEmpty())
            DeleteWorkplane_Dialog->setObjectName(QString::fromUtf8("DeleteWorkplane_Dialog"));
        DeleteWorkplane_Dialog->resize(250, 255);
        DeleteWorkplane_Dialog->setMinimumSize(QSize(250, 255));
        DeleteWorkplane_Dialog->setMaximumSize(QSize(250, 255));
        QFont font;
        font.setPointSize(10);
        DeleteWorkplane_Dialog->setFont(font);
        label = new QLabel(DeleteWorkplane_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(25, 8, 331, 20));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DeleteWorkplane_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(150, 204, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(DeleteWorkplane_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(66, 204, 75, 23));
        _1->setMinimumSize(QSize(75, 23));
        _1->setMaximumSize(QSize(75, 23));
        _100 = new RCComboList(DeleteWorkplane_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 28, 209, 169));
        _100->setMinimumSize(QSize(201, 169));
        _100->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        retranslateUi(DeleteWorkplane_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DeleteWorkplane_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DeleteWorkplane_Dialog)
    {
        DeleteWorkplane_Dialog->setWindowTitle(QCoreApplication::translate("DeleteWorkplane_Dialog", "Delete workplane", nullptr));
        label->setText(QCoreApplication::translate("DeleteWorkplane_Dialog", "Select the workplane to delete:", nullptr));
        _2->setText(QCoreApplication::translate("DeleteWorkplane_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DeleteWorkplane_Dialog", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteWorkplane_Dialog: public Ui_DeleteWorkplane_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEWORKPLANE_DIALOG_H
