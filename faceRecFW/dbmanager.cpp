#include <QDebug>
#include <QByteArray>

#include "dbmanager.h"

QSqlDatabase DbManager::db_connection = QSqlDatabase::addDatabase("QSQLITE");

DbManager::DbManager()
{
    dbconnected = false;
}

bool DbManager::db_connect()
{
    if (dbconnected) {
        return true;
    }

    db_connection.setDatabaseName("./faceRecFW.db");
    if (!db_connection.open()) {
        std::cerr << "cannot open database \"faceRecFW.db\"" << std::endl;
        return false;
    }

    dbconnected = true;

    return true;
}

void DbManager::db_disconnect()
{
    db_connection.close();
    dbconnected = false;
}

bool DbManager::create_table()
{
    QString table_person_info;
    QString table_image_path_info;
    QString table_gabor_feature_info;
    QString table_mapping_matrix_info;

    if (!dbconnected) return false;

    table_person_info = QString("CREATE TABLE person_info ("
                                "   person_id        INTEGER PRIMARY KEY,"
                                "   employee_id      TEXT NOT NULL,"
                                "   employee_name    TEXT,"
                                "   employee_gender  INTEGER,"
                                "   employee_age     INTEGER,"
                                "   department       TEXT,"
                                "   team             TEXT,"
                                "   description      TEXT)");

    table_image_path_info = QString("CREATE TABLE image_path_info ("
                                    "   image_id          INTEGER PRIMARY KEY,"
                                    "   employee_id       TEXT NOT NULL,"
                                    "   path              TEXT NOT NULL)");

    table_gabor_feature_info = QString("CREATE TABLE gabor_feature_info ("
                                       "   feature_id     INTEGER PRIMARY KEY,"
                                       "   image_id       INTEGER NOT NULL,"
                                       "   gabor_feature  BLOB NOT NULL,"
                                       "   flda_feature   BOLB)");

    table_mapping_matrix_info = QString("CREATE TABLE mapping_matrix_info ("
                                        "   pca_mapping     BOLB NOT NULL,"
                                        "   flda_mapping    BOLB NOT NULL,"
                                        "   general_mapping BOLB NOT NULL,"
                                        "   feature_len     INTEGER NOT NULL,"
                                        "   mean_matrix     BOLB NOT NULL)");

    QSqlQuery q_table_person_info(table_person_info);
    QSqlQuery q_table_image_path_info(table_image_path_info);
    QSqlQuery q_table_gabor_feature_info(table_gabor_feature_info);
    QSqlQuery q_table_mapping_matrix_info(table_mapping_matrix_info);

    if (!q_table_person_info.isActive() ||
        !q_table_image_path_info.isActive() ||
        !q_table_gabor_feature_info.isActive() ||
        !q_table_mapping_matrix_info.isActive()) {

        std::cerr << "Create Table Error: " << std::endl;
        std::cerr << "For table person_info: " <<
                     q_table_person_info.lastError().text().toAscii().data()
                  << std::endl
                  << q_table_person_info.lastQuery().toAscii().data() << std::endl;
        std::cerr << "For table image_path_info: " <<
                     q_table_image_path_info.lastError().text().toAscii().data()
                  << std::endl
                  << q_table_image_path_info.lastQuery().toAscii().data() << std::endl;
        std::cerr << "For table gabor_feature_info: " <<
                     q_table_gabor_feature_info.lastError().text().toAscii().data()
                  << std::endl
                  << q_table_gabor_feature_info.lastQuery().toAscii().data() << std::endl;
        std::cerr << "For table mapping_matrix_info: " <<
                     q_table_mapping_matrix_info.lastError().text().toAscii().data()
                  << std::endl
                  << q_table_mapping_matrix_info.lastQuery().toAscii().data() << std::endl;
    }

    db_connection.commit();

    return true;
}

