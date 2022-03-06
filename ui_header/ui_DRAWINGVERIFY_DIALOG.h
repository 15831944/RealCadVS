/********************************************************************************
** Form generated from reading UI file 'DRAWINGVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWINGVERIFY_DIALOG_H
#define UI_DRAWINGVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DrawingVerify_Dialog
{
public:
    QLabel *label_53;
    QLineEdit *_101;
    QLabel *label_54;
    QPushButton *_104;
    QPushButton *_1;
    QLabel *label_56;
    QLineEdit *_100;
    QLabel *label_55;
    QLineEdit *_102;
    QLineEdit *_103;

    void setupUi(QDialog *DrawingVerify_Dialog)
    {
        if (DrawingVerify_Dialog->objectName().isEmpty())
            DrawingVerify_Dialog->setObjectName(QString::fromUtf8("DrawingVerify_Dialog"));
        DrawingVerify_Dialog->resize(261, 162);
        DrawingVerify_Dialog->setMinimumSize(QSize(261, 162));
        DrawingVerify_Dialog->setMaximumSize(QSize(261, 162));
        QFont font;
        font.setPointSize(10);
        DrawingVerify_Dialog->setFont(font);
        label_53 = new QLabel(DrawingVerify_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(20, 40, 59, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        label_53->setFont(font1);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QLineEdit(DrawingVerify_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(90, 40, 147, 20));
        _101->setFont(font1);
        _101->setReadOnly(true);
        label_54 = new QLabel(DrawingVerify_Dialog);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(20, 70, 57, 20));
        label_54->setFont(font1);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QPushButton(DrawingVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(142, 130, 75, 23));
        _104->setFont(font1);
        _104->setAutoDefault(false);
        _1 = new QPushButton(DrawingVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(52, 130, 75, 23));
        _1->setFont(font1);
        label_56 = new QLabel(DrawingVerify_Dialog);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(20, 100, 59, 20));
        label_56->setFont(font1);
        label_56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QLineEdit(DrawingVerify_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(90, 10, 147, 20));
        _100->setFont(font1);
        _100->setReadOnly(true);
        label_55 = new QLabel(DrawingVerify_Dialog);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(20, 10, 59, 20));
        label_55->setFont(font1);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QLineEdit(DrawingVerify_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(90, 70, 147, 21));
        _102->setFont(font1);
        _102->setReadOnly(true);
        _103 = new QLineEdit(DrawingVerify_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(90, 100, 147, 21));
        _103->setFont(font1);
        _103->setReadOnly(true);

        retranslateUi(DrawingVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DrawingVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DrawingVerify_Dialog)
    {
        DrawingVerify_Dialog->setWindowTitle(QCoreApplication::translate("DrawingVerify_Dialog", "Layout drawing verify", nullptr));
        label_53->setText(QCoreApplication::translate("DrawingVerify_Dialog", "Paper size", nullptr));
        label_54->setText(QCoreApplication::translate("DrawingVerify_Dialog", "Width", nullptr));
        _104->setText(QCoreApplication::translate("DrawingVerify_Dialog", "Change", nullptr));
        _1->setText(QCoreApplication::translate("DrawingVerify_Dialog", "OK", nullptr));
        label_56->setText(QCoreApplication::translate("DrawingVerify_Dialog", "Height", nullptr));
        label_55->setText(QCoreApplication::translate("DrawingVerify_Dialog", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DrawingVerify_Dialog: public Ui_DrawingVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWINGVERIFY_DIALOG_H
