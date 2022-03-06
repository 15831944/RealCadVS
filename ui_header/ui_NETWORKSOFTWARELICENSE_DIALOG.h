/********************************************************************************
** Form generated from reading UI file 'NETWORKSOFTWARELICENSE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKSOFTWARELICENSE_DIALOG_H
#define UI_NETWORKSOFTWARELICENSE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_NetworkSoftwareLicense_Dialog
{
public:
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *_100;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *_101;
    QPushButton *_102;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;

    void setupUi(QDialog *NetworkSoftwareLicense_Dialog)
    {
        if (NetworkSoftwareLicense_Dialog->objectName().isEmpty())
            NetworkSoftwareLicense_Dialog->setObjectName(QString::fromUtf8("NetworkSoftwareLicense_Dialog"));
        NetworkSoftwareLicense_Dialog->resize(528, 259);
        NetworkSoftwareLicense_Dialog->setMinimumSize(QSize(528, 259));
        NetworkSoftwareLicense_Dialog->setMaximumSize(QSize(554, 259));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        NetworkSoftwareLicense_Dialog->setFont(font);
        label_3 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 10, 491, 21));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 30, 491, 21));
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 491, 21));
        label_5->setAlignment(Qt::AlignCenter);
        _100 = new QPushButton(NetworkSoftwareLicense_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(10, 90, 151, 23));
        label_6 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(170, 90, 371, 21));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(170, 120, 371, 21));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QPushButton(NetworkSoftwareLicense_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(10, 120, 151, 23));
        _101->setAutoDefault(false);
        _102 = new QPushButton(NetworkSoftwareLicense_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(10, 210, 151, 23));
        _102->setAutoDefault(false);
        label_8 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(170, 210, 371, 21));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(171, 150, 371, 21));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_10 = new QLabel(NetworkSoftwareLicense_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(171, 170, 371, 21));
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        retranslateUi(NetworkSoftwareLicense_Dialog);

        _100->setDefault(true);


        QMetaObject::connectSlotsByName(NetworkSoftwareLicense_Dialog);
    } // setupUi

    void retranslateUi(QDialog *NetworkSoftwareLicense_Dialog)
    {
        NetworkSoftwareLicense_Dialog->setWindowTitle(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "Network Software License", nullptr));
        label_3->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "The server containing valid registration keys could not be found", nullptr));
        label_4->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "Please contact the network administrator. The software may be operated", nullptr));
        label_5->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "in demonstration mode which allows only small models to be saved.", nullptr));
        _100->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "Retry", nullptr));
        label_6->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "To retry the network server, click Retry.", nullptr));
        label_7->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "To operate in demonstration mode, click Demonstration Mode.", nullptr));
        _101->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "Demonstration Mode", nullptr));
        _102->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "Exit CAD", nullptr));
        label_8->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "To exit the software, click Exit CAD.", nullptr));
        label_9->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "In demonstration mode, only small models may be saved,", nullptr));
        label_10->setText(QCoreApplication::translate("NetworkSoftwareLicense_Dialog", "and no application entities (such as walls) may be saved.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NetworkSoftwareLicense_Dialog: public Ui_NetworkSoftwareLicense_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKSOFTWARELICENSE_DIALOG_H
