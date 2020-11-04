#include <QDir>
#include <QTime>
#include <QIcon>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "addpersondialog.h"
#include "trainingdialog.h"
#include "configdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* default background load-ups */
    /* (!--) direct QImage.load() will lead to a crash, QImage don't work well */
    /* with OpenCV? */
    /* reminder: it seems that it is libpng something is the cause of this conflict */
    cv::Mat mat_webcam_frame = read_image("./resources/webcam_frame_default.jpg");
    cv::Mat mat_norm_face = read_image("./resources/logo.jpg");
    cv::Mat mat_recognition_face = read_image("./resources/recognition_face_default.jpg");
    cv::Mat mat_no_face = read_image("./resources/no_face.jpg");;

    storage_dir = QString("./face_image_storage");

    webcam_frame_default = mat2qimage(mat_webcam_frame);
    norm_face_default = mat2qimage(mat_norm_face);
    recognition_face_default = mat2qimage(mat_recognition_face);
    no_face_default = mat2qimage(mat_no_face);

    faceGatherCount = 0;
    samplesPerClass = SAMPLE_PER_CLASS;

    threshold = 0.8;

    /* Timer for face gathering */
    face_gather_timer = new QTimer(this);

    /* Timer for face recognition */
    face_recognition_timer = new QTimer(this);

    ui->listWidget->setIconSize(QSize(50, 50));

    /* state machine starts with state_undefined */
    state = state_undefined;
    gui_state_update();

    /* at first, it is ok to exit */
    exit_flag = true;

    /* local bindings */
    createConnections();

    /* create database if it hasn't been built */
    QFileInfo fileInfo = QFileInfo(QDir::current(), QString("faceRecFW.db"));
    if (!fileInfo.exists()) {
        db_manager.db_connect();
        db_manager.create_table();
        db_manager.db_disconnect();
    }

    /* at first start up, recognition is not allowed */
    recognition_flag = true;

    /* system start-up frame */
    recognitionServer.system_startup();

    /* system start */
    this->showMaximized();
    statusBar()->showMessage(tr("System Ready"), 2000);
    setWindowTitle(tr("faceRecFW"));
}

MainWindow::~MainWindow()
{
    delete webcam_frame_default.bits();
    delete norm_face_default.bits();
    delete recognition_face_default.bits();
    delete no_face_default.bits();
    delete ui;
}

void MainWindow::controlRecognition()
{
    if (state == state_detection && videoStreamReader.isRunning())
    {
        state = state_recognition;
        face_recognition_timer->start(2000);
    } else {
        face_recognition_timer->stop();
        recognition_flag = true;
        state = state_detection;
    }
    gui_state_update();
}

void MainWindow::controlVideo()
{
    if (state == state_undefined && !videoStreamReader.isRunning())
    {
        videoStreamReader.startRecord();
        state = state_detection;
    } else {
        videoStreamReader.stopRecord();
        state = state_undefined;
        /* videoStreamThread.wait(); */
    }
    gui_state_update();
}

void MainWindow::about()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}

void MainWindow::showResult()
{
    cv::Mat face_image;
    QImage face_qimage;
    QString face_image_path;
    double threshold_result;
    std::vector<ResultRecord> result_set;

    threshold_result = recognitionServer.get_threshold();
    recognitionServer.get_recognition_result(result_set);
    QString employee_id = result_set[0].employee_id;
    db_manager.db_connect();

    ui->listWidget->clear();

    if (threshold_result < threshold) {
        ui->recPersonLabel->setPixmap(QPixmap::fromImage(no_face_default));
        recognition_result_clear();
    } else {
        QString employee_name;
        QString department;
        QString team;
        QString description;

        bool employee_gender;
        int employee_age;

        /* show recognition result on GUI */
        db_manager.get_person_info(employee_id,
                                   employee_name,
                                   employee_gender,
                                   employee_age,
                                   department, team, description);

        db_manager.get_person_face(employee_id,
                                   face_image_path);

        face_image = read_image(face_image_path.toAscii().data());
        face_qimage = mat2qimage(face_image);
        ui->recPersonLabel->setPixmap(QPixmap::fromImage(face_qimage));
        delete face_qimage.bits();

        ui->idLineEdit->setText(employee_id);
        ui->nameLineEdit->setText(employee_name);
        ui->genderLineEdit->setText(employee_gender ?
                                        tr("Male") : tr("Female"));
        ui->ageLineEdit->setText(tr("%1").arg(employee_age));
        ui->teamLineEdit->setText(team);
        ui->departLineEdit->setText(department);
        ui->descPlainTextEdit->appendPlainText(description);

        update();
       /* end of show recognition result */
    }

    for (size_t i = 0; i < result_set.size(); i++) {
        QString id = result_set[i].employee_id;
        double distance = result_set[i].distance;
        double rel = result_set[i].rel;

        db_manager.get_person_face(id,
                                   face_image_path);

        face_image = read_image(face_image_path.toAscii().data());
        face_qimage = mat2qimage(face_image);

        faceItem = new QListWidgetItem(QIcon(QPixmap::fromImage(face_qimage)),
                                       tr("distance = %1, rel = %2").arg(distance)
                                       .arg(rel));
        delete face_qimage.bits();

        ui->listWidget->addItem(faceItem);
        update();
    }

    db_manager.db_disconnect();

    /* at last, enable recognition again by setting the flag */
    recognition_flag = true;
}

