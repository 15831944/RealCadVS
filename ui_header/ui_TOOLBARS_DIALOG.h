/********************************************************************************
** Form generated from reading UI file 'TOOLBARS_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBARS_DIALOG_H
#define UI_TOOLBARS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ToolBars_Dialog
{
public:
    QListWidget *_100;
    QPushButton *_102;
    QLabel *label;
    QLabel *label_2;
    QListWidget *_101;
    QPushButton *_2;
    QPushButton *_104;
    QPushButton *_103;
    QPushButton *_105;

    void setupUi(QDialog *ToolBars_Dialog)
    {
        if (ToolBars_Dialog->objectName().isEmpty())
            ToolBars_Dialog->setObjectName(QString::fromUtf8("ToolBars_Dialog"));
        ToolBars_Dialog->resize(508, 279);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ToolBars_Dialog->sizePolicy().hasHeightForWidth());
        ToolBars_Dialog->setSizePolicy(sizePolicy);
        ToolBars_Dialog->setMinimumSize(QSize(508, 279));
        ToolBars_Dialog->setMaximumSize(QSize(508, 279));
        _100 = new QListWidget(ToolBars_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(12, 40, 191, 192));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        _100->setFont(font);
        _100->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _100->setProperty("showDropIndicator", QVariant(false));
        _100->setSelectionBehavior(QAbstractItemView::SelectRows);
        _102 = new QPushButton(ToolBars_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(214, 92, 79, 23));
        QFont font1;
        font1.setPointSize(10);
        _102->setFont(font1);
        _102->setAutoDefault(false);
        label = new QLabel(ToolBars_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 14, 181, 20));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(ToolBars_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(302, 14, 181, 20));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);
        _101 = new QListWidget(ToolBars_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(304, 40, 191, 192));
        _101->setFont(font);
        _101->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _101->setProperty("showDropIndicator", QVariant(false));
        _101->setSelectionMode(QAbstractItemView::SingleSelection);
        _101->setSelectionBehavior(QAbstractItemView::SelectRows);
        _2 = new QPushButton(ToolBars_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(417, 242, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _104 = new QPushButton(ToolBars_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(328, 242, 75, 23));
        _104->setFont(font1);
        _103 = new QPushButton(ToolBars_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(214, 127, 79, 23));
        _103->setFont(font1);
        _103->setAutoDefault(false);
        _105 = new QPushButton(ToolBars_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(16, 242, 115, 23));
        _105->setAutoDefault(false);
        QWidget::setTabOrder(_100, _102);
        QWidget::setTabOrder(_102, _101);
        QWidget::setTabOrder(_101, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _2);

        retranslateUi(ToolBars_Dialog);

        _104->setDefault(true);


        QMetaObject::connectSlotsByName(ToolBars_Dialog);
    } // setupUi

    void retranslateUi(QDialog *ToolBars_Dialog)
    {
        ToolBars_Dialog->setWindowTitle(QCoreApplication::translate("ToolBars_Dialog", "Select the toolbars to display", nullptr));
        _102->setText(QCoreApplication::translate("ToolBars_Dialog", "Add >>", nullptr));
        label->setText(QCoreApplication::translate("ToolBars_Dialog", "Hidden toolbars", nullptr));
        label_2->setText(QCoreApplication::translate("ToolBars_Dialog", "Visible toolbars", nullptr));
        _2->setText(QCoreApplication::translate("ToolBars_Dialog", "Cancel", nullptr));
        _104->setText(QCoreApplication::translate("ToolBars_Dialog", "OK", nullptr));
        _103->setText(QCoreApplication::translate("ToolBars_Dialog", "<< Remove", nullptr));
        _105->setText(QCoreApplication::translate("ToolBars_Dialog", "Reset Toolbars", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToolBars_Dialog: public Ui_ToolBars_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBARS_DIALOG_H
