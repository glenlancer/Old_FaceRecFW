#include "addpersondialog.h"
#include "ui_addpersondialog.h"

AddPersonDialog::AddPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPersonDialog)
{
    ui->setupUi(this);

    ui->ageSpinBox->setRange(1, 100);
}

AddPersonDialog::~AddPersonDialog()
{
    delete ui;
}

QString AddPersonDialog::getId()
{
    return ui->iDLineEdit->text();
}

QString AddPersonDialog::getName()
{
    return ui->nameLineEdit->text().toUpper();
}

QString AddPersonDialog::getTeam()
{
    return ui->teamLineEdit->text();
}

QString AddPersonDialog::getDepartment()
{
    return ui->departLineEdit->text();
}

QString AddPersonDialog::getDescription()
{
    QString desc = ui->descriptionText->toPlainText();
    desc.truncate(MAX_DESCRIPTION_SIZE);
    return desc;
}

bool AddPersonDialog::getGenger()
{
    if (ui->maleRadioButton->isChecked()) {
        return true;
    } else {
        return false;
    }
}

int AddPersonDialog::getAge()
{
    return ui->ageSpinBox->value();
}
