/********************************************************************************
** Form generated from reading UI file 'DIMENSIONPREFIX0_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONPREFIX0_DIALOG_H
#define UI_DIMENSIONPREFIX0_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DimensionPrefix0_Dialog
{
public:
    QPushButton *_2;
    QCheckBox *_100;
    QPushButton *_101;
    QGraphicsView *_103;
    QPlainTextEdit *_102;
    QCheckBox *_104;
    QLabel *PrefixLabel;
    QPushButton *_1;

    void setupUi(QDialog *DimensionPrefix0_Dialog)
    {
        if (DimensionPrefix0_Dialog->objectName().isEmpty())
            DimensionPrefix0_Dialog->setObjectName(QString::fromUtf8("DimensionPrefix0_Dialog"));
        DimensionPrefix0_Dialog->resize(381, 278);
        QFont font;
        font.setPointSize(10);
        DimensionPrefix0_Dialog->setFont(font);
        _2 = new QPushButton(DimensionPrefix0_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(266, 248, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _100 = new QCheckBox(DimensionPrefix0_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(8, 10, 283, 21));
        _100->setFont(font1);
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _101 = new QPushButton(DimensionPrefix0_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(296, 6, 80, 23));
        _101->setFont(font1);
        _103 = new QGraphicsView(DimensionPrefix0_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(10, 84, 361, 161));
        _103->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _103->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _103->setSceneRect(QRectF(0, 0, 329, 161));
        _103->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _103->setTransformationAnchor(QGraphicsView::NoAnchor);
        _103->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _102 = new QPlainTextEdit(DimensionPrefix0_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(74, 38, 297, 43));
        _104 = new QCheckBox(DimensionPrefix0_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(14, 38, 57, 20));
        _104->setFont(font1);
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setTristate(false);
        PrefixLabel = new QLabel(DimensionPrefix0_Dialog);
        PrefixLabel->setObjectName(QString::fromUtf8("PrefixLabel"));
        PrefixLabel->setEnabled(true);
        PrefixLabel->setGeometry(QRect(10, 40, 57, 16));
        PrefixLabel->setFont(font1);
        PrefixLabel->setLayoutDirection(Qt::LeftToRight);
        PrefixLabel->setAlignment(Qt::AlignCenter);
        _1 = new QPushButton(DimensionPrefix0_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(184, 248, 75, 23));
        _1->setFont(font1);
        PrefixLabel->raise();
        _2->raise();
        _100->raise();
        _101->raise();
        _103->raise();
        _102->raise();
        _104->raise();
        _1->raise();
        QWidget::setTabOrder(_104, _102);
        QWidget::setTabOrder(_102, _100);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _103);

        retranslateUi(DimensionPrefix0_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionPrefix0_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionPrefix0_Dialog)
    {
        DimensionPrefix0_Dialog->setWindowTitle(QCoreApplication::translate("DimensionPrefix0_Dialog", "Dimension prefix options", nullptr));
        _2->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "Cancel", nullptr));
        _100->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "Use value text options for prefix text options", nullptr));
        _101->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "Text options", nullptr));
        _102->setPlainText(QString());
        _104->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "Prefix", nullptr));
        PrefixLabel->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "Prefix", nullptr));
        _1->setText(QCoreApplication::translate("DimensionPrefix0_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DimensionPrefix0_Dialog: public Ui_DimensionPrefix0_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONPREFIX0_DIALOG_H
