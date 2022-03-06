/********************************************************************************
** Form generated from reading UI file 'KEYBOARDSHORTCUTS_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYBOARDSHORTCUTS_DIALOG_H
#define UI_KEYBOARDSHORTCUTS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_KeyboardShortcuts_Dialog
{
public:
    QLabel *label;
    QListWidget *_100;
    QComboBox *_101;
    QLabel *label_2;
    QCheckBox *_102;
    QCheckBox *_103;
    QPushButton *_1;
    QPushButton *_2;
    QPushButton *_104;

    void setupUi(QDialog *KeyboardShortcuts_Dialog)
    {
        if (KeyboardShortcuts_Dialog->objectName().isEmpty())
            KeyboardShortcuts_Dialog->setObjectName(QString::fromUtf8("KeyboardShortcuts_Dialog"));
        KeyboardShortcuts_Dialog->resize(449, 292);
        KeyboardShortcuts_Dialog->setMinimumSize(QSize(449, 292));
        KeyboardShortcuts_Dialog->setMaximumSize(QSize(449, 292));
        QFont font;
        font.setPointSize(10);
        KeyboardShortcuts_Dialog->setFont(font);
        label = new QLabel(KeyboardShortcuts_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 121, 20));
        label->setFont(font);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QListWidget(KeyboardShortcuts_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 30, 411, 181));
        _101 = new QComboBox(KeyboardShortcuts_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(70, 230, 91, 22));
        label_2 = new QLabel(KeyboardShortcuts_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 230, 41, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QCheckBox(KeyboardShortcuts_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(190, 230, 51, 20));
        _102->setLayoutDirection(Qt::LeftToRight);
        _103 = new QCheckBox(KeyboardShortcuts_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(260, 230, 51, 20));
        _103->setLayoutDirection(Qt::LeftToRight);
        _1 = new QPushButton(KeyboardShortcuts_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(350, 230, 75, 23));
        _2 = new QPushButton(KeyboardShortcuts_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(350, 260, 75, 23));
        _104 = new QPushButton(KeyboardShortcuts_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(260, 260, 75, 23));

        retranslateUi(KeyboardShortcuts_Dialog);

        QMetaObject::connectSlotsByName(KeyboardShortcuts_Dialog);
    } // setupUi

    void retranslateUi(QDialog *KeyboardShortcuts_Dialog)
    {
        KeyboardShortcuts_Dialog->setWindowTitle(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Keyboard shortcuts", nullptr));
        label->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Existing shortcuts:", nullptr));
        label_2->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Key", nullptr));
        _102->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Ctrl", nullptr));
        _103->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Alt", nullptr));
        _1->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Add", nullptr));
        _2->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Close", nullptr));
        _104->setText(QCoreApplication::translate("KeyboardShortcuts_Dialog", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KeyboardShortcuts_Dialog: public Ui_KeyboardShortcuts_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYBOARDSHORTCUTS_DIALOG_H
