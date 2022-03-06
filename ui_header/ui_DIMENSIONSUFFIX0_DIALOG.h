/********************************************************************************
** Form generated from reading UI file 'DIMENSIONSUFFIX0_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONSUFFIX0_DIALOG_H
#define UI_DIMENSIONSUFFIX0_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DimensionSuffix0_Dialog
{
public:
    QPushButton *_1;
    QGraphicsView *_103;
    QPushButton *_101;
    QCheckBox *_100;
    QPushButton *_2;
    QPlainTextEdit *_102;
    QLabel *SuffixLabel;
    QCheckBox *_104;

    void setupUi(QDialog *DimensionSuffix0_Dialog)
    {
        if (DimensionSuffix0_Dialog->objectName().isEmpty())
            DimensionSuffix0_Dialog->setObjectName(QString::fromUtf8("DimensionSuffix0_Dialog"));
        DimensionSuffix0_Dialog->resize(381, 278);
        QFont font;
        font.setPointSize(10);
        DimensionSuffix0_Dialog->setFont(font);
        _1 = new QPushButton(DimensionSuffix0_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(184, 248, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        _1->setFont(font1);
        _103 = new QGraphicsView(DimensionSuffix0_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(10, 84, 361, 161));
        _101 = new QPushButton(DimensionSuffix0_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(296, 6, 80, 23));
        _101->setFont(font1);
        _100 = new QCheckBox(DimensionSuffix0_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(8, 10, 283, 21));
        _100->setFont(font1);
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _2 = new QPushButton(DimensionSuffix0_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(266, 248, 75, 23));
        _2->setFont(font1);
        _102 = new QPlainTextEdit(DimensionSuffix0_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(74, 38, 297, 43));
        SuffixLabel = new QLabel(DimensionSuffix0_Dialog);
        SuffixLabel->setObjectName(QString::fromUtf8("SuffixLabel"));
        SuffixLabel->setEnabled(true);
        SuffixLabel->setGeometry(QRect(10, 40, 57, 16));
        SuffixLabel->setFont(font1);
        SuffixLabel->setLayoutDirection(Qt::LeftToRight);
        SuffixLabel->setAlignment(Qt::AlignCenter);
        _104 = new QCheckBox(DimensionSuffix0_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(14, 38, 57, 20));
        _104->setFont(font1);
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setTristate(false);
        SuffixLabel->raise();
        _104->raise();
        _1->raise();
        _103->raise();
        _101->raise();
        _100->raise();
        _2->raise();
        _102->raise();
        QWidget::setTabOrder(_102, _100);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _103);

        retranslateUi(DimensionSuffix0_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionSuffix0_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionSuffix0_Dialog)
    {
        DimensionSuffix0_Dialog->setWindowTitle(QCoreApplication::translate("DimensionSuffix0_Dialog", "Dimension suffix options", nullptr));
        _1->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "OK", nullptr));
        _101->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "Text options", nullptr));
        _100->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "Use value text options for suffix text options", nullptr));
        _2->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "Cancel", nullptr));
        _102->setPlainText(QString());
        SuffixLabel->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "Suffix", nullptr));
        _104->setText(QCoreApplication::translate("DimensionSuffix0_Dialog", "Suffix", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DimensionSuffix0_Dialog: public Ui_DimensionSuffix0_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONSUFFIX0_DIALOG_H
