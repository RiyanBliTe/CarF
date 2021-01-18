QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authenticationform.cpp \
    bakform.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp \
    registrationform.cpp \
    userinfo.cpp

HEADERS += \
    authenticationform.h \
    bakform.h \
    item.h \
    mainwindow.h \
    registrationform.h \
    userinfo.h

FORMS += \
    authenticationform.ui \
    bakform.ui \
    item.ui \
    mainwindow.ui \
    registrationform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
