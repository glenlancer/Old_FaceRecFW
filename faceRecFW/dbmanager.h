#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include <vector>

#include "common.h"

class DbManager {
public:
    DbManager();

    bool db_connect();  // ok
    void db_disconnect(); // ok

    bool get_image_content_list(std::vector<ImageContent> &image_content_list); // ok

    bool load_feature_len(int &feature_len);

    bool load_gabor_feature_from_blob(cv::Mat &load_matrix, int image_id); // ok
    bool save_gabor_feature_to_blob(cv::Mat &save_matrix, int image_id);   // ok
    bool load_flda_feature_from_bolb(cv::Mat &load_matrix, int image_id);
    bool save_flda_feature_to_bolb(cv::Mat &save_matrix, int image_id);

    bool save_mapping_matrix(cv::Mat &pca_transfer,
                             cv::Mat &flda_transfer,
                             cv::Mat &general_transfer,
                             int     flda_feature_len,
                             cv::Mat &mean_matrix);

    bool load_general_mapping_matrix(cv::Mat &general_transfer,
                                     cv::Mat &mean_matrix);

    bool add_person_info(QString &employee_id,
                         QString &employee_name,
                         bool     employee_gender,
                         int      employee_age,
                         QString &department,
                         QString &team,
                         QString &description); // ok

    bool get_person_info(const QString &employee_id,
                         QString &employee_name,
                         bool    &employee_gender,
                         int     &employee_age,
                         QString &department,
                         QString &team,
                         QString &description);

    bool get_person_face(const QString &employee_id, QString &path);

    bool add_image_path_info(QString &employee_id, QString &path); // ok

    bool load_feature_vectors(std::vector<SubClass> &subclass_content,
                              int feature_len);

    int get_person_info_count(); // ok

    bool create_table(); // ok

    bool has_feature_data(int image_id); // ok
private:
    bool has_mapping_matrix();
    bool get_feature_list(SubClass &sub_class, int feature_len);

    bool has_employee_id(QString &employee_id); // ok
    bool                dbconnected;
    static QSqlDatabase db_connection;
    QSqlRecord          db_record;
};

#endif // DBMANAGER_H
