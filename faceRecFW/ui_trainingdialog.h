/********************************************************************************
** Form generated from reading UI file 'trainingdialog.ui'
**
** Created: Sun Jul 29 19:20:09 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAININGDIALOG_H
#define UI_TRAININGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TrainingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QProgressBar *progressBar;
    QPlainTextEdit *consoleTextEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *trainingPushButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *closePushButton;
    QPushButton *savePushButton;

    void setupUi(QDialog *TrainingDialog)
    {
        if (TrainingDialog->objectName().isEmpty())
            TrainingDialog->setObjectName(QString::fromUtf8("TrainingDialog"));
        TrainingDialog->resize(579, 519);
        verticalLayout = new QVBoxLayout(TrainingDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(TrainingDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAutoFillBackground(false);
        label->setScaledContents(false);
        label->setWordWrap(false);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        progressBar = new QProgressBar(TrainingDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setAutoFillBackground(false);
        progressBar->setValue(20);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);
        progressBar->setTextDirection(QProgressBar::TopToBottom);

        verticalLayout->addWidget(progressBar);

        consoleTextEdit = new QPlainTextEdit(TrainingDialog);
        consoleTextEdit->setObjectName(QString::fromUtf8("consoleTextEdit"));
        consoleTextEdit->setReadOnly(false);
        consoleTextEdit->setTextInteractionFlags(Qt::TextEditorInteraction);

        verticalLayout->addWidget(consoleTextEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        trainingPushButton = new QPushButton(TrainingDialog);
        trainingPushButton->setObjectName(QString::fromUtf8("trainingPushButton"));

        horizontalLayout_2->addWidget(trainingPushButton);

        horizontalSpacer_3 = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        closePushButton = new QPushButton(TrainingDialog);
        closePushButton->setObjectName(QString::fromUtf8("closePushButton"));

        horizontalLayout_2->addWidget(closePushButton);

        savePushButton = new QPushButton(TrainingDialog);
        savePushButton->setObjectName(QString::fromUtf8("savePushButton"));

        horizontalLayout_2->addWidget(savePushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(TrainingDialog);

        QMetaObject::connectSlotsByName(TrainingDialog);
    } // setupUi

    void retranslateUi(QDialog *TrainingDialog)
    {
        TrainingDialog->setWindowTitle(QApplication::translate("TrainingDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TrainingDialog", "Training in Progress", 0, QApplication::UnicodeUTF8));
        trainingPushButton->setText(QApplication::translate("TrainingDialog", "Start Training", 0, QApplication::UnicodeUTF8));
        closePushButton->setText(QApplication::translate("TrainingDialog", "Close", 0, QApplication::UnicodeUTF8));
        savePushButton->setText(QApplication::translate("TrainingDialog", "Save", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TrainingDialog: public Ui_TrainingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAININGDIALOG_H
