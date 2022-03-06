/********************************************************************************
** Form generated from reading UI file 'TEXTURE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTURE_DIALOG_H
#define UI_TEXTURE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Texture_Dialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *_100;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QLineEdit *_102;
    QPushButton *_112;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QDoubleSpinBox *_106;
    QLabel *label_2;
    QDoubleSpinBox *_103;
    QLabel *label_6;
    QDoubleSpinBox *_104;
    QLabel *label_3;
    QLabel *label_4;
    QDoubleSpinBox *_105;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *_107;
    QCheckBox *_119;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QLabel *label_8;
    QDoubleSpinBox *_113;
    QLabel *label_9;
    QDoubleSpinBox *_114;
    QLabel *label_10;
    QDoubleSpinBox *_115;
    QPushButton *_117;
    QGraphicsView *_118;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QGridLayout *gridLayout;
    QLabel *label_12;
    QDoubleSpinBox *_109;
    QDoubleSpinBox *_111;
    QDoubleSpinBox *_110;
    QLabel *label_14;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *Texture_Dialog)
    {
        if (Texture_Dialog->objectName().isEmpty())
            Texture_Dialog->setObjectName(QString::fromUtf8("Texture_Dialog"));
        Texture_Dialog->resize(719, 504);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Texture_Dialog->sizePolicy().hasHeightForWidth());
        Texture_Dialog->setSizePolicy(sizePolicy);
        Texture_Dialog->setMinimumSize(QSize(719, 480));
        Texture_Dialog->setMaximumSize(QSize(4000, 4000));
        verticalLayout_3 = new QVBoxLayout(Texture_Dialog);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Texture_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        _100 = new QComboBox(Texture_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(_100);


        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        groupBox = new QGroupBox(Texture_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        horizontalLayout_9 = new QHBoxLayout(groupBox);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        horizontalLayout_7->addWidget(label_5);

        _102 = new QLineEdit(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));

        horizontalLayout_7->addWidget(_102);

        _112 = new QPushButton(groupBox);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setAutoDefault(false);

        horizontalLayout_7->addWidget(_112);


        verticalLayout_4->addLayout(horizontalLayout_7);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setMinimumSize(QSize(0, 0));
        label_7->setFont(font);

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        _106 = new QDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setDecimals(4);
        _106->setMaximum(1000000.000000000000000);

        gridLayout_2->addWidget(_106, 1, 4, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 1, 1, 1, Qt::AlignLeft);

        _103 = new QDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setDecimals(4);
        _103->setMaximum(1000000.000000000000000);

        gridLayout_2->addWidget(_103, 0, 2, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 3, 1, 1);

        _104 = new QDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setDecimals(4);
        _104->setMaximum(1000000.000000000000000);

        gridLayout_2->addWidget(_104, 0, 4, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 3, 1, 1);

        _105 = new QDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setDecimals(4);
        _105->setMaximum(1000000.000000000000000);

        gridLayout_2->addWidget(_105, 1, 2, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        _107 = new QCheckBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);

        verticalLayout_2->addWidget(_107);

        _119 = new QCheckBox(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setLayoutDirection(Qt::RightToLeft);
        _119->setCheckable(true);
        _119->setChecked(false);
        _119->setTristate(false);

        verticalLayout_2->addWidget(_119);


        horizontalLayout_8->addLayout(verticalLayout_2);

        horizontalSpacer_6 = new QSpacerItem(182, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);

        horizontalLayout_6->addWidget(label_11);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_8);

        _113 = new QDoubleSpinBox(groupBox);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setMaximum(1.000000000000000);
        _113->setSingleStep(0.100000000000000);

        horizontalLayout_6->addWidget(_113);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_9);

        _114 = new QDoubleSpinBox(groupBox);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setMaximum(1.000000000000000);
        _114->setSingleStep(0.100000000000000);

        horizontalLayout_6->addWidget(_114);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_10);

        _115 = new QDoubleSpinBox(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setMaximum(1.000000000000000);
        _115->setSingleStep(0.100000000000000);

        horizontalLayout_6->addWidget(_115);

        _117 = new QPushButton(groupBox);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setAutoDefault(false);

        horizontalLayout_6->addWidget(_117);


        verticalLayout_4->addLayout(horizontalLayout_6);


        horizontalLayout_9->addLayout(verticalLayout_4);

        _118 = new QGraphicsView(groupBox);
        _118->setObjectName(QString::fromUtf8("_118"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(2);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(_118->sizePolicy().hasHeightForWidth());
        _118->setSizePolicy(sizePolicy2);
        _118->setMinimumSize(QSize(200, 200));
        _118->setMaximumSize(QSize(500, 500));
        _118->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setSceneRect(QRectF(0, 0, 200, 200));
        _118->setResizeAnchor(QGraphicsView::AnchorViewCenter);

        horizontalLayout_9->addWidget(_118);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Texture_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        horizontalLayout_5 = new QHBoxLayout(groupBox_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font);

        gridLayout->addWidget(label_12, 0, 0, 1, 1);

        _109 = new QDoubleSpinBox(groupBox_2);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setMinimumSize(QSize(90, 0));
        _109->setDecimals(6);
        _109->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_109, 0, 1, 1, 1);

        _111 = new QDoubleSpinBox(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setMinimumSize(QSize(90, 0));
        _111->setDecimals(6);
        _111->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_111, 2, 1, 1, 1);

        _110 = new QDoubleSpinBox(groupBox_2);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setMinimumSize(QSize(90, 0));
        _110->setDecimals(6);
        _110->setMaximum(100000.000000000000000);

        gridLayout->addWidget(_110, 1, 1, 1, 1);

        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);

        gridLayout->addWidget(label_14, 2, 0, 1, 1);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font);

        gridLayout->addWidget(label_13, 1, 0, 1, 1);


        horizontalLayout_4->addLayout(gridLayout);

        horizontalSpacer_5 = new QSpacerItem(482, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout_3->addWidget(groupBox_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(20);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        _1 = new QPushButton(Texture_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setMinimumSize(QSize(100, 0));
        _1->setAutoDefault(true);

        horizontalLayout_3->addWidget(_1);

        _2 = new QPushButton(Texture_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setMinimumSize(QSize(100, 0));
        _2->setAutoDefault(false);

        horizontalLayout_3->addWidget(_2);


        verticalLayout_3->addLayout(horizontalLayout_3);

        QWidget::setTabOrder(_100, _102);
        QWidget::setTabOrder(_102, _112);
        QWidget::setTabOrder(_112, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _119);
        QWidget::setTabOrder(_119, _113);
        QWidget::setTabOrder(_113, _114);
        QWidget::setTabOrder(_114, _115);
        QWidget::setTabOrder(_115, _117);
        QWidget::setTabOrder(_117, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _111);
        QWidget::setTabOrder(_111, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _118);

        retranslateUi(Texture_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Texture_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Texture_Dialog)
    {
        Texture_Dialog->setWindowTitle(QCoreApplication::translate("Texture_Dialog", "Texture Map Options", nullptr));
        label->setText(QCoreApplication::translate("Texture_Dialog", "Texture", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Texture_Dialog", "2D Texture Parameters", nullptr));
        label_5->setText(QCoreApplication::translate("Texture_Dialog", "Filename", nullptr));
        _112->setText(QCoreApplication::translate("Texture_Dialog", "Browse...", nullptr));
        label_7->setText(QCoreApplication::translate("Texture_Dialog", "Area", nullptr));
        label_2->setText(QCoreApplication::translate("Texture_Dialog", "X Left", nullptr));
        label_6->setText(QCoreApplication::translate("Texture_Dialog", "Y Top", nullptr));
        label_3->setText(QCoreApplication::translate("Texture_Dialog", "X Right", nullptr));
        label_4->setText(QCoreApplication::translate("Texture_Dialog", "Y Bottom", nullptr));
        _107->setText(QCoreApplication::translate("Texture_Dialog", "Swap X and Y texture map directions", nullptr));
        _119->setText(QCoreApplication::translate("Texture_Dialog", "Use transparent colour", nullptr));
        label_11->setText(QCoreApplication::translate("Texture_Dialog", "Transparent colour", nullptr));
        label_8->setText(QCoreApplication::translate("Texture_Dialog", "R", nullptr));
        label_9->setText(QCoreApplication::translate("Texture_Dialog", "G", nullptr));
        label_10->setText(QCoreApplication::translate("Texture_Dialog", "B", nullptr));
        _117->setText(QCoreApplication::translate("Texture_Dialog", "Select...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Texture_Dialog", "Solid Texture Parameters", nullptr));
        label_12->setText(QCoreApplication::translate("Texture_Dialog", "Texture size", nullptr));
        label_14->setText(QCoreApplication::translate("Texture_Dialog", "Colour scale", nullptr));
        label_13->setText(QCoreApplication::translate("Texture_Dialog", "Pattern scale", nullptr));
        _1->setText(QCoreApplication::translate("Texture_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Texture_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Texture_Dialog: public Ui_Texture_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTURE_DIALOG_H
