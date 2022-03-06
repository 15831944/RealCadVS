/********************************************************************************
** Form generated from reading UI file 'FINDINFILES_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDINFILES_DIALOG_H
#define UI_FINDINFILES_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_FindInFiles_Dialog
{
public:
    QLineEdit *_100;
    QLabel *label_44;
    QLineEdit *_102;
    QLabel *label_45;
    QPushButton *_101;
    QGroupBox *groupBox_2;
    QCheckBox *_103;
    QCheckBox *_104;
    QCheckBox *_105;
    QPushButton *_2;
    QPushButton *_108;
    QPushButton *_107;
    RCComboList *_106;

    void setupUi(QDialog *FindInFiles_Dialog)
    {
        if (FindInFiles_Dialog->objectName().isEmpty())
            FindInFiles_Dialog->setObjectName(QString::fromUtf8("FindInFiles_Dialog"));
        FindInFiles_Dialog->resize(500, 342);
        FindInFiles_Dialog->setMinimumSize(QSize(500, 342));
        FindInFiles_Dialog->setMaximumSize(QSize(500, 342));
        QFont font;
        font.setPointSize(10);
        FindInFiles_Dialog->setFont(font);
        _100 = new QLineEdit(FindInFiles_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(100, 10, 291, 20));
        label_44 = new QLabel(FindInFiles_Dialog);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(20, 10, 71, 20));
        label_44->setFont(font);
        label_44->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QLineEdit(FindInFiles_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(100, 40, 381, 20));
        label_45 = new QLabel(FindInFiles_Dialog);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(20, 40, 71, 20));
        label_45->setFont(font);
        label_45->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QPushButton(FindInFiles_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(400, 10, 81, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _101->setFont(font1);
        groupBox_2 = new QGroupBox(FindInFiles_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 70, 461, 71));
        _103 = new QCheckBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(14, 20, 167, 21));
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _104 = new QCheckBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(true);
        _104->setGeometry(QRect(16, 40, 165, 21));
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setCheckable(true);
        _104->setChecked(false);
        _104->setTristate(false);
        _105 = new QCheckBox(groupBox_2);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(194, 20, 209, 21));
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _2 = new QPushButton(FindInFiles_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(400, 310, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _108 = new QPushButton(FindInFiles_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(310, 310, 75, 23));
        _108->setFont(font1);
        _108->setAutoDefault(false);
        _107 = new QPushButton(FindInFiles_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(220, 310, 75, 23));
        _107->setFont(font1);
        _106 = new RCComboList(FindInFiles_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(20, 150, 461, 153));
        _106->setMinimumSize(QSize(461, 153));
        _106->setMaximumSize(QSize(461, 153));

        retranslateUi(FindInFiles_Dialog);

        _107->setDefault(true);


        QMetaObject::connectSlotsByName(FindInFiles_Dialog);
    } // setupUi

    void retranslateUi(QDialog *FindInFiles_Dialog)
    {
        FindInFiles_Dialog->setWindowTitle(QCoreApplication::translate("FindInFiles_Dialog", "Find in files", nullptr));
        label_44->setText(QCoreApplication::translate("FindInFiles_Dialog", "Path", nullptr));
        label_45->setText(QCoreApplication::translate("FindInFiles_Dialog", "Text to find", nullptr));
        _101->setText(QCoreApplication::translate("FindInFiles_Dialog", "Browse...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FindInFiles_Dialog", "Options", nullptr));
        _103->setText(QCoreApplication::translate("FindInFiles_Dialog", "Search text entities", nullptr));
        _104->setText(QCoreApplication::translate("FindInFiles_Dialog", "Match case", nullptr));
        _105->setText(QCoreApplication::translate("FindInFiles_Dialog", "Search entity attributes", nullptr));
        _2->setText(QCoreApplication::translate("FindInFiles_Dialog", "Cancel", nullptr));
        _108->setText(QCoreApplication::translate("FindInFiles_Dialog", "Open", nullptr));
        _107->setText(QCoreApplication::translate("FindInFiles_Dialog", "Find", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindInFiles_Dialog: public Ui_FindInFiles_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDINFILES_DIALOG_H
