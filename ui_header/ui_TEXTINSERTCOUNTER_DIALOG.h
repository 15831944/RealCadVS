/********************************************************************************
** Form generated from reading UI file 'TEXTINSERTCOUNTER_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTINSERTCOUNTER_DIALOG_H
#define UI_TEXTINSERTCOUNTER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_TextInsertCounter_Dialog
{
public:
    QLabel *label;
    QLineEdit *_100;
    QLineEdit *_101;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *_2;
    QPushButton *_1;
    RCIntegerSpinBox *_102;
    RCIntegerSpinBox *_103;

    void setupUi(QDialog *TextInsertCounter_Dialog)
    {
        if (TextInsertCounter_Dialog->objectName().isEmpty())
            TextInsertCounter_Dialog->setObjectName(QString::fromUtf8("TextInsertCounter_Dialog"));
        TextInsertCounter_Dialog->resize(370, 190);
        TextInsertCounter_Dialog->setMinimumSize(QSize(370, 190));
        TextInsertCounter_Dialog->setMaximumSize(QSize(370, 190));
        label = new QLabel(TextInsertCounter_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 71, 20));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(TextInsertCounter_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(80, 20, 271, 20));
        _101 = new QLineEdit(TextInsertCounter_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(80, 50, 271, 20));
        label_2 = new QLabel(TextInsertCounter_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 71, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3 = new QLabel(TextInsertCounter_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 71, 20));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(TextInsertCounter_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 110, 71, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(TextInsertCounter_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(270, 140, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(TextInsertCounter_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(180, 140, 75, 23));
        _1->setFont(font);
        _102 = new RCIntegerSpinBox(TextInsertCounter_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(80, 78, 87, 22));
        _102->setMaximum(1000);
        _103 = new RCIntegerSpinBox(TextInsertCounter_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(80, 108, 87, 22));
        _103->setMaximum(10000);

        retranslateUi(TextInsertCounter_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(TextInsertCounter_Dialog);
    } // setupUi

    void retranslateUi(QDialog *TextInsertCounter_Dialog)
    {
        TextInsertCounter_Dialog->setWindowTitle(QCoreApplication::translate("TextInsertCounter_Dialog", "Insert counter", nullptr));
        label->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "Prefix", nullptr));
        label_2->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "Suffix", nullptr));
        label_3->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "Start", nullptr));
        label_4->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "Increment", nullptr));
        _2->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("TextInsertCounter_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextInsertCounter_Dialog: public Ui_TextInsertCounter_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTINSERTCOUNTER_DIALOG_H
