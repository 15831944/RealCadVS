/********************************************************************************
** Form generated from reading UI file 'INFORMATION_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATION_DIALOG_H
#define UI_INFORMATION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Information_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *_100;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *Information_Dialog)
    {
        if (Information_Dialog->objectName().isEmpty())
            Information_Dialog->setObjectName(QString::fromUtf8("Information_Dialog"));
        Information_Dialog->resize(481, 278);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Information_Dialog->sizePolicy().hasHeightForWidth());
        Information_Dialog->setSizePolicy(sizePolicy);
        Information_Dialog->setMinimumSize(QSize(481, 259));
        Information_Dialog->setMaximumSize(QSize(481, 100000));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        Information_Dialog->setFont(font);
        Information_Dialog->setModal(true);
        verticalLayout = new QVBoxLayout(Information_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _100 = new QTableWidget(Information_Dialog);
        if (_100->columnCount() < 2)
            _100->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        _100->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        _100->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (_100->rowCount() < 50)
            _100->setRowCount(50);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setFont(font);
        _100->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::SelectedClicked);
        _100->setTabKeyNavigation(true);
        _100->setProperty("showDropIndicator", QVariant(true));
        _100->setDragEnabled(true);
        _100->setDragDropOverwriteMode(true);
        _100->setDragDropMode(QAbstractItemView::DragDrop);
        _100->setDefaultDropAction(Qt::CopyAction);
        _100->setWordWrap(false);
        _100->setCornerButtonEnabled(false);
        _100->setRowCount(50);
        _100->horizontalHeader()->setMinimumSectionSize(175);
        _100->horizontalHeader()->setDefaultSectionSize(175);
        _100->horizontalHeader()->setStretchLastSection(true);
        _100->verticalHeader()->setVisible(false);
        _100->verticalHeader()->setMinimumSectionSize(16);
        _100->verticalHeader()->setDefaultSectionSize(16);

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 4, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(Information_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout->addWidget(_1, 0, Qt::AlignVCenter);

        _2 = new QPushButton(Information_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setAutoDefault(false);

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Information_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Information_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Information_Dialog)
    {
        Information_Dialog->setWindowTitle(QCoreApplication::translate("Information_Dialog", "Information options", nullptr));
        QTableWidgetItem *___qtablewidgetitem = _100->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Information_Dialog", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = _100->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Information_Dialog", "Description", nullptr));
        _1->setText(QCoreApplication::translate("Information_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Information_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Information_Dialog: public Ui_Information_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATION_DIALOG_H
