/********************************************************************************
** Form generated from reading UI file 'TRACE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACE_DIALOG_H
#define UI_TRACE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Trace_Dialog
{
public:
    QLabel *label;
    QPushButton *_7;
    QPushButton *_6;
    QPushButton *_2;

    void setupUi(QDialog *Trace_Dialog)
    {
        if (Trace_Dialog->objectName().isEmpty())
            Trace_Dialog->setObjectName(QString::fromUtf8("Trace_Dialog"));
        Trace_Dialog->resize(310, 74);
        Trace_Dialog->setMinimumSize(QSize(310, 74));
        Trace_Dialog->setMaximumSize(QSize(310, 74));
        label = new QLabel(Trace_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 291, 20));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        _7 = new QPushButton(Trace_Dialog);
        _7->setObjectName(QString::fromUtf8("_7"));
        _7->setGeometry(QRect(122, 40, 75, 23));
        _7->setFont(font);
        _7->setAutoDefault(false);
        _6 = new QPushButton(Trace_Dialog);
        _6->setObjectName(QString::fromUtf8("_6"));
        _6->setGeometry(QRect(32, 40, 75, 23));
        _6->setFont(font);
        _6->setAutoDefault(true);
        _2 = new QPushButton(Trace_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(210, 40, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        QWidget::setTabOrder(_6, _7);
        QWidget::setTabOrder(_7, _2);

        retranslateUi(Trace_Dialog);

        _6->setDefault(true);


        QMetaObject::connectSlotsByName(Trace_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Trace_Dialog)
    {
        Trace_Dialog->setWindowTitle(QCoreApplication::translate("Trace_Dialog", "Trace connected entities", nullptr));
        label->setText(QCoreApplication::translate("Trace_Dialog", "Several paths located - Trace this path?", nullptr));
        _7->setText(QCoreApplication::translate("Trace_Dialog", "No", nullptr));
        _6->setText(QCoreApplication::translate("Trace_Dialog", "Yes", nullptr));
        _2->setText(QCoreApplication::translate("Trace_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Trace_Dialog: public Ui_Trace_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACE_DIALOG_H
