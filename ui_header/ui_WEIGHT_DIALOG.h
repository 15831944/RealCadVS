/********************************************************************************
** Form generated from reading UI file 'WEIGHT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIGHT_DIALOG_H
#define UI_WEIGHT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Weight_Dialog
{
public:
    QLabel *label;
    QPushButton *_102;
    QPushButton *_1;
    QPushButton *_2;
    WeightSpinBox *_100;
    QListView *_101;

    void setupUi(QDialog *Weight_Dialog)
    {
        if (Weight_Dialog->objectName().isEmpty())
            Weight_Dialog->setObjectName(QString::fromUtf8("Weight_Dialog"));
        Weight_Dialog->resize(314, 236);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Weight_Dialog->sizePolicy().hasHeightForWidth());
        Weight_Dialog->setSizePolicy(sizePolicy);
        Weight_Dialog->setMinimumSize(QSize(314, 236));
        Weight_Dialog->setMaximumSize(QSize(314, 236));
        label = new QLabel(Weight_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(74, 10, 61, 20));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QPushButton(Weight_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(115, 200, 75, 23));
        _102->setFont(font);
        _1 = new QPushButton(Weight_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(25, 200, 75, 23));
        _1->setFont(font);
        _2 = new QPushButton(Weight_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(205, 200, 75, 23));
        _2->setFont(font);
        _100 = new WeightSpinBox(Weight_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(130, 10, 71, 22));
        _100->setWrapping(true);
        _100->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        _100->setMaximum(3.000000000000000);
        _100->setSingleStep(0.100000000000000);
        _100->setValue(0.100000000000000);
        _101 = new QListView(Weight_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(25, 40, 256, 146));
        _101->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _101->setAutoScroll(true);
        _101->setAutoScrollMargin(10);
        _101->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _101->setTabKeyNavigation(true);
        _101->setProperty("showDropIndicator", QVariant(false));
        _101->setGridSize(QSize(0, 20));
        _101->setUniformItemSizes(true);
        _101->setSelectionRectVisible(true);

        retranslateUi(Weight_Dialog);

        QMetaObject::connectSlotsByName(Weight_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Weight_Dialog)
    {
        Weight_Dialog->setWindowTitle(QCoreApplication::translate("Weight_Dialog", "Weight selection", nullptr));
        label->setText(QCoreApplication::translate("Weight_Dialog", "Weight", nullptr));
        _102->setText(QCoreApplication::translate("Weight_Dialog", "Clone", nullptr));
        _1->setText(QCoreApplication::translate("Weight_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Weight_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Weight_Dialog: public Ui_Weight_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIGHT_DIALOG_H
