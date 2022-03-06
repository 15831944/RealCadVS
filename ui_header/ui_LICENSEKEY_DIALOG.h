/********************************************************************************
** Form generated from reading UI file 'LICENSEKEY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENSEKEY_DIALOG_H
#define UI_LICENSEKEY_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LicenseKey_Dialog
{
public:
    QLabel *label_4;
    QPushButton *_100;
    QPushButton *_101;
    QPushButton *_102;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;

    void setupUi(QDialog *LicenseKey_Dialog)
    {
        if (LicenseKey_Dialog->objectName().isEmpty())
            LicenseKey_Dialog->setObjectName(QString::fromUtf8("LicenseKey_Dialog"));
        LicenseKey_Dialog->resize(479, 258);
        LicenseKey_Dialog->setMinimumSize(QSize(479, 258));
        LicenseKey_Dialog->setMaximumSize(QSize(479, 258));
        QFont font;
        font.setPointSize(10);
        LicenseKey_Dialog->setFont(font);
        label_4 = new QLabel(LicenseKey_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 10, 431, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);
        _100 = new QPushButton(LicenseKey_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(80, 50, 101, 31));
        _101 = new QPushButton(LicenseKey_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(80, 100, 101, 31));
        _101->setAutoDefault(false);
        _102 = new QPushButton(LicenseKey_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(80, 150, 101, 31));
        _102->setAutoDefault(false);
        label_3 = new QLabel(LicenseKey_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 60, 46, 21));
        label_5 = new QLabel(LicenseKey_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 110, 46, 21));
        label_6 = new QLabel(LicenseKey_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 160, 46, 21));
        label_7 = new QLabel(LicenseKey_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(200, 106, 281, 21));
        label_8 = new QLabel(LicenseKey_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(200, 56, 281, 21));
        label_9 = new QLabel(LicenseKey_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(200, 156, 281, 21));
        label_10 = new QLabel(LicenseKey_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(100, 190, 301, 21));
        label_11 = new QLabel(LicenseKey_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(100, 210, 291, 21));

        retranslateUi(LicenseKey_Dialog);

        _100->setDefault(true);


        QMetaObject::connectSlotsByName(LicenseKey_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LicenseKey_Dialog)
    {
        LicenseKey_Dialog->setWindowTitle(QCoreApplication::translate("LicenseKey_Dialog", "Software license", nullptr));
        label_4->setText(QCoreApplication::translate("LicenseKey_Dialog", "This software does not have a valid license key", nullptr));
        _100->setText(QCoreApplication::translate("LicenseKey_Dialog", "Buy Now", nullptr));
        _101->setText(QCoreApplication::translate("LicenseKey_Dialog", "Set License Key", nullptr));
        _102->setText(QCoreApplication::translate("LicenseKey_Dialog", "Trial Mode", nullptr));
        label_3->setText(QCoreApplication::translate("LicenseKey_Dialog", "Click", nullptr));
        label_5->setText(QCoreApplication::translate("LicenseKey_Dialog", "Click", nullptr));
        label_6->setText(QCoreApplication::translate("LicenseKey_Dialog", "Click", nullptr));
        label_7->setText(QCoreApplication::translate("LicenseKey_Dialog", "to obtain or enter your license KEY details", nullptr));
        label_8->setText(QCoreApplication::translate("LicenseKey_Dialog", "to purchase your software license", nullptr));
        label_9->setText(QCoreApplication::translate("LicenseKey_Dialog", "to run the software in trial mode", nullptr));
        label_10->setText(QCoreApplication::translate("LicenseKey_Dialog", "In trial mode, only small files can be saved and", nullptr));
        label_11->setText(QCoreApplication::translate("LicenseKey_Dialog", "no special entities (such as walls) can be saved.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LicenseKey_Dialog: public Ui_LicenseKey_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEKEY_DIALOG_H
