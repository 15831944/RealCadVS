/********************************************************************************
** Form generated from reading UI file 'ATTRIBUTEDELETE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTEDELETE_DIALOG_H
#define UI_ATTRIBUTEDELETE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_AttributeDelete_Dialog
{
public:
    QLabel *_100;
    QPushButton *_2;
    QPushButton *_104;
    QPushButton *_102;
    QPushButton *_103;
    QTextEdit *_101;

    void setupUi(QDialog *AttributeDelete_Dialog)
    {
        if (AttributeDelete_Dialog->objectName().isEmpty())
            AttributeDelete_Dialog->setObjectName(QString::fromUtf8("AttributeDelete_Dialog"));
        AttributeDelete_Dialog->resize(371, 260);
        AttributeDelete_Dialog->setMinimumSize(QSize(371, 260));
        AttributeDelete_Dialog->setMaximumSize(QSize(371, 260));
        QFont font;
        font.setPointSize(10);
        AttributeDelete_Dialog->setFont(font);
        _100 = new QLabel(AttributeDelete_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 10, 331, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _100->setFont(font1);
        _100->setAlignment(Qt::AlignCenter);
        _2 = new QPushButton(AttributeDelete_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(280, 212, 75, 23));
        _2->setFont(font1);
        _104 = new QPushButton(AttributeDelete_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(190, 212, 75, 23));
        _104->setFont(font1);
        _104->setAutoDefault(false);
        _102 = new QPushButton(AttributeDelete_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(24, 212, 61, 23));
        _102->setFont(font1);
        _102->setAutoDefault(false);
        _103 = new QPushButton(AttributeDelete_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(100, 212, 61, 23));
        _103->setFont(font1);
        _103->setAutoDefault(false);
        _101 = new QTextEdit(AttributeDelete_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(12, 40, 347, 163));
        _101->setFont(font1);
        _101->setAutoFillBackground(false);
        _101->setFrameShape(QFrame::Box);
        _101->setFrameShadow(QFrame::Sunken);
        _101->setLineWrapMode(QTextEdit::NoWrap);
        _101->setReadOnly(true);
        _101->setAcceptRichText(false);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _2);
        QWidget::setTabOrder(_2, _101);

        retranslateUi(AttributeDelete_Dialog);

        _2->setDefault(true);


        QMetaObject::connectSlotsByName(AttributeDelete_Dialog);
    } // setupUi

    void retranslateUi(QDialog *AttributeDelete_Dialog)
    {
        AttributeDelete_Dialog->setWindowTitle(QCoreApplication::translate("AttributeDelete_Dialog", "Delete attribute", nullptr));
        _100->setText(QCoreApplication::translate("AttributeDelete_Dialog", "Attribute %d of %d", nullptr));
        _2->setText(QCoreApplication::translate("AttributeDelete_Dialog", "Exit", nullptr));
        _104->setText(QCoreApplication::translate("AttributeDelete_Dialog", "Delete", nullptr));
        _102->setText(QCoreApplication::translate("AttributeDelete_Dialog", "Next", nullptr));
        _103->setText(QCoreApplication::translate("AttributeDelete_Dialog", "Last", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AttributeDelete_Dialog: public Ui_AttributeDelete_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTEDELETE_DIALOG_H
