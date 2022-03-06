/********************************************************************************
** Form generated from reading UI file 'DIMENSIONTEXT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONTEXT_DIALOG_H
#define UI_DIMENSIONTEXT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DimensionText_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *_2;
    QPushButton *_1;
    QTextEdit *_100;
    QTextEdit *_101;

    void setupUi(QDialog *DimensionText_Dialog)
    {
        if (DimensionText_Dialog->objectName().isEmpty())
            DimensionText_Dialog->setObjectName(QString::fromUtf8("DimensionText_Dialog"));
        DimensionText_Dialog->resize(381, 265);
        QFont font;
        font.setPointSize(10);
        DimensionText_Dialog->setFont(font);
        label = new QLabel(DimensionText_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 121, 20));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(DimensionText_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 124, 121, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DimensionText_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(290, 230, 75, 23));
        _1 = new QPushButton(DimensionText_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(200, 230, 75, 23));
        _100 = new QTextEdit(DimensionText_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(18, 32, 343, 87));
        _101 = new QTextEdit(DimensionText_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(16, 142, 343, 79));

        retranslateUi(DimensionText_Dialog);

        QMetaObject::connectSlotsByName(DimensionText_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionText_Dialog)
    {
        DimensionText_Dialog->setWindowTitle(QCoreApplication::translate("DimensionText_Dialog", "Change dimension text", nullptr));
        label->setText(QCoreApplication::translate("DimensionText_Dialog", "Dimension prefix:", nullptr));
        label_2->setText(QCoreApplication::translate("DimensionText_Dialog", "Dimension suffix:", nullptr));
        _2->setText(QCoreApplication::translate("DimensionText_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DimensionText_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DimensionText_Dialog: public Ui_DimensionText_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONTEXT_DIALOG_H
