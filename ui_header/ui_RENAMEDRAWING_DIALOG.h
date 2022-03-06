/********************************************************************************
** Form generated from reading UI file 'RENAMEDRAWING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEDRAWING_DIALOG_H
#define UI_RENAMEDRAWING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_RenameDrawing_Dialog
{
public:
    QLabel *label;
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_8;
    QLineEdit *_101;
    RCComboList *_100;

    void setupUi(QDialog *RenameDrawing_Dialog)
    {
        if (RenameDrawing_Dialog->objectName().isEmpty())
            RenameDrawing_Dialog->setObjectName(QString::fromUtf8("RenameDrawing_Dialog"));
        RenameDrawing_Dialog->resize(278, 271);
        RenameDrawing_Dialog->setMinimumSize(QSize(278, 271));
        RenameDrawing_Dialog->setMaximumSize(QSize(278, 271));
        QFont font;
        font.setPointSize(10);
        RenameDrawing_Dialog->setFont(font);
        label = new QLabel(RenameDrawing_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(24, 10, 251, 20));
        label->setFont(font);
        _2 = new QPushButton(RenameDrawing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(181, 230, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _1 = new QPushButton(RenameDrawing_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(102, 230, 75, 23));
        _1->setFont(font1);
        label_8 = new QLabel(RenameDrawing_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(26, 180, 231, 20));
        label_8->setFont(font);
        _101 = new QLineEdit(RenameDrawing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(24, 200, 231, 23));
        _100 = new RCComboList(RenameDrawing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(24, 31, 231, 149));
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(RenameDrawing_Dialog);

        QMetaObject::connectSlotsByName(RenameDrawing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *RenameDrawing_Dialog)
    {
        RenameDrawing_Dialog->setWindowTitle(QCoreApplication::translate("RenameDrawing_Dialog", "Rename layout drawing", nullptr));
        label->setText(QCoreApplication::translate("RenameDrawing_Dialog", "Select the layout drawing to rename", nullptr));
        _2->setText(QCoreApplication::translate("RenameDrawing_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("RenameDrawing_Dialog", "OK", nullptr));
        label_8->setText(QCoreApplication::translate("RenameDrawing_Dialog", "New drawing name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RenameDrawing_Dialog: public Ui_RenameDrawing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEDRAWING_DIALOG_H
