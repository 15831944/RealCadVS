/********************************************************************************
** Form generated from reading UI file 'CREATEWORKPLANE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEWORKPLANE_DIALOG_H
#define UI_CREATEWORKPLANE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_CreateWorkplane_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *_100;
    RCComboList *_101;

    void setupUi(QDialog *CreateWorkplane_Dialog)
    {
        if (CreateWorkplane_Dialog->objectName().isEmpty())
            CreateWorkplane_Dialog->setObjectName(QString::fromUtf8("CreateWorkplane_Dialog"));
        CreateWorkplane_Dialog->resize(250, 320);
        CreateWorkplane_Dialog->setMinimumSize(QSize(250, 320));
        CreateWorkplane_Dialog->setMaximumSize(QSize(250, 310));
        QFont font;
        font.setPointSize(10);
        CreateWorkplane_Dialog->setFont(font);
        _2 = new QPushButton(CreateWorkplane_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(136, 258, 75, 23));
        _1 = new QPushButton(CreateWorkplane_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(40, 258, 75, 23));
        label_4 = new QLabel(CreateWorkplane_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(25, 10, 131, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(CreateWorkplane_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(25, 60, 131, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(CreateWorkplane_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(26, 30, 200, 20));
        _101 = new RCComboList(CreateWorkplane_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(26, 80, 201, 169));
        _101->setMinimumSize(QSize(201, 169));
        _101->setMaximumSize(QSize(201, 169));
        _101->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(CreateWorkplane_Dialog);

        QMetaObject::connectSlotsByName(CreateWorkplane_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CreateWorkplane_Dialog)
    {
        CreateWorkplane_Dialog->setWindowTitle(QCoreApplication::translate("CreateWorkplane_Dialog", "Create workplane", nullptr));
        _2->setText(QCoreApplication::translate("CreateWorkplane_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CreateWorkplane_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("CreateWorkplane_Dialog", "New workplane name", nullptr));
        label_5->setText(QCoreApplication::translate("CreateWorkplane_Dialog", "Copy from workplane", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateWorkplane_Dialog: public Ui_CreateWorkplane_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEWORKPLANE_DIALOG_H
