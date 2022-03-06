/********************************************************************************
** Form generated from reading UI file 'MISSING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MISSING_DIALOG_H
#define UI_MISSING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Missing_Dialog
{
public:
    QLabel *label_4;
    QPushButton *_101;
    QPushButton *_102;
    QPushButton *_2;
    QListWidget *_100;

    void setupUi(QDialog *Missing_Dialog)
    {
        if (Missing_Dialog->objectName().isEmpty())
            Missing_Dialog->setObjectName(QString::fromUtf8("Missing_Dialog"));
        Missing_Dialog->resize(682, 320);
        Missing_Dialog->setMinimumSize(QSize(682, 320));
        Missing_Dialog->setMaximumSize(QSize(682, 320));
        QFont font;
        font.setPointSize(10);
        Missing_Dialog->setFont(font);
        label_4 = new QLabel(Missing_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 10, 661, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);
        _101 = new QPushButton(Missing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(422, 280, 75, 24));
        _101->setFont(font);
        _102 = new QPushButton(Missing_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(276, 280, 131, 24));
        _102->setFont(font);
        _2 = new QPushButton(Missing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(184, 280, 75, 24));
        _2->setFont(font);
        _100 = new QListWidget(Missing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(14, 35, 655, 233));
        _100->setSelectionMode(QAbstractItemView::ExtendedSelection);
        _100->setSelectionBehavior(QAbstractItemView::SelectRows);
        _100->setSortingEnabled(true);

        retranslateUi(Missing_Dialog);

        _2->setDefault(true);


        QMetaObject::connectSlotsByName(Missing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Missing_Dialog)
    {
        Missing_Dialog->setWindowTitle(QCoreApplication::translate("Missing_Dialog", "Missing Components", nullptr));
        label_4->setText(QCoreApplication::translate("Missing_Dialog", "The following components are missing.", nullptr));
        _101->setText(QCoreApplication::translate("Missing_Dialog", "Delete", nullptr));
        _102->setText(QCoreApplication::translate("Missing_Dialog", "View as text file", nullptr));
        _2->setText(QCoreApplication::translate("Missing_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Missing_Dialog: public Ui_Missing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MISSING_DIALOG_H
