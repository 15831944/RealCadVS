/********************************************************************************
** Form generated from reading UI file 'PATCHTRIANGULATE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCHTRIANGULATE_DIALOG_H
#define UI_PATCHTRIANGULATE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PatchTriangulate_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *_100;
    QSpinBox *_101;

    void setupUi(QDialog *PatchTriangulate_Dialog)
    {
        if (PatchTriangulate_Dialog->objectName().isEmpty())
            PatchTriangulate_Dialog->setObjectName(QString::fromUtf8("PatchTriangulate_Dialog"));
        PatchTriangulate_Dialog->resize(378, 130);
        PatchTriangulate_Dialog->setMinimumSize(QSize(378, 130));
        PatchTriangulate_Dialog->setMaximumSize(QSize(378, 130));
        QFont font;
        font.setPointSize(10);
        PatchTriangulate_Dialog->setFont(font);
        _2 = new QPushButton(PatchTriangulate_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(290, 80, 75, 23));
        _1 = new QPushButton(PatchTriangulate_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(200, 80, 75, 23));
        label_7 = new QLabel(PatchTriangulate_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(66, 20, 141, 20));
        label_7->setFont(font);
        label_8 = new QLabel(PatchTriangulate_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(66, 46, 141, 20));
        label_8->setFont(font);
        _100 = new QSpinBox(PatchTriangulate_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(208, 16, 77, 24));
        _100->setMinimum(2);
        _100->setMaximum(1000);
        _100->setSingleStep(1);
        _101 = new QSpinBox(PatchTriangulate_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(208, 44, 77, 24));
        _101->setMinimum(2);
        _101->setMaximum(1000);

        retranslateUi(PatchTriangulate_Dialog);

        QMetaObject::connectSlotsByName(PatchTriangulate_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PatchTriangulate_Dialog)
    {
        PatchTriangulate_Dialog->setWindowTitle(QCoreApplication::translate("PatchTriangulate_Dialog", "Triangulate Surface Options", nullptr));
        _2->setText(QCoreApplication::translate("PatchTriangulate_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PatchTriangulate_Dialog", "OK", nullptr));
        label_7->setText(QCoreApplication::translate("PatchTriangulate_Dialog", "Mesh points X", nullptr));
        label_8->setText(QCoreApplication::translate("PatchTriangulate_Dialog", "Mesh points Y", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PatchTriangulate_Dialog: public Ui_PatchTriangulate_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCHTRIANGULATE_DIALOG_H
