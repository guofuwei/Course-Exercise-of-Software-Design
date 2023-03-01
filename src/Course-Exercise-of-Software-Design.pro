QT       += core gui multimedia gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qscintilla2
RC_ICONS+=1.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    compilerprocess.cpp \
    audiorecord.cpp \
    main.cpp \
    mainwindow.cpp \
    gdbprogress.cpp \
    logdialog.cpp \
    mypaint.cpp \
    stringhandler.cpp \
    texteditor.cpp \
    timerwidget.cpp \
    utils.cpp

HEADERS += \
    compilerprocess.h \
    logdialog.h \
    audiorecord.h \
    mainwindow.h \
    gdbprogress.h \
    mainwindow.h \
    mypaint.h \
    stringhandler.h \
    texteditor.h \
    timerwidget.h \
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