void MainWindow::recognition_result_clear()
{
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->genderLineEdit->clear();
    ui->ageLineEdit->clear();
    ui->departLineEdit->clear();
    ui->teamLineEdit->clear();
    ui->descPlainTextEdit->clear();

    update();
}

void MainWindow::updateFrames()
{
    if (videoStreamReader.running_flag())
    {
        cv::Mat webcamFrame = videoStreamReader.getFrame();

        if (videoStreamReader.getHasNewNormFace()) {
            cv::Mat normFrame = videoStreamReader.getNormFace();
            QImage normImage = mat2qimage(normFrame);
            ui->normFacelabel->setPixmap(QPixmap::fromImage(normImage));
            delete normImage.bits();
        }

        QImage webcamImage = mat2qimage(webcamFrame);
        ui->webcamLabel->setPixmap(QPixmap::fromImage(webcamImage));
        delete webcamImage.bits();
        update();
    }

    videoStreamReader.transferCompleted();
}

void MainWindow::addNewPerson()
{
    AddPersonDialog addPersonDialog;

    if (addPersonDialog.exec() && db_manager.db_connect()) {
        QString Id = addPersonDialog.getId();
        QString Name = addPersonDialog.getName();
        bool Gender = addPersonDialog.getGenger();
        int Age = addPersonDialog.getAge();
        QString Department = addPersonDialog.getDepartment();
        QString Team = addPersonDialog.getTeam();
        QString Description = addPersonDialog.getDescription();

        if(db_manager.add_person_info(Id, Name, Gender,
                                      Age, Department, Team, Description)) {
            /* start to take samples */
            employee_id = Id;
            startGathering();
        } else {
            QMessageBox::warning(this, tr("Warning..."), tr("person already in the database!"),
                                 QMessageBox::Ok);
            db_manager.db_disconnect();
        }
    }
}

void MainWindow::faceRecognition()
{
    if (recognition_flag && videoStreamReader.getHasNewNormFace()) {
        recognition_flag = false;
        cv::Mat normFrame = videoStreamReader.getNormFace();
        recognitionServer.set_norm_face(normFrame);
        recognitionServer.start();
    }
}

void MainWindow::faceGathering()
{
    if (videoStreamReader.getHasNewNormFace()) {

        QTime time = QTime::currentTime();
        QString filename = QString(image_path + QString("/%1_%2.jpg").arg(faceGatherCount)
                                   .arg(time.toString(QString("hh:mm:ss.zzz"))));

        cv::Mat norm_face = videoStreamReader.getNormFace();
        QImage norm_face_q = mat2qimage(norm_face);

        write_image(filename.toAscii().data(),
                    norm_face);

        faceGatherCount++;
        ui->progressBar->setValue(faceGatherCount);

        faceItem = new QListWidgetItem(QIcon(QPixmap::fromImage(norm_face_q)),
                                       filename);
        delete norm_face_q.bits();

        ui->listWidget->addItem(faceItem);

        /* store image info into database */
        db_manager.add_image_path_info(employee_id, filename);

        if (faceGatherCount == samplesPerClass) {
            faceGatherCount = 0;
            face_gather_timer->stop();

            state = state_detection;
            gui_state_update();
            db_manager.db_disconnect();
        }
    }
}

void MainWindow::startGathering()
{
    QDir dirInfo = QDir(storage_dir);
    dirInfo.mkdir(employee_id);
    image_path = storage_dir + QString("/") + employee_id;

    state = state_recording;
    gui_state_update();

    ui->listWidget->clear();
    face_gather_timer->start(2000);
}

void MainWindow::startTraining()
{
    TrainingDialog trainDialog;
    exit_flag = false;

    ui->action_Start_Recognition->setEnabled(false);

    trainDialog.exec();

    if (trainDialog.get_reloading_flag()) {
        recognitionServer.reloading_data();
    }

    ui->action_Start_Recognition->setEnabled(true);
    exit_flag = true;
}

void MainWindow::configuration()
{
    ConfigDialog configDialog(threshold);
    exit_flag = false;
    configDialog.exec();

    QString new_device = configDialog.get_device_name();

    threshold = configDialog.get_threshold();

    videoStreamReader.setWebCamDevice(new_device.toStdString());

    exit_flag = true;
}

