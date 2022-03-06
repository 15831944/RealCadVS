/********************************************************************************
** Form generated from reading UI file 'FIGURETEXT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGURETEXT_DIALOG_H
#define UI_FIGURETEXT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FigureText_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_3;
    QLineEdit *_100;
    QLineEdit *_101;
    QLabel *label_4;

    void setupUi(QDialog *FigureText_Dialog)
    {
        if (FigureText_Dialog->objectName().isEmpty())
            FigureText_Dialog->setObjectName(QString::fromUtf8("FigureText_Dialog"));
        FigureText_Dialog->resize(501, 100);
        QFont font;
        font.setPointSize(10);
        FigureText_Dialog->setFont(font);
        _2 = new QPushButton(FigureText_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(410, 70, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(FigureText_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(320, 70, 75, 23));
        label_3 = new QLabel(FigureText_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(17, 10, 111, 21));
        _100 = new QLineEdit(FigureText_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(130, 10, 351, 21));
        _101 = new QLineEdit(FigureText_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(130, 40, 351, 21));
        label_4 = new QLabel(FigureText_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 40, 111, 21));

        retranslateUi(FigureText_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(FigureText_Dialog);
    } // setupUi

    void retranslateUi(QDialog *FigureText_Dialog)
    {
        FigureText_Dialog->setWindowTitle(QCoreApplication::translate("FigureText_Dialog", "Insert component annotation text", nullptr));
        _2->setText(QCoreApplication::translate("FigureText_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("FigureText_Dialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("FigureText_Dialog", "Prompt", nullptr));
        label_4->setText(QCoreApplication::translate("FigureText_Dialog", "Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FigureText_Dialog: public Ui_FigureText_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGURETEXT_DIALOG_H
