/********************************************************************************
** Form generated from reading UI file 'PATCHMESH_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCHMESH_DIALOG_H
#define UI_PATCHMESH_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PatchMesh_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QCheckBox *_102;
    QLabel *label_9;
    QLabel *label_8;
    QSpinBox *_100;
    QSpinBox *_101;

    void setupUi(QDialog *PatchMesh_Dialog)
    {
        if (PatchMesh_Dialog->objectName().isEmpty())
            PatchMesh_Dialog->setObjectName(QString::fromUtf8("PatchMesh_Dialog"));
        PatchMesh_Dialog->resize(302, 140);
        PatchMesh_Dialog->setMinimumSize(QSize(302, 140));
        PatchMesh_Dialog->setMaximumSize(QSize(302, 140));
        QFont font;
        font.setPointSize(10);
        PatchMesh_Dialog->setFont(font);
        _2 = new QPushButton(PatchMesh_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(188, 92, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PatchMesh_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(98, 92, 75, 23));
        _102 = new QCheckBox(PatchMesh_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(84, 12, 125, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setAutoFillBackground(false);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        label_9 = new QLabel(PatchMesh_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(14, 62, 167, 20));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(PatchMesh_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(16, 36, 159, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _100 = new QSpinBox(PatchMesh_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(192, 34, 69, 24));
        _100->setMinimum(2);
        _100->setMaximum(10);
        _101 = new QSpinBox(PatchMesh_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(192, 61, 69, 24));
        _101->setMinimum(2);
        _101->setMaximum(10);

        retranslateUi(PatchMesh_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PatchMesh_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PatchMesh_Dialog)
    {
        PatchMesh_Dialog->setWindowTitle(QCoreApplication::translate("PatchMesh_Dialog", "Insert patch by polygon mesh options", nullptr));
        _2->setText(QCoreApplication::translate("PatchMesh_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PatchMesh_Dialog", "OK", nullptr));
        _102->setText(QCoreApplication::translate("PatchMesh_Dialog", "Triangulated mesh ", nullptr));
        label_9->setText(QCoreApplication::translate("PatchMesh_Dialog", "Degree across mesh points", nullptr));
        label_8->setText(QCoreApplication::translate("PatchMesh_Dialog", "Degree along mesh points", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PatchMesh_Dialog: public Ui_PatchMesh_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCHMESH_DIALOG_H
