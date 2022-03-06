/********************************************************************************
** Form generated from reading UI file 'ATTRIBUTENEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTENEW_DIALOG_H
#define UI_ATTRIBUTENEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_AttributeNew_Dialog
{
public:
    QTextEdit *_100;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *AttributeNew_Dialog)
    {
        if (AttributeNew_Dialog->objectName().isEmpty())
            AttributeNew_Dialog->setObjectName(QString::fromUtf8("AttributeNew_Dialog"));
        AttributeNew_Dialog->resize(371, 240);
        AttributeNew_Dialog->setMinimumSize(QSize(371, 240));
        AttributeNew_Dialog->setMaximumSize(QSize(371, 240));
        _100 = new QTextEdit(AttributeNew_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(12, 17, 347, 163));
        QFont font;
        font.setFamily(QString::fromUtf8("Lucida Grande"));
        font.setPointSize(10);
        _100->setFont(font);
        _100->setAutoFillBackground(false);
        _100->setFrameShape(QFrame::Box);
        _100->setFrameShadow(QFrame::Sunken);
        _100->setLineWrapMode(QTextEdit::NoWrap);
        _100->setAcceptRichText(false);
        _1 = new QPushButton(AttributeNew_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(180, 190, 75, 23));
        _1->setFont(font);
        _2 = new QPushButton(AttributeNew_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(270, 190, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        QWidget::setTabOrder(_100, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(AttributeNew_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(AttributeNew_Dialog);
    } // setupUi

    void retranslateUi(QDialog *AttributeNew_Dialog)
    {
        AttributeNew_Dialog->setWindowTitle(QCoreApplication::translate("AttributeNew_Dialog", "New attribute", nullptr));
        _1->setText(QCoreApplication::translate("AttributeNew_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("AttributeNew_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AttributeNew_Dialog: public Ui_AttributeNew_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTENEW_DIALOG_H
