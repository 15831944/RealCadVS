/********************************************************************************
** Form generated from reading UI file 'UNLICENSE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNLICENSE_DIALOG_H
#define UI_UNLICENSE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Unlicense_Dialog
{
public:
    QCheckBox *_100;
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *Unlicense_Dialog)
    {
        if (Unlicense_Dialog->objectName().isEmpty())
            Unlicense_Dialog->setObjectName(QString::fromUtf8("Unlicense_Dialog"));
        Unlicense_Dialog->resize(501, 149);
        Unlicense_Dialog->setMinimumSize(QSize(501, 149));
        Unlicense_Dialog->setMaximumSize(QSize(501, 149));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        Unlicense_Dialog->setFont(font);
        _100 = new QCheckBox(Unlicense_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(60, 80, 311, 20));
        QFont font1;
        font1.setPointSize(10);
        _100->setFont(font1);
        _100->setLayoutDirection(Qt::RightToLeft);
        _2 = new QPushButton(Unlicense_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(411, 112, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Unlicense_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(321, 112, 75, 23));
        _1->setFont(font1);
        label = new QLabel(Unlicense_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 481, 20));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(Unlicense_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 481, 20));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Unlicense_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 50, 481, 20));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignCenter);

        retranslateUi(Unlicense_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Unlicense_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Unlicense_Dialog)
    {
        Unlicense_Dialog->setWindowTitle(QCoreApplication::translate("Unlicense_Dialog", "Unlicense Software", nullptr));
        _100->setText(QCoreApplication::translate("Unlicense_Dialog", "Yes, I want to unlicense this software now", nullptr));
        _2->setText(QCoreApplication::translate("Unlicense_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Unlicense_Dialog", "OK", nullptr));
        label->setText(QCoreApplication::translate("Unlicense_Dialog", "This will remove the license information from this computer.", nullptr));
        label_2->setText(QCoreApplication::translate("Unlicense_Dialog", "You will need to obtain a new license key to continue to use this software.", nullptr));
        label_3->setText(QCoreApplication::translate("Unlicense_Dialog", "The old license information will be automatically emailed to support@cad.com.au", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Unlicense_Dialog: public Ui_Unlicense_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNLICENSE_DIALOG_H
