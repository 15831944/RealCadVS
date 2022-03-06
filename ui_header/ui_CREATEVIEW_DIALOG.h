/********************************************************************************
** Form generated from reading UI file 'CREATEVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEVIEW_DIALOG_H
#define UI_CREATEVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_CreateView_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *_100;
    RCComboList *_101;

    void setupUi(QDialog *CreateView_Dialog)
    {
        if (CreateView_Dialog->objectName().isEmpty())
            CreateView_Dialog->setObjectName(QString::fromUtf8("CreateView_Dialog"));
        CreateView_Dialog->resize(346, 280);
        CreateView_Dialog->setMinimumSize(QSize(346, 280));
        CreateView_Dialog->setMaximumSize(QSize(346, 280));
        QFont font;
        font.setPointSize(10);
        CreateView_Dialog->setFont(font);
        _2 = new QPushButton(CreateView_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(250, 228, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CreateView_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(160, 228, 75, 23));
        label_4 = new QLabel(CreateView_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 22, 101, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(CreateView_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 107, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(CreateView_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(130, 22, 200, 20));
        _101 = new RCComboList(CreateView_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(130, 50, 200, 169));
        _101->setMinimumSize(QSize(200, 169));
        _101->setMaximumSize(QSize(200, 169));
        _101->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        retranslateUi(CreateView_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CreateView_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CreateView_Dialog)
    {
        CreateView_Dialog->setWindowTitle(QCoreApplication::translate("CreateView_Dialog", "Create view", nullptr));
        _2->setText(QCoreApplication::translate("CreateView_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CreateView_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("CreateView_Dialog", "New view name", nullptr));
        label_5->setText(QCoreApplication::translate("CreateView_Dialog", "Copy from view", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateView_Dialog: public Ui_CreateView_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEVIEW_DIALOG_H
