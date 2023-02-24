QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qscintilla2

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    compilerprocess.cpp \
    main.cpp \
    mainwindow.cpp \
    gdbprogress.cpp \
    logdialog.cpp \
    stringhandler.cpp \
    texteditor.cpp \
    utils.cpp

HEADERS += \
    compilerprocess.h \
    logdialog.h \
    mainwindow.h \
    gdbprogress.h \
    mainwindow.h \
    stringhandler.h \
    texteditor.h \
    utils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    src_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
