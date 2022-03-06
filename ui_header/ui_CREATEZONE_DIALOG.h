/********************************************************************************
** Form generated from reading UI file 'CREATEZONE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEZONE_DIALOG_H
#define UI_CREATEZONE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CreateZone_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *_100;
    QListWidget *_101;

    void setupUi(QDialog *CreateZone_Dialog)
    {
        if (CreateZone_Dialog->objectName().isEmpty())
            CreateZone_Dialog->setObjectName(QString::fromUtf8("CreateZone_Dialog"));
        CreateZone_Dialog->resize(374, 280);
        CreateZone_Dialog->setMinimumSize(QSize(374, 280));
        CreateZone_Dialog->setMaximumSize(QSize(374, 280));
        QFont font;
        font.setPointSize(10);
        CreateZone_Dialog->setFont(font);
        _2 = new QPushButton(CreateZone_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(276, 232, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CreateZone_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(188, 232, 75, 23));
        label_4 = new QLabel(CreateZone_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 24, 131, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(CreateZone_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 131, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(CreateZone_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(148, 24, 209, 20));
        _101 = new QListWidget(CreateZone_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(148, 55, 209, 169));
        _101->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        _101->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        retranslateUi(CreateZone_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CreateZone_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CreateZone_Dialog)
    {
        CreateZone_Dialog->setWindowTitle(QCoreApplication::translate("CreateZone_Dialog", "Create zone", nullptr));
        _2->setText(QCoreApplication::translate("CreateZone_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CreateZone_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("CreateZone_Dialog", "New zone name", nullptr));
        label_5->setText(QCoreApplication::translate("CreateZone_Dialog", "Existing zone names", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateZone_Dialog: public Ui_CreateZone_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEZONE_DIALOG_H