bool DbManager::load_general_mapping_matrix(cv::Mat &general_transfer, cv::Mat &mean_matrix)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("SELECT general_mapping, mean_matrix FROM mapping_matrix_info");

    QSqlQuery query(q_str);

    if (!query.next()) return false;

    QByteArray bolb_1 = query.value(0).toByteArray();
    QByteArray bolb_2 = query.value(1).toByteArray();

    double *matrix_data_1 = (double *)bolb_1.data();
    double *matrix_data_2 = (double *)bolb_2.data();

    int matrix_size_1 = bolb_1.size() / sizeof(double);
    int matrix_size_2 = bolb_2.size() / sizeof(double);

    if (matrix_size_1 != general_transfer.cols * general_transfer.rows ||
            matrix_size_2 != mean_matrix.cols * mean_matrix.rows) {
        std::cerr << "loading general matrix error" << std::endl;
        exit(EXIT_FAILURE);
    }

    int i = 0;

    for (int x = 0; x < general_transfer.cols; x++) {
        for (int y = 0; y < general_transfer.rows; y++) {
            general_transfer.at<double>(y, x) = matrix_data_1[i++];
        }
    }

    i = 0;

    for (int x = 0; x < mean_matrix.cols; x++) {
        for (int y = 0; y < mean_matrix.rows; y++) {
            mean_matrix.at<double>(y, x) = matrix_data_2[i++];
        }
    }

    return true;
}

bool DbManager::add_image_path_info(QString &employee_id, QString &path)
{
    if (!dbconnected) return false;

    QString q_str = QString("INSERT INTO image_path_info "
                            "(employee_id, path) "
                            "VALUES (:employee_id, :path)");

    QSqlQuery query;

    query.prepare(q_str);
    query.bindValue(":employee_id", employee_id);
    query.bindValue(":path", path);
    query.exec();

    return true;
}

bool DbManager::get_person_face(const QString &employee_id, QString &path)
{
    if (!dbconnected) return false;

    QString q_str = QString("SELECT path from image_path_info WHERE employee_id = '%1'")
            .arg(employee_id);

    QSqlQuery query(q_str);

    query.next();

    path = query.value(0).toString();

    return true;
}

bool DbManager::get_person_info(const QString &employee_id,
                                QString &employee_name,
                                bool &employee_gender,
                                int &employee_age,
                                QString &department,
                                QString &team,
                                QString &description)
{
    if (!dbconnected) return false;

    QString q_str = QString("SELECT employee_name, employee_gender, employee_age, "
                            "department, team, description FROM person_info WHERE empolyee_id = '%1'")
            .arg(employee_id);

    QSqlQuery query(q_str);

    employee_name = query.value(0).toString();
    employee_gender = (query.value(1).toInt() == 1) ? true : false;
    employee_age = query.value(2).toInt();
    department = query.value(3).toString();
    team = query.value(4).toString();
    description = query.value(5).toString();

    return true;
}

bool DbManager::add_person_info(QString &employee_id,
                                QString &employee_name,
                                bool     employee_gender,
                                int      employee_age,
                                QString &department,
                                QString &team,
                                QString &description)
{
    if (!dbconnected) return false;

    if (has_employee_id(employee_id)) {
        printf("employee id already existed.\n");
        return false;
    }

    QString q_str = QString("INSERT INTO person_info (employee_id, "
                            "employee_name, employee_gender, employee_age, department, team, description) "
                            "VALUES (:employee_id, :employee_name, :employee_gender, :employee_age, :department, :team, :description)");

    QSqlQuery query;

    query.prepare(q_str);
    query.bindValue(":employee_id", employee_id);
    query.bindValue(":employee_name", employee_name);
    query.bindValue(":employee_gender", employee_gender ? 1 : 0);
    query.bindValue(":employee_age", employee_age);
    query.bindValue(":department", department);
    query.bindValue(":team", team);
    query.bindValue(":description", description);
    query.exec();

    return true;
}

bool DbManager::has_mapping_matrix()
{
    if (!dbconnected) return false;

    QString q_str = QString("SELECT * FROM mapping_matrix_info");

    QSqlQuery query(q_str);

    if (query.next())
        return true;
    else return false;
}

bool DbManager::has_feature_data(int image_id)
{
    if (!dbconnected) return false;

    QString q_str = QString("SELECT image_id FROM gabor_feature_info WHERE image_id = %1")
            .arg(image_id);

    QSqlQuery query(q_str);

    if (query.next())
        return true;
    else
        return false;
}

