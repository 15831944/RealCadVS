/********************************************************************************
** Form generated from reading UI file 'LTTEXTINSERT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LTTEXTINSERT_DIALOG_H
#define UI_LTTEXTINSERT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LTTextInsert_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *_100;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *LTTextInsert_Dialog)
    {
        if (LTTextInsert_Dialog->objectName().isEmpty())
            LTTextInsert_Dialog->setObjectName(QString::fromUtf8("LTTextInsert_Dialog"));
        LTTextInsert_Dialog->resize(461, 231);
        LTTextInsert_Dialog->setMinimumSize(QSize(461, 231));
        LTTextInsert_Dialog->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(10);
        LTTextInsert_Dialog->setFont(font);
        verticalLayout = new QVBoxLayout(LTTextInsert_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _100 = new QTextEdit(LTTextInsert_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(LTTextInsert_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setMinimumSize(QSize(81, 32));
        _1->setMaximumSize(QSize(81, 32));

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(LTTextInsert_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setMinimumSize(QSize(81, 32));
        _2->setMaximumSize(QSize(81, 32));

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LTTextInsert_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(LTTextInsert_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LTTextInsert_Dialog)
    {
        LTTextInsert_Dialog->setWindowTitle(QCoreApplication::translate("LTTextInsert_Dialog", "Insert text", nullptr));
        _1->setText(QCoreApplication::translate("LTTextInsert_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("LTTextInsert_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LTTextInsert_Dialog: public Ui_LTTextInsert_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LTTEXTINSERT_DIALOG_H
