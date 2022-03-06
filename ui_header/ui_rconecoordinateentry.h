/********************************************************************************
** Form generated from reading UI file 'rconecoordinateentry.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCONECOORDINATEENTRY_H
#define UI_RCONECOORDINATEENTRY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RCOneCoordinateEntry
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *Label;
    QLabel *Icon;
    QLineEdit *Edit;

    void setupUi(QWidget *RCOneCoordinateEntry)
    {
        if (RCOneCoordinateEntry->objectName().isEmpty())
            RCOneCoordinateEntry->setObjectName(QString::fromUtf8("RCOneCoordinateEntry"));
        RCOneCoordinateEntry->resize(176, 28);
        verticalLayout = new QVBoxLayout(RCOneCoordinateEntry);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(-1);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Label = new QLabel(RCOneCoordinateEntry);
        Label->setObjectName(QString::fromUtf8("Label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Label->sizePolicy().hasHeightForWidth());
        Label->setSizePolicy(sizePolicy);
        Label->setMinimumSize(QSize(24, 0));
        Label->setMaximumSize(QSize(24, 16777215));
        QFont font;
        font.setPointSize(14);
        Label->setFont(font);
        Label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        horizontalLayout->addWidget(Label);

        Icon = new QLabel(RCOneCoordinateEntry);
        Icon->setObjectName(QString::fromUtf8("Icon"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Icon->sizePolicy().hasHeightForWidth());
        Icon->setSizePolicy(sizePolicy1);
        Icon->setMinimumSize(QSize(24, 24));
        Icon->setMaximumSize(QSize(24, 24));
        Icon->setTextFormat(Qt::PlainText);
        Icon->setPixmap(QPixmap(QString::fromUtf8(":/images/COORDINATE-unlocked.svg")));
        Icon->setScaledContents(true);
        Icon->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Icon);

        Edit = new QLineEdit(RCOneCoordinateEntry);
        Edit->setObjectName(QString::fromUtf8("Edit"));
        Edit->setMinimumSize(QSize(100, 24));
        Edit->setMaximumSize(QSize(100, 24));

        horizontalLayout->addWidget(Edit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(RCOneCoordinateEntry);

        QMetaObject::connectSlotsByName(RCOneCoordinateEntry);
    } // setupUi

    void retranslateUi(QWidget *RCOneCoordinateEntry)
    {
        RCOneCoordinateEntry->setWindowTitle(QCoreApplication::translate("RCOneCoordinateEntry", "Form", nullptr));
        Label->setText(QCoreApplication::translate("RCOneCoordinateEntry", "XX", nullptr));
        Icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RCOneCoordinateEntry: public Ui_RCOneCoordinateEntry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCONECOORDINATEENTRY_H
