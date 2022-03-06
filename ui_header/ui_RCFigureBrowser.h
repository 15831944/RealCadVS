/********************************************************************************
** Form generated from reading UI file 'RCFigureBrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCFIGUREBROWSER_H
#define UI_RCFIGUREBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_RCFigureBrowser
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QTreeView *IDC_FOLDER_TREE;
    QScrollArea *IDC_STATICPIC;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QGraphicsView *IDC_FIGURE_PICTURE;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *IDC_FILENAME_COMBO;
    QStackedWidget *stackedWidget;
    QWidget *figure;
    QGroupBox *SettingsGroup;
    RCDoubleSpinBox *YScale;
    RCDoubleSpinBox *XScale;
    QLabel *Xlabel;
    QLabel *Ylabel;
    RCDoubleSpinBox *ZScale;
    QLabel *Zlabel;
    QCheckBox *originalLayer;
    QCheckBox *originalStyle;
    QCheckBox *originalColour;
    QCheckBox *originalWeight;
    QCheckBox *orientToView;
    RCDoubleSpinBox *YSize;
    RCDoubleSpinBox *ZSize;
    RCDoubleSpinBox *XSize;
    QPushButton *_1008;
    QCheckBox *SizeCheckBox;
    QCheckBox *ScaleCheckBox;
    QWidget *bitmap;
    QGroupBox *SettingsGroup_2;
    RCDoubleSpinBox *Height;
    RCDoubleSpinBox *Width;
    QLabel *WLabel;
    QLabel *HLabel;
    QCheckBox *originalShape;
    QCheckBox *linkToFile;
    QWidget *none;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QComboBox *IDC_FOLDER_COMBO;
    QPushButton *IDC_BROWSE_BTN;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *ID_OK;
    QPushButton *ID_CANCEL;

    void setupUi(QDialog *RCFigureBrowser)
    {
        if (RCFigureBrowser->objectName().isEmpty())
            RCFigureBrowser->setObjectName(QString::fromUtf8("RCFigureBrowser"));
        RCFigureBrowser->resize(970, 587);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RCFigureBrowser->sizePolicy().hasHeightForWidth());
        RCFigureBrowser->setSizePolicy(sizePolicy);
        RCFigureBrowser->setMinimumSize(QSize(970, 587));
        RCFigureBrowser->setMaximumSize(QSize(1500, 587));
        RCFigureBrowser->setSizeIncrement(QSize(5, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        RCFigureBrowser->setFont(font);
        RCFigureBrowser->setSizeGripEnabled(true);
        verticalLayout_2 = new QVBoxLayout(RCFigureBrowser);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(5);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        IDC_FOLDER_TREE = new QTreeView(RCFigureBrowser);
        IDC_FOLDER_TREE->setObjectName(QString::fromUtf8("IDC_FOLDER_TREE"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(IDC_FOLDER_TREE->sizePolicy().hasHeightForWidth());
        IDC_FOLDER_TREE->setSizePolicy(sizePolicy1);
        IDC_FOLDER_TREE->setMinimumSize(QSize(215, 0));
        IDC_FOLDER_TREE->setMaximumSize(QSize(16777215, 16777215));
        IDC_FOLDER_TREE->setProperty("showDropIndicator", QVariant(false));
        IDC_FOLDER_TREE->setRootIsDecorated(false);
        IDC_FOLDER_TREE->header()->setCascadingSectionResizes(true);
        IDC_FOLDER_TREE->header()->setMinimumSectionSize(100);

        horizontalLayout_3->addWidget(IDC_FOLDER_TREE);

        IDC_STATICPIC = new QScrollArea(RCFigureBrowser);
        IDC_STATICPIC->setObjectName(QString::fromUtf8("IDC_STATICPIC"));
        sizePolicy1.setHeightForWidth(IDC_STATICPIC->sizePolicy().hasHeightForWidth());
        IDC_STATICPIC->setSizePolicy(sizePolicy1);
        IDC_STATICPIC->setMinimumSize(QSize(420, 518));
        IDC_STATICPIC->setMaximumSize(QSize(1500, 1500));
        IDC_STATICPIC->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        IDC_STATICPIC->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        IDC_STATICPIC->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        IDC_STATICPIC->setWidgetResizable(true);
        IDC_STATICPIC->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 403, 519));
        IDC_STATICPIC->setWidget(scrollAreaWidgetContents);

        horizontalLayout_3->addWidget(IDC_STATICPIC);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(-1, 0, -1, 0);
        IDC_FIGURE_PICTURE = new QGraphicsView(RCFigureBrowser);
        IDC_FIGURE_PICTURE->setObjectName(QString::fromUtf8("IDC_FIGURE_PICTURE"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(IDC_FIGURE_PICTURE->sizePolicy().hasHeightForWidth());
        IDC_FIGURE_PICTURE->setSizePolicy(sizePolicy2);
        IDC_FIGURE_PICTURE->setMinimumSize(QSize(301, 280));
        IDC_FIGURE_PICTURE->setMaximumSize(QSize(301, 280));
        IDC_FIGURE_PICTURE->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        IDC_FIGURE_PICTURE->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        IDC_FIGURE_PICTURE->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        IDC_FIGURE_PICTURE->setInteractive(false);
        IDC_FIGURE_PICTURE->setSceneRect(QRectF(0, 0, 301, 301));
        IDC_FIGURE_PICTURE->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
        IDC_FIGURE_PICTURE->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        IDC_FIGURE_PICTURE->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        verticalLayout->addWidget(IDC_FIGURE_PICTURE);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(RCFigureBrowser);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMargin(2);

        horizontalLayout_2->addWidget(label_4);

        IDC_FILENAME_COMBO = new QComboBox(RCFigureBrowser);
        IDC_FILENAME_COMBO->setObjectName(QString::fromUtf8("IDC_FILENAME_COMBO"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(IDC_FILENAME_COMBO->sizePolicy().hasHeightForWidth());
        IDC_FILENAME_COMBO->setSizePolicy(sizePolicy3);
        IDC_FILENAME_COMBO->setMinimumSize(QSize(230, 26));
        IDC_FILENAME_COMBO->setMaximumSize(QSize(230, 26));
        IDC_FILENAME_COMBO->setEditable(false);
        IDC_FILENAME_COMBO->setInsertPolicy(QComboBox::InsertAtTop);

        horizontalLayout_2->addWidget(IDC_FILENAME_COMBO);


        verticalLayout->addLayout(horizontalLayout_2);

        stackedWidget = new QStackedWidget(RCFigureBrowser);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        sizePolicy2.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy2);
        stackedWidget->setMinimumSize(QSize(301, 185));
        stackedWidget->setMaximumSize(QSize(301, 185));
        figure = new QWidget();
        figure->setObjectName(QString::fromUtf8("figure"));
        SettingsGroup = new QGroupBox(figure);
        SettingsGroup->setObjectName(QString::fromUtf8("SettingsGroup"));
        SettingsGroup->setGeometry(QRect(4, 8, 293, 175));
        SettingsGroup->setMinimumSize(QSize(293, 175));
        YScale = new RCDoubleSpinBox(SettingsGroup);
        YScale->setObjectName(QString::fromUtf8("YScale"));
        YScale->setGeometry(QRect(172, 68, 80, 24));
        YScale->setMinimum(-1000000.000000000000000);
        YScale->setMaximum(1000000.000000000000000);
        YScale->setValue(1.000000000000000);
        XScale = new RCDoubleSpinBox(SettingsGroup);
        XScale->setObjectName(QString::fromUtf8("XScale"));
        XScale->setGeometry(QRect(172, 44, 80, 24));
        XScale->setMinimum(-1000000.000000000000000);
        XScale->setMaximum(1000000.000000000000000);
        XScale->setValue(1.000000000000000);
        Xlabel = new QLabel(SettingsGroup);
        Xlabel->setObjectName(QString::fromUtf8("Xlabel"));
        Xlabel->setGeometry(QRect(26, 48, 21, 16));
        QFont font1;
        font1.setPointSize(11);
        Xlabel->setFont(font1);
        Xlabel->setLayoutDirection(Qt::RightToLeft);
        Xlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        Ylabel = new QLabel(SettingsGroup);
        Ylabel->setObjectName(QString::fromUtf8("Ylabel"));
        Ylabel->setGeometry(QRect(26, 72, 21, 16));
        Ylabel->setFont(font1);
        Ylabel->setLayoutDirection(Qt::RightToLeft);
        Ylabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ZScale = new RCDoubleSpinBox(SettingsGroup);
        ZScale->setObjectName(QString::fromUtf8("ZScale"));
        ZScale->setGeometry(QRect(172, 92, 80, 24));
        ZScale->setMinimum(-1000000.000000000000000);
        ZScale->setMaximum(1000000.000000000000000);
        ZScale->setValue(1.000000000000000);
        Zlabel = new QLabel(SettingsGroup);
        Zlabel->setObjectName(QString::fromUtf8("Zlabel"));
        Zlabel->setGeometry(QRect(26, 96, 21, 16));
        Zlabel->setFont(font1);
        Zlabel->setLayoutDirection(Qt::RightToLeft);
        Zlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        originalLayer = new QCheckBox(SettingsGroup);
        originalLayer->setObjectName(QString::fromUtf8("originalLayer"));
        originalLayer->setGeometry(QRect(2, 118, 129, 20));
        originalLayer->setFont(font1);
        originalLayer->setLayoutDirection(Qt::RightToLeft);
        originalStyle = new QCheckBox(SettingsGroup);
        originalStyle->setObjectName(QString::fromUtf8("originalStyle"));
        originalStyle->setGeometry(QRect(136, 118, 139, 20));
        originalStyle->setFont(font1);
        originalStyle->setLayoutDirection(Qt::RightToLeft);
        originalColour = new QCheckBox(SettingsGroup);
        originalColour->setObjectName(QString::fromUtf8("originalColour"));
        originalColour->setGeometry(QRect(2, 136, 129, 20));
        originalColour->setFont(font1);
        originalColour->setLayoutDirection(Qt::RightToLeft);
        originalWeight = new QCheckBox(SettingsGroup);
        originalWeight->setObjectName(QString::fromUtf8("originalWeight"));
        originalWeight->setGeometry(QRect(136, 136, 139, 20));
        originalWeight->setFont(font1);
        originalWeight->setLayoutDirection(Qt::RightToLeft);
        orientToView = new QCheckBox(SettingsGroup);
        orientToView->setObjectName(QString::fromUtf8("orientToView"));
        orientToView->setGeometry(QRect(38, 152, 183, 20));
        orientToView->setFont(font1);
        orientToView->setLayoutDirection(Qt::RightToLeft);
        YSize = new RCDoubleSpinBox(SettingsGroup);
        YSize->setObjectName(QString::fromUtf8("YSize"));
        YSize->setGeometry(QRect(80, 68, 80, 24));
        YSize->setMinimum(-1000000.000000000000000);
        YSize->setMaximum(1000000.000000000000000);
        YSize->setValue(0.000000000000000);
        ZSize = new RCDoubleSpinBox(SettingsGroup);
        ZSize->setObjectName(QString::fromUtf8("ZSize"));
        ZSize->setGeometry(QRect(80, 92, 80, 24));
        ZSize->setMinimum(-1000000.000000000000000);
        ZSize->setMaximum(1000000.000000000000000);
        ZSize->setValue(0.000000000000000);
        XSize = new RCDoubleSpinBox(SettingsGroup);
        XSize->setObjectName(QString::fromUtf8("XSize"));
        XSize->setGeometry(QRect(80, 44, 80, 24));
        XSize->setMinimum(-1000000.000000000000000);
        XSize->setMaximum(1000000.000000000000000);
        XSize->setValue(0.000000000000000);
        _1008 = new QPushButton(SettingsGroup);
        _1008->setObjectName(QString::fromUtf8("_1008"));
        _1008->setGeometry(QRect(56, 46, 19, 47));
        _1008->setMouseTracking(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/images/LinkIcon.png"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Disabled, QIcon::Off);
        _1008->setIcon(icon);
        _1008->setIconSize(QSize(20, 30));
        _1008->setCheckable(true);
        _1008->setChecked(true);
        _1008->setAutoDefault(false);
        _1008->setFlat(true);
        SizeCheckBox = new QCheckBox(SettingsGroup);
        SizeCheckBox->setObjectName(QString::fromUtf8("SizeCheckBox"));
        SizeCheckBox->setGeometry(QRect(86, 22, 53, 20));
        SizeCheckBox->setFont(font1);
        SizeCheckBox->setLayoutDirection(Qt::RightToLeft);
        ScaleCheckBox = new QCheckBox(SettingsGroup);
        ScaleCheckBox->setObjectName(QString::fromUtf8("ScaleCheckBox"));
        ScaleCheckBox->setGeometry(QRect(178, 22, 53, 20));
        ScaleCheckBox->setFont(font1);
        ScaleCheckBox->setLayoutDirection(Qt::RightToLeft);
        stackedWidget->addWidget(figure);
        bitmap = new QWidget();
        bitmap->setObjectName(QString::fromUtf8("bitmap"));
        SettingsGroup_2 = new QGroupBox(bitmap);
        SettingsGroup_2->setObjectName(QString::fromUtf8("SettingsGroup_2"));
        SettingsGroup_2->setGeometry(QRect(4, 8, 293, 175));
        Height = new RCDoubleSpinBox(SettingsGroup_2);
        Height->setObjectName(QString::fromUtf8("Height"));
        Height->setGeometry(QRect(142, 62, 70, 24));
        Height->setMinimum(-100000000.000000000000000);
        Height->setMaximum(100000000.000000000000000);
        Height->setValue(1.000000000000000);
        Width = new RCDoubleSpinBox(SettingsGroup_2);
        Width->setObjectName(QString::fromUtf8("Width"));
        Width->setGeometry(QRect(142, 36, 70, 24));
        Width->setMinimum(-100000000.000000000000000);
        Width->setMaximum(100000000.000000000000000);
        Width->setValue(1.000000000000000);
        WLabel = new QLabel(SettingsGroup_2);
        WLabel->setObjectName(QString::fromUtf8("WLabel"));
        WLabel->setGeometry(QRect(80, 40, 55, 16));
        HLabel = new QLabel(SettingsGroup_2);
        HLabel->setObjectName(QString::fromUtf8("HLabel"));
        HLabel->setGeometry(QRect(76, 66, 61, 16));
        originalShape = new QCheckBox(SettingsGroup_2);
        originalShape->setObjectName(QString::fromUtf8("originalShape"));
        originalShape->setGeometry(QRect(74, 110, 127, 20));
        originalShape->setLayoutDirection(Qt::RightToLeft);
        linkToFile = new QCheckBox(SettingsGroup_2);
        linkToFile->setObjectName(QString::fromUtf8("linkToFile"));
        linkToFile->setGeometry(QRect(70, 132, 131, 20));
        linkToFile->setLayoutDirection(Qt::RightToLeft);
        stackedWidget->addWidget(bitmap);
        none = new QWidget();
        none->setObjectName(QString::fromUtf8("none"));
        stackedWidget->addWidget(none);

        verticalLayout->addWidget(stackedWidget);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_3 = new QLabel(RCFigureBrowser);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(56, 0));
        label_3->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label_3);

        IDC_FOLDER_COMBO = new QComboBox(RCFigureBrowser);
        IDC_FOLDER_COMBO->setObjectName(QString::fromUtf8("IDC_FOLDER_COMBO"));
        sizePolicy2.setHeightForWidth(IDC_FOLDER_COMBO->sizePolicy().hasHeightForWidth());
        IDC_FOLDER_COMBO->setSizePolicy(sizePolicy2);
        IDC_FOLDER_COMBO->setMinimumSize(QSize(500, 28));
        IDC_FOLDER_COMBO->setMaximumSize(QSize(500, 28));
        IDC_FOLDER_COMBO->setEditable(true);
        IDC_FOLDER_COMBO->setMaxCount(10);
        IDC_FOLDER_COMBO->setInsertPolicy(QComboBox::InsertAtTop);

        horizontalLayout->addWidget(IDC_FOLDER_COMBO);

        IDC_BROWSE_BTN = new QPushButton(RCFigureBrowser);
        IDC_BROWSE_BTN->setObjectName(QString::fromUtf8("IDC_BROWSE_BTN"));
        IDC_BROWSE_BTN->setEnabled(true);
        IDC_BROWSE_BTN->setMinimumSize(QSize(81, 0));
        IDC_BROWSE_BTN->setMaximumSize(QSize(85, 16777215));
        IDC_BROWSE_BTN->setAutoDefault(false);

        horizontalLayout->addWidget(IDC_BROWSE_BTN);

        horizontalSpacer_2 = new QSpacerItem(150, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        ID_OK = new QPushButton(RCFigureBrowser);
        ID_OK->setObjectName(QString::fromUtf8("ID_OK"));
        ID_OK->setEnabled(true);
        ID_OK->setMinimumSize(QSize(81, 0));
        ID_OK->setMaximumSize(QSize(81, 16777215));

        horizontalLayout->addWidget(ID_OK);

        ID_CANCEL = new QPushButton(RCFigureBrowser);
        ID_CANCEL->setObjectName(QString::fromUtf8("ID_CANCEL"));
        ID_CANCEL->setEnabled(true);
        ID_CANCEL->setMinimumSize(QSize(81, 0));
        ID_CANCEL->setMaximumSize(QSize(81, 16777215));
        ID_CANCEL->setAutoDefault(false);

        horizontalLayout->addWidget(ID_CANCEL);


        verticalLayout_2->addLayout(horizontalLayout);

        QWidget::setTabOrder(IDC_FILENAME_COMBO, IDC_STATICPIC);
        QWidget::setTabOrder(IDC_STATICPIC, IDC_FOLDER_COMBO);
        QWidget::setTabOrder(IDC_FOLDER_COMBO, IDC_BROWSE_BTN);
        QWidget::setTabOrder(IDC_BROWSE_BTN, XScale);
        QWidget::setTabOrder(XScale, YScale);
        QWidget::setTabOrder(YScale, originalLayer);
        QWidget::setTabOrder(originalLayer, originalStyle);
        QWidget::setTabOrder(originalStyle, originalColour);
        QWidget::setTabOrder(originalColour, originalWeight);
        QWidget::setTabOrder(originalWeight, orientToView);
        QWidget::setTabOrder(orientToView, ID_OK);
        QWidget::setTabOrder(ID_OK, ID_CANCEL);
        QWidget::setTabOrder(ID_CANCEL, IDC_FOLDER_TREE);
        QWidget::setTabOrder(IDC_FOLDER_TREE, IDC_FIGURE_PICTURE);
        QWidget::setTabOrder(IDC_FIGURE_PICTURE, ZScale);
        QWidget::setTabOrder(ZScale, Height);
        QWidget::setTabOrder(Height, Width);
        QWidget::setTabOrder(Width, originalShape);
        QWidget::setTabOrder(originalShape, linkToFile);

        retranslateUi(RCFigureBrowser);

        stackedWidget->setCurrentIndex(0);
        ID_OK->setDefault(true);


        QMetaObject::connectSlotsByName(RCFigureBrowser);
    } // setupUi

    void retranslateUi(QDialog *RCFigureBrowser)
    {
        RCFigureBrowser->setWindowTitle(QCoreApplication::translate("RCFigureBrowser", "Select Component", nullptr));
        label_4->setText(QCoreApplication::translate("RCFigureBrowser", "File Name", nullptr));
        SettingsGroup->setTitle(QCoreApplication::translate("RCFigureBrowser", "Settings", nullptr));
        Xlabel->setText(QCoreApplication::translate("RCFigureBrowser", "X", nullptr));
        Ylabel->setText(QCoreApplication::translate("RCFigureBrowser", "Y", nullptr));
        Zlabel->setText(QCoreApplication::translate("RCFigureBrowser", "Z", nullptr));
        originalLayer->setText(QCoreApplication::translate("RCFigureBrowser", "Use original layer", nullptr));
        originalStyle->setText(QCoreApplication::translate("RCFigureBrowser", "Use original style", nullptr));
        originalColour->setText(QCoreApplication::translate("RCFigureBrowser", "Use original colour", nullptr));
        originalWeight->setText(QCoreApplication::translate("RCFigureBrowser", "Use original weight", nullptr));
        orientToView->setText(QCoreApplication::translate("RCFigureBrowser", "Orient component to view", nullptr));
#if QT_CONFIG(tooltip)
        _1008->setToolTip(QCoreApplication::translate("RCFigureBrowser", "Link/Unlink", nullptr));
#endif // QT_CONFIG(tooltip)
        _1008->setText(QString());
        SizeCheckBox->setText(QCoreApplication::translate("RCFigureBrowser", "Size", nullptr));
        ScaleCheckBox->setText(QCoreApplication::translate("RCFigureBrowser", "Scale", nullptr));
        SettingsGroup_2->setTitle(QCoreApplication::translate("RCFigureBrowser", "Settings", nullptr));
        WLabel->setText(QCoreApplication::translate("RCFigureBrowser", "Width", nullptr));
        HLabel->setText(QCoreApplication::translate("RCFigureBrowser", "Height", nullptr));
        originalShape->setText(QCoreApplication::translate("RCFigureBrowser", "Original Shape", nullptr));
        linkToFile->setText(QCoreApplication::translate("RCFigureBrowser", "Link to filename", nullptr));
        label_3->setText(QCoreApplication::translate("RCFigureBrowser", "Directory", nullptr));
        IDC_BROWSE_BTN->setText(QCoreApplication::translate("RCFigureBrowser", "Browse...", nullptr));
        ID_OK->setText(QCoreApplication::translate("RCFigureBrowser", "OK", nullptr));
        ID_CANCEL->setText(QCoreApplication::translate("RCFigureBrowser", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RCFigureBrowser: public Ui_RCFigureBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCFIGUREBROWSER_H
