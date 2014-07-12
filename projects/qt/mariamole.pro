#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T08:46:32
#
#-------------------------------------------------

QT       += core gui xml widgets qml serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



CONFIG(debug, debug|release) {
        UI_DIR = ../../temp/debug
        DESTDIR = ../../build/debug
        TARGET = mariamoled
        OBJECTS_DIR = ../../temp/debug
        MOC_DIR = ../../temp/debug
        QMAKE_CXXFLAGS_WARN_OFF += -g
        QMAKE_CFLAGS += -g
        QMAKE_CXXFLAGS_WARN_OFF -= -O3
        QMAKE_CFLAGS -= -O3
}

CONFIG(release, debug|release) {
        UI_DIR = ../../temp/debug
        DESTDIR = ../../build/debug
        TARGET = mariamole
        OBJECTS_DIR = ../../temp/debug
        MOC_DIR = ../../temp/debug
}

TARGET = mariamole
TEMPLATE = app

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CFLAGS -= -Wno-unused-parameter


SOURCES += \
    ../../src/about.cpp \
    ../../src/build_message_item.cpp \
    ../../src/builder.cpp \
    ../../src/buildwindow.cpp \
    ../../src/burnbootloader.cpp \
    ../../src/config.cpp \
    ../../src/editor.cpp \
    ../../src/editortab.cpp \
    ../../src/getuserstring.cpp \
    ../../src/launcher.cpp \
    ../../src/main.cpp \
    ../../src/mainwindow.cpp \
    ../../src/message_handler.cpp \
    ../../src/mm_utils.cpp \
    ../../src/preferences.cpp \
    ../../src/project.cpp \
    ../../src/qserialselector.cpp \
    ../../src/projectproperties.cpp \
    ../../src/serialmonitor.cpp \
    ../../src/setworkspace.cpp \
    ../../src/wizard.cpp \
    ../../src/workspace.cpp

HEADERS  += \
    ../../src/about.h \
    ../../src/build_message_item.h \
    ../../src/builder.h \
    ../../src/buildwindow.h \
    ../../src/burnbootloader.h \
    ../../src/config.h \
    ../../src/editor.h \
    ../../src/editortab.h \
    ../../src/getuserstring.h \
    ../../src/launcher.h \
    ../../src/mainwindow.h \
    ../../src/message_handler.h \
    ../../src/mm_utils.h \
    ../../src/preferences.h \
    ../../src/project.h \
    ../../src/qserialselector.h \
    ../../src/projectproperties.h \
    ../../src/serialmonitor.h \
    ../../src/setworkspace.h \
    ../../src/wizard.h \
    ../../src/workspace.h

FORMS    += \
    ../../src/about.ui \
    ../../src/buildwindow.ui \
    ../../src/burnbootloader.ui \
    ../../src/getuserstring.ui \
    ../../src/mainwindow.ui \
    ../../src/preferences.ui \
    ../../src/qserialselector.ui \
    ../../src/projectproperties.ui \
    ../../src/serialmonitor.ui \
    ../../src/setworkspace.ui \
    ../../src/wizard.ui

RESOURCES += \
    ../../src/mainwindow.qrc

OTHER_FILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libs/qscintilla/2.8.2/Qt4Qt5/release/ -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libs/qscintilla/2.8.2/Qt4Qt5/debug/ -lqscintilla2
else:unix: LIBS += -L$$PWD/../../libs/qscintilla/2.8.2/Qt4Qt5/ -lqscintilla2

INCLUDEPATH += $$PWD/../../libs/qscintilla/2.8.2/Qt4Qt5 \
    ../../src
DEPENDPATH += $$PWD/../../libs/qscintilla/2.8.2/Qt4Qt5
