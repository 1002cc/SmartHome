QT       += core gui sql multimedia multimediawidgets xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

# set icon
RC_ICONS = icon.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin.cpp \
    camera.cpp \
    control.cpp \
    controlwidget.cpp \
    equipmentcontrol.cpp \
    face.cpp \
    faceidentify.cpp \
    main.cpp \
    land.cpp \
    music.cpp \
    photo.cpp \
    sql.cpp \
    switchcontrol.cpp

HEADERS += \
    admin.h \
    camera.h \
    control.h \
    controlwidget.h \
    equipmentcontrol.h \
    face.h \
    faceidentify.h \
    image.h \
    land.h \
    music.h \
    photo.h \
    sql.h \
    switchcontrol.h

FORMS += \
    admin.ui \
    camera.ui \
    control.ui \
    controlwidget.ui \
    equipmentcontrol.ui \
    face.ui \
    land.ui \
    music.ui \
    photo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    face.qrc \
    image.qrc

INCLUDEPATH += $$PWD/install/include \
                $$PWD/install/include/opencv \
                $$PWD/install/include/opencv2

LIBS += $$PWD/install/lib/libopencv*.a
