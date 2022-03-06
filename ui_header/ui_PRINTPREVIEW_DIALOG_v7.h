/********************************************************************************
** Form generated from reading UI file 'PRINTPREVIEW_DIALOG_v7.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTPREVIEW_DIALOG_V7_H
#define UI_PRINTPREVIEW_DIALOG_V7_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PrintPreview_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_6;
    QComboBox *_100;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_8;
    QDoubleSpinBox *_101;
    QDoubleSpinBox *_102;
    QDoubleSpinBox *_103;

    void setupUi(QDialog *PrintPreview_Dialog)
    {
        if (PrintPreview_Dialog->objectName().isEmpty())
            PrintPreview_Dialog->setObjectName(QString::fromUtf8("PrintPreview_Dialog"));
        PrintPreview_Dialog->resize(327, 180);
        PrintPreview_Dialog->setMinimumSize(QSize(327, 180));
        PrintPreview_Dialog->setMaximumSize(QSize(327, 180));
        QFont font;
        font.setPointSize(10);
        PrintPreview_Dialog->setFont(font);
        _2 = new QPushButton(PrintPreview_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(210, 130, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PrintPreview_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(120, 130, 75, 23));
        label_6 = new QLabel(PrintPreview_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(50, 10, 71, 20));
        label_6->setFont(font);
        _100 = new QComboBox(PrintPreview_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(130, 9, 151, 22));
        _100->setFrame(true);
        label_9 = new QLabel(PrintPreview_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(50, 66, 61, 20));
        label_9->setFont(font);
        label_12 = new QLabel(PrintPreview_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(50, 92, 61, 20));
        label_12->setFont(font);
        label_8 = new QLabel(PrintPreview_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 42, 61, 20));
        label_8->setFont(font);
        _101 = new QDoubleSpinBox(PrintPreview_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(130, 40, 80, 22));
        _101->setDecimals(6);
        _101->setMaximum(100000000.000000000000000);
        _102 = new QDoubleSpinBox(PrintPreview_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(130, 65, 80, 22));
        _102->setDecimals(6);
        _102->setMaximum(100000000.000000000000000);
        _103 = new QDoubleSpinBox(PrintPreview_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(130, 92, 80, 22));
        _103->setDecimals(6);
        _103->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _100);
        QWidget::setTabOrder(_100, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(PrintPreview_Dialog);

        QMetaObject::connectSlotsByName(PrintPreview_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PrintPreview_Dialog)
    {
        PrintPreview_Dialog->setWindowTitle(QCoreApplication::translate("PrintPreview_Dialog", "Print Preview selection options", nullptr));
        _2->setText(QCoreApplication::translate("PrintPreview_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PrintPreview_Dialog", "OK", nullptr));
        label_6->setText(QCoreApplication::translate("PrintPreview_Dialog", "Paper size", nullptr));
        label_9->setText(QCoreApplication::translate("PrintPreview_Dialog", "Height", nullptr));
        label_12->setText(QCoreApplication::translate("PrintPreview_Dialog", "Scale", nullptr));
        label_8->setText(QCoreApplication::translate("PrintPreview_Dialog", "Width", nullptr));
        _103->setSpecialValueText(QCoreApplication::translate("PrintPreview_Dialog", "Fit to paper", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintPreview_Dialog: public Ui_PrintPreview_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTPREVIEW_DIALOG_V7_H
