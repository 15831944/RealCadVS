/********************************************************************************
** Form generated from reading UI file 'SELECTDRAWING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTDRAWING_DIALOG_H
#define UI_SELECTDRAWING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectDrawing_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *_2;
    QPushButton *_1;
    QListWidget *_100;
    RCComboList *_101;

    void setupUi(QDialog *SelectDrawing_Dialog)
    {
        if (SelectDrawing_Dialog->objectName().isEmpty())
            SelectDrawing_Dialog->setObjectName(QString::fromUtf8("SelectDrawing_Dialog"));
        SelectDrawing_Dialog->resize(525, 253);
        SelectDrawing_Dialog->setMinimumSize(QSize(525, 253));
        SelectDrawing_Dialog->setMaximumSize(QSize(525, 253));
        QFont font;
        font.setPointSize(10);
        SelectDrawing_Dialog->setFont(font);
        label = new QLabel(SelectDrawing_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(16, 8, 251, 20));
        label->setFont(font);
        label_2 = new QLabel(SelectDrawing_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(268, 8, 251, 20));
        label_2->setFont(font);
        _2 = new QPushButton(SelectDrawing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(437, 220, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(SelectDrawing_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(347, 220, 75, 23));
        _100 = new QListWidget(SelectDrawing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(16, 30, 243, 181));
        _100->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        _100->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _100->setSelectionBehavior(QAbstractItemView::SelectRows);
        _101 = new RCComboList(SelectDrawing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(268, 30, 243, 181));
        _101->setSelectionBehavior(QAbstractItemView::SelectRows);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(SelectDrawing_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SelectDrawing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectDrawing_Dialog)
    {
        SelectDrawing_Dialog->setWindowTitle(QCoreApplication::translate("SelectDrawing_Dialog", "Select the layout drawing and zone to display", nullptr));
        label->setText(QCoreApplication::translate("SelectDrawing_Dialog", "Select the drawing to display", nullptr));
        label_2->setText(QCoreApplication::translate("SelectDrawing_Dialog", "Select the zone to display", nullptr));
        _2->setText(QCoreApplication::translate("SelectDrawing_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SelectDrawing_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectDrawing_Dialog: public Ui_SelectDrawing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDRAWING_DIALOG_H
