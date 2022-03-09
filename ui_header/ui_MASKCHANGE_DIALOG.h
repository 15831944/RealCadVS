/********************************************************************************
** Form generated from reading UI file 'MASKCHANGE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MASKCHANGE_DIALOG_H
#define UI_MASKCHANGE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaskChange_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *_100;
    QWidget *_101;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_12;
    QTableView *_1111;
    QTableView *_1112;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *_115;
    QLineEdit *_116;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *_108_label;
    QComboBox *_108;
    QPushButton *_113;
    QSpacerItem *horizontalSpacer_7;
    QWidget *_102;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_9;
    QTableView *_1121;
    QTableView *_1122;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *_112;
    QSpacerItem *horizontalSpacer_6;
    QWidget *_103;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_7;
    QTableView *_1131;
    QTableView *_1132;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *_118;
    QLineEdit *_117;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *_106;
    QPushButton *_119;
    QPushButton *_109;
    QPushButton *_120;
    QSpacerItem *horizontalSpacer_3;
    QWidget *_104;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QTableView *_1141;
    QTableView *_1142;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *_114;
    QPushButton *_111;
    QSpacerItem *horizontalSpacer_4;
    QWidget *_105;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QTableView *_1151;
    QTableView *_1152;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *_107;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *MaskChange_Dialog)
    {
        if (MaskChange_Dialog->objectName().isEmpty())
            MaskChange_Dialog->setObjectName(QString::fromUtf8("MaskChange_Dialog"));
        MaskChange_Dialog->resize(540, 522);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MaskChange_Dialog->sizePolicy().hasHeightForWidth());
        MaskChange_Dialog->setSizePolicy(sizePolicy);
        MaskChange_Dialog->setMinimumSize(QSize(540, 522));
        MaskChange_Dialog->setMaximumSize(QSize(536, 4000));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        MaskChange_Dialog->setFont(font);
        verticalLayout = new QVBoxLayout(MaskChange_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        _100 = new QTabWidget(MaskChange_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        sizePolicy.setHeightForWidth(_100->sizePolicy().hasHeightForWidth());
        _100->setSizePolicy(sizePolicy);
        _100->setMinimumSize(QSize(518, 455));
        _100->setMaximumSize(QSize(518, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _100->setFont(font1);
        _100->setFocusPolicy(Qt::TabFocus);
        _100->setUsesScrollButtons(false);
        _101 = new QWidget();
        _101->setObjectName(QString::fromUtf8("_101"));
        verticalLayout_4 = new QVBoxLayout(_101);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 6);
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        _1111 = new QTableView(_101);
        _1111->setObjectName(QString::fromUtf8("_1111"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(_1111->sizePolicy().hasHeightForWidth());
        _1111->setSizePolicy(sizePolicy1);
        _1111->setMinimumSize(QSize(340, 300));
        _1111->setMaximumSize(QSize(10000, 10000));
        _1111->setFont(font1);
        _1111->setContextMenuPolicy(Qt::DefaultContextMenu);
        _1111->setFrameShape(QFrame::NoFrame);
        _1111->setFrameShadow(QFrame::Plain);
        _1111->setLineWidth(0);
        _1111->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1111->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1111->setAutoScroll(true);
        _1111->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        _1111->setProperty("showDropIndicator", QVariant(false));
        _1111->setDragDropOverwriteMode(false);
        _1111->setAlternatingRowColors(true);
        _1111->setSelectionMode(QAbstractItemView::SingleSelection);
        _1111->setSelectionBehavior(QAbstractItemView::SelectRows);
        _1111->setShowGrid(false);
        _1111->setSortingEnabled(false);
        _1111->setWordWrap(false);
        _1111->horizontalHeader()->setMinimumSectionSize(20);
        _1111->horizontalHeader()->setDefaultSectionSize(20);
        _1111->horizontalHeader()->setHighlightSections(false);
        _1111->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        _1111->verticalHeader()->setVisible(false);
        _1111->verticalHeader()->setHighlightSections(false);

        horizontalLayout_12->addWidget(_1111);

        _1112 = new QTableView(_101);
        _1112->setObjectName(QString::fromUtf8("_1112"));
        sizePolicy.setHeightForWidth(_1112->sizePolicy().hasHeightForWidth());
        _1112->setSizePolicy(sizePolicy);
        _1112->setMinimumSize(QSize(160, 300));
        _1112->setMaximumSize(QSize(160, 16777215));
        _1112->setFont(font1);
        _1112->setFrameShape(QFrame::NoFrame);
        _1112->setFrameShadow(QFrame::Plain);
        _1112->setLineWidth(0);
        _1112->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1112->setAlternatingRowColors(true);
        _1112->setShowGrid(false);
        _1112->horizontalHeader()->setHighlightSections(false);
        _1112->verticalHeader()->setVisible(false);
        _1112->verticalHeader()->setHighlightSections(false);

        horizontalLayout_12->addWidget(_1112);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(10, -1, -1, -1);
        _115 = new QPushButton(_101);
        _115->setObjectName(QString::fromUtf8("_115"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(_115->sizePolicy().hasHeightForWidth());
        _115->setSizePolicy(sizePolicy2);
        _115->setFont(font);
        _115->setAutoDefault(false);

        horizontalLayout_11->addWidget(_115);

        _116 = new QLineEdit(_101);
        _116->setObjectName(QString::fromUtf8("_116"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(_116->sizePolicy().hasHeightForWidth());
        _116->setSizePolicy(sizePolicy3);
        _116->setMinimumSize(QSize(200, 0));
        QFont font2;
        font2.setPointSize(12);
        _116->setFont(font2);

        horizontalLayout_11->addWidget(_116);

        horizontalSpacer_8 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_8);


        verticalLayout_4->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(10, -1, -1, -1);
        _108_label = new QLabel(_101);
        _108_label->setObjectName(QString::fromUtf8("_108_label"));
        _108_label->setFont(font1);
        _108_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(_108_label);

        _108 = new QComboBox(_101);
        _108->setObjectName(QString::fromUtf8("_108"));
        sizePolicy3.setHeightForWidth(_108->sizePolicy().hasHeightForWidth());
        _108->setSizePolicy(sizePolicy3);
        _108->setMinimumSize(QSize(200, 0));

        horizontalLayout_10->addWidget(_108);

        _113 = new QPushButton(_101);
        _113->setObjectName(QString::fromUtf8("_113"));
        sizePolicy2.setHeightForWidth(_113->sizePolicy().hasHeightForWidth());
        _113->setSizePolicy(sizePolicy2);
        _113->setMinimumSize(QSize(100, 0));
        _113->setFont(font);
        _113->setAutoDefault(false);

        horizontalLayout_10->addWidget(_113);

        horizontalSpacer_7 = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout_4->addLayout(horizontalLayout_10);

        _100->addTab(_101, QString());
        _102 = new QWidget();
        _102->setObjectName(QString::fromUtf8("_102"));
        verticalLayout_10 = new QVBoxLayout(_102);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(-1, -1, -1, 10);
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        _1121 = new QTableView(_102);
        _1121->setObjectName(QString::fromUtf8("_1121"));
        sizePolicy.setHeightForWidth(_1121->sizePolicy().hasHeightForWidth());
        _1121->setSizePolicy(sizePolicy);
        _1121->setMinimumSize(QSize(314, 300));
        _1121->setMaximumSize(QSize(10000, 10000));
        _1121->setFont(font1);
        _1121->setFrameShape(QFrame::NoFrame);
        _1121->setFrameShadow(QFrame::Plain);
        _1121->setLineWidth(0);
        _1121->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1121->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1121->setAutoScroll(true);
        _1121->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        _1121->setProperty("showDropIndicator", QVariant(false));
        _1121->setDragDropOverwriteMode(false);
        _1121->setAlternatingRowColors(true);
        _1121->setShowGrid(false);
        _1121->setWordWrap(false);
        _1121->horizontalHeader()->setHighlightSections(false);
        _1121->verticalHeader()->setVisible(false);
        _1121->verticalHeader()->setHighlightSections(false);

        horizontalLayout_9->addWidget(_1121);

        _1122 = new QTableView(_102);
        _1122->setObjectName(QString::fromUtf8("_1122"));
        sizePolicy.setHeightForWidth(_1122->sizePolicy().hasHeightForWidth());
        _1122->setSizePolicy(sizePolicy);
        _1122->setMinimumSize(QSize(160, 300));
        _1122->setMaximumSize(QSize(160, 10000));
        _1122->setFont(font1);
        _1122->setFrameShape(QFrame::NoFrame);
        _1122->setFrameShadow(QFrame::Plain);
        _1122->setLineWidth(0);
        _1122->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1122->setProperty("showDropIndicator", QVariant(false));
        _1122->setDragDropOverwriteMode(false);
        _1122->setAlternatingRowColors(true);
        _1122->setShowGrid(false);
        _1122->setWordWrap(false);
        _1122->horizontalHeader()->setHighlightSections(false);
        _1122->verticalHeader()->setVisible(false);
        _1122->verticalHeader()->setHighlightSections(false);

        horizontalLayout_9->addWidget(_1122);


        verticalLayout_9->addLayout(horizontalLayout_9);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_3);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(10, -1, -1, -1);
        _112 = new QCheckBox(_102);
        _112->setObjectName(QString::fromUtf8("_112"));

        horizontalLayout_8->addWidget(_112);

        horizontalSpacer_6 = new QSpacerItem(332, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout_9->addLayout(horizontalLayout_8);


        verticalLayout_10->addLayout(verticalLayout_9);

        _100->addTab(_102, QString());
        _103 = new QWidget();
        _103->setObjectName(QString::fromUtf8("_103"));
        verticalLayout_8 = new QVBoxLayout(_103);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, -1, -1, 10);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        _1131 = new QTableView(_103);
        _1131->setObjectName(QString::fromUtf8("_1131"));
        sizePolicy.setHeightForWidth(_1131->sizePolicy().hasHeightForWidth());
        _1131->setSizePolicy(sizePolicy);
        _1131->setMinimumSize(QSize(314, 300));
        _1131->setMaximumSize(QSize(10000, 10000));
        _1131->setFont(font1);
        _1131->setFrameShape(QFrame::NoFrame);
        _1131->setFrameShadow(QFrame::Plain);
        _1131->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1131->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1131->setAutoScroll(true);
        _1131->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        _1131->setProperty("showDropIndicator", QVariant(false));
        _1131->setDragDropOverwriteMode(false);
        _1131->setAlternatingRowColors(true);
        _1131->setShowGrid(false);
        _1131->setWordWrap(false);
        _1131->horizontalHeader()->setHighlightSections(false);
        _1131->verticalHeader()->setVisible(false);
        _1131->verticalHeader()->setHighlightSections(false);

        horizontalLayout_7->addWidget(_1131);

        _1132 = new QTableView(_103);
        _1132->setObjectName(QString::fromUtf8("_1132"));
        sizePolicy.setHeightForWidth(_1132->sizePolicy().hasHeightForWidth());
        _1132->setSizePolicy(sizePolicy);
        _1132->setMinimumSize(QSize(160, 300));
        _1132->setMaximumSize(QSize(160, 16777215));
        _1132->setFont(font1);
        _1132->setFrameShape(QFrame::NoFrame);
        _1132->setFrameShadow(QFrame::Plain);
        _1132->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1132->setProperty("showDropIndicator", QVariant(false));
        _1132->setDragDropOverwriteMode(false);
        _1132->setAlternatingRowColors(true);
        _1132->setShowGrid(false);
        _1132->setWordWrap(false);
        _1132->horizontalHeader()->setHighlightSections(false);
        _1132->verticalHeader()->setVisible(false);
        _1132->verticalHeader()->setHighlightSections(false);

        horizontalLayout_7->addWidget(_1132);


        verticalLayout_7->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_6->setContentsMargins(10, -1, -1, -1);
        _118 = new QPushButton(_103);
        _118->setObjectName(QString::fromUtf8("_118"));
        sizePolicy2.setHeightForWidth(_118->sizePolicy().hasHeightForWidth());
        _118->setSizePolicy(sizePolicy2);
        _118->setFont(font);
        _118->setAutoDefault(false);

        horizontalLayout_6->addWidget(_118);

        _117 = new QLineEdit(_103);
        _117->setObjectName(QString::fromUtf8("_117"));
        sizePolicy3.setHeightForWidth(_117->sizePolicy().hasHeightForWidth());
        _117->setSizePolicy(sizePolicy3);
        _117->setMinimumSize(QSize(150, 0));
        _117->setFont(font2);

        horizontalLayout_6->addWidget(_117);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_7->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(10, -1, -1, -1);
        _106 = new QPushButton(_103);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        sizePolicy2.setHeightForWidth(_106->sizePolicy().hasHeightForWidth());
        _106->setSizePolicy(sizePolicy2);
        _106->setMinimumSize(QSize(0, 0));
        _106->setFont(font);
        _106->setAutoDefault(false);

        horizontalLayout_5->addWidget(_106);

        _119 = new QPushButton(_103);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        sizePolicy2.setHeightForWidth(_119->sizePolicy().hasHeightForWidth());
        _119->setSizePolicy(sizePolicy2);
        _119->setFont(font);
        _119->setAutoDefault(false);

        horizontalLayout_5->addWidget(_119);

        _109 = new QPushButton(_103);
        _109->setObjectName(QString::fromUtf8("_109"));
        sizePolicy2.setHeightForWidth(_109->sizePolicy().hasHeightForWidth());
        _109->setSizePolicy(sizePolicy2);
        _109->setMinimumSize(QSize(120, 0));
        _109->setFont(font);
        _109->setAutoDefault(false);

        horizontalLayout_5->addWidget(_109);

        _120 = new QPushButton(_103);
        _120->setObjectName(QString::fromUtf8("_120"));
        sizePolicy2.setHeightForWidth(_120->sizePolicy().hasHeightForWidth());
        _120->setSizePolicy(sizePolicy2);
        _120->setMinimumSize(QSize(120, 0));
        _120->setFont(font);
        _120->setAutoDefault(false);

        horizontalLayout_5->addWidget(_120);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_7->addLayout(horizontalLayout_5);


        verticalLayout_8->addLayout(verticalLayout_7);

        _100->addTab(_103, QString());
        _104 = new QWidget();
        _104->setObjectName(QString::fromUtf8("_104"));
        verticalLayout_6 = new QVBoxLayout(_104);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, -1, -1, 10);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        _1141 = new QTableView(_104);
        _1141->setObjectName(QString::fromUtf8("_1141"));
        sizePolicy.setHeightForWidth(_1141->sizePolicy().hasHeightForWidth());
        _1141->setSizePolicy(sizePolicy);
        _1141->setMinimumSize(QSize(314, 300));
        _1141->setMaximumSize(QSize(10000, 10000));
        _1141->setFont(font1);
        _1141->setFrameShape(QFrame::NoFrame);
        _1141->setFrameShadow(QFrame::Plain);
        _1141->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1141->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1141->setAutoScroll(true);
        _1141->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        _1141->setProperty("showDropIndicator", QVariant(false));
        _1141->setDragDropOverwriteMode(false);
        _1141->setAlternatingRowColors(true);
        _1141->setShowGrid(false);
        _1141->setWordWrap(false);
        _1141->horizontalHeader()->setHighlightSections(false);
        _1141->verticalHeader()->setVisible(false);
        _1141->verticalHeader()->setHighlightSections(false);

        horizontalLayout_4->addWidget(_1141);

        _1142 = new QTableView(_104);
        _1142->setObjectName(QString::fromUtf8("_1142"));
        sizePolicy.setHeightForWidth(_1142->sizePolicy().hasHeightForWidth());
        _1142->setSizePolicy(sizePolicy);
        _1142->setMinimumSize(QSize(160, 300));
        _1142->setMaximumSize(QSize(160, 16777215));
        _1142->setFont(font1);
        _1142->setFrameShape(QFrame::NoFrame);
        _1142->setFrameShadow(QFrame::Plain);
        _1142->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1142->setProperty("showDropIndicator", QVariant(false));
        _1142->setDragDropOverwriteMode(false);
        _1142->setAlternatingRowColors(true);
        _1142->setShowGrid(false);
        _1142->setWordWrap(false);
        _1142->horizontalHeader()->setHighlightSections(false);
        _1142->verticalHeader()->setVisible(false);
        _1142->verticalHeader()->setHighlightSections(false);

        horizontalLayout_4->addWidget(_1142);


        verticalLayout_5->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(14, 14, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(10, -1, -1, -1);
        _114 = new QPushButton(_104);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        sizePolicy2.setHeightForWidth(_114->sizePolicy().hasHeightForWidth());
        _114->setSizePolicy(sizePolicy2);
        _114->setMinimumSize(QSize(77, 0));
        _114->setFont(font);
        _114->setAutoDefault(false);

        horizontalLayout_3->addWidget(_114);

        _111 = new QPushButton(_104);
        _111->setObjectName(QString::fromUtf8("_111"));
        sizePolicy2.setHeightForWidth(_111->sizePolicy().hasHeightForWidth());
        _111->setSizePolicy(sizePolicy2);
        _111->setMinimumSize(QSize(131, 0));
        _111->setFont(font);
        _111->setAutoDefault(false);

        horizontalLayout_3->addWidget(_111);

        horizontalSpacer_4 = new QSpacerItem(228, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_3);


        verticalLayout_6->addLayout(verticalLayout_5);

        _100->addTab(_104, QString());
        _105 = new QWidget();
        _105->setObjectName(QString::fromUtf8("_105"));
        verticalLayout_3 = new QVBoxLayout(_105);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        _1151 = new QTableView(_105);
        _1151->setObjectName(QString::fromUtf8("_1151"));
        sizePolicy.setHeightForWidth(_1151->sizePolicy().hasHeightForWidth());
        _1151->setSizePolicy(sizePolicy);
        _1151->setMinimumSize(QSize(314, 320));
        _1151->setMaximumSize(QSize(10000, 10000));
        _1151->setFont(font1);
        _1151->setFrameShape(QFrame::NoFrame);
        _1151->setFrameShadow(QFrame::Plain);
        _1151->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1151->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1151->setAutoScroll(true);
        _1151->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        _1151->setProperty("showDropIndicator", QVariant(false));
        _1151->setDragDropOverwriteMode(false);
        _1151->setAlternatingRowColors(true);
        _1151->setShowGrid(false);
        _1151->setWordWrap(false);
        _1151->horizontalHeader()->setHighlightSections(false);
        _1151->verticalHeader()->setVisible(false);
        _1151->verticalHeader()->setHighlightSections(false);

        horizontalLayout_2->addWidget(_1151);

        _1152 = new QTableView(_105);
        _1152->setObjectName(QString::fromUtf8("_1152"));
        sizePolicy.setHeightForWidth(_1152->sizePolicy().hasHeightForWidth());
        _1152->setSizePolicy(sizePolicy);
        _1152->setMinimumSize(QSize(160, 320));
        _1152->setMaximumSize(QSize(160, 10000));
        _1152->setFont(font1);
        _1152->setFrameShape(QFrame::NoFrame);
        _1152->setFrameShadow(QFrame::Plain);
        _1152->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _1152->setProperty("showDropIndicator", QVariant(false));
        _1152->setDragDropOverwriteMode(false);
        _1152->setAlternatingRowColors(true);
        _1152->setShowGrid(false);
        _1152->setWordWrap(false);
        _1152->horizontalHeader()->setHighlightSections(false);
        _1152->verticalHeader()->setVisible(false);
        _1152->verticalHeader()->setHighlightSections(false);

        horizontalLayout_2->addWidget(_1152);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 101, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);

        _100->addTab(_105, QString());

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 10, -1);
        horizontalSpacer = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _107 = new QPushButton(MaskChange_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setFont(font);
        _107->setAutoDefault(false);

        horizontalLayout->addWidget(_107);

        horizontalSpacer_2 = new QSpacerItem(24, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        _1 = new QPushButton(MaskChange_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setFont(font);
        _1->setAutoDefault(false);

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(MaskChange_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setFont(font);
        _2->setAutoDefault(false);

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(_1111, _1112);
        QWidget::setTabOrder(_1112, _1121);
        QWidget::setTabOrder(_1121, _1122);
        QWidget::setTabOrder(_1122, _1131);
        QWidget::setTabOrder(_1131, _1132);
        QWidget::setTabOrder(_1132, _1141);
        QWidget::setTabOrder(_1141, _1142);
        QWidget::setTabOrder(_1142, _1151);
        QWidget::setTabOrder(_1151, _1152);
        QWidget::setTabOrder(_1152, _107);
        QWidget::setTabOrder(_107, _1);

        retranslateUi(MaskChange_Dialog);

        _100->setCurrentIndex(2);
        _1->setDefault(true);


        QMetaObject::connectSlotsByName(MaskChange_Dialog);
    } // setupUi

    void retranslateUi(QDialog *MaskChange_Dialog)
    {
        MaskChange_Dialog->setWindowTitle(QCoreApplication::translate("MaskChange_Dialog", "General Properties", nullptr));
        _115->setText(QCoreApplication::translate("MaskChange_Dialog", "Search", nullptr));
        _116->setPlaceholderText(QCoreApplication::translate("MaskChange_Dialog", "Enter a number or name", nullptr));
        _108_label->setText(QCoreApplication::translate("MaskChange_Dialog", "Palette", nullptr));
        _113->setText(QCoreApplication::translate("MaskChange_Dialog", "Edit Palette", nullptr));
        _100->setTabText(_100->indexOf(_101), QCoreApplication::translate("MaskChange_Dialog", "Layer", nullptr));
#if QT_CONFIG(tooltip)
        _112->setToolTip(QCoreApplication::translate("MaskChange_Dialog", "Toggle the display of line weights in model views", nullptr));
#endif // QT_CONFIG(tooltip)
        _112->setText(QCoreApplication::translate("MaskChange_Dialog", "Display weights", nullptr));
        _100->setTabText(_100->indexOf(_102), QCoreApplication::translate("MaskChange_Dialog", "Weight", nullptr));
        _118->setText(QCoreApplication::translate("MaskChange_Dialog", "Search", nullptr));
        _117->setPlaceholderText(QCoreApplication::translate("MaskChange_Dialog", "Enter a colour number", nullptr));
        _106->setText(QCoreApplication::translate("MaskChange_Dialog", "Change", nullptr));
        _119->setText(QCoreApplication::translate("MaskChange_Dialog", "Reset", nullptr));
        _109->setText(QCoreApplication::translate("MaskChange_Dialog", "Load Palette", nullptr));
        _120->setText(QCoreApplication::translate("MaskChange_Dialog", "Save Palette", nullptr));
        _100->setTabText(_100->indexOf(_103), QCoreApplication::translate("MaskChange_Dialog", "Colour", nullptr));
        _114->setText(QCoreApplication::translate("MaskChange_Dialog", "Change", nullptr));
        _111->setText(QCoreApplication::translate("MaskChange_Dialog", "Define Pattern Style", nullptr));
        _100->setTabText(_100->indexOf(_104), QCoreApplication::translate("MaskChange_Dialog", "Style", nullptr));
        _100->setTabText(_100->indexOf(_105), QCoreApplication::translate("MaskChange_Dialog", "Entity", nullptr));
        _107->setText(QCoreApplication::translate("MaskChange_Dialog", "Clone", nullptr));
        _1->setText(QCoreApplication::translate("MaskChange_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("MaskChange_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaskChange_Dialog: public Ui_MaskChange_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MASKCHANGE_DIALOG_H
