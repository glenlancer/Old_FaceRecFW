HEADERS += \
    v4l2_driver.h \
    mainwindow.h \
    common.h \
    videostreamreader.h \
    facedetector.h \
    aboutdialog.h \
    dbmanager.h \
    addpersondialog.h \
    pcautils.h \
    gaborfeature.h \
    fisherldautils.h \
    trainingserver.h \
    trainingdialog.h \
    recognitionserver.h \
    configdialog.h

SOURCES += \
    v4l2_driver.cpp \
    main.cpp \
    mainwindow.cpp \
    common.cpp \
    videostreamreader.cpp \
    facedetector.cpp \
    aboutdialog.cpp \
    dbmanager.cpp \
    addpersondialog.cpp \
    pcautils.cpp \
    fisherldautils.cpp \
    gaborfeature.cpp \
    trainingdialog.cpp \
    trainingserver.cpp \
    recognitionserver.cpp \
    configdialog.cpp

FORMS += \
    mainwindow.ui \
    aboutdialog.ui \
    addpersondialog.ui \
    trainingdialog.ui \
    configdialog.ui

RESOURCES += \
    resources.qrc

# OpenCV Library
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_objdetect \
-lopencv_highgui

# SQLite Interface
QT += sql

OTHER_FILES += \
    ringbuf.txt
