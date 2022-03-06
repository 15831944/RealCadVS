/********************************************************************************
** Form generated from reading UI file 'LICENSEAGREEMENT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENSEAGREEMENT_DIALOG_H
#define UI_LICENSEAGREEMENT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_LicenseAgreement_Dialog
{
public:
    QLabel *label_4;
    QPushButton *_102;
    QPushButton *_101;
    QTextEdit *_100;

    void setupUi(QDialog *LicenseAgreement_Dialog)
    {
        if (LicenseAgreement_Dialog->objectName().isEmpty())
            LicenseAgreement_Dialog->setObjectName(QString::fromUtf8("LicenseAgreement_Dialog"));
        LicenseAgreement_Dialog->resize(551, 325);
        LicenseAgreement_Dialog->setMinimumSize(QSize(551, 325));
        LicenseAgreement_Dialog->setMaximumSize(QSize(551, 325));
        QFont font;
        font.setPointSize(10);
        LicenseAgreement_Dialog->setFont(font);
        label_4 = new QLabel(LicenseAgreement_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 10, 161, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QPushButton(LicenseAgreement_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(460, 290, 75, 23));
        _101 = new QPushButton(LicenseAgreement_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(370, 290, 75, 23));
        _100 = new QTextEdit(LicenseAgreement_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(16, 34, 517, 247));
        _100->setReadOnly(true);

        retranslateUi(LicenseAgreement_Dialog);

        QMetaObject::connectSlotsByName(LicenseAgreement_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LicenseAgreement_Dialog)
    {
        LicenseAgreement_Dialog->setWindowTitle(QCoreApplication::translate("LicenseAgreement_Dialog", "Software license agreement", nullptr));
        label_4->setText(QCoreApplication::translate("LicenseAgreement_Dialog", "License agreement:", nullptr));
        _102->setText(QCoreApplication::translate("LicenseAgreement_Dialog", "Exit", nullptr));
        _101->setText(QCoreApplication::translate("LicenseAgreement_Dialog", "I Accept", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LicenseAgreement_Dialog: public Ui_LicenseAgreement_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEAGREEMENT_DIALOG_H
