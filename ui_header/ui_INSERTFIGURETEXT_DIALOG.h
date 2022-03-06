/********************************************************************************
** Form generated from reading UI file 'INSERTFIGURETEXT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSERTFIGURETEXT_DIALOG_H
#define UI_INSERTFIGURETEXT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_InsertFigureText_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *_100;
    QTextEdit *_101;

    void setupUi(QDialog *InsertFigureText_Dialog)
    {
        if (InsertFigureText_Dialog->objectName().isEmpty())
            InsertFigureText_Dialog->setObjectName(QString::fromUtf8("InsertFigureText_Dialog"));
        InsertFigureText_Dialog->resize(407, 246);
        InsertFigureText_Dialog->setMinimumSize(QSize(407, 246));
        InsertFigureText_Dialog->setMaximumSize(QSize(407, 246));
        QFont font;
        font.setPointSize(10);
        InsertFigureText_Dialog->setFont(font);
        _2 = new QPushButton(InsertFigureText_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(322, 210, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(InsertFigureText_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(236, 210, 75, 23));
        _100 = new QLabel(InsertFigureText_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(12, 10, 383, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(12);
        _100->setFont(font1);
        _100->setAlignment(Qt::AlignCenter);
        _101 = new QTextEdit(InsertFigureText_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(12, 31, 381, 172));
        _101->setFont(font);
        _101->setAutoFillBackground(false);
        _101->setFrameShape(QFrame::Box);
        _101->setFrameShadow(QFrame::Sunken);
        _101->setAutoFormatting(QTextEdit::AutoNone);
        _101->setTabChangesFocus(true);
        _101->setLineWrapMode(QTextEdit::NoWrap);
        _101->setAcceptRichText(true);
        _101->setTextInteractionFlags(Qt::TextEditorInteraction);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(InsertFigureText_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(InsertFigureText_Dialog);
    } // setupUi

    void retranslateUi(QDialog *InsertFigureText_Dialog)
    {
        InsertFigureText_Dialog->setWindowTitle(QCoreApplication::translate("InsertFigureText_Dialog", "Insert component annotation text", nullptr));
        _2->setText(QCoreApplication::translate("InsertFigureText_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("InsertFigureText_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("InsertFigureText_Dialog", "Text", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InsertFigureText_Dialog: public Ui_InsertFigureText_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSERTFIGURETEXT_DIALOG_H
