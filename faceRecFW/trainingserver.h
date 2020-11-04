#ifndef TRAININGSERVER_H
#define TRAININGSERVER_H

#include <QThread>
#include <vector>

#include "common.h"
#include "dbmanager.h"

class TrainingServer : public QThread
{
    Q_OBJECT
public:
    explicit TrainingServer(QObject *parent = 0);
    
protected:
    void run();

signals:
    void trainingComplete();
    void updateGUI(QString output, int progress_value);
    void updateRange(int min, int max);

private:
    int sample_total_number;
    int sample_class_number;
    int sample_per_class;
    int gabor_feature_len;
    int image_len;

    int pca_feature_len_reference;

    DbManager db_manager;
};

#endif // TRAININGSERVER_H
