#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    cv::Mat mat = read_image("./resources/logo.jpg");
    ui->logoLabel->setPixmap(QPixmap::fromImage(mat2qimage(mat)));
    update();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
