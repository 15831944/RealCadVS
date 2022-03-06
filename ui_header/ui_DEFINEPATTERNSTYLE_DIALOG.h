/********************************************************************************
** Form generated from reading UI file 'DEFINEPATTERNSTYLE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFINEPATTERNSTYLE_DIALOG_H
#define UI_DEFINEPATTERNSTYLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DefinePatternStyle_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *_110;
    QPushButton *_121;
    QPushButton *_120;
    QComboBox *_112;
    QLabel *label_8;
    QLineEdit *_106;
    QPushButton *_107;
    QLabel *label_12;
    QPushButton *_122;
    QGraphicsView *_109;
    QDoubleSpinBox *_100;
    QDoubleSpinBox *_101;

    void setupUi(QDialog *DefinePatternStyle_Dialog)
    {
        if (DefinePatternStyle_Dialog->objectName().isEmpty())
            DefinePatternStyle_Dialog->setObjectName(QString::fromUtf8("DefinePatternStyle_Dialog"));
        DefinePatternStyle_Dialog->resize(510, 390);
        DefinePatternStyle_Dialog->setMinimumSize(QSize(510, 390));
        DefinePatternStyle_Dialog->setMaximumSize(QSize(510, 390));
        QFont font;
        font.setPointSize(10);
        DefinePatternStyle_Dialog->setFont(font);
        _2 = new QPushButton(DefinePatternStyle_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(404, 338, 75, 25));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(DefinePatternStyle_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(324, 338, 75, 25));
        _1->setFont(font1);
        label_4 = new QLabel(DefinePatternStyle_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(24, 48, 53, 20));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(DefinePatternStyle_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(322, 50, 79, 20));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(DefinePatternStyle_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(24, 14, 93, 20));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _110 = new QComboBox(DefinePatternStyle_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(121, 14, 221, 22));
        _110->setFont(font);
        _110->setEditable(true);
        _121 = new QPushButton(DefinePatternStyle_Dialog);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(424, 12, 70, 25));
        _121->setFont(font1);
        _120 = new QPushButton(DefinePatternStyle_Dialog);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(350, 12, 70, 25));
        _120->setFont(font1);
        _112 = new QComboBox(DefinePatternStyle_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(408, 48, 85, 22));
        _112->setFont(font);
        _112->setEditable(false);
        label_8 = new QLabel(DefinePatternStyle_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(22, 84, 67, 20));
        label_8->setFont(font1);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QLineEdit(DefinePatternStyle_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(94, 83, 319, 21));
        _106->setFont(font);
        _106->setReadOnly(true);
        _107 = new QPushButton(DefinePatternStyle_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(420, 80, 75, 25));
        _107->setFont(font1);
        label_12 = new QLabel(DefinePatternStyle_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(162, 50, 69, 20));
        label_12->setFont(font1);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _122 = new QPushButton(DefinePatternStyle_Dialog);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setGeometry(QRect(32, 338, 149, 25));
        _122->setFont(font1);
        _109 = new QGraphicsView(DefinePatternStyle_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(20, 115, 469, 220));
        _109->setMinimumSize(QSize(469, 220));
        _109->setMaximumSize(QSize(469, 220));
        _109->setAcceptDrops(false);
        _109->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _109->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _109->setInteractive(false);
        _109->setSceneRect(QRectF(0, 0, 469, 220));
        _109->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        _100 = new QDoubleSpinBox(DefinePatternStyle_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(96, 48, 60, 24));
        _100->setFont(font);
        _100->setMinimum(0.010000000000000);
        _100->setMaximum(100000000.000000000000000);
        _100->setValue(1.000000000000000);
        _101 = new QDoubleSpinBox(DefinePatternStyle_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(252, 46, 60, 24));
        _101->setFont(font);
        _101->setMinimum(0.010000000000000);
        _101->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_110, _100);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _112);
        QWidget::setTabOrder(_112, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _120);
        QWidget::setTabOrder(_120, _121);
        QWidget::setTabOrder(_121, _122);
        QWidget::setTabOrder(_122, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _109);

        retranslateUi(DefinePatternStyle_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DefinePatternStyle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DefinePatternStyle_Dialog)
    {
        DefinePatternStyle_Dialog->setWindowTitle(QCoreApplication::translate("DefinePatternStyle_Dialog", "Define pattern line style", nullptr));
        _2->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "OK", nullptr));
        label_4->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Height", nullptr));
        label_6->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Pattern size", nullptr));
        label_7->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Line style name", nullptr));
        _121->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Delete", nullptr));
        _120->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Update", nullptr));
        label_8->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Pattern file ", nullptr));
        _107->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Browse", nullptr));
        label_12->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Length scale", nullptr));
        _122->setText(QCoreApplication::translate("DefinePatternStyle_Dialog", "Create pattern file", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DefinePatternStyle_Dialog: public Ui_DefinePatternStyle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFINEPATTERNSTYLE_DIALOG_H
