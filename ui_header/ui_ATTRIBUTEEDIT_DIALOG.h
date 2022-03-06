/********************************************************************************
** Form generated from reading UI file 'ATTRIBUTEEDIT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTEEDIT_DIALOG_H
#define UI_ATTRIBUTEEDIT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_AttributeEdit_Dialog
{
public:
    QTextEdit *_101;
    QPushButton *_102;
    QPushButton *_103;
    QPushButton *_1;
    QPushButton *_2;
    QLabel *_100;

    void setupUi(QDialog *AttributeEdit_Dialog)
    {
        if (AttributeEdit_Dialog->objectName().isEmpty())
            AttributeEdit_Dialog->setObjectName(QString::fromUtf8("AttributeEdit_Dialog"));
        AttributeEdit_Dialog->resize(371, 260);
        AttributeEdit_Dialog->setMinimumSize(QSize(371, 260));
        AttributeEdit_Dialog->setMaximumSize(QSize(371, 260));
        _101 = new QTextEdit(AttributeEdit_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(12, 38, 347, 161));
        QFont font;
        font.setFamily(QString::fromUtf8("Lucida Grande"));
        font.setPointSize(10);
        _101->setFont(font);
        _101->setAutoFillBackground(false);
        _101->setFrameShape(QFrame::Box);
        _101->setFrameShadow(QFrame::Sunken);
        _101->setLineWrapMode(QTextEdit::NoWrap);
        _101->setAcceptRichText(false);
        _102 = new QPushButton(AttributeEdit_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(16, 210, 61, 23));
        _102->setFont(font);
        _102->setContextMenuPolicy(Qt::DefaultContextMenu);
        _102->setAutoDefault(false);
        _103 = new QPushButton(AttributeEdit_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(108, 210, 61, 23));
        _103->setFont(font);
        _103->setContextMenuPolicy(Qt::DefaultContextMenu);
        _103->setAutoDefault(false);
        _1 = new QPushButton(AttributeEdit_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(198, 210, 71, 23));
        _1->setFont(font);
        _1->setContextMenuPolicy(Qt::DefaultContextMenu);
        _1->setAutoDefault(false);
        _2 = new QPushButton(AttributeEdit_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(292, 210, 61, 23));
        _2->setFont(font);
        _100 = new QLabel(AttributeEdit_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(60, 10, 261, 20));
        _100->setFont(font);
        _100->setAlignment(Qt::AlignCenter);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _101);

        retranslateUi(AttributeEdit_Dialog);

        _2->setDefault(true);


        QMetaObject::connectSlotsByName(AttributeEdit_Dialog);
    } // setupUi

    void retranslateUi(QDialog *AttributeEdit_Dialog)
    {
        AttributeEdit_Dialog->setWindowTitle(QCoreApplication::translate("AttributeEdit_Dialog", "Edit attribute", nullptr));
        _102->setText(QCoreApplication::translate("AttributeEdit_Dialog", "Next", nullptr));
        _103->setText(QCoreApplication::translate("AttributeEdit_Dialog", "Last", nullptr));
        _1->setText(QCoreApplication::translate("AttributeEdit_Dialog", "Change", nullptr));
        _2->setText(QCoreApplication::translate("AttributeEdit_Dialog", "Exit", nullptr));
        _100->setText(QCoreApplication::translate("AttributeEdit_Dialog", "Attribute %d of %d", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AttributeEdit_Dialog: public Ui_AttributeEdit_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTEEDIT_DIALOG_H
