#ifndef RECOGNITIONSERVER_H
#define RECOGNITIONSERVER_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <vector>

#include "common.h"
#include "dbmanager.h"

class RecognitionServer : public QThread
{
    Q_OBJECT
public:
    explicit RecognitionServer(QObject *parent = 0);

    void system_startup();
    void reloading_data();

    void get_recognition_result(std::vector<ResultRecord> &result_set);
    double get_threshold();
    void set_norm_face(cv::Mat &face);

protected:
    void run();

signals:
    void recognitionComplete();
    void updateGUI(int progress_value);
    void updateRange(int min, int max);

private:
    double get_relevance(cv::Mat &mat_X, cv::Mat &mat_Y);
    double get_euclid_distance(cv::Mat &mat_X, cv::Mat &mat_Y);

    void min_per_subclass(SubClass &subclass);
    void sort_result();

    /* variables used for recognition */
    int sample_per_class;
    int gabor_feature_len;
    int flda_feature_len;       /* this length is read from database */
    cv::Mat mapping_matrix;
    cv::Mat mean_matrix;
    std::vector<SubClass> subclass_content;

    /* storage of recognition result */
    std::vector<ResultRecord> resultSet;

    DbManager db_manager;

    QMutex mutex;

    int index_max;
    int index_current;

    cv::Mat norm_face;

    double threshold;
};

#endif // RECOGNITIONSERVER_H