bool DbManager::has_employee_id(QString &employee_id)
{
    if (!dbconnected) return false;

    QString q_str = QString("SELECT employee_id FROM person_info WHERE employee_id = '%1'")
            .arg(employee_id);

    QSqlQuery query(q_str);

    /* use QSqlQuery::first() here, since QSqlQuery::size() seems not */
    /* supported in QSQLITE */
    if (query.first())
        return true;
    else
        return false;
}

bool DbManager::get_feature_list(SubClass &sub_class, int feature_len)
{
    if (!dbconnected) return false;

    std::vector<int> image_id_list;
    int flda_feature_len = feature_len;

    QString q_str = QString("SELECT image_id FROM image_path_info WHERE employee_id = '%1'")
            .arg(sub_class.employee_id);

    QSqlQuery query(q_str);

    while (query.next()) {
        image_id_list.push_back(query.value(0).toInt());
    }

    for (size_t i = 0; i < image_id_list.size(); i++) {
        int image_id = image_id_list[i];
        cv::Mat load_matrix;
        load_matrix.create(flda_feature_len, 1, CV_64FC1);
        load_flda_feature_from_bolb(load_matrix, image_id);
        sub_class.feature_list.push_back(load_matrix);
    }

    assert(image_id_list.size() == 10);

    return true;
}

bool DbManager::load_feature_vectors(std::vector<SubClass> &subclass_content,
                                     int feature_len)
{
    if (!dbconnected) return false;

    if (!subclass_content.empty())
        subclass_content.clear();

    QString q_str = QString("SELECT employee_id FROM person_info");
    QSqlQuery query(q_str);

    while (query.next()) {
        SubClass sub_class;
        sub_class.employee_id = query.value(0).toString();
        get_feature_list(sub_class, feature_len);
        if (sub_class.feature_list.size() != 10) {
            std::cout << "feature number for one person is not ten." << std::endl;
            exit(EXIT_FAILURE);
        }

        subclass_content.push_back(sub_class);
    }

    return true;
}

bool DbManager::load_flda_feature_from_bolb(cv::Mat &load_matrix, int image_id)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("SELECT flda_feature FROM gabor_feature_info WHERE image_id = %1")
                .arg(image_id);

    QSqlQuery query(q_str);
    query.next();

    QByteArray bolb = query.value(0).toByteArray();
    double *matrix_data = (double *)bolb.data();
    int matrix_size = bolb.size() / sizeof(double);

    if (matrix_size != load_matrix.cols * load_matrix.rows) {
        return false;
    }

    int i = 0;

    for (int x = 0; x < load_matrix.cols; x++) {
        for (int y = 0; y < load_matrix.rows; y++) {
            load_matrix.at<double>(y, x) = matrix_data[i++];
        }
    }

    return true;
}

bool DbManager::load_feature_len(int &feature_len)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("SELECT feature_len FROM mapping_matrix_info");

    QSqlQuery query(q_str);
    query.next();

    feature_len = query.value(0).toInt();
    return true;
}

bool DbManager::save_mapping_matrix(cv::Mat &pca_transfer,
                                    cv::Mat &flda_transfer,
                                    cv::Mat &general_transfer,
                                    int     flda_feature_len,
                                    cv::Mat &mean_matrix)
{
    if (!dbconnected) return false;

    QString q_str;

    if (has_mapping_matrix()) {
        q_str = QString("UPDATE mapping_matrix_info SET "
                        "pca_mapping = :pca, flda_mapping = :flda, general_mapping = :general, "
                        "feature_len = :feature_len, mean_matrix = :mean_matrix");
    } else {
        q_str = QString("INSERT INTO mapping_matrix_info ("
                        "pca_mapping, flda_mapping, general_mapping, feature_len, mean_matrix) "
                        "VALUES(:pca, :flda, :general, :feature_len, :mean_matrix)");
    }

    int size_pca = sizeof(double) * pca_transfer.cols * pca_transfer.rows;
    int size_flda = sizeof(double) * flda_transfer.cols * flda_transfer.rows;
    int size_general = sizeof(double) * general_transfer.cols * general_transfer.rows;
    int size_mean = sizeof(double) * mean_matrix.cols * mean_matrix.rows;

    QByteArray pca = QByteArray((char *)pca_transfer.data, size_pca);
    QByteArray flda = QByteArray((char *)flda_transfer.data, size_flda);
    QByteArray general = QByteArray((char *)general_transfer.data, size_general);
    QByteArray mean = QByteArray((char *)mean_matrix.data, size_mean);

    QSqlQuery query;
    query.prepare(q_str);
    query.bindValue(":pca", pca);
    query.bindValue(":flda", flda);
    query.bindValue(":general", general);
    query.bindValue(":feature_len", flda_feature_len);
    query.bindValue(":mean_matrix", mean);
    query.exec();

    return true;
}

