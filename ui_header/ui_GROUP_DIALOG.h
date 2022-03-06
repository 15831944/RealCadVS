/********************************************************************************
** Form generated from reading UI file 'GROUP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUP_DIALOG_H
#define UI_GROUP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Group_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_3;
    QLineEdit *_100;
    QLineEdit *_101;
    QLabel *label_4;

    void setupUi(QDialog *Group_Dialog)
    {
        if (Group_Dialog->objectName().isEmpty())
            Group_Dialog->setObjectName(QString::fromUtf8("Group_Dialog"));
        Group_Dialog->resize(484, 120);
        Group_Dialog->setMinimumSize(QSize(484, 120));
        Group_Dialog->setMaximumSize(QSize(484, 120));
        QFont font;
        font.setPointSize(10);
        Group_Dialog->setFont(font);
        _2 = new QPushButton(Group_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(392, 70, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Group_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(298, 70, 75, 23));
        label_3 = new QLabel(Group_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(24, 12, 77, 21));
        _100 = new QLineEdit(Group_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(110, 10, 350, 21));
        _101 = new QLineEdit(Group_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(110, 36, 350, 21));
        label_4 = new QLabel(Group_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(24, 36, 77, 21));

        retranslateUi(Group_Dialog);

        _2->setDefault(false);
        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Group_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Group_Dialog)
    {
        Group_Dialog->setWindowTitle(QCoreApplication::translate("Group_Dialog", "Group options", nullptr));
        _2->setText(QCoreApplication::translate("Group_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Group_Dialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("Group_Dialog", "Name", nullptr));
        label_4->setText(QCoreApplication::translate("Group_Dialog", "Instance", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Group_Dialog: public Ui_Group_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUP_DIALOG_H
