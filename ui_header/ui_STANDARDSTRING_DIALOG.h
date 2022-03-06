/********************************************************************************
** Form generated from reading UI file 'STANDARDSTRING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STANDARDSTRING_DIALOG_H
#define UI_STANDARDSTRING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_StandardString_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *_100;
    QLineEdit *_101;

    void setupUi(QDialog *StandardString_Dialog)
    {
        if (StandardString_Dialog->objectName().isEmpty())
            StandardString_Dialog->setObjectName(QString::fromUtf8("StandardString_Dialog"));
        StandardString_Dialog->resize(400, 90);
        StandardString_Dialog->setMinimumSize(QSize(400, 90));
        StandardString_Dialog->setMaximumSize(QSize(400, 80));
        QFont font;
        font.setPointSize(10);
        StandardString_Dialog->setFont(font);
        _2 = new QPushButton(StandardString_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(304, 44, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(StandardString_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(214, 44, 75, 23));
        _1->setFont(font);
        _100 = new QLabel(StandardString_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 10, 201, 20));
        _100->setFont(font);
        _101 = new QLineEdit(StandardString_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(132, 12, 247, 20));
        QWidget::setTabOrder(_1, _2);

        retranslateUi(StandardString_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(StandardString_Dialog);
    } // setupUi

    void retranslateUi(QDialog *StandardString_Dialog)
    {
        StandardString_Dialog->setWindowTitle(QCoreApplication::translate("StandardString_Dialog", "TITLE", nullptr));
        _2->setText(QCoreApplication::translate("StandardString_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("StandardString_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("StandardString_Dialog", "Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StandardString_Dialog: public Ui_StandardString_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STANDARDSTRING_DIALOG_H
