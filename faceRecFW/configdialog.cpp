#include <QFile>

#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(double threshold, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    if (threshold < 0.0) threshold = 0.0;
    if (threshold > 1.0) threshold = 1.0;

    ui->thresDoubleSpinBox->setRange(0.0, 1.0);
    ui->thresDoubleSpinBox->setValue(threshold);

    for (int i = 0; i < 99; i++) {
        QString device = QString("/dev/video%1").arg(i);
        if (QFile::exists(device)) {
            ui->camCombBox->addItem(device);
        }
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

QString ConfigDialog::get_device_name()
{
    return ui->camCombBox->currentText();
}

double ConfigDialog::get_threshold()
{
    return ui->thresDoubleSpinBox->value();
}
