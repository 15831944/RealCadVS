/********************************************************************************
** Form generated from reading UI file 'ATTRIBUTELIST_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTELIST_DIALOG_H
#define UI_ATTRIBUTELIST_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_AttributeList_Dialog
{
public:
    QTextEdit *_101;
    QPushButton *_102;
    QPushButton *_103;
    QPushButton *_2;
    QLabel *_100;

    void setupUi(QDialog *AttributeList_Dialog)
    {
        if (AttributeList_Dialog->objectName().isEmpty())
            AttributeList_Dialog->setObjectName(QString::fromUtf8("AttributeList_Dialog"));
        AttributeList_Dialog->resize(371, 260);
        AttributeList_Dialog->setMinimumSize(QSize(371, 260));
        AttributeList_Dialog->setMaximumSize(QSize(371, 260));
        _101 = new QTextEdit(AttributeList_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(12, 42, 347, 163));
        QFont font;
        font.setFamily(QString::fromUtf8("Lucida Grande"));
        font.setPointSize(10);
        _101->setFont(font);
        _101->setAutoFillBackground(false);
        _101->setFrameShape(QFrame::Box);
        _101->setFrameShadow(QFrame::Sunken);
        _101->setLineWrapMode(QTextEdit::NoWrap);
        _101->setReadOnly(true);
        _101->setAcceptRichText(false);
        _102 = new QPushButton(AttributeList_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(20, 214, 61, 23));
        _102->setFont(font);
        _102->setContextMenuPolicy(Qt::DefaultContextMenu);
        _102->setAutoDefault(false);
        _103 = new QPushButton(AttributeList_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(110, 214, 61, 23));
        _103->setFont(font);
        _103->setContextMenuPolicy(Qt::DefaultContextMenu);
        _103->setAutoDefault(false);
        _2 = new QPushButton(AttributeList_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(290, 214, 61, 23));
        _2->setFont(font);
        _100 = new QLabel(AttributeList_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(60, 12, 261, 20));
        QFont font1;
        font1.setPointSize(10);
        _100->setFont(font1);
        _100->setAlignment(Qt::AlignCenter);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _2);
        QWidget::setTabOrder(_2, _101);

        retranslateUi(AttributeList_Dialog);

        _2->setDefault(true);


        QMetaObject::connectSlotsByName(AttributeList_Dialog);
    } // setupUi

    void retranslateUi(QDialog *AttributeList_Dialog)
    {
        AttributeList_Dialog->setWindowTitle(QCoreApplication::translate("AttributeList_Dialog", "List attribute", nullptr));
        _102->setText(QCoreApplication::translate("AttributeList_Dialog", "Next", nullptr));
        _103->setText(QCoreApplication::translate("AttributeList_Dialog", "Last", nullptr));
        _2->setText(QCoreApplication::translate("AttributeList_Dialog", "Exit", nullptr));
        _100->setText(QCoreApplication::translate("AttributeList_Dialog", "Attribute %d of %d", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AttributeList_Dialog: public Ui_AttributeList_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTELIST_DIALOG_H
