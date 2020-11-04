#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QDialog>

#define MAX_DESCRIPTION_SIZE  2000

namespace Ui {
class AddPersonDialog;
}

class AddPersonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddPersonDialog(QWidget *parent = 0);
    ~AddPersonDialog();

    QString getId();
    QString getName();
    QString getDepartment();
    QString getTeam();
    QString getDescription();
    bool getGenger();
    int  getAge();

private:
    Ui::AddPersonDialog *ui;
};

#endif // ADDPERSONDIALOG_H
