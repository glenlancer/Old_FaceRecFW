#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(double threshold, QWidget *parent = 0);
    ~ConfigDialog();
    
    QString get_device_name();

    double get_threshold();

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