bool DbManager::save_flda_feature_to_bolb(cv::Mat &save_matrix, int image_id)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("UPDATE gabor_feature_info SET flda_feature = :bolb "
                    "WHERE image_id = :image_id");

    int size = sizeof(double) * save_matrix.cols * save_matrix.rows;
    QByteArray bolb = QByteArray((char *)save_matrix.data, size);

    QSqlQuery query;
    query.prepare(q_str);
    query.bindValue(":bolb", bolb);
    query.bindValue(":image_id", image_id);
    query.exec();

    return true;
}

bool DbManager::save_gabor_feature_to_blob(cv::Mat &save_matrix, int image_id)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("INSERT INTO gabor_feature_info (image_id, gabor_feature) "
                    "VALUES(:image_id, :gabor_feature)");

    int size = sizeof(double) * save_matrix.cols * save_matrix.rows;
    QByteArray bolb = QByteArray((char *)save_matrix.data, size);

    QSqlQuery query;
    query.prepare(q_str);
    query.bindValue(":image_id", image_id);
    query.bindValue(":gabor_feature", bolb);
    query.exec();

    return true;
}

bool DbManager::load_gabor_feature_from_blob(cv::Mat &load_matrix, int image_id)
{
    if (!dbconnected) return false;

    QString q_str =
            QString("SELECT gabor_feature FROM gabor_feature_info WHERE image_id = %1")
                .arg(image_id);

    QSqlQuery query(q_str);
    query.next();

    QByteArray bolb = query.value(0).toByteArray();
    double *matrix_data = (double *)bolb.data();
    int matrix_size = bolb.size() / sizeof(double);

    if (matrix_size != load_matrix.cols * load_matrix.rows) {
        return false;
    }

    int i = 0;

    for (int x = 0; x < load_matrix.cols; x++) {
        for (int y = 0; y < load_matrix.rows; y++) {
            load_matrix.at<double>(y, x) = matrix_data[i++];
        }
    }

    return true;
}

bool DbManager::get_image_content_list(std::vector<ImageContent> &image_content_list)
{
    if (!dbconnected) return false;

    QString q_str("SELECT image_id, path FROM image_path_info ORDER BY employee_id");

    QSqlQuery query(q_str);

    if (!query.isActive()) {
        std::cerr << "Get Image Content List Error: "
                  << query.lastError().text().toAscii().data()
                  << std::endl
                  << query.lastQuery().toAscii().data() << std::endl;
        return false;
    }

    while (query.next()) {
        ImageContent image_content;
        image_content.image_id = query.value(0).toInt();
        image_content.image_path = query.value(1).toString();
        image_content_list.push_back(image_content);
    }

    return true;
}

int DbManager::get_person_info_count()
{
    if (!dbconnected) return 0;

    QString q_str("SELECT COUNT(*) FROM person_info");

    QSqlQuery q(q_str);

    if (!q.isActive()) {
        qDebug() << "Get Person Info Count Error: " << q.lastError().text()
                 << q.lastQuery();
        return 0;
    }

    if (!q.first()) return 0;

    db_record = q.record();

    QSqlField field = db_record.field(0);

    QVariant value = field.value();

    return value.toInt();
}
