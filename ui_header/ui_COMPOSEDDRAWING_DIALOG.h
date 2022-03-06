/********************************************************************************
** Form generated from reading UI file 'COMPOSEDDRAWING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPOSEDDRAWING_DIALOG_H
#define UI_COMPOSEDDRAWING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_ComposedDrawing_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLineEdit *_100;
    QLabel *label_42;
    QLabel *label_43;
    QComboBox *_101;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *_104;
    QPushButton *_105;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;

    void setupUi(QDialog *ComposedDrawing_Dialog)
    {
        if (ComposedDrawing_Dialog->objectName().isEmpty())
            ComposedDrawing_Dialog->setObjectName(QString::fromUtf8("ComposedDrawing_Dialog"));
        ComposedDrawing_Dialog->resize(386, 200);
        ComposedDrawing_Dialog->setMinimumSize(QSize(386, 200));
        ComposedDrawing_Dialog->setMaximumSize(QSize(386, 200));
        QFont font;
        font.setPointSize(10);
        ComposedDrawing_Dialog->setFont(font);
        _2 = new QPushButton(ComposedDrawing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(300, 164, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(ComposedDrawing_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(210, 164, 75, 23));
        _1->setFont(font1);
        _100 = new QLineEdit(ComposedDrawing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(92, 10, 245, 20));
        label_42 = new QLabel(ComposedDrawing_Dialog);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(18, 10, 71, 20));
        label_42->setFont(font);
        label_42->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_43 = new QLabel(ComposedDrawing_Dialog);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(16, 34, 71, 20));
        label_43->setFont(font);
        label_43->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QComboBox(ComposedDrawing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(90, 34, 247, 22));
        label_3 = new QLabel(ComposedDrawing_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 72, 57, 21));
        label_3->setFont(font1);
        label_4 = new QLabel(ComposedDrawing_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 102, 57, 21));
        label_4->setFont(font1);
        label_5 = new QLabel(ComposedDrawing_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 130, 61, 21));
        label_5->setFont(font1);
        _104 = new QLineEdit(ComposedDrawing_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(88, 130, 209, 21));
        _105 = new QPushButton(ComposedDrawing_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(300, 128, 75, 23));
        _105->setFont(font1);
        _105->setAutoDefault(false);
        _102 = new RCDoubleSpinBox(ComposedDrawing_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(88, 71, 119, 24));
        _102->setDecimals(6);
        _102->setMaximum(100000000.000000000000000);
        _103 = new RCDoubleSpinBox(ComposedDrawing_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(88, 100, 119, 24));
        _103->setDecimals(6);
        _103->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(ComposedDrawing_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(ComposedDrawing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ComposedDrawing_Dialog)
    {
        ComposedDrawing_Dialog->setWindowTitle(QCoreApplication::translate("ComposedDrawing_Dialog", "Layout drawing options", nullptr));
        _2->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "OK", nullptr));
        label_42->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Name", nullptr));
        label_43->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Paper size", nullptr));
        label_3->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Width", nullptr));
        label_4->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Height", nullptr));
        label_5->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Title block", nullptr));
        _105->setText(QCoreApplication::translate("ComposedDrawing_Dialog", "Browse", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ComposedDrawing_Dialog: public Ui_ComposedDrawing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPOSEDDRAWING_DIALOG_H
