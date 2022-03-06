/********************************************************************************
** Form generated from reading UI file 'PATCHCORNERS_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCHCORNERS_DIALOG_H
#define UI_PATCHCORNERS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PatchCorners_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QSpinBox *_100;
    QSpinBox *_101;
    QSpinBox *_102;
    QSpinBox *_103;

    void setupUi(QDialog *PatchCorners_Dialog)
    {
        if (PatchCorners_Dialog->objectName().isEmpty())
            PatchCorners_Dialog->setObjectName(QString::fromUtf8("PatchCorners_Dialog"));
        PatchCorners_Dialog->resize(400, 169);
        QFont font;
        font.setPointSize(10);
        PatchCorners_Dialog->setFont(font);
        _2 = new QPushButton(PatchCorners_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(290, 130, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PatchCorners_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(200, 130, 75, 23));
        label_8 = new QLabel(PatchCorners_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 18, 271, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9 = new QLabel(PatchCorners_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 45, 271, 20));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_10 = new QLabel(PatchCorners_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 98, 271, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_11 = new QLabel(PatchCorners_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 70, 271, 20));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QSpinBox(PatchCorners_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(294, 16, 69, 24));
        _100->setMinimum(2);
        _100->setMaximum(10);
        _100->setValue(3);
        _101 = new QSpinBox(PatchCorners_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(294, 42, 69, 24));
        _101->setMinimum(2);
        _101->setMaximum(10);
        _101->setValue(3);
        _102 = new QSpinBox(PatchCorners_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(294, 68, 69, 24));
        _102->setMinimum(3);
        _102->setMaximum(200);
        _102->setValue(4);
        _103 = new QSpinBox(PatchCorners_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(294, 94, 69, 24));
        _103->setMinimum(3);
        _103->setMaximum(200);
        _103->setValue(4);

        retranslateUi(PatchCorners_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PatchCorners_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PatchCorners_Dialog)
    {
        PatchCorners_Dialog->setWindowTitle(QCoreApplication::translate("PatchCorners_Dialog", "Insert patch by corners options", nullptr));
        _2->setText(QCoreApplication::translate("PatchCorners_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PatchCorners_Dialog", "OK", nullptr));
        label_8->setText(QCoreApplication::translate("PatchCorners_Dialog", "Degree along first two corners", nullptr));
        label_9->setText(QCoreApplication::translate("PatchCorners_Dialog", "Degree along first and third corner", nullptr));
        label_10->setText(QCoreApplication::translate("PatchCorners_Dialog", "Control points between first and third corners", nullptr));
        label_11->setText(QCoreApplication::translate("PatchCorners_Dialog", "Control points between first two corners", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PatchCorners_Dialog: public Ui_PatchCorners_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCHCORNERS_DIALOG_H
