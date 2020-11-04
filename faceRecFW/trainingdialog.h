#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QDialog>
#include <vector>

#include "common.h"
#include "trainingserver.h"

namespace Ui {
class TrainingDialog;
}

class TrainingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TrainingDialog(QWidget *parent = 0);
    ~TrainingDialog();

    bool get_reloading_flag();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void saveOutput();
    void startTraining();
    void updateGUI(QString output, int progress_value);
    void updateRange(int min, int max);
    void trainingCompleted();

private:
    void saveFile(QString &filename);

    Ui::TrainingDialog *ui;

    bool reloading_flag;

    bool exit_flag;

    TrainingServer trainingServer;
};

#endif // TRAININGDIALOG_H
