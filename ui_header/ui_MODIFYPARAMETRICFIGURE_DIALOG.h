/********************************************************************************
** Form generated from reading UI file 'MODIFYPARAMETRICFIGURE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFYPARAMETRICFIGURE_DIALOG_H
#define UI_MODIFYPARAMETRICFIGURE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ModifyParametricFigure_Dialog
{
public:
    QPushButton *_1;
    QPushButton *_2;
    QComboBox *_100;
    QLabel *label_7;
    QLabel *label_21;
    QDoubleSpinBox *_101;

    void setupUi(QDialog *ModifyParametricFigure_Dialog)
    {
        if (ModifyParametricFigure_Dialog->objectName().isEmpty())
            ModifyParametricFigure_Dialog->setObjectName(QString::fromUtf8("ModifyParametricFigure_Dialog"));
        ModifyParametricFigure_Dialog->resize(395, 126);
        QFont font;
        font.setPointSize(10);
        ModifyParametricFigure_Dialog->setFont(font);
        _1 = new QPushButton(ModifyParametricFigure_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(210, 90, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _1->setFont(font1);
        _2 = new QPushButton(ModifyParametricFigure_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(300, 90, 75, 23));
        _2->setFont(font1);
        _100 = new QComboBox(ModifyParametricFigure_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(147, 18, 231, 23));
        label_7 = new QLabel(ModifyParametricFigure_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(12, 20, 135, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(13);
        label_7->setFont(font2);
        label_21 = new QLabel(ModifyParametricFigure_Dialog);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(12, 50, 133, 20));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        label_21->setFont(font3);
        label_21->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QDoubleSpinBox(ModifyParametricFigure_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(148, 48, 105, 24));
        _101->setMinimum(-100000000.000000000000000);
        _101->setMaximum(100000000.000000000000000);

        retranslateUi(ModifyParametricFigure_Dialog);

        QMetaObject::connectSlotsByName(ModifyParametricFigure_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyParametricFigure_Dialog)
    {
        ModifyParametricFigure_Dialog->setWindowTitle(QCoreApplication::translate("ModifyParametricFigure_Dialog", "Modify Parametric component", nullptr));
        _1->setText(QCoreApplication::translate("ModifyParametricFigure_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("ModifyParametricFigure_Dialog", "Cancel", nullptr));
        label_7->setText(QCoreApplication::translate("ModifyParametricFigure_Dialog", "Parameter to modify", nullptr));
        label_21->setText(QCoreApplication::translate("ModifyParametricFigure_Dialog", "Parameter value", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifyParametricFigure_Dialog: public Ui_ModifyParametricFigure_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYPARAMETRICFIGURE_DIALOG_H
