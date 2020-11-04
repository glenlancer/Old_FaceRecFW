#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>
#include <QListWidgetItem>

#include "common.h"
#include "videostreamreader.h"
#include "recognitionserver.h"
#include "dbmanager.h"

enum state_machine {
    state_undefined,
    state_detection,
    state_recording,
    state_recognition
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void controlVideo();
    void controlRecognition();
    void updateFrames();
    void addNewPerson();
    void faceGathering();
    void faceRecognition();
    void startTraining();
    void configuration();
    void about();

    /* used to communicate with RecognitionServer */
    void updateProgressBar(int value);
    void updateRange(int min, int max);
    void showResult();

private:
    void gui_state_update();
    void createConnections();
    void startGathering();

    void recognition_result_clear();
    void show_sorted_result(std::vector<cv::Mat> &result_set);

    QImage webcam_frame_default;
    QImage norm_face_default;
    QImage recognition_face_default;
    QImage no_face_default;

    /* parameter used in Nearest Neighbor Classifier */
    double threshold;

    /* store each gathered face in QListWidget */
    QListWidgetItem *faceItem;

    /* timer for face gathering */
    QTimer *face_gather_timer;

    /* timer for face recognition */
    QTimer *face_recognition_timer;

    /* index to indicate gathering progress */
    int faceGatherCount;
    int samplesPerClass;

    /* application's entry dir */
    QString storage_dir;

    /* current employee_id and image path for storage */
    QString employee_id;
    QString image_path;

    /* local variables */
    /* GUI state machine flag */
    state_machine state;

    /* thread used to extract frame from camera and do detection job */
    VideoStreamReader videoStreamReader;

    /* thread used to do face recognition job */
    RecognitionServer recognitionServer;

    /* flag to enable exiting program */
    bool exit_flag;

    /* flag to decide whether do recognition job */
    bool recognition_flag;

    /* Ui for centeral widget */
    Ui::MainWindow *ui;

    /* Object to establish databse connections */
    DbManager db_manager;
};

#endif // MAINWINDOW_H
