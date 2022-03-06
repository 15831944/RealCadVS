/********************************************************************************
** Form generated from reading UI file 'SURFACEOFREVINSERT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SURFACEOFREVINSERT_DIALOG_H
#define UI_SURFACEOFREVINSERT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SurfaceOfRevInsert_Dialog
{
public:
    QCheckBox *_100;
    QCheckBox *_101;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *SurfaceOfRevInsert_Dialog)
    {
        if (SurfaceOfRevInsert_Dialog->objectName().isEmpty())
            SurfaceOfRevInsert_Dialog->setObjectName(QString::fromUtf8("SurfaceOfRevInsert_Dialog"));
        SurfaceOfRevInsert_Dialog->resize(219, 99);
        QFont font;
        font.setPointSize(10);
        SurfaceOfRevInsert_Dialog->setFont(font);
        _100 = new QCheckBox(SurfaceOfRevInsert_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(14, 4, 177, 31));
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _101 = new QCheckBox(SurfaceOfRevInsert_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(12, 30, 179, 31));
        _101->setLayoutDirection(Qt::RightToLeft);
        _101->setCheckable(true);
        _101->setChecked(false);
        _101->setTristate(false);
        _2 = new QPushButton(SurfaceOfRevInsert_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(122, 64, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(SurfaceOfRevInsert_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(30, 64, 75, 23));

        retranslateUi(SurfaceOfRevInsert_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SurfaceOfRevInsert_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SurfaceOfRevInsert_Dialog)
    {
        SurfaceOfRevInsert_Dialog->setWindowTitle(QCoreApplication::translate("SurfaceOfRevInsert_Dialog", "Surface of revolution options", nullptr));
        _100->setText(QCoreApplication::translate("SurfaceOfRevInsert_Dialog", "Copy profile entities  ", nullptr));
        _101->setText(QCoreApplication::translate("SurfaceOfRevInsert_Dialog", "Insert side surfaces   ", nullptr));
        _2->setText(QCoreApplication::translate("SurfaceOfRevInsert_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SurfaceOfRevInsert_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SurfaceOfRevInsert_Dialog: public Ui_SurfaceOfRevInsert_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURFACEOFREVINSERT_DIALOG_H
