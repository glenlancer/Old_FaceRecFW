/********************************************************************************
** Form generated from reading UI file 'addpersondialog.ui'
**
** Created: Sun Jul 29 00:18:46 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDPERSONDIALOG_H
#define UI_ADDPERSONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddPersonDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *iDLineEdit;
    QLabel *label_2;
    QLineEdit *nameLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QRadioButton *maleRadioButton;
    QRadioButton *femaleRadioButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSpinBox *ageSpinBox;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *departLineEdit;
    QLabel *label_7;
    QLineEdit *teamLineEdit;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *label_8;
    QSpacerItem *verticalSpacer;
    QPlainTextEdit *descriptionText;
    QDialogButtonBox *buttonBox;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *AddPersonDialog)
    {
        if (AddPersonDialog->objectName().isEmpty())
            AddPersonDialog->setObjectName(QString::fromUtf8("AddPersonDialog"));
        AddPersonDialog->resize(456, 352);
        QIcon icon;
        icon.addFile(QString::fromUtf8("resources/person.png"), QSize(), QIcon::Normal, QIcon::Off);
        AddPersonDialog->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(AddPersonDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_6 = new QLabel(AddPersonDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(71, 0));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_6->setFont(font);

        verticalLayout_2->addWidget(label_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(AddPersonDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(71, 0));

        horizontalLayout->addWidget(label);

        iDLineEdit = new QLineEdit(AddPersonDialog);
        iDLineEdit->setObjectName(QString::fromUtf8("iDLineEdit"));

        horizontalLayout->addWidget(iDLineEdit);

        label_2 = new QLabel(AddPersonDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(71, 0));

        horizontalLayout->addWidget(label_2);

        nameLineEdit = new QLineEdit(AddPersonDialog);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        horizontalLayout->addWidget(nameLineEdit);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(AddPersonDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(71, 0));

        horizontalLayout_2->addWidget(label_3);

        maleRadioButton = new QRadioButton(AddPersonDialog);
        buttonGroup = new QButtonGroup(AddPersonDialog);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(maleRadioButton);
        maleRadioButton->setObjectName(QString::fromUtf8("maleRadioButton"));
        maleRadioButton->setChecked(true);

        horizontalLayout_2->addWidget(maleRadioButton);

        femaleRadioButton = new QRadioButton(AddPersonDialog);
        buttonGroup->addButton(femaleRadioButton);
        femaleRadioButton->setObjectName(QString::fromUtf8("femaleRadioButton"));

        horizontalLayout_2->addWidget(femaleRadioButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(AddPersonDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(71, 0));

        horizontalLayout_3->addWidget(label_4);

        ageSpinBox = new QSpinBox(AddPersonDialog);
        ageSpinBox->setObjectName(QString::fromUtf8("ageSpinBox"));

        horizontalLayout_3->addWidget(ageSpinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(AddPersonDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(71, 0));

        horizontalLayout_4->addWidget(label_5);

        departLineEdit = new QLineEdit(AddPersonDialog);
        departLineEdit->setObjectName(QString::fromUtf8("departLineEdit"));

        horizontalLayout_4->addWidget(departLineEdit);

        label_7 = new QLabel(AddPersonDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(71, 0));

        horizontalLayout_4->addWidget(label_7);

        teamLineEdit = new QLineEdit(AddPersonDialog);
        teamLineEdit->setObjectName(QString::fromUtf8("teamLineEdit"));

        horizontalLayout_4->addWidget(teamLineEdit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_8 = new QLabel(AddPersonDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(71, 0));

        verticalLayout->addWidget(label_8);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_5->addLayout(verticalLayout);

        descriptionText = new QPlainTextEdit(AddPersonDialog);
        descriptionText->setObjectName(QString::fromUtf8("descriptionText"));

        horizontalLayout_5->addWidget(descriptionText);


        verticalLayout_2->addLayout(horizontalLayout_5);

        buttonBox = new QDialogButtonBox(AddPersonDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(AddPersonDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddPersonDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddPersonDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AddPersonDialog);
    } // setupUi

    void retranslateUi(QDialog *AddPersonDialog)
    {
        AddPersonDialog->setWindowTitle(QApplication::translate("AddPersonDialog", "Add New Person", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("AddPersonDialog", "Information About the New Employee", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddPersonDialog", "ID:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddPersonDialog", "Name:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddPersonDialog", "Gender:", 0, QApplication::UnicodeUTF8));
        maleRadioButton->setText(QApplication::translate("AddPersonDialog", "Male", 0, QApplication::UnicodeUTF8));
        femaleRadioButton->setText(QApplication::translate("AddPersonDialog", "Female", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AddPersonDialog", "Age:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AddPersonDialog", "Department:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("AddPersonDialog", "Team:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("AddPersonDialog", "Description:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddPersonDialog: public Ui_AddPersonDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDPERSONDIALOG_H
