/********************************************************************************
** Form generated from reading UI file 'COPYCOMPOSEDDRAWING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYCOMPOSEDDRAWING_DIALOG_H
#define UI_COPYCOMPOSEDDRAWING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_CopyComposedDrawing_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *_100;
    RCComboList *_101;

    void setupUi(QDialog *CopyComposedDrawing_Dialog)
    {
        if (CopyComposedDrawing_Dialog->objectName().isEmpty())
            CopyComposedDrawing_Dialog->setObjectName(QString::fromUtf8("CopyComposedDrawing_Dialog"));
        CopyComposedDrawing_Dialog->resize(484, 214);
        CopyComposedDrawing_Dialog->setMinimumSize(QSize(484, 214));
        CopyComposedDrawing_Dialog->setMaximumSize(QSize(484, 214));
        QFont font;
        font.setPointSize(10);
        CopyComposedDrawing_Dialog->setFont(font);
        _2 = new QPushButton(CopyComposedDrawing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(390, 184, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CopyComposedDrawing_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(300, 184, 75, 23));
        label_4 = new QLabel(CopyComposedDrawing_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 181, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(CopyComposedDrawing_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 181, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(CopyComposedDrawing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(210, 20, 261, 20));
        _101 = new RCComboList(CopyComposedDrawing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(210, 44, 261, 135));
        _101->setMinimumSize(QSize(261, 135));
        _101->setMaximumSize(QSize(261, 135));
        _101->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _101->setProperty("showDropIndicator", QVariant(false));
        _101->setSelectionBehavior(QAbstractItemView::SelectRows);
        _101->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

        retranslateUi(CopyComposedDrawing_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CopyComposedDrawing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CopyComposedDrawing_Dialog)
    {
        CopyComposedDrawing_Dialog->setWindowTitle(QCoreApplication::translate("CopyComposedDrawing_Dialog", "Copy layout drawing", nullptr));
        _2->setText(QCoreApplication::translate("CopyComposedDrawing_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CopyComposedDrawing_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("CopyComposedDrawing_Dialog", "New layout drawing name", nullptr));
        label_5->setText(QCoreApplication::translate("CopyComposedDrawing_Dialog", "Copy from layout drawing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CopyComposedDrawing_Dialog: public Ui_CopyComposedDrawing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYCOMPOSEDDRAWING_DIALOG_H
