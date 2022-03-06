/********************************************************************************
** Form generated from reading UI file 'OFFSET_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OFFSET_DIALOG_H
#define UI_OFFSET_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Offset_Dialog
{
public:
    QCheckBox *_100;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *Offset_Dialog)
    {
        if (Offset_Dialog->objectName().isEmpty())
            Offset_Dialog->setObjectName(QString::fromUtf8("Offset_Dialog"));
        Offset_Dialog->resize(269, 90);
        Offset_Dialog->setMinimumSize(QSize(269, 90));
        Offset_Dialog->setMaximumSize(QSize(269, 90));
        QFont font;
        font.setPointSize(10);
        Offset_Dialog->setFont(font);
        _100 = new QCheckBox(Offset_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(18, 8, 181, 21));
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _2 = new QPushButton(Offset_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(163, 40, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Offset_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(73, 40, 75, 23));
        QWidget::setTabOrder(_100, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Offset_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Offset_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Offset_Dialog)
    {
        Offset_Dialog->setWindowTitle(QCoreApplication::translate("Offset_Dialog", "Offset profile", nullptr));
        _100->setText(QCoreApplication::translate("Offset_Dialog", "Offset using round corners", nullptr));
        _2->setText(QCoreApplication::translate("Offset_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Offset_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Offset_Dialog: public Ui_Offset_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OFFSET_DIALOG_H
