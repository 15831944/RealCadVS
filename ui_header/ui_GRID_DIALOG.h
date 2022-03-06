/********************************************************************************
** Form generated from reading UI file 'GRID_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRID_DIALOG_H
#define UI_GRID_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Grid_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QCheckBox *_105;
    QCheckBox *_107;
    QCheckBox *_106;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_104;
    RCIntegerSpinBox *_102;
    RCIntegerSpinBox *_103;
    QCheckBox *_108;
    QCheckBox *_109;
    QGraphicsView *_111;
    QLabel *_110;
    QLabel *_112;
    QGraphicsView *_113;
    QPushButton *_1110;
    QPushButton *_1130;

    void setupUi(QDialog *Grid_Dialog)
    {
        if (Grid_Dialog->objectName().isEmpty())
            Grid_Dialog->setObjectName(QString::fromUtf8("Grid_Dialog"));
        Grid_Dialog->resize(271, 380);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Grid_Dialog->sizePolicy().hasHeightForWidth());
        Grid_Dialog->setSizePolicy(sizePolicy);
        Grid_Dialog->setMinimumSize(QSize(271, 380));
        Grid_Dialog->setMaximumSize(QSize(271, 380));
        QFont font;
        font.setPointSize(10);
        Grid_Dialog->setFont(font);
        Grid_Dialog->setWindowOpacity(1.000000000000000);
        _2 = new QPushButton(Grid_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(148, 330, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Grid_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(58, 330, 75, 23));
        _1->setMinimumSize(QSize(75, 23));
        _1->setMaximumSize(QSize(75, 23));
        label_3 = new QLabel(Grid_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(25, 20, 107, 21));
        label_4 = new QLabel(Grid_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(25, 50, 109, 21));
        label_5 = new QLabel(Grid_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(25, 80, 111, 21));
        label_6 = new QLabel(Grid_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(25, 110, 111, 21));
        label_7 = new QLabel(Grid_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(25, 140, 71, 21));
        _105 = new QCheckBox(Grid_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(26, 174, 205, 21));
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _107 = new QCheckBox(Grid_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(28, 214, 203, 21));
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);
        _106 = new QCheckBox(Grid_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(28, 194, 203, 21));
        _106->setLayoutDirection(Qt::RightToLeft);
        _106->setCheckable(true);
        _106->setChecked(false);
        _106->setTristate(false);
        _100 = new RCDoubleSpinBox(Grid_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(134, 18, 115, 22));
        _100->setMaximum(100000000.000000000000000);
        _101 = new RCDoubleSpinBox(Grid_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(134, 48, 115, 22));
        _101->setMaximum(100000000.000000000000000);
        _104 = new RCDoubleSpinBox(Grid_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(134, 138, 115, 22));
        _104->setMinimum(0.000000000000000);
        _104->setMaximum(360.000000000000000);
        _102 = new RCIntegerSpinBox(Grid_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(134, 78, 115, 22));
        _102->setMaximum(1000000000);
        _103 = new RCIntegerSpinBox(Grid_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(134, 108, 115, 22));
        _103->setMaximum(1000000000);
        _108 = new QCheckBox(Grid_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(32, 234, 199, 20));
        _108->setLayoutDirection(Qt::RightToLeft);
        _109 = new QCheckBox(Grid_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(38, 254, 193, 20));
        _109->setLayoutDirection(Qt::RightToLeft);
        _111 = new QGraphicsView(Grid_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(false);
        _111->setGeometry(QRect(196, 282, 32, 16));
        _111->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _111->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        _111->setBackgroundBrush(brush);
        _111->setSceneRect(QRectF(0, 0, 32, 16));
        _111->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _110 = new QLabel(Grid_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(false);
        _110->setGeometry(QRect(34, 282, 157, 16));
        _110->setLayoutDirection(Qt::LeftToRight);
        _110->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _112 = new QLabel(Grid_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(false);
        _112->setGeometry(QRect(44, 306, 147, 16));
        _112->setLayoutDirection(Qt::LeftToRight);
        _112->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _113 = new QGraphicsView(Grid_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(false);
        _113->setGeometry(QRect(196, 306, 32, 16));
        _113->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::NoBrush);
        _113->setBackgroundBrush(brush1);
        _113->setSceneRect(QRectF(0, 0, 32, 16));
        _113->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _1110 = new QPushButton(Grid_Dialog);
        _1110->setObjectName(QString::fromUtf8("_1110"));
        _1110->setGeometry(QRect(230, 278, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1110->setIcon(icon);
        _1110->setIconSize(QSize(20, 20));
        _1110->setFlat(true);
        _1130 = new QPushButton(Grid_Dialog);
        _1130->setObjectName(QString::fromUtf8("_1130"));
        _1130->setEnabled(false);
        _1130->setGeometry(QRect(230, 302, 24, 24));
        _1130->setIcon(icon);
        _1130->setIconSize(QSize(20, 20));
        _1130->setFlat(true);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _111);
        QWidget::setTabOrder(_111, _1110);
        QWidget::setTabOrder(_1110, _113);
        QWidget::setTabOrder(_113, _1130);
        QWidget::setTabOrder(_1130, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Grid_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Grid_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Grid_Dialog)
    {
        Grid_Dialog->setWindowTitle(QCoreApplication::translate("Grid_Dialog", "Grid options", nullptr));
        _2->setText(QCoreApplication::translate("Grid_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Grid_Dialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("Grid_Dialog", "X spacing", nullptr));
        label_4->setText(QCoreApplication::translate("Grid_Dialog", "Y spacing", nullptr));
        label_5->setText(QCoreApplication::translate("Grid_Dialog", "X reference grid", nullptr));
        label_6->setText(QCoreApplication::translate("Grid_Dialog", "Y reference grid", nullptr));
        label_7->setText(QCoreApplication::translate("Grid_Dialog", "Angle", nullptr));
        _105->setText(QCoreApplication::translate("Grid_Dialog", "Snap to grid", nullptr));
        _107->setText(QCoreApplication::translate("Grid_Dialog", "Display reference grid only", nullptr));
        _106->setText(QCoreApplication::translate("Grid_Dialog", "Display grid", nullptr));
        _108->setText(QCoreApplication::translate("Grid_Dialog", "Display grid as lines", nullptr));
        _109->setText(QCoreApplication::translate("Grid_Dialog", "Use custom grid colour", nullptr));
        _110->setText(QCoreApplication::translate("Grid_Dialog", "Grid colour", nullptr));
        _112->setText(QCoreApplication::translate("Grid_Dialog", "Grid colour", nullptr));
        _1110->setText(QString());
        _1130->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Grid_Dialog: public Ui_Grid_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRID_DIALOG_H
