/********************************************************************************
** Form generated from reading UI file 'LAYER_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYER_DIALOG_H
#define UI_LAYER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Layer_Dialog
{
public:
    QPushButton *_105;
    QPushButton *_102;
    QPushButton *_104;
    QLabel *label_3;
    QLineEdit *_101;
    QPushButton *_103;
    QScrollBar *_1103;
    QPushButton *_110;
    QPushButton *_111;
    QPushButton *_106;
    QPushButton *_108;
    QPushButton *_113;
    QPushButton *_107;
    QPushButton *_112;
    QPushButton *_114;
    QPushButton *_109;
    QPushButton *_2;
    QPushButton *_1;
    QSpinBox *_100;

    void setupUi(QDialog *Layer_Dialog)
    {
        if (Layer_Dialog->objectName().isEmpty())
            Layer_Dialog->setObjectName(QString::fromUtf8("Layer_Dialog"));
        Layer_Dialog->resize(609, 292);
        QFont font;
        font.setPointSize(10);
        Layer_Dialog->setFont(font);
        _105 = new QPushButton(Layer_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(480, 40, 51, 23));
        _102 = new QPushButton(Layer_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(360, 10, 75, 23));
        _104 = new QPushButton(Layer_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(180, 260, 75, 23));
        label_3 = new QLabel(Layer_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 10, 46, 21));
        _101 = new QLineEdit(Layer_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(170, 10, 181, 21));
        _103 = new QPushButton(Layer_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(20, 40, 411, 211));
        _1103 = new QScrollBar(Layer_Dialog);
        _1103->setObjectName(QString::fromUtf8("_1103"));
        _1103->setGeometry(QRect(430, 40, 20, 211));
        _1103->setOrientation(Qt::Vertical);
        _110 = new QPushButton(Layer_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(540, 40, 51, 23));
        _111 = new QPushButton(Layer_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(540, 70, 51, 23));
        _106 = new QPushButton(Layer_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(480, 70, 51, 23));
        _108 = new QPushButton(Layer_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(480, 130, 51, 23));
        _113 = new QPushButton(Layer_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(540, 130, 51, 23));
        _107 = new QPushButton(Layer_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(480, 100, 51, 23));
        _112 = new QPushButton(Layer_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(540, 100, 51, 23));
        _114 = new QPushButton(Layer_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(540, 160, 51, 23));
        _109 = new QPushButton(Layer_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(480, 160, 51, 23));
        _2 = new QPushButton(Layer_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(510, 260, 75, 23));
        _1 = new QPushButton(Layer_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(420, 260, 75, 23));
        _100 = new QSpinBox(Layer_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(70, 10, 76, 22));

        retranslateUi(Layer_Dialog);

        QMetaObject::connectSlotsByName(Layer_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Layer_Dialog)
    {
        Layer_Dialog->setWindowTitle(QCoreApplication::translate("Layer_Dialog", "Layer selection", nullptr));
        _105->setText(QCoreApplication::translate("Layer_Dialog", "0", nullptr));
        _102->setText(QCoreApplication::translate("Layer_Dialog", "Save name", nullptr));
        _104->setText(QCoreApplication::translate("Layer_Dialog", "Clone", nullptr));
        label_3->setText(QCoreApplication::translate("Layer_Dialog", "Layer", nullptr));
        _103->setText(QCoreApplication::translate("Layer_Dialog", "PushButton", nullptr));
        _110->setText(QCoreApplication::translate("Layer_Dialog", "500", nullptr));
        _111->setText(QCoreApplication::translate("Layer_Dialog", "600", nullptr));
        _106->setText(QCoreApplication::translate("Layer_Dialog", "100", nullptr));
        _108->setText(QCoreApplication::translate("Layer_Dialog", "300", nullptr));
        _113->setText(QCoreApplication::translate("Layer_Dialog", "800", nullptr));
        _107->setText(QCoreApplication::translate("Layer_Dialog", "200", nullptr));
        _112->setText(QCoreApplication::translate("Layer_Dialog", "700", nullptr));
        _114->setText(QCoreApplication::translate("Layer_Dialog", "900", nullptr));
        _109->setText(QCoreApplication::translate("Layer_Dialog", "400", nullptr));
        _2->setText(QCoreApplication::translate("Layer_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Layer_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Layer_Dialog: public Ui_Layer_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYER_DIALOG_H
