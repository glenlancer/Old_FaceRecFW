/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created: Fri Aug 17 22:16:33 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *camLabel;
    QComboBox *camCombBox;
    QLabel *commentLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *thresLabel;
    QDoubleSpinBox *thresDoubleSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(296, 172);
        verticalLayout = new QVBoxLayout(ConfigDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        camLabel = new QLabel(ConfigDialog);
        camLabel->setObjectName(QString::fromUtf8("camLabel"));

        horizontalLayout_2->addWidget(camLabel);

        camCombBox = new QComboBox(ConfigDialog);
        camCombBox->setObjectName(QString::fromUtf8("camCombBox"));

        horizontalLayout_2->addWidget(camCombBox);


        verticalLayout->addLayout(horizontalLayout_2);

        commentLabel = new QLabel(ConfigDialog);
        commentLabel->setObjectName(QString::fromUtf8("commentLabel"));

        verticalLayout->addWidget(commentLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        thresLabel = new QLabel(ConfigDialog);
        thresLabel->setObjectName(QString::fromUtf8("thresLabel"));

        horizontalLayout->addWidget(thresLabel);

        thresDoubleSpinBox = new QDoubleSpinBox(ConfigDialog);
        thresDoubleSpinBox->setObjectName(QString::fromUtf8("thresDoubleSpinBox"));

        horizontalLayout->addWidget(thresDoubleSpinBox);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(ConfigDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Configuration", 0, QApplication::UnicodeUTF8));
        camLabel->setText(QApplication::translate("ConfigDialog", "Choose Camera:", 0, QApplication::UnicodeUTF8));
        commentLabel->setText(QApplication::translate("ConfigDialog", "<html><head/><body><p><span style=\" font-weight:600;\">Threshold is used in Nearest Neighbor Classifier.</span></p><p><span style=\" font-weight:600;\">Normally it should be set between 0.5 and 0.8.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        thresLabel->setText(QApplication::translate("ConfigDialog", "Threahold:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
