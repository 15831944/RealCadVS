/********************************************************************************
** Form generated from reading UI file 'VIEWCLIP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWCLIP_DIALOG_H
#define UI_VIEWCLIP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_ViewClip_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *_2;
    QPushButton *_102;
    QPushButton *_1;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_101;

    void setupUi(QDialog *ViewClip_Dialog)
    {
        if (ViewClip_Dialog->objectName().isEmpty())
            ViewClip_Dialog->setObjectName(QString::fromUtf8("ViewClip_Dialog"));
        ViewClip_Dialog->resize(285, 112);
        ViewClip_Dialog->setMinimumSize(QSize(285, 112));
        ViewClip_Dialog->setMaximumSize(QSize(285, 112));
        QFont font;
        font.setPointSize(10);
        ViewClip_Dialog->setFont(font);
        label = new QLabel(ViewClip_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(28, 20, 125, 20));
        label_2 = new QLabel(ViewClip_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(28, 46, 125, 20));
        _2 = new QPushButton(ViewClip_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(185, 74, 75, 24));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _102 = new QPushButton(ViewClip_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(105, 74, 75, 24));
        _102->setFont(font);
        _1 = new QPushButton(ViewClip_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(25, 74, 75, 24));
        _1->setFont(font);
        _100 = new RCDoubleSpinBox(ViewClip_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(158, 18, 100, 24));
        _100->setAccelerated(true);
        _100->setMinimum(-1000000000.000000000000000);
        _100->setMaximum(1000000000.000000000000000);
        _101 = new RCDoubleSpinBox(ViewClip_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(158, 44, 100, 24));
        _101->setAccelerated(true);
        _101->setMinimum(-100000000.000000000000000);
        _101->setMaximum(100000000.000000000000000);

        retranslateUi(ViewClip_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(ViewClip_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ViewClip_Dialog)
    {
        ViewClip_Dialog->setWindowTitle(QCoreApplication::translate("ViewClip_Dialog", "Change view clip planes", nullptr));
        label->setText(QCoreApplication::translate("ViewClip_Dialog", "Front clip distance", nullptr));
        label_2->setText(QCoreApplication::translate("ViewClip_Dialog", "Back clip distance", nullptr));
        _2->setText(QCoreApplication::translate("ViewClip_Dialog", "Cancel", nullptr));
        _102->setText(QCoreApplication::translate("ViewClip_Dialog", "Locate", nullptr));
        _1->setText(QCoreApplication::translate("ViewClip_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewClip_Dialog: public Ui_ViewClip_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWCLIP_DIALOG_H
