/********************************************************************************
** Form generated from reading UI file 'MEASUREANGLE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREANGLE_DIALOG_H
#define UI_MEASUREANGLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_MeasureAngle_Dialog
{
public:
    QLineEdit *_101;
    QLineEdit *_100;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_7;
    QLineEdit *_102;

    void setupUi(QDialog *MeasureAngle_Dialog)
    {
        if (MeasureAngle_Dialog->objectName().isEmpty())
            MeasureAngle_Dialog->setObjectName(QString::fromUtf8("MeasureAngle_Dialog"));
        MeasureAngle_Dialog->resize(362, 104);
        MeasureAngle_Dialog->setMinimumSize(QSize(362, 104));
        MeasureAngle_Dialog->setMaximumSize(QSize(362, 104));
        QFont font;
        font.setPointSize(10);
        MeasureAngle_Dialog->setFont(font);
        MeasureAngle_Dialog->setModal(false);
        _101 = new QLineEdit(MeasureAngle_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(160, 37, 181, 20));
        _100 = new QLineEdit(MeasureAngle_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(160, 10, 181, 20));
        label_6 = new QLabel(MeasureAngle_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 37, 141, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(MeasureAngle_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 10, 141, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(MeasureAngle_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 64, 141, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QLineEdit(MeasureAngle_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(160, 64, 181, 20));

        retranslateUi(MeasureAngle_Dialog);

        QMetaObject::connectSlotsByName(MeasureAngle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *MeasureAngle_Dialog)
    {
        MeasureAngle_Dialog->setWindowTitle(QCoreApplication::translate("MeasureAngle_Dialog", "Measure Angle", nullptr));
        label_6->setText(QCoreApplication::translate("MeasureAngle_Dialog", "Angle two", nullptr));
        label_5->setText(QCoreApplication::translate("MeasureAngle_Dialog", "Angle one", nullptr));
        label_7->setText(QCoreApplication::translate("MeasureAngle_Dialog", "Distance between lines", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureAngle_Dialog: public Ui_MeasureAngle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREANGLE_DIALOG_H
