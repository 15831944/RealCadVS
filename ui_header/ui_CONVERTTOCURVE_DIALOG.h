/********************************************************************************
** Form generated from reading UI file 'CONVERTTOCURVE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERTTOCURVE_DIALOG_H
#define UI_CONVERTTOCURVE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ConvertToCurve_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *_102;
    QSpinBox *_100;
    QSpinBox *_101;

    void setupUi(QDialog *ConvertToCurve_Dialog)
    {
        if (ConvertToCurve_Dialog->objectName().isEmpty())
            ConvertToCurve_Dialog->setObjectName(QString::fromUtf8("ConvertToCurve_Dialog"));
        ConvertToCurve_Dialog->resize(330, 149);
        ConvertToCurve_Dialog->setMinimumSize(QSize(330, 149));
        ConvertToCurve_Dialog->setMaximumSize(QSize(330, 149));
        QFont font;
        font.setPointSize(10);
        ConvertToCurve_Dialog->setFont(font);
        _2 = new QPushButton(ConvertToCurve_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(220, 110, 75, 23));
        _1 = new QPushButton(ConvertToCurve_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(130, 110, 75, 23));
        label_3 = new QLabel(ConvertToCurve_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 131, 21));
        label_4 = new QLabel(ConvertToCurve_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 40, 131, 21));
        _102 = new QCheckBox(ConvertToCurve_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(10, 80, 151, 20));
        _102->setLayoutDirection(Qt::RightToLeft);
        _100 = new QSpinBox(ConvertToCurve_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(174, 12, 129, 22));
        _101 = new QSpinBox(ConvertToCurve_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(174, 40, 129, 22));
        QWidget::setTabOrder(_102, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(ConvertToCurve_Dialog);

        QMetaObject::connectSlotsByName(ConvertToCurve_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ConvertToCurve_Dialog)
    {
        ConvertToCurve_Dialog->setWindowTitle(QCoreApplication::translate("ConvertToCurve_Dialog", "Convert line or arc to a curve options", nullptr));
        _2->setText(QCoreApplication::translate("ConvertToCurve_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("ConvertToCurve_Dialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("ConvertToCurve_Dialog", "Curve degree", nullptr));
        label_4->setText(QCoreApplication::translate("ConvertToCurve_Dialog", "Number of control points", nullptr));
        _102->setText(QCoreApplication::translate("ConvertToCurve_Dialog", "Delete existing entity", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConvertToCurve_Dialog: public Ui_ConvertToCurve_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTTOCURVE_DIALOG_H
