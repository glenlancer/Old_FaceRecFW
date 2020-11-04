#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>

#include "trainingdialog.h"
#include "ui_trainingdialog.h"

TrainingDialog::TrainingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainingDialog)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

    connect(ui->closePushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->savePushButton, SIGNAL(clicked()), this, SLOT(saveOutput()));
    connect(ui->trainingPushButton, SIGNAL(clicked()), this, SLOT(startTraining()));

    connect(&trainingServer, SIGNAL(updateGUI(QString, int)), this, SLOT(updateGUI(QString, int)));
    connect(&trainingServer, SIGNAL(updateRange(int,int)), this, SLOT(updateRange(int,int)));
    connect(&trainingServer, SIGNAL(trainingComplete()), this, SLOT(trainingCompleted()));

    setWindowTitle(tr("Training Dialog"));

    ui->progressBar->setValue(0);
    ui->savePushButton->setEnabled(false);

    exit_flag = true;
    reloading_flag = false;
}

TrainingDialog::~TrainingDialog()
{
    delete ui;
}

void TrainingDialog::startTraining()
{
    exit_flag = false;
    ui->trainingPushButton->setEnabled(false);
    ui->closePushButton->setEnabled(false);
    ui->savePushButton->setEnabled(false);

    trainingServer.start();
}

bool TrainingDialog::get_reloading_flag()
{
    return reloading_flag;
}

void TrainingDialog::updateRange(int min, int max)
{
    ui->progressBar->setRange(min, max);
}

void TrainingDialog::trainingCompleted()
{
    ui->trainingPushButton->setEnabled(true);
    ui->closePushButton->setEnabled(true);
    ui->savePushButton->setEnabled(true);

    exit_flag = true;
    reloading_flag = true;
}

void TrainingDialog::saveOutput()
{
    QString fileName = QFileDialog::getSaveFileName(this);

    if (QFile::exists(fileName)) {
        int r = QMessageBox::warning(this, tr("Saving Results"),
                        tr("File %1 already exists.\n"
                           "Do you want to overwrite it?")
                        .arg(QDir::toNativeSeparators(fileName)),
                        QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::No)
            return;
    }
    if (!fileName.isEmpty())
        saveFile(fileName);
}

void TrainingDialog::saveFile(QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Training Results"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));

    } else {
        QDataStream out(&file);
        out << ui->consoleTextEdit->toPlainText();
    }
    ui->savePushButton->setEnabled(false);
}

void TrainingDialog::updateGUI(QString output, int progress_value)
{
    ui->consoleTextEdit->appendPlainText(output);
    ui->progressBar->setValue(progress_value);
    update();
}

void TrainingDialog::closeEvent(QCloseEvent *event)
{
    if (exit_flag) {
        event->accept();
    } else
        event->ignore();
}
