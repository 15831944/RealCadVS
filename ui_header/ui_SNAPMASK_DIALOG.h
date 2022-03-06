/********************************************************************************
** Form generated from reading UI file 'SNAPMASK_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAPMASK_DIALOG_H
#define UI_SNAPMASK_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SnapMask_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QCheckBox *_100;
    QCheckBox *_101;
    QCheckBox *_102;
    QGroupBox *groupBox_2;
    QCheckBox *_103;
    QCheckBox *_104;

    void setupUi(QDialog *SnapMask_Dialog)
    {
        if (SnapMask_Dialog->objectName().isEmpty())
            SnapMask_Dialog->setObjectName(QString::fromUtf8("SnapMask_Dialog"));
        SnapMask_Dialog->resize(223, 250);
        SnapMask_Dialog->setMinimumSize(QSize(223, 250));
        SnapMask_Dialog->setMaximumSize(QSize(223, 250));
        QFont font;
        font.setPointSize(10);
        SnapMask_Dialog->setFont(font);
        _2 = new QPushButton(SnapMask_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(120, 202, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(SnapMask_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(30, 202, 75, 23));
        groupBox = new QGroupBox(SnapMask_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(11, 7, 201, 101));
        groupBox->setFont(font);
        _100 = new QCheckBox(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(30, 20, 121, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setAutoFillBackground(false);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _101 = new QCheckBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(30, 43, 121, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _101->setAutoFillBackground(false);
        _101->setCheckable(true);
        _101->setChecked(false);
        _101->setTristate(false);
        _102 = new QCheckBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(30, 66, 121, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setAutoFillBackground(false);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        groupBox_2 = new QGroupBox(SnapMask_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 114, 201, 81));
        groupBox_2->setFont(font);
        _103 = new QCheckBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(30, 20, 121, 20));
        _103->setFont(font);
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setAutoFillBackground(false);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _104 = new QCheckBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(true);
        _104->setGeometry(QRect(30, 43, 121, 20));
        _104->setFont(font);
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setAutoFillBackground(false);
        _104->setCheckable(true);
        _104->setChecked(false);
        _104->setTristate(false);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(SnapMask_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SnapMask_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SnapMask_Dialog)
    {
        SnapMask_Dialog->setWindowTitle(QCoreApplication::translate("SnapMask_Dialog", "Snap and Drag Settings", nullptr));
        _2->setText(QCoreApplication::translate("SnapMask_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SnapMask_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SnapMask_Dialog", "Snap Settings", nullptr));
        _100->setText(QCoreApplication::translate("SnapMask_Dialog", "Origin", nullptr));
        _101->setText(QCoreApplication::translate("SnapMask_Dialog", "Intersection", nullptr));
        _102->setText(QCoreApplication::translate("SnapMask_Dialog", "End", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("SnapMask_Dialog", "Drag Settings", nullptr));
        _103->setText(QCoreApplication::translate("SnapMask_Dialog", "Stretch", nullptr));
        _104->setText(QCoreApplication::translate("SnapMask_Dialog", "Constrain drag", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SnapMask_Dialog: public Ui_SnapMask_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAPMASK_DIALOG_H
