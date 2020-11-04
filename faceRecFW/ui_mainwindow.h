/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Aug 1 20:53:20 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Start_WebCam;
    QAction *action_Start_Recognition;
    QAction *action_Exit;
    QAction *action_Add_Person;
    QAction *action_Training;
    QAction *action_About_Qt;
    QAction *action_About_faceRecFW;
    QAction *action_Configuration;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *webcamLabel;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *recPersonLabel;
    QGridLayout *gridLayout_2;
    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *genderLabel;
    QLineEdit *genderLineEdit;
    QLabel *ageLabel;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *ageLineEdit;
    QSpacerItem *horizontalSpacer;
    QLabel *departLabel;
    QLineEdit *departLineEdit;
    QLabel *teamLabel;
    QLineEdit *teamLineEdit;
    QLabel *descLabel;
    QPlainTextEdit *descPlainTextEdit;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QLabel *stateLabel;
    QProgressBar *progressBar;
    QLabel *currentFacelabel;
    QListWidget *listWidget;
    QLabel *normFacelabel;
    QMenuBar *menubar;
    QMenu *menu_Control;
    QMenu *menu_Manage;
    QMenu *menu_About;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1062, 774);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setDockNestingEnabled(false);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        action_Start_WebCam = new QAction(MainWindow);
        action_Start_WebCam->setObjectName(QString::fromUtf8("action_Start_WebCam"));
        action_Start_Recognition = new QAction(MainWindow);
        action_Start_Recognition->setObjectName(QString::fromUtf8("action_Start_Recognition"));
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        action_Add_Person = new QAction(MainWindow);
        action_Add_Person->setObjectName(QString::fromUtf8("action_Add_Person"));
        action_Training = new QAction(MainWindow);
        action_Training->setObjectName(QString::fromUtf8("action_Training"));
        action_About_Qt = new QAction(MainWindow);
        action_About_Qt->setObjectName(QString::fromUtf8("action_About_Qt"));
        action_About_faceRecFW = new QAction(MainWindow);
        action_About_faceRecFW->setObjectName(QString::fromUtf8("action_About_faceRecFW"));
        action_Configuration = new QAction(MainWindow);
        action_Configuration->setObjectName(QString::fromUtf8("action_Configuration"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        webcamLabel = new QLabel(centralwidget);
        webcamLabel->setObjectName(QString::fromUtf8("webcamLabel"));
        webcamLabel->setEnabled(true);
        sizePolicy.setHeightForWidth(webcamLabel->sizePolicy().hasHeightForWidth());
        webcamLabel->setSizePolicy(sizePolicy);
        webcamLabel->setMinimumSize(QSize(640, 480));
        webcamLabel->setFrameShape(QFrame::Box);

        horizontalLayout->addWidget(webcamLabel);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setAutoFillBackground(false);
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(false);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        recPersonLabel = new QLabel(groupBox);
        recPersonLabel->setObjectName(QString::fromUtf8("recPersonLabel"));
        sizePolicy.setHeightForWidth(recPersonLabel->sizePolicy().hasHeightForWidth());
        recPersonLabel->setSizePolicy(sizePolicy);
        recPersonLabel->setFrameShape(QFrame::Box);

        gridLayout_3->addWidget(recPersonLabel, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        idLabel = new QLabel(groupBox);
        idLabel->setObjectName(QString::fromUtf8("idLabel"));

        gridLayout_2->addWidget(idLabel, 0, 0, 1, 1);

        idLineEdit = new QLineEdit(groupBox);
        idLineEdit->setObjectName(QString::fromUtf8("idLineEdit"));
        idLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(idLineEdit, 0, 1, 1, 1);

        nameLabel = new QLabel(groupBox);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        gridLayout_2->addWidget(nameLabel, 1, 0, 1, 1);

        nameLineEdit = new QLineEdit(groupBox);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        nameLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(nameLineEdit, 1, 1, 1, 1);

        genderLabel = new QLabel(groupBox);
        genderLabel->setObjectName(QString::fromUtf8("genderLabel"));

        gridLayout_2->addWidget(genderLabel, 2, 0, 1, 1);

        genderLineEdit = new QLineEdit(groupBox);
        genderLineEdit->setObjectName(QString::fromUtf8("genderLineEdit"));
        genderLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(genderLineEdit, 2, 1, 1, 1);

        ageLabel = new QLabel(groupBox);
        ageLabel->setObjectName(QString::fromUtf8("ageLabel"));

        gridLayout_2->addWidget(ageLabel, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ageLineEdit = new QLineEdit(groupBox);
        ageLineEdit->setObjectName(QString::fromUtf8("ageLineEdit"));
        ageLineEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(ageLineEdit);

        horizontalSpacer = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 1, 1, 3);

        departLabel = new QLabel(groupBox);
        departLabel->setObjectName(QString::fromUtf8("departLabel"));

        gridLayout_3->addWidget(departLabel, 1, 0, 1, 1);

        departLineEdit = new QLineEdit(groupBox);
        departLineEdit->setObjectName(QString::fromUtf8("departLineEdit"));
        departLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(departLineEdit, 1, 1, 1, 1);

        teamLabel = new QLabel(groupBox);
        teamLabel->setObjectName(QString::fromUtf8("teamLabel"));

        gridLayout_3->addWidget(teamLabel, 1, 2, 1, 1);

        teamLineEdit = new QLineEdit(groupBox);
        teamLineEdit->setObjectName(QString::fromUtf8("teamLineEdit"));
        teamLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(teamLineEdit, 1, 3, 1, 1);

        descLabel = new QLabel(groupBox);
        descLabel->setObjectName(QString::fromUtf8("descLabel"));

        gridLayout_3->addWidget(descLabel, 2, 0, 1, 1);

        descPlainTextEdit = new QPlainTextEdit(groupBox);
        descPlainTextEdit->setObjectName(QString::fromUtf8("descPlainTextEdit"));

        gridLayout_3->addWidget(descPlainTextEdit, 2, 1, 2, 3);

        verticalSpacer = new QSpacerItem(20, 201, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 3, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stateLabel = new QLabel(centralwidget);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));

        gridLayout->addWidget(stateLabel, 0, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setContextMenuPolicy(Qt::DefaultContextMenu);
        progressBar->setLayoutDirection(Qt::LeftToRight);
        progressBar->setAutoFillBackground(false);
        progressBar->setValue(24);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);
        progressBar->setTextDirection(QProgressBar::TopToBottom);

        gridLayout->addWidget(progressBar, 0, 1, 1, 1);

        currentFacelabel = new QLabel(centralwidget);
        currentFacelabel->setObjectName(QString::fromUtf8("currentFacelabel"));
        currentFacelabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(currentFacelabel, 1, 0, 1, 1);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 1, 1, 2, 1);

        normFacelabel = new QLabel(centralwidget);
        normFacelabel->setObjectName(QString::fromUtf8("normFacelabel"));
        normFacelabel->setMinimumSize(QSize(128, 128));
        normFacelabel->setFrameShape(QFrame::Box);
        normFacelabel->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(normFacelabel, 2, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1062, 23));
        menu_Control = new QMenu(menubar);
        menu_Control->setObjectName(QString::fromUtf8("menu_Control"));
        menu_Manage = new QMenu(menubar);
        menu_Manage->setObjectName(QString::fromUtf8("menu_Manage"));
        menu_About = new QMenu(menubar);
        menu_About->setObjectName(QString::fromUtf8("menu_About"));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menubar->addAction(menu_Control->menuAction());
        menubar->addAction(menu_Manage->menuAction());
        menubar->addAction(menu_About->menuAction());
        menu_Control->addAction(action_Start_WebCam);
        menu_Control->addAction(action_Start_Recognition);
        menu_Control->addSeparator();
        menu_Control->addAction(action_Exit);
        menu_Manage->addAction(action_Add_Person);
        menu_Manage->addAction(action_Training);
        menu_Manage->addSeparator();
        menu_Manage->addAction(action_Configuration);
        menu_About->addAction(action_About_Qt);
        menu_About->addAction(action_About_faceRecFW);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_Start_WebCam->setText(QApplication::translate("MainWindow", "&Start WebCam", 0, QApplication::UnicodeUTF8));
        action_Start_Recognition->setText(QApplication::translate("MainWindow", "Start &Recognition", 0, QApplication::UnicodeUTF8));
        action_Exit->setText(QApplication::translate("MainWindow", "&Exit", 0, QApplication::UnicodeUTF8));
        action_Add_Person->setText(QApplication::translate("MainWindow", "&Add Person", 0, QApplication::UnicodeUTF8));
        action_Training->setText(QApplication::translate("MainWindow", "&Training", 0, QApplication::UnicodeUTF8));
        action_About_Qt->setText(QApplication::translate("MainWindow", "About &Qt", 0, QApplication::UnicodeUTF8));
        action_About_faceRecFW->setText(QApplication::translate("MainWindow", "&About faceRecFW", 0, QApplication::UnicodeUTF8));
        action_Configuration->setText(QApplication::translate("MainWindow", "&Configuration", 0, QApplication::UnicodeUTF8));
        webcamLabel->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Recognition Result", 0, QApplication::UnicodeUTF8));
        recPersonLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        idLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">ID:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">Name:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        genderLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">Gender:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        ageLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">Age:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        departLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">Department:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        teamLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#0000ff;\">Team:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        descLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; text-decoration: underline; color:#0000ff;\">Description:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        stateLabel->setText(QApplication::translate("MainWindow", "Nothing in Progress:", 0, QApplication::UnicodeUTF8));
        currentFacelabel->setText(QApplication::translate("MainWindow", "Current Norm Face", 0, QApplication::UnicodeUTF8));
        normFacelabel->setText(QString());
        menu_Control->setTitle(QApplication::translate("MainWindow", "&Control", 0, QApplication::UnicodeUTF8));
        menu_Manage->setTitle(QApplication::translate("MainWindow", "&Manage", 0, QApplication::UnicodeUTF8));
        menu_About->setTitle(QApplication::translate("MainWindow", "&About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
