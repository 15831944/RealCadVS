/********************************************************************************
** Form generated from reading UI file 'MIRROR_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIRROR_DIALOG_H
#define UI_MIRROR_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_Mirror_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QCheckBox *_102;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *Mirror_Dialog)
    {
        if (Mirror_Dialog->objectName().isEmpty())
            Mirror_Dialog->setObjectName(QString::fromUtf8("Mirror_Dialog"));
        Mirror_Dialog->resize(311, 175);
        Mirror_Dialog->setMinimumSize(QSize(311, 175));
        Mirror_Dialog->setMaximumSize(QSize(311, 175));
        QFont font;
        font.setPointSize(10);
        Mirror_Dialog->setFont(font);
        groupBox = new QGroupBox(Mirror_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 291, 81));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 23, 241, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(20, 46, 241, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QCheckBox(Mirror_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(89, 95, 181, 21));
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        _2 = new QPushButton(Mirror_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(224, 124, 75, 23));
        _1 = new QPushButton(Mirror_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(134, 124, 75, 23));
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Mirror_Dialog);

        QMetaObject::connectSlotsByName(Mirror_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Mirror_Dialog)
    {
        Mirror_Dialog->setWindowTitle(QCoreApplication::translate("Mirror_Dialog", "Mirror Options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Mirror_Dialog", "Define mirror plane by", nullptr));
        _100->setText(QCoreApplication::translate("Mirror_Dialog", "Two points or a line", nullptr));
        _101->setText(QCoreApplication::translate("Mirror_Dialog", "Three points, a circle or a plane", nullptr));
        _102->setText(QCoreApplication::translate("Mirror_Dialog", "Leave original", nullptr));
        _2->setText(QCoreApplication::translate("Mirror_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Mirror_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mirror_Dialog: public Ui_Mirror_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIRROR_DIALOG_H
