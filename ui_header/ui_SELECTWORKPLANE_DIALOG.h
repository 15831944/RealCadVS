/********************************************************************************
** Form generated from reading UI file 'SELECTWORKPLANE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTWORKPLANE_DIALOG_H
#define UI_SELECTWORKPLANE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectWorkplane_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_102;
    QPushButton *_1;
    QPushButton *_101;
    QLabel *label_6;
    QCheckBox *_103;
    QCheckBox *_104;
    RCComboList *_100;

    void setupUi(QDialog *SelectWorkplane_Dialog)
    {
        if (SelectWorkplane_Dialog->objectName().isEmpty())
            SelectWorkplane_Dialog->setObjectName(QString::fromUtf8("SelectWorkplane_Dialog"));
        SelectWorkplane_Dialog->resize(338, 320);
        SelectWorkplane_Dialog->setMinimumSize(QSize(338, 320));
        SelectWorkplane_Dialog->setMaximumSize(QSize(338, 320));
        QFont font;
        font.setPointSize(10);
        SelectWorkplane_Dialog->setFont(font);
        _2 = new QPushButton(SelectWorkplane_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(252, 260, 75, 24));
        _2->setFont(font);
        _102 = new QPushButton(SelectWorkplane_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(170, 260, 75, 24));
        _102->setFont(font);
        _1 = new QPushButton(SelectWorkplane_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(10, 260, 75, 24));
        _1->setFont(font);
        _101 = new QPushButton(SelectWorkplane_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(90, 260, 75, 24));
        _101->setFont(font);
        label_6 = new QLabel(SelectWorkplane_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(12, 8, 311, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);
        _103 = new QCheckBox(SelectWorkplane_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(50, 206, 171, 20));
        _103->setFont(font);
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setAutoFillBackground(false);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _104 = new QCheckBox(SelectWorkplane_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(true);
        _104->setGeometry(QRect(70, 228, 151, 20));
        _104->setFont(font);
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setAutoFillBackground(false);
        _104->setCheckable(true);
        _104->setChecked(false);
        _104->setTristate(false);
        _100 = new RCComboList(SelectWorkplane_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(46, 29, 250, 169));
        _100->setMinimumSize(QSize(250, 169));
        _100->setMaximumSize(QSize(250, 169));
        _100->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _100->setSortingEnabled(true);

        retranslateUi(SelectWorkplane_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SelectWorkplane_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectWorkplane_Dialog)
    {
        SelectWorkplane_Dialog->setWindowTitle(QCoreApplication::translate("SelectWorkplane_Dialog", "Select workplane", nullptr));
        _2->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Cancel", nullptr));
        _102->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Display Off", nullptr));
        _1->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "OK", nullptr));
        _101->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Display On", nullptr));
        label_6->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Select the new workplane to use", nullptr));
        _103->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Display workplanes", nullptr));
        _104->setText(QCoreApplication::translate("SelectWorkplane_Dialog", "Display Z axis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectWorkplane_Dialog: public Ui_SelectWorkplane_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTWORKPLANE_DIALOG_H
