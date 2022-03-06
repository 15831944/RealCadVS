/********************************************************************************
** Form generated from reading UI file 'DELETEVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEVIEW_DIALOG_H
#define UI_DELETEVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_DeleteView_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_101;
    RCComboList *_100;

    void setupUi(QDialog *DeleteView_Dialog)
    {
        if (DeleteView_Dialog->objectName().isEmpty())
            DeleteView_Dialog->setObjectName(QString::fromUtf8("DeleteView_Dialog"));
        DeleteView_Dialog->resize(246, 255);
        DeleteView_Dialog->setMinimumSize(QSize(246, 255));
        DeleteView_Dialog->setMaximumSize(QSize(246, 255));
        QFont font;
        font.setPointSize(10);
        DeleteView_Dialog->setFont(font);
        label = new QLabel(DeleteView_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 331, 20));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DeleteView_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(148, 206, 75, 23));
        _2->setAutoDefault(false);
        _101 = new QPushButton(DeleteView_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(66, 206, 75, 23));
        _100 = new RCComboList(DeleteView_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(18, 34, 209, 169));
        _100->setMinimumSize(QSize(209, 169));
        _100->setMaximumSize(QSize(209, 169));
        _100->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _100->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _100->setSelectionBehavior(QAbstractItemView::SelectRows);

        retranslateUi(DeleteView_Dialog);

        _101->setDefault(true);


        QMetaObject::connectSlotsByName(DeleteView_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DeleteView_Dialog)
    {
        DeleteView_Dialog->setWindowTitle(QCoreApplication::translate("DeleteView_Dialog", "Delete view", nullptr));
        label->setText(QCoreApplication::translate("DeleteView_Dialog", "Select the view to delete:", nullptr));
        _2->setText(QCoreApplication::translate("DeleteView_Dialog", "Cancel", nullptr));
        _101->setText(QCoreApplication::translate("DeleteView_Dialog", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteView_Dialog: public Ui_DeleteView_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEVIEW_DIALOG_H
