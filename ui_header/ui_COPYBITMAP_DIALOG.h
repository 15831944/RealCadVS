/********************************************************************************
** Form generated from reading UI file 'COPYBITMAP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYBITMAP_DIALOG_H
#define UI_COPYBITMAP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CopyBitmap_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QComboBox *_100;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *_101;
    QComboBox *_102;
    QLabel *label_6;

    void setupUi(QDialog *CopyBitmap_Dialog)
    {
        if (CopyBitmap_Dialog->objectName().isEmpty())
            CopyBitmap_Dialog->setObjectName(QString::fromUtf8("CopyBitmap_Dialog"));
        CopyBitmap_Dialog->resize(303, 170);
        CopyBitmap_Dialog->setMinimumSize(QSize(303, 170));
        CopyBitmap_Dialog->setMaximumSize(QSize(303, 170));
        QFont font;
        font.setPointSize(10);
        CopyBitmap_Dialog->setFont(font);
        _2 = new QPushButton(CopyBitmap_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(216, 120, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CopyBitmap_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(126, 120, 75, 23));
        _100 = new QComboBox(CopyBitmap_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(150, 20, 141, 22));
        label_4 = new QLabel(CopyBitmap_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 20, 111, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(CopyBitmap_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 111, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QComboBox(CopyBitmap_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(150, 50, 141, 22));
        _102 = new QComboBox(CopyBitmap_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(150, 80, 141, 22));
        label_6 = new QLabel(CopyBitmap_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 80, 111, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        retranslateUi(CopyBitmap_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CopyBitmap_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CopyBitmap_Dialog)
    {
        CopyBitmap_Dialog->setWindowTitle(QCoreApplication::translate("CopyBitmap_Dialog", "Copy bitmap options", nullptr));
        _2->setText(QCoreApplication::translate("CopyBitmap_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CopyBitmap_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("CopyBitmap_Dialog", "Bitmap size", nullptr));
        label_5->setText(QCoreApplication::translate("CopyBitmap_Dialog", "Copy format", nullptr));
        label_6->setText(QCoreApplication::translate("CopyBitmap_Dialog", "Quality", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CopyBitmap_Dialog: public Ui_CopyBitmap_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYBITMAP_DIALOG_H