void MainWindow::createConnections()
{
    /* GUI action bindings */
    connect(ui->action_Start_WebCam, SIGNAL(triggered()), this, SLOT(controlVideo()));
    connect(ui->action_Start_Recognition, SIGNAL(triggered()), this, SLOT(controlRecognition()));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_About_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_About_faceRecFW, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->action_Add_Person, SIGNAL(triggered()), this, SLOT(addNewPerson()));
    connect(ui->action_Training, SIGNAL(triggered()), this, SLOT(startTraining()));
    connect(ui->action_Configuration, SIGNAL(triggered()), this, SLOT(configuration()));

    /* video stream and mainwindow GUI update bindings */
    connect(&videoStreamReader, SIGNAL(transferStart()), this, SLOT(updateFrames()));

    /* recognition server update with mainwindow GUI bindings */
    connect(&recognitionServer, SIGNAL(updateGUI(int)), this, SLOT(updateProgressBar(int)));
    connect(&recognitionServer, SIGNAL(updateRange(int,int)), this, SLOT(updateRange(int,int)));
    connect(&recognitionServer, SIGNAL(recognitionComplete()), this, SLOT(showResult()));

    /* timeout triggers */
    connect(face_gather_timer, SIGNAL(timeout()), this, SLOT(faceGathering()));
    connect(face_recognition_timer, SIGNAL(timeout()), this, SLOT(faceRecognition()));
}

void MainWindow::gui_state_update()
{
    switch (state) {
    /*                          state_recording                          */
    /*                                 /\                                */
    /*                                 ||                                */
    /*                                 \/                                */
    /* state_undefined <------> state_detaction                          */
    /*                                 /\                                */
    /*                                 ||                                */
    /*                                 \/                                */
    /*                          state_recognition                        */
        case state_undefined:
            ui->stateLabel->setText(tr("Nothing in Progress"));
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(false);
            ui->action_Start_WebCam->setEnabled(true);
            ui->action_Start_WebCam->setText(tr("&Start WebCam"));
            ui->action_Start_WebCam->setStatusTip(tr("Start the webcam"));
            ui->action_Start_Recognition->setEnabled(false);
            ui->action_Exit->setEnabled(true);
            exit_flag = true;
            ui->action_Add_Person->setEnabled(false);
            ui->action_Training->setEnabled(true);
            ui->action_Configuration->setEnabled(true);
            ui->webcamLabel->setPixmap(QPixmap::fromImage(webcam_frame_default));
            ui->normFacelabel->setPixmap(QPixmap::fromImage(norm_face_default));
            ui->recPersonLabel->setPixmap(QPixmap::fromImage(recognition_face_default));
            break;
        case state_detection:
            ui->stateLabel->setText(tr("Face Detection"));
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(false);
            ui->action_Start_WebCam->setEnabled(true);
            ui->action_Start_WebCam->setText(tr("&Stop WebCam"));
            ui->action_Start_WebCam->setStatusTip(tr("Stop the webcam"));
            ui->action_Start_Recognition->setEnabled(true);
            ui->action_Start_Recognition->setText(tr("Start &Recognition"));
            ui->action_Exit->setEnabled(true);
            exit_flag = true;
            ui->action_Add_Person->setEnabled(true);
            ui->action_Training->setEnabled(true);
            ui->action_Configuration->setEnabled(false);
            ui->recPersonLabel->setPixmap(QPixmap::fromImage(recognition_face_default));
            break;
        case state_recording:
            ui->stateLabel->setText(tr("Face Recording"));
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(true);
            ui->progressBar->setRange(0, 10);
            ui->action_Start_WebCam->setEnabled(false);
            ui->action_Start_Recognition->setEnabled(false);
            ui->action_Exit->setEnabled(false);
            exit_flag = false;
            ui->action_Add_Person->setEnabled(false);
            ui->action_Training->setEnabled(false);
            ui->action_Configuration->setEnabled(false);
            ui->recPersonLabel->setPixmap(QPixmap::fromImage(recognition_face_default));
            break;
        case state_recognition:
            ui->stateLabel->setText(tr("Face Recognition"));
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(true);
            ui->action_Start_WebCam->setEnabled(false);
            ui->action_Start_Recognition->setEnabled(true);
            ui->action_Start_Recognition->setText(tr("Stop &Recognition"));
            ui->action_Exit->setEnabled(false);
            exit_flag = false;
            ui->action_Add_Person->setEnabled(false);
            ui->action_Training->setEnabled(false);
            ui->action_Configuration->setEnabled(false);
            ui->recPersonLabel->setPixmap(QPixmap::fromImage(recognition_face_default));
            break;
    }
    update();
}

/* in some states, it is not right to just exit, */
/* so, add exit_flag to control it */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (exit_flag) {
        videoStreamReader.stopRecord();
        event->accept();
    } else {
        event->ignore();
    }
}

/* GUI updates from RecognitionServer */
void MainWindow::updateProgressBar(int value)
{
    ui->progressBar->setValue(value);
    update();
}

void MainWindow::updateRange(int min, int max)
{
    ui->progressBar->setRange(min, max);
}
