/********************************************************************************
** Form generated from reading UI file 'bitmaptransparency_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITMAPTRANSPARENCY_DIALOG_H
#define UI_BITMAPTRANSPARENCY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BitmapTransparency_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *_104;
    QSpacerItem *horizontalSpacer;
    QCheckBox *_105;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_4;
    QGraphicsView *_100;
    QLabel *_200;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QGraphicsView *_101;
    QLabel *previewback_label;
    QPushButton *_1011;
    QPushButton *_1012;
    QPushButton *_1013;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label;
    QDoubleSpinBox *_102;
    QSlider *_103;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *BitmapTransparency_Dialog)
    {
        if (BitmapTransparency_Dialog->objectName().isEmpty())
            BitmapTransparency_Dialog->setObjectName(QString::fromUtf8("BitmapTransparency_Dialog"));
        BitmapTransparency_Dialog->resize(545, 438);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BitmapTransparency_Dialog->sizePolicy().hasHeightForWidth());
        BitmapTransparency_Dialog->setSizePolicy(sizePolicy);
        BitmapTransparency_Dialog->setMinimumSize(QSize(545, 438));
        BitmapTransparency_Dialog->setMaximumSize(QSize(20000, 20000));
        BitmapTransparency_Dialog->setBaseSize(QSize(545, 438));
        verticalLayout = new QVBoxLayout(BitmapTransparency_Dialog);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 1, 0, 1);
        label_3 = new QLabel(BitmapTransparency_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(200, 23));
        label_3->setMaximumSize(QSize(16777215, 23));
        QFont font;
        font.setPointSize(14);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_3);

        label_4 = new QLabel(BitmapTransparency_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(200, 23));
        label_4->setMaximumSize(QSize(16777215, 23));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_4);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(1);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        _104 = new QCheckBox(BitmapTransparency_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_6->addWidget(_104);

        horizontalSpacer = new QSpacerItem(160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        _105 = new QCheckBox(BitmapTransparency_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_6->addWidget(_105);

        horizontalSpacer_7 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(5, 5, 5, 5);
        _100 = new QGraphicsView(BitmapTransparency_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(_100->sizePolicy().hasHeightForWidth());
        _100->setSizePolicy(sizePolicy1);
        _100->setMinimumSize(QSize(250, 250));
        _100->setMaximumSize(QSize(2000, 2000));
        _100->setBaseSize(QSize(250, 250));
        _100->setAcceptDrops(false);
        _100->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _100->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _100->setInteractive(false);
        _100->setSceneRect(QRectF(0, 0, 250, 250));
        _100->setResizeAnchor(QGraphicsView::AnchorViewCenter);

        horizontalLayout_4->addWidget(_100);

        _200 = new QLabel(BitmapTransparency_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        sizePolicy1.setHeightForWidth(_200->sizePolicy().hasHeightForWidth());
        _200->setSizePolicy(sizePolicy1);
        _200->setMinimumSize(QSize(250, 250));
        _200->setMaximumSize(QSize(2000, 2000));
        _200->setBaseSize(QSize(250, 250));
        _200->setCursor(QCursor(Qt::CrossCursor));
        _200->setFrameShape(QFrame::NoFrame);
        _200->setPixmap(QPixmap(QString::fromUtf8(":/images/ATTACHMENT-image.svg")));
        _200->setScaledContents(true);

        horizontalLayout_4->addWidget(_200);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_2 = new QLabel(BitmapTransparency_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(118, 23));
        label_2->setMaximumSize(QSize(16777215, 23));

        horizontalLayout_3->addWidget(label_2);

        _101 = new QGraphicsView(BitmapTransparency_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(_101->sizePolicy().hasHeightForWidth());
        _101->setSizePolicy(sizePolicy2);
        _101->setMinimumSize(QSize(87, 23));
        _101->setMaximumSize(QSize(87, 23));

        horizontalLayout_3->addWidget(_101);

        previewback_label = new QLabel(BitmapTransparency_Dialog);
        previewback_label->setObjectName(QString::fromUtf8("previewback_label"));
        QFont font1;
        font1.setPointSize(13);
        previewback_label->setFont(font1);
        previewback_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(previewback_label);

        _1011 = new QPushButton(BitmapTransparency_Dialog);
        _1011->setObjectName(QString::fromUtf8("_1011"));
        _1011->setMinimumSize(QSize(25, 25));
        _1011->setMaximumSize(QSize(25, 25));
        _1011->setBaseSize(QSize(20, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Lucida Grande"));
        font2.setPointSize(10);
        _1011->setFont(font2);
        _1011->setText(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/TranspBack1-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/images/TranspBack1.png"), QSize(), QIcon::Normal, QIcon::On);
        _1011->setIcon(icon);
        _1011->setIconSize(QSize(21, 21));
        _1011->setCheckable(true);
        _1011->setChecked(true);
        _1011->setAutoExclusive(true);
        _1011->setAutoDefault(false);
        _1011->setFlat(true);

        horizontalLayout_3->addWidget(_1011);

        _1012 = new QPushButton(BitmapTransparency_Dialog);
        _1012->setObjectName(QString::fromUtf8("_1012"));
        _1012->setMinimumSize(QSize(25, 25));
        _1012->setMaximumSize(QSize(25, 25));
        _1012->setBaseSize(QSize(20, 20));
        _1012->setFont(font2);
        _1012->setText(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/TranspBack2-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/images/TranspBack2.png"), QSize(), QIcon::Normal, QIcon::On);
        _1012->setIcon(icon1);
        _1012->setIconSize(QSize(21, 21));
        _1012->setCheckable(true);
        _1012->setAutoExclusive(true);
        _1012->setAutoDefault(false);
        _1012->setFlat(true);

        horizontalLayout_3->addWidget(_1012);

        _1013 = new QPushButton(BitmapTransparency_Dialog);
        _1013->setObjectName(QString::fromUtf8("_1013"));
        _1013->setMinimumSize(QSize(25, 25));
        _1013->setMaximumSize(QSize(25, 25));
        _1013->setBaseSize(QSize(20, 20));
        _1013->setFont(font2);
        _1013->setText(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/TranspBack3-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/images/TranspBack3.png"), QSize(), QIcon::Normal, QIcon::On);
        _1013->setIcon(icon2);
        _1013->setIconSize(QSize(21, 21));
        _1013->setCheckable(true);
        _1013->setAutoExclusive(true);
        _1013->setAutoDefault(false);
        _1013->setFlat(true);

        horizontalLayout_3->addWidget(_1013);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(4);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        label = new QLabel(BitmapTransparency_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(118, 23));
        label->setMaximumSize(QSize(16777215, 23));

        horizontalLayout_2->addWidget(label);

        _102 = new QDoubleSpinBox(BitmapTransparency_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setDecimals(3);
        _102->setMaximum(1.000000000000000);
        _102->setSingleStep(0.001000000000000);

        horizontalLayout_2->addWidget(_102);

        _103 = new QSlider(BitmapTransparency_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setMinimumSize(QSize(200, 27));
        _103->setMaximumSize(QSize(150, 27));
        _103->setStyleSheet(QString::fromUtf8("QSlider::handle {\n"
"      background-color: white;\n"
"      border: 2px solid white;\n"
"      border-radius: 7px; \n"
"}"));
        _103->setMaximum(100);
        _103->setSingleStep(1);
        _103->setPageStep(5);
        _103->setOrientation(Qt::Horizontal);
        _103->setTickPosition(QSlider::TicksBothSides);
        _103->setTickInterval(5);

        horizontalLayout_2->addWidget(_103);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 10, 10, -1);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        _1 = new QPushButton(BitmapTransparency_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(BitmapTransparency_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(_101, _103);
        QWidget::setTabOrder(_103, _102);
        QWidget::setTabOrder(_102, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _100);

        retranslateUi(BitmapTransparency_Dialog);

        QMetaObject::connectSlotsByName(BitmapTransparency_Dialog);
    } // setupUi

    void retranslateUi(QDialog *BitmapTransparency_Dialog)
    {
        BitmapTransparency_Dialog->setWindowTitle(QCoreApplication::translate("BitmapTransparency_Dialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Preview", nullptr));
        label_4->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Colour Picker", nullptr));
        _104->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Original Shape", nullptr));
        _105->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Original Shape", nullptr));
#if QT_CONFIG(tooltip)
        _200->setToolTip(QCoreApplication::translate("BitmapTransparency_Dialog", "Pick and drag here to choose a colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _200->setText(QString());
        label_2->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Transparent Colour", nullptr));
        previewback_label->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Preview background", nullptr));
#if QT_CONFIG(tooltip)
        _1011->setToolTip(QCoreApplication::translate("BitmapTransparency_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1012->setToolTip(QCoreApplication::translate("BitmapTransparency_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1013->setToolTip(QCoreApplication::translate("BitmapTransparency_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Colour Range", nullptr));
        _1->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "     OK     ", nullptr));
        _2->setText(QCoreApplication::translate("BitmapTransparency_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BitmapTransparency_Dialog: public Ui_BitmapTransparency_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITMAPTRANSPARENCY_DIALOG_H
