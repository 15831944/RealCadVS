/********************************************************************************
** Form generated from reading UI file 'TEXTINSERT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTINSERT_DIALOG_H
#define UI_TEXTINSERT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_TextInsert_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *_100;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    RCDoubleSpinBox *_101;
    QLabel *label_102;
    RCDoubleSpinBox *_102;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QCheckBox *_103;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *TextInsert_Dialog)
    {
        if (TextInsert_Dialog->objectName().isEmpty())
            TextInsert_Dialog->setObjectName(QString::fromUtf8("TextInsert_Dialog"));
        TextInsert_Dialog->resize(453, 331);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextInsert_Dialog->sizePolicy().hasHeightForWidth());
        TextInsert_Dialog->setSizePolicy(sizePolicy);
        TextInsert_Dialog->setMinimumSize(QSize(453, 331));
        verticalLayout = new QVBoxLayout(TextInsert_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _100 = new QTextEdit(TextInsert_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        QFont font;
        font.setPointSize(10);
        _100->setFont(font);
        _100->setAutoFillBackground(false);
        _100->setFrameShape(QFrame::Box);
        _100->setFrameShadow(QFrame::Sunken);
        _100->setAutoFormatting(QTextEdit::AutoNone);
        _100->setTabChangesFocus(true);
        _100->setLineWrapMode(QTextEdit::NoWrap);
        _100->setAcceptRichText(true);
        _100->setTextInteractionFlags(Qt::TextEditorInteraction);

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(TextInsert_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_4, 0, Qt::AlignRight);

        _101 = new RCDoubleSpinBox(TextInsert_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        sizePolicy1.setHeightForWidth(_101->sizePolicy().hasHeightForWidth());
        _101->setSizePolicy(sizePolicy1);
        _101->setMinimumSize(QSize(101, 24));
        _101->setMaximumSize(QSize(101, 24));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _101->setFont(font1);
        _101->setMaximum(100000000.000000000000000);

        horizontalLayout->addWidget(_101);

        label_102 = new QLabel(TextInsert_Dialog);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        sizePolicy1.setHeightForWidth(label_102->sizePolicy().hasHeightForWidth());
        label_102->setSizePolicy(sizePolicy1);
        label_102->setFont(font);
        label_102->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_102, 0, Qt::AlignRight);

        _102 = new RCDoubleSpinBox(TextInsert_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        sizePolicy1.setHeightForWidth(_102->sizePolicy().hasHeightForWidth());
        _102->setSizePolicy(sizePolicy1);
        _102->setMinimumSize(QSize(101, 24));
        _102->setMaximumSize(QSize(101, 24));
        _102->setFont(font1);
        _102->setWrapping(true);
        _102->setMinimum(0.000000000000000);
        _102->setMaximum(360.000000000000000);

        horizontalLayout->addWidget(_102);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        _103 = new QCheckBox(TextInsert_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setFont(font);
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);

        horizontalLayout_2->addWidget(_103);

        horizontalSpacer_2 = new QSpacerItem(150, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        _1 = new QPushButton(TextInsert_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setMinimumSize(QSize(81, 32));
        _1->setMaximumSize(QSize(81, 32));
        _1->setFont(font);

        horizontalLayout_3->addWidget(_1);

        _2 = new QPushButton(TextInsert_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setMinimumSize(QSize(81, 32));
        _2->setMaximumSize(QSize(81, 32));
        _2->setFont(font);

        horizontalLayout_3->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout_3);

        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(TextInsert_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(TextInsert_Dialog);
    } // setupUi

    void retranslateUi(QDialog *TextInsert_Dialog)
    {
        TextInsert_Dialog->setWindowTitle(QCoreApplication::translate("TextInsert_Dialog", "Insert Text", nullptr));
        label_4->setText(QCoreApplication::translate("TextInsert_Dialog", "Height", nullptr));
        label_102->setText(QCoreApplication::translate("TextInsert_Dialog", "Angle", nullptr));
        _103->setText(QCoreApplication::translate("TextInsert_Dialog", "Text height in points ", nullptr));
        _1->setText(QCoreApplication::translate("TextInsert_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("TextInsert_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextInsert_Dialog: public Ui_TextInsert_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTINSERT_DIALOG_H
