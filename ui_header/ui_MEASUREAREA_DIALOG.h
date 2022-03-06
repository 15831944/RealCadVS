/********************************************************************************
** Form generated from reading UI file 'MEASUREAREA_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREAREA_DIALOG_H
#define UI_MEASUREAREA_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_MeasureArea_Dialog
{
public:
    QLineEdit *_101;
    QLineEdit *_100;
    QLabel *label_6;
    QLabel *label_5;

    void setupUi(QDialog *MeasureArea_Dialog)
    {
        if (MeasureArea_Dialog->objectName().isEmpty())
            MeasureArea_Dialog->setObjectName(QString::fromUtf8("MeasureArea_Dialog"));
        MeasureArea_Dialog->resize(284, 75);
        MeasureArea_Dialog->setMinimumSize(QSize(284, 75));
        MeasureArea_Dialog->setMaximumSize(QSize(284, 75));
        QFont font;
        font.setPointSize(10);
        MeasureArea_Dialog->setFont(font);
        _101 = new QLineEdit(MeasureArea_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(90, 37, 161, 20));
        _100 = new QLineEdit(MeasureArea_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(90, 10, 161, 20));
        label_6 = new QLabel(MeasureArea_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 37, 71, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(MeasureArea_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 10, 71, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        retranslateUi(MeasureArea_Dialog);

        QMetaObject::connectSlotsByName(MeasureArea_Dialog);
    } // setupUi

    void retranslateUi(QDialog *MeasureArea_Dialog)
    {
        MeasureArea_Dialog->setWindowTitle(QCoreApplication::translate("MeasureArea_Dialog", "Measure Area", nullptr));
        label_6->setText(QCoreApplication::translate("MeasureArea_Dialog", "Total area", nullptr));
        label_5->setText(QCoreApplication::translate("MeasureArea_Dialog", "Area", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureArea_Dialog: public Ui_MeasureArea_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREAREA_DIALOG_H
